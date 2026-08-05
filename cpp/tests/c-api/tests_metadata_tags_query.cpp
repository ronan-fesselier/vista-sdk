#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/local_id.h>
#include <dnv/vista/sdk/c/query/metadata_tags_query.h>
#include <dnv/vista/sdk/c/query/metadata_tags_query_builder.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::metadata_tags_query")
{
    TEST_CASE("dnv_vista_sdk_metadata_tags_query_match - empty query matches any LocalId")
    {
        auto* builder = dnv_vista_sdk_metadata_tags_query_builder_create();
        REQUIRE(builder != nullptr);
        auto* query = dnv_vista_sdk_metadata_tags_query_builder_build(builder);
        REQUIRE(query != nullptr);

        auto* localId1 = dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature");
        REQUIRE(localId1 != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId1) == 1);

        auto* localId2 =
            dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas");
        REQUIRE(localId2 != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId2) == 1);

        dnv_vista_sdk_local_id_free(localId1);
        dnv_vista_sdk_local_id_free(localId2);
        dnv_vista_sdk_metadata_tags_query_free(query);
        dnv_vista_sdk_metadata_tags_query_builder_free(builder);
    }

    TEST_CASE(
        "dnv_vista_sdk_metadata_tags_query_match - empty query with exact mode matches only LocalIds with no tags")
    {
        auto* b0 = dnv_vista_sdk_metadata_tags_query_builder_create();
        auto* b1 = dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags(b0, 0);
        REQUIRE(b1 != nullptr);
        auto* query = dnv_vista_sdk_metadata_tags_query_builder_build(b1);
        REQUIRE(query != nullptr);

        auto* withTags = dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature");
        REQUIRE(withTags != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, withTags) == 0);

        dnv_vista_sdk_local_id_free(withTags);
        dnv_vista_sdk_metadata_tags_query_free(query);
        dnv_vista_sdk_metadata_tags_query_builder_free(b0);
        dnv_vista_sdk_metadata_tags_query_builder_free(b1);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_match - single tag subset mode")
    {
        auto* b0 = dnv_vista_sdk_metadata_tags_query_builder_create();
        auto* b1 =
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(b0, DNV_VISTA_SDK_CODEBOOK_NAME_CONTENT, "sea.water");
        REQUIRE(b1 != nullptr);
        auto* b2 = dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags(b1, 1);
        REQUIRE(b2 != nullptr);
        auto* query = dnv_vista_sdk_metadata_tags_query_builder_build(b2);
        REQUIRE(query != nullptr);

        auto* localId1 = dnv_vista_sdk_local_id_from_string(
            "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened");
        REQUIRE(localId1 != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId1) == 1);

        auto* localId2 =
            dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas");
        REQUIRE(localId2 != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId2) == 0);

        auto* localId3 = dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature");
        REQUIRE(localId3 != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId3) == 0);

        dnv_vista_sdk_local_id_free(localId1);
        dnv_vista_sdk_local_id_free(localId2);
        dnv_vista_sdk_local_id_free(localId3);
        dnv_vista_sdk_metadata_tags_query_free(query);
        dnv_vista_sdk_metadata_tags_query_builder_free(b0);
        dnv_vista_sdk_metadata_tags_query_builder_free(b1);
        dnv_vista_sdk_metadata_tags_query_builder_free(b2);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_match - single tag exact mode")
    {
        auto* b0 = dnv_vista_sdk_metadata_tags_query_builder_create();
        auto* b1 =
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(b0, DNV_VISTA_SDK_CODEBOOK_NAME_CONTENT, "exhaust.gas");
        REQUIRE(b1 != nullptr);
        auto* b2 = dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags(b1, 0);
        REQUIRE(b2 != nullptr);
        auto* query = dnv_vista_sdk_metadata_tags_query_builder_build(b2);
        REQUIRE(query != nullptr);

        auto* localId1 = dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/cnt-exhaust.gas");
        REQUIRE(localId1 != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId1) == 1);

        auto* localId2 =
            dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas");
        REQUIRE(localId2 != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId2) == 0);

        dnv_vista_sdk_local_id_free(localId1);
        dnv_vista_sdk_local_id_free(localId2);
        dnv_vista_sdk_metadata_tags_query_free(query);
        dnv_vista_sdk_metadata_tags_query_builder_free(b0);
        dnv_vista_sdk_metadata_tags_query_builder_free(b1);
        dnv_vista_sdk_metadata_tags_query_builder_free(b2);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_match - multiple tags subset mode")
    {
        auto* b0 = dnv_vista_sdk_metadata_tags_query_builder_create();
        auto* b1 =
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(b0, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY, "temperature");
        auto* b2 =
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(b1, DNV_VISTA_SDK_CODEBOOK_NAME_CONTENT, "exhaust.gas");
        auto* b3 = dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags(b2, 1);
        REQUIRE(b3 != nullptr);
        auto* query = dnv_vista_sdk_metadata_tags_query_builder_build(b3);
        REQUIRE(query != nullptr);

        auto* localId1 = dnv_vista_sdk_local_id_from_string(
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet");
        REQUIRE(localId1 != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId1) == 1);

        auto* localId2 = dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature");
        REQUIRE(localId2 != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId2) == 0);

        auto* localId3 =
            dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-sea.water");
        REQUIRE(localId3 != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId3) == 0);

        dnv_vista_sdk_local_id_free(localId1);
        dnv_vista_sdk_local_id_free(localId2);
        dnv_vista_sdk_local_id_free(localId3);
        dnv_vista_sdk_metadata_tags_query_free(query);
        dnv_vista_sdk_metadata_tags_query_builder_free(b0);
        dnv_vista_sdk_metadata_tags_query_builder_free(b1);
        dnv_vista_sdk_metadata_tags_query_builder_free(b2);
        dnv_vista_sdk_metadata_tags_query_builder_free(b3);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_match - multiple tags exact mode")
    {
        auto* b0 = dnv_vista_sdk_metadata_tags_query_builder_create();
        auto* b1 =
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(b0, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY, "temperature");
        auto* b2 =
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(b1, DNV_VISTA_SDK_CODEBOOK_NAME_CONTENT, "exhaust.gas");
        auto* b3 = dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags(b2, 0);
        REQUIRE(b3 != nullptr);
        auto* query = dnv_vista_sdk_metadata_tags_query_builder_build(b3);
        REQUIRE(query != nullptr);

        auto* localId1 =
            dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas");
        REQUIRE(localId1 != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId1) == 1);

        auto* localId2 = dnv_vista_sdk_local_id_from_string(
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet");
        REQUIRE(localId2 != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId2) == 0);

        dnv_vista_sdk_local_id_free(localId1);
        dnv_vista_sdk_local_id_free(localId2);
        dnv_vista_sdk_metadata_tags_query_free(query);
        dnv_vista_sdk_metadata_tags_query_builder_free(b0);
        dnv_vista_sdk_metadata_tags_query_builder_free(b1);
        dnv_vista_sdk_metadata_tags_query_builder_free(b2);
        dnv_vista_sdk_metadata_tags_query_builder_free(b3);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_match - chained withTag calls")
    {
        auto* b0 = dnv_vista_sdk_metadata_tags_query_builder_create();
        auto* b1 =
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(b0, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY, "temperature");
        auto* b2 =
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(b1, DNV_VISTA_SDK_CODEBOOK_NAME_CONTENT, "exhaust.gas");
        auto* b3 =
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(b2, DNV_VISTA_SDK_CODEBOOK_NAME_POSITION, "inlet");
        auto* b4 = dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags(b3, 0);
        REQUIRE(b4 != nullptr);
        auto* query = dnv_vista_sdk_metadata_tags_query_builder_build(b4);
        REQUIRE(query != nullptr);

        auto* localId =
            dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas/pos-inlet");
        REQUIRE(localId != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId) == 1);

        dnv_vista_sdk_local_id_free(localId);
        dnv_vista_sdk_metadata_tags_query_free(query);
        dnv_vista_sdk_metadata_tags_query_builder_free(b0);
        dnv_vista_sdk_metadata_tags_query_builder_free(b1);
        dnv_vista_sdk_metadata_tags_query_builder_free(b2);
        dnv_vista_sdk_metadata_tags_query_builder_free(b3);
        dnv_vista_sdk_metadata_tags_query_builder_free(b4);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_match - null arguments return 0 and set last error")
    {
        auto* builder = dnv_vista_sdk_metadata_tags_query_builder_create();
        REQUIRE(builder != nullptr);
        auto* query = dnv_vista_sdk_metadata_tags_query_builder_build(builder);
        REQUIRE(query != nullptr);

        auto* localId = dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature");
        REQUIRE(localId != nullptr);

        CHECK(dnv_vista_sdk_metadata_tags_query_match(nullptr, localId) == 0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, nullptr) == 0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_local_id_free(localId);
        dnv_vista_sdk_metadata_tags_query_free(query);
        dnv_vista_sdk_metadata_tags_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_builder - returns the originating builder")
    {
        auto* b0 = dnv_vista_sdk_metadata_tags_query_builder_create();
        auto* b1 =
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(b0, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY, "temperature");
        REQUIRE(b1 != nullptr);
        auto* query = dnv_vista_sdk_metadata_tags_query_builder_build(b1);
        REQUIRE(query != nullptr);

        const auto* builder = dnv_vista_sdk_metadata_tags_query_builder(query);
        REQUIRE(builder != nullptr);

        dnv_vista_sdk_metadata_tags_query_free(query);
        dnv_vista_sdk_metadata_tags_query_builder_free(b0);
        dnv_vista_sdk_metadata_tags_query_builder_free(b1);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_builder - null query returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_metadata_tags_query_builder(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_free - null is a no-op")
    {
        dnv_vista_sdk_metadata_tags_query_free(nullptr);
    }
}
