#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/imo_number.h>
#include <dnv/vista/sdk/c/core/local_id.h>
#include <dnv/vista/sdk/c/core/local_id_builder.h>
#include <dnv/vista/sdk/c/core/parsing_errors.h>
#include <dnv/vista/sdk/c/core/universal_id.h>
#include <dnv/vista/sdk/c/core/universal_id_builder.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

namespace
{
    constexpr const char* validUniversalIdStr =
        "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/"
        "qty-mass/cnt-fuel.oil/pos-inlet";
    constexpr const char* validLocalIdStr =
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet";
} // namespace

TEST_SUITE("c-api::universal_id_builder")
{
    TEST_CASE("dnv_vista_sdk_universal_id_builder_naming_entity - is data.dnv.com")
    {
        CHECK(std::string_view{ dnv_vista_sdk_universal_id_builder_naming_entity() } == "data.dnv.com");
    }

    TEST_CASE("dnv_vista_sdk_universal_id_builder_create - valid version returns non-null, invalid")
    {
        auto* builder = dnv_vista_sdk_universal_id_builder_create("3-4a");

        REQUIRE(builder != nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder_is_valid(builder) == 0);

        dnv_vista_sdk_universal_id_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_builder_create - invalid version returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_universal_id_builder_create("not-a-version") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_builder_create - null returns null")
    {
        CHECK(dnv_vista_sdk_universal_id_builder_create(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_builder - fluent chain builds a valid UniversalId")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);
        auto* localIdBuilder = dnv_vista_sdk_local_id_builder(localId);
        REQUIRE(localIdBuilder != nullptr);

        auto* imoNumber = dnv_vista_sdk_imo_number_create(9074729);
        REQUIRE(imoNumber != nullptr);

        auto* b0 = dnv_vista_sdk_universal_id_builder_create("3-4a");
        auto* b1 = dnv_vista_sdk_universal_id_builder_with_imo_number(b0, imoNumber);
        REQUIRE(b1 != nullptr);
        auto* b2 = dnv_vista_sdk_universal_id_builder_with_local_id(b1, localIdBuilder);
        REQUIRE(b2 != nullptr);

        CHECK(dnv_vista_sdk_universal_id_builder_is_valid(b2) == 1);

        auto* universalId = dnv_vista_sdk_universal_id_builder_build(b2);
        REQUIRE(universalId != nullptr);

        dnv_vista_sdk_universal_id_free(universalId);
        dnv_vista_sdk_universal_id_builder_free(b0);
        dnv_vista_sdk_universal_id_builder_free(b1);
        dnv_vista_sdk_universal_id_builder_free(b2);
        dnv_vista_sdk_imo_number_free(imoNumber);
        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_builder_build - invalid builder returns null and sets last error")
    {
        auto* builder = dnv_vista_sdk_universal_id_builder_create("3-4a");
        REQUIRE(builder != nullptr);

        CHECK(dnv_vista_sdk_universal_id_builder_build(builder) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_universal_id_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_builder_without_imo_number/without_local_id - clear state")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);
        auto* localIdBuilder = dnv_vista_sdk_local_id_builder(localId);
        REQUIRE(localIdBuilder != nullptr);

        auto* imoNumber = dnv_vista_sdk_imo_number_create(9074729);
        REQUIRE(imoNumber != nullptr);

        auto* b0 = dnv_vista_sdk_universal_id_builder_create("3-4a");
        auto* b1 = dnv_vista_sdk_universal_id_builder_with_imo_number(b0, imoNumber);
        auto* b2 = dnv_vista_sdk_universal_id_builder_with_local_id(b1, localIdBuilder);
        REQUIRE(b2 != nullptr);

        auto* b3 = dnv_vista_sdk_universal_id_builder_without_imo_number(b2);
        REQUIRE(b3 != nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder_imo_number(b3) == nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder_is_valid(b3) == 0);

        auto* b4 = dnv_vista_sdk_universal_id_builder_without_local_id(b2);
        REQUIRE(b4 != nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder_local_id(b4) == nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder_is_valid(b4) == 0);

        dnv_vista_sdk_universal_id_builder_free(b0);
        dnv_vista_sdk_universal_id_builder_free(b1);
        dnv_vista_sdk_universal_id_builder_free(b2);
        dnv_vista_sdk_universal_id_builder_free(b3);
        dnv_vista_sdk_universal_id_builder_free(b4);
        dnv_vista_sdk_imo_number_free(imoNumber);
        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_builder_to_string - round-trips the input string")
    {
        auto* universalId = dnv_vista_sdk_universal_id_builder_from_string(validUniversalIdStr);
        REQUIRE(universalId != nullptr);

        const auto* builder = dnv_vista_sdk_universal_id_builder(universalId);
        REQUIRE(builder != nullptr);

        char* str = dnv_vista_sdk_universal_id_builder_to_string(builder);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == validUniversalIdStr);

        dnv_vista_sdk_universal_id_builder_string_free(str);
        dnv_vista_sdk_universal_id_free(universalId);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_builder_from_string - valid string returns non-null")
    {
        auto* universalId = dnv_vista_sdk_universal_id_builder_from_string(validUniversalIdStr);

        REQUIRE(universalId != nullptr);

        dnv_vista_sdk_universal_id_free(universalId);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_builder_from_string - invalid string returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_universal_id_builder_from_string("not-a-universal-id") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_builder_from_string - null returns null")
    {
        CHECK(dnv_vista_sdk_universal_id_builder_from_string(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_builder_from_string_with_errors - invalid string sets outErrors")
    {
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;

        auto* universalId = dnv_vista_sdk_universal_id_builder_from_string_with_errors("not-a-universal-id", &errors);

        CHECK(universalId == nullptr);
        REQUIRE(errors != nullptr);
        CHECK(dnv_vista_sdk_parsing_errors_has_errors(errors) == 1);

        dnv_vista_sdk_parsing_errors_free(errors);
    }

    TEST_CASE(
        "dnv_vista_sdk_universal_id_builder_from_string_with_errors - valid string returns non-null and no errors")
    {
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;

        auto* universalId = dnv_vista_sdk_universal_id_builder_from_string_with_errors(validUniversalIdStr, &errors);

        REQUIRE(universalId != nullptr);
        REQUIRE(errors != nullptr);
        CHECK(dnv_vista_sdk_parsing_errors_has_errors(errors) == 0);

        dnv_vista_sdk_universal_id_free(universalId);
        dnv_vista_sdk_parsing_errors_free(errors);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_builder_from_string_with_errors - null arguments return null")
    {
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;

        CHECK(dnv_vista_sdk_universal_id_builder_from_string_with_errors(nullptr, &errors) == nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder_from_string_with_errors(validUniversalIdStr, nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_builder_free - null is a no-op")
    {
        dnv_vista_sdk_universal_id_builder_free(nullptr);
    }

    TEST_CASE("null builder handle - accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_universal_id_builder_imo_number(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder_local_id(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder_is_valid(nullptr) == 0);
        CHECK(dnv_vista_sdk_universal_id_builder_with_imo_number(nullptr, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder_without_imo_number(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder_with_local_id(nullptr, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder_without_local_id(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder_build(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder_to_string(nullptr) == nullptr);
    }
}
