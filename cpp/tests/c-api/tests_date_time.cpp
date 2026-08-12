#include <doctest/doctest.h>

#include <dnv/VistaSDK_c_api.h>

#include <string_view>

TEST_SUITE("c-api::date_time")
{
    TEST_CASE("dnv_vista_sdk_date_time_from_date_time - components round-trip")
    {
        auto dt = dnv_vista_sdk_date_time_from_date_time(2024, 1, 15, 12, 30, 45);

        CHECK(dnv_vista_sdk_date_time_year(dt) == 2024);
        CHECK(dnv_vista_sdk_date_time_month(dt) == 1);
        CHECK(dnv_vista_sdk_date_time_day(dt) == 15);
        CHECK(dnv_vista_sdk_date_time_hour(dt) == 12);
        CHECK(dnv_vista_sdk_date_time_minute(dt) == 30);
        CHECK(dnv_vista_sdk_date_time_second(dt) == 45);
    }

    TEST_CASE("dnv_vista_sdk_date_time_from_string - valid ISO 8601 string")
    {
        dnv_vista_sdk_date_time_t result{};
        CHECK(dnv_vista_sdk_date_time_from_string("2024-01-01T12:00:00Z", &result) == 1);
        CHECK(dnv_vista_sdk_date_time_year(result) == 2024);
    }

    TEST_CASE("dnv_vista_sdk_date_time_from_string - invalid string returns 0")
    {
        dnv_vista_sdk_date_time_t result{};
        CHECK(dnv_vista_sdk_date_time_from_string("not-a-date", &result) == 0);
    }

    TEST_CASE("dnv_vista_sdk_date_time_from_string - null arguments return 0")
    {
        dnv_vista_sdk_date_time_t result{};
        CHECK(dnv_vista_sdk_date_time_from_string(nullptr, &result) == 0);
        CHECK(dnv_vista_sdk_date_time_from_string("2024-01-01T12:00:00Z", nullptr) == 0);
    }

    TEST_CASE("dnv_vista_sdk_date_time_to_string - Iso8601 round-trips through from_string")
    {
        auto dt = dnv_vista_sdk_date_time_from_date_time(2024, 6, 15, 10, 20, 30);

        char* str = dnv_vista_sdk_date_time_to_string(dt, DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601);
        REQUIRE(str != nullptr);

        dnv_vista_sdk_date_time_t parsed{};
        CHECK(dnv_vista_sdk_date_time_from_string(str, &parsed) == 1);
        CHECK(dnv_vista_sdk_date_time_equals(dt, parsed) == 1);

        dnv_vista_sdk_date_time_string_free(str);
    }

    TEST_CASE("dnv_vista_sdk_date_time_add_days/add_months/add_years")
    {
        auto dt = dnv_vista_sdk_date_time_from_date(2024, 1, 31);

        auto plusDay = dnv_vista_sdk_date_time_add_days(dt, 1.0);
        CHECK(dnv_vista_sdk_date_time_month(plusDay) == 2);
        CHECK(dnv_vista_sdk_date_time_day(plusDay) == 1);

        auto plusMonth = dnv_vista_sdk_date_time_add_months(dt, 1);
        CHECK(dnv_vista_sdk_date_time_month(plusMonth) == 2);
        CHECK(dnv_vista_sdk_date_time_day(plusMonth) == 29); // clamped, 2024 is a leap year

        auto plusYear = dnv_vista_sdk_date_time_add_years(dt, 1);
        CHECK(dnv_vista_sdk_date_time_year(plusYear) == 2025);
    }

    TEST_CASE("dnv_vista_sdk_date_time_add_time_span/subtract_time_span/subtract")
    {
        auto dt = dnv_vista_sdk_date_time_from_date_time(2024, 1, 1, 0, 0, 0);
        auto oneHour = dnv_vista_sdk_time_span_from_hours(1.0);

        auto plusOneHour = dnv_vista_sdk_date_time_add_time_span(dt, oneHour);
        CHECK(dnv_vista_sdk_date_time_hour(plusOneHour) == 1);

        auto backToDt = dnv_vista_sdk_date_time_subtract_time_span(plusOneHour, oneHour);
        CHECK(dnv_vista_sdk_date_time_equals(dt, backToDt) == 1);

        auto diff = dnv_vista_sdk_date_time_subtract(plusOneHour, dt);
        CHECK(dnv_vista_sdk_time_span_hours(diff) == doctest::Approx(1.0));
    }

    TEST_CASE("dnv_vista_sdk_date_time_is_valid")
    {
        auto valid = dnv_vista_sdk_date_time_from_date(2024, 1, 1);
        CHECK(dnv_vista_sdk_date_time_is_valid(valid) == 1);
    }

    TEST_CASE("dnv_vista_sdk_date_time_is_leap_year/days_in_month")
    {
        CHECK(dnv_vista_sdk_date_time_is_leap_year(2024) == 1);
        CHECK(dnv_vista_sdk_date_time_is_leap_year(2023) == 0);
        CHECK(dnv_vista_sdk_date_time_days_in_month(2024, 2) == 29);
        CHECK(dnv_vista_sdk_date_time_days_in_month(2023, 2) == 28);
    }

    TEST_CASE("dnv_vista_sdk_date_time_epoch/min/max - sanity")
    {
        auto epoch = dnv_vista_sdk_date_time_epoch();
        CHECK(dnv_vista_sdk_date_time_year(epoch) == 1970);

        auto min = dnv_vista_sdk_date_time_min();
        auto max = dnv_vista_sdk_date_time_max();
        CHECK(dnv_vista_sdk_date_time_compare(min, max) == -1);
    }

    TEST_CASE("dnv_vista_sdk_date_time_from_epoch_seconds/to_epoch_seconds - round-trip")
    {
        auto dt = dnv_vista_sdk_date_time_from_epoch_seconds(1704110400);
        CHECK(dnv_vista_sdk_date_time_to_epoch_seconds(dt) == 1704110400);
    }

    TEST_CASE("dnv_vista_sdk_date_time_compare/equals")
    {
        auto a = dnv_vista_sdk_date_time_from_date(2024, 1, 1);
        auto b = dnv_vista_sdk_date_time_from_date(2024, 1, 2);
        auto c = dnv_vista_sdk_date_time_from_date(2024, 1, 1);

        CHECK(dnv_vista_sdk_date_time_compare(a, b) == -1);
        CHECK(dnv_vista_sdk_date_time_compare(b, a) == 1);
        CHECK(dnv_vista_sdk_date_time_compare(a, c) == 0);
        CHECK(dnv_vista_sdk_date_time_equals(a, c) == 1);
    }

    TEST_CASE("dnv_vista_sdk_date_time_date/time_of_day")
    {
        auto dt = dnv_vista_sdk_date_time_from_date_time(2024, 6, 15, 10, 20, 30);

        auto dateOnly = dnv_vista_sdk_date_time_date(dt);
        CHECK(dnv_vista_sdk_date_time_hour(dateOnly) == 0);
        CHECK(dnv_vista_sdk_date_time_day(dateOnly) == 15);

        auto timeOfDay = dnv_vista_sdk_date_time_time_of_day(dt);
        CHECK(dnv_vista_sdk_time_span_hours(timeOfDay) == doctest::Approx(10.0 + 20.0 / 60.0 + 30.0 / 3600.0));
    }
}
