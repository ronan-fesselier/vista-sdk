#include "dnv/vista/sdk/c/types/decimal/decimal.h"

#include "../../cast_internal.h"
#include "../../error_internal.h"

#include <limits>

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromDecimal;
using dnv::vista::sdk::c::toDecimal;
using dnv::vista::sdk::c::toOwnedCString;
using dnv::vista::sdk::c::toRoundingMode;

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_zero(void)
{
    return fromDecimal(Decimal{});
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_from_double(double value)
{
    return fromDecimal(Decimal{ value });
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_from_int64(int64_t value)
{
    return fromDecimal(Decimal{ value });
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_from_uint64(uint64_t value)
{
    return fromDecimal(Decimal{ value });
}

int dnv_vista_sdk_decimal_from_string(const char* str, dnv_vista_sdk_decimal_t* result)
{
    if (str == nullptr || result == nullptr)
    {
        c::setLastError("str and result must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    Decimal parsed;
    if (!Decimal::fromString(str, parsed))
    {
        c::setLastError("invalid decimal string", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    *result = fromDecimal(parsed);
    return 1;
}

uint8_t dnv_vista_sdk_decimal_scale(dnv_vista_sdk_decimal_t d)
{
    return toDecimal(d).scale();
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_add(dnv_vista_sdk_decimal_t a, dnv_vista_sdk_decimal_t b)
{
    return fromDecimal(toDecimal(a) + toDecimal(b));
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_subtract(dnv_vista_sdk_decimal_t a, dnv_vista_sdk_decimal_t b)
{
    return fromDecimal(toDecimal(a) - toDecimal(b));
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_multiply(dnv_vista_sdk_decimal_t a, dnv_vista_sdk_decimal_t b)
{
    return fromDecimal(toDecimal(a) * toDecimal(b));
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_divide(dnv_vista_sdk_decimal_t a, dnv_vista_sdk_decimal_t b)
{
    return c::cApiTryCatch<dnv_vista_sdk_decimal_t>(
        [&]() -> dnv_vista_sdk_decimal_t { return fromDecimal(toDecimal(a) / toDecimal(b)); });
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_negate(dnv_vista_sdk_decimal_t d)
{
    return fromDecimal(-toDecimal(d));
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_abs(dnv_vista_sdk_decimal_t d)
{
    return fromDecimal(toDecimal(d).abs());
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_ceil(dnv_vista_sdk_decimal_t d)
{
    return fromDecimal(toDecimal(d).ceil());
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_floor(dnv_vista_sdk_decimal_t d)
{
    return fromDecimal(toDecimal(d).floor());
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_trunc(dnv_vista_sdk_decimal_t d)
{
    return fromDecimal(toDecimal(d).trunc());
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_round(
    dnv_vista_sdk_decimal_t d, int32_t decimalPlacesCount, dnv_vista_sdk_decimal_rounding_mode_t mode)
{
    return fromDecimal(toDecimal(d).round(decimalPlacesCount, toRoundingMode(mode)));
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_sqrt(dnv_vista_sdk_decimal_t d)
{
    return c::cApiTryCatch<dnv_vista_sdk_decimal_t>(
        [&]() -> dnv_vista_sdk_decimal_t { return fromDecimal(toDecimal(d).sqrt()); });
}

int dnv_vista_sdk_decimal_compare(dnv_vista_sdk_decimal_t a, dnv_vista_sdk_decimal_t b)
{
    const auto cmp = toDecimal(a) <=> toDecimal(b);
    if (cmp < 0)
    {
        return -1;
    }
    if (cmp > 0)
    {
        return 1;
    }
    return 0;
}

int dnv_vista_sdk_decimal_equals(dnv_vista_sdk_decimal_t a, dnv_vista_sdk_decimal_t b)
{
    return toDecimal(a) == toDecimal(b) ? 1 : 0;
}

int dnv_vista_sdk_decimal_compare_double(dnv_vista_sdk_decimal_t d, double val)
{
    const auto dec = toDecimal(d);
    if (dec == val)
    {
        return 0;
    }
    return dec < val ? -1 : 1;
}

int dnv_vista_sdk_decimal_compare_int64(dnv_vista_sdk_decimal_t d, int64_t val)
{
    const auto dec = toDecimal(d);
    if (dec == val)
    {
        return 0;
    }
    return dec < val ? -1 : 1;
}

int dnv_vista_sdk_decimal_compare_uint64(dnv_vista_sdk_decimal_t d, uint64_t val)
{
    const auto dec = toDecimal(d);
    if (dec == val)
    {
        return 0;
    }
    return dec < val ? -1 : 1;
}

double dnv_vista_sdk_decimal_to_double(dnv_vista_sdk_decimal_t d)
{
    return toDecimal(d).toDouble();
}

void dnv_vista_sdk_decimal_to_bits(dnv_vista_sdk_decimal_t d, int32_t bits[4])
{
    if (bits == nullptr)
    {
        c::setLastError("bits must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    const auto arr = toDecimal(d).toBits();
    bits[0] = arr[0];
    bits[1] = arr[1];
    bits[2] = arr[2];
    bits[3] = arr[3];
}

uint8_t dnv_vista_sdk_decimal_decimal_places_count(dnv_vista_sdk_decimal_t d)
{
    return toDecimal(d).decimalPlacesCount();
}

uint32_t dnv_vista_sdk_decimal_total_digits_count(dnv_vista_sdk_decimal_t d)
{
    return toDecimal(d).totalDigitsCount();
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_min(void)
{
    return fromDecimal(std::numeric_limits<Decimal>::min());
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_max(void)
{
    return fromDecimal(std::numeric_limits<Decimal>::max());
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_lowest(void)
{
    return fromDecimal(std::numeric_limits<Decimal>::lowest());
}

dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_epsilon(void)
{
    return fromDecimal(std::numeric_limits<Decimal>::epsilon());
}

char* dnv_vista_sdk_decimal_to_string(dnv_vista_sdk_decimal_t d)
{
    return toOwnedCString(toDecimal(d).toString());
}
