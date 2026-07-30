#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/codebook.h>
#include <dnv/vista/sdk/c/core/codebooks.h>
#include <dnv/vista/sdk/c/core/vis.h>
#include <dnv/vista/sdk/c/common.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::codebooks")
{
    TEST_CASE("dnv_vista_sdk_vis_codebooks - valid version returns non-null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");

        CHECK(codebooks != nullptr);
    }

    TEST_CASE("dnv_vista_sdk_vis_codebooks - invalid version returns null and sets last error")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();

        CHECK(dnv_vista_sdk_vis_codebooks(vis, "not-a-version") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_vis_codebooks - null arguments return null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();

        CHECK(dnv_vista_sdk_vis_codebooks(nullptr, "3-11a") == nullptr);
        CHECK(dnv_vista_sdk_vis_codebooks(vis, nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_codebooks_at - known name returns non-null codebook")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);

        CHECK(codebook != nullptr);
    }

    TEST_CASE("dnv_vista_sdk_codebooks_at - null codebooks returns null")
    {
        CHECK(dnv_vista_sdk_codebooks_at(nullptr, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_codebook_has_standard_value - known value returns true")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);

        CHECK(dnv_vista_sdk_codebook_has_standard_value(codebook, "temperature") == 1);
    }

    TEST_CASE("dnv_vista_sdk_codebook_has_standard_value - unknown value returns false")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);

        CHECK(dnv_vista_sdk_codebook_has_standard_value(codebook, "not-a-value") == 0);
    }

    TEST_CASE("dnv_vista_sdk_codebook_create_tag - valid value returns non-null tag")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);

        auto* tag = dnv_vista_sdk_codebook_create_tag(codebook, "temperature");

        REQUIRE(tag != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_metadata_tag_value(tag) } == "temperature");
        CHECK(dnv_vista_sdk_metadata_tag_is_custom(tag) == 0);
        CHECK(dnv_vista_sdk_metadata_tag_name(tag) == DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);

        dnv_vista_sdk_metadata_tag_free(tag);
    }

    TEST_CASE("dnv_vista_sdk_codebook_create_tag - custom value returns tag marked custom")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);

        auto* tag = dnv_vista_sdk_codebook_create_tag(codebook, "some-custom-value");

        REQUIRE(tag != nullptr);
        CHECK(dnv_vista_sdk_metadata_tag_is_custom(tag) == 1);

        dnv_vista_sdk_metadata_tag_free(tag);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tag_to_string - formats as prefix-value")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);
        auto* tag = dnv_vista_sdk_codebook_create_tag(codebook, "temperature");
        REQUIRE(tag != nullptr);

        char* str = dnv_vista_sdk_metadata_tag_to_string(tag);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "qty-temperature");

        dnv_vista_sdk_string_free(str);
        dnv_vista_sdk_metadata_tag_free(tag);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tag_free - null is a no-op")
    {
        dnv_vista_sdk_metadata_tag_free(nullptr);
    }
}
