#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/transport/serialization/json/datachannel/data_channel_json.h>
#include <dnv/vista/sdk/c/transport/datachannel/data_channel.h>
#include <dnv/vista/sdk/c/common.h>
#include <dnv/vista/sdk/c/error.h>

#include <EmbeddedTestData.h>

#include <string_view>

namespace
{
    const std::string& validJson()
    {
        static const std::string json = dnv::vista::sdk::EmbeddedTestData::text("DataChannelList.json");
        return json;
    }
} // namespace

TEST_SUITE("c-api::data_channel_json")
{
    TEST_CASE("dnv_vista_sdk_dcl_list_package_from_json - parses the reference file")
    {
        auto* package = dnv_vista_sdk_dcl_list_package_from_json(validJson().c_str());
        REQUIRE(package != nullptr);

        const auto* list = dnv_vista_sdk_dcl_list_package_data_channel_list(package);
        REQUIRE(list != nullptr);
        CHECK(dnv_vista_sdk_dcl_data_channel_list_size(list) > 0);

        const auto* dc = dnv_vista_sdk_dcl_data_channel_list_at(list, 0);
        REQUIRE(dc != nullptr);
        const auto* channelId = dnv_vista_sdk_dcl_data_channel_channel_id(dc);
        REQUIRE(channelId != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_dcl_channel_id_short_id(channelId) } == "0010");

        const auto* header = dnv_vista_sdk_dcl_package_header(dnv_vista_sdk_dcl_list_package_package(package));
        REQUIRE(header != nullptr);
        const auto* shipId = dnv_vista_sdk_dcl_header_ship_id(header);
        REQUIRE(shipId != nullptr);

        dnv_vista_sdk_dcl_list_package_free(package);
    }

    TEST_CASE("dnv_vista_sdk_dcl_list_package_from_json - null returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_dcl_list_package_from_json(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_dcl_list_package_from_json - malformed JSON returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_dcl_list_package_from_json("{ not valid json") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_dcl_list_package_from_json - missing mandatory field returns null")
    {
        constexpr const char* missingShipId = R"({
            "Package": {
                "Header": {
                    "DataChannelListID": {
                        "ID": "DataChannelList.xml",
                        "TimeStamp": "2016-01-01T00:00:00Z"
                    }
                },
                "DataChannelList": { "DataChannel": [] }
            }
        })";

        CHECK(dnv_vista_sdk_dcl_list_package_from_json(missingShipId) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_dcl_list_package_to_json - null returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_dcl_list_package_to_json(nullptr, 0) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("roundtrip - domain -> JSON -> domain (compact)")
    {
        auto* package = dnv_vista_sdk_dcl_list_package_from_json(validJson().c_str());
        REQUIRE(package != nullptr);

        char* json = dnv_vista_sdk_dcl_list_package_to_json(package, 0);
        REQUIRE(json != nullptr);

        auto* roundTripped = dnv_vista_sdk_dcl_list_package_from_json(json);
        REQUIRE(roundTripped != nullptr);

        const auto* originalList = dnv_vista_sdk_dcl_list_package_data_channel_list(package);
        const auto* roundTrippedList = dnv_vista_sdk_dcl_list_package_data_channel_list(roundTripped);
        REQUIRE(originalList != nullptr);
        REQUIRE(roundTrippedList != nullptr);
        CHECK(
            dnv_vista_sdk_dcl_data_channel_list_size(originalList) ==
            dnv_vista_sdk_dcl_data_channel_list_size(roundTrippedList));

        dnv_vista_sdk_string_free(json);
        dnv_vista_sdk_dcl_list_package_free(roundTripped);
        dnv_vista_sdk_dcl_list_package_free(package);
    }

    TEST_CASE("roundtrip - prettyPrint output still parses back correctly")
    {
        auto* package = dnv_vista_sdk_dcl_list_package_from_json(validJson().c_str());
        REQUIRE(package != nullptr);

        char* prettyJson = dnv_vista_sdk_dcl_list_package_to_json(package, 1);
        REQUIRE(prettyJson != nullptr);
        CHECK(std::string_view{ prettyJson }.find("\n") != std::string_view::npos);

        auto* roundTripped = dnv_vista_sdk_dcl_list_package_from_json(prettyJson);
        REQUIRE(roundTripped != nullptr);

        dnv_vista_sdk_string_free(prettyJson);
        dnv_vista_sdk_dcl_list_package_free(roundTripped);
        dnv_vista_sdk_dcl_list_package_free(package);
    }

    TEST_CASE("dnv_vista_sdk_dcl_json_string_free - null is a no-op")
    {
        dnv_vista_sdk_string_free(nullptr);
    }
}
