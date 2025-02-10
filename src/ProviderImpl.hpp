/*
 * (C) 2020 The University of Chicago
 *
 * See COPYRIGHT in top-level directory.
 */
#ifndef __YP_PROVIDER_IMPL_H
#define __YP_PROVIDER_IMPL_H

#include "YP/PhonebookInterface.hpp"

#include <thallium.hpp>
#include <thallium/serialization/stl/string.hpp>
#include <thallium/serialization/stl/vector.hpp>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <tuple>

namespace YP {

using namespace std::string_literals;
namespace tl = thallium;

class ProviderImpl : public tl::provider<ProviderImpl> {

    auto id() const { return get_provider_id(); } // for convenience

    using json = nlohmann::json;

    #define DEF_LOGGING_FUNCTION(__name__)                         \
    template<typename ... Args>                                    \
    void __name__(Args&&... args) {                                \
        auto msg = fmt::format(std::forward<Args>(args)...);       \
        spdlog::__name__("[YP:{}] {}", get_provider_id(), msg); \
    }

    DEF_LOGGING_FUNCTION(trace)
    DEF_LOGGING_FUNCTION(debug)
    DEF_LOGGING_FUNCTION(info)
    DEF_LOGGING_FUNCTION(warn)
    DEF_LOGGING_FUNCTION(error)
    DEF_LOGGING_FUNCTION(critical)

    #undef DEF_LOGGING_FUNCTION

    public:

    tl::engine           m_engine;
    tl::pool             m_pool;
    // Client RPC
    tl::auto_remote_procedure m_compute_sum;
    // FIXME: other RPCs go here ...
    // PhonebookInterfaces
    std::shared_ptr<PhonebookInterface> m_backend;

    ProviderImpl(const tl::engine& engine, uint16_t provider_id, const std::string& config, const tl::pool& pool)
    : tl::provider<ProviderImpl>(engine, provider_id, "YP")
    , m_engine(engine)
    , m_pool(pool)
    , m_compute_sum(define("YP_compute_sum",  &ProviderImpl::computeSumRPC, pool))
    {
        trace("Registered provider with id {}", get_provider_id());
        json json_config;
        try {
            json_config = json::parse(config);
        } catch(json::parse_error& e) {
            error("Could not parse provider configuration: {}", e.what());
            return;
        }
        if(!json_config.is_object()) return;
        if(!json_config.contains("phonebook")) return;
        auto& phonebook = json_config["phonebook"];
        if(!phonebook.is_object()) return;
        if(phonebook.contains("type") && phonebook["type"].is_string()) {
            auto& phonebook_type = phonebook["type"].get_ref<const std::string&>();
            auto phonebook_config = phonebook.contains("config") ? phonebook["config"] : json::object();
            auto result = createPhonebook(phonebook_type, phonebook_config);
            result.check();
        }
    }

    ~ProviderImpl() {
        trace("Deregistering provider");
        if(m_backend) {
            m_backend->destroy();
        }
    }

    std::string getConfig() const {
        auto config = json::object();
        if(m_backend) {
            config["phonebook"] = json::object();
            auto phonebook_config = json::object();
            phonebook_config["type"] = m_backend->name();
            phonebook_config["config"] = json::parse(m_backend->getConfig());
            config["phonebook"] = std::move(phonebook_config);
        }
        return config.dump();
    }

    Result<bool> createPhonebook(const std::string& phonebook_type,
                                const json& phonebook_config) {

        Result<bool> result;

        try {
            m_backend = PhonebookFactory::createPhonebook(phonebook_type, get_engine(), phonebook_config);
        } catch(const std::exception& ex) {
            result.success() = false;
            result.error() = ex.what();
            error("Error when creating phonebook of type {}: {}",
                  phonebook_type, result.error());
            return result;
        }

        if(not m_backend) {
            result.success() = false;
            result.error() = "Unknown phonebook type "s + phonebook_type;
            error("Unknown phonebook type {}", phonebook_type);
            return result;
        }

        trace("Successfully created phonebook of type {}", phonebook_type);
        return result;
    }

    void computeSumRPC(const tl::request& req,
                       int32_t x, int32_t y) {
        trace("Received computeSum request");
        Result<int32_t> result;
        tl::auto_respond<decltype(result)> response{req, result};
        if(!m_backend) {
            result.success() = false;
            result.error() = "Provider has no phonebook attached";
        } else {
            result = m_backend->computeSum(x, y);
        }
        trace("Successfully executed computeSum");
    }

};

}

#endif
