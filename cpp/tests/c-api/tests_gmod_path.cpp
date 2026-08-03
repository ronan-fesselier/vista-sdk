#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/gmod.h>
#include <dnv/vista/sdk/c/core/gmod_node.h>
#include <dnv/vista/sdk/c/core/gmod_path.h>
#include <dnv/vista/sdk/c/core/locations.h>
#include <dnv/vista/sdk/c/core/parsing_errors.h>
#include <dnv/vista/sdk/c/core/vis.h>
#include <dnv/vista/sdk/c/common.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::gmod_path")
{
    TEST_CASE("dnv_vista_sdk_gmod_path_from_short_path_version - valid path returns non-null")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.72/I101", "3-11a");

        REQUIRE(path != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_gmod_path_version(path) } == "3-11a");

        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_from_short_path_version - invalid path returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_gmod_path_from_short_path_version("not-a-path", "3-11a") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_from_short_path_version - null arguments return null")
    {
        CHECK(dnv_vista_sdk_gmod_path_from_short_path_version(nullptr, "3-11a") == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_from_short_path_version("411.1", nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_from_short_path - matches the version-only overload")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");

        auto* path = dnv_vista_sdk_gmod_path_from_short_path("411.1/C101.72/I101", gmod, locations);

        REQUIRE(path != nullptr);

        char* str = dnv_vista_sdk_gmod_path_to_string(path);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "411.1/C101.72/I101");

        dnv_vista_sdk_string_free(str);
        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_from_short_path - null arguments return null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");

        CHECK(dnv_vista_sdk_gmod_path_from_short_path(nullptr, gmod, locations) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_from_short_path("411.1", nullptr, locations) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_from_short_path("411.1", gmod, nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_from_short_path_with_errors - valid path yields empty errors")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;

        auto* path =
            dnv_vista_sdk_gmod_path_from_short_path_with_errors("411.1/C101.72/I101", gmod, locations, &errors);

        REQUIRE(path != nullptr);
        REQUIRE(errors != nullptr);
        CHECK(dnv_vista_sdk_parsing_errors_has_errors(errors) == 0);

        dnv_vista_sdk_gmod_path_free(path);
        dnv_vista_sdk_parsing_errors_free(errors);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_from_short_path_with_errors - invalid path yields non-empty errors")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;

        auto* path = dnv_vista_sdk_gmod_path_from_short_path_with_errors("not-a-path", gmod, locations, &errors);

        CHECK(path == nullptr);
        REQUIRE(errors != nullptr);
        CHECK(dnv_vista_sdk_parsing_errors_has_errors(errors) == 1);

        dnv_vista_sdk_parsing_errors_free(errors);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_from_full_path - matches short path result")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");

        auto* path = dnv_vista_sdk_gmod_path_from_full_path(
            "VE/400a/410/411/411i/411.1/CS1/C101/C101.7/C101.72/I101", gmod, locations);

        REQUIRE(path != nullptr);

        char* str = dnv_vista_sdk_gmod_path_to_string(path);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "411.1/C101.72/I101");

        dnv_vista_sdk_string_free(str);
        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_from_full_path_with_errors - invalid path yields non-empty errors")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;

        auto* path = dnv_vista_sdk_gmod_path_from_full_path_with_errors("not-a-path", gmod, locations, &errors);

        CHECK(path == nullptr);
        REQUIRE(errors != nullptr);
        CHECK(dnv_vista_sdk_parsing_errors_has_errors(errors) == 1);

        dnv_vista_sdk_parsing_errors_free(errors);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_node/length/at - reflect the parsed path")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.72/I101", "3-11a");
        REQUIRE(path != nullptr);

        const auto* node = dnv_vista_sdk_gmod_path_node(path);
        REQUIRE(node != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_gmod_node_code(node) } == "I101");

        const size_t length = dnv_vista_sdk_gmod_path_length(path);
        REQUIRE(length > 0);

        const auto* root = dnv_vista_sdk_gmod_path_at(path, 0);
        REQUIRE(root != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_gmod_node_code(root) } == "VE");

        const auto* last = dnv_vista_sdk_gmod_path_at(path, length - 1);
        REQUIRE(last != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_gmod_node_code(last) } == "I101");

        CHECK(dnv_vista_sdk_gmod_path_at(path, length) == nullptr);

        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_to_string/to_full_path_string/to_string_dump - non-null")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.72/I101", "3-11a");
        REQUIRE(path != nullptr);

        char* str = dnv_vista_sdk_gmod_path_to_string(path);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "411.1/C101.72/I101");
        dnv_vista_sdk_string_free(str);

        char* fullStr = dnv_vista_sdk_gmod_path_to_full_path_string(path);
        REQUIRE(fullStr != nullptr);
        CHECK(std::string_view{ fullStr } == "VE/400a/410/411/411i/411.1/CS1/C101/C101.7/C101.72/I101");
        dnv_vista_sdk_string_free(fullStr);

        char* dumpStr = dnv_vista_sdk_gmod_path_to_string_dump(path);
        REQUIRE(dumpStr != nullptr);
        CHECK(std::string_view{ dumpStr }.size() > 0);
        dnv_vista_sdk_string_free(dumpStr);

        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_without_locations - non-null")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.72/I101", "3-11a");
        REQUIRE(path != nullptr);

        auto* withoutLocations = dnv_vista_sdk_gmod_path_without_locations(path);
        REQUIRE(withoutLocations != nullptr);

        dnv_vista_sdk_gmod_path_free(path);
        dnv_vista_sdk_gmod_path_free(withoutLocations);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_free - null is a no-op")
    {
        dnv_vista_sdk_gmod_path_free(nullptr);
    }

    TEST_CASE("null path handle - accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_gmod_path_version(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_node(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_length(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_path_at(nullptr, 0) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_is_mappable(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_path_is_individualizable(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_path_without_locations(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_normal_assignment_name(nullptr, 0) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_individualizable_set_count(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_path_individualizable_set_at(nullptr, 0) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_common_name_count(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_path_common_name_at(nullptr, 0) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_to_string(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_to_full_path_string(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_to_string_dump(nullptr) == nullptr);
    }
}
