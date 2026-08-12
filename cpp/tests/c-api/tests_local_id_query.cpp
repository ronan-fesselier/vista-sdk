#include <doctest/doctest.h>

#include <dnv/VistaSDK_c_api.h>

#include <string_view>

TEST_SUITE("c-api::local_id_query")
{
    TEST_CASE("dnv_vista_sdk_local_id_query_match - empty query matches all LocalIds")
    {
        auto* builder = dnv_vista_sdk_local_id_query_builder_create();
        REQUIRE(builder != nullptr);
        auto* query = dnv_vista_sdk_local_id_query_builder_build(builder);
        REQUIRE(query != nullptr);

        auto* localId1 = dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power");
        REQUIRE(localId1 != nullptr);
        CHECK(dnv_vista_sdk_local_id_query_match(query, localId1) == 1);

        auto* localId2 = dnv_vista_sdk_local_id_from_string(
            "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened");
        REQUIRE(localId2 != nullptr);
        CHECK(dnv_vista_sdk_local_id_query_match(query, localId2) == 1);

        dnv_vista_sdk_local_id_free(localId1);
        dnv_vista_sdk_local_id_free(localId2);
        dnv_vista_sdk_local_id_query_free(query);
        dnv_vista_sdk_local_id_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_match - from LocalId builds an exact-match query")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power");
        REQUIRE(localId != nullptr);

        auto* builder = dnv_vista_sdk_local_id_query_builder_from(localId);
        REQUIRE(builder != nullptr);
        auto* query = dnv_vista_sdk_local_id_query_builder_build(builder);
        REQUIRE(query != nullptr);

        CHECK(dnv_vista_sdk_local_id_query_match(query, localId) == 1);

        dnv_vista_sdk_local_id_query_free(query);
        dnv_vista_sdk_local_id_query_builder_free(builder);
        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_match_string - from LocalId string overload")
    {
        auto* builder =
            dnv_vista_sdk_local_id_query_builder_from_string("/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power");
        REQUIRE(builder != nullptr);
        auto* query = dnv_vista_sdk_local_id_query_builder_build(builder);
        REQUIRE(query != nullptr);

        CHECK(dnv_vista_sdk_local_id_query_match_string(query, "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power") == 1);
        CHECK(dnv_vista_sdk_local_id_query_match_string(query, "/dnv-v2/vis-3-4a/411.1/C102.31/meta/qty-power") == 0);

        dnv_vista_sdk_local_id_query_free(query);
        dnv_vista_sdk_local_id_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_match - secondary item presence: exact match requires same secondary")
    {
        auto* baseLocalId = dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-9a/411.1/C101.31/meta/qty-power");
        REQUIRE(baseLocalId != nullptr);

        auto* builder = dnv_vista_sdk_local_id_query_builder_from(baseLocalId);
        REQUIRE(builder != nullptr);
        auto* query = dnv_vista_sdk_local_id_query_builder_build(builder);
        REQUIRE(query != nullptr);
        CHECK(dnv_vista_sdk_local_id_query_match(query, baseLocalId) == 1);

        auto* otherLocalId =
            dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-9a/411.1/C101.31/sec/412.3/meta/qty-power");
        REQUIRE(otherLocalId != nullptr);
        CHECK(dnv_vista_sdk_local_id_query_match(query, otherLocalId) == 0);

        dnv_vista_sdk_local_id_free(otherLocalId);
        dnv_vista_sdk_local_id_query_free(query);
        dnv_vista_sdk_local_id_query_builder_free(builder);
        dnv_vista_sdk_local_id_free(baseLocalId);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_match - without_secondary_item rejects LocalIds that have secondary")
    {
        auto* baseLocalId = dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-9a/411.1/C101.31/meta/qty-power");
        REQUIRE(baseLocalId != nullptr);
        auto* otherLocalId =
            dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-9a/411.1/C101.31/sec/412.3/meta/qty-power");
        REQUIRE(otherLocalId != nullptr);

        auto* b0 = dnv_vista_sdk_local_id_query_builder_from(baseLocalId);
        auto* b1 = dnv_vista_sdk_local_id_query_builder_without_secondary_item(b0);
        REQUIRE(b1 != nullptr);
        auto* query = dnv_vista_sdk_local_id_query_builder_build(b1);
        REQUIRE(query != nullptr);

        CHECK(dnv_vista_sdk_local_id_query_match(query, otherLocalId) == 0);

        dnv_vista_sdk_local_id_free(otherLocalId);
        dnv_vista_sdk_local_id_query_free(query);
        dnv_vista_sdk_local_id_query_builder_free(b0);
        dnv_vista_sdk_local_id_query_builder_free(b1);
        dnv_vista_sdk_local_id_free(baseLocalId);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_match - with_any_secondary_item accepts either")
    {
        auto* baseLocalId = dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-9a/411.1/C101.31/meta/qty-power");
        REQUIRE(baseLocalId != nullptr);
        auto* otherLocalId =
            dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-9a/411.1/C101.31/sec/412.3/meta/qty-power");
        REQUIRE(otherLocalId != nullptr);

        auto* b0 = dnv_vista_sdk_local_id_query_builder_from(baseLocalId);
        auto* b1 = dnv_vista_sdk_local_id_query_builder_with_any_secondary_item(b0);
        REQUIRE(b1 != nullptr);
        auto* query = dnv_vista_sdk_local_id_query_builder_build(b1);
        REQUIRE(query != nullptr);

        CHECK(dnv_vista_sdk_local_id_query_match(query, baseLocalId) == 1);
        CHECK(dnv_vista_sdk_local_id_query_match(query, otherLocalId) == 1);

        dnv_vista_sdk_local_id_free(otherLocalId);
        dnv_vista_sdk_local_id_query_free(query);
        dnv_vista_sdk_local_id_query_builder_free(b0);
        dnv_vista_sdk_local_id_query_builder_free(b1);
        dnv_vista_sdk_local_id_free(baseLocalId);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_match - null arguments return 0 and set last error")
    {
        auto* builder = dnv_vista_sdk_local_id_query_builder_create();
        REQUIRE(builder != nullptr);
        auto* query = dnv_vista_sdk_local_id_query_builder_build(builder);
        REQUIRE(query != nullptr);

        auto* localId = dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-power");
        REQUIRE(localId != nullptr);

        CHECK(dnv_vista_sdk_local_id_query_match(nullptr, localId) == 0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
        CHECK(dnv_vista_sdk_local_id_query_match(query, nullptr) == 0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_local_id_free(localId);
        dnv_vista_sdk_local_id_query_free(query);
        dnv_vista_sdk_local_id_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_match_string - null arguments return 0 and set last error")
    {
        auto* builder = dnv_vista_sdk_local_id_query_builder_create();
        REQUIRE(builder != nullptr);
        auto* query = dnv_vista_sdk_local_id_query_builder_build(builder);
        REQUIRE(query != nullptr);

        CHECK(dnv_vista_sdk_local_id_query_match_string(nullptr, "/dnv-v2/vis-3-4a/411.1/meta/qty-power") == 0);
        CHECK(dnv_vista_sdk_local_id_query_match_string(query, nullptr) == 0);

        dnv_vista_sdk_local_id_query_free(query);
        dnv_vista_sdk_local_id_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_local_id_query_free - null is a no-op")
    {
        dnv_vista_sdk_local_id_query_free(nullptr);
    }
}
