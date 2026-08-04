/**
 * @file local_id.h
 * @brief C API for dnv::vista::sdk::LocalId
 * @details `dnv_vista_sdk_local_id_t*` is owned by value and must be released with
 *          dnv_vista_sdk_local_id_free
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "codebook_name.h"
#include "gmod_path.h"
#include "metadata_tag.h"
#include "parsing_errors.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_local_id dnv_vista_sdk_local_id_t;
    typedef struct dnv_vista_sdk_local_id_builder dnv_vista_sdk_local_id_builder_t;

    /** @brief Get the naming rule identifier ("dnv-v2") - static, never NULL */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_local_id_naming_rule(void);

    /**
     * @brief Release a LocalId handle
     * @param localId Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_local_id_free(dnv_vista_sdk_local_id_t* localId);

    /**
     * @brief Parse a LocalId string
     * @param localIdStr String representation (e.g. "/dnv-v2/vis-3-4a/411.1/H101/meta/qty-power")
     * @return Owned handle, must be released with dnv_vista_sdk_local_id_free, or
     *         NULL if `localIdStr` is invalid or NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_t* dnv_vista_sdk_local_id_from_string(const char* localIdStr);

    /**
     * @brief Parse a LocalId string, capturing detailed parsing errors
     * @param localIdStr String representation (e.g. "/dnv-v2/vis-3-4a/411.1/H101/meta/qty-power")
     * @param outErrors Receives an owned ParsingErrors handle (must be released with
     *                  dnv_vista_sdk_parsing_errors_free), even on success (empty in that case).
     *                  Must not be NULL
     * @return Owned handle, must be released with dnv_vista_sdk_local_id_free, or
     *         NULL if `localIdStr` is invalid or either argument is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_t* dnv_vista_sdk_local_id_from_string_with_errors(
        const char* localIdStr, dnv_vista_sdk_parsing_errors_t** outErrors);

    /** @brief Get the VIS version string, or NULL if `localId` is NULL */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_local_id_version(const dnv_vista_sdk_local_id_t* localId);

    /**
     * @brief Get the primary item
     * @return Borrowed pointer, valid as long as `localId` is valid, or NULL if
     *         `localId` is NULL
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_primary_item(
        const dnv_vista_sdk_local_id_t* localId);

    /**
     * @brief Get the secondary item
     * @return Borrowed pointer, valid as long as `localId` is valid, or NULL if
     *         `localId` is NULL or unset
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_secondary_item(
        const dnv_vista_sdk_local_id_t* localId);

    /** @brief 1 if verbose mode is enabled, 0 otherwise or if `localId` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_local_id_is_verbose_mode(const dnv_vista_sdk_local_id_t* localId);

    /** @brief 1 if any metadata tag is custom, 0 otherwise or if `localId` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_local_id_has_custom_tag(const dnv_vista_sdk_local_id_t* localId);

    /**
     * @brief Get a metadata tag by codebook name
     * @return Borrowed pointer, valid as long as `localId` is valid, or NULL if
     *         `localId` is NULL or the tag is unset
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_metadata_tag_t* dnv_vista_sdk_local_id_metadata_tag(
        const dnv_vista_sdk_local_id_t* localId, dnv_vista_sdk_codebook_name_t name);

    /**
     * @brief Get the builder that created this LocalId
     * @return Borrowed pointer, valid as long as `localId` is valid, or NULL if
     *         `localId` is NULL
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder(
        const dnv_vista_sdk_local_id_t* localId);

    /**
     * @brief Convert to string representation
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_local_id_string_free, or NULL if `localId` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_local_id_to_string(const dnv_vista_sdk_local_id_t* localId);

    /**
     * @brief Release a string obtained from this API
     * @param str String obtained from dnv_vista_sdk_local_id_to_string, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_local_id_string_free(char* str);

#ifdef __cplusplus
}
#endif
