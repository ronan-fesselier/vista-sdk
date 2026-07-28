#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/vis.h>
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
}
