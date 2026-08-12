#include <doctest/doctest.h>

#include <dnv/VistaSDK_c_api.h>

#include <string_view>

TEST_SUITE("c-api::gmod_path_query_builder")
{
    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_create - returns non-null")
    {
        auto* builder = dnv_vista_sdk_gmod_path_query_builder_create();

        REQUIRE(builder != nullptr);

        dnv_vista_sdk_gmod_path_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_from - null path returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_gmod_path_query_builder_from(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_from - path builder always matches itself")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1-1/C101", "3-4a");
        REQUIRE(path != nullptr);

        auto* builder = dnv_vista_sdk_gmod_path_query_builder_from(path);
        REQUIRE(builder != nullptr);
        auto* query = dnv_vista_sdk_gmod_path_query_builder_build(builder);
        REQUIRE(query != nullptr);

        CHECK(dnv_vista_sdk_gmod_path_query_match(query, path) == 1);

        dnv_vista_sdk_gmod_path_query_free(query);
        dnv_vista_sdk_gmod_path_query_builder_free(builder);
        dnv_vista_sdk_gmod_path_free(path);
        (void)vis;
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_path - returns the base path (Path variant only)")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1-1/C101", "3-4a");
        REQUIRE(path != nullptr);

        auto* builder = dnv_vista_sdk_gmod_path_query_builder_from(path);
        REQUIRE(builder != nullptr);

        const auto* basePath = dnv_vista_sdk_gmod_path_query_builder_path(builder);
        REQUIRE(basePath != nullptr);

        dnv_vista_sdk_gmod_path_query_builder_free(builder);
        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_path - Nodes-variant handle returns null and sets last error")
    {
        auto* builder = dnv_vista_sdk_gmod_path_query_builder_create();
        REQUIRE(builder != nullptr);

        CHECK(dnv_vista_sdk_gmod_path_query_builder_path(builder) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_gmod_path_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_path_with_node_all_locations - matches with any location")
    {
        auto* basePath = dnv_vista_sdk_gmod_path_from_short_path_version("411.1-1/C101", "3-4a");
        REQUIRE(basePath != nullptr);

        auto* b0 = dnv_vista_sdk_gmod_path_query_builder_from(basePath);
        auto* b1 = dnv_vista_sdk_gmod_path_query_builder_path_with_node_all_locations(b0, "411.1", 1);
        REQUIRE(b1 != nullptr);

        auto* query = dnv_vista_sdk_gmod_path_query_builder_build(b1);
        REQUIRE(query != nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_match(query, basePath) == 1);

        dnv_vista_sdk_gmod_path_query_free(query);
        dnv_vista_sdk_gmod_path_query_builder_free(b0);
        dnv_vista_sdk_gmod_path_query_builder_free(b1);
        dnv_vista_sdk_gmod_path_free(basePath);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_path_with_node_all_locations - unknown code returns null")
    {
        auto* basePath = dnv_vista_sdk_gmod_path_from_short_path_version("411.1-1/C101", "3-4a");
        REQUIRE(basePath != nullptr);

        auto* builder = dnv_vista_sdk_gmod_path_query_builder_from(basePath);
        REQUIRE(builder != nullptr);

        CHECK(dnv_vista_sdk_gmod_path_query_builder_path_with_node_all_locations(builder, "not-a-code", 1) == nullptr);

        dnv_vista_sdk_gmod_path_query_builder_free(builder);
        dnv_vista_sdk_gmod_path_free(basePath);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_path_with_node_locations - matches on specific location")
    {
        auto* basePath = dnv_vista_sdk_gmod_path_from_short_path_version("411.1-1/C101", "3-4a");
        REQUIRE(basePath != nullptr);

        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-4a");
        REQUIRE(locations != nullptr);
        auto* location1 = dnv_vista_sdk_locations_parse(locations, "1");
        REQUIRE(location1 != nullptr);

        const dnv_vista_sdk_location_t* locs[] = { location1 };

        auto* b0 = dnv_vista_sdk_gmod_path_query_builder_from(basePath);
        auto* b1 = dnv_vista_sdk_gmod_path_query_builder_path_with_node_locations(b0, "411.1", locs, 1);
        REQUIRE(b1 != nullptr);

        auto* query = dnv_vista_sdk_gmod_path_query_builder_build(b1);
        REQUIRE(query != nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_match(query, basePath) == 1);

        dnv_vista_sdk_gmod_path_query_free(query);
        dnv_vista_sdk_gmod_path_query_builder_free(b0);
        dnv_vista_sdk_gmod_path_query_builder_free(b1);
        dnv_vista_sdk_location_free(location1);
        dnv_vista_sdk_gmod_path_free(basePath);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_path_with_node_locations - mismatched location does not match")
    {
        auto* basePath = dnv_vista_sdk_gmod_path_from_short_path_version("411.1-1/C101", "3-4a");
        REQUIRE(basePath != nullptr);

        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-4a");
        REQUIRE(locations != nullptr);
        auto* locationA = dnv_vista_sdk_locations_parse(locations, "A");
        REQUIRE(locationA != nullptr);

        const dnv_vista_sdk_location_t* locs[] = { locationA };

        auto* b0 = dnv_vista_sdk_gmod_path_query_builder_from(basePath);
        auto* b1 = dnv_vista_sdk_gmod_path_query_builder_path_with_node_locations(b0, "411.1", locs, 1);
        REQUIRE(b1 != nullptr);

        auto* query = dnv_vista_sdk_gmod_path_query_builder_build(b1);
        REQUIRE(query != nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_match(query, basePath) == 0);

        dnv_vista_sdk_gmod_path_query_free(query);
        dnv_vista_sdk_gmod_path_query_builder_free(b0);
        dnv_vista_sdk_gmod_path_query_builder_free(b1);
        dnv_vista_sdk_location_free(locationA);
        dnv_vista_sdk_gmod_path_free(basePath);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_with_any_node_before - ignores parent nodes")
    {
        auto* basePath = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.31", "3-9a");
        REQUIRE(basePath != nullptr);

        auto* b0 = dnv_vista_sdk_gmod_path_query_builder_from(basePath);
        auto* b1 = dnv_vista_sdk_gmod_path_query_builder_with_any_node_before(b0, "C101");
        REQUIRE(b1 != nullptr);
        auto* b2 = dnv_vista_sdk_gmod_path_query_builder_without_locations(b1);
        REQUIRE(b2 != nullptr);

        auto* query = dnv_vista_sdk_gmod_path_query_builder_build(b2);
        REQUIRE(query != nullptr);

        CHECK(dnv_vista_sdk_gmod_path_query_match(query, basePath) == 1);

        auto* pathDiffParent = dnv_vista_sdk_gmod_path_from_short_path_version("511.11/C101.31", "3-9a");
        REQUIRE(pathDiffParent != nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_match(query, pathDiffParent) == 1);

        auto* pathDiffCNode = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C102.31", "3-9a");
        REQUIRE(pathDiffCNode != nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_match(query, pathDiffCNode) == 0);

        dnv_vista_sdk_gmod_path_free(pathDiffParent);
        dnv_vista_sdk_gmod_path_free(pathDiffCNode);
        dnv_vista_sdk_gmod_path_query_free(query);
        dnv_vista_sdk_gmod_path_query_builder_free(b0);
        dnv_vista_sdk_gmod_path_query_builder_free(b1);
        dnv_vista_sdk_gmod_path_query_builder_free(b2);
        dnv_vista_sdk_gmod_path_free(basePath);
    }

    TEST_CASE(
        "dnv_vista_sdk_gmod_path_query_builder_with_any_node_before - unknown code returns null and sets last error")
    {
        auto* basePath = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.63/S206", "3-4a");
        REQUIRE(basePath != nullptr);

        auto* builder = dnv_vista_sdk_gmod_path_query_builder_from(basePath);
        REQUIRE(builder != nullptr);

        CHECK(dnv_vista_sdk_gmod_path_query_builder_with_any_node_before(builder, "C101.31") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_gmod_path_query_builder_free(builder);
        dnv_vista_sdk_gmod_path_free(basePath);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_with_any_node_after - prefix matching ignores children")
    {
        auto* basePath = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.31", "3-4a");
        REQUIRE(basePath != nullptr);

        auto* b0 = dnv_vista_sdk_gmod_path_query_builder_from(basePath);
        auto* b1 = dnv_vista_sdk_gmod_path_query_builder_without_locations(b0);
        REQUIRE(b1 != nullptr);
        auto* b2 = dnv_vista_sdk_gmod_path_query_builder_with_any_node_after(b1, "411.1");
        REQUIRE(b2 != nullptr);

        auto* query = dnv_vista_sdk_gmod_path_query_builder_build(b2);
        REQUIRE(query != nullptr);

        auto* pathC10131 = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.31-2", "3-4a");
        REQUIRE(pathC10131 != nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_match(query, pathC10131) == 1);

        auto* path511 = dnv_vista_sdk_gmod_path_from_short_path_version("511.11/C101.63/S206", "3-4a");
        REQUIRE(path511 != nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_match(query, path511) == 0);

        dnv_vista_sdk_gmod_path_free(pathC10131);
        dnv_vista_sdk_gmod_path_free(path511);
        dnv_vista_sdk_gmod_path_query_free(query);
        dnv_vista_sdk_gmod_path_query_builder_free(b0);
        dnv_vista_sdk_gmod_path_query_builder_free(b1);
        dnv_vista_sdk_gmod_path_query_builder_free(b2);
        dnv_vista_sdk_gmod_path_free(basePath);
    }

    TEST_CASE(
        "dnv_vista_sdk_gmod_path_query_builder_with_any_node_after - unknown code returns null and does not mutate")
    {
        auto* basePath = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.63/S206", "3-4a");
        REQUIRE(basePath != nullptr);

        auto* b0 = dnv_vista_sdk_gmod_path_query_builder_from(basePath);
        auto* b1 = dnv_vista_sdk_gmod_path_query_builder_without_locations(b0);
        REQUIRE(b1 != nullptr);

        CHECK(dnv_vista_sdk_gmod_path_query_builder_with_any_node_after(b1, "C101.31") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        // original builder still valid and matches base path
        auto* query = dnv_vista_sdk_gmod_path_query_builder_build(b1);
        REQUIRE(query != nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_match(query, basePath) == 1);

        dnv_vista_sdk_gmod_path_query_free(query);
        dnv_vista_sdk_gmod_path_query_builder_free(b0);
        dnv_vista_sdk_gmod_path_query_builder_free(b1);
        dnv_vista_sdk_gmod_path_free(basePath);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_without_locations - Nodes-variant handle returns null")
    {
        auto* builder = dnv_vista_sdk_gmod_path_query_builder_create();
        REQUIRE(builder != nullptr);

        CHECK(dnv_vista_sdk_gmod_path_query_builder_without_locations(builder) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_gmod_path_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_with_node_all_locations - Nodes-variant builds a matching query")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-4a");
        REQUIRE(gmod != nullptr);
        const auto* node = dnv_vista_sdk_gmod_get_node(gmod, "411.1");
        REQUIRE(node != nullptr);

        auto* b0 = dnv_vista_sdk_gmod_path_query_builder_create();
        auto* b1 = dnv_vista_sdk_gmod_path_query_builder_with_node_all_locations(b0, node, 1);
        REQUIRE(b1 != nullptr);

        auto* query = dnv_vista_sdk_gmod_path_query_builder_build(b1);
        REQUIRE(query != nullptr);

        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1-1/C101", "3-4a");
        REQUIRE(path != nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_match(query, path) == 1);

        dnv_vista_sdk_gmod_path_free(path);
        dnv_vista_sdk_gmod_path_query_free(query);
        dnv_vista_sdk_gmod_path_query_builder_free(b0);
        dnv_vista_sdk_gmod_path_query_builder_free(b1);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_with_node_all_locations - Path-variant handle returns null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-4a");
        REQUIRE(gmod != nullptr);
        const auto* node = dnv_vista_sdk_gmod_get_node(gmod, "411.1");
        REQUIRE(node != nullptr);

        auto* basePath = dnv_vista_sdk_gmod_path_from_short_path_version("411.1-1/C101", "3-4a");
        REQUIRE(basePath != nullptr);
        auto* builder = dnv_vista_sdk_gmod_path_query_builder_from(basePath);
        REQUIRE(builder != nullptr);

        CHECK(dnv_vista_sdk_gmod_path_query_builder_with_node_all_locations(builder, node, 1) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_gmod_path_query_builder_free(builder);
        dnv_vista_sdk_gmod_path_free(basePath);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_with_node_locations - Nodes-variant matches on location")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-4a");
        REQUIRE(gmod != nullptr);
        const auto* node = dnv_vista_sdk_gmod_get_node(gmod, "411.1");
        REQUIRE(node != nullptr);

        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-4a");
        REQUIRE(locations != nullptr);
        auto* location1 = dnv_vista_sdk_locations_parse(locations, "1");
        REQUIRE(location1 != nullptr);
        const dnv_vista_sdk_location_t* locs[] = { location1 };

        auto* b0 = dnv_vista_sdk_gmod_path_query_builder_create();
        auto* b1 = dnv_vista_sdk_gmod_path_query_builder_with_node_locations(b0, node, locs, 1);
        REQUIRE(b1 != nullptr);

        auto* query = dnv_vista_sdk_gmod_path_query_builder_build(b1);
        REQUIRE(query != nullptr);

        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1-1/C101", "3-4a");
        REQUIRE(path != nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_match(query, path) == 1);

        dnv_vista_sdk_gmod_path_free(path);
        dnv_vista_sdk_location_free(location1);
        dnv_vista_sdk_gmod_path_query_free(query);
        dnv_vista_sdk_gmod_path_query_builder_free(b0);
        dnv_vista_sdk_gmod_path_query_builder_free(b1);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_build - null builder returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_gmod_path_query_builder_build(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_builder_free - null is a no-op")
    {
        dnv_vista_sdk_gmod_path_query_builder_free(nullptr);
    }

    TEST_CASE("null builder handle - accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_gmod_path_query_builder_path(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_builder_path_with_node_all_locations(nullptr, "x", 1) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_builder_path_with_node_locations(nullptr, "x", nullptr, 0) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_builder_with_any_node_before(nullptr, "x") == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_builder_with_any_node_after(nullptr, "x") == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_builder_without_locations(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_builder_with_node_all_locations(nullptr, nullptr, 1) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_builder_with_node_locations(nullptr, nullptr, nullptr, 0) == nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_builder_build(nullptr) == nullptr);
    }
}
