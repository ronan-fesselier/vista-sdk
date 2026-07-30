#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/location.h>
#include <dnv/vista/sdk/c/core/locations.h>
#include <dnv/vista/sdk/c/core/parsing_errors.h>
#include <dnv/vista/sdk/c/core/relative_location.h>
#include <dnv/vista/sdk/c/core/vis.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::locations")
{
    TEST_CASE("dnv_vista_sdk_vis_locations - valid version returns non-null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");

        CHECK(locations != nullptr);
    }

    TEST_CASE("dnv_vista_sdk_vis_locations - invalid version returns null and sets last error")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();

        CHECK(dnv_vista_sdk_vis_locations(vis, "not-a-version") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_vis_locations - null arguments return null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();

        CHECK(dnv_vista_sdk_vis_locations(nullptr, "3-11a") == nullptr);
        CHECK(dnv_vista_sdk_vis_locations(vis, nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_locations_parse - valid string returns matching location")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");

        auto* location = dnv_vista_sdk_locations_parse(locations, "11FIPU");

        REQUIRE(location != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_location_value(location) } == "11FIPU");

        dnv_vista_sdk_location_free(location);
    }

    TEST_CASE("dnv_vista_sdk_locations_parse - invalid string returns null and sets last error")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");

        CHECK(dnv_vista_sdk_locations_parse(locations, "not-a-location") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_locations_parse - null arguments return null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");

        CHECK(dnv_vista_sdk_locations_parse(nullptr, "11FIPU") == nullptr);
        CHECK(dnv_vista_sdk_locations_parse(locations, nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_locations_parse_with_errors - valid string yields empty errors")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;

        auto* location = dnv_vista_sdk_locations_parse_with_errors(locations, "11FIPU", &errors);

        REQUIRE(location != nullptr);
        REQUIRE(errors != nullptr);
        CHECK(dnv_vista_sdk_parsing_errors_has_errors(errors) == 0);

        dnv_vista_sdk_location_free(location);
        dnv_vista_sdk_parsing_errors_free(errors);
    }

    TEST_CASE("dnv_vista_sdk_locations_parse_with_errors - invalid string yields non-empty errors")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;

        auto* location = dnv_vista_sdk_locations_parse_with_errors(locations, "not-a-location", &errors);

        CHECK(location == nullptr);
        REQUIRE(errors != nullptr);
        CHECK(dnv_vista_sdk_parsing_errors_has_errors(errors) == 1);
        CHECK(dnv_vista_sdk_parsing_errors_count(errors) > 0);

        const auto* type = dnv_vista_sdk_parsing_errors_type_at(errors, 0);
        REQUIRE(type != nullptr);
        CHECK(std::string_view{ type }.size() > 0);

        dnv_vista_sdk_parsing_errors_free(errors);
    }

    TEST_CASE("dnv_vista_sdk_locations_version - returns the requested VIS version")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");

        CHECK(std::string_view{ dnv_vista_sdk_locations_version(locations) } == "3-11a");
    }

    TEST_CASE("dnv_vista_sdk_locations_version - null locations returns null")
    {
        CHECK(dnv_vista_sdk_locations_version(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_locations_relative_location_count/at - covers all entries")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");

        const size_t count = dnv_vista_sdk_locations_relative_location_count(locations);
        REQUIRE(count > 0);

        const auto* first = dnv_vista_sdk_locations_relative_location_at(locations, 0);
        REQUIRE(first != nullptr);
        CHECK(dnv_vista_sdk_relative_location_code(first) != '\0');
    }

    TEST_CASE("dnv_vista_sdk_locations_relative_location_at - out of range index returns null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        const size_t count = dnv_vista_sdk_locations_relative_location_count(locations);

        CHECK(dnv_vista_sdk_locations_relative_location_at(locations, count) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_locations_relative_location_count/at - null locations return 0/null")
    {
        CHECK(dnv_vista_sdk_locations_relative_location_count(nullptr) == 0);
        CHECK(dnv_vista_sdk_locations_relative_location_at(nullptr, 0) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_locations_group_count/at - side group has entries")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");

        const size_t count = dnv_vista_sdk_locations_group_count(locations, DNV_VISTA_SDK_LOCATION_GROUP_SIDE);
        REQUIRE(count > 0);

        const auto* first = dnv_vista_sdk_locations_group_at(locations, DNV_VISTA_SDK_LOCATION_GROUP_SIDE, 0);
        REQUIRE(first != nullptr);
    }

    TEST_CASE("dnv_vista_sdk_locations_group_at - out of range index returns null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        const size_t count = dnv_vista_sdk_locations_group_count(locations, DNV_VISTA_SDK_LOCATION_GROUP_SIDE);

        CHECK(dnv_vista_sdk_locations_group_at(locations, DNV_VISTA_SDK_LOCATION_GROUP_SIDE, count) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_locations_group_count/at - null locations return 0/null")
    {
        CHECK(dnv_vista_sdk_locations_group_count(nullptr, DNV_VISTA_SDK_LOCATION_GROUP_SIDE) == 0);
        CHECK(dnv_vista_sdk_locations_group_at(nullptr, DNV_VISTA_SDK_LOCATION_GROUP_SIDE, 0) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_relative_location - accessors reflect a real entry")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        const auto* entry = dnv_vista_sdk_locations_relative_location_at(locations, 0);
        REQUIRE(entry != nullptr);

        CHECK(dnv_vista_sdk_relative_location_code(entry) != '\0');
        CHECK(dnv_vista_sdk_relative_location_name(entry) != nullptr);

        const auto* location = dnv_vista_sdk_relative_location_location(entry);
        REQUIRE(location != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_location_value(location) }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_relative_location - null handle accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_relative_location_code(nullptr) == '\0');
        CHECK(dnv_vista_sdk_relative_location_name(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_relative_location_definition(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_relative_location_location(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_location_free - null is a no-op")
    {
        dnv_vista_sdk_location_free(nullptr);
    }

    TEST_CASE("dnv_vista_sdk_location_value - null location returns null")
    {
        CHECK(dnv_vista_sdk_location_value(nullptr) == nullptr);
    }
}
