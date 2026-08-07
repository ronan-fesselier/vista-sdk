#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/types/datetime/date_time.h>
#include <dnv/vista/sdk/c/types/datetime/date_time_offset.h>
#include <dnv/vista/sdk/c/types/datetime/time_span.h>
#include <dnv/vista/sdk/c/common.h>

#include <string_view>

TEST_SUITE("c-api::date_time_offset")
{
    TEST_CASE("dnv_vista_sdk_date_time_offset_create - components round-trip")
    {
        auto dt = dnv_vista_sdk_date_time_from_date_time(2024, 1, 15, 12, 0, 0);
        auto offset = dnv_vista_sdk_time_span_from_hours(2.0);

        auto dto = dnv_vista_sdk_date_time_offset_create(dt, offset);

        CHECK(dnv_vista_sdk_date_time_offset_year(dto) == 2024);
        CHECK(dnv_vista_sdk_date_time_offset_hour(dto) == 12);
        CHECK(dnv_vista_sdk_time_span_hours(dnv_vista_sdk_date_time_offset_offset(dto)) == doctest::Approx(2.0));
    }

    TEST_CASE("dnv_vista_sdk_date_time_offset_from_string - valid ISO 8601 string with offset")
    {
        dnv_vista_sdk_date_time_offset_t result{};
        CHECK(dnv_vista_sdk_date_time_offset_from_string("2024-01-01T12:00:00+02:00", &result) == 1);
        CHECK(dnv_vista_sdk_date_time_offset_year(result) == 2024);
        CHECK(dnv_vista_sdk_date_time_offset_total_offset_minutes(result) == 120);
    }

    TEST_CASE("dnv_vista_sdk_date_time_offset_from_string - invalid string returns 0")
    {
        dnv_vista_sdk_date_time_offset_t result{};
        CHECK(dnv_vista_sdk_date_time_offset_from_string("not-a-date", &result) == 0);
    }

    TEST_CASE("dnv_vista_sdk_date_time_offset_from_string - null arguments return 0")
    {
        dnv_vista_sdk_date_time_offset_t result{};
        CHECK(dnv_vista_sdk_date_time_offset_from_string(nullptr, &result) == 0);
        CHECK(dnv_vista_sdk_date_time_offset_from_string("2024-01-01T12:00:00Z", nullptr) == 0);
    }

    TEST_CASE("dnv_vista_sdk_date_time_offset_utc_date_time - converts to UTC instant")
    {
        auto dt = dnv_vista_sdk_date_time_from_date_time(2024, 1, 1, 12, 0, 0);
        auto offset = dnv_vista_sdk_time_span_from_hours(2.0);
        auto dto = dnv_vista_sdk_date_time_offset_create(dt, offset);

        auto utc = dnv_vista_sdk_date_time_offset_utc_date_time(dto);
        CHECK(dnv_vista_sdk_date_time_hour(utc) == 10);
    }

    TEST_CASE("dnv_vista_sdk_date_time_offset_to_string - round-trips through from_string")
    {
        auto dt = dnv_vista_sdk_date_time_from_date_time(2024, 6, 15, 10, 20, 30);
        auto offset = dnv_vista_sdk_time_span_from_hours(1.0);
        auto dto = dnv_vista_sdk_date_time_offset_create(dt, offset);

        char* str = dnv_vista_sdk_date_time_offset_to_string(dto, DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601);
        REQUIRE(str != nullptr);

        dnv_vista_sdk_date_time_offset_t parsed{};
        CHECK(dnv_vista_sdk_date_time_offset_from_string(str, &parsed) == 1);
        CHECK(dnv_vista_sdk_date_time_offset_equals(dto, parsed) == 1);

        dnv_vista_sdk_string_free(str);
    }

    TEST_CASE("dnv_vista_sdk_date_time_offset_equals - same instant, different offsets")
    {
        auto dtUtc = dnv_vista_sdk_date_time_from_date_time(2024, 1, 1, 10, 0, 0);
        auto dtoUtc = dnv_vista_sdk_date_time_offset_create(dtUtc, dnv_vista_sdk_time_span_from_hours(0.0));

        auto dtPlus2 = dnv_vista_sdk_date_time_from_date_time(2024, 1, 1, 12, 0, 0);
        auto dtoPlus2 = dnv_vista_sdk_date_time_offset_create(dtPlus2, dnv_vista_sdk_time_span_from_hours(2.0));

        CHECK(dnv_vista_sdk_date_time_offset_equals(dtoUtc, dtoPlus2) == 1);
        CHECK(dnv_vista_sdk_date_time_offset_equals_exact(dtoUtc, dtoPlus2) == 0);
    }

    TEST_CASE("dnv_vista_sdk_date_time_offset_to_offset/to_universal_time")
    {
        auto dt = dnv_vista_sdk_date_time_from_date_time(2024, 1, 1, 12, 0, 0);
        auto dto = dnv_vista_sdk_date_time_offset_create(dt, dnv_vista_sdk_time_span_from_hours(2.0));

        auto utc = dnv_vista_sdk_date_time_offset_to_universal_time(dto);
        CHECK(dnv_vista_sdk_date_time_offset_hour(utc) == 10);
        CHECK(dnv_vista_sdk_date_time_offset_total_offset_minutes(utc) == 0);

        auto toPlus5 = dnv_vista_sdk_date_time_offset_to_offset(dto, dnv_vista_sdk_time_span_from_hours(5.0));
        CHECK(dnv_vista_sdk_date_time_offset_equals(dto, toPlus5) == 1);
    }

    TEST_CASE("dnv_vista_sdk_date_time_offset_add_time_span/subtract/subtract_time_span")
    {
        auto dt = dnv_vista_sdk_date_time_from_date_time(2024, 1, 1, 0, 0, 0);
        auto dto = dnv_vista_sdk_date_time_offset_create(dt, dnv_vista_sdk_time_span_from_hours(0.0));
        auto oneHour = dnv_vista_sdk_time_span_from_hours(1.0);

        auto plusOneHour = dnv_vista_sdk_date_time_offset_add_time_span(dto, oneHour);
        CHECK(dnv_vista_sdk_date_time_offset_hour(plusOneHour) == 1);

        auto backToDto = dnv_vista_sdk_date_time_offset_subtract_time_span(plusOneHour, oneHour);
        CHECK(dnv_vista_sdk_date_time_offset_equals_exact(dto, backToDto) == 1);

        auto diff = dnv_vista_sdk_date_time_offset_subtract(plusOneHour, dto);
        CHECK(dnv_vista_sdk_time_span_hours(diff) == doctest::Approx(1.0));
    }

    TEST_CASE("dnv_vista_sdk_date_time_offset_is_valid")
    {
        auto dto = dnv_vista_sdk_date_time_offset_create(
            dnv_vista_sdk_date_time_from_date(2024, 1, 1), dnv_vista_sdk_time_span_from_hours(0.0));

        CHECK(dnv_vista_sdk_date_time_offset_is_valid(dto) == 1);
    }

    TEST_CASE("dnv_vista_sdk_date_time_offset_epoch/min/max - sanity")
    {
        auto epoch = dnv_vista_sdk_date_time_offset_epoch();
        CHECK(dnv_vista_sdk_date_time_offset_year(epoch) == 1970);

        auto min = dnv_vista_sdk_date_time_offset_min();
        auto max = dnv_vista_sdk_date_time_offset_max();
        CHECK(dnv_vista_sdk_date_time_offset_compare(min, max) == -1);
    }

    TEST_CASE("dnv_vista_sdk_date_time_offset_from_epoch_seconds/to_epoch_seconds - round-trip")
    {
        auto dto = dnv_vista_sdk_date_time_offset_from_epoch_seconds(1704110400);
        CHECK(dnv_vista_sdk_date_time_offset_to_epoch_seconds(dto) == 1704110400);
    }

    TEST_CASE("dnv_vista_sdk_date_time_offset_from_filetime/to_filetime - round-trip")
    {
        auto dto = dnv_vista_sdk_date_time_offset_epoch();
        auto filetime = dnv_vista_sdk_date_time_offset_to_filetime(dto);

        auto roundTripped = dnv_vista_sdk_date_time_offset_from_filetime(filetime);
        CHECK(dnv_vista_sdk_date_time_offset_equals(dto, roundTripped) == 1);
    }
}
