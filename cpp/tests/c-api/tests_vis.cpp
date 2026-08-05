#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/gmod.h>
#include <dnv/vista/sdk/c/core/gmod_path.h>
#include <dnv/vista/sdk/c/core/local_id.h>
#include <dnv/vista/sdk/c/core/local_id_builder.h>
#include <dnv/vista/sdk/c/core/vis.h>
#include <dnv/vista/sdk/c/common.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::vis")
{
    TEST_CASE("dnv_vista_sdk_vis_instance - returns non-null and is stable across calls")
    {
        const auto* first = dnv_vista_sdk_vis_instance();
        const auto* second = dnv_vista_sdk_vis_instance();

        CHECK(first != nullptr);
        CHECK(first == second);
    }

    TEST_CASE("dnv_vista_sdk_vis_latest - returns a non-empty version string")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();

        CHECK(std::string_view{ dnv_vista_sdk_vis_latest(vis) }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_vis_latest - null vis returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_vis_latest(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_vis_version_count - returns a positive count")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();

        CHECK(dnv_vista_sdk_vis_version_count(vis) > 0);
    }

    TEST_CASE("dnv_vista_sdk_vis_version_count - null vis returns 0")
    {
        CHECK(dnv_vista_sdk_vis_version_count(nullptr) == 0);
    }

    TEST_CASE("dnv_vista_sdk_vis_version_at - valid index returns a non-empty version string")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* version = dnv_vista_sdk_vis_version_at(vis, 0);

        REQUIRE(version != nullptr);
        CHECK(std::string_view{ version }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_vis_version_at - last version matches latest")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const size_t count = dnv_vista_sdk_vis_version_count(vis);
        const auto* lastVersion = dnv_vista_sdk_vis_version_at(vis, count - 1);

        REQUIRE(lastVersion != nullptr);
        CHECK(std::string_view{ lastVersion } == std::string_view{ dnv_vista_sdk_vis_latest(vis) });
    }

    TEST_CASE("dnv_vista_sdk_vis_version_at - out of range index returns null and sets last error")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const size_t count = dnv_vista_sdk_vis_version_count(vis);

        CHECK(dnv_vista_sdk_vis_version_at(vis, count) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_vis_version_at - null vis returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_vis_version_at(nullptr, 0) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_vis_convert_node - converts a node across VIS versions")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-4a");
        REQUIRE(gmod != nullptr);

        const auto* node = dnv_vista_sdk_gmod_get_node(gmod, "C101.31");
        REQUIRE(node != nullptr);

        auto* converted = dnv_vista_sdk_vis_convert_node(vis, "3-4a", node, "3-5a");
        REQUIRE(converted != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_gmod_node_code(converted) } == "C101.31");

        dnv_vista_sdk_gmod_node_free(converted);
    }

    TEST_CASE("dnv_vista_sdk_vis_convert_node - null arguments return null and set last error")
    {
        CHECK(dnv_vista_sdk_vis_convert_node(nullptr, "3-4a", nullptr, "3-5a") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_vis_convert_path - converts a path across VIS versions")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.72/I101", "3-4a");
        REQUIRE(path != nullptr);

        auto* converted = dnv_vista_sdk_vis_convert_path(vis, "3-4a", path, "3-6a");
        REQUIRE(converted != nullptr);

        char* str = dnv_vista_sdk_gmod_path_to_string(converted);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "411.1/C101.72/I101");

        dnv_vista_sdk_string_free(str);
        dnv_vista_sdk_gmod_path_free(converted);
        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_vis_convert_path - null arguments return null and set last error")
    {
        CHECK(dnv_vista_sdk_vis_convert_path(nullptr, "3-4a", nullptr, "3-6a") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_vis_convert_local_id - converts a LocalId across VIS versions")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        constexpr const char* sourceLocalIdStr =
            "/dnv-v2/vis-3-4a/411.1/C101/sec/411.1/C101.64i/S201/meta/cnt-condensate";

        auto* localId = dnv_vista_sdk_local_id_from_string(sourceLocalIdStr);
        REQUIRE(localId != nullptr);

        auto* converted = dnv_vista_sdk_vis_convert_local_id(vis, localId, "3-5a");
        REQUIRE(converted != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_local_id_version(converted) } == "3-5a");

        dnv_vista_sdk_local_id_free(converted);
        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("dnv_vista_sdk_vis_convert_local_id - null arguments return null and set last error")
    {
        CHECK(dnv_vista_sdk_vis_convert_local_id(nullptr, nullptr, "3-5a") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_vis_convert_local_id_builder - converts a LocalIdBuilder across VIS versions")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        constexpr const char* sourceLocalIdStr =
            "/dnv-v2/vis-3-4a/411.1/C101/sec/411.1/C101.64i/S201/meta/cnt-condensate";

        auto* localId = dnv_vista_sdk_local_id_from_string(sourceLocalIdStr);
        REQUIRE(localId != nullptr);
        const auto* builder = dnv_vista_sdk_local_id_builder(localId);
        REQUIRE(builder != nullptr);

        auto* converted = dnv_vista_sdk_vis_convert_local_id_builder(vis, builder, "3-5a");
        REQUIRE(converted != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_local_id_builder_version(converted) } == "3-5a");

        dnv_vista_sdk_local_id_builder_free(converted);
        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("dnv_vista_sdk_vis_convert_local_id_builder - null arguments return null and set last error")
    {
        CHECK(dnv_vista_sdk_vis_convert_local_id_builder(nullptr, nullptr, "3-5a") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }
}
