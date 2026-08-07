/**
 * @file time_span.h
 * @brief C API for dnv::vista::sdk::TimeSpan
 * @details `dnv_vista_sdk_time_span_t` is a trivial value type (a single int64_t tick
 *          count) passed and returned by value - no allocation, no _free function
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** @brief Mirrors dnv::vista::sdk::TimeSpan - a duration in 100-nanosecond ticks */
    typedef struct
    {
        int64_t ticks;
    } dnv_vista_sdk_time_span_t;

    /** @brief Construct a TimeSpan from a tick count (100-nanosecond units) */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_from_ticks(int64_t ticks);

    /**
     * @brief Parse a TimeSpan from an ISO 8601 duration string or a numeric seconds string
     * @param str String to parse (e.g. "PT1H30M45S", "123.45", "-60")
     * @param result Set to the parsed TimeSpan on success, untouched on failure or if `str` is NULL
     * @return 1 if parsing succeeded, 0 otherwise
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_time_span_from_string(const char* str, dnv_vista_sdk_time_span_t* result);

    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_from_days(double days);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_from_hours(double hours);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_from_minutes(double minutes);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_from_seconds(double seconds);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_from_milliseconds(double milliseconds);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_from_microseconds(double microseconds);

    /** @brief Total days (fractional) */
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_time_span_days(dnv_vista_sdk_time_span_t ts);
    /** @brief Total hours (fractional) */
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_time_span_hours(dnv_vista_sdk_time_span_t ts);
    /** @brief Total minutes (fractional) */
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_time_span_minutes(dnv_vista_sdk_time_span_t ts);
    /** @brief Total seconds (fractional) */
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_time_span_seconds(dnv_vista_sdk_time_span_t ts);
    /** @brief Total milliseconds (fractional) */
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_time_span_milliseconds(dnv_vista_sdk_time_span_t ts);
    /** @brief Total microseconds (fractional) */
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_time_span_microseconds(dnv_vista_sdk_time_span_t ts);
    /** @brief Total nanoseconds (fractional) */
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_time_span_nanoseconds(dnv_vista_sdk_time_span_t ts);

    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t
    dnv_vista_sdk_time_span_add(dnv_vista_sdk_time_span_t a, dnv_vista_sdk_time_span_t b);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t
    dnv_vista_sdk_time_span_subtract(dnv_vista_sdk_time_span_t a, dnv_vista_sdk_time_span_t b);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t dnv_vista_sdk_time_span_negate(dnv_vista_sdk_time_span_t ts);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t
    dnv_vista_sdk_time_span_multiply(dnv_vista_sdk_time_span_t ts, double multiplier);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_time_span_t
    dnv_vista_sdk_time_span_divide(dnv_vista_sdk_time_span_t ts, double divisor);
    /** @brief Ratio of `a` to `b` (how many times `b` fits in `a`) */
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_time_span_ratio(dnv_vista_sdk_time_span_t a, dnv_vista_sdk_time_span_t b);

    /** @brief -1 if a < b, 0 if a == b, 1 if a > b */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_time_span_compare(dnv_vista_sdk_time_span_t a, dnv_vista_sdk_time_span_t b);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_time_span_equals(dnv_vista_sdk_time_span_t a, dnv_vista_sdk_time_span_t b);

    /**
     * @brief Convert to ISO 8601 duration string representation
     * @return Owned, null-terminated string, must be released with dnv_vista_sdk_string_free
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_time_span_to_string(dnv_vista_sdk_time_span_t ts);

#ifdef __cplusplus
}
#endif
