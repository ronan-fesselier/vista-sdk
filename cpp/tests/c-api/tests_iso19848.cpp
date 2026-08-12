#include <doctest/doctest.h>

#include <dnv/VistaSDK_c_api.h>

#include <cstring>
#include <string_view>

TEST_SUITE("c-api::iso19848")
{
    TEST_CASE("dnv_vista_sdk_iso19848_instance - returns a stable pointer")
    {
        const auto* a = dnv_vista_sdk_iso19848_instance();
        const auto* b = dnv_vista_sdk_iso19848_instance();
        REQUIRE(a != nullptr);
        CHECK(a == b);
    }

    TEST_CASE("dnv_vista_sdk_iso19848_version_count / version_at / latest")
    {
        const auto* iso = dnv_vista_sdk_iso19848_instance();
        REQUIRE(iso != nullptr);

        const size_t count = dnv_vista_sdk_iso19848_version_count(iso);
        CHECK(count > 0);

        for (size_t i = 0; i < count; ++i)
        {
            const auto version = dnv_vista_sdk_iso19848_version_at(iso, i);
            CHECK((version == DNV_VISTA_SDK_ISO19848_VERSION_V2018 || version == DNV_VISTA_SDK_ISO19848_VERSION_V2024));
        }

        CHECK(dnv_vista_sdk_iso19848_latest(iso) == DNV_VISTA_SDK_ISO19848_VERSION_V2024);
    }

    TEST_CASE("dnv_vista_sdk_iso19848_version_at - out of range sets last error")
    {
        const auto* iso = dnv_vista_sdk_iso19848_instance();
        REQUIRE(iso != nullptr);

        static_cast<void>(dnv_vista_sdk_iso19848_version_at(iso, 1000));
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_iso19848_data_channel_type_names - load and iterate")
    {
        const auto* iso = dnv_vista_sdk_iso19848_instance();
        REQUIRE(iso != nullptr);

        auto* typeNames = dnv_vista_sdk_iso19848_data_channel_type_names(iso, DNV_VISTA_SDK_ISO19848_VERSION_V2024);
        REQUIRE(typeNames != nullptr);

        const size_t count = dnv_vista_sdk_iso19848_data_channel_type_names_count(typeNames);
        CHECK(count > 0);

        for (size_t i = 0; i < count; ++i)
        {
            const auto* entry = dnv_vista_sdk_iso19848_data_channel_type_names_at(typeNames, i);
            REQUIRE(entry != nullptr);
            CHECK(dnv_vista_sdk_iso19848_data_channel_type_name_type(entry) != nullptr);
            CHECK(dnv_vista_sdk_iso19848_data_channel_type_name_description(entry) != nullptr);
        }

        dnv_vista_sdk_iso19848_data_channel_type_names_free(typeNames);
    }

    TEST_CASE("dnv_vista_sdk_iso19848_data_channel_type_names_from_string")
    {
        const auto* iso = dnv_vista_sdk_iso19848_instance();
        REQUIRE(iso != nullptr);

        auto* typeNames = dnv_vista_sdk_iso19848_data_channel_type_names(iso, DNV_VISTA_SDK_ISO19848_VERSION_V2024);
        REQUIRE(typeNames != nullptr);

        SUBCASE("known type")
        {
            auto* entry = dnv_vista_sdk_iso19848_data_channel_type_names_from_string(typeNames, "Inst");
            REQUIRE(entry != nullptr);
            CHECK(std::string_view{ dnv_vista_sdk_iso19848_data_channel_type_name_type(entry) } == "Inst");
            dnv_vista_sdk_iso19848_data_channel_type_name_free(entry);
        }

        SUBCASE("unknown type returns null and sets last error")
        {
            auto* entry = dnv_vista_sdk_iso19848_data_channel_type_names_from_string(typeNames, "not-a-type");
            CHECK(entry == nullptr);
            CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
        }

        dnv_vista_sdk_iso19848_data_channel_type_names_free(typeNames);
    }

    TEST_CASE("dnv_vista_sdk_iso19848_format_data_types - load and iterate")
    {
        const auto* iso = dnv_vista_sdk_iso19848_instance();
        REQUIRE(iso != nullptr);

        auto* formatDataTypes = dnv_vista_sdk_iso19848_format_data_types(iso, DNV_VISTA_SDK_ISO19848_VERSION_V2024);
        REQUIRE(formatDataTypes != nullptr);

        const size_t count = dnv_vista_sdk_iso19848_format_data_types_count(formatDataTypes);
        CHECK(count > 0);

        for (size_t i = 0; i < count; ++i)
        {
            const auto* entry = dnv_vista_sdk_iso19848_format_data_types_at(formatDataTypes, i);
            REQUIRE(entry != nullptr);
            CHECK(dnv_vista_sdk_iso19848_format_data_type_type(entry) != nullptr);
            CHECK(dnv_vista_sdk_iso19848_format_data_type_description(entry) != nullptr);
        }

        dnv_vista_sdk_iso19848_format_data_types_free(formatDataTypes);
    }

    TEST_CASE("dnv_vista_sdk_iso19848_format_data_type_validate")
    {
        const auto* iso = dnv_vista_sdk_iso19848_instance();
        REQUIRE(iso != nullptr);

        auto* formatDataTypes = dnv_vista_sdk_iso19848_format_data_types(iso, DNV_VISTA_SDK_ISO19848_VERSION_V2024);
        REQUIRE(formatDataTypes != nullptr);

        SUBCASE("Decimal - valid")
        {
            auto* decimalType = dnv_vista_sdk_iso19848_format_data_types_from_string(formatDataTypes, "Decimal");
            REQUIRE(decimalType != nullptr);

            dnv_vista_sdk_iso19848_value_t* value = nullptr;
            CHECK(dnv_vista_sdk_iso19848_format_data_type_validate(decimalType, "0.1", &value) == 1);
            REQUIRE(value != nullptr);
            CHECK(dnv_vista_sdk_iso19848_value_type(value) == DNV_VISTA_SDK_ISO19848_VALUE_TYPE_DECIMAL);

            dnv_vista_sdk_iso19848_value_free(value);
            dnv_vista_sdk_iso19848_format_data_type_free(decimalType);
        }

        SUBCASE("Boolean - invalid returns 0 and sets last error")
        {
            auto* booleanType = dnv_vista_sdk_iso19848_format_data_types_from_string(formatDataTypes, "Boolean");
            REQUIRE(booleanType != nullptr);

            dnv_vista_sdk_iso19848_value_t* value = nullptr;
            CHECK(dnv_vista_sdk_iso19848_format_data_type_validate(booleanType, "yes", &value) == 0);
            CHECK(value == nullptr);
            CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

            dnv_vista_sdk_iso19848_format_data_type_free(booleanType);
        }

        dnv_vista_sdk_iso19848_format_data_types_free(formatDataTypes);
    }

    TEST_CASE("dnv_vista_sdk_iso19848_value - construction and type accessor")
    {
        SUBCASE("String")
        {
            auto* value = dnv_vista_sdk_iso19848_value_from_string("test");
            REQUIRE(value != nullptr);
            CHECK(dnv_vista_sdk_iso19848_value_type(value) == DNV_VISTA_SDK_ISO19848_VALUE_TYPE_STRING);
            CHECK(std::string_view{ dnv_vista_sdk_iso19848_value_string(value) } == "test");

            int boolResult = 0;
            CHECK(dnv_vista_sdk_iso19848_value_boolean(value, &boolResult) == 0);

            dnv_vista_sdk_iso19848_value_free(value);
        }

        SUBCASE("Integer")
        {
            auto* value = dnv_vista_sdk_iso19848_value_from_integer(42);
            REQUIRE(value != nullptr);
            CHECK(dnv_vista_sdk_iso19848_value_type(value) == DNV_VISTA_SDK_ISO19848_VALUE_TYPE_INTEGER);

            int64_t result = 0;
            CHECK(dnv_vista_sdk_iso19848_value_integer(value, &result) == 1);
            CHECK(result == 42);
            CHECK(dnv_vista_sdk_iso19848_value_string(value) == nullptr);

            dnv_vista_sdk_iso19848_value_free(value);
        }

        SUBCASE("Boolean")
        {
            auto* value = dnv_vista_sdk_iso19848_value_from_boolean(1);
            REQUIRE(value != nullptr);
            CHECK(dnv_vista_sdk_iso19848_value_type(value) == DNV_VISTA_SDK_ISO19848_VALUE_TYPE_BOOLEAN);

            int result = 0;
            CHECK(dnv_vista_sdk_iso19848_value_boolean(value, &result) == 1);
            CHECK(result == 1);

            dnv_vista_sdk_iso19848_value_free(value);
        }

        SUBCASE("Decimal")
        {
            dnv_vista_sdk_decimal_t decimal{};
            REQUIRE(dnv_vista_sdk_decimal_from_string("3.14", &decimal) == 1);

            auto* value = dnv_vista_sdk_iso19848_value_from_decimal(decimal);
            REQUIRE(value != nullptr);
            CHECK(dnv_vista_sdk_iso19848_value_type(value) == DNV_VISTA_SDK_ISO19848_VALUE_TYPE_DECIMAL);

            dnv_vista_sdk_decimal_t result{};
            CHECK(dnv_vista_sdk_iso19848_value_decimal(value, &result) == 1);
            CHECK(dnv_vista_sdk_decimal_equals(result, decimal) == 1);

            dnv_vista_sdk_iso19848_value_free(value);
        }

        SUBCASE("DateTime")
        {
            const auto dto = dnv_vista_sdk_date_time_offset_epoch();

            auto* value = dnv_vista_sdk_iso19848_value_from_date_time(dto);
            REQUIRE(value != nullptr);
            CHECK(dnv_vista_sdk_iso19848_value_type(value) == DNV_VISTA_SDK_ISO19848_VALUE_TYPE_DATE_TIME);

            dnv_vista_sdk_date_time_offset_t result{};
            CHECK(dnv_vista_sdk_iso19848_value_date_time(value, &result) == 1);
            CHECK(dnv_vista_sdk_date_time_offset_equals(result, dto) == 1);

            dnv_vista_sdk_iso19848_value_free(value);
        }
    }

    TEST_CASE("dnv_vista_sdk_iso19848_value_to_string")
    {
        auto* value = dnv_vista_sdk_iso19848_value_from_integer(123);
        REQUIRE(value != nullptr);

        char* str = dnv_vista_sdk_iso19848_value_to_string(value);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "123");

        dnv_vista_sdk_iso19848_value_string_free(str);
        dnv_vista_sdk_iso19848_value_free(value);
    }

    TEST_CASE("null argument handling")
    {
        CHECK(dnv_vista_sdk_iso19848_version_count(nullptr) == 0);
        CHECK(dnv_vista_sdk_iso19848_data_channel_type_names(nullptr, DNV_VISTA_SDK_ISO19848_VERSION_V2024) == nullptr);
        CHECK(dnv_vista_sdk_iso19848_format_data_types(nullptr, DNV_VISTA_SDK_ISO19848_VERSION_V2024) == nullptr);
        CHECK(dnv_vista_sdk_iso19848_data_channel_type_name_type(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_iso19848_format_data_type_type(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_iso19848_value_type(nullptr) == DNV_VISTA_SDK_ISO19848_VALUE_TYPE_STRING);
        CHECK(dnv_vista_sdk_iso19848_value_string(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_iso19848_value_to_string(nullptr) == nullptr);

        dnv_vista_sdk_iso19848_data_channel_type_name_free(nullptr);
        dnv_vista_sdk_iso19848_data_channel_type_names_free(nullptr);
        dnv_vista_sdk_iso19848_format_data_type_free(nullptr);
        dnv_vista_sdk_iso19848_format_data_types_free(nullptr);
        dnv_vista_sdk_iso19848_value_free(nullptr);
        dnv_vista_sdk_iso19848_value_string_free(nullptr);
    }
}
