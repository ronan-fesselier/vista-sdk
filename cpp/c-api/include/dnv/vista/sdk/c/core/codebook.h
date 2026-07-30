/**
 * @file codebook.h
 * @brief C API for dnv::vista::sdk::Codebook
 * @details `dnv_vista_sdk_codebook_t*` is a borrowed pointer, owned by the parent
 *          Codebooks - never freed independently
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "metadata_tag.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_codebook dnv_vista_sdk_codebook_t;

    /**
     * @brief Result codes for position string validation
     * @details Values below 100 indicate invalid positions, values 100 and above are acceptable
     */
    typedef enum dnv_vista_sdk_position_validation_result
    {
        DNV_VISTA_SDK_POSITION_VALIDATION_INVALID = 0,
        DNV_VISTA_SDK_POSITION_VALIDATION_INVALID_ORDER = 1,
        DNV_VISTA_SDK_POSITION_VALIDATION_INVALID_GROUPING = 2,
        DNV_VISTA_SDK_POSITION_VALIDATION_VALID = 100,
        DNV_VISTA_SDK_POSITION_VALIDATION_CUSTOM = 101
    } dnv_vista_sdk_position_validation_result_t;

    /**
     * @brief Get the codebook name
     * @param codebook Handle obtained from dnv_vista_sdk_codebooks_at
     * @return Codebook name, or 0 if `codebook` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_codebook_name_t
    dnv_vista_sdk_codebook_name(const dnv_vista_sdk_codebook_t* codebook);

    /**
     * @brief Get the number of standard values in this codebook
     * @param codebook Handle obtained from dnv_vista_sdk_codebooks_at
     * @return Number of standard values, or 0 if `codebook` is NULL
     */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_codebook_standard_values_count(const dnv_vista_sdk_codebook_t* codebook);

    /**
     * @brief Get a standard value by index
     * @param codebook Handle obtained from dnv_vista_sdk_codebooks_at
     * @param index Index into the standard values set
     * @return Null-terminated string owned by the codebook, valid as long as `codebook` is valid,
     *         or NULL if `codebook` is NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_codebook_standard_value_at(
        const dnv_vista_sdk_codebook_t* codebook, size_t index);

    /**
     * @brief Get the number of groups in this codebook
     * @param codebook Handle obtained from dnv_vista_sdk_codebooks_at
     * @return Number of groups, or 0 if `codebook` is NULL
     */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_codebook_groups_count(const dnv_vista_sdk_codebook_t* codebook);

    /**
     * @brief Get a group name by index
     * @param codebook Handle obtained from dnv_vista_sdk_codebooks_at
     * @param index Index into the groups set
     * @return Null-terminated string owned by the codebook, valid as long as `codebook` is valid,
     *         or NULL if `codebook` is NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_codebook_group_at(
        const dnv_vista_sdk_codebook_t* codebook, size_t index);

    /**
     * @brief Check if a group exists in this codebook
     * @param codebook Handle obtained from dnv_vista_sdk_codebooks_at
     * @param group Group name to check
     * @return 1 if the group exists, 0 otherwise or if `codebook`/`group` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_codebook_has_group(
        const dnv_vista_sdk_codebook_t* codebook, const char* group);

    /**
     * @brief Check if a value is a standard value in this codebook
     * @param codebook Handle obtained from dnv_vista_sdk_vis_codebooks
     * @param value Value to check
     * @return 1 if `value` is a standard value, 0 otherwise or if `codebook`/`value` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_codebook_has_standard_value(
        const dnv_vista_sdk_codebook_t* codebook, const char* value);

    /**
     * @brief Validate a position string with VIS position rules
     * @param codebook Handle obtained from dnv_vista_sdk_codebooks_at
     * @param position The position string to validate
     * @return Validation result, or DNV_VISTA_SDK_POSITION_VALIDATION_INVALID if `codebook`/`position` is NULL
     * @note Only meaningful for the Position codebook
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_position_validation_result_t
    dnv_vista_sdk_codebook_validate_position(const dnv_vista_sdk_codebook_t* codebook, const char* position);

    /**
     * @brief Create a metadata tag with validation
     * @param codebook Handle obtained from dnv_vista_sdk_vis_codebooks
     * @param value The tag value to validate
     * @return Owned handle, must be released with dnv_vista_sdk_metadata_tag_free, or
     *         NULL if `value` is invalid or `codebook`/`value` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_metadata_tag_t* dnv_vista_sdk_codebook_create_tag(
        const dnv_vista_sdk_codebook_t* codebook, const char* value);

#ifdef __cplusplus
}
#endif
