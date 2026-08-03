#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/gmod.h>
#include <dnv/vista/sdk/c/core/vis.h>
#include <dnv/vista/sdk/c/error.h>

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

TEST_SUITE("c-api::gmod")
{
    TEST_CASE("dnv_vista_sdk_gmod_get - valid version returns non-null and is stable")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* first = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* second = dnv_vista_sdk_vis_gmod(vis, "3-11a");

        REQUIRE(first != nullptr);
        CHECK(first == second);
    }

    TEST_CASE("dnv_vista_sdk_gmod_get - invalid version returns null and sets last error")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();

        CHECK(dnv_vista_sdk_vis_gmod(vis, "not-a-version") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_gmod_get - null arguments return null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();

        CHECK(dnv_vista_sdk_vis_gmod(nullptr, "3-11a") == nullptr);
        CHECK(dnv_vista_sdk_vis_gmod(vis, nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_version - matches the requested version")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");

        CHECK(std::string_view{ dnv_vista_sdk_gmod_version(gmod) } == "3-11a");
    }

    TEST_CASE("dnv_vista_sdk_gmod_version - null gmod returns null")
    {
        CHECK(dnv_vista_sdk_gmod_version(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_root_node - returns node with code VE")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* root = dnv_vista_sdk_gmod_root_node(gmod);

        REQUIRE(root != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_gmod_node_code(root) } == "VE");
    }

    TEST_CASE("dnv_vista_sdk_gmod_root_node - null gmod returns null")
    {
        CHECK(dnv_vista_sdk_gmod_root_node(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_get_node - known code returns matching node")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* node = dnv_vista_sdk_gmod_get_node(gmod, "411.1");

        REQUIRE(node != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_gmod_node_code(node) } == "411.1");
    }

    TEST_CASE("dnv_vista_sdk_gmod_get_node - unknown code returns null and sets last error")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");

        CHECK(dnv_vista_sdk_gmod_get_node(gmod, "not-a-code") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_gmod_get_node - null arguments return null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");

        CHECK(dnv_vista_sdk_gmod_get_node(nullptr, "VE") == nullptr);
        CHECK(dnv_vista_sdk_gmod_get_node(gmod, nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_count/at - covers all nodes, including root")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");

        const size_t count = dnv_vista_sdk_gmod_node_count(gmod);
        REQUIRE(count > 0);

        bool foundRoot = false;
        for (size_t i = 0; i < count; ++i)
        {
            const auto* node = dnv_vista_sdk_gmod_node_at(gmod, i);
            REQUIRE(node != nullptr);
            if (std::string_view{ dnv_vista_sdk_gmod_node_code(node) } == "VE")
            {
                foundRoot = true;
            }
        }
        CHECK(foundRoot);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_at - out of range index returns null and sets last error")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const size_t count = dnv_vista_sdk_gmod_node_count(gmod);

        CHECK(dnv_vista_sdk_gmod_node_at(gmod, count) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_gmod_node_count/at - null gmod return 0/null")
    {
        CHECK(dnv_vista_sdk_gmod_node_count(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_node_at(nullptr, 0) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_gmod_is_product_type_assignment - valid pair returns 1")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-4a");
        const auto* parent = dnv_vista_sdk_gmod_get_node(gmod, "1241.31");
        const auto* child = dnv_vista_sdk_gmod_get_node(gmod, "C1013");

        REQUIRE(parent != nullptr);
        REQUIRE(child != nullptr);

        CHECK(dnv_vista_sdk_gmod_is_product_type_assignment(parent, child) == 1);
        CHECK(dnv_vista_sdk_gmod_is_product_type_assignment(child, parent) == 0);
        CHECK(dnv_vista_sdk_gmod_is_product_type_assignment(nullptr, child) == 0);
        CHECK(dnv_vista_sdk_gmod_is_product_type_assignment(parent, nullptr) == 0);
    }

    TEST_CASE("dnv_vista_sdk_gmod_is_product_selection_assignment - valid pair returns 1")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-4a");
        const auto* child = dnv_vista_sdk_gmod_get_node(gmod, "CS1");

        REQUIRE(child != nullptr);

        const size_t parentCount = dnv_vista_sdk_gmod_node_parent_count(child);
        REQUIRE(parentCount > 0);

        const auto* parent = dnv_vista_sdk_gmod_node_parent_at(child, 0);
        REQUIRE(parent != nullptr);

        CHECK(dnv_vista_sdk_gmod_is_product_selection_assignment(parent, child) == 1);
        CHECK(dnv_vista_sdk_gmod_is_product_selection_assignment(child, parent) == 0);
        CHECK(dnv_vista_sdk_gmod_is_product_selection_assignment(nullptr, child) == 0);
        CHECK(dnv_vista_sdk_gmod_is_product_selection_assignment(parent, nullptr) == 0);
    }

    TEST_CASE("dnv_vista_sdk_gmod_traverse - visits root and stops early")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-4a");

        struct State
        {
            std::vector<std::string> visited;
        } state;

        auto handler = [](const dnv_vista_sdk_gmod_node_t* const* parents,
                          size_t parent_count,
                          const dnv_vista_sdk_gmod_node_t* node,
                          void* userdata) -> dnv_vista_sdk_traversal_result_t {
            (void)parents;
            (void)parent_count;
            auto* s = static_cast<State*>(userdata);
            s->visited.emplace_back(dnv_vista_sdk_gmod_node_code(node));
            return s->visited.size() >= 20 ? DNV_VISTA_SDK_TRAVERSAL_STOP : DNV_VISTA_SDK_TRAVERSAL_CONTINUE;
        };

        const int completed = dnv_vista_sdk_gmod_traverse(gmod, handler, 1, &state);

        CHECK(completed == 0);
        CHECK(std::any_of(state.visited.begin(), state.visited.end(), [](const auto& c) { return c == "VE"; }));
        CHECK(state.visited.size() == 20);
    }

    TEST_CASE("dnv_vista_sdk_gmod_traverse - skips subtree")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-4a");

        struct State
        {
            std::vector<std::string> visited;
        } state;

        auto handler = [](const dnv_vista_sdk_gmod_node_t* const* parents,
                          size_t parent_count,
                          const dnv_vista_sdk_gmod_node_t* node,
                          void* userdata) -> dnv_vista_sdk_traversal_result_t {
            (void)parents;
            (void)parent_count;
            auto* s = static_cast<State*>(userdata);
            std::string c{ dnv_vista_sdk_gmod_node_code(node) };
            s->visited.push_back(c);
            return c == "400a" ? DNV_VISTA_SDK_TRAVERSAL_SKIP_SUBTREE : DNV_VISTA_SDK_TRAVERSAL_CONTINUE;
        };

        dnv_vista_sdk_gmod_traverse(gmod, handler, 1, &state);

        CHECK(std::none_of(state.visited.begin(), state.visited.end(), [](const auto& c) { return c == "410"; }));
        CHECK(std::any_of(state.visited.begin(), state.visited.end(), [](const auto& c) { return c == "VE"; }));
        CHECK(std::any_of(state.visited.begin(), state.visited.end(), [](const auto& c) { return c == "500a"; }));
    }
}
