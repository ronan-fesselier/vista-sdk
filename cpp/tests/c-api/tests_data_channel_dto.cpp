#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/transport/serialization/json/datachannel/data_channel_dto.h>
#include <dnv/vista/sdk/c/transport/serialization/json/datachannel/data_channel_json.h>

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

TEST_SUITE("c-api::data_channel_dto")
{
    TEST_CASE("from_json round-trip preserves channel count")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        REQUIRE(pkg != nullptr);

        auto* list = dnv_vista_sdk_dcl_dto_pkg_get_channel_list(pkg);
        REQUIRE(list != nullptr);
        CHECK(dnv_vista_sdk_dcl_dto_ch_list_count(list) > 0);

        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("from_json null returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_dcl_dto_from_json(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("from_json malformed JSON returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_dcl_dto_from_json("{ not valid json") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("to_json produces non-empty string")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        char* json = dnv_vista_sdk_dcl_dto_to_json(dto, 0);
        REQUIRE(json != nullptr);
        CHECK(std::string_view{ json }.size() > 0);

        dnv_vista_sdk_string_free(json);
        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("to_domain round-trip preserves ship id")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        REQUIRE(pkg != nullptr);
        auto* header = dnv_vista_sdk_dcl_dto_pkg_get_header(pkg);
        REQUIRE(header != nullptr);
        const char* shipId = dnv_vista_sdk_dcl_dto_header_get_ship_id(header);
        REQUIRE(shipId != nullptr);
        std::string originalShipId = shipId;

        auto* domain = dnv_vista_sdk_dcl_to_domain(dto);
        REQUIRE(domain != nullptr);
        auto* dto2 = dnv_vista_sdk_dcl_to_dto(domain);
        REQUIRE(dto2 != nullptr);

        auto* pkg2 = dnv_vista_sdk_dcl_dto_package_get_pkg(dto2);
        REQUIRE(pkg2 != nullptr);
        auto* header2 = dnv_vista_sdk_dcl_dto_pkg_get_header(pkg2);
        REQUIRE(header2 != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_dcl_dto_header_get_ship_id(header2) } == originalShipId);

        dnv_vista_sdk_dcl_list_package_free(domain);
        dnv_vista_sdk_dcl_dto_package_free(dto2);
        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("header ship_id get and set")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        auto* header = dnv_vista_sdk_dcl_dto_pkg_get_header(pkg);
        REQUIRE(header != nullptr);

        dnv_vista_sdk_dcl_dto_header_set_ship_id(header, "IMO9999999");
        CHECK(std::string_view{ dnv_vista_sdk_dcl_dto_header_get_ship_id(header) } == "IMO9999999");

        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("header author optional get set clear")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        auto* header = dnv_vista_sdk_dcl_dto_pkg_get_header(pkg);
        REQUIRE(header != nullptr);

        dnv_vista_sdk_dcl_dto_header_set_author(header, "Test Author");
        CHECK(dnv_vista_sdk_dcl_dto_header_has_author(header) == 1);
        CHECK(std::string_view{ dnv_vista_sdk_dcl_dto_header_get_author(header) } == "Test Author");

        dnv_vista_sdk_dcl_dto_header_clear_author(header);
        CHECK(dnv_vista_sdk_dcl_dto_header_has_author(header) == 0);
        CHECK(dnv_vista_sdk_dcl_dto_header_get_author(header) == nullptr);

        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("header ver_info ensure and clear")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        auto* header = dnv_vista_sdk_dcl_dto_pkg_get_header(pkg);
        REQUIRE(header != nullptr);

        dnv_vista_sdk_dcl_dto_header_ensure_ver_info(header);
        CHECK(dnv_vista_sdk_dcl_dto_header_has_ver_info(header) == 1);

        auto* vi = dnv_vista_sdk_dcl_dto_header_get_ver_info(header);
        REQUIRE(vi != nullptr);
        dnv_vista_sdk_dcl_dto_ver_info_set_naming_rule(vi, "VIS");
        CHECK(std::string_view{ dnv_vista_sdk_dcl_dto_ver_info_get_naming_rule(vi) } == "VIS");

        dnv_vista_sdk_dcl_dto_header_clear_ver_info(header);
        CHECK(dnv_vista_sdk_dcl_dto_header_has_ver_info(header) == 0);

        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("cfg_ref id and timestamp")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        auto* header = dnv_vista_sdk_dcl_dto_pkg_get_header(pkg);
        auto* ref = dnv_vista_sdk_dcl_dto_header_get_cfg_ref(header);
        REQUIRE(ref != nullptr);

        const char* id = dnv_vista_sdk_dcl_dto_cfg_ref_get_id(ref);
        REQUIRE(id != nullptr);
        CHECK(std::string_view{ id }.size() > 0);

        const char* ts = dnv_vista_sdk_dcl_dto_cfg_ref_get_timestamp(ref);
        REQUIRE(ts != nullptr);
        CHECK(std::string_view{ ts }.size() > 0);

        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("channel list at and push and remove")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        auto* list = dnv_vista_sdk_dcl_dto_pkg_get_channel_list(pkg);
        REQUIRE(list != nullptr);

        size_t originalCount = dnv_vista_sdk_dcl_dto_ch_list_count(list);
        REQUIRE(originalCount > 0);

        auto* ch0 = dnv_vista_sdk_dcl_dto_ch_list_at(list, 0);
        REQUIRE(ch0 != nullptr);

        auto* newCh = dnv_vista_sdk_dcl_dto_ch_list_push(list);
        REQUIRE(newCh != nullptr);
        CHECK(dnv_vista_sdk_dcl_dto_ch_list_count(list) == originalCount + 1);

        dnv_vista_sdk_dcl_dto_ch_list_remove(list, originalCount);
        CHECK(dnv_vista_sdk_dcl_dto_ch_list_count(list) == originalCount);

        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("channel id local_id is non-empty")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        auto* list = dnv_vista_sdk_dcl_dto_pkg_get_channel_list(pkg);
        auto* ch = dnv_vista_sdk_dcl_dto_ch_list_at(list, 0);
        REQUIRE(ch != nullptr);

        auto* id = dnv_vista_sdk_dcl_dto_channel_get_id(ch);
        REQUIRE(id != nullptr);
        const char* localId = dnv_vista_sdk_dcl_dto_ch_id_get_local_id(id);
        REQUIRE(localId != nullptr);
        CHECK(std::string_view{ localId }.size() > 0);

        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("property format type is non-empty")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        auto* list = dnv_vista_sdk_dcl_dto_pkg_get_channel_list(pkg);
        auto* ch = dnv_vista_sdk_dcl_dto_ch_list_at(list, 0);
        REQUIRE(ch != nullptr);

        auto* prop = dnv_vista_sdk_dcl_dto_channel_get_property(ch);
        REQUIRE(prop != nullptr);

        auto* fmt = dnv_vista_sdk_dcl_dto_property_get_format(prop);
        REQUIRE(fmt != nullptr);
        const char* ftype = dnv_vista_sdk_dcl_dto_format_get_type(fmt);
        REQUIRE(ftype != nullptr);
        CHECK(std::string_view{ ftype }.size() > 0);

        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("property channel type set and get")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        auto* list = dnv_vista_sdk_dcl_dto_pkg_get_channel_list(pkg);
        auto* ch = dnv_vista_sdk_dcl_dto_ch_list_at(list, 0);
        auto* prop = dnv_vista_sdk_dcl_dto_channel_get_property(ch);
        auto* ct = dnv_vista_sdk_dcl_dto_property_get_ch_type(prop);
        REQUIRE(ct != nullptr);

        const char* originalType = dnv_vista_sdk_dcl_dto_ch_type_get_type(ct);
        REQUIRE(originalType != nullptr);
        std::string saved = originalType;

        dnv_vista_sdk_dcl_dto_ch_type_set_type(ct, "Inst");
        CHECK(std::string_view{ dnv_vista_sdk_dcl_dto_ch_type_get_type(ct) } == "Inst");

        dnv_vista_sdk_dcl_dto_ch_type_set_type(ct, saved.c_str());
        CHECK(std::string_view{ dnv_vista_sdk_dcl_dto_ch_type_get_type(ct) } == saved);

        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("range ensure set and clear")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        auto* list = dnv_vista_sdk_dcl_dto_pkg_get_channel_list(pkg);
        auto* ch = dnv_vista_sdk_dcl_dto_ch_list_at(list, 0);
        auto* prop = dnv_vista_sdk_dcl_dto_channel_get_property(ch);
        REQUIRE(prop != nullptr);

        dnv_vista_sdk_dcl_dto_property_ensure_range(prop);
        CHECK(dnv_vista_sdk_dcl_dto_property_has_range(prop) == 1);

        auto* range = dnv_vista_sdk_dcl_dto_property_get_range(prop);
        REQUIRE(range != nullptr);
        dnv_vista_sdk_dcl_dto_range_set_low(range, -10.0);
        dnv_vista_sdk_dcl_dto_range_set_high(range, 100.0);
        CHECK(dnv_vista_sdk_dcl_dto_range_get_low(range) == doctest::Approx(-10.0));
        CHECK(dnv_vista_sdk_dcl_dto_range_get_high(range) == doctest::Approx(100.0));

        dnv_vista_sdk_dcl_dto_property_clear_range(prop);
        CHECK(dnv_vista_sdk_dcl_dto_property_has_range(prop) == 0);

        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("unit ensure symbol and quantity name")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        auto* list = dnv_vista_sdk_dcl_dto_pkg_get_channel_list(pkg);
        auto* ch = dnv_vista_sdk_dcl_dto_ch_list_at(list, 0);
        auto* prop = dnv_vista_sdk_dcl_dto_channel_get_property(ch);

        dnv_vista_sdk_dcl_dto_property_ensure_unit(prop);
        CHECK(dnv_vista_sdk_dcl_dto_property_has_unit(prop) == 1);

        auto* unit = dnv_vista_sdk_dcl_dto_property_get_unit(prop);
        REQUIRE(unit != nullptr);
        dnv_vista_sdk_dcl_dto_unit_set_symbol(unit, "m/s");
        CHECK(std::string_view{ dnv_vista_sdk_dcl_dto_unit_get_symbol(unit) } == "m/s");

        dnv_vista_sdk_dcl_dto_unit_set_quantity_name(unit, "Speed");
        CHECK(dnv_vista_sdk_dcl_dto_unit_has_quantity_name(unit) == 1);
        CHECK(std::string_view{ dnv_vista_sdk_dcl_dto_unit_get_quantity_name(unit) } == "Speed");
        dnv_vista_sdk_dcl_dto_unit_clear_quantity_name(unit);
        CHECK(dnv_vista_sdk_dcl_dto_unit_has_quantity_name(unit) == 0);

        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("restriction enumeration set and read")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        auto* list = dnv_vista_sdk_dcl_dto_pkg_get_channel_list(pkg);
        auto* ch = dnv_vista_sdk_dcl_dto_ch_list_at(list, 0);
        auto* prop = dnv_vista_sdk_dcl_dto_channel_get_property(ch);
        auto* fmt = dnv_vista_sdk_dcl_dto_property_get_format(prop);

        dnv_vista_sdk_dcl_dto_format_ensure_restriction(fmt);
        CHECK(dnv_vista_sdk_dcl_dto_format_has_restriction(fmt) == 1);

        auto* restr = dnv_vista_sdk_dcl_dto_format_get_restriction(fmt);
        REQUIRE(restr != nullptr);

        const char* vals[] = { "On", "Off", "Standby" };
        dnv_vista_sdk_dcl_dto_restriction_set_enumeration(restr, vals, 3);
        CHECK(dnv_vista_sdk_dcl_dto_restriction_enumeration_count(restr) == 3);
        CHECK(std::string_view{ dnv_vista_sdk_dcl_dto_restriction_enumeration_at(restr, 0) } == "On");
        CHECK(std::string_view{ dnv_vista_sdk_dcl_dto_restriction_enumeration_at(restr, 2) } == "Standby");

        dnv_vista_sdk_dcl_dto_restriction_clear_enumeration(restr);
        CHECK(dnv_vista_sdk_dcl_dto_restriction_enumeration_count(restr) == 0);

        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("restriction numeric facets set and clear")
    {
        auto* dto = dnv_vista_sdk_dcl_dto_from_json(validJson().c_str());
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        auto* list = dnv_vista_sdk_dcl_dto_pkg_get_channel_list(pkg);
        auto* ch = dnv_vista_sdk_dcl_dto_ch_list_at(list, 0);
        auto* prop = dnv_vista_sdk_dcl_dto_channel_get_property(ch);
        auto* fmt = dnv_vista_sdk_dcl_dto_property_get_format(prop);
        dnv_vista_sdk_dcl_dto_format_ensure_restriction(fmt);
        auto* restr = dnv_vista_sdk_dcl_dto_format_get_restriction(fmt);
        REQUIRE(restr != nullptr);

        dnv_vista_sdk_dcl_dto_restriction_set_min_inclusive(restr, 0.0);
        dnv_vista_sdk_dcl_dto_restriction_set_max_inclusive(restr, 360.0);
        CHECK(dnv_vista_sdk_dcl_dto_restriction_has_min_inclusive(restr) == 1);
        CHECK(dnv_vista_sdk_dcl_dto_restriction_get_min_inclusive(restr) == doctest::Approx(0.0));
        CHECK(dnv_vista_sdk_dcl_dto_restriction_get_max_inclusive(restr) == doctest::Approx(360.0));

        dnv_vista_sdk_dcl_dto_restriction_clear_min_inclusive(restr);
        dnv_vista_sdk_dcl_dto_restriction_clear_max_inclusive(restr);
        CHECK(dnv_vista_sdk_dcl_dto_restriction_has_min_inclusive(restr) == 0);
        CHECK(dnv_vista_sdk_dcl_dto_restriction_has_max_inclusive(restr) == 0);

        dnv_vista_sdk_dcl_dto_package_free(dto);
    }

    TEST_CASE("to_dto null returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_dcl_to_dto(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("to_domain null returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_dcl_to_domain(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("to_dto from domain package")
    {
        auto* domain = dnv_vista_sdk_dcl_list_package_from_json(validJson().c_str());
        REQUIRE(domain != nullptr);

        auto* dto = dnv_vista_sdk_dcl_to_dto(domain);
        REQUIRE(dto != nullptr);

        auto* pkg = dnv_vista_sdk_dcl_dto_package_get_pkg(dto);
        REQUIRE(pkg != nullptr);
        auto* list = dnv_vista_sdk_dcl_dto_pkg_get_channel_list(pkg);
        REQUIRE(list != nullptr);
        CHECK(dnv_vista_sdk_dcl_dto_ch_list_count(list) > 0);

        dnv_vista_sdk_dcl_dto_package_free(dto);
        dnv_vista_sdk_dcl_list_package_free(domain);
    }
}
