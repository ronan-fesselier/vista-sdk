#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/local_id.h>
#include <dnv/vista/sdk/c/transport/timeseries/data_channel_id.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::data_channel_id")
{
    TEST_CASE("dnv_vista_sdk_tsd_channel_id_from_string - from LocalId string")
    {
        auto* id = dnv_vista_sdk_tsd_channel_id_from_string(
            "/dnv-v2/vis-3-4a/411.1/C101.62/S205/meta/qty-temperature/cnt-lubricating.oil/pos-inlet");
        REQUIRE(id != nullptr);

        CHECK(dnv_vista_sdk_tsd_channel_id_is_local_id(id) == 1);
        CHECK(dnv_vista_sdk_tsd_channel_id_is_short_id(id) == 0);
        CHECK(dnv_vista_sdk_tsd_channel_id_local_id(id) != nullptr);
        CHECK(dnv_vista_sdk_tsd_channel_id_short_id(id) == nullptr);

        dnv_vista_sdk_tsd_channel_id_free(id);
    }

    TEST_CASE("dnv_vista_sdk_tsd_channel_id_from_string - from short id string")
    {
        auto* id = dnv_vista_sdk_tsd_channel_id_from_string("CH001");
        REQUIRE(id != nullptr);

        CHECK(dnv_vista_sdk_tsd_channel_id_is_local_id(id) == 0);
        CHECK(dnv_vista_sdk_tsd_channel_id_is_short_id(id) == 1);
        REQUIRE(dnv_vista_sdk_tsd_channel_id_short_id(id) != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_channel_id_short_id(id) } == "CH001");
        CHECK(dnv_vista_sdk_tsd_channel_id_local_id(id) == nullptr);

        dnv_vista_sdk_tsd_channel_id_free(id);
    }

    TEST_CASE("dnv_vista_sdk_tsd_channel_id_equals - same LocalIds")
    {
        const char* s = "/dnv-v2/vis-3-4a/411.1/C101.62/S205/meta/qty-temperature/cnt-lubricating.oil/pos-inlet";
        auto* id1 = dnv_vista_sdk_tsd_channel_id_from_string(s);
        auto* id2 = dnv_vista_sdk_tsd_channel_id_from_string(s);
        REQUIRE(id1 != nullptr);
        REQUIRE(id2 != nullptr);

        CHECK(dnv_vista_sdk_tsd_channel_id_equals(id1, id2) == 1);

        dnv_vista_sdk_tsd_channel_id_free(id1);
        dnv_vista_sdk_tsd_channel_id_free(id2);
    }

    TEST_CASE("dnv_vista_sdk_tsd_channel_id_equals - same ShortIds")
    {
        auto* id1 = dnv_vista_sdk_tsd_channel_id_from_string("CH001");
        auto* id2 = dnv_vista_sdk_tsd_channel_id_from_string("CH001");
        REQUIRE(id1 != nullptr);
        REQUIRE(id2 != nullptr);

        CHECK(dnv_vista_sdk_tsd_channel_id_equals(id1, id2) == 1);

        dnv_vista_sdk_tsd_channel_id_free(id1);
        dnv_vista_sdk_tsd_channel_id_free(id2);
    }

    TEST_CASE("dnv_vista_sdk_tsd_channel_id_equals - different types")
    {
        auto* localId =
            dnv_vista_sdk_tsd_channel_id_from_string("/dnv-v2/vis-3-4a/411.1/C101.44i-5A/C261/meta/qty-temperature");
        auto* shortId = dnv_vista_sdk_tsd_channel_id_from_string("CH001");
        REQUIRE(localId != nullptr);
        REQUIRE(shortId != nullptr);

        CHECK(dnv_vista_sdk_tsd_channel_id_equals(localId, shortId) == 0);

        dnv_vista_sdk_tsd_channel_id_free(localId);
        dnv_vista_sdk_tsd_channel_id_free(shortId);
    }

    TEST_CASE("dnv_vista_sdk_tsd_channel_id_equals - different values")
    {
        auto* id1 = dnv_vista_sdk_tsd_channel_id_from_string("CH001");
        auto* id2 = dnv_vista_sdk_tsd_channel_id_from_string("CH002");
        REQUIRE(id1 != nullptr);
        REQUIRE(id2 != nullptr);

        CHECK(dnv_vista_sdk_tsd_channel_id_equals(id1, id2) == 0);

        dnv_vista_sdk_tsd_channel_id_free(id1);
        dnv_vista_sdk_tsd_channel_id_free(id2);
    }

    TEST_CASE("dnv_vista_sdk_tsd_channel_id_to_string - LocalId")
    {
        const char* s = "/dnv-v2/vis-3-4a/411.1/C101.44i-6A/C261/meta/qty-temperature";
        auto* id = dnv_vista_sdk_tsd_channel_id_from_string(s);
        REQUIRE(id != nullptr);

        char* str = dnv_vista_sdk_tsd_channel_id_to_string(id);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == s);

        dnv_vista_sdk_tsd_channel_id_string_free(str);
        dnv_vista_sdk_tsd_channel_id_free(id);
    }

    TEST_CASE("dnv_vista_sdk_tsd_channel_id_to_string - ShortId")
    {
        auto* id = dnv_vista_sdk_tsd_channel_id_from_string("CH001");
        REQUIRE(id != nullptr);

        char* str = dnv_vista_sdk_tsd_channel_id_to_string(id);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "CH001");

        dnv_vista_sdk_tsd_channel_id_string_free(str);
        dnv_vista_sdk_tsd_channel_id_free(id);
    }

    TEST_CASE("dnv_vista_sdk_tsd_channel_id_from_string - invalid LocalId becomes ShortId")
    {
        auto* id = dnv_vista_sdk_tsd_channel_id_from_string("/invalid/local/id");
        REQUIRE(id != nullptr);

        CHECK(dnv_vista_sdk_tsd_channel_id_is_local_id(id) == 0);
        CHECK(dnv_vista_sdk_tsd_channel_id_is_short_id(id) == 1);
        REQUIRE(dnv_vista_sdk_tsd_channel_id_short_id(id) != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_channel_id_short_id(id) } == "/invalid/local/id");

        dnv_vista_sdk_tsd_channel_id_free(id);
    }

    TEST_CASE("dnv_vista_sdk_tsd_channel_id_from_string - empty string returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_tsd_channel_id_from_string("") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_tsd_channel_id_from_string - null returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_tsd_channel_id_from_string(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_tsd_channel_id_local_id - borrowed pointer usable with local_id API")
    {
        const char* s = "/dnv-v2/vis-3-4a/511.15-1/E32/meta/qty-power";
        auto* id = dnv_vista_sdk_tsd_channel_id_from_string(s);
        REQUIRE(id != nullptr);

        const auto* localId = dnv_vista_sdk_tsd_channel_id_local_id(id);
        REQUIRE(localId != nullptr);

        char* str = dnv_vista_sdk_local_id_to_string(localId);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == s);

        dnv_vista_sdk_local_id_string_free(str);
        dnv_vista_sdk_tsd_channel_id_free(id);
    }

    TEST_CASE("dnv_vista_sdk_tsd_channel_id_free - null is a no-op")
    {
        dnv_vista_sdk_tsd_channel_id_free(nullptr);
    }

    TEST_CASE("null channelId handle - accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_tsd_channel_id_is_local_id(nullptr) == 0);
        CHECK(dnv_vista_sdk_tsd_channel_id_is_short_id(nullptr) == 0);
        CHECK(dnv_vista_sdk_tsd_channel_id_local_id(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_tsd_channel_id_short_id(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_tsd_channel_id_to_string(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_tsd_channel_id_equals(nullptr, nullptr) == 0);
    }
}
