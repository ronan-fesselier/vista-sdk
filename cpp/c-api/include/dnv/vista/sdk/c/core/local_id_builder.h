/**
 * @file local_id_builder.h
 * @brief C API for dnv::vista::sdk::LocalIdBuilder
 * @details `dnv_vista_sdk_local_id_builder_t*` is owned by value and must be released
 *          with dnv_vista_sdk_local_id_builder_free. Every with_x/without_x function
 *          returns a new, independent handle - it does not mutate the input
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "codebook_name.h"
#include "gmod_path.h"
#include "local_id.h"
#include "metadata_tag.h"
#include "parsing_errors.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_local_id_builder dnv_vista_sdk_local_id_builder_t;

    /** @brief Get the naming rule identifier ("dnv-v2") - static, never NULL */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_local_id_builder_naming_rule(void);

    /**
     * @brief Create a new LocalIdBuilder for a VIS version
     * @param visVersion VIS version string (e.g. "3-11a")
     * @return Owned handle, must be released with dnv_vista_sdk_local_id_builder_free,
     *         or NULL if `visVersion` is invalid or NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_create(const char* visVersion);

    /**
     * @brief Release a LocalIdBuilder handle
     * @param builder Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_local_id_builder_free(dnv_vista_sdk_local_id_builder_t* builder);

    /** @brief Get the VIS version string, or NULL if `builder` is NULL or unset */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_local_id_builder_version(
        const dnv_vista_sdk_local_id_builder_t* builder);

    /** @brief 1 if verbose mode is enabled, 0 otherwise or if `builder` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_local_id_builder_is_verbose_mode(
        const dnv_vista_sdk_local_id_builder_t* builder);

    /** @brief 1 if the builder can build a LocalId, 0 otherwise or if `builder` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_local_id_builder_is_valid(const dnv_vista_sdk_local_id_builder_t* builder);

    /** @brief 1 if no components are set, 0 otherwise or if `builder` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_local_id_builder_is_empty(const dnv_vista_sdk_local_id_builder_t* builder);

    /** @brief 1 if no metadata tags are set, 0 otherwise or if `builder` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_local_id_builder_is_empty_metadata(
        const dnv_vista_sdk_local_id_builder_t* builder);

    /** @brief 1 if any metadata tag is custom, 0 otherwise or if `builder` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_local_id_builder_has_custom_tag(
        const dnv_vista_sdk_local_id_builder_t* builder);

    /**
     * @brief Get the primary item
     * @return Owned handle (a copy), must be released with dnv_vista_sdk_gmod_path_free,
     *         or NULL if `builder` is NULL or unset
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_builder_primary_item(
        const dnv_vista_sdk_local_id_builder_t* builder);

    /**
     * @brief Get the secondary item
     * @return Owned handle (a copy), must be released with dnv_vista_sdk_gmod_path_free,
     *         or NULL if `builder` is NULL or unset
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_builder_secondary_item(
        const dnv_vista_sdk_local_id_builder_t* builder);

    /**
     * @brief Get a metadata tag by codebook name
     * @return Owned handle (a copy), must be released with dnv_vista_sdk_metadata_tag_free,
     *         or NULL if `builder` is NULL or the tag is unset
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_metadata_tag_t* dnv_vista_sdk_local_id_builder_metadata_tag(
        const dnv_vista_sdk_local_id_builder_t* builder, dnv_vista_sdk_codebook_name_t name);

    /** @brief Set the VIS version. Returns NULL if `builder` is NULL or `visVersion` is invalid */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_with_vis_version(
        const dnv_vista_sdk_local_id_builder_t* builder, const char* visVersion);

    /** @brief Remove the VIS version. Returns NULL if `builder` is NULL */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_without_vis_version(
        const dnv_vista_sdk_local_id_builder_t* builder);

    /** @brief Set the primary item. Returns NULL if `builder`/`path` is NULL */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_with_primary_item(
        const dnv_vista_sdk_local_id_builder_t* builder, const dnv_vista_sdk_gmod_path_t* path);

    /** @brief Remove the primary item. Returns NULL if `builder` is NULL */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_without_primary_item(
        const dnv_vista_sdk_local_id_builder_t* builder);

    /** @brief Set the secondary item. Returns NULL if `builder`/`path` is NULL */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_with_secondary_item(
        const dnv_vista_sdk_local_id_builder_t* builder, const dnv_vista_sdk_gmod_path_t* path);

    /** @brief Remove the secondary item. Returns NULL if `builder` is NULL */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_without_secondary_item(
        const dnv_vista_sdk_local_id_builder_t* builder);

    /**
     * @brief Set a metadata tag (codebook name determines which slot)
     * @return New owned handle, or NULL if `builder`/`tag` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_with_metadata_tag(
        const dnv_vista_sdk_local_id_builder_t* builder, const dnv_vista_sdk_metadata_tag_t* tag);

    /** @brief Remove a metadata tag by codebook name. Returns NULL if `builder` is NULL */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_without_metadata_tag(
        const dnv_vista_sdk_local_id_builder_t* builder, dnv_vista_sdk_codebook_name_t name);

    /** @brief Set verbose mode. Returns NULL if `builder` is NULL */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_builder_with_verbose_mode(
        const dnv_vista_sdk_local_id_builder_t* builder, int verbose);

    /**
     * @brief Build the final LocalId from the builder's current state
     * @return Owned handle, must be released with dnv_vista_sdk_local_id_free, or
     *         NULL if `builder` is NULL or invalid (e.g. no primary item)
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_t* dnv_vista_sdk_local_id_builder_build(
        const dnv_vista_sdk_local_id_builder_t* builder);

    /**
     * @brief Convert builder state to string representation
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_string_free, or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_local_id_builder_to_string(const dnv_vista_sdk_local_id_builder_t* builder);

    /**
     * @brief Parse a LocalId string into a builder-backed LocalId
     * @return Owned handle, must be released with dnv_vista_sdk_local_id_free, or
     *         NULL if `localIdStr` is invalid or NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_t* dnv_vista_sdk_local_id_builder_from_string(const char* localIdStr);

    /**
     * @brief Parse a LocalId string, collecting detailed error information on failure
     * @param localIdStr LocalId string to parse
     * @param outErrors Set to an owned ParsingErrors handle (must be released with
     *                  dnv_vista_sdk_parsing_errors_free), even on success. Left
     *                  untouched if `localIdStr`/`outErrors` is NULL
     * @return Owned handle, must be released with dnv_vista_sdk_local_id_free, or
     *         NULL if `localIdStr` is invalid or NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_t* dnv_vista_sdk_local_id_builder_from_string_with_errors(
        const char* localIdStr, dnv_vista_sdk_parsing_errors_t** outErrors);

#ifdef __cplusplus
}
#endif
