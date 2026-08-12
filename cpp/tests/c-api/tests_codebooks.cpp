#include <doctest/doctest.h>

#include <dnv/VistaSDK_c_api.h>

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

    TEST_CASE("dnv_vista_sdk_codebooks_version - returns version string")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-4a");

        const char* version = dnv_vista_sdk_codebooks_version(codebooks);

        REQUIRE(version != nullptr);
        CHECK(std::string_view{ version } == "3-4a");
    }

    TEST_CASE("dnv_vista_sdk_codebooks_version - null returns null")
    {
        CHECK(dnv_vista_sdk_codebooks_version(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_codebook_name - returns codebook name")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_POSITION);

        CHECK(dnv_vista_sdk_codebook_name(codebook) == DNV_VISTA_SDK_CODEBOOK_NAME_POSITION);
    }

    TEST_CASE("dnv_vista_sdk_codebook_name - null returns 0")
    {
        CHECK(dnv_vista_sdk_codebook_name(nullptr) == 0);
    }

    TEST_CASE("dnv_vista_sdk_codebook_standard_values_count - returns non-zero for quantity")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);

        CHECK(dnv_vista_sdk_codebook_standard_values_count(codebook) > 0);
    }

    TEST_CASE("dnv_vista_sdk_codebook_standard_value_at - returns known value")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);
        const size_t count = dnv_vista_sdk_codebook_standard_values_count(codebook);

        bool found = false;
        for (size_t i = 0; i < count; ++i)
        {
            const char* value = dnv_vista_sdk_codebook_standard_value_at(codebook, i);
            REQUIRE(value != nullptr);
            if (std::string_view{ value } == "temperature")
            {
                found = true;
                break;
            }
        }
        CHECK(found);
    }

    TEST_CASE("dnv_vista_sdk_codebook_standard_value_at - out of range returns null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);
        const size_t count = dnv_vista_sdk_codebook_standard_values_count(codebook);

        CHECK(dnv_vista_sdk_codebook_standard_value_at(codebook, count) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_codebook_groups_count - returns non-zero for quantity")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);

        CHECK(dnv_vista_sdk_codebook_groups_count(codebook) > 0);
    }

    TEST_CASE("dnv_vista_sdk_codebook_group_at - returns non-null strings")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);
        const size_t count = dnv_vista_sdk_codebook_groups_count(codebook);

        for (size_t i = 0; i < count; ++i)
        {
            CHECK(dnv_vista_sdk_codebook_group_at(codebook, i) != nullptr);
        }
    }

    TEST_CASE("dnv_vista_sdk_codebook_has_group - returns true for existing group")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);
        const size_t count = dnv_vista_sdk_codebook_groups_count(codebook);
        REQUIRE(count > 0);

        const char* first_group = dnv_vista_sdk_codebook_group_at(codebook, 0);
        REQUIRE(first_group != nullptr);
        CHECK(dnv_vista_sdk_codebook_has_group(codebook, first_group) == 1);
    }

    TEST_CASE("dnv_vista_sdk_codebook_has_group - returns false for unknown group")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);

        CHECK(dnv_vista_sdk_codebook_has_group(codebook, "not-a-group") == 0);
    }

    TEST_CASE("dnv_vista_sdk_codebook_validate_position - standard value is valid")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_POSITION);

        CHECK(dnv_vista_sdk_codebook_validate_position(codebook, "centre") == DNV_VISTA_SDK_POSITION_VALIDATION_VALID);
        CHECK(dnv_vista_sdk_codebook_validate_position(codebook, "port") == DNV_VISTA_SDK_POSITION_VALIDATION_VALID);
        CHECK(dnv_vista_sdk_codebook_validate_position(codebook, "1") == DNV_VISTA_SDK_POSITION_VALIDATION_VALID);
        CHECK(dnv_vista_sdk_codebook_validate_position(codebook, "42") == DNV_VISTA_SDK_POSITION_VALIDATION_VALID);
    }

    TEST_CASE("dnv_vista_sdk_codebook_validate_position - custom value is custom")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_POSITION);

        CHECK(
            dnv_vista_sdk_codebook_validate_position(codebook, "custom-position") ==
            DNV_VISTA_SDK_POSITION_VALIDATION_CUSTOM);
    }

    TEST_CASE("dnv_vista_sdk_codebook_validate_position - invalid value is invalid")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_POSITION);

        CHECK(
            dnv_vista_sdk_codebook_validate_position(codebook, "invalid position!") ==
            DNV_VISTA_SDK_POSITION_VALIDATION_INVALID);
    }

    TEST_CASE("dnv_vista_sdk_codebook_validate_position - wrong order is invalid order")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_POSITION);

        CHECK(
            dnv_vista_sdk_codebook_validate_position(codebook, "starboard-centre") ==
            DNV_VISTA_SDK_POSITION_VALIDATION_INVALID_ORDER);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tag_prefix - standard tag returns dash")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);
        auto* tag = dnv_vista_sdk_codebook_create_tag(codebook, "temperature");
        REQUIRE(tag != nullptr);

        CHECK(dnv_vista_sdk_metadata_tag_prefix(tag) == '-');

        dnv_vista_sdk_metadata_tag_free(tag);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tag_prefix - custom tag returns tilde")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* codebooks = dnv_vista_sdk_vis_codebooks(vis, "3-11a");
        const auto* codebook = dnv_vista_sdk_codebooks_at(codebooks, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);
        auto* tag = dnv_vista_sdk_codebook_create_tag(codebook, "some-custom-value");
        REQUIRE(tag != nullptr);

        CHECK(dnv_vista_sdk_metadata_tag_prefix(tag) == '~');

        dnv_vista_sdk_metadata_tag_free(tag);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tag_prefix - null returns null char")
    {
        CHECK(dnv_vista_sdk_metadata_tag_prefix(nullptr) == '\0');
    }
}
