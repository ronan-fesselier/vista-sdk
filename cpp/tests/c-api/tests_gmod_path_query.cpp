#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/gmod_path.h>
#include <dnv/vista/sdk/c/query/gmod_path_query.h>
#include <dnv/vista/sdk/c/query/gmod_path_query_builder.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::gmod_path_query")
{
    TEST_CASE("dnv_vista_sdk_gmod_path_query_match - matches the base path it was built from")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101", "3-4a");
        REQUIRE(path != nullptr);

        auto* builder = dnv_vista_sdk_gmod_path_query_builder_from(path);
        REQUIRE(builder != nullptr);
        auto* query = dnv_vista_sdk_gmod_path_query_builder_build(builder);
        REQUIRE(query != nullptr);

        CHECK(dnv_vista_sdk_gmod_path_query_match(query, path) == 1);

        dnv_vista_sdk_gmod_path_query_free(query);
        dnv_vista_sdk_gmod_path_query_builder_free(builder);
        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_match - different path does not match a specific query")
    {
        auto* basePath = dnv_vista_sdk_gmod_path_from_short_path_version("411.1-1/C101", "3-4a");
        REQUIRE(basePath != nullptr);

        auto* builder = dnv_vista_sdk_gmod_path_query_builder_from(basePath);
        REQUIRE(builder != nullptr);
        auto* query = dnv_vista_sdk_gmod_path_query_builder_build(builder);
        REQUIRE(query != nullptr);

        auto* otherPath = dnv_vista_sdk_gmod_path_from_short_path_version("411.1-2/C101", "3-4a");
        REQUIRE(otherPath != nullptr);
        CHECK(dnv_vista_sdk_gmod_path_query_match(query, otherPath) == 0);

        dnv_vista_sdk_gmod_path_free(otherPath);
        dnv_vista_sdk_gmod_path_query_free(query);
        dnv_vista_sdk_gmod_path_query_builder_free(builder);
        dnv_vista_sdk_gmod_path_free(basePath);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_match - null arguments return 0 and set last error")
    {
        auto* path = dnv_vista_sdk_gmod_path_from_short_path_version("411.1/C101", "3-4a");
        REQUIRE(path != nullptr);
        auto* builder = dnv_vista_sdk_gmod_path_query_builder_from(path);
        REQUIRE(builder != nullptr);
        auto* query = dnv_vista_sdk_gmod_path_query_builder_build(builder);
        REQUIRE(query != nullptr);

        CHECK(dnv_vista_sdk_gmod_path_query_match(nullptr, path) == 0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
        CHECK(dnv_vista_sdk_gmod_path_query_match(query, nullptr) == 0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_gmod_path_query_free(query);
        dnv_vista_sdk_gmod_path_query_builder_free(builder);
        dnv_vista_sdk_gmod_path_free(path);
    }

    TEST_CASE("dnv_vista_sdk_gmod_path_query_free - null is a no-op")
    {
        dnv_vista_sdk_gmod_path_query_free(nullptr);
    }
}
