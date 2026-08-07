#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/types/datetime/time_span.h>

#include <string_view>

TEST_SUITE("c-api::time_span")
{
    TEST_CASE("dnv_vista_sdk_time_span_from_ticks/ticks - round-trips via seconds")
    {
        auto ts = dnv_vista_sdk_time_span_from_seconds(90.0);

        CHECK(dnv_vista_sdk_time_span_minutes(ts) == doctest::Approx(1.5));
        CHECK(dnv_vista_sdk_time_span_seconds(ts) == doctest::Approx(90.0));
    }

    TEST_CASE("dnv_vista_sdk_time_span_from_string - valid ISO 8601 duration")
    {
        dnv_vista_sdk_time_span_t result{};
        CHECK(dnv_vista_sdk_time_span_from_string("PT1H30M45S", &result) == 1);
        CHECK(dnv_vista_sdk_time_span_hours(result) == doctest::Approx(1.5125));
    }

    TEST_CASE("dnv_vista_sdk_time_span_from_string - invalid string returns 0")
    {
        dnv_vista_sdk_time_span_t result{};
        CHECK(dnv_vista_sdk_time_span_from_string("not-a-duration", &result) == 0);
    }

    TEST_CASE("dnv_vista_sdk_time_span_from_string - null arguments return 0")
    {
        dnv_vista_sdk_time_span_t result{};
        CHECK(dnv_vista_sdk_time_span_from_string(nullptr, &result) == 0);
        CHECK(dnv_vista_sdk_time_span_from_string("PT1H", nullptr) == 0);
    }

    TEST_CASE("dnv_vista_sdk_time_span_add/subtract")
    {
        auto a = dnv_vista_sdk_time_span_from_hours(1.0);
        auto b = dnv_vista_sdk_time_span_from_minutes(30.0);

        auto sum = dnv_vista_sdk_time_span_add(a, b);
        CHECK(dnv_vista_sdk_time_span_minutes(sum) == doctest::Approx(90.0));

        auto diff = dnv_vista_sdk_time_span_subtract(a, b);
        CHECK(dnv_vista_sdk_time_span_minutes(diff) == doctest::Approx(30.0));
    }

    TEST_CASE("dnv_vista_sdk_time_span_negate/multiply/divide")
    {
        auto ts = dnv_vista_sdk_time_span_from_hours(2.0);

        CHECK(dnv_vista_sdk_time_span_hours(dnv_vista_sdk_time_span_negate(ts)) == doctest::Approx(-2.0));
        CHECK(dnv_vista_sdk_time_span_hours(dnv_vista_sdk_time_span_multiply(ts, 3.0)) == doctest::Approx(6.0));
        CHECK(dnv_vista_sdk_time_span_hours(dnv_vista_sdk_time_span_divide(ts, 4.0)) == doctest::Approx(0.5));
    }

    TEST_CASE("dnv_vista_sdk_time_span_ratio")
    {
        auto a = dnv_vista_sdk_time_span_from_hours(4.0);
        auto b = dnv_vista_sdk_time_span_from_hours(2.0);

        CHECK(dnv_vista_sdk_time_span_ratio(a, b) == doctest::Approx(2.0));
    }

    TEST_CASE("dnv_vista_sdk_time_span_compare/equals")
    {
        auto a = dnv_vista_sdk_time_span_from_seconds(1.0);
        auto b = dnv_vista_sdk_time_span_from_seconds(2.0);
        auto c = dnv_vista_sdk_time_span_from_seconds(1.0);

        CHECK(dnv_vista_sdk_time_span_compare(a, b) == -1);
        CHECK(dnv_vista_sdk_time_span_compare(b, a) == 1);
        CHECK(dnv_vista_sdk_time_span_compare(a, c) == 0);
        CHECK(dnv_vista_sdk_time_span_equals(a, c) == 1);
        CHECK(dnv_vista_sdk_time_span_equals(a, b) == 0);
    }

    TEST_CASE("dnv_vista_sdk_time_span_to_string - ISO 8601 duration round-trip")
    {
        auto ts = dnv_vista_sdk_time_span_from_seconds(45.0);

        char* str = dnv_vista_sdk_time_span_to_string(ts);
        REQUIRE(str != nullptr);

        dnv_vista_sdk_time_span_t parsed{};
        CHECK(dnv_vista_sdk_time_span_from_string(str, &parsed) == 1);
        CHECK(dnv_vista_sdk_time_span_equals(ts, parsed) == 1);

        dnv_vista_sdk_time_span_string_free(str);
    }
}
