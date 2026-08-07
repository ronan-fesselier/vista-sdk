/**
 * @file date_time.h
 * @brief C API for dnv::vista::sdk::DateTime
 * @details `dnv_vista_sdk_date_time_t` is a trivial value type (a single int64_t tick
 *          count) passed and returned by value - no allocation, no _free function.
 *          toChrono()/fromChrono() are not exposed - std::chrono::system_clock::time_point
 *          has no portable C representation: use the epoch-seconds/milliseconds
 *          functions instead, which cover the same practical need
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "time_span.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** @brief Mirrors dnv::vista::sdk::DateTime - 100-nanosecond ticks since January 1, 0001 UTC */
    typedef struct
    {
        int64_t ticks;
    } dnv_vista_sdk_date_time_t;

    /** @brief Mirrors dnv::vista::sdk::DateTime::Format */
    typedef enum
    {
        DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601 = 0,
        DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_PRECISE,
        DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_PRECISE_TRIMMED,
        DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_MILLIS,
        DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_MICROS,
        DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_EXTENDED,
        DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_BASIC,
        DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_DATE,
        DNV_VISTA_SDK_DATE_TIME_FORMAT_ISO8601_TIME,
        DNV_VISTA_SDK_DATE_TIME_FORMAT_UNIX_SECONDS,
        DNV_VISTA_SDK_DATE_TIME_FORMAT_UNIX_MILLISECONDS
    } dnv_vista_sdk_date_time_format_t;

    /** @brief Construct a DateTime from a tick count (100-nanosecond units since year 1) */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_from_ticks(int64_t ticks);

    /**
     * @brief Construct a DateTime from date components
     * @return Zero-initialized (min()) if the components are invalid
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t
    dnv_vista_sdk_date_time_from_date(int32_t year, int32_t month, int32_t day);

    /**
     * @brief Construct a DateTime from date and time components
     * @return Zero-initialized (min()) if the components are invalid
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_from_date_time(
        int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second);

    /**
     * @brief Construct a DateTime from date and time components with milliseconds
     * @return Zero-initialized (min()) if the components are invalid
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_from_date_time_millis(
        int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond);

    /**
     * @brief Parse a DateTime from an ISO 8601 string
     * @param iso8601String String to parse, must not be NULL
     * @param result Set to the parsed DateTime on success, untouched on failure
     * @return 1 if parsing succeeded, 0 otherwise
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_date_time_from_string(
        const char* iso8601String, dnv_vista_sdk_date_time_t* result);

    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_year(dnv_vista_sdk_date_time_t dt);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_month(dnv_vista_sdk_date_time_t dt);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_day(dnv_vista_sdk_date_time_t dt);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_hour(dnv_vista_sdk_date_time_t dt);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_minute(dnv_vista_sdk_date_time_t dt);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_second(dnv_vista_sdk_date_time_t dt);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_millisecond(dnv_vista_sdk_date_time_t dt);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_microsecond(dnv_vista_sdk_date_time_t dt);
    /** @brief Nanosecond component in 100ns increments (0, 100, 200, ..., 900) */
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_nanosecond(dnv_vista_sdk_date_time_t dt);

    DNV_VISTA_SDK_C_API int64_t dnv_vista_sdk_date_time_ticks(dnv_vista_sdk_date_time_t dt);
    /** @brief Day of week (0=Sunday, 6=Saturday) */
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_day_of_week(dnv_vista_sdk_date_time_t dt);
    /** @brief Day of year (1-366) */
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_day_of_year(dnv_vista_sdk_date_time_t dt);
    DNV_VISTA_SDK_C_API int64_t dnv_vista_sdk_date_time_to_epoch_seconds(dnv_vista_sdk_date_time_t dt);
    DNV_VISTA_SDK_C_API int64_t dnv_vista_sdk_date_time_to_epoch_milliseconds(dnv_vista_sdk_date_time_t dt);
    /** @brief Date component with time set to 00:00:00 */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_date(dnv_vista_sdk_date_time_t dt);
    /** @brief Elapsed time since midnight */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t dnv_vista_sdk_date_time_time_of_day(dnv_vista_sdk_date_time_t dt);

    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t
    dnv_vista_sdk_date_time_add_days(dnv_vista_sdk_date_time_t dt, double days);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t
    dnv_vista_sdk_date_time_add_hours(dnv_vista_sdk_date_time_t dt, double hours);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t
    dnv_vista_sdk_date_time_add_minutes(dnv_vista_sdk_date_time_t dt, double minutes);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t
    dnv_vista_sdk_date_time_add_seconds(dnv_vista_sdk_date_time_t dt, double seconds);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t
    dnv_vista_sdk_date_time_add_milliseconds(dnv_vista_sdk_date_time_t dt, double milliseconds);
    /** @brief If the resulting day exceeds the days in the target month, it is clamped */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t
    dnv_vista_sdk_date_time_add_months(dnv_vista_sdk_date_time_t dt, int32_t months);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t
    dnv_vista_sdk_date_time_add_years(dnv_vista_sdk_date_time_t dt, int32_t years);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t
    dnv_vista_sdk_date_time_add_time_span(dnv_vista_sdk_date_time_t dt, dnv_vista_sdk_time_span_t duration);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t
    dnv_vista_sdk_date_time_subtract_time_span(dnv_vista_sdk_date_time_t dt, dnv_vista_sdk_time_span_t duration);
    /** @brief Elapsed TimeSpan between two DateTime instances */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t
    dnv_vista_sdk_date_time_subtract(dnv_vista_sdk_date_time_t a, dnv_vista_sdk_date_time_t b);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_date_time_is_valid(dnv_vista_sdk_date_time_t dt);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_date_time_is_leap_year(int32_t year);
    DNV_VISTA_SDK_C_API int32_t dnv_vista_sdk_date_time_days_in_month(int32_t year, int32_t month);

    /** @brief Current UTC time */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_utc_now(void);
    /** @brief January 1, 0001 00:00:00.0000000 UTC */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_min(void);
    /** @brief December 31, 9999 23:59:59.9999999 UTC */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_max(void);
    /** @brief January 1, 1970 00:00:00 UTC */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_epoch(void);

    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_from_epoch_seconds(int64_t seconds);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_t dnv_vista_sdk_date_time_from_epoch_milliseconds(int64_t milliseconds);

    /** @brief -1 if a < b, 0 if a == b, 1 if a > b */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_date_time_compare(dnv_vista_sdk_date_time_t a, dnv_vista_sdk_date_time_t b);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_date_time_equals(dnv_vista_sdk_date_time_t a, dnv_vista_sdk_date_time_t b);

    /**
     * @brief Convert to string representation using the specified format
     * @return Owned, null-terminated string, must be released with dnv_vista_sdk_date_time_string_free
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_date_time_to_string(
        dnv_vista_sdk_date_time_t dt, dnv_vista_sdk_date_time_format_t format);

    /**
     * @brief Release a string obtained from this API
     * @param str String obtained from dnv_vista_sdk_date_time_to_string, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_date_time_string_free(char* str);

#ifdef __cplusplus
}
#endif
