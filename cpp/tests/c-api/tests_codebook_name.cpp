#include <doctest/doctest.h>

#include <dnv/VistaSDK_c_api.h>

#include <string_view>

TEST_SUITE("c-api::codebook_name")
{
    TEST_CASE("dnv_vista_sdk_codebook_names_from_prefix - known prefix succeeds")
    {
        dnv_vista_sdk_codebook_name_t name;

        CHECK(dnv_vista_sdk_codebook_names_from_prefix("qty", &name) == 1);
        CHECK(name == DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);
    }

    TEST_CASE("dnv_vista_sdk_codebook_names_from_prefix - unknown prefix fails and sets last error")
    {
        dnv_vista_sdk_codebook_name_t name;

        CHECK(dnv_vista_sdk_codebook_names_from_prefix("not-a-prefix", &name) == 0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_codebook_names_from_prefix - null arguments fail")
    {
        dnv_vista_sdk_codebook_name_t name;

        CHECK(dnv_vista_sdk_codebook_names_from_prefix(nullptr, &name) == 0);
        CHECK(dnv_vista_sdk_codebook_names_from_prefix("qty", nullptr) == 0);
    }

    TEST_CASE("dnv_vista_sdk_codebook_names_to_prefix - valid name returns matching prefix")
    {
        CHECK(
            std::string_view{ dnv_vista_sdk_codebook_names_to_prefix(DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY) } == "qty");
        CHECK(
            std::string_view{ dnv_vista_sdk_codebook_names_to_prefix(DNV_VISTA_SDK_CODEBOOK_NAME_DETAIL) } == "detail");
    }

    TEST_CASE("dnv_vista_sdk_codebook_names_to_string - valid name returns human-readable name")
    {
        CHECK(
            std::string_view{ dnv_vista_sdk_codebook_names_to_string(DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY) } ==
            "Quantity");
    }

    TEST_CASE("dnv_vista_sdk_codebook_names_to_prefix/to_string - roundtrip via from_prefix")
    {
        dnv_vista_sdk_codebook_name_t name;
        REQUIRE(dnv_vista_sdk_codebook_names_from_prefix("funct.svc", &name) == 1);

        CHECK(std::string_view{ dnv_vista_sdk_codebook_names_to_prefix(name) } == "funct.svc");
        CHECK(std::string_view{ dnv_vista_sdk_codebook_names_to_string(name) } == "FunctionalServices");
    }
}
