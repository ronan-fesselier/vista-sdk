#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/imo_number.h>
#include <dnv/vista/sdk/c/transport/serialization/json/datachannel/data_channel_json.h>
#include <dnv/vista/sdk/c/transport/serialization/json/serializable_document.h>
#include <dnv/vista/sdk/c/transport/timeseries/data_channel_id.h>
#include <dnv/vista/sdk/c/transport/timeseries/time_series_data.h>
#include <dnv/vista/sdk/c/transport/ship_id.h>
#include <dnv/vista/sdk/c/error.h>

#include <EmbeddedTestData.h>

#include <string_view>

TEST_SUITE("c-api::time_series_data::time_span")
{
    TEST_CASE("create - start before end succeeds")
    {
        auto start = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704110400);
        auto end = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704196800);

        auto* ts = dnv_vista_sdk_tsd_time_span_create(start, end);
        REQUIRE(ts != nullptr);

        CHECK(dnv_vista_sdk_date_time_offset_equals(dnv_vista_sdk_tsd_time_span_start(ts), start) == 1);
        CHECK(dnv_vista_sdk_date_time_offset_equals(dnv_vista_sdk_tsd_time_span_end(ts), end) == 1);

        dnv_vista_sdk_tsd_time_span_free(ts);
    }

    TEST_CASE("create - start after end returns null and sets last error")
    {
        auto start = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704196800);
        auto end = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704110400);

        CHECK(dnv_vista_sdk_tsd_time_span_create(start, end) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("set_start - after current end sets last error and leaves unchanged")
    {
        auto start = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704110400);
        auto end = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704196800);
        auto* ts = dnv_vista_sdk_tsd_time_span_create(start, end);
        REQUIRE(ts != nullptr);

        auto tooLate = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704283200);
        dnv_vista_sdk_tsd_time_span_set_start(ts, tooLate);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
        CHECK(dnv_vista_sdk_date_time_offset_equals(dnv_vista_sdk_tsd_time_span_start(ts), start) == 1);

        dnv_vista_sdk_tsd_time_span_free(ts);
    }
}

TEST_SUITE("c-api::time_series_data::config_ref")
{
    TEST_CASE("create - id and timestamp round-trip")
    {
        auto ts = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704110400);
        auto* configRef = dnv_vista_sdk_tsd_config_ref_create("DataChannelList.xml", ts);
        REQUIRE(configRef != nullptr);

        CHECK(std::string_view{ dnv_vista_sdk_tsd_config_ref_id(configRef) } == "DataChannelList.xml");
        CHECK(dnv_vista_sdk_date_time_offset_equals(dnv_vista_sdk_tsd_config_ref_timestamp(configRef), ts) == 1);

        dnv_vista_sdk_tsd_config_ref_free(configRef);
    }

    TEST_CASE("create - null id returns null and sets last error")
    {
        auto ts = dnv_vista_sdk_date_time_offset_utc_now();
        CHECK(dnv_vista_sdk_tsd_config_ref_create(nullptr, ts) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }
}

TEST_SUITE("c-api::time_series_data::header")
{
    TEST_CASE("create - ship_id round-trips, system_configuration list")
    {
        auto* imo = dnv_vista_sdk_imo_number_from_string("9074729");
        REQUIRE(imo != nullptr);
        auto* shipId = dnv_vista_sdk_ship_id_from_imo_number(imo);
        REQUIRE(shipId != nullptr);

        auto* header = dnv_vista_sdk_tsd_header_create(shipId);
        REQUIRE(header != nullptr);
        CHECK(dnv_vista_sdk_tsd_header_ship_id(header) != nullptr);
        CHECK(dnv_vista_sdk_tsd_header_system_configuration_count(header) == 0);

        auto ts1 = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704067200);
        auto ts2 = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704240000);
        auto* cfg1 = dnv_vista_sdk_tsd_config_ref_create("SystemConfiguration.xml", ts1);
        auto* cfg2 = dnv_vista_sdk_tsd_config_ref_create("SystemConfiguration.xml", ts2);
        REQUIRE(cfg1 != nullptr);
        REQUIRE(cfg2 != nullptr);

        const dnv_vista_sdk_tsd_config_ref_t* entries[] = { cfg1, cfg2 };
        dnv_vista_sdk_tsd_header_set_system_configuration(header, entries, 2);

        REQUIRE(dnv_vista_sdk_tsd_header_system_configuration_count(header) == 2);
        const auto* first = dnv_vista_sdk_tsd_header_system_configuration_at(header, 0);
        REQUIRE(first != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_config_ref_id(first) } == "SystemConfiguration.xml");

        dnv_vista_sdk_tsd_config_ref_free(cfg1);
        dnv_vista_sdk_tsd_config_ref_free(cfg2);
        dnv_vista_sdk_tsd_header_free(header);
        dnv_vista_sdk_ship_id_free(shipId);
        dnv_vista_sdk_imo_number_free(imo);
    }

    TEST_CASE("set_custom_headers - takes ownership")
    {
        auto* imo = dnv_vista_sdk_imo_number_from_string("9074729");
        auto* shipId = dnv_vista_sdk_ship_id_from_imo_number(imo);
        auto* header = dnv_vista_sdk_tsd_header_create(shipId);
        REQUIRE(header != nullptr);

        auto* doc = dnv_vista_sdk_serializable_document_from_string("custom");
        REQUIRE(doc != nullptr);
        dnv_vista_sdk_tsd_header_set_custom_headers(header, doc);

        const auto* stored = dnv_vista_sdk_tsd_header_custom_headers(header);
        REQUIRE(stored != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_serializable_document_as_string(stored) } == "custom");

        dnv_vista_sdk_tsd_header_free(header);
        dnv_vista_sdk_ship_id_free(shipId);
        dnv_vista_sdk_imo_number_free(imo);
    }

    TEST_CASE("create - null ship_id returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_tsd_header_create(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }
}

TEST_SUITE("c-api::time_series_data::tabular_data_set")
{
    TEST_CASE("create - values round-trip")
    {
        auto ts = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704110400);
        const char* values[] = { "100.0", "200.0" };

        auto* dataSet = dnv_vista_sdk_tsd_tabular_data_set_create(ts, values, 2);
        REQUIRE(dataSet != nullptr);

        REQUIRE(dnv_vista_sdk_tsd_tabular_data_set_value_count(dataSet) == 2);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_tabular_data_set_value_at(dataSet, 0) } == "100.0");
        CHECK(std::string_view{ dnv_vista_sdk_tsd_tabular_data_set_value_at(dataSet, 1) } == "200.0");
        CHECK(dnv_vista_sdk_tsd_tabular_data_set_quality_count(dataSet) == 0);

        const char* quality[] = { "0", "0" };
        dnv_vista_sdk_tsd_tabular_data_set_set_quality(dataSet, quality, 2);
        REQUIRE(dnv_vista_sdk_tsd_tabular_data_set_quality_count(dataSet) == 2);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_tabular_data_set_quality_at(dataSet, 0) } == "0");

        dnv_vista_sdk_tsd_tabular_data_set_free(dataSet);
    }
}

TEST_SUITE("c-api::time_series_data::tabular_data")
{
    TEST_CASE("create/validate - matching counts is valid")
    {
        auto* id1 = dnv_vista_sdk_tsd_channel_id_from_string("0010");
        auto* id2 = dnv_vista_sdk_tsd_channel_id_from_string("0020");
        REQUIRE(id1 != nullptr);
        REQUIRE(id2 != nullptr);
        const dnv_vista_sdk_tsd_channel_id_t* ids[] = { id1, id2 };

        auto ts = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704110400);
        const char* values[] = { "100.0", "200.0" };
        auto* dataSet = dnv_vista_sdk_tsd_tabular_data_set_create(ts, values, 2);
        REQUIRE(dataSet != nullptr);
        const dnv_vista_sdk_tsd_tabular_data_set_t* dataSets[] = { dataSet };

        auto* tabularData = dnv_vista_sdk_tsd_tabular_data_create(ids, 2, dataSets, 1);
        REQUIRE(tabularData != nullptr);

        CHECK(dnv_vista_sdk_tsd_tabular_data_channel_id_count(tabularData) == 2);
        CHECK(dnv_vista_sdk_tsd_tabular_data_data_set_count(tabularData) == 1);
        CHECK(dnv_vista_sdk_tsd_tabular_data_validate(tabularData) == 1);

        dnv_vista_sdk_tsd_tabular_data_free(tabularData);
        dnv_vista_sdk_tsd_tabular_data_set_free(dataSet);
        dnv_vista_sdk_tsd_channel_id_free(id1);
        dnv_vista_sdk_tsd_channel_id_free(id2);
    }

    TEST_CASE("validate - mismatched value count is invalid")
    {
        auto* id1 = dnv_vista_sdk_tsd_channel_id_from_string("0010");
        REQUIRE(id1 != nullptr);
        const dnv_vista_sdk_tsd_channel_id_t* ids[] = { id1 };

        auto ts = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704110400);
        const char* values[] = { "100.0", "200.0" };
        auto* dataSet = dnv_vista_sdk_tsd_tabular_data_set_create(ts, values, 2);
        REQUIRE(dataSet != nullptr);
        const dnv_vista_sdk_tsd_tabular_data_set_t* dataSets[] = { dataSet };

        auto* tabularData = dnv_vista_sdk_tsd_tabular_data_create(ids, 1, dataSets, 1);
        REQUIRE(tabularData != nullptr);

        CHECK(dnv_vista_sdk_tsd_tabular_data_validate(tabularData) == 0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_tsd_tabular_data_free(tabularData);
        dnv_vista_sdk_tsd_tabular_data_set_free(dataSet);
        dnv_vista_sdk_tsd_channel_id_free(id1);
    }
}

TEST_SUITE("c-api::time_series_data::event_data_set")
{
    TEST_CASE("create - fields round-trip")
    {
        auto* channelId = dnv_vista_sdk_tsd_channel_id_from_string("0010");
        REQUIRE(channelId != nullptr);
        auto ts = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704110400);

        auto* eventDataSet = dnv_vista_sdk_tsd_event_data_set_create(ts, channelId, "100.0");
        REQUIRE(eventDataSet != nullptr);

        CHECK(std::string_view{ dnv_vista_sdk_tsd_event_data_set_value(eventDataSet) } == "100.0");
        CHECK(dnv_vista_sdk_tsd_event_data_set_data_channel_id(eventDataSet) != nullptr);
        CHECK(dnv_vista_sdk_tsd_event_data_set_quality(eventDataSet) == nullptr);

        dnv_vista_sdk_tsd_event_data_set_set_quality(eventDataSet, "0");
        REQUIRE(dnv_vista_sdk_tsd_event_data_set_quality(eventDataSet) != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_event_data_set_quality(eventDataSet) } == "0");

        dnv_vista_sdk_tsd_event_data_set_free(eventDataSet);
        dnv_vista_sdk_tsd_channel_id_free(channelId);
    }
}

TEST_SUITE("c-api::time_series_data::event_data")
{
    TEST_CASE("create - empty then populated")
    {
        auto* eventData = dnv_vista_sdk_tsd_event_data_create();
        REQUIRE(eventData != nullptr);
        CHECK(dnv_vista_sdk_tsd_event_data_data_set_count(eventData) == 0);

        auto* channelId = dnv_vista_sdk_tsd_channel_id_from_string("0010");
        auto ts = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704110400);
        auto* eventDataSet = dnv_vista_sdk_tsd_event_data_set_create(ts, channelId, "100.0");
        REQUIRE(eventDataSet != nullptr);

        const dnv_vista_sdk_tsd_event_data_set_t* sets[] = { eventDataSet };
        dnv_vista_sdk_tsd_event_data_set_data_set(eventData, sets, 1);

        REQUIRE(dnv_vista_sdk_tsd_event_data_data_set_count(eventData) == 1);
        CHECK(dnv_vista_sdk_tsd_event_data_data_set_at(eventData, 0) != nullptr);

        dnv_vista_sdk_tsd_event_data_free(eventData);
        dnv_vista_sdk_tsd_event_data_set_free(eventDataSet);
        dnv_vista_sdk_tsd_channel_id_free(channelId);
    }
}

TEST_SUITE("c-api::time_series_data::time_series_data")
{
    TEST_CASE("create - empty, then set data_configuration/tabular_data/event_data")
    {
        auto* timeSeriesData = dnv_vista_sdk_tsd_time_series_data_create();
        REQUIRE(timeSeriesData != nullptr);
        CHECK(dnv_vista_sdk_tsd_time_series_data_data_configuration(timeSeriesData) == nullptr);
        CHECK(dnv_vista_sdk_tsd_time_series_data_tabular_data_count(timeSeriesData) == 0);
        CHECK(dnv_vista_sdk_tsd_time_series_data_event_data(timeSeriesData) == nullptr);

        auto ts = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704067200);
        auto* configRef = dnv_vista_sdk_tsd_config_ref_create("DataChannelList.xml", ts);
        REQUIRE(configRef != nullptr);
        dnv_vista_sdk_tsd_time_series_data_set_data_configuration(timeSeriesData, configRef);
        CHECK(dnv_vista_sdk_tsd_time_series_data_data_configuration(timeSeriesData) != nullptr);

        dnv_vista_sdk_tsd_config_ref_free(configRef);
        dnv_vista_sdk_tsd_time_series_data_free(timeSeriesData);
    }
}

TEST_SUITE("c-api::time_series_data::package")
{
    TEST_CASE("create - header optional, time_series_data list")
    {
        auto* timeSeriesData = dnv_vista_sdk_tsd_time_series_data_create();
        REQUIRE(timeSeriesData != nullptr);
        const dnv_vista_sdk_tsd_time_series_data_t* entries[] = { timeSeriesData };

        auto* package = dnv_vista_sdk_tsd_package_create(nullptr, entries, 1);
        REQUIRE(package != nullptr);
        CHECK(dnv_vista_sdk_tsd_package_header(package) == nullptr);
        CHECK(dnv_vista_sdk_tsd_package_time_series_data_count(package) == 1);

        dnv_vista_sdk_tsd_package_free(package);
        dnv_vista_sdk_tsd_time_series_data_free(timeSeriesData);
    }
}

TEST_SUITE("c-api::time_series_data::data_package")
{
    TEST_CASE("create - package round-trips")
    {
        auto* timeSeriesData = dnv_vista_sdk_tsd_time_series_data_create();
        REQUIRE(timeSeriesData != nullptr);
        const dnv_vista_sdk_tsd_time_series_data_t* entries[] = { timeSeriesData };
        auto* package = dnv_vista_sdk_tsd_package_create(nullptr, entries, 1);
        REQUIRE(package != nullptr);

        auto* dataPackage = dnv_vista_sdk_tsd_data_package_create(package);
        REQUIRE(dataPackage != nullptr);
        CHECK(dnv_vista_sdk_tsd_data_package_package(dataPackage) != nullptr);

        dnv_vista_sdk_tsd_data_package_free(dataPackage);
        dnv_vista_sdk_tsd_package_free(package);
        dnv_vista_sdk_tsd_time_series_data_free(timeSeriesData);
    }

    TEST_CASE("create - null returns null")
    {
        CHECK(dnv_vista_sdk_tsd_data_package_create(nullptr) == nullptr);
    }
}

namespace
{
    const std::string& validDataChannelListJson()
    {
        static const std::string json = dnv::vista::sdk::EmbeddedTestData::text("DataChannelList.json");
        return json;
    }

    int okCallback(
        dnv_vista_sdk_date_time_offset_t,
        const dnv_vista_sdk_dcl_data_channel_t*,
        const dnv_vista_sdk_iso19848_value_t*,
        const char*,
        const char**,
        void*)
    {
        return 1;
    }

    int failCallback(
        dnv_vista_sdk_date_time_offset_t,
        const dnv_vista_sdk_dcl_data_channel_t*,
        const dnv_vista_sdk_iso19848_value_t*,
        const char*,
        const char** outErrorMessage,
        void*)
    {
        if (outErrorMessage != nullptr)
        {
            *outErrorMessage = "custom callback rejected this point";
        }
        return 0;
    }

    int countingCallback(
        dnv_vista_sdk_date_time_offset_t,
        const dnv_vista_sdk_dcl_data_channel_t*,
        const dnv_vista_sdk_iso19848_value_t*,
        const char*,
        const char**,
        void* userdata)
    {
        *static_cast<int*>(userdata) += 1;
        return 1;
    }
} // namespace

TEST_SUITE("c-api::time_series_data::validate")
{
    TEST_CASE("dnv_vista_sdk_tsd_time_series_data_validate - valid tabular data point")
    {
        auto* dcListPackage = dnv_vista_sdk_dcl_list_package_from_json(validDataChannelListJson().c_str());
        REQUIRE(dcListPackage != nullptr);

        auto* channelId = dnv_vista_sdk_tsd_channel_id_from_string("0010");
        REQUIRE(channelId != nullptr);
        const dnv_vista_sdk_tsd_channel_id_t* channelIds[] = { channelId };

        auto ts = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704110400);
        const char* values[] = { "12.5" };
        auto* dataSet = dnv_vista_sdk_tsd_tabular_data_set_create(ts, values, 1);
        REQUIRE(dataSet != nullptr);
        const dnv_vista_sdk_tsd_tabular_data_set_t* dataSets[] = { dataSet };

        auto* tabularData = dnv_vista_sdk_tsd_tabular_data_create(channelIds, 1, dataSets, 1);
        REQUIRE(tabularData != nullptr);

        auto* timeSeriesData = dnv_vista_sdk_tsd_time_series_data_create();
        REQUIRE(timeSeriesData != nullptr);
        const dnv_vista_sdk_tsd_tabular_data_t* tabularEntries[] = { tabularData };
        dnv_vista_sdk_tsd_time_series_data_set_tabular_data(timeSeriesData, tabularEntries, 1);

        int callCount = 0;
        CHECK(
            dnv_vista_sdk_tsd_time_series_data_validate(
                timeSeriesData, dcListPackage, countingCallback, &callCount, okCallback, nullptr) == 1);
        CHECK(callCount == 1);

        dnv_vista_sdk_tsd_time_series_data_free(timeSeriesData);
        dnv_vista_sdk_tsd_tabular_data_free(tabularData);
        dnv_vista_sdk_tsd_tabular_data_set_free(dataSet);
        dnv_vista_sdk_tsd_channel_id_free(channelId);
        dnv_vista_sdk_dcl_list_package_free(dcListPackage);
    }

    TEST_CASE("dnv_vista_sdk_tsd_time_series_data_validate - unknown channel id fails")
    {
        auto* dcListPackage = dnv_vista_sdk_dcl_list_package_from_json(validDataChannelListJson().c_str());
        REQUIRE(dcListPackage != nullptr);

        auto* channelId = dnv_vista_sdk_tsd_channel_id_from_string("9999");
        REQUIRE(channelId != nullptr);
        const dnv_vista_sdk_tsd_channel_id_t* channelIds[] = { channelId };

        auto ts = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704110400);
        const char* values[] = { "12.5" };
        auto* dataSet = dnv_vista_sdk_tsd_tabular_data_set_create(ts, values, 1);
        REQUIRE(dataSet != nullptr);
        const dnv_vista_sdk_tsd_tabular_data_set_t* dataSets[] = { dataSet };

        auto* tabularData = dnv_vista_sdk_tsd_tabular_data_create(channelIds, 1, dataSets, 1);
        REQUIRE(tabularData != nullptr);

        auto* timeSeriesData = dnv_vista_sdk_tsd_time_series_data_create();
        REQUIRE(timeSeriesData != nullptr);
        const dnv_vista_sdk_tsd_tabular_data_t* tabularEntries[] = { tabularData };
        dnv_vista_sdk_tsd_time_series_data_set_tabular_data(timeSeriesData, tabularEntries, 1);

        CHECK(
            dnv_vista_sdk_tsd_time_series_data_validate(
                timeSeriesData, dcListPackage, okCallback, nullptr, okCallback, nullptr) == 0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.find("not found") != std::string_view::npos);

        dnv_vista_sdk_tsd_time_series_data_free(timeSeriesData);
        dnv_vista_sdk_tsd_tabular_data_free(tabularData);
        dnv_vista_sdk_tsd_tabular_data_set_free(dataSet);
        dnv_vista_sdk_tsd_channel_id_free(channelId);
        dnv_vista_sdk_dcl_list_package_free(dcListPackage);
    }

    TEST_CASE("dnv_vista_sdk_tsd_time_series_data_validate - custom callback failure propagates its error message")
    {
        auto* dcListPackage = dnv_vista_sdk_dcl_list_package_from_json(validDataChannelListJson().c_str());
        REQUIRE(dcListPackage != nullptr);

        auto* channelId = dnv_vista_sdk_tsd_channel_id_from_string("0010");
        REQUIRE(channelId != nullptr);
        const dnv_vista_sdk_tsd_channel_id_t* channelIds[] = { channelId };

        auto ts = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704110400);
        const char* values[] = { "12.5" };
        auto* dataSet = dnv_vista_sdk_tsd_tabular_data_set_create(ts, values, 1);
        REQUIRE(dataSet != nullptr);
        const dnv_vista_sdk_tsd_tabular_data_set_t* dataSets[] = { dataSet };

        auto* tabularData = dnv_vista_sdk_tsd_tabular_data_create(channelIds, 1, dataSets, 1);
        REQUIRE(tabularData != nullptr);

        auto* timeSeriesData = dnv_vista_sdk_tsd_time_series_data_create();
        REQUIRE(timeSeriesData != nullptr);
        const dnv_vista_sdk_tsd_tabular_data_t* tabularEntries[] = { tabularData };
        dnv_vista_sdk_tsd_time_series_data_set_tabular_data(timeSeriesData, tabularEntries, 1);

        CHECK(
            dnv_vista_sdk_tsd_time_series_data_validate(
                timeSeriesData, dcListPackage, failCallback, nullptr, okCallback, nullptr) == 0);
        CHECK(
            std::string_view{ dnv_vista_sdk_last_error_message() }.find("custom callback rejected this point") !=
            std::string_view::npos);

        dnv_vista_sdk_tsd_time_series_data_free(timeSeriesData);
        dnv_vista_sdk_tsd_tabular_data_free(tabularData);
        dnv_vista_sdk_tsd_tabular_data_set_free(dataSet);
        dnv_vista_sdk_tsd_channel_id_free(channelId);
        dnv_vista_sdk_dcl_list_package_free(dcListPackage);
    }

    TEST_CASE("dnv_vista_sdk_tsd_time_series_data_validate - null arguments")
    {
        CHECK(
            dnv_vista_sdk_tsd_time_series_data_validate(nullptr, nullptr, okCallback, nullptr, okCallback, nullptr) ==
            0);
    }
}
