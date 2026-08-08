#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/types/decimal/decimal.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::decimal")
{
    TEST_CASE("dnv_vista_sdk_decimal_from_string - valid string round-trips through to_string")
    {
        dnv_vista_sdk_decimal_t d{};
        CHECK(dnv_vista_sdk_decimal_from_string("123.45", &d) == 1);

        char* str = dnv_vista_sdk_decimal_to_string(d);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "123.45");

        dnv_vista_sdk_decimal_string_free(str);
    }

    TEST_CASE("dnv_vista_sdk_decimal_from_string - invalid string returns 0")
    {
        dnv_vista_sdk_decimal_t d{};
        CHECK(dnv_vista_sdk_decimal_from_string("not-a-decimal", &d) == 0);
    }

    TEST_CASE("dnv_vista_sdk_decimal_from_string - null arguments return 0")
    {
        dnv_vista_sdk_decimal_t d{};
        CHECK(dnv_vista_sdk_decimal_from_string(nullptr, &d) == 0);
        CHECK(dnv_vista_sdk_decimal_from_string("1.0", nullptr) == 0);
    }

    TEST_CASE("dnv_vista_sdk_decimal_from_int64/from_uint64/from_double")
    {
        auto fromInt = dnv_vista_sdk_decimal_from_int64(-42);
        CHECK(dnv_vista_sdk_decimal_compare_int64(fromInt, -42) == 0);

        auto fromUint = dnv_vista_sdk_decimal_from_uint64(42);
        CHECK(dnv_vista_sdk_decimal_compare_uint64(fromUint, 42) == 0);

        auto fromDouble = dnv_vista_sdk_decimal_from_double(1.5);
        CHECK(dnv_vista_sdk_decimal_compare_double(fromDouble, 1.5) == 0);
    }

    TEST_CASE("dnv_vista_sdk_decimal_scale")
    {
        dnv_vista_sdk_decimal_t d{};
        REQUIRE(dnv_vista_sdk_decimal_from_string("123.456", &d) == 1);
        CHECK(dnv_vista_sdk_decimal_scale(d) == 3);
    }

    TEST_CASE("dnv_vista_sdk_decimal_add/subtract/multiply")
    {
        dnv_vista_sdk_decimal_t a{};
        dnv_vista_sdk_decimal_t b{};
        REQUIRE(dnv_vista_sdk_decimal_from_string("1.5", &a) == 1);
        REQUIRE(dnv_vista_sdk_decimal_from_string("2.5", &b) == 1);

        auto sum = dnv_vista_sdk_decimal_add(a, b);
        CHECK(dnv_vista_sdk_decimal_compare_double(sum, 4.0) == 0);

        auto diff = dnv_vista_sdk_decimal_subtract(b, a);
        CHECK(dnv_vista_sdk_decimal_compare_double(diff, 1.0) == 0);

        auto product = dnv_vista_sdk_decimal_multiply(a, b);
        CHECK(dnv_vista_sdk_decimal_compare_double(product, 3.75) == 0);
    }

    TEST_CASE("dnv_vista_sdk_decimal_divide - normal division")
    {
        dnv_vista_sdk_decimal_t a{};
        dnv_vista_sdk_decimal_t b{};
        REQUIRE(dnv_vista_sdk_decimal_from_string("10", &a) == 1);
        REQUIRE(dnv_vista_sdk_decimal_from_string("4", &b) == 1);

        auto quotient = dnv_vista_sdk_decimal_divide(a, b);
        CHECK(dnv_vista_sdk_decimal_compare_double(quotient, 2.5) == 0);
    }

    TEST_CASE("dnv_vista_sdk_decimal_divide - division by zero sets last error and returns zero")
    {
        auto a = dnv_vista_sdk_decimal_from_int64(10);
        auto zero = dnv_vista_sdk_decimal_zero();

        auto result = dnv_vista_sdk_decimal_divide(a, zero);
        CHECK(dnv_vista_sdk_decimal_equals(result, dnv_vista_sdk_decimal_zero()) == 1);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_decimal_negate/abs")
    {
        auto d = dnv_vista_sdk_decimal_from_int64(-5);

        CHECK(dnv_vista_sdk_decimal_compare_int64(dnv_vista_sdk_decimal_negate(d), 5) == 0);
        CHECK(dnv_vista_sdk_decimal_compare_int64(dnv_vista_sdk_decimal_abs(d), 5) == 0);
    }

    TEST_CASE("dnv_vista_sdk_decimal_ceil/floor/trunc")
    {
        dnv_vista_sdk_decimal_t d{};
        REQUIRE(dnv_vista_sdk_decimal_from_string("2.5", &d) == 1);

        CHECK(dnv_vista_sdk_decimal_compare_int64(dnv_vista_sdk_decimal_ceil(d), 3) == 0);
        CHECK(dnv_vista_sdk_decimal_compare_int64(dnv_vista_sdk_decimal_floor(d), 2) == 0);

        dnv_vista_sdk_decimal_t neg{};
        REQUIRE(dnv_vista_sdk_decimal_from_string("-2.7", &neg) == 1);
        CHECK(dnv_vista_sdk_decimal_compare_int64(dnv_vista_sdk_decimal_trunc(neg), -2) == 0);
    }

    TEST_CASE("dnv_vista_sdk_decimal_round - ToNearest banker's rounding")
    {
        dnv_vista_sdk_decimal_t d{};
        REQUIRE(dnv_vista_sdk_decimal_from_string("2.5", &d) == 1);

        auto rounded = dnv_vista_sdk_decimal_round(d, 0, DNV_VISTA_SDK_DECIMAL_ROUNDING_MODE_TO_NEAREST);
        CHECK(dnv_vista_sdk_decimal_compare_int64(rounded, 2) == 0);
    }

    TEST_CASE("dnv_vista_sdk_decimal_sqrt - normal value")
    {
        auto d = dnv_vista_sdk_decimal_from_int64(4);

        auto root = dnv_vista_sdk_decimal_sqrt(d);
        CHECK(dnv_vista_sdk_decimal_compare_int64(root, 2) == 0);
    }

    TEST_CASE("dnv_vista_sdk_decimal_sqrt - negative value sets last error and returns zero")
    {
        auto d = dnv_vista_sdk_decimal_from_int64(-4);

        auto root = dnv_vista_sdk_decimal_sqrt(d);
        CHECK(dnv_vista_sdk_decimal_equals(root, dnv_vista_sdk_decimal_zero()) == 1);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_decimal_compare/equals")
    {
        auto a = dnv_vista_sdk_decimal_from_int64(1);
        auto b = dnv_vista_sdk_decimal_from_int64(2);
        auto c = dnv_vista_sdk_decimal_from_int64(1);

        CHECK(dnv_vista_sdk_decimal_compare(a, b) == -1);
        CHECK(dnv_vista_sdk_decimal_compare(b, a) == 1);
        CHECK(dnv_vista_sdk_decimal_compare(a, c) == 0);
        CHECK(dnv_vista_sdk_decimal_equals(a, c) == 1);
        CHECK(dnv_vista_sdk_decimal_equals(a, b) == 0);
    }

    TEST_CASE("dnv_vista_sdk_decimal_to_double")
    {
        dnv_vista_sdk_decimal_t d{};
        REQUIRE(dnv_vista_sdk_decimal_from_string("3.14", &d) == 1);

        CHECK(dnv_vista_sdk_decimal_to_double(d) == doctest::Approx(3.14));
    }

    TEST_CASE("dnv_vista_sdk_decimal_to_bits - returns 4 ints, mantissa[0] matches a small integer value")
    {
        auto d = dnv_vista_sdk_decimal_from_int64(42);

        int32_t bits[4]{};
        dnv_vista_sdk_decimal_to_bits(d, bits);
        CHECK(bits[0] == 42);
        CHECK(bits[1] == 0);
        CHECK(bits[2] == 0);
        CHECK(bits[3] == 0);
    }

    TEST_CASE("dnv_vista_sdk_decimal_decimal_places_count/total_digits_count")
    {
        dnv_vista_sdk_decimal_t d{};
        REQUIRE(dnv_vista_sdk_decimal_from_string("123.4500", &d) == 1);

        CHECK(dnv_vista_sdk_decimal_decimal_places_count(d) == 2);
        CHECK(dnv_vista_sdk_decimal_total_digits_count(d) == 5);
    }

    TEST_CASE("dnv_vista_sdk_decimal_min/max/lowest/epsilon - sanity")
    {
        auto min = dnv_vista_sdk_decimal_min();
        auto max = dnv_vista_sdk_decimal_max();
        auto lowest = dnv_vista_sdk_decimal_lowest();
        auto epsilon = dnv_vista_sdk_decimal_epsilon();

        CHECK(dnv_vista_sdk_decimal_compare(min, max) == -1);
        CHECK(dnv_vista_sdk_decimal_compare(lowest, min) == -1);
        CHECK(dnv_vista_sdk_decimal_equals(min, epsilon) == 1);
    }

    TEST_CASE("dnv_vista_sdk_decimal_zero")
    {
        auto zero = dnv_vista_sdk_decimal_zero();
        CHECK(dnv_vista_sdk_decimal_compare_int64(zero, 0) == 0);
    }
}
