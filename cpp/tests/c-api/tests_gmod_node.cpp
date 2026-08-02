#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/gmod.h>
#include <dnv/vista/sdk/c/core/gmod_node.h>
#include <dnv/vista/sdk/c/core/vis.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::gmod_node")
{
    TEST_CASE("dnv_vista_sdk_gmod_node_version/code - root node")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* root = dnv_vista_sdk_gmod_root_node(gmod);

        CHECK(std::string_view{ dnv_vista_sdk_gmod_node_version(root) } == "3-11a");
        CHECK(std::string_view{ dnv_vista_sdk_gmod_node_code(root) } == "VE");
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_version/code - null node returns null")
    {
        CHECK(dnv_vista_sdk_gmod_node_version(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_node_code(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_location - node without location returns null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* root = dnv_vista_sdk_gmod_root_node(gmod);

        CHECK(dnv_vista_sdk_gmod_node_location(root) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_location - null node returns null")
    {
        CHECK(dnv_vista_sdk_gmod_node_location(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_metadata - non-null for a real node")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* root = dnv_vista_sdk_gmod_root_node(gmod);

        CHECK(dnv_vista_sdk_gmod_node_metadata(root) != nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_metadata - null node returns null")
    {
        CHECK(dnv_vista_sdk_gmod_node_metadata(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_child_count/at - root has children")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* root = dnv_vista_sdk_gmod_root_node(gmod);

        const size_t count = dnv_vista_sdk_gmod_node_child_count(root);
        REQUIRE(count > 0);

        const auto* child = dnv_vista_sdk_gmod_node_child_at(root, 0);
        REQUIRE(child != nullptr);
        CHECK(dnv_vista_sdk_gmod_node_code(child) != nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_child_at - out of range returns null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* root = dnv_vista_sdk_gmod_root_node(gmod);
        const size_t count = dnv_vista_sdk_gmod_node_child_count(root);

        CHECK(dnv_vista_sdk_gmod_node_child_at(root, count) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_child_count/at - null node return 0/null")
    {
        CHECK(dnv_vista_sdk_gmod_node_child_count(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_node_child_at(nullptr, 0) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_parent_count/at - child of root has root as parent")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* root = dnv_vista_sdk_gmod_root_node(gmod);
        const auto* child = dnv_vista_sdk_gmod_node_child_at(root, 0);
        REQUIRE(child != nullptr);

        const size_t parentCount = dnv_vista_sdk_gmod_node_parent_count(child);
        REQUIRE(parentCount > 0);

        bool foundRoot = false;
        for (size_t i = 0; i < parentCount; ++i)
        {
            const auto* parent = dnv_vista_sdk_gmod_node_parent_at(child, i);
            REQUIRE(parent != nullptr);
            if (std::string_view{ dnv_vista_sdk_gmod_node_code(parent) } == "VE")
            {
                foundRoot = true;
            }
        }
        CHECK(foundRoot);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_parent_count/at - null node return 0/null")
    {
        CHECK(dnv_vista_sdk_gmod_node_parent_count(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_node_parent_at(nullptr, 0) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_is_root - true for VE, false for a child")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* root = dnv_vista_sdk_gmod_root_node(gmod);
        const auto* child = dnv_vista_sdk_gmod_node_child_at(root, 0);

        CHECK(dnv_vista_sdk_gmod_node_is_root(root) == 1);
        CHECK(dnv_vista_sdk_gmod_node_is_root(child) == 0);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_is_child - root is parent of its own child")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* root = dnv_vista_sdk_gmod_root_node(gmod);
        const auto* child = dnv_vista_sdk_gmod_node_child_at(root, 0);

        CHECK(dnv_vista_sdk_gmod_node_is_child(root, child) == 1);
        CHECK(dnv_vista_sdk_gmod_node_is_child(child, root) == 0);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_is_child_code - matches the same child")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* root = dnv_vista_sdk_gmod_root_node(gmod);
        const auto* child = dnv_vista_sdk_gmod_node_child_at(root, 0);
        const auto* code = dnv_vista_sdk_gmod_node_code(child);

        CHECK(dnv_vista_sdk_gmod_node_is_child_code(root, code) == 1);
        CHECK(dnv_vista_sdk_gmod_node_is_child_code(root, "not-a-code") == 0);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_to_string - non-null for a real node")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* root = dnv_vista_sdk_gmod_root_node(gmod);

        char* str = dnv_vista_sdk_gmod_node_to_string(root);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "VE");

        dnv_vista_sdk_gmod_node_string_free(str);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_to_string - null node returns null")
    {
        CHECK(dnv_vista_sdk_gmod_node_to_string(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_string_free - null is a no-op")
    {
        dnv_vista_sdk_gmod_node_string_free(nullptr);
    }

    TEST_CASE("predicate functions - null node returns 0 for all")
    {
        CHECK(dnv_vista_sdk_gmod_node_is_function_composition(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_node_is_mappable(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_node_is_product_selection(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_node_is_product_type(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_node_is_asset(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_node_is_leaf_node(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_node_is_function_node(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_node_is_asset_function_node(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_node_is_root(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_node_product_type(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_node_product_selection(nullptr) == nullptr);
    }
}
