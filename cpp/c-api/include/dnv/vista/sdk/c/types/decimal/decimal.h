/**
 * @file decimal.h
 * @brief C API for dnv::vista::sdk::Decimal
 * @details `dnv_vista_sdk_decimal_t` is a trivial 128-bit value type (flags + 96-bit
 *          mantissa) passed and returned by value - no allocation, no _free function.
 *          Comparisons against float/int32_t are not exposed as separate functions -
 *          the C++ overloads for those types are themselves trivial widening wrappers
 *          around the double/int64_t comparisons (cast the value on the caller side
 *          and use dnv_vista_sdk_decimal_compare_double/compare_int64)
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** @brief Mirrors dnv::vista::sdk::Decimal - mantissa/10^scale, sign in bit 31 of flags */
    typedef struct
    {
        uint32_t flags;
        uint32_t mantissa[3];
    } dnv_vista_sdk_decimal_t;

    /** @brief Mirrors dnv::vista::sdk::Decimal::RoundingMode */
    typedef enum
    {
        DNV_VISTA_SDK_DECIMAL_ROUNDING_MODE_TO_NEAREST = 0,
        DNV_VISTA_SDK_DECIMAL_ROUNDING_MODE_TO_NEAREST_TIES_AWAY,
        DNV_VISTA_SDK_DECIMAL_ROUNDING_MODE_TO_ZERO,
        DNV_VISTA_SDK_DECIMAL_ROUNDING_MODE_TO_POSITIVE_INFINITY,
        DNV_VISTA_SDK_DECIMAL_ROUNDING_MODE_TO_NEGATIVE_INFINITY
    } dnv_vista_sdk_decimal_rounding_mode_t;

    /** @brief Zero-valued Decimal */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_zero(void);

    /** @brief Construct from double. NaN/Infinity become zero. Limited to ~15-17 significant digits */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_from_double(double value);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_from_int64(int64_t value);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_from_uint64(uint64_t value);

    /**
     * @brief Parse a Decimal from a string
     * @param str String to parse (e.g. "123.45", "-0.001", "1.23e5"), must not be NULL
     * @param result Set to the parsed Decimal on success, untouched on failure
     * @return 1 if parsing succeeded, 0 otherwise
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_decimal_from_string(const char* str, dnv_vista_sdk_decimal_t* result);

    /** @brief Number of decimal places in the stored value (0-28) */
    DNV_VISTA_SDK_C_API uint8_t dnv_vista_sdk_decimal_scale(dnv_vista_sdk_decimal_t d);

    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t
    dnv_vista_sdk_decimal_add(dnv_vista_sdk_decimal_t a, dnv_vista_sdk_decimal_t b);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t
    dnv_vista_sdk_decimal_subtract(dnv_vista_sdk_decimal_t a, dnv_vista_sdk_decimal_t b);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t
    dnv_vista_sdk_decimal_multiply(dnv_vista_sdk_decimal_t a, dnv_vista_sdk_decimal_t b);

    /**
     * @brief Division sets the last error message and returns zero if `b` is zero
     * @details The C++ operator throws std::overflow_error on division by zero - caught
     *          at the ABI boundary and reported via dnv_vista_sdk_last_error_message()
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t
    dnv_vista_sdk_decimal_divide(dnv_vista_sdk_decimal_t a, dnv_vista_sdk_decimal_t b);

    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_negate(dnv_vista_sdk_decimal_t d);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_abs(dnv_vista_sdk_decimal_t d);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_ceil(dnv_vista_sdk_decimal_t d);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_floor(dnv_vista_sdk_decimal_t d);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_trunc(dnv_vista_sdk_decimal_t d);

    /**
     * @brief Round to the specified precision using the given rounding mode
     * @param d Decimal value to round
     * @param decimalPlacesCount Number of decimal places to round to (0 for integer rounding)
     * @param mode Rounding mode to apply
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_round(
        dnv_vista_sdk_decimal_t d, int32_t decimalPlacesCount, dnv_vista_sdk_decimal_rounding_mode_t mode);

    /**
     * @brief Compute square root
     * @details Sets the last error message and returns zero if `d` is negative (the C++
     *          method throws std::domain_error, caught at the ABI boundary)
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_sqrt(dnv_vista_sdk_decimal_t d);

    /** @brief -1 if a < b, 0 if a == b, 1 if a > b */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_decimal_compare(dnv_vista_sdk_decimal_t a, dnv_vista_sdk_decimal_t b);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_decimal_equals(dnv_vista_sdk_decimal_t a, dnv_vista_sdk_decimal_t b);

    /** @brief -1 if d < val, 0 if d == val, 1 if d > val. NaN/Infinity in `val` always compare unordered (0) */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_decimal_compare_double(dnv_vista_sdk_decimal_t d, double val);
    /** @brief -1 if d < val, 0 if d == val, 1 if d > val */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_decimal_compare_int64(dnv_vista_sdk_decimal_t d, int64_t val);
    /** @brief -1 if d < val, 0 if d == val, 1 if d > val */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_decimal_compare_uint64(dnv_vista_sdk_decimal_t d, uint64_t val);

    /** @brief Convert to double; may lose precision beyond ~15-17 significant digits */
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_decimal_to_double(dnv_vista_sdk_decimal_t d);

    /**
     * @brief Get the internal 32-bit representation (mirrors System.Decimal.GetBits/toBits)
     * @param d Decimal value to inspect
     * @param bits Array of 4 int32_t, written on return: [0]=mantissa[0], [1]=mantissa[1],
     *             [2]=mantissa[2], [3]=flags
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_decimal_to_bits(dnv_vista_sdk_decimal_t d, int32_t bits[4]);

    /** @brief Number of significant decimal places, excluding trailing zeros (0-28) */
    DNV_VISTA_SDK_C_API uint8_t dnv_vista_sdk_decimal_decimal_places_count(dnv_vista_sdk_decimal_t d);
    /** @brief Number of significant digits, excluding sign and decimal point (at least 1) */
    DNV_VISTA_SDK_C_API uint32_t dnv_vista_sdk_decimal_total_digits_count(dnv_vista_sdk_decimal_t d);

    /** @brief Smallest positive value: 1 x 10^-28 */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_min(void);
    /** @brief Maximum value: 79,228,162,514,264,337,593,543,950,335 */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_max(void);
    /** @brief Most negative value: -79,228,162,514,264,337,593,543,950,335 */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_lowest(void);
    /** @brief Epsilon for 28 decimal places: 1 x 10^-28 */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_decimal_t dnv_vista_sdk_decimal_epsilon(void);

    /**
     * @brief Convert to string with exact precision
     * @return Owned, null-terminated string, must be released with dnv_vista_sdk_string_free
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_decimal_to_string(dnv_vista_sdk_decimal_t d);

#ifdef __cplusplus
}
#endif
