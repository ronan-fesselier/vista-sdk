#include <doctest/doctest.h>

#include <dnv/VistaSDK_c_api.h>

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

TEST_SUITE("c-api::time_series_data_dto")
{
    TEST_CASE("from_json round-trip preserves tsd count")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        REQUIRE(pkg != nullptr);
        CHECK(dnv_vista_sdk_tsd_dto_pkg_tsd_count(pkg) > 0);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("from_json null returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_tsd_dto_from_json(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("from_json malformed JSON returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_tsd_dto_from_json("{ not valid json") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("to_json produces non-empty string")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        char* json = dnv_vista_sdk_tsd_dto_to_json(dto, 0);
        REQUIRE(json != nullptr);
        CHECK(std::string_view{ json }.size() > 0);

        dnv_vista_sdk_string_free(json);
        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("to_domain round-trip preserves ship id")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        REQUIRE(pkg != nullptr);
        REQUIRE(dnv_vista_sdk_tsd_dto_pkg_has_header(pkg) == 1);
        auto* header = dnv_vista_sdk_tsd_dto_pkg_get_header(pkg);
        REQUIRE(header != nullptr);
        const char* shipId = dnv_vista_sdk_tsd_dto_header_get_ship_id(header);
        REQUIRE(shipId != nullptr);
        std::string originalShipId = shipId;

        auto* domain = dnv_vista_sdk_tsd_to_domain(dto);
        REQUIRE(domain != nullptr);
        auto* dto2 = dnv_vista_sdk_tsd_to_dto(domain);
        REQUIRE(dto2 != nullptr);

        auto* pkg2 = dnv_vista_sdk_tsd_dto_package_get_pkg(dto2);
        REQUIRE(pkg2 != nullptr);
        REQUIRE(dnv_vista_sdk_tsd_dto_pkg_has_header(pkg2) == 1);
        auto* header2 = dnv_vista_sdk_tsd_dto_pkg_get_header(pkg2);
        REQUIRE(header2 != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_header_get_ship_id(header2) } == originalShipId);

        dnv_vista_sdk_tsd_data_package_free(domain);
        dnv_vista_sdk_tsd_dto_package_free(dto2);
        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("header ship_id get and set")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        REQUIRE(dnv_vista_sdk_tsd_dto_pkg_has_header(pkg) == 1);
        auto* header = dnv_vista_sdk_tsd_dto_pkg_get_header(pkg);
        REQUIRE(header != nullptr);

        dnv_vista_sdk_tsd_dto_header_set_ship_id(header, "IMO9999999");
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_header_get_ship_id(header) } == "IMO9999999");

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("header author optional get set clear")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        auto* header = dnv_vista_sdk_tsd_dto_pkg_get_header(pkg);
        REQUIRE(header != nullptr);

        dnv_vista_sdk_tsd_dto_header_set_author(header, "Test Author");
        CHECK(dnv_vista_sdk_tsd_dto_header_has_author(header) == 1);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_header_get_author(header) } == "Test Author");

        dnv_vista_sdk_tsd_dto_header_clear_author(header);
        CHECK(dnv_vista_sdk_tsd_dto_header_has_author(header) == 0);
        CHECK(dnv_vista_sdk_tsd_dto_header_get_author(header) == nullptr);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("header time_span ensure and clear")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        auto* header = dnv_vista_sdk_tsd_dto_pkg_get_header(pkg);
        REQUIRE(header != nullptr);

        CHECK(dnv_vista_sdk_tsd_dto_header_has_time_span(header) == 1);
        auto* ts = dnv_vista_sdk_tsd_dto_header_get_time_span(header);
        REQUIRE(ts != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_time_span_get_start(ts) }.size() > 0);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_time_span_get_end(ts) }.size() > 0);

        dnv_vista_sdk_tsd_dto_header_clear_time_span(header);
        CHECK(dnv_vista_sdk_tsd_dto_header_has_time_span(header) == 0);

        dnv_vista_sdk_tsd_dto_header_ensure_time_span(header);
        CHECK(dnv_vista_sdk_tsd_dto_header_has_time_span(header) == 1);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("header time_span set start and end")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        auto* header = dnv_vista_sdk_tsd_dto_pkg_get_header(pkg);
        auto* ts = dnv_vista_sdk_tsd_dto_header_get_time_span(header);
        REQUIRE(ts != nullptr);

        dnv_vista_sdk_tsd_dto_time_span_set_start(ts, "2020-01-01T00:00:00Z");
        dnv_vista_sdk_tsd_dto_time_span_set_end(ts, "2020-01-02T00:00:00Z");
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_time_span_get_start(ts) } == "2020-01-01T00:00:00Z");
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_time_span_get_end(ts) } == "2020-01-02T00:00:00Z");

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("header system_cfg count and at")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        auto* header = dnv_vista_sdk_tsd_dto_pkg_get_header(pkg);
        REQUIRE(header != nullptr);

        size_t count = dnv_vista_sdk_tsd_dto_header_system_cfg_count(header);
        CHECK(count > 0);

        auto* ref = dnv_vista_sdk_tsd_dto_header_system_cfg_at(header, 0);
        REQUIRE(ref != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_cfg_ref_get_id(ref) }.size() > 0);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_cfg_ref_get_timestamp(ref) }.size() > 0);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("header system_cfg push and remove")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        auto* header = dnv_vista_sdk_tsd_dto_pkg_get_header(pkg);
        REQUIRE(header != nullptr);

        size_t originalCount = dnv_vista_sdk_tsd_dto_header_system_cfg_count(header);
        auto* newRef = dnv_vista_sdk_tsd_dto_header_system_cfg_push(header);
        REQUIRE(newRef != nullptr);
        CHECK(dnv_vista_sdk_tsd_dto_header_system_cfg_count(header) == originalCount + 1);

        dnv_vista_sdk_tsd_dto_header_system_cfg_remove(header, originalCount);
        CHECK(dnv_vista_sdk_tsd_dto_header_system_cfg_count(header) == originalCount);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("cfg_ref set id and timestamp")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        auto* ref = dnv_vista_sdk_tsd_dto_header_system_cfg_at(dnv_vista_sdk_tsd_dto_pkg_get_header(pkg), 0);
        REQUIRE(ref != nullptr);

        dnv_vista_sdk_tsd_dto_cfg_ref_set_id(ref, "new-config-id");
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_cfg_ref_get_id(ref) } == "new-config-id");

        dnv_vista_sdk_tsd_dto_cfg_ref_set_timestamp(ref, "2025-01-01T00:00:00Z");
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_cfg_ref_get_timestamp(ref) } == "2025-01-01T00:00:00Z");

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("pkg tsd push and remove")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        REQUIRE(pkg != nullptr);

        size_t originalCount = dnv_vista_sdk_tsd_dto_pkg_tsd_count(pkg);
        REQUIRE(originalCount > 0);

        auto* newTsd = dnv_vista_sdk_tsd_dto_pkg_tsd_push(pkg);
        REQUIRE(newTsd != nullptr);
        CHECK(dnv_vista_sdk_tsd_dto_pkg_tsd_count(pkg) == originalCount + 1);

        dnv_vista_sdk_tsd_dto_pkg_tsd_remove(pkg, originalCount);
        CHECK(dnv_vista_sdk_tsd_dto_pkg_tsd_count(pkg) == originalCount);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("tsd data_cfg get id and timestamp")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        auto* tsd = dnv_vista_sdk_tsd_dto_pkg_tsd_at(pkg, 0);
        REQUIRE(tsd != nullptr);

        CHECK(dnv_vista_sdk_tsd_dto_tsd_has_data_cfg(tsd) == 1);
        auto* cfg = dnv_vista_sdk_tsd_dto_tsd_get_data_cfg(tsd);
        REQUIRE(cfg != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_cfg_ref_get_id(cfg) }.size() > 0);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_cfg_ref_get_timestamp(cfg) }.size() > 0);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("tsd tabular count and channel ids")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        auto* tsd = dnv_vista_sdk_tsd_dto_pkg_tsd_at(pkg, 0);
        REQUIRE(tsd != nullptr);

        size_t tabCount = dnv_vista_sdk_tsd_dto_tsd_tabular_count(tsd);
        CHECK(tabCount > 0);

        auto* tab = dnv_vista_sdk_tsd_dto_tsd_tabular_at(tsd, 0);
        REQUIRE(tab != nullptr);
        CHECK(dnv_vista_sdk_tsd_dto_tabular_channel_id_count(tab) > 0);

        const char* chId = dnv_vista_sdk_tsd_dto_tabular_channel_id_at(tab, 0);
        REQUIRE(chId != nullptr);
        CHECK(std::string_view{ chId }.size() > 0);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("tabular data_set count and values")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        auto* tsd = dnv_vista_sdk_tsd_dto_pkg_tsd_at(pkg, 0);
        auto* tab = dnv_vista_sdk_tsd_dto_tsd_tabular_at(tsd, 0);
        REQUIRE(tab != nullptr);

        size_t setCount = dnv_vista_sdk_tsd_dto_tabular_data_set_count(tab);
        CHECK(setCount > 0);

        auto* set0 = dnv_vista_sdk_tsd_dto_tabular_data_set_at(tab, 0);
        REQUIRE(set0 != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_tab_set_get_timestamp(set0) }.size() > 0);
        CHECK(dnv_vista_sdk_tsd_dto_tab_set_value_count(set0) > 0);

        const char* val = dnv_vista_sdk_tsd_dto_tab_set_value_at(set0, 0);
        REQUIRE(val != nullptr);
        CHECK(std::string_view{ val }.size() > 0);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("tabular data_set quality count and at")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        auto* tsd = dnv_vista_sdk_tsd_dto_pkg_tsd_at(pkg, 0);
        auto* tab = dnv_vista_sdk_tsd_dto_tsd_tabular_at(tsd, 0);
        auto* set0 = dnv_vista_sdk_tsd_dto_tabular_data_set_at(tab, 0);
        REQUIRE(set0 != nullptr);

        CHECK(dnv_vista_sdk_tsd_dto_tab_set_quality_count(set0) > 0);
        const char* q = dnv_vista_sdk_tsd_dto_tab_set_quality_at(set0, 0);
        REQUIRE(q != nullptr);
        CHECK(std::string_view{ q }.size() > 0);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("tabular set values and quality")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        auto* tsd = dnv_vista_sdk_tsd_dto_pkg_tsd_at(pkg, 0);
        auto* tab = dnv_vista_sdk_tsd_dto_tsd_tabular_at(tsd, 0);
        auto* set0 = dnv_vista_sdk_tsd_dto_tabular_data_set_at(tab, 0);
        REQUIRE(set0 != nullptr);

        const char* vals[] = { "42.0", "43.0" };
        dnv_vista_sdk_tsd_dto_tab_set_set_values(set0, vals, 2);
        CHECK(dnv_vista_sdk_tsd_dto_tab_set_value_count(set0) == 2);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_tab_set_value_at(set0, 0) } == "42.0");

        const char* qs[] = { "0", "0" };
        dnv_vista_sdk_tsd_dto_tab_set_set_quality(set0, qs, 2);
        CHECK(dnv_vista_sdk_tsd_dto_tab_set_quality_count(set0) == 2);

        dnv_vista_sdk_tsd_dto_tab_set_clear_quality(set0);
        CHECK(dnv_vista_sdk_tsd_dto_tab_set_quality_count(set0) == 0);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("tsd event data count and values")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        auto* tsd = dnv_vista_sdk_tsd_dto_pkg_tsd_at(pkg, 0);
        REQUIRE(tsd != nullptr);

        CHECK(dnv_vista_sdk_tsd_dto_tsd_has_event(tsd) == 1);
        auto* event = dnv_vista_sdk_tsd_dto_tsd_get_event(tsd);
        REQUIRE(event != nullptr);
        CHECK(dnv_vista_sdk_tsd_dto_event_data_set_count(event) > 0);

        auto* evSet = dnv_vista_sdk_tsd_dto_event_data_set_at(event, 0);
        REQUIRE(evSet != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_event_set_get_timestamp(evSet) }.size() > 0);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_event_set_get_data_channel_id(evSet) }.size() > 0);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_event_set_get_value(evSet) }.size() > 0);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("event set quality optional get set clear")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        auto* tsd = dnv_vista_sdk_tsd_dto_pkg_tsd_at(pkg, 0);
        auto* event = dnv_vista_sdk_tsd_dto_tsd_get_event(tsd);
        auto* evSet = dnv_vista_sdk_tsd_dto_event_data_set_at(event, 0);
        REQUIRE(evSet != nullptr);

        dnv_vista_sdk_tsd_dto_event_set_set_quality(evSet, "0");
        CHECK(dnv_vista_sdk_tsd_dto_event_set_has_quality(evSet) == 1);
        CHECK(std::string_view{ dnv_vista_sdk_tsd_dto_event_set_get_quality(evSet) } == "0");

        dnv_vista_sdk_tsd_dto_event_set_clear_quality(evSet);
        CHECK(dnv_vista_sdk_tsd_dto_event_set_has_quality(evSet) == 0);
        CHECK(dnv_vista_sdk_tsd_dto_event_set_get_quality(evSet) == nullptr);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("event data push and remove")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        auto* tsd = dnv_vista_sdk_tsd_dto_pkg_tsd_at(pkg, 0);
        auto* event = dnv_vista_sdk_tsd_dto_tsd_get_event(tsd);
        REQUIRE(event != nullptr);

        size_t originalCount = dnv_vista_sdk_tsd_dto_event_data_set_count(event);
        auto* newSet = dnv_vista_sdk_tsd_dto_event_data_set_push(event);
        REQUIRE(newSet != nullptr);
        CHECK(dnv_vista_sdk_tsd_dto_event_data_set_count(event) == originalCount + 1);

        dnv_vista_sdk_tsd_dto_event_data_set_remove(event, originalCount);
        CHECK(dnv_vista_sdk_tsd_dto_event_data_set_count(event) == originalCount);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }

    TEST_CASE("to_dto null returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_tsd_to_dto(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("to_domain null returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_tsd_to_domain(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("to_dto from domain package")
    {
        auto* domain = dnv_vista_sdk_tsd_data_package_from_json(validJson().c_str());
        REQUIRE(domain != nullptr);

        auto* dto = dnv_vista_sdk_tsd_to_dto(domain);
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        REQUIRE(pkg != nullptr);
        CHECK(dnv_vista_sdk_tsd_dto_pkg_tsd_count(pkg) > 0);

        dnv_vista_sdk_tsd_dto_package_free(dto);
        dnv_vista_sdk_tsd_data_package_free(domain);
    }

    TEST_CASE("header ensure and clear")
    {
        auto* dto = dnv_vista_sdk_tsd_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_tsd_dto_package_get_pkg(dto);
        REQUIRE(pkg != nullptr);
        CHECK(dnv_vista_sdk_tsd_dto_pkg_has_header(pkg) == 1);

        dnv_vista_sdk_tsd_dto_pkg_clear_header(pkg);
        CHECK(dnv_vista_sdk_tsd_dto_pkg_has_header(pkg) == 0);
        CHECK(dnv_vista_sdk_tsd_dto_pkg_get_header(pkg) == nullptr);

        dnv_vista_sdk_tsd_dto_pkg_ensure_header(pkg);
        CHECK(dnv_vista_sdk_tsd_dto_pkg_has_header(pkg) == 1);

        dnv_vista_sdk_tsd_dto_package_free(dto);
    }
}
