/*
 * (C) 2020 The University of Chicago
 *
 * See COPYRIGHT in top-level directory.
 */
#include "YP/Provider.hpp"

#include "ProviderImpl.hpp"

#include <thallium/serialization/stl/string.hpp>

namespace tl = thallium;

namespace YP {

Provider::Provider(const tl::engine& engine, uint16_t provider_id, const std::string& config, const tl::pool& p)
: self(std::make_shared<ProviderImpl>(engine, provider_id, config, p)) {
    self->get_engine().push_finalize_callback(this, [p=this]() { p->self.reset(); });
}

Provider::Provider(Provider&& other) {
    other.self->get_engine().pop_finalize_callback(&other);
    self = std::move(other.self);
    self->get_engine().push_finalize_callback(this, [p=this]() { p->self.reset(); });
}

Provider::~Provider() {
    if(self) {
        self->get_engine().pop_finalize_callback(this);
    }
}

std::string Provider::getConfig() const {
    return self ? self->getConfig() : "{}";
}

Provider::operator bool() const {
    return static_cast<bool>(self);
}

}
