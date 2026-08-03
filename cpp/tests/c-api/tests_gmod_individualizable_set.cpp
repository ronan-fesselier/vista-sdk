#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/gmod_individualizable_set.h>
#include <dnv/vista/sdk/c/core/gmod_node.h>
#include <dnv/vista/sdk/c/core/gmod_path.h>
#include <dnv/vista/sdk/c/core/location_builder.h>
#include <dnv/vista/sdk/c/common.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::gmod_individualizable_set")
{
    TEST_CASE("dnv_vista_sdk_gmod_path_individualizable_set_at - via a path with an individualizable node")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.72/I101", "3-11a");
        REQUIRE(path != nullptr);

        const size_t setCount = dnv_vista_sdk_gmod_path_individualizable_set_count(path);
        REQUIRE(setCount > 0);

        auto* set = dnv_vista_sdk_gmod_path_individualizable_set_at(path, 0);
        REQUIRE(set != nullptr);

        const size_t nodeCount = dnv_vista_sdk_gmod_individualizable_set_node_count(set);
        REQUIRE(nodeCount > 0);

        auto* node = dnv_vista_sdk_gmod_individualizable_set_node_at(set, 0);
        REQUIRE(node != nullptr);
        CHECK(dnv_vista_sdk_gmod_node_code(node) != nullptr);

        dnv_vista_sdk_gmod_node_free(node);
        dnv_vista_sdk_gmod_individualizable_set_free(set);
        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_gmod_individualizable_set_index_count/at - reflect the node indices")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.72/I101", "3-11a");
        REQUIRE(path != nullptr);

        auto* set = dnv_vista_sdk_gmod_path_individualizable_set_at(path, 0);
        REQUIRE(set != nullptr);

        const size_t indexCount = dnv_vista_sdk_gmod_individualizable_set_index_count(set);
        REQUIRE(indexCount > 0);

        int index;
        CHECK(dnv_vista_sdk_gmod_individualizable_set_index_at(set, 0, &index) == 1);
        CHECK(index >= 0);
        CHECK(dnv_vista_sdk_gmod_individualizable_set_index_at(set, indexCount, &index) == 0);

        dnv_vista_sdk_gmod_individualizable_set_free(set);
        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_gmod_individualizable_set_to_string - non-null")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.72/I101", "3-11a");
        REQUIRE(path != nullptr);

        auto* set = dnv_vista_sdk_gmod_path_individualizable_set_at(path, 0);
        REQUIRE(set != nullptr);

        char* str = dnv_vista_sdk_gmod_individualizable_set_to_string(set);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str }.size() > 0);

        dnv_vista_sdk_string_free(str);
        dnv_vista_sdk_gmod_individualizable_set_free(set);
        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_gmod_individualizable_set_build - consumes the set into a new path")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.72/I101", "3-11a");
        REQUIRE(path != nullptr);

        auto* set = dnv_vista_sdk_gmod_path_individualizable_set_at(path, 0);
        REQUIRE(set != nullptr);

        auto* built = dnv_vista_sdk_gmod_individualizable_set_build(set);
        REQUIRE(built != nullptr);

        dnv_vista_sdk_gmod_path_free(built);
        dnv_vista_sdk_gmod_individualizable_set_free(set);
        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_gmod_individualizable_set_create - null arguments return null")
    {
        int indices[] = { 0 };
        CHECK(dnv_vista_sdk_gmod_individualizable_set_create(nullptr, 1, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_individualizable_set_create(indices, 1, nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_individualizable_set_free - null is a no-op")
    {
        dnv_vista_sdk_gmod_individualizable_set_free(nullptr);
    }

    TEST_CASE("null set handle - accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_gmod_individualizable_set_build(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_individualizable_set_node_count(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_individualizable_set_node_at(nullptr, 0) == nullptr);
        CHECK(dnv_vista_sdk_gmod_individualizable_set_index_count(nullptr) == 0);

        int index;
        CHECK(dnv_vista_sdk_gmod_individualizable_set_index_at(nullptr, 0, &index) == 0);
        CHECK(dnv_vista_sdk_gmod_individualizable_set_location(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_individualizable_set_to_string(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_individualizable_set_set_location(nullptr, nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_individualizable_set_clear_location(nullptr) == 0);
    }

    TEST_CASE("dnv_vista_sdk_gmod_individualizable_set_set_location - assigns location to all nodes in the set")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.72/I101", "3-11a");
        REQUIRE(path != nullptr);

        auto* set = dnv_vista_sdk_gmod_path_individualizable_set_at(path, 0);
        REQUIRE(set != nullptr);

        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        REQUIRE(locations != nullptr);

        auto* b0 = dnv_vista_sdk_location_builder_create(locations);
        auto* b1 = dnv_vista_sdk_location_builder_with_number(b0, 3);
        auto* b2 = dnv_vista_sdk_location_builder_with_side(b1, 'P');
        auto* loc = dnv_vista_sdk_location_builder_build(b2);
        REQUIRE(loc != nullptr);

        CHECK(dnv_vista_sdk_gmod_individualizable_set_set_location(set, loc) == 1);

        auto* locAfter = dnv_vista_sdk_gmod_individualizable_set_location(set);
        REQUIRE(locAfter != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_location_value(locAfter) } == "3P");

        dnv_vista_sdk_location_free(locAfter);
        dnv_vista_sdk_location_free(loc);
        dnv_vista_sdk_location_builder_free(b0);
        dnv_vista_sdk_location_builder_free(b1);
        dnv_vista_sdk_location_builder_free(b2);
        dnv_vista_sdk_gmod_individualizable_set_free(set);
        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_gmod_individualizable_set_clear_location - removes location from all nodes")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1-1/C101.72/I101", "3-11a");
        REQUIRE(path != nullptr);

        auto* set = dnv_vista_sdk_gmod_path_individualizable_set_at(path, 0);
        REQUIRE(set != nullptr);

        auto* locBefore = dnv_vista_sdk_gmod_individualizable_set_location(set);
        REQUIRE(locBefore != nullptr);
        dnv_vista_sdk_location_free(locBefore);

        CHECK(dnv_vista_sdk_gmod_individualizable_set_clear_location(set) == 1);

        auto* locAfter = dnv_vista_sdk_gmod_individualizable_set_location(set);
        CHECK(locAfter == nullptr);

        dnv_vista_sdk_gmod_individualizable_set_free(set);
        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_gmod_individualizable_set_set_location - null location returns 0")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.72/I101", "3-11a");
        REQUIRE(path != nullptr);

        auto* set = dnv_vista_sdk_gmod_path_individualizable_set_at(path, 0);
        REQUIRE(set != nullptr);

        CHECK(dnv_vista_sdk_gmod_individualizable_set_set_location(set, nullptr) == 0);

        dnv_vista_sdk_gmod_individualizable_set_free(set);
        dnv_vista_sdk_gmod_path_free(path);
    }
}
