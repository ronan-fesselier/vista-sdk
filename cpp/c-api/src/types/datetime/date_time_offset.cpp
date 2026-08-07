#include "dnv/vista/sdk/c/types/datetime/date_time_offset.h"

#include "../../cast_internal.h"
#include "../../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromDateTime;
using dnv::vista::sdk::c::fromDateTimeOffset;
using dnv::vista::sdk::c::fromTimeSpan;
using dnv::vista::sdk::c::toDateTime;
using dnv::vista::sdk::c::toDateTimeFormat;
using dnv::vista::sdk::c::toDateTimeOffset;
using dnv::vista::sdk::c::toOwnedCString;
using dnv::vista::sdk::c::toTimeSpan;

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_create(
    dnv_vista_sdk_date_time_t dateTime, dnv_vista_sdk_time_span_t offset)
{
    return fromDateTimeOffset(DateTimeOffset{ toDateTime(dateTime), toTimeSpan(offset) });
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_from_date_time(dnv_vista_sdk_date_time_t dateTime)
{
    return fromDateTimeOffset(DateTimeOffset{ toDateTime(dateTime) });
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_from_ticks(
    int64_t ticks, dnv_vista_sdk_time_span_t offset)
{
    return fromDateTimeOffset(DateTimeOffset{ ticks, toTimeSpan(offset) });
}

int dnv_vista_sdk_date_time_offset_from_string(const char* iso8601String, dnv_vista_sdk_date_time_offset_t* result)
{
    if (iso8601String == nullptr || result == nullptr)
    {
        c::setLastErrorMessage("iso8601String and result must not be null");
        return 0;
    }

    DateTimeOffset parsed;
    if (!DateTimeOffset::fromString(iso8601String, parsed))
    {
        c::setLastErrorMessage("invalid ISO 8601 DateTimeOffset string");
        return 0;
    }

    *result = fromDateTimeOffset(parsed);
    return 1;
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_offset_date_time(dnv_vista_sdk_date_time_offset_t dto)
{
    return fromDateTime(toDateTimeOffset(dto).dateTime());
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_date_time_offset_offset(dnv_vista_sdk_date_time_offset_t dto)
{
    return fromTimeSpan(toDateTimeOffset(dto).offset());
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_offset_utc_date_time(dnv_vista_sdk_date_time_offset_t dto)
{
    return fromDateTime(toDateTimeOffset(dto).utcDateTime());
}

dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_offset_local_date_time(dnv_vista_sdk_date_time_offset_t dto)
{
    return fromDateTime(toDateTimeOffset(dto).localDateTime());
}

int64_t dnv_vista_sdk_date_time_offset_ticks(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).ticks();
}

int64_t dnv_vista_sdk_date_time_offset_utc_ticks(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).utcTicks();
}

int32_t dnv_vista_sdk_date_time_offset_year(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).year();
}

int32_t dnv_vista_sdk_date_time_offset_month(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).month();
}

int32_t dnv_vista_sdk_date_time_offset_day(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).day();
}

int32_t dnv_vista_sdk_date_time_offset_hour(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).hour();
}

int32_t dnv_vista_sdk_date_time_offset_minute(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).minute();
}

int32_t dnv_vista_sdk_date_time_offset_second(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).second();
}

int32_t dnv_vista_sdk_date_time_offset_millisecond(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).millisecond();
}

int32_t dnv_vista_sdk_date_time_offset_microsecond(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).microsecond();
}

int32_t dnv_vista_sdk_date_time_offset_nanosecond(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).nanosecond();
}

int32_t dnv_vista_sdk_date_time_offset_day_of_week(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).dayOfWeek();
}

int32_t dnv_vista_sdk_date_time_offset_day_of_year(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).dayOfYear();
}

int32_t dnv_vista_sdk_date_time_offset_total_offset_minutes(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).totalOffsetMinutes();
}

int64_t dnv_vista_sdk_date_time_offset_to_epoch_seconds(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).toEpochSeconds();
}

int64_t dnv_vista_sdk_date_time_offset_to_epoch_milliseconds(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).toEpochMilliseconds();
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_date(dnv_vista_sdk_date_time_offset_t dto)
{
    return fromDateTimeOffset(toDateTimeOffset(dto).date());
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_date_time_offset_time_of_day(dnv_vista_sdk_date_time_offset_t dto)
{
    return fromTimeSpan(toDateTimeOffset(dto).timeOfDay());
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_to_offset(
    dnv_vista_sdk_date_time_offset_t dto, dnv_vista_sdk_time_span_t newOffset)
{
    return fromDateTimeOffset(toDateTimeOffset(dto).toOffset(toTimeSpan(newOffset)));
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_to_universal_time(dnv_vista_sdk_date_time_offset_t dto)
{
    return fromDateTimeOffset(toDateTimeOffset(dto).toUniversalTime());
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_to_local_time(dnv_vista_sdk_date_time_offset_t dto)
{
    return fromDateTimeOffset(toDateTimeOffset(dto).toLocalTime());
}

int64_t dnv_vista_sdk_date_time_offset_to_filetime(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).toFILETIME();
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_add_time_span(
    dnv_vista_sdk_date_time_offset_t dto, dnv_vista_sdk_time_span_t value)
{
    return fromDateTimeOffset(toDateTimeOffset(dto).add(toTimeSpan(value)));
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_add_days(
    dnv_vista_sdk_date_time_offset_t dto, double days)
{
    return fromDateTimeOffset(toDateTimeOffset(dto).addDays(days));
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_add_hours(
    dnv_vista_sdk_date_time_offset_t dto, double hours)
{
    return fromDateTimeOffset(toDateTimeOffset(dto).addHours(hours));
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_add_minutes(
    dnv_vista_sdk_date_time_offset_t dto, double minutes)
{
    return fromDateTimeOffset(toDateTimeOffset(dto).addMinutes(minutes));
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_add_seconds(
    dnv_vista_sdk_date_time_offset_t dto, double seconds)
{
    return fromDateTimeOffset(toDateTimeOffset(dto).addSeconds(seconds));
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_add_milliseconds(
    dnv_vista_sdk_date_time_offset_t dto, double milliseconds)
{
    return fromDateTimeOffset(toDateTimeOffset(dto).addMilliseconds(milliseconds));
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_add_months(
    dnv_vista_sdk_date_time_offset_t dto, int32_t months)
{
    return fromDateTimeOffset(toDateTimeOffset(dto).addMonths(months));
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_add_years(
    dnv_vista_sdk_date_time_offset_t dto, int32_t years)
{
    return fromDateTimeOffset(toDateTimeOffset(dto).addYears(years));
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_add_ticks(
    dnv_vista_sdk_date_time_offset_t dto, int64_t ticks)
{
    return fromDateTimeOffset(toDateTimeOffset(dto).addTicks(ticks));
}

dnv_vista_sdk_time_span_t dnv_vista_sdk_date_time_offset_subtract(
    dnv_vista_sdk_date_time_offset_t a, dnv_vista_sdk_date_time_offset_t b)
{
    return fromTimeSpan(toDateTimeOffset(a).subtract(toDateTimeOffset(b)));
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_subtract_time_span(
    dnv_vista_sdk_date_time_offset_t dto, dnv_vista_sdk_time_span_t value)
{
    return fromDateTimeOffset(toDateTimeOffset(dto).subtract(toTimeSpan(value)));
}

int dnv_vista_sdk_date_time_offset_equals(dnv_vista_sdk_date_time_offset_t a, dnv_vista_sdk_date_time_offset_t b)
{
    return toDateTimeOffset(a).equals(toDateTimeOffset(b)) ? 1 : 0;
}

int dnv_vista_sdk_date_time_offset_equals_exact(dnv_vista_sdk_date_time_offset_t a, dnv_vista_sdk_date_time_offset_t b)
{
    return toDateTimeOffset(a).equalsExact(toDateTimeOffset(b)) ? 1 : 0;
}

int dnv_vista_sdk_date_time_offset_compare(dnv_vista_sdk_date_time_offset_t a, dnv_vista_sdk_date_time_offset_t b)
{
    const auto cmp = toDateTimeOffset(a) <=> toDateTimeOffset(b);
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

int dnv_vista_sdk_date_time_offset_is_valid(dnv_vista_sdk_date_time_offset_t dto)
{
    return toDateTimeOffset(dto).isValid() ? 1 : 0;
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_now(void)
{
    return fromDateTimeOffset(DateTimeOffset::now());
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_utc_now(void)
{
    return fromDateTimeOffset(DateTimeOffset::utcNow());
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_today(void)
{
    return fromDateTimeOffset(DateTimeOffset::today());
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_min(void)
{
    return fromDateTimeOffset(DateTimeOffset::min());
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_max(void)
{
    return fromDateTimeOffset(DateTimeOffset::max());
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_epoch(void)
{
    return fromDateTimeOffset(DateTimeOffset::epoch());
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_from_epoch_seconds(int64_t seconds)
{
    return fromDateTimeOffset(DateTimeOffset::fromEpochSeconds(seconds));
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_from_epoch_milliseconds(int64_t milliseconds)
{
    return fromDateTimeOffset(DateTimeOffset::fromEpochMilliseconds(milliseconds));
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_from_filetime(int64_t filetime)
{
    return fromDateTimeOffset(DateTimeOffset::fromFILETIME(filetime));
}

char* dnv_vista_sdk_date_time_offset_to_string(
    dnv_vista_sdk_date_time_offset_t dto, dnv_vista_sdk_date_time_format_t format)
{
    return toOwnedCString(toDateTimeOffset(dto).toString(toDateTimeFormat(format)));
}

void dnv_vista_sdk_date_time_offset_string_free(char* str)
{
    delete[] str;
}
