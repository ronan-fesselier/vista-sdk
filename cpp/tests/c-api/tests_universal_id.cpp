#include <doctest/doctest.h>

#include <dnv/VistaSDK_c_api.h>

#include <string_view>

namespace
{
    constexpr const char* validUniversalIdStr =
        "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/"
        "qty-mass/cnt-fuel.oil/pos-inlet";
} // namespace

TEST_SUITE("c-api::universal_id")
{
    TEST_CASE("dnv_vista_sdk_universal_id_naming_entity - is data.dnv.com")
    {
        CHECK(std::string_view{ dnv_vista_sdk_universal_id_naming_entity() } == "data.dnv.com");
    }

    TEST_CASE("dnv_vista_sdk_universal_id_from_string - valid string returns non-null")
    {
        auto* universalId = dnv_vista_sdk_universal_id_from_string(validUniversalIdStr);

        REQUIRE(universalId != nullptr);

        dnv_vista_sdk_universal_id_free(universalId);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_from_string - invalid string returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_universal_id_from_string("not-a-universal-id") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_from_string - null returns null")
    {
        CHECK(dnv_vista_sdk_universal_id_from_string(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_imo_number - matches parsed IMO number")
    {
        auto* universalId = dnv_vista_sdk_universal_id_from_string(validUniversalIdStr);
        REQUIRE(universalId != nullptr);

        const auto* imoNumber = dnv_vista_sdk_universal_id_imo_number(universalId);
        REQUIRE(imoNumber != nullptr);

        dnv_vista_sdk_universal_id_free(universalId);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_local_id - non-null")
    {
        auto* universalId = dnv_vista_sdk_universal_id_from_string(validUniversalIdStr);
        REQUIRE(universalId != nullptr);

        CHECK(dnv_vista_sdk_universal_id_local_id(universalId) != nullptr);

        dnv_vista_sdk_universal_id_free(universalId);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_builder - returns a valid builder handle")
    {
        auto* universalId = dnv_vista_sdk_universal_id_from_string(validUniversalIdStr);
        REQUIRE(universalId != nullptr);

        const auto* builder = dnv_vista_sdk_universal_id_builder(universalId);
        REQUIRE(builder != nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder_is_valid(builder) == 1);

        dnv_vista_sdk_universal_id_free(universalId);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_to_string - round-trips the input string")
    {
        auto* universalId = dnv_vista_sdk_universal_id_from_string(validUniversalIdStr);
        REQUIRE(universalId != nullptr);

        char* str = dnv_vista_sdk_universal_id_to_string(universalId);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == validUniversalIdStr);

        dnv_vista_sdk_string_free(str);
        dnv_vista_sdk_universal_id_free(universalId);
    }

    TEST_CASE("dnv_vista_sdk_universal_id_free - null is a no-op")
    {
        dnv_vista_sdk_universal_id_free(nullptr);
    }

    TEST_CASE("null universalId handle - accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_universal_id_imo_number(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_universal_id_local_id(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_universal_id_builder(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_universal_id_to_string(nullptr) == nullptr);
    }
}
