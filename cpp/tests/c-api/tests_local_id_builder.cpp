#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/gmod_path.h>
#include <dnv/vista/sdk/c/core/local_id.h>
#include <dnv/vista/sdk/c/core/local_id_builder.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::local_id_builder")
{
    TEST_CASE("dnv_vista_sdk_local_id_builder_naming_rule - is dnv-v2")
    {
        CHECK(std::string_view{ dnv_vista_sdk_local_id_builder_naming_rule() } == "dnv-v2");
    }

    TEST_CASE("dnv_vista_sdk_local_id_builder_create - valid version returns non-null")
    {
        auto* builder = dnv_vista_sdk_local_id_builder_create("3-4a");

        REQUIRE(builder != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_local_id_builder_version(builder) } == "3-4a");
        CHECK(dnv_vista_sdk_local_id_builder_is_empty(builder) == 1);
        CHECK(dnv_vista_sdk_local_id_builder_is_valid(builder) == 0);

        dnv_vista_sdk_local_id_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_local_id_builder_create - invalid version returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_local_id_builder_create("not-a-version") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_local_id_builder_create - null returns null")
    {
        CHECK(dnv_vista_sdk_local_id_builder_create(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_local_id_builder - fluent chain builds a valid LocalId")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.31-2", "3-4a");
        REQUIRE(path != nullptr);

        auto* b0 = dnv_vista_sdk_local_id_builder_create("3-4a");
        auto* b1 = dnv_vista_sdk_local_id_builder_with_primary_item(b0, path);

        REQUIRE(b1 != nullptr);
        CHECK(dnv_vista_sdk_local_id_builder_is_empty(b1) == 0);

        dnv_vista_sdk_gmod_path_free(path);
        dnv_vista_sdk_local_id_builder_free(b0);
        dnv_vista_sdk_local_id_builder_free(b1);
    }

    TEST_CASE("dnv_vista_sdk_local_id_builder_with_vis_version - invalid string throws and sets last error")
    {
        auto* builder = dnv_vista_sdk_local_id_builder_create("3-4a");
        REQUIRE(builder != nullptr);

        CHECK(dnv_vista_sdk_local_id_builder_with_vis_version(builder, "not-a-version") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_local_id_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_local_id_builder_from_string - valid string yields non-null LocalId")
    {
        auto* localId = dnv_vista_sdk_local_id_builder_from_string(
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet");

        REQUIRE(localId != nullptr);

        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("dnv_vista_sdk_local_id_builder_from_string_with_errors - invalid string yields non-empty errors")
    {
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;

        auto* localId = dnv_vista_sdk_local_id_builder_from_string_with_errors("not-a-local-id", &errors);

        CHECK(localId == nullptr);
        REQUIRE(errors != nullptr);
        CHECK(dnv_vista_sdk_parsing_errors_has_errors(errors) == 1);

        dnv_vista_sdk_parsing_errors_free(errors);
    }

    TEST_CASE("dnv_vista_sdk_local_id_builder_free - null is a no-op")
    {
        dnv_vista_sdk_local_id_builder_free(nullptr);
    }

    TEST_CASE("null builder handle - accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_local_id_builder_version(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_builder_is_verbose_mode(nullptr) == 0);
        CHECK(dnv_vista_sdk_local_id_builder_is_valid(nullptr) == 0);
        CHECK(dnv_vista_sdk_local_id_builder_is_empty(nullptr) == 0);
        CHECK(dnv_vista_sdk_local_id_builder_is_empty_metadata(nullptr) == 0);
        CHECK(dnv_vista_sdk_local_id_builder_has_custom_tag(nullptr) == 0);
        CHECK(dnv_vista_sdk_local_id_builder_primary_item(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_builder_secondary_item(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_builder_metadata_tag(nullptr, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY) == nullptr);
        CHECK(dnv_vista_sdk_local_id_builder_build(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_builder_to_string(nullptr) == nullptr);
    }
}
