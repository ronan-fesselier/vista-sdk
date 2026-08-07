/**
 * @file date_time_offset.h
 * @brief C API for dnv::vista::sdk::DateTimeOffset
 * @details `dnv_vista_sdk_date_time_offset_t` is a trivial value type (a DateTime tick
 *          count and a TimeSpan offset tick count) passed and returned by value - no
 *          allocation, no _free function
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "date_time.h"
#include "time_span.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** @brief Mirrors dnv::vista::sdk::DateTimeOffset - local DateTime paired with a UTC offset */
    typedef struct
    {
        dnv_vista_sdk_date_time_t dateTime;
        dnv_vista_sdk_time_span_t offset;
    } dnv_vista_sdk_date_time_offset_t;

    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_create(dnv_vista_sdk_date_time_t dateTime, dnv_vista_sdk_time_span_t offset);

    /** @brief Construct from a DateTime, using the system's local timezone offset */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_from_date_time(dnv_vista_sdk_date_time_t dateTime);

    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_from_ticks(int64_t ticks, dnv_vista_sdk_time_span_t offset);

    /**
     * @brief Parse a DateTimeOffset from an ISO 8601 string with timezone offset
     * @param iso8601String String to parse, must not be NULL
     * @param result Set to the parsed DateTimeOffset on success, untouched on failure
     * @return 1 if parsing succeeded, 0 otherwise
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_date_time_offset_from_string(
        const char* iso8601String, dnv_vista_sdk_date_time_offset_t* result);

    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t
    dnv_vista_sdk_date_time_offset_date_time(dnv_vista_sdk_date_time_offset_t dto);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t
    dnv_vista_sdk_date_time_offset_offset(dnv_vista_sdk_date_time_offset_t dto);
    /** @brief Equivalent UTC DateTime */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t
    dnv_vista_sdk_date_time_offset_utc_date_time(dnv_vista_sdk_date_time_offset_t dto);
    /** @brief Equivalent local DateTime (system timezone) */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t
    dnv_vista_sdk_date_time_offset_local_date_time(dnv_vista_sdk_date_time_offset_t dto);
    /** @brief Local time as 100ns ticks */
    DNV_VISTA_SDK_C_API int64_t dnv_vista_sdk_date_time_offset_ticks(dnv_vista_sdk_date_time_offset_t dto);
    /** @brief UTC time as 100ns ticks */
    DNV_VISTA_SDK_C_API int64_t dnv_vista_sdk_date_time_offset_utc_ticks(dnv_vista_sdk_date_time_offset_t dto);

    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_offset_year(dnv_vista_sdk_date_time_offset_t dto);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_offset_month(dnv_vista_sdk_date_time_offset_t dto);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_offset_day(dnv_vista_sdk_date_time_offset_t dto);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_offset_hour(dnv_vista_sdk_date_time_offset_t dto);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_offset_minute(dnv_vista_sdk_date_time_offset_t dto);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_offset_second(dnv_vista_sdk_date_time_offset_t dto);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_offset_millisecond(dnv_vista_sdk_date_time_offset_t dto);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_offset_microsecond(dnv_vista_sdk_date_time_offset_t dto);
    /** @brief Nanosecond component in 100ns increments (0, 100, 200, ..., 900) */
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_offset_nanosecond(dnv_vista_sdk_date_time_offset_t dto);
    /** @brief Day of week (0=Sunday, 6=Saturday) */
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_offset_day_of_week(dnv_vista_sdk_date_time_offset_t dto);
    /** @brief Day of year (1-366) */
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_offset_day_of_year(dnv_vista_sdk_date_time_offset_t dto);
    /** @brief Total minutes offset from UTC (positive for East, negative for West) */
    DNV_VISTA_SDK_C_API int32_t
    dnv_vista_sdk_date_time_offset_total_offset_minutes(dnv_vista_sdk_date_time_offset_t dto);

    DNV_VISTA_SDK_C_API int64_t dnv_vista_sdk_date_time_offset_to_epoch_seconds(dnv_vista_sdk_date_time_offset_t dto);
    DNV_VISTA_SDK_C_API int64_t
    dnv_vista_sdk_date_time_offset_to_epoch_milliseconds(dnv_vista_sdk_date_time_offset_t dto);
    /** @brief Date component with time set to 00:00:00 */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_date(dnv_vista_sdk_date_time_offset_t dto);
    /** @brief Elapsed time since midnight */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t
    dnv_vista_sdk_date_time_offset_time_of_day(dnv_vista_sdk_date_time_offset_t dto);

    /** @brief Convert to a different UTC offset, representing the same instant */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_to_offset(dnv_vista_sdk_date_time_offset_t dto, dnv_vista_sdk_time_span_t newOffset);
    /** @brief Convert to UTC (offset = 00:00:00) */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_to_universal_time(dnv_vista_sdk_date_time_offset_t dto);
    /** @brief Convert to local time (system timezone) */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_to_local_time(dnv_vista_sdk_date_time_offset_t dto);
    /** @brief Windows FILETIME: 100-nanosecond intervals since January 1, 1601 UTC */
    DNV_VISTA_SDK_C_API int64_t dnv_vista_sdk_date_time_offset_to_filetime(dnv_vista_sdk_date_time_offset_t dto);

    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_add_time_span(dnv_vista_sdk_date_time_offset_t dto, dnv_vista_sdk_time_span_t value);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_add_days(dnv_vista_sdk_date_time_offset_t dto, double days);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_add_hours(dnv_vista_sdk_date_time_offset_t dto, double hours);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_add_minutes(dnv_vista_sdk_date_time_offset_t dto, double minutes);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_add_seconds(dnv_vista_sdk_date_time_offset_t dto, double seconds);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_add_milliseconds(dnv_vista_sdk_date_time_offset_t dto, double milliseconds);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_add_months(dnv_vista_sdk_date_time_offset_t dto, int32_t months);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_add_years(dnv_vista_sdk_date_time_offset_t dto, int32_t years);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_add_ticks(dnv_vista_sdk_date_time_offset_t dto, int64_t ticks);

    /** @brief Elapsed TimeSpan between two DateTimeOffset instances (UTC instants) */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t
    dnv_vista_sdk_date_time_offset_subtract(dnv_vista_sdk_date_time_offset_t a, dnv_vista_sdk_date_time_offset_t b);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_subtract_time_span(
        dnv_vista_sdk_date_time_offset_t dto, dnv_vista_sdk_time_span_t value);

    /** @brief True if both represent the same instant in UTC (offsets may differ) */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_date_time_offset_equals(
        dnv_vista_sdk_date_time_offset_t a, dnv_vista_sdk_date_time_offset_t b);
    /** @brief True if both have the same local time AND the same offset */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_date_time_offset_equals_exact(
        dnv_vista_sdk_date_time_offset_t a, dnv_vista_sdk_date_time_offset_t b);
    /** @brief -1 if a < b, 0 if a == b, 1 if a > b (compares UTC instants) */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_date_time_offset_compare(
        dnv_vista_sdk_date_time_offset_t a, dnv_vista_sdk_date_time_offset_t b);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_date_time_offset_is_valid(dnv_vista_sdk_date_time_offset_t dto);

    /** @brief Current local time with system timezone offset */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_now(void);
    /** @brief Current UTC time (offset = 00:00:00) */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_utc_now(void);
    /** @brief Current local date with time set to 00:00:00 */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_today(void);
    /** @brief January 1, 0001 00:00:00.0000000, zero offset */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_min(void);
    /** @brief December 31, 9999 23:59:59.9999999, zero offset */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_max(void);
    /** @brief January 1, 1970 00:00:00 UTC, zero offset */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_epoch(void);

    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_from_epoch_seconds(int64_t seconds);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_date_time_offset_from_epoch_milliseconds(int64_t milliseconds);
    /** @brief From Windows FILETIME (100-nanosecond intervals since January 1, 1601 UTC) */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_date_time_offset_from_filetime(int64_t filetime);

    /**
     * @brief Convert to string representation using the specified format
     * @return Owned, null-terminated string, must be released with dnv_vista_sdk_string_free
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_date_time_offset_to_string(
        dnv_vista_sdk_date_time_offset_t dto, dnv_vista_sdk_date_time_format_t format);

#ifdef __cplusplus
}
#endif
