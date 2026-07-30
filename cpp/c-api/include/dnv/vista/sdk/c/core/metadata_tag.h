/**
 * @file metadata_tag.h
 * @brief C API for dnv::vista::sdk::MetadataTag
 * @details `dnv_vista_sdk_metadata_tag_t*` is owned by value and must be released
 *          with dnv_vista_sdk_metadata_tag_free
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "codebook_name.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_metadata_tag dnv_vista_sdk_metadata_tag_t;

    /**
     * @brief Release a MetadataTag handle
     * @param tag Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_metadata_tag_free(dnv_vista_sdk_metadata_tag_t* tag);

    /**
     * @brief Get the codebook name
     * @param tag Handle obtained from this API
     * @return Codebook name, or 0 if `tag` is NULL (not a valid codebook name)
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_codebook_name_t
    dnv_vista_sdk_metadata_tag_name(const dnv_vista_sdk_metadata_tag_t* tag);

    /**
     * @brief Get the tag value
     * @param tag Handle obtained from this API
     * @return Null-terminated value string, owned by `tag` - valid as long as `tag`
     *         is valid, or NULL if `tag` is NULL
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_metadata_tag_value(const dnv_vista_sdk_metadata_tag_t* tag);

    /**
     * @brief Check if this is a custom tag
     * @param tag Handle obtained from this API
     * @return 1 if custom, 0 if standard or `tag` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_metadata_tag_is_custom(const dnv_vista_sdk_metadata_tag_t* tag);

    /**
     * @brief Convert a MetadataTag to its string representation
     * @param tag Handle obtained from this API
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_string_free, or NULL if `tag` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_metadata_tag_to_string(const dnv_vista_sdk_metadata_tag_t* tag);

#ifdef __cplusplus
}
#endif
