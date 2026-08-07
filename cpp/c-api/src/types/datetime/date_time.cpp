#include "dnv/vista/sdk/c/types/datetime/date_time.h"

#include "../../cast_internal.h"
#include "../../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromDateTime;
using dnv::vista::sdk::c::fromTimeSpan;
using dnv::vista::sdk::c::toDateTime;
using dnv::vista::sdk::c::toDateTimeFormat;
using dnv::vista::sdk::c::toOwnedCString;

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_from_ticks(int64_t ticks)
{
    return fromDateTime(DateTime{ ticks });
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_from_date(int32_t year, int32_t month, int32_t day)
{
    return fromDateTime(DateTime{ year, month, day });
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_from_date_time(
    int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second)
{
    return fromDateTime(DateTime{ year, month, day, hour, minute, second });
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_from_date_time_millis(
    int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond)
{
    return fromDateTime(DateTime{ year, month, day, hour, minute, second, millisecond });
}

int dnv_vista_sdk_date_time_from_string(const char* iso8601String, dnv_vista_sdk_date_time_t* result)
{
    if (iso8601String == nullptr || result == nullptr)
    {
        c::setLastErrorMessage("iso8601String and result must not be null");
        return 0;
    }

    DateTime parsed;
    if (!DateTime::fromString(iso8601String, parsed))
    {
        c::setLastErrorMessage("invalid ISO 8601 DateTime string");
        return 0;
    }

    *result = fromDateTime(parsed);
    return 1;
}

int32_t dnv_vista_sdk_date_time_year(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).year();
}

int32_t dnv_vista_sdk_date_time_month(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).month();
}

int32_t dnv_vista_sdk_date_time_day(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).day();
}

int32_t dnv_vista_sdk_date_time_hour(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).hour();
}

int32_t dnv_vista_sdk_date_time_minute(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).minute();
}

int32_t dnv_vista_sdk_date_time_second(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).second();
}

int32_t dnv_vista_sdk_date_time_millisecond(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).millisecond();
}

int32_t dnv_vista_sdk_date_time_microsecond(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).microsecond();
}

int32_t dnv_vista_sdk_date_time_nanosecond(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).nanosecond();
}

int64_t dnv_vista_sdk_date_time_ticks(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).ticks();
}

int32_t dnv_vista_sdk_date_time_day_of_week(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).dayOfWeek();
}

int32_t dnv_vista_sdk_date_time_day_of_year(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).dayOfYear();
}

int64_t dnv_vista_sdk_date_time_to_epoch_seconds(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).toEpochSeconds();
}

int64_t dnv_vista_sdk_date_time_to_epoch_milliseconds(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).toEpochMilliseconds();
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_date(dnv_vista_sdk_date_time_t dt)
{
    return fromDateTime(toDateTime(dt).date());
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_date_time_time_of_day(dnv_vista_sdk_date_time_t dt)
{
    return fromTimeSpan(toDateTime(dt).timeOfDay());
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_add_days(dnv_vista_sdk_date_time_t dt, double days)
{
    return fromDateTime(toDateTime(dt).addDays(days));
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_add_hours(dnv_vista_sdk_date_time_t dt, double hours)
{
    return fromDateTime(toDateTime(dt).addHours(hours));
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_add_minutes(dnv_vista_sdk_date_time_t dt, double minutes)
{
    return fromDateTime(toDateTime(dt).addMinutes(minutes));
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_add_seconds(dnv_vista_sdk_date_time_t dt, double seconds)
{
    return fromDateTime(toDateTime(dt).addSeconds(seconds));
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_add_milliseconds(dnv_vista_sdk_date_time_t dt, double milliseconds)
{
    return fromDateTime(toDateTime(dt).addMilliseconds(milliseconds));
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_add_months(dnv_vista_sdk_date_time_t dt, int32_t months)
{
    return fromDateTime(toDateTime(dt).addMonths(months));
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_add_years(dnv_vista_sdk_date_time_t dt, int32_t years)
{
    return fromDateTime(toDateTime(dt).addYears(years));
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_add_time_span(
    dnv_vista_sdk_date_time_t dt, dnv_vista_sdk_time_span_t duration)
{
    return fromDateTime(toDateTime(dt) + c::toTimeSpan(duration));
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_subtract_time_span(
    dnv_vista_sdk_date_time_t dt, dnv_vista_sdk_time_span_t duration)
{
    return fromDateTime(toDateTime(dt) - c::toTimeSpan(duration));
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_date_time_subtract(dnv_vista_sdk_date_time_t a, dnv_vista_sdk_date_time_t b)
{
    return fromTimeSpan(toDateTime(a) - toDateTime(b));
}

int dnv_vista_sdk_date_time_is_valid(dnv_vista_sdk_date_time_t dt)
{
    return toDateTime(dt).isValid() ? 1 : 0;
}

int dnv_vista_sdk_date_time_is_leap_year(int32_t year)
{
    return DateTime::isLeapYear(year) ? 1 : 0;
}

int32_t dnv_vista_sdk_date_time_days_in_month(int32_t year, int32_t month)
{
    return DateTime::daysInMonth(year, month);
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_utc_now(void)
{
    return fromDateTime(DateTime::utcNow());
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_min(void)
{
    return fromDateTime(DateTime::min());
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_max(void)
{
    return fromDateTime(DateTime::max());
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_epoch(void)
{
    return fromDateTime(DateTime::epoch());
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_from_epoch_seconds(int64_t seconds)
{
    return fromDateTime(DateTime::fromEpochSeconds(seconds));
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_from_epoch_milliseconds(int64_t milliseconds)
{
    return fromDateTime(DateTime::fromEpochMilliseconds(milliseconds));
}

int dnv_vista_sdk_date_time_compare(dnv_vista_sdk_date_time_t a, dnv_vista_sdk_date_time_t b)
{
    const auto cmp = toDateTime(a) <=> toDateTime(b);
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

int dnv_vista_sdk_date_time_equals(dnv_vista_sdk_date_time_t a, dnv_vista_sdk_date_time_t b)
{
    return toDateTime(a) == toDateTime(b) ? 1 : 0;
}

char* dnv_vista_sdk_date_time_to_string(dnv_vista_sdk_date_time_t dt, dnv_vista_sdk_date_time_format_t format)
{
    return toOwnedCString(toDateTime(dt).toString(toDateTimeFormat(format)));
}

void dnv_vista_sdk_date_time_string_free(char* str)
{
    delete[] str;
}
