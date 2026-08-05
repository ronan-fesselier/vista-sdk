/**
 * @file metadata_tags_query_builder.h
 * @brief C API for dnv::vista::sdk::MetadataTagsQueryBuilder
 * @details `dnv_vista_sdk_metadata_tags_query_builder_t*` is owned by value and must
 *          be released with dnv_vista_sdk_metadata_tags_query_builder_free. Every
 *          with_x function returns a new, independent handle - it does not mutate
 *          the input
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "dnv/vista/sdk/c/core/local_id.h"
#include "dnv/vista/sdk/c/core/metadata_tag.h"

#include "metadata_tags_query.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_metadata_tags_query_builder dnv_vista_sdk_metadata_tags_query_builder_t;

    /**
     * @brief Create an empty builder (matches any LocalId by default)
     * @return Owned handle, must be released with dnv_vista_sdk_metadata_tags_query_builder_free
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_metadata_tags_query_builder_t* dnv_vista_sdk_metadata_tags_query_builder_create(
        void);

    /**
     * @brief Create a builder from an existing LocalId's tags
     * @param localId LocalId to copy tags from
     * @param allowOtherTags 1 to allow additional tags (subset match), 0 for exact match
     * @return Owned handle, must be released with dnv_vista_sdk_metadata_tags_query_builder_free,
     *         or NULL if `localId` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_metadata_tags_query_builder_t* dnv_vista_sdk_metadata_tags_query_builder_from(
        const dnv_vista_sdk_local_id_t* localId, int allowOtherTags);

    /**
     * @brief Release a MetadataTagsQueryBuilder handle
     * @param builder Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_metadata_tags_query_builder_free(
        dnv_vista_sdk_metadata_tags_query_builder_t* builder);

    /**
     * @brief Add a metadata tag to match, by codebook name and value
     * @param builder Handle obtained from this API
     * @param name Codebook name
     * @param value Tag value
     * @return New owned handle with the tag added (replaces any existing tag for
     *         this codebook), or NULL if `builder`/`value` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_metadata_tags_query_builder_t* dnv_vista_sdk_metadata_tags_query_builder_with_tag(
        const dnv_vista_sdk_metadata_tags_query_builder_t* builder,
        dnv_vista_sdk_codebook_name_t name,
        const char* value);

    /**
     * @brief Add a metadata tag to match, from an existing MetadataTag
     * @param builder Handle obtained from this API
     * @param tag MetadataTag to match
     * @return New owned handle with the tag added (replaces any existing tag for
     *         this codebook), or NULL if `builder`/`tag` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_metadata_tags_query_builder_t*
    dnv_vista_sdk_metadata_tags_query_builder_with_metadata_tag(
        const dnv_vista_sdk_metadata_tags_query_builder_t* builder, const dnv_vista_sdk_metadata_tag_t* tag);

    /**
     * @brief Set whether tags not specified in the query are allowed
     * @param builder Handle obtained from this API
     * @param allowOthers 1 (default) for subset matching (extra tags allowed), 0 for exact matching
     * @return New owned handle with the setting updated, or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_metadata_tags_query_builder_t*
    dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags(
        const dnv_vista_sdk_metadata_tags_query_builder_t* builder, int allowOthers);

    /**
     * @brief Build the immutable query from the builder's current state
     * @param builder Handle obtained from this API
     * @return Owned handle, must be released with dnv_vista_sdk_metadata_tags_query_free,
     *         or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_metadata_tags_query_t* dnv_vista_sdk_metadata_tags_query_builder_build(
        const dnv_vista_sdk_metadata_tags_query_builder_t* builder);

#ifdef __cplusplus
}
#endif
