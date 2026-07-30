/**
 * @file location.h
 * @brief C API for dnv::vista::sdk::Location
 * @details `dnv_vista_sdk_location_t*` is owned by value and must be released
 *          with dnv_vista_sdk_location_free
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_location dnv_vista_sdk_location_t;

    /**
     * @brief Release a Location handle
     * @param location Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_location_free(dnv_vista_sdk_location_t* location);

    /**
     * @brief Get the canonical location string
     * @param location Handle obtained from this API
     * @return Null-terminated string, owned by `location` - valid as long as
     *         `location` is valid, or NULL if `location` is NULL
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_location_value(const dnv_vista_sdk_location_t* location);

#ifdef __cplusplus
}
#endif
