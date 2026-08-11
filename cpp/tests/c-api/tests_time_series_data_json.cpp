#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/transport/serialization/json/timeseries/time_series_data_json.h>
#include <dnv/vista/sdk/c/transport/timeseries/time_series_data.h>
#include <dnv/vista/sdk/c/error.h>

#include <EmbeddedTestData.h>

#include <string_view>

namespace
{
    const std::string& validJson()
    {
        static const std::string json = dnv::vista::sdk::EmbeddedTestData::text("TimeSeriesData.json");
        return json;
    }
} // namespace

TEST_SUITE("c-api::time_series_data_json")
{
    TEST_CASE("dnv_vista_sdk_tsd_data_package_from_json - parses the reference file")
    {
        auto* dataPackage = dnv_vista_sdk_tsd_data_package_from_json(validJson().c_str());
        REQUIRE(dataPackage != nullptr);

        const auto* package = dnv_vista_sdk_tsd_data_package_package(dataPackage);
        REQUIRE(package != nullptr);
        CHECK(dnv_vista_sdk_tsd_package_time_series_data_count(package) > 0);

        const auto* header = dnv_vista_sdk_tsd_package_header(package);
        REQUIRE(header != nullptr);
        const auto* shipId = dnv_vista_sdk_tsd_header_ship_id(header);
        REQUIRE(shipId != nullptr);

        dnv_vista_sdk_tsd_data_package_free(dataPackage);
    }

    TEST_CASE("dnv_vista_sdk_tsd_data_package_from_json - null returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_tsd_data_package_from_json(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_tsd_data_package_from_json - malformed JSON returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_tsd_data_package_from_json("{ not valid json") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_tsd_data_package_from_json - missing mandatory field returns null")
    {
        constexpr const char* missingShipId = R"({
            "Package": {
                "Header": {
                    "DateCreated": "2016-01-03T12:00:00Z"
                },
                "TimeSeriesData": []
            }
        })";

        CHECK(dnv_vista_sdk_tsd_data_package_from_json(missingShipId) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_tsd_data_package_to_json - null returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_tsd_data_package_to_json(nullptr, 0) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("roundtrip - domain -> JSON -> domain (compact)")
    {
        auto* dataPackage = dnv_vista_sdk_tsd_data_package_from_json(validJson().c_str());
        REQUIRE(dataPackage != nullptr);

        char* json = dnv_vista_sdk_tsd_data_package_to_json(dataPackage, 0);
        REQUIRE(json != nullptr);

        auto* roundTripped = dnv_vista_sdk_tsd_data_package_from_json(json);
        REQUIRE(roundTripped != nullptr);

        const auto* originalPackage = dnv_vista_sdk_tsd_data_package_package(dataPackage);
        const auto* roundTrippedPackage = dnv_vista_sdk_tsd_data_package_package(roundTripped);
        REQUIRE(originalPackage != nullptr);
        REQUIRE(roundTrippedPackage != nullptr);
        CHECK(
            dnv_vista_sdk_tsd_package_time_series_data_count(originalPackage) ==
            dnv_vista_sdk_tsd_package_time_series_data_count(roundTrippedPackage));

        dnv_vista_sdk_tsd_json_string_free(json);
        dnv_vista_sdk_tsd_data_package_free(roundTripped);
        dnv_vista_sdk_tsd_data_package_free(dataPackage);
    }

    TEST_CASE("roundtrip - prettyPrint output still parses back correctly")
    {
        auto* dataPackage = dnv_vista_sdk_tsd_data_package_from_json(validJson().c_str());
        REQUIRE(dataPackage != nullptr);

        char* prettyJson = dnv_vista_sdk_tsd_data_package_to_json(dataPackage, 1);
        REQUIRE(prettyJson != nullptr);
        CHECK(std::string_view{ prettyJson }.find("\n") != std::string_view::npos);

        auto* roundTripped = dnv_vista_sdk_tsd_data_package_from_json(prettyJson);
        REQUIRE(roundTripped != nullptr);

        dnv_vista_sdk_tsd_json_string_free(prettyJson);
        dnv_vista_sdk_tsd_data_package_free(roundTripped);
        dnv_vista_sdk_tsd_data_package_free(dataPackage);
    }

    TEST_CASE("dnv_vista_sdk_tsd_json_string_free - null is a no-op")
    {
        dnv_vista_sdk_tsd_json_string_free(nullptr);
    }
}
