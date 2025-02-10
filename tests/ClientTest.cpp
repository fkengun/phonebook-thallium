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
#include <YP/PhonebookHandle.hpp>

TEST_CASE("Client test", "[client]") {

    auto engine = thallium::engine("na+sm", THALLIUM_SERVER_MODE);
    ENSURE(engine.finalize());
    // Initialize the provider
    const auto provider_config = R"(
    {
        "phonebook": {
            "type": "dummy",
            "config": {}
        }
    }
    )";

    YP::Provider provider(engine, 42, provider_config);

    SECTION("Open phonebook") {

        YP::Client client(engine);
        std::string addr = engine.self();

        YP::PhonebookHandle my_phonebook = client.makePhonebookHandle(addr, 42);
        REQUIRE(static_cast<bool>(my_phonebook));

        REQUIRE_THROWS_AS(client.makePhonebookHandle(addr, 55), YP::Exception);
        REQUIRE_NOTHROW(client.makePhonebookHandle(addr, 55, false));
    }
}
