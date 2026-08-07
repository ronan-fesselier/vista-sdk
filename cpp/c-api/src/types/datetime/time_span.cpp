#include "dnv/vista/sdk/c/types/datetime/time_span.h"

#include "../../cast_internal.h"
#include "../../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromTimeSpan;
using dnv::vista::sdk::c::toOwnedCString;
using dnv::vista::sdk::c::toTimeSpan;

dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_from_ticks(int64_t ticks)
{
    return fromTimeSpan(TimeSpan{ ticks });
}

int dnv_vista_sdk_time_span_from_string(const char* str, dnv_vista_sdk_time_span_t* result)
{
    if (str == nullptr || result == nullptr)
    {
        c::setLastError("str and result must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    TimeSpan parsed;
    if (!TimeSpan::fromString(str, parsed))
    {
        c::setLastError("invalid TimeSpan string", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    *result = fromTimeSpan(parsed);
    return 1;
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_from_days(double days)
{
    return fromTimeSpan(TimeSpan::fromDays(days));
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_from_hours(double hours)
{
    return fromTimeSpan(TimeSpan::fromHours(hours));
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_from_minutes(double minutes)
{
    return fromTimeSpan(TimeSpan::fromMinutes(minutes));
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_from_seconds(double seconds)
{
    return fromTimeSpan(TimeSpan::fromSeconds(seconds));
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_from_milliseconds(double milliseconds)
{
    return fromTimeSpan(TimeSpan::fromMilliseconds(milliseconds));
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_from_microseconds(double microseconds)
{
    return fromTimeSpan(TimeSpan::fromMicroseconds(microseconds));
}

double dnv_vista_sdk_time_span_days(dnv_vista_sdk_time_span_t ts)
{
    return toTimeSpan(ts).days();
}

double dnv_vista_sdk_time_span_hours(dnv_vista_sdk_time_span_t ts)
{
    return toTimeSpan(ts).hours();
}

double dnv_vista_sdk_time_span_minutes(dnv_vista_sdk_time_span_t ts)
{
    return toTimeSpan(ts).minutes();
}

double dnv_vista_sdk_time_span_seconds(dnv_vista_sdk_time_span_t ts)
{
    return toTimeSpan(ts).seconds();
}

double dnv_vista_sdk_time_span_milliseconds(dnv_vista_sdk_time_span_t ts)
{
    return toTimeSpan(ts).milliseconds();
}

double dnv_vista_sdk_time_span_microseconds(dnv_vista_sdk_time_span_t ts)
{
    return toTimeSpan(ts).microseconds();
}

double dnv_vista_sdk_time_span_nanoseconds(dnv_vista_sdk_time_span_t ts)
{
    return toTimeSpan(ts).nanoseconds();
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_add(dnv_vista_sdk_time_span_t a, dnv_vista_sdk_time_span_t b)
{
    return fromTimeSpan(toTimeSpan(a) + toTimeSpan(b));
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_subtract(dnv_vista_sdk_time_span_t a, dnv_vista_sdk_time_span_t b)
{
    return fromTimeSpan(toTimeSpan(a) - toTimeSpan(b));
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_negate(dnv_vista_sdk_time_span_t ts)
{
    return fromTimeSpan(-toTimeSpan(ts));
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_multiply(dnv_vista_sdk_time_span_t ts, double multiplier)
{
    return fromTimeSpan(toTimeSpan(ts) * multiplier);
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_divide(dnv_vista_sdk_time_span_t ts, double divisor)
{
    return fromTimeSpan(toTimeSpan(ts) / divisor);
}

double dnv_vista_sdk_time_span_ratio(dnv_vista_sdk_time_span_t a, dnv_vista_sdk_time_span_t b)
{
    return toTimeSpan(a) / toTimeSpan(b);
}

int dnv_vista_sdk_time_span_compare(dnv_vista_sdk_time_span_t a, dnv_vista_sdk_time_span_t b)
{
    const auto cmp = toTimeSpan(a) <=> toTimeSpan(b);
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

int dnv_vista_sdk_time_span_equals(dnv_vista_sdk_time_span_t a, dnv_vista_sdk_time_span_t b)
{
    return toTimeSpan(a) == toTimeSpan(b) ? 1 : 0;
}

char* dnv_vista_sdk_time_span_to_string(dnv_vista_sdk_time_span_t ts)
{
    return toOwnedCString(toTimeSpan(ts).toString());
}
