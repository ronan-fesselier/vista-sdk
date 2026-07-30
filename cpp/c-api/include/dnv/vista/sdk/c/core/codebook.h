/**
 * @file codebook.h
 * @brief C API for dnv::vista::sdk::Codebook
 * @details `dnv_vista_sdk_codebook_t*` is a borrowed pointer, owned by the parent
 *          Codebooks - never freed independently
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "metadata_tag.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_codebook dnv_vista_sdk_codebook_t;

    /**
     * @brief Check if a value is a standard value in this codebook
     * @param codebook Handle obtained from dnv_vista_sdk_vis_codebooks
     * @param value Value to check
     * @return 1 if `value` is a standard value, 0 otherwise or if `codebook`/`value` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_codebook_has_standard_value(
        const dnv_vista_sdk_codebook_t* codebook, const char* value);

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
