/*
 * (C) 2020 The University of Chicago
 *
 * See COPYRIGHT in top-level directory.
 */
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "Ensure.hpp"
#include <YP/Client.hpp>
#include <YP/Provider.hpp>

TEST_CASE("Phonebook test", "[phonebook]") {
    auto engine = thallium::engine("na+sm", THALLIUM_SERVER_MODE);
    ENSURE(engine.finalize());
    const auto provider_config = R"(
    {
        "phonebook": {
            "type": "dummy",
            "config": {}
        }
    }
    )";
    YP::Provider provider(engine, 42, provider_config);

    SECTION("Create PhonebookHandle") {
        YP::Client client(engine);
        std::string addr = engine.self();

        auto rh = client.makePhonebookHandle(addr, 42);

        SECTION("Send Sum RPC") {
            int32_t result;
            REQUIRE_NOTHROW([&]() { result = rh.computeSum(42, 51).wait(); }());
            REQUIRE(result == 93);
        }
    }
}
