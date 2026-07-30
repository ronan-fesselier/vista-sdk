#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/location.h>
#include <dnv/vista/sdk/c/core/location_builder.h>
#include <dnv/vista/sdk/c/core/locations.h>
#include <dnv/vista/sdk/c/core/vis.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::location_builder")
{
    TEST_CASE("dnv_vista_sdk_location_builder_create - valid locations returns non-null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");

        auto* builder = dnv_vista_sdk_location_builder_create(locations);

        CHECK(builder != nullptr);

        dnv_vista_sdk_location_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_location_builder_create - null locations returns null")
    {
        CHECK(dnv_vista_sdk_location_builder_create(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_location_builder - fluent chain builds matching location")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");

        auto* b0 = dnv_vista_sdk_location_builder_create(locations);
        auto* b1 = dnv_vista_sdk_location_builder_with_number(b0, 11);
        auto* b2 = dnv_vista_sdk_location_builder_with_side(b1, 'P');
        auto* b3 = dnv_vista_sdk_location_builder_with_transverse(b2, 'I');
        auto* b4 = dnv_vista_sdk_location_builder_with_longitudinal(b3, 'F');
        auto* b5 = dnv_vista_sdk_location_builder_with_vertical(b4, 'U');

        REQUIRE(b5 != nullptr);

        auto* location = dnv_vista_sdk_location_builder_build(b5);
        REQUIRE(location != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_location_value(location) } == "11FIPU");

        dnv_vista_sdk_location_free(location);
        dnv_vista_sdk_location_builder_free(b0);
        dnv_vista_sdk_location_builder_free(b1);
        dnv_vista_sdk_location_builder_free(b2);
        dnv_vista_sdk_location_builder_free(b3);
        dnv_vista_sdk_location_builder_free(b4);
        dnv_vista_sdk_location_builder_free(b5);
    }

    TEST_CASE("dnv_vista_sdk_location_builder - with* is immutable (does not mutate the input)")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");

        auto* base = dnv_vista_sdk_location_builder_create(locations);
        auto* withNum = dnv_vista_sdk_location_builder_with_number(base, 11);

        CHECK(base != withNum);

        char* baseStr = dnv_vista_sdk_location_builder_to_string(base);
        REQUIRE(baseStr != nullptr);
        CHECK(std::string_view{ baseStr } == "");

        dnv_vista_sdk_location_builder_string_free(baseStr);
        dnv_vista_sdk_location_builder_free(base);
        dnv_vista_sdk_location_builder_free(withNum);
    }

    TEST_CASE("dnv_vista_sdk_location_builder_with_side - invalid character throws and sets last error")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        auto* builder = dnv_vista_sdk_location_builder_create(locations);

        CHECK(dnv_vista_sdk_location_builder_with_side(builder, 'A') == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_location_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_location_builder_with_number - invalid number throws and sets last error")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        auto* builder = dnv_vista_sdk_location_builder_create(locations);

        CHECK(dnv_vista_sdk_location_builder_with_number(builder, 0) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_location_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_location_builder_without_number - clears the number component")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        auto* base = dnv_vista_sdk_location_builder_create(locations);
        auto* withNum = dnv_vista_sdk_location_builder_with_number(base, 11);
        auto* withoutNum = dnv_vista_sdk_location_builder_without_number(withNum);

        char* str = dnv_vista_sdk_location_builder_to_string(withoutNum);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "");

        dnv_vista_sdk_location_builder_string_free(str);
        dnv_vista_sdk_location_builder_free(base);
        dnv_vista_sdk_location_builder_free(withNum);
        dnv_vista_sdk_location_builder_free(withoutNum);
    }

    TEST_CASE("dnv_vista_sdk_location_builder_with_code - auto-detects group and matches with_side")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        auto* base = dnv_vista_sdk_location_builder_create(locations);

        auto* viaCode = dnv_vista_sdk_location_builder_with_code(base, 'P');
        auto* viaSide = dnv_vista_sdk_location_builder_with_side(base, 'P');

        REQUIRE(viaCode != nullptr);
        REQUIRE(viaSide != nullptr);

        char* codeStr = dnv_vista_sdk_location_builder_to_string(viaCode);
        char* sideStr = dnv_vista_sdk_location_builder_to_string(viaSide);
        REQUIRE(codeStr != nullptr);
        REQUIRE(sideStr != nullptr);
        CHECK(std::string_view{ codeStr } == std::string_view{ sideStr });

        dnv_vista_sdk_location_builder_string_free(codeStr);
        dnv_vista_sdk_location_builder_string_free(sideStr);
        dnv_vista_sdk_location_builder_free(base);
        dnv_vista_sdk_location_builder_free(viaCode);
        dnv_vista_sdk_location_builder_free(viaSide);
    }

    TEST_CASE("dnv_vista_sdk_location_builder_with_location - reconstructs matching builder")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        auto* location = dnv_vista_sdk_locations_parse(locations, "11FIPU");
        REQUIRE(location != nullptr);

        auto* base = dnv_vista_sdk_location_builder_create(locations);
        auto* builder = dnv_vista_sdk_location_builder_with_location(base, location);

        REQUIRE(builder != nullptr);

        char* str = dnv_vista_sdk_location_builder_to_string(builder);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "11FIPU");

        dnv_vista_sdk_location_builder_string_free(str);
        dnv_vista_sdk_location_free(location);
        dnv_vista_sdk_location_builder_free(base);
        dnv_vista_sdk_location_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_location_builder_with_location - null location returns null")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        auto* base = dnv_vista_sdk_location_builder_create(locations);

        CHECK(dnv_vista_sdk_location_builder_with_location(base, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_location_builder_with_location(nullptr, nullptr) == nullptr);

        dnv_vista_sdk_location_builder_free(base);
    }

    TEST_CASE("dnv_vista_sdk_location_builder_without_value - clears the targeted component")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        auto* location = dnv_vista_sdk_locations_parse(locations, "11FIPU");
        REQUIRE(location != nullptr);

        auto* base = dnv_vista_sdk_location_builder_create(locations);
        auto* full = dnv_vista_sdk_location_builder_with_location(base, location);
        auto* cleared = dnv_vista_sdk_location_builder_without_value(full, DNV_VISTA_SDK_LOCATION_GROUP_SIDE);

        REQUIRE(cleared != nullptr);

        char side;
        CHECK(dnv_vista_sdk_location_builder_side(cleared, &side) == 0);

        dnv_vista_sdk_location_free(location);
        dnv_vista_sdk_location_builder_free(base);
        dnv_vista_sdk_location_builder_free(full);
        dnv_vista_sdk_location_builder_free(cleared);
    }

    TEST_CASE("dnv_vista_sdk_location_builder_without_value - null builder returns null")
    {
        CHECK(dnv_vista_sdk_location_builder_without_value(nullptr, DNV_VISTA_SDK_LOCATION_GROUP_SIDE) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_location_builder component getters - reflect the fluent chain")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");

        auto* base = dnv_vista_sdk_location_builder_create(locations);
        auto* full = dnv_vista_sdk_location_builder_with_number(base, 11);
        auto* tmp = dnv_vista_sdk_location_builder_with_side(full, 'P');
        dnv_vista_sdk_location_builder_free(full);
        full = tmp;
        tmp = dnv_vista_sdk_location_builder_with_transverse(full, 'I');
        dnv_vista_sdk_location_builder_free(full);
        full = tmp;
        tmp = dnv_vista_sdk_location_builder_with_longitudinal(full, 'F');
        dnv_vista_sdk_location_builder_free(full);
        full = tmp;
        tmp = dnv_vista_sdk_location_builder_with_vertical(full, 'U');
        dnv_vista_sdk_location_builder_free(full);
        full = tmp;

        int number;
        char side, transverse, longitudinal, vertical;

        REQUIRE(dnv_vista_sdk_location_builder_number(full, &number) == 1);
        CHECK(number == 11);
        REQUIRE(dnv_vista_sdk_location_builder_side(full, &side) == 1);
        CHECK(side == 'P');
        REQUIRE(dnv_vista_sdk_location_builder_transverse(full, &transverse) == 1);
        CHECK(transverse == 'I');
        REQUIRE(dnv_vista_sdk_location_builder_longitudinal(full, &longitudinal) == 1);
        CHECK(longitudinal == 'F');
        REQUIRE(dnv_vista_sdk_location_builder_vertical(full, &vertical) == 1);
        CHECK(vertical == 'U');

        dnv_vista_sdk_location_builder_free(base);
        dnv_vista_sdk_location_builder_free(full);
    }

    TEST_CASE("dnv_vista_sdk_location_builder component getters - unset components return 0")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-11a");
        auto* builder = dnv_vista_sdk_location_builder_create(locations);

        int number;
        char side, transverse, longitudinal, vertical;

        CHECK(dnv_vista_sdk_location_builder_number(builder, &number) == 0);
        CHECK(dnv_vista_sdk_location_builder_side(builder, &side) == 0);
        CHECK(dnv_vista_sdk_location_builder_transverse(builder, &transverse) == 0);
        CHECK(dnv_vista_sdk_location_builder_longitudinal(builder, &longitudinal) == 0);
        CHECK(dnv_vista_sdk_location_builder_vertical(builder, &vertical) == 0);

        dnv_vista_sdk_location_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_location_builder_free - null is a no-op")
    {
        dnv_vista_sdk_location_builder_free(nullptr);
    }

    TEST_CASE("null builder handle - all accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_location_builder_with_number(nullptr, 1) == nullptr);
        CHECK(dnv_vista_sdk_location_builder_without_number(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_location_builder_with_side(nullptr, 'P') == nullptr);
        CHECK(dnv_vista_sdk_location_builder_without_side(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_location_builder_with_vertical(nullptr, 'U') == nullptr);
        CHECK(dnv_vista_sdk_location_builder_without_vertical(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_location_builder_with_transverse(nullptr, 'I') == nullptr);
        CHECK(dnv_vista_sdk_location_builder_without_transverse(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_location_builder_with_longitudinal(nullptr, 'F') == nullptr);
        CHECK(dnv_vista_sdk_location_builder_without_longitudinal(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_location_builder_with_code(nullptr, 'P') == nullptr);
        CHECK(dnv_vista_sdk_location_builder_build(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_location_builder_to_string(nullptr) == nullptr);
    }
}
