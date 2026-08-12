#include <doctest/doctest.h>

#include <dnv/VistaSDK_c_api.h>

#include <string_view>

TEST_SUITE("c-api::local_id_query_builder")
{
    TEST_CASE("dnv_vista_sdk_local_id_query_builder_create - returns non-null")
    {
        auto* builder = dnv_vista_sdk_local_id_query_builder_create();

        REQUIRE(builder != nullptr);

        dnv_vista_sdk_local_id_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_from - null localId returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_local_id_query_builder_from(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_from_string - valid string returns non-null")
    {
        auto* builder =
            dnv_vista_sdk_local_id_query_builder_from_string("/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power");

        REQUIRE(builder != nullptr);

        dnv_vista_sdk_local_id_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_from_string - invalid string returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_local_id_query_builder_from_string("not-a-local-id") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_from_string - null returns null")
    {
        CHECK(dnv_vista_sdk_local_id_query_builder_from_string(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_with_primary_item - match without locations required")
    {
        const auto* vis = dnv_vista_sdk_vis_instance();
        const auto* gmod = dnv_vista_sdk_vis_gmod(vis, "3-4a");
        REQUIRE(gmod != nullptr);
        const auto* locations = dnv_vista_sdk_vis_locations(vis, "3-4a");
        REQUIRE(locations != nullptr);

        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.31-1", "3-4a");
        REQUIRE(path != nullptr);

        auto* b0 = dnv_vista_sdk_local_id_query_builder_create();
        auto* b1 = dnv_vista_sdk_local_id_query_builder_with_primary_item(b0, path);
        REQUIRE(b1 != nullptr);
        auto* query = dnv_vista_sdk_local_id_query_builder_build(b1);
        REQUIRE(query != nullptr);

        auto* localIdSameLoc = dnv_vista_sdk_local_id_from_string(
            "/dnv-v2/vis-3-4a/411.1/C101.31-1/meta/qty-temperature/cnt-exhaust.gas/pos-inlet");
        REQUIRE(localIdSameLoc != nullptr);
        CHECK(dnv_vista_sdk_local_id_query_match(query, localIdSameLoc) == 1);

        auto* localIdDiffLoc = dnv_vista_sdk_local_id_from_string(
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet");
        REQUIRE(localIdDiffLoc != nullptr);
        CHECK(dnv_vista_sdk_local_id_query_match(query, localIdDiffLoc) == 0);

        dnv_vista_sdk_local_id_free(localIdSameLoc);
        dnv_vista_sdk_local_id_free(localIdDiffLoc);
        dnv_vista_sdk_local_id_query_free(query);
        dnv_vista_sdk_local_id_query_builder_free(b0);
        dnv_vista_sdk_local_id_query_builder_free(b1);
        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_with_primary_item - null arguments return null")
    {
        auto* builder = dnv_vista_sdk_local_id_query_builder_create();
        REQUIRE(builder != nullptr);

        CHECK(dnv_vista_sdk_local_id_query_builder_with_primary_item(nullptr, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_with_primary_item(builder, nullptr) == nullptr);

        dnv_vista_sdk_local_id_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_with_primary_item_query - matches with locations ignored")
    {
        auto* basePath = dnv_vista_sdk_gmod_path_from_short_path_version("411.1-2/C101.63/S206", "3-4a");
        REQUIRE(basePath != nullptr);

        auto* pb0 = dnv_vista_sdk_gmod_path_query_builder_from(basePath);
        auto* pb1 = dnv_vista_sdk_gmod_path_query_builder_without_locations(pb0);
        REQUIRE(pb1 != nullptr);
        auto* primaryQuery = dnv_vista_sdk_gmod_path_query_builder_build(pb1);
        REQUIRE(primaryQuery != nullptr);

        auto* b0 = dnv_vista_sdk_local_id_query_builder_create();
        auto* b1 = dnv_vista_sdk_local_id_query_builder_with_primary_item_query(b0, primaryQuery);
        REQUIRE(b1 != nullptr);
        auto* query = dnv_vista_sdk_local_id_query_builder_build(b1);
        REQUIRE(query != nullptr);

        auto* localId = dnv_vista_sdk_local_id_from_string(
            "/dnv-v2/vis-3-4a/411.1-1/C101.63/S206/~propulsion.engine/~cooling.system/meta/qty-temperature/"
            "cnt-exhaust.gas/pos-inlet");
        REQUIRE(localId != nullptr);
        CHECK(dnv_vista_sdk_local_id_query_match(query, localId) == 1);

        dnv_vista_sdk_local_id_free(localId);
        dnv_vista_sdk_local_id_query_free(query);
        dnv_vista_sdk_local_id_query_builder_free(b0);
        dnv_vista_sdk_local_id_query_builder_free(b1);
        dnv_vista_sdk_gmod_path_query_free(primaryQuery);
        dnv_vista_sdk_gmod_path_query_builder_free(pb0);
        dnv_vista_sdk_gmod_path_query_builder_free(pb1);
        dnv_vista_sdk_gmod_path_free(basePath);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_with_primary_item_query - null arguments return null")
    {
        auto* builder = dnv_vista_sdk_local_id_query_builder_create();
        REQUIRE(builder != nullptr);

        CHECK(dnv_vista_sdk_local_id_query_builder_with_primary_item_query(nullptr, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_with_primary_item_query(builder, nullptr) == nullptr);

        dnv_vista_sdk_local_id_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_with_secondary_item_query - matches secondary without locations")
    {
        auto* basePath = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101.31-2", "3-4a");
        REQUIRE(basePath != nullptr);

        auto* pb0 = dnv_vista_sdk_gmod_path_query_builder_from(basePath);
        auto* pb1 = dnv_vista_sdk_gmod_path_query_builder_without_locations(pb0);
        REQUIRE(pb1 != nullptr);
        auto* secondaryQuery = dnv_vista_sdk_gmod_path_query_builder_build(pb1);
        REQUIRE(secondaryQuery != nullptr);

        auto* b0 = dnv_vista_sdk_local_id_query_builder_create();
        auto* b1 = dnv_vista_sdk_local_id_query_builder_with_secondary_item_query(b0, secondaryQuery);
        REQUIRE(b1 != nullptr);
        auto* query = dnv_vista_sdk_local_id_query_builder_build(b1);
        REQUIRE(query != nullptr);

        auto* localId = dnv_vista_sdk_local_id_from_string(
            "/dnv-v2/vis-3-4a/411.1/C101.63/S206/sec/411.1/C101.31-5/~propulsion.engine/~cooling.system"
            "/~for.propulsion.engine/~cylinder.5/meta/qty-temperature/cnt-exhaust.gas/pos-inlet");
        REQUIRE(localId != nullptr);
        CHECK(dnv_vista_sdk_local_id_query_match(query, localId) == 1);

        dnv_vista_sdk_local_id_free(localId);
        dnv_vista_sdk_local_id_query_free(query);
        dnv_vista_sdk_local_id_query_builder_free(b0);
        dnv_vista_sdk_local_id_query_builder_free(b1);
        dnv_vista_sdk_gmod_path_query_free(secondaryQuery);
        dnv_vista_sdk_gmod_path_query_builder_free(pb0);
        dnv_vista_sdk_gmod_path_query_builder_free(pb1);
        dnv_vista_sdk_gmod_path_free(basePath);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_with_secondary_item - null arguments return null")
    {
        auto* builder = dnv_vista_sdk_local_id_query_builder_create();
        REQUIRE(builder != nullptr);

        CHECK(dnv_vista_sdk_local_id_query_builder_with_secondary_item(nullptr, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_with_secondary_item(builder, nullptr) == nullptr);

        dnv_vista_sdk_local_id_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_with_tags - MetadataTagsQuery overload matches by content")
    {
        auto* tb0 = dnv_vista_sdk_metadata_tags_query_builder_create();
        auto* tb1 =
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(tb0, DNV_VISTA_SDK_CODEBOOK_NAME_CONTENT, "sea.water");
        REQUIRE(tb1 != nullptr);
        auto* tagsQuery = dnv_vista_sdk_metadata_tags_query_builder_build(tb1);
        REQUIRE(tagsQuery != nullptr);

        auto* b0 = dnv_vista_sdk_local_id_query_builder_create();
        auto* b1 = dnv_vista_sdk_local_id_query_builder_with_tags(b0, tagsQuery);
        REQUIRE(b1 != nullptr);
        auto* query = dnv_vista_sdk_local_id_query_builder_build(b1);
        REQUIRE(query != nullptr);

        auto* localId = dnv_vista_sdk_local_id_from_string(
            "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened");
        REQUIRE(localId != nullptr);
        CHECK(dnv_vista_sdk_local_id_query_match(query, localId) == 1);

        dnv_vista_sdk_local_id_free(localId);
        dnv_vista_sdk_local_id_query_free(query);
        dnv_vista_sdk_local_id_query_builder_free(b0);
        dnv_vista_sdk_local_id_query_builder_free(b1);
        dnv_vista_sdk_metadata_tags_query_free(tagsQuery);
        dnv_vista_sdk_metadata_tags_query_builder_free(tb0);
        dnv_vista_sdk_metadata_tags_query_builder_free(tb1);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_with_tags - null arguments return null")
    {
        auto* builder = dnv_vista_sdk_local_id_query_builder_create();
        REQUIRE(builder != nullptr);

        CHECK(dnv_vista_sdk_local_id_query_builder_with_tags(nullptr, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_with_tags(builder, nullptr) == nullptr);

        dnv_vista_sdk_local_id_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_without_locations - strips locations from primary item path")
    {
        auto* primaryPath = dnv_vista_sdk_gmod_path_from_short_path_version("1036.13i-2/C662", "3-4a");
        REQUIRE(primaryPath != nullptr);

        auto* localId = dnv_vista_sdk_local_id_from_string(
            "/dnv-v2/vis-3-4a/1036.13i-1/C662/sec/411.1-2/C101/meta/qty-pressure/cnt-cargo"
            "/state-high.high/pos-stage-3/detail-discharge");
        REQUIRE(localId != nullptr);

        auto* withLoc0 = dnv_vista_sdk_local_id_query_builder_create();
        auto* withLoc1 = dnv_vista_sdk_local_id_query_builder_with_primary_item(withLoc0, primaryPath);
        REQUIRE(withLoc1 != nullptr);
        auto* withLocQuery = dnv_vista_sdk_local_id_query_builder_build(withLoc1);
        REQUIRE(withLocQuery != nullptr);
        CHECK(dnv_vista_sdk_local_id_query_match(withLocQuery, localId) == 0);

        auto* withoutLoc0 = dnv_vista_sdk_local_id_query_builder_create();
        auto* withoutLoc1 = dnv_vista_sdk_local_id_query_builder_with_primary_item(withoutLoc0, primaryPath);
        REQUIRE(withoutLoc1 != nullptr);
        auto* withoutLoc2 = dnv_vista_sdk_local_id_query_builder_without_locations(withoutLoc1);
        REQUIRE(withoutLoc2 != nullptr);
        auto* withoutLocQuery = dnv_vista_sdk_local_id_query_builder_build(withoutLoc2);
        REQUIRE(withoutLocQuery != nullptr);
        CHECK(dnv_vista_sdk_local_id_query_match(withoutLocQuery, localId) == 1);

        dnv_vista_sdk_local_id_free(localId);
        dnv_vista_sdk_local_id_query_free(withLocQuery);
        dnv_vista_sdk_local_id_query_free(withoutLocQuery);
        dnv_vista_sdk_local_id_query_builder_free(withLoc0);
        dnv_vista_sdk_local_id_query_builder_free(withLoc1);
        dnv_vista_sdk_local_id_query_builder_free(withoutLoc0);
        dnv_vista_sdk_local_id_query_builder_free(withoutLoc1);
        dnv_vista_sdk_local_id_query_builder_free(withoutLoc2);
        dnv_vista_sdk_gmod_path_free(primaryPath);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_without_locations - null builder returns null")
    {
        CHECK(dnv_vista_sdk_local_id_query_builder_without_locations(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_primary_item/secondary_item - Path-variant queries return paths")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(
            "/dnv-v2/vis-3-4a/1036.13i-1/C662/sec/411.1-2/C101/meta/qty-pressure/cnt-cargo"
            "/state-high.high/pos-stage-3/detail-discharge");
        REQUIRE(localId != nullptr);

        auto* builder = dnv_vista_sdk_local_id_query_builder_from(localId);
        REQUIRE(builder != nullptr);

        const auto* primary = dnv_vista_sdk_local_id_query_builder_primary_item(builder);
        REQUIRE(primary != nullptr);
        const auto* secondary = dnv_vista_sdk_local_id_query_builder_secondary_item(builder);
        REQUIRE(secondary != nullptr);

        dnv_vista_sdk_local_id_query_builder_free(builder);
        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_primary_item - unset returns null")
    {
        auto* builder = dnv_vista_sdk_local_id_query_builder_create();
        REQUIRE(builder != nullptr);

        CHECK(dnv_vista_sdk_local_id_query_builder_primary_item(builder) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_secondary_item(builder) == nullptr);

        dnv_vista_sdk_local_id_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_build - null builder returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_local_id_query_builder_build(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_builder_free - null is a no-op")
    {
        dnv_vista_sdk_local_id_query_builder_free(nullptr);
    }

    TEST_CASE("null builder handle - accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_local_id_query_builder_with_primary_item(nullptr, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_with_primary_item_query(nullptr, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_with_secondary_item(nullptr, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_with_secondary_item_query(nullptr, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_with_any_secondary_item(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_without_secondary_item(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_with_tags(nullptr, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_without_locations(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_primary_item(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_secondary_item(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_query_builder_build(nullptr) == nullptr);
    }
}
