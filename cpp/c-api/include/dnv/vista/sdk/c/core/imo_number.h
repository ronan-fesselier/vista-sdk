/**
 * @file imo_number.h
 * @brief C API for dnv::vista::sdk::ImoNumber
 * @details `dnv_vista_sdk_imo_number_t*` is owned by value and must be released
 *          with dnv_vista_sdk_imo_number_free
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_imo_number dnv_vista_sdk_imo_number_t;

    /**
     * @brief Check if an integer value represents a valid IMO number
     * @param imoNumber Integer value to check (e.g. 9074729)
     * @return 1 if valid (7-digit structure and checksum), 0 otherwise
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_imo_number_is_valid(int imoNumber);

    /**
     * @brief Construct an IMO number from an integer, with checksum validation
     * @param value Seven-digit IMO number (e.g. 9074729)
     * @return Owned handle, must be released with dnv_vista_sdk_imo_number_free, or
     *         NULL if `value` fails checksum validation
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_imo_number_t* dnv_vista_sdk_imo_number_create(int value);

    /**
     * @brief Parse an IMO number from a string
     * @param value String with or without "IMO" prefix (e.g. "IMO9074729" or "9074729")
     * @return Owned handle, must be released with dnv_vista_sdk_imo_number_free, or
     *         NULL if `value` is invalid (bad format or failed checksum) or NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_imo_number_t* dnv_vista_sdk_imo_number_from_string(const char* value);

    /**
     * @brief Release an ImoNumber handle
     * @param imoNumber Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_imo_number_free(dnv_vista_sdk_imo_number_t* imoNumber);

    /**
     * @brief Compare two ImoNumber handles for equality
     * @param a First handle
     * @param b Second handle
     * @return 1 if both represent the same IMO number, 0 otherwise or if either is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_imo_number_equals(
        const dnv_vista_sdk_imo_number_t* a, const dnv_vista_sdk_imo_number_t* b);

    /**
     * @brief Convert to string with "IMO" prefix
     * @param imoNumber Handle obtained from this API
     * @return Owned, null-terminated string (e.g. "IMO9074729"), must be released with
     *         dnv_vista_sdk_string_free, or NULL if `imoNumber` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_imo_number_to_string(const dnv_vista_sdk_imo_number_t* imoNumber);

#ifdef __cplusplus
}
#endif
