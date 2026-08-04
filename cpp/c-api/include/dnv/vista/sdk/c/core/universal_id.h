/**
 * @file universal_id.h
 * @brief C API for dnv::vista::sdk::UniversalId
 * @details `dnv_vista_sdk_universal_id_t*` is owned by value and must be released
 *          with dnv_vista_sdk_universal_id_free
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "imo_number.h"
#include "local_id.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_universal_id dnv_vista_sdk_universal_id_t;
    typedef struct dnv_vista_sdk_universal_id_builder dnv_vista_sdk_universal_id_builder_t;

    /** @brief Get the naming entity identifier ("data.dnv.com") - static, never NULL */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_universal_id_naming_entity(void);

    /**
     * @brief Release a UniversalId handle
     * @param universalId Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_universal_id_free(dnv_vista_sdk_universal_id_t* universalId);

    /**
     * @brief Parse a UniversalId string
     * @param universalIdStr String representation (e.g.
     *        "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass")
     * @return Owned handle, must be released with dnv_vista_sdk_universal_id_free,
     *         or NULL if `universalIdStr` is invalid or NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_universal_id_t* dnv_vista_sdk_universal_id_from_string(
        const char* universalIdStr);

    /**
     * @brief Get the IMO number
     * @return Borrowed pointer, valid as long as `universalId` is valid, or NULL if
     *         `universalId` is NULL
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_imo_number_t* dnv_vista_sdk_universal_id_imo_number(
        const dnv_vista_sdk_universal_id_t* universalId);

    /**
     * @brief Get the LocalId
     * @return Borrowed pointer, valid as long as `universalId` is valid, or NULL if
     *         `universalId` is NULL
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_local_id_t* dnv_vista_sdk_universal_id_local_id(
        const dnv_vista_sdk_universal_id_t* universalId);

    /**
     * @brief Get the builder that created this UniversalId
     * @return Borrowed pointer, valid as long as `universalId` is valid, or NULL if
     *         `universalId` is NULL
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_universal_id_builder_t* dnv_vista_sdk_universal_id_builder(
        const dnv_vista_sdk_universal_id_t* universalId);

    /**
     * @brief Convert to string representation
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_universal_id_string_free, or NULL if `universalId` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_universal_id_to_string(const dnv_vista_sdk_universal_id_t* universalId);

    /**
     * @brief Release a string obtained from this API
     * @param str String obtained from dnv_vista_sdk_universal_id_to_string, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_universal_id_string_free(char* str);

#ifdef __cplusplus
}
#endif
