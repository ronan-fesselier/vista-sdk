#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/imo_number.h>
#include <dnv/vista/sdk/c/common.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::imo_number")
{
    TEST_CASE("dnv_vista_sdk_imo_number_is_valid - valid numbers")
    {
        CHECK(dnv_vista_sdk_imo_number_is_valid(9074729) == 1);
        CHECK(dnv_vista_sdk_imo_number_is_valid(9785811) == 1);
        CHECK(dnv_vista_sdk_imo_number_is_valid(9704611) == 1);
    }

    TEST_CASE("dnv_vista_sdk_imo_number_is_valid - invalid numbers")
    {
        CHECK(dnv_vista_sdk_imo_number_is_valid(-1) == 0);
        CHECK(dnv_vista_sdk_imo_number_is_valid(0) == 0);
        CHECK(dnv_vista_sdk_imo_number_is_valid(1) == 0);
        CHECK(dnv_vista_sdk_imo_number_is_valid(1234507) == 0);
    }

    TEST_CASE("dnv_vista_sdk_imo_number_create - valid value matches from_string")
    {
        auto* fromInt = dnv_vista_sdk_imo_number_create(9074729);
        auto* fromStr = dnv_vista_sdk_imo_number_from_string("9074729");

        REQUIRE(fromInt != nullptr);
        REQUIRE(fromStr != nullptr);
        CHECK(dnv_vista_sdk_imo_number_equals(fromInt, fromStr) == 1);

        dnv_vista_sdk_imo_number_free(fromInt);
        dnv_vista_sdk_imo_number_free(fromStr);
    }

    TEST_CASE("dnv_vista_sdk_imo_number_create - invalid checksum returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_imo_number_create(1234507) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_imo_number_from_string - valid string with and without prefix")
    {
        auto* withoutPrefix = dnv_vista_sdk_imo_number_from_string("9074729");
        auto* withPrefix = dnv_vista_sdk_imo_number_from_string("IMO9074729");

        REQUIRE(withoutPrefix != nullptr);
        REQUIRE(withPrefix != nullptr);
        CHECK(dnv_vista_sdk_imo_number_equals(withoutPrefix, withPrefix) == 1);

        dnv_vista_sdk_imo_number_free(withoutPrefix);
        dnv_vista_sdk_imo_number_free(withPrefix);
    }

    TEST_CASE("dnv_vista_sdk_imo_number_from_string - invalid string returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_imo_number_from_string("not-an-imo") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_imo_number_from_string - null value returns null")
    {
        CHECK(dnv_vista_sdk_imo_number_from_string(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_imo_number_to_string - formats with IMO prefix")
    {
        auto* imoNumber = dnv_vista_sdk_imo_number_from_string("9074729");
        REQUIRE(imoNumber != nullptr);

        char* str = dnv_vista_sdk_imo_number_to_string(imoNumber);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "IMO9074729");

        dnv_vista_sdk_string_free(str);
        dnv_vista_sdk_imo_number_free(imoNumber);
    }

    TEST_CASE("dnv_vista_sdk_imo_number_equals - different numbers are not equal")
    {
        auto* a = dnv_vista_sdk_imo_number_from_string("9074729");
        auto* b = dnv_vista_sdk_imo_number_from_string("9785811");

        REQUIRE(a != nullptr);
        REQUIRE(b != nullptr);
        CHECK(dnv_vista_sdk_imo_number_equals(a, b) == 0);

        dnv_vista_sdk_imo_number_free(a);
        dnv_vista_sdk_imo_number_free(b);
    }

    TEST_CASE("dnv_vista_sdk_imo_number_free - null is a no-op")
    {
        dnv_vista_sdk_imo_number_free(nullptr);
    }

    TEST_CASE("null handle - accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_imo_number_equals(nullptr, nullptr) == 0);
        CHECK(dnv_vista_sdk_imo_number_to_string(nullptr) == nullptr);
    }
}
