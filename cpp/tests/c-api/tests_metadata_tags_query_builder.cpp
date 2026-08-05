#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/local_id.h>
#include <dnv/vista/sdk/c/query/metadata_tags_query.h>
#include <dnv/vista/sdk/c/query/metadata_tags_query_builder.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::metadata_tags_query_builder")
{
    TEST_CASE("dnv_vista_sdk_metadata_tags_query_builder_create - returns non-null")
    {
        auto* builder = dnv_vista_sdk_metadata_tags_query_builder_create();

        REQUIRE(builder != nullptr);

        dnv_vista_sdk_metadata_tags_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_builder_with_tag - null arguments return null")
    {
        auto* builder = dnv_vista_sdk_metadata_tags_query_builder_create();
        REQUIRE(builder != nullptr);

        CHECK(
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(
                nullptr, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY, "temperature") == nullptr);
        CHECK(
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(
                builder, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY, nullptr) == nullptr);

        dnv_vista_sdk_metadata_tags_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags - null builder returns null")
    {
        CHECK(dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags(nullptr, 1) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_builder_build - null builder returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_metadata_tags_query_builder_build(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_builder - fluent chain builds a matching query")
    {
        auto* b0 = dnv_vista_sdk_metadata_tags_query_builder_create();
        auto* b1 =
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(b0, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY, "temperature");
        REQUIRE(b1 != nullptr);
        auto* b2 =
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(b1, DNV_VISTA_SDK_CODEBOOK_NAME_CONTENT, "exhaust.gas");
        REQUIRE(b2 != nullptr);
        auto* b3 = dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags(b2, 0);
        REQUIRE(b3 != nullptr);

        auto* query = dnv_vista_sdk_metadata_tags_query_builder_build(b3);
        REQUIRE(query != nullptr);

        auto* localId =
            dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas");
        REQUIRE(localId != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId) == 1);

        dnv_vista_sdk_local_id_free(localId);
        dnv_vista_sdk_metadata_tags_query_free(query);
        dnv_vista_sdk_metadata_tags_query_builder_free(b0);
        dnv_vista_sdk_metadata_tags_query_builder_free(b1);
        dnv_vista_sdk_metadata_tags_query_builder_free(b2);
        dnv_vista_sdk_metadata_tags_query_builder_free(b3);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_builder_with_metadata_tag - matches using an existing tag")
    {
        auto* source = dnv_vista_sdk_local_id_from_string(
            "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened");
        REQUIRE(source != nullptr);

        const auto* tag = dnv_vista_sdk_local_id_metadata_tag(source, DNV_VISTA_SDK_CODEBOOK_NAME_CONTENT);
        REQUIRE(tag != nullptr);

        auto* b0 = dnv_vista_sdk_metadata_tags_query_builder_create();
        auto* b1 = dnv_vista_sdk_metadata_tags_query_builder_with_metadata_tag(b0, tag);
        REQUIRE(b1 != nullptr);

        auto* query = dnv_vista_sdk_metadata_tags_query_builder_build(b1);
        REQUIRE(query != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, source) == 1);

        dnv_vista_sdk_metadata_tags_query_free(query);
        dnv_vista_sdk_metadata_tags_query_builder_free(b0);
        dnv_vista_sdk_metadata_tags_query_builder_free(b1);
        dnv_vista_sdk_local_id_free(source);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_builder_with_metadata_tag - null arguments return null")
    {
        auto* builder = dnv_vista_sdk_metadata_tags_query_builder_create();
        REQUIRE(builder != nullptr);

        CHECK(dnv_vista_sdk_metadata_tags_query_builder_with_metadata_tag(nullptr, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_builder_with_metadata_tag(builder, nullptr) == nullptr);

        dnv_vista_sdk_metadata_tags_query_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_builder_from - subset mode matches LocalId with extra tags")
    {
        auto* source =
            dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas");
        REQUIRE(source != nullptr);

        auto* builder = dnv_vista_sdk_metadata_tags_query_builder_from(source, 1);
        REQUIRE(builder != nullptr);

        auto* query = dnv_vista_sdk_metadata_tags_query_builder_build(builder);
        REQUIRE(query != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, source) == 1);

        auto* localId2 = dnv_vista_sdk_local_id_from_string(
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet");
        REQUIRE(localId2 != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId2) == 1);

        dnv_vista_sdk_local_id_free(localId2);
        dnv_vista_sdk_metadata_tags_query_free(query);
        dnv_vista_sdk_metadata_tags_query_builder_free(builder);
        dnv_vista_sdk_local_id_free(source);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_builder_from - exact mode rejects LocalId with extra tags")
    {
        auto* source =
            dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas");
        REQUIRE(source != nullptr);

        auto* builder = dnv_vista_sdk_metadata_tags_query_builder_from(source, 0);
        REQUIRE(builder != nullptr);

        auto* query = dnv_vista_sdk_metadata_tags_query_builder_build(builder);
        REQUIRE(query != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, source) == 1);

        auto* localId2 = dnv_vista_sdk_local_id_from_string(
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet");
        REQUIRE(localId2 != nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query, localId2) == 0);

        dnv_vista_sdk_local_id_free(localId2);
        dnv_vista_sdk_metadata_tags_query_free(query);
        dnv_vista_sdk_metadata_tags_query_builder_free(builder);
        dnv_vista_sdk_local_id_free(source);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_builder_from - null localId returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_metadata_tags_query_builder_from(nullptr, 1) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_builder - immutability: withTag returns a new, independent handle")
    {
        auto* builder1 = dnv_vista_sdk_metadata_tags_query_builder_create();
        auto* builder2 = dnv_vista_sdk_metadata_tags_query_builder_with_tag(
            builder1, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY, "temperature");
        REQUIRE(builder2 != nullptr);
        CHECK(builder1 != builder2);

        auto* query1 = dnv_vista_sdk_metadata_tags_query_builder_build(builder1);
        auto* query2 = dnv_vista_sdk_metadata_tags_query_builder_build(builder2);
        REQUIRE(query1 != nullptr);
        REQUIRE(query2 != nullptr);

        auto* localId = dnv_vista_sdk_local_id_from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature");
        REQUIRE(localId != nullptr);

        CHECK(dnv_vista_sdk_metadata_tags_query_match(query1, localId) == 1);
        CHECK(dnv_vista_sdk_metadata_tags_query_match(query2, localId) == 1);

        dnv_vista_sdk_local_id_free(localId);
        dnv_vista_sdk_metadata_tags_query_free(query1);
        dnv_vista_sdk_metadata_tags_query_free(query2);
        dnv_vista_sdk_metadata_tags_query_builder_free(builder1);
        dnv_vista_sdk_metadata_tags_query_builder_free(builder2);
    }

    TEST_CASE("dnv_vista_sdk_metadata_tags_query_builder_free - null is a no-op")
    {
        dnv_vista_sdk_metadata_tags_query_builder_free(nullptr);
    }

    TEST_CASE("null builder handle - accessors fail gracefully")
    {
        CHECK(
            dnv_vista_sdk_metadata_tags_query_builder_with_tag(nullptr, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY, "x") ==
            nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_builder_with_metadata_tag(nullptr, nullptr) == nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags(nullptr, 1) == nullptr);
        CHECK(dnv_vista_sdk_metadata_tags_query_builder_build(nullptr) == nullptr);
    }
}
