/**
 * @file relative_location.h
 * @brief C API for dnv::vista::sdk::RelativeLocation
 * @details `dnv_vista_sdk_relative_location_t*` is a borrowed pointer, owned by the
 *          parent Locations - never freed independently
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "location.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_relative_location dnv_vista_sdk_relative_location_t;

    /**
     * @brief Get the single-character location code
     * @param relativeLocation Handle obtained from this API
     * @return The character code, or '\0' if `relativeLocation` is NULL
     */
    DNV_VISTA_SDK_C_API char dnv_vista_sdk_relative_location_code(
        const dnv_vista_sdk_relative_location_t* relativeLocation);

    /**
     * @brief Get the human-readable name
     * @param relativeLocation Handle obtained from this API
     * @return Null-terminated string, owned by `relativeLocation` - valid as long as
     *         `relativeLocation` is valid, or NULL if `relativeLocation` is NULL
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_relative_location_name(
        const dnv_vista_sdk_relative_location_t* relativeLocation);

    /**
     * @brief Get the optional definition text
     * @param relativeLocation Handle obtained from this API
     * @return Null-terminated string, owned by `relativeLocation` - valid as long as
     *         `relativeLocation` is valid, or NULL if `relativeLocation` is NULL or has
     *         no definition set
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_relative_location_definition(
        const dnv_vista_sdk_relative_location_t* relativeLocation);

    /**
     * @brief Get the canonical Location representation
     * @param relativeLocation Handle obtained from this API
     * @return Borrowed pointer, valid as long as `relativeLocation` is valid, or NULL
     *         if `relativeLocation` is NULL
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_location_t* dnv_vista_sdk_relative_location_location(
        const dnv_vista_sdk_relative_location_t* relativeLocation);

#ifdef __cplusplus
}
#endif
