#include <doctest/doctest.h>

#include <dnv/VistaSDK_c_api.h>

#include <string_view>

TEST_SUITE("c-api::gmod_node_metadata")
{
    TEST_CASE("category/type/full_type/name - non-null and consistent for root node")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* root = dnv_vista_sdk_gmod_root_node(gmod);
        const auto* metadata = dnv_vista_sdk_gmod_node_metadata(root);
        REQUIRE(metadata != nullptr);

        const auto* category = dnv_vista_sdk_gmod_node_metadata_category(metadata);
        const auto* type = dnv_vista_sdk_gmod_node_metadata_type(metadata);
        const auto* fullType = dnv_vista_sdk_gmod_node_metadata_full_type(metadata);
        const auto* name = dnv_vista_sdk_gmod_node_metadata_name(metadata);

        REQUIRE(category != nullptr);
        REQUIRE(type != nullptr);
        REQUIRE(fullType != nullptr);
        REQUIRE(name != nullptr);
        CHECK(std::string_view{ fullType }.find(category) != std::string_view::npos);
    }

    TEST_CASE("null metadata - all accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_gmod_node_metadata_category(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_node_metadata_type(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_node_metadata_full_type(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_node_metadata_name(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_node_metadata_common_name(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_node_metadata_definition(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_gmod_node_metadata_common_definition(nullptr) == nullptr);

        int flag;
        CHECK(dnv_vista_sdk_gmod_node_metadata_install_substructure(nullptr, &flag) == 0);
        CHECK(dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_count(nullptr) == 0);
        CHECK(dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_key_at(nullptr, 0) == nullptr);
        CHECK(dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_value_at(nullptr, 0) == nullptr);
    }

    TEST_CASE("normal_assignment_name_count/key_at/value_at - out of range on empty map")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-11a");
        const auto* root = dnv_vista_sdk_gmod_root_node(gmod);
        const auto* metadata = dnv_vista_sdk_gmod_node_metadata(root);

        const size_t count = dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_count(metadata);
        CHECK(dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_key_at(metadata, count) == nullptr);
        CHECK(dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_value_at(metadata, count) == nullptr);
    }
}
