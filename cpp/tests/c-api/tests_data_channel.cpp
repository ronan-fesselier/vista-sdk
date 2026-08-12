#include <doctest/doctest.h>

#include <dnv/VistaSDK_c_api.h>

#include <string_view>

namespace
{
    constexpr const char* validLocalIdStr =
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet";
} // namespace

TEST_SUITE("c-api::data_channel::restriction")
{
    TEST_CASE("create - fresh restriction has no fields set")
    {
        auto* r = dnv_vista_sdk_dcl_restriction_create();
        REQUIRE(r != nullptr);

        CHECK(dnv_vista_sdk_dcl_restriction_has_fraction_digits(r) == 0);
        CHECK(dnv_vista_sdk_dcl_restriction_has_length(r) == 0);
        CHECK(dnv_vista_sdk_dcl_restriction_enumeration_count(r) == 0);

        dnv_vista_sdk_dcl_restriction_free(r);
    }

    TEST_CASE("set_enumeration/enumeration_at - round-trip")
    {
        auto* r = dnv_vista_sdk_dcl_restriction_create();
        REQUIRE(r != nullptr);

        const char* values[] = { "A", "B", "C" };
        dnv_vista_sdk_dcl_restriction_set_enumeration(r, values, 3);

        REQUIRE(dnv_vista_sdk_dcl_restriction_enumeration_count(r) == 3);
        CHECK(std::string_view{ dnv_vista_sdk_dcl_restriction_enumeration_at(r, 0) } == "A");
        CHECK(std::string_view{ dnv_vista_sdk_dcl_restriction_enumeration_at(r, 2) } == "C");

        dnv_vista_sdk_dcl_restriction_free(r);
    }

    TEST_CASE("set_total_digits - zero sets last error and leaves unset")
    {
        auto* r = dnv_vista_sdk_dcl_restriction_create();
        REQUIRE(r != nullptr);

        dnv_vista_sdk_dcl_restriction_set_total_digits(r, 0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
        CHECK(dnv_vista_sdk_dcl_restriction_has_total_digits(r) == 0);

        dnv_vista_sdk_dcl_restriction_free(r);
    }

    TEST_CASE("set_white_space/white_space - round-trip")
    {
        auto* r = dnv_vista_sdk_dcl_restriction_create();
        REQUIRE(r != nullptr);

        dnv_vista_sdk_dcl_restriction_set_white_space(r, DNV_VISTA_SDK_DCL_WHITE_SPACE_COLLAPSE);
        REQUIRE(dnv_vista_sdk_dcl_restriction_has_white_space(r) == 1);
        CHECK(dnv_vista_sdk_dcl_restriction_white_space(r) == DNV_VISTA_SDK_DCL_WHITE_SPACE_COLLAPSE);

        dnv_vista_sdk_dcl_restriction_free(r);
    }

    TEST_CASE("free - null is a no-op")
    {
        dnv_vista_sdk_dcl_restriction_free(nullptr);
    }
}

TEST_SUITE("c-api::data_channel::range")
{
    TEST_CASE("create - valid bounds")
    {
        auto* range = dnv_vista_sdk_dcl_range_create(0.0, 100.0);
        REQUIRE(range != nullptr);

        CHECK(dnv_vista_sdk_dcl_range_low(range) == doctest::Approx(0.0));
        CHECK(dnv_vista_sdk_dcl_range_high(range) == doctest::Approx(100.0));

        dnv_vista_sdk_dcl_range_free(range);
    }

    TEST_CASE("create - low >= high returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_dcl_range_create(100.0, 0.0) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("set_low - value greater than high sets last error, leaves unchanged")
    {
        auto* range = dnv_vista_sdk_dcl_range_create(0.0, 10.0);
        REQUIRE(range != nullptr);

        dnv_vista_sdk_dcl_range_set_low(range, 20.0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
        CHECK(dnv_vista_sdk_dcl_range_low(range) == doctest::Approx(0.0));

        dnv_vista_sdk_dcl_range_free(range);
    }
}

TEST_SUITE("c-api::data_channel::format")
{
    TEST_CASE("create - valid type")
    {
        auto* format = dnv_vista_sdk_dcl_format_create("Decimal");
        REQUIRE(format != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_dcl_format_type(format) } == "Decimal");

        dnv_vista_sdk_dcl_format_free(format);
    }

    TEST_CASE("create - null returns null")
    {
        CHECK(dnv_vista_sdk_dcl_format_create(nullptr) == nullptr);
    }

    TEST_CASE("set_restriction/restriction - round-trip via copy")
    {
        auto* format = dnv_vista_sdk_dcl_format_create("Decimal");
        REQUIRE(format != nullptr);
        auto* r = dnv_vista_sdk_dcl_restriction_create();
        REQUIRE(r != nullptr);
        dnv_vista_sdk_dcl_restriction_set_total_digits(r, 5);

        dnv_vista_sdk_dcl_format_set_restriction(format, r);

        const auto* restriction = dnv_vista_sdk_dcl_format_restriction(format);
        REQUIRE(restriction != nullptr);
        CHECK(dnv_vista_sdk_dcl_restriction_total_digits(restriction) == 5);

        dnv_vista_sdk_dcl_restriction_free(r);
        dnv_vista_sdk_dcl_format_free(format);
    }

    TEST_CASE("validate_value - valid decimal string")
    {
        auto* format = dnv_vista_sdk_dcl_format_create("Decimal");
        REQUIRE(format != nullptr);

        CHECK(dnv_vista_sdk_dcl_format_validate_value(format, "42.5", nullptr) == 1);

        dnv_vista_sdk_dcl_format_free(format);
    }

    TEST_CASE("validate_value - parsedValue out-param returns the parsed Value")
    {
        auto* format = dnv_vista_sdk_dcl_format_create("Decimal");
        REQUIRE(format != nullptr);

        dnv_vista_sdk_iso19848_value_t* parsedValue = nullptr;
        CHECK(dnv_vista_sdk_dcl_format_validate_value(format, "42.5", &parsedValue) == 1);
        REQUIRE(parsedValue != nullptr);
        CHECK(dnv_vista_sdk_iso19848_value_type(parsedValue) == DNV_VISTA_SDK_ISO19848_VALUE_TYPE_DECIMAL);

        dnv_vista_sdk_iso19848_value_free(parsedValue);
        dnv_vista_sdk_dcl_format_free(format);
    }

    TEST_CASE("validate_value - invalid value leaves parsedValue untouched")
    {
        auto* format = dnv_vista_sdk_dcl_format_create("Decimal");
        REQUIRE(format != nullptr);

        dnv_vista_sdk_iso19848_value_t* parsedValue = reinterpret_cast<dnv_vista_sdk_iso19848_value_t*>(0x1);
        CHECK(dnv_vista_sdk_dcl_format_validate_value(format, "not_a_decimal", &parsedValue) == 0);
        CHECK(parsedValue == reinterpret_cast<dnv_vista_sdk_iso19848_value_t*>(0x1));

        dnv_vista_sdk_dcl_format_free(format);
    }
}

TEST_SUITE("c-api::data_channel::data_channel_type")
{
    TEST_CASE("create - valid type, is_alert reflects type")
    {
        auto* dct = dnv_vista_sdk_dcl_data_channel_type_create("Alert");
        REQUIRE(dct != nullptr);
        CHECK(dnv_vista_sdk_dcl_data_channel_type_is_alert(dct) == 1);

        dnv_vista_sdk_dcl_data_channel_type_free(dct);
    }

    TEST_CASE("set_update_cycle - negative sets last error, leaves unset")
    {
        auto* dct = dnv_vista_sdk_dcl_data_channel_type_create("Inst");
        REQUIRE(dct != nullptr);

        dnv_vista_sdk_dcl_data_channel_type_set_update_cycle(dct, -1.0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
        CHECK(dnv_vista_sdk_dcl_data_channel_type_has_update_cycle(dct) == 0);

        dnv_vista_sdk_dcl_data_channel_type_free(dct);
    }
}

TEST_SUITE("c-api::data_channel::name_object")
{
    TEST_CASE("create_default - matches Annex C naming rule")
    {
        auto* n = dnv_vista_sdk_dcl_name_object_create_default();
        REQUIRE(n != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_dcl_name_object_naming_rule(n) } == "/dnv-v2");

        dnv_vista_sdk_dcl_name_object_free(n);
    }

    TEST_CASE("set_custom_name_objects - takes ownership")
    {
        auto* n = dnv_vista_sdk_dcl_name_object_create_default();
        REQUIRE(n != nullptr);

        auto* doc = dnv_vista_sdk_serializable_document_from_string("custom");
        REQUIRE(doc != nullptr);
        dnv_vista_sdk_dcl_name_object_set_custom_name_objects(n, doc);

        const auto* stored = dnv_vista_sdk_dcl_name_object_custom_name_objects(n);
        REQUIRE(stored != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_serializable_document_as_string(stored) } == "custom");

        dnv_vista_sdk_dcl_name_object_free(n);
    }
}

TEST_SUITE("c-api::data_channel::unit")
{
    TEST_CASE("create - unit symbol round-trips")
    {
        auto* unit = dnv_vista_sdk_dcl_unit_create("Cel");
        REQUIRE(unit != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_dcl_unit_unit_symbol(unit) } == "Cel");
        CHECK(dnv_vista_sdk_dcl_unit_quantity_name(unit) == nullptr);

        dnv_vista_sdk_dcl_unit_free(unit);
    }
}

TEST_SUITE("c-api::data_channel::property")
{
    TEST_CASE("create - Decimal type without Range/Unit fails validation")
    {
        auto* dct = dnv_vista_sdk_dcl_data_channel_type_create("Inst");
        REQUIRE(dct != nullptr);
        auto* format = dnv_vista_sdk_dcl_format_create("Decimal");
        REQUIRE(format != nullptr);

        auto* property = dnv_vista_sdk_dcl_property_create(dct, format);
        REQUIRE(property != nullptr);

        CHECK(dnv_vista_sdk_dcl_property_validate(property) == 0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_dcl_property_free(property);
        dnv_vista_sdk_dcl_format_free(format);
        dnv_vista_sdk_dcl_data_channel_type_free(dct);
    }

    TEST_CASE("create - Decimal type with Range and Unit validates")
    {
        auto* dct = dnv_vista_sdk_dcl_data_channel_type_create("Inst");
        REQUIRE(dct != nullptr);
        auto* format = dnv_vista_sdk_dcl_format_create("Decimal");
        REQUIRE(format != nullptr);
        auto* range = dnv_vista_sdk_dcl_range_create(0.0, 100.0);
        REQUIRE(range != nullptr);
        auto* unit = dnv_vista_sdk_dcl_unit_create("Cel");
        REQUIRE(unit != nullptr);

        auto* property = dnv_vista_sdk_dcl_property_create(dct, format);
        REQUIRE(property != nullptr);
        dnv_vista_sdk_dcl_property_set_range(property, range);
        dnv_vista_sdk_dcl_property_set_unit(property, unit);

        CHECK(dnv_vista_sdk_dcl_property_validate(property) == 1);

        dnv_vista_sdk_dcl_property_free(property);
        dnv_vista_sdk_dcl_unit_free(unit);
        dnv_vista_sdk_dcl_range_free(range);
        dnv_vista_sdk_dcl_format_free(format);
        dnv_vista_sdk_dcl_data_channel_type_free(dct);
    }

    TEST_CASE("create - Alert type without AlertPriority fails validation")
    {
        auto* dct = dnv_vista_sdk_dcl_data_channel_type_create("Alert");
        REQUIRE(dct != nullptr);
        auto* format = dnv_vista_sdk_dcl_format_create("Boolean");
        REQUIRE(format != nullptr);

        auto* property = dnv_vista_sdk_dcl_property_create(dct, format);
        REQUIRE(property != nullptr);

        CHECK(dnv_vista_sdk_dcl_property_validate(property) == 0);

        dnv_vista_sdk_dcl_property_free(property);
        dnv_vista_sdk_dcl_format_free(format);
        dnv_vista_sdk_dcl_data_channel_type_free(dct);
    }
}

TEST_SUITE("c-api::data_channel::configuration_reference")
{
    TEST_CASE("create - id and timestamp round-trip")
    {
        auto ts = dnv_vista_sdk_date_time_offset_utc_now();
        auto* configRef = dnv_vista_sdk_dcl_configuration_reference_create("cfg-1", ts);
        REQUIRE(configRef != nullptr);

        CHECK(std::string_view{ dnv_vista_sdk_dcl_configuration_reference_id(configRef) } == "cfg-1");
        CHECK(dnv_vista_sdk_dcl_configuration_reference_version(configRef) == nullptr);

        dnv_vista_sdk_dcl_configuration_reference_free(configRef);
    }
}

TEST_SUITE("c-api::data_channel::version_information")
{
    TEST_CASE("create_default - matches Annex C defaults")
    {
        auto* v = dnv_vista_sdk_dcl_version_information_create_default();
        REQUIRE(v != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_dcl_version_information_naming_rule(v) }.size() > 0);
        CHECK(std::string_view{ dnv_vista_sdk_dcl_version_information_naming_scheme_version(v) }.size() > 0);

        dnv_vista_sdk_dcl_version_information_free(v);
    }
}

TEST_SUITE("c-api::data_channel::channel_id")
{
    TEST_CASE("create - local_id round-trips")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);

        auto* channelId = dnv_vista_sdk_dcl_channel_id_create(localId);
        REQUIRE(channelId != nullptr);

        const auto* storedLocalId = dnv_vista_sdk_dcl_channel_id_local_id(channelId);
        REQUIRE(storedLocalId != nullptr);
        char* str = dnv_vista_sdk_local_id_to_string(storedLocalId);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == validLocalIdStr);

        dnv_vista_sdk_string_free(str);
        dnv_vista_sdk_dcl_channel_id_free(channelId);
        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("set_short_id - round-trips")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);
        auto* channelId = dnv_vista_sdk_dcl_channel_id_create(localId);
        REQUIRE(channelId != nullptr);

        dnv_vista_sdk_dcl_channel_id_set_short_id(channelId, "SID-1");
        CHECK(std::string_view{ dnv_vista_sdk_dcl_channel_id_short_id(channelId) } == "SID-1");

        dnv_vista_sdk_dcl_channel_id_free(channelId);
        dnv_vista_sdk_local_id_free(localId);
    }
}

TEST_SUITE("c-api::data_channel::header")
{
    TEST_CASE("create - ship_id and data_channel_list_id round-trip")
    {
        auto* imo = dnv_vista_sdk_imo_number_from_string("9074729");
        REQUIRE(imo != nullptr);
        auto* shipId = dnv_vista_sdk_ship_id_from_imo_number(imo);
        REQUIRE(shipId != nullptr);
        auto ts = dnv_vista_sdk_date_time_offset_utc_now();
        auto* configRef = dnv_vista_sdk_dcl_configuration_reference_create("cfg-1", ts);
        REQUIRE(configRef != nullptr);

        auto* header = dnv_vista_sdk_dcl_header_create(shipId, configRef);
        REQUIRE(header != nullptr);

        const auto* storedShipId = dnv_vista_sdk_dcl_header_ship_id(header);
        REQUIRE(storedShipId != nullptr);
        CHECK(dnv_vista_sdk_ship_id_equals(shipId, storedShipId) == 1);

        dnv_vista_sdk_dcl_header_free(header);
        dnv_vista_sdk_dcl_configuration_reference_free(configRef);
        dnv_vista_sdk_ship_id_free(shipId);
        dnv_vista_sdk_imo_number_free(imo);
    }
}

TEST_SUITE("c-api::data_channel::data_channel")
{
    TEST_CASE("create - valid property succeeds")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);
        auto* channelId = dnv_vista_sdk_dcl_channel_id_create(localId);
        REQUIRE(channelId != nullptr);

        auto* dct = dnv_vista_sdk_dcl_data_channel_type_create("Inst");
        REQUIRE(dct != nullptr);
        auto* format = dnv_vista_sdk_dcl_format_create("Boolean");
        REQUIRE(format != nullptr);
        auto* property = dnv_vista_sdk_dcl_property_create(dct, format);
        REQUIRE(property != nullptr);

        auto* dataChannel = dnv_vista_sdk_dcl_data_channel_create(channelId, property);
        REQUIRE(dataChannel != nullptr);

        dnv_vista_sdk_dcl_data_channel_free(dataChannel);
        dnv_vista_sdk_dcl_property_free(property);
        dnv_vista_sdk_dcl_format_free(format);
        dnv_vista_sdk_dcl_data_channel_type_free(dct);
        dnv_vista_sdk_dcl_channel_id_free(channelId);
        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("create - invalid property (Decimal without Range/Unit) returns null and sets last error")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);
        auto* channelId = dnv_vista_sdk_dcl_channel_id_create(localId);
        REQUIRE(channelId != nullptr);

        auto* dct = dnv_vista_sdk_dcl_data_channel_type_create("Inst");
        REQUIRE(dct != nullptr);
        auto* format = dnv_vista_sdk_dcl_format_create("Decimal");
        REQUIRE(format != nullptr);
        auto* property = dnv_vista_sdk_dcl_property_create(dct, format);
        REQUIRE(property != nullptr);

        CHECK(dnv_vista_sdk_dcl_data_channel_create(channelId, property) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_dcl_property_free(property);
        dnv_vista_sdk_dcl_format_free(format);
        dnv_vista_sdk_dcl_data_channel_type_free(dct);
        dnv_vista_sdk_dcl_channel_id_free(channelId);
        dnv_vista_sdk_local_id_free(localId);
    }
}

TEST_SUITE("c-api::data_channel::data_channel_list")
{
    TEST_CASE("create - empty list")
    {
        auto* list = dnv_vista_sdk_dcl_data_channel_list_create();
        REQUIRE(list != nullptr);
        CHECK(dnv_vista_sdk_dcl_data_channel_list_size(list) == 0);

        dnv_vista_sdk_dcl_data_channel_list_free(list);
    }

    TEST_CASE("add/at/from_local_id/from_short_id - single entry")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);
        auto* channelId = dnv_vista_sdk_dcl_channel_id_create(localId);
        REQUIRE(channelId != nullptr);
        dnv_vista_sdk_dcl_channel_id_set_short_id(channelId, "SID-1");

        auto* dct = dnv_vista_sdk_dcl_data_channel_type_create("Inst");
        auto* format = dnv_vista_sdk_dcl_format_create("Boolean");
        auto* property = dnv_vista_sdk_dcl_property_create(dct, format);
        auto* dataChannel = dnv_vista_sdk_dcl_data_channel_create(channelId, property);
        REQUIRE(dataChannel != nullptr);

        auto* list = dnv_vista_sdk_dcl_data_channel_list_create();
        REQUIRE(dnv_vista_sdk_dcl_data_channel_list_add(list, dataChannel) == 1);
        CHECK(dnv_vista_sdk_dcl_data_channel_list_size(list) == 1);

        CHECK(dnv_vista_sdk_dcl_data_channel_list_at(list, 0) != nullptr);
        CHECK(dnv_vista_sdk_dcl_data_channel_list_from_short_id(list, "SID-1") != nullptr);
        CHECK(dnv_vista_sdk_dcl_data_channel_list_from_local_id(list, localId) != nullptr);
        CHECK(dnv_vista_sdk_dcl_data_channel_list_from_short_id(list, "missing") == nullptr);

        dnv_vista_sdk_dcl_data_channel_list_free(list);
        dnv_vista_sdk_dcl_data_channel_free(dataChannel);
        dnv_vista_sdk_dcl_property_free(property);
        dnv_vista_sdk_dcl_format_free(format);
        dnv_vista_sdk_dcl_data_channel_type_free(dct);
        dnv_vista_sdk_dcl_channel_id_free(channelId);
        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("add - duplicate LocalId fails")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);
        auto* channelId = dnv_vista_sdk_dcl_channel_id_create(localId);
        auto* dct = dnv_vista_sdk_dcl_data_channel_type_create("Inst");
        auto* format = dnv_vista_sdk_dcl_format_create("Boolean");
        auto* property = dnv_vista_sdk_dcl_property_create(dct, format);
        auto* dataChannel = dnv_vista_sdk_dcl_data_channel_create(channelId, property);
        REQUIRE(dataChannel != nullptr);

        auto* list = dnv_vista_sdk_dcl_data_channel_list_create();
        REQUIRE(dnv_vista_sdk_dcl_data_channel_list_add(list, dataChannel) == 1);
        CHECK(dnv_vista_sdk_dcl_data_channel_list_add(list, dataChannel) == 0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_dcl_data_channel_list_free(list);
        dnv_vista_sdk_dcl_data_channel_free(dataChannel);
        dnv_vista_sdk_dcl_property_free(property);
        dnv_vista_sdk_dcl_format_free(format);
        dnv_vista_sdk_dcl_data_channel_type_free(dct);
        dnv_vista_sdk_dcl_channel_id_free(channelId);
        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("remove/clear")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);
        auto* channelId = dnv_vista_sdk_dcl_channel_id_create(localId);
        auto* dct = dnv_vista_sdk_dcl_data_channel_type_create("Inst");
        auto* format = dnv_vista_sdk_dcl_format_create("Boolean");
        auto* property = dnv_vista_sdk_dcl_property_create(dct, format);
        auto* dataChannel = dnv_vista_sdk_dcl_data_channel_create(channelId, property);
        REQUIRE(dataChannel != nullptr);

        auto* list = dnv_vista_sdk_dcl_data_channel_list_create();
        dnv_vista_sdk_dcl_data_channel_list_add(list, dataChannel);
        REQUIRE(dnv_vista_sdk_dcl_data_channel_list_size(list) == 1);

        CHECK(dnv_vista_sdk_dcl_data_channel_list_remove(list, dataChannel) == 1);
        CHECK(dnv_vista_sdk_dcl_data_channel_list_size(list) == 0);

        dnv_vista_sdk_dcl_data_channel_list_add(list, dataChannel);
        dnv_vista_sdk_dcl_data_channel_list_clear(list);
        CHECK(dnv_vista_sdk_dcl_data_channel_list_size(list) == 0);

        dnv_vista_sdk_dcl_data_channel_list_free(list);
        dnv_vista_sdk_dcl_data_channel_free(dataChannel);
        dnv_vista_sdk_dcl_property_free(property);
        dnv_vista_sdk_dcl_format_free(format);
        dnv_vista_sdk_dcl_data_channel_type_free(dct);
        dnv_vista_sdk_dcl_channel_id_free(channelId);
        dnv_vista_sdk_local_id_free(localId);
    }
}

TEST_SUITE("c-api::data_channel::package")
{
    TEST_CASE("create/header/data_channel_list - round-trip")
    {
        auto* imo = dnv_vista_sdk_imo_number_from_string("9074729");
        auto* shipId = dnv_vista_sdk_ship_id_from_imo_number(imo);
        auto ts = dnv_vista_sdk_date_time_offset_utc_now();
        auto* configRef = dnv_vista_sdk_dcl_configuration_reference_create("cfg-1", ts);
        auto* header = dnv_vista_sdk_dcl_header_create(shipId, configRef);
        REQUIRE(header != nullptr);

        auto* list = dnv_vista_sdk_dcl_data_channel_list_create();
        REQUIRE(list != nullptr);

        auto* package = dnv_vista_sdk_dcl_package_create(header, list);
        REQUIRE(package != nullptr);

        CHECK(dnv_vista_sdk_dcl_package_header(package) != nullptr);
        CHECK(dnv_vista_sdk_dcl_package_data_channel_list(package) != nullptr);

        dnv_vista_sdk_dcl_package_free(package);
        dnv_vista_sdk_dcl_data_channel_list_free(list);
        dnv_vista_sdk_dcl_header_free(header);
        dnv_vista_sdk_dcl_configuration_reference_free(configRef);
        dnv_vista_sdk_ship_id_free(shipId);
        dnv_vista_sdk_imo_number_free(imo);
    }
}

TEST_SUITE("c-api::data_channel::list_package")
{
    TEST_CASE("create - convenience data_channel_list accessor matches package's")
    {
        auto* imo = dnv_vista_sdk_imo_number_from_string("9074729");
        auto* shipId = dnv_vista_sdk_ship_id_from_imo_number(imo);
        auto ts = dnv_vista_sdk_date_time_offset_utc_now();
        auto* configRef = dnv_vista_sdk_dcl_configuration_reference_create("cfg-1", ts);
        auto* header = dnv_vista_sdk_dcl_header_create(shipId, configRef);
        auto* list = dnv_vista_sdk_dcl_data_channel_list_create();
        auto* package = dnv_vista_sdk_dcl_package_create(header, list);
        REQUIRE(package != nullptr);

        auto* listPackage = dnv_vista_sdk_dcl_list_package_create(package);
        REQUIRE(listPackage != nullptr);

        CHECK(dnv_vista_sdk_dcl_list_package_package(listPackage) != nullptr);
        CHECK(dnv_vista_sdk_dcl_list_package_data_channel_list(listPackage) != nullptr);

        dnv_vista_sdk_dcl_list_package_free(listPackage);
        dnv_vista_sdk_dcl_package_free(package);
        dnv_vista_sdk_dcl_data_channel_list_free(list);
        dnv_vista_sdk_dcl_header_free(header);
        dnv_vista_sdk_dcl_configuration_reference_free(configRef);
        dnv_vista_sdk_ship_id_free(shipId);
        dnv_vista_sdk_imo_number_free(imo);
    }

    TEST_CASE("create - null returns null")
    {
        CHECK(dnv_vista_sdk_dcl_list_package_create(nullptr) == nullptr);
    }
}
