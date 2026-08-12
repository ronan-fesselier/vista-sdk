#include <doctest/doctest.h>

#include <dnv/VistaSDK_c_api.h>

#include <string_view>

TEST_SUITE("c-api::ship_id")
{
    TEST_CASE("dnv_vista_sdk_ship_id_from_imo_number - construction and accessors")
    {
        auto* imo = dnv_vista_sdk_imo_number_from_string("9074729");
        REQUIRE(imo != nullptr);

        auto* shipId = dnv_vista_sdk_ship_id_from_imo_number(imo);
        REQUIRE(shipId != nullptr);

        CHECK(dnv_vista_sdk_ship_id_is_imo_number(shipId) == 1);
        CHECK(dnv_vista_sdk_ship_id_is_other_id(shipId) == 0);

        dnv_vista_sdk_ship_id_free(shipId);
        dnv_vista_sdk_imo_number_free(imo);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_from_imo_number - null returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_ship_id_from_imo_number(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_from_other_id - construction and accessors")
    {
        auto* shipId = dnv_vista_sdk_ship_id_from_other_id("VESSEL-XYZ-789");
        REQUIRE(shipId != nullptr);

        CHECK(dnv_vista_sdk_ship_id_is_other_id(shipId) == 1);
        CHECK(dnv_vista_sdk_ship_id_is_imo_number(shipId) == 0);

        dnv_vista_sdk_ship_id_free(shipId);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_from_other_id - empty string returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_ship_id_from_other_id("") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_from_other_id - null returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_ship_id_from_other_id(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_imo_number - returns an owned copy")
    {
        auto* imo = dnv_vista_sdk_imo_number_from_string("9074729");
        REQUIRE(imo != nullptr);
        auto* shipId = dnv_vista_sdk_ship_id_from_imo_number(imo);
        REQUIRE(shipId != nullptr);

        auto* retrieved = dnv_vista_sdk_ship_id_imo_number(shipId);
        REQUIRE(retrieved != nullptr);
        char* str = dnv_vista_sdk_imo_number_to_string(retrieved);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "IMO9074729");

        dnv_vista_sdk_imo_number_string_free(str);
        dnv_vista_sdk_imo_number_free(retrieved);
        dnv_vista_sdk_ship_id_free(shipId);
        dnv_vista_sdk_imo_number_free(imo);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_imo_number - other-id ShipId returns null")
    {
        auto* shipId = dnv_vista_sdk_ship_id_from_other_id("VESSEL-XYZ-789");
        REQUIRE(shipId != nullptr);

        CHECK(dnv_vista_sdk_ship_id_imo_number(shipId) == nullptr);

        dnv_vista_sdk_ship_id_free(shipId);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_other_id - returns the stored alternative identifier")
    {
        auto* shipId = dnv_vista_sdk_ship_id_from_other_id("VESSEL-XYZ-789");
        REQUIRE(shipId != nullptr);

        const auto* otherId = dnv_vista_sdk_ship_id_other_id(shipId);
        REQUIRE(otherId != nullptr);
        CHECK(std::string_view{ otherId } == "VESSEL-XYZ-789");

        dnv_vista_sdk_ship_id_free(shipId);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_other_id - imo-number ShipId returns null")
    {
        auto* imo = dnv_vista_sdk_imo_number_from_string("9074729");
        REQUIRE(imo != nullptr);
        auto* shipId = dnv_vista_sdk_ship_id_from_imo_number(imo);
        REQUIRE(shipId != nullptr);

        CHECK(dnv_vista_sdk_ship_id_other_id(shipId) == nullptr);

        dnv_vista_sdk_ship_id_free(shipId);
        dnv_vista_sdk_imo_number_free(imo);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_to_string - imo number includes IMO prefix")
    {
        auto* imo = dnv_vista_sdk_imo_number_from_string("9074729");
        REQUIRE(imo != nullptr);
        auto* shipId = dnv_vista_sdk_ship_id_from_imo_number(imo);
        REQUIRE(shipId != nullptr);

        char* str = dnv_vista_sdk_ship_id_to_string(shipId);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "IMO9074729");

        dnv_vista_sdk_ship_id_string_free(str);
        dnv_vista_sdk_ship_id_free(shipId);
        dnv_vista_sdk_imo_number_free(imo);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_to_string - alternative id round-trips")
    {
        auto* shipId = dnv_vista_sdk_ship_id_from_other_id("CUSTOM-SHIP-ID");
        REQUIRE(shipId != nullptr);

        char* str = dnv_vista_sdk_ship_id_to_string(shipId);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "CUSTOM-SHIP-ID");

        dnv_vista_sdk_ship_id_string_free(str);
        dnv_vista_sdk_ship_id_free(shipId);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_from_string - imo with prefix")
    {
        auto* shipId = dnv_vista_sdk_ship_id_from_string("IMO9074729");
        REQUIRE(shipId != nullptr);
        CHECK(dnv_vista_sdk_ship_id_is_imo_number(shipId) == 1);

        auto* imo = dnv_vista_sdk_ship_id_imo_number(shipId);
        REQUIRE(imo != nullptr);
        char* str = dnv_vista_sdk_imo_number_to_string(imo);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "IMO9074729");

        dnv_vista_sdk_imo_number_string_free(str);
        dnv_vista_sdk_imo_number_free(imo);
        dnv_vista_sdk_ship_id_free(shipId);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_from_string - imo with prefix, case insensitive")
    {
        auto* shipId1 = dnv_vista_sdk_ship_id_from_string("imo9074729");
        REQUIRE(shipId1 != nullptr);
        CHECK(dnv_vista_sdk_ship_id_is_imo_number(shipId1) == 1);

        auto* shipId2 = dnv_vista_sdk_ship_id_from_string("ImO9074729");
        REQUIRE(shipId2 != nullptr);
        CHECK(dnv_vista_sdk_ship_id_is_imo_number(shipId2) == 1);

        dnv_vista_sdk_ship_id_free(shipId1);
        dnv_vista_sdk_ship_id_free(shipId2);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_from_string - imo with prefix, invalid checksum falls back to other id")
    {
        auto* shipId = dnv_vista_sdk_ship_id_from_string("IMO1234568");
        REQUIRE(shipId != nullptr);
        CHECK(dnv_vista_sdk_ship_id_is_other_id(shipId) == 1);

        const auto* otherId = dnv_vista_sdk_ship_id_other_id(shipId);
        REQUIRE(otherId != nullptr);
        CHECK(std::string_view{ otherId } == "IMO1234568");

        dnv_vista_sdk_ship_id_free(shipId);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_from_string - alternative id")
    {
        auto* shipId = dnv_vista_sdk_ship_id_from_string("VESSEL-XYZ-789");
        REQUIRE(shipId != nullptr);
        CHECK(dnv_vista_sdk_ship_id_is_other_id(shipId) == 1);

        dnv_vista_sdk_ship_id_free(shipId);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_from_string - empty string returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_ship_id_from_string("") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_from_string - null returns null")
    {
        CHECK(dnv_vista_sdk_ship_id_from_string(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_equals - imo numbers")
    {
        auto* imo1 = dnv_vista_sdk_imo_number_from_string("9074729");
        auto* imo2 = dnv_vista_sdk_imo_number_from_string("9074729");
        auto* imo3 = dnv_vista_sdk_imo_number_from_string("1234567");
        REQUIRE(imo1 != nullptr);
        REQUIRE(imo2 != nullptr);
        REQUIRE(imo3 != nullptr);

        auto* shipId1 = dnv_vista_sdk_ship_id_from_imo_number(imo1);
        auto* shipId2 = dnv_vista_sdk_ship_id_from_imo_number(imo2);
        auto* shipId3 = dnv_vista_sdk_ship_id_from_imo_number(imo3);

        CHECK(dnv_vista_sdk_ship_id_equals(shipId1, shipId2) == 1);
        CHECK(dnv_vista_sdk_ship_id_equals(shipId1, shipId3) == 0);

        dnv_vista_sdk_ship_id_free(shipId1);
        dnv_vista_sdk_ship_id_free(shipId2);
        dnv_vista_sdk_ship_id_free(shipId3);
        dnv_vista_sdk_imo_number_free(imo1);
        dnv_vista_sdk_imo_number_free(imo2);
        dnv_vista_sdk_imo_number_free(imo3);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_equals - imo vs alternative")
    {
        auto* imo = dnv_vista_sdk_imo_number_from_string("9074729");
        REQUIRE(imo != nullptr);
        auto* shipId1 = dnv_vista_sdk_ship_id_from_imo_number(imo);
        auto* shipId2 = dnv_vista_sdk_ship_id_from_other_id("9074729");

        CHECK(dnv_vista_sdk_ship_id_equals(shipId1, shipId2) == 0);

        dnv_vista_sdk_ship_id_free(shipId1);
        dnv_vista_sdk_ship_id_free(shipId2);
        dnv_vista_sdk_imo_number_free(imo);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_equals - null arguments return 0 and set last error")
    {
        auto* shipId = dnv_vista_sdk_ship_id_from_other_id("X");
        REQUIRE(shipId != nullptr);

        CHECK(dnv_vista_sdk_ship_id_equals(nullptr, shipId) == 0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
        CHECK(dnv_vista_sdk_ship_id_equals(shipId, nullptr) == 0);

        dnv_vista_sdk_ship_id_free(shipId);
    }

    TEST_CASE("dnv_vista_sdk_ship_id_free - null is a no-op")
    {
        dnv_vista_sdk_ship_id_free(nullptr);
    }

    TEST_CASE("null shipId handle - accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_ship_id_is_imo_number(nullptr) == 0);
        CHECK(dnv_vista_sdk_ship_id_is_other_id(nullptr) == 0);
        CHECK(dnv_vista_sdk_ship_id_imo_number(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_ship_id_other_id(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_ship_id_to_string(nullptr) == nullptr);
    }
}
