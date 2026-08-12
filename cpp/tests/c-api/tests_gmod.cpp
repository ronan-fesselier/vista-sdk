#include <doctest/doctest.h>

#include <dnv/VistaSDK_c_api.h>

#include <string_view>

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
}
