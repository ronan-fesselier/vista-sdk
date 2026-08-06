/**
 * @file local_id_query_builder.h
 * @brief C API for dnv::vista::sdk::LocalIdQueryBuilder
 * @details `dnv_vista_sdk_local_id_query_builder_t*` is owned by value and must be
 *          released with dnv_vista_sdk_local_id_query_builder_free. Every with_x
 *          function returns a new, independent handle - it does not mutate the input.
 *
 *          The C++ `configure`-callback overloads of withPrimaryItem/withSecondaryItem/
 *          withTags (taking a std::function that receives a sub-builder by value) are
 *          not exposed here - they are equivalent to building the sub-query separately
 *          via the gmod_path_query_builder and metadata_tags_query_builder functions,
 *          then passing the resulting GmodPathQuery/MetadataTagsQuery to the plain
 *          with_primary_item/with_secondary_item/with_tags functions below - this
 *          avoids exposing a C function-pointer/closure callback mechanism across the ABI
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "../core/gmod_path.h"
#include "../query/gmod_path_query.h"
#include "../query/local_id_query.h"
#include "../query/metadata_tags_query.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_local_id_query_builder dnv_vista_sdk_local_id_query_builder_t;

    /**
     * @brief Create an empty builder (matches all LocalIds)
     * @return Owned handle, must be released with dnv_vista_sdk_local_id_query_builder_free
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_create(void);

    /**
     * @brief Create a builder configured to match the given LocalId exactly
     * @param localId LocalId to build the query from
     * @return Owned handle, must be released with dnv_vista_sdk_local_id_query_builder_free,
     *         or NULL if `localId` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_from(
        const dnv_vista_sdk_local_id_t* localId);

    /**
     * @brief Create a builder configured to match the given LocalId string exactly
     * @param localIdStr LocalId string in dnv-v2 format
     * @return Owned handle, must be released with dnv_vista_sdk_local_id_query_builder_free,
     *         or NULL if `localIdStr` is NULL or invalid
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_from_string(
        const char* localIdStr);

    /**
     * @brief Release a LocalIdQueryBuilder handle
     * @param builder Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_local_id_query_builder_free(dnv_vista_sdk_local_id_query_builder_t* builder);

    /**
     * @brief Set the primary item query from a GmodPath (exact match)
     * @param builder Handle obtained from this API
     * @param primaryItem GmodPath to match
     * @return New owned handle with the primary item set, or NULL if `builder`/`primaryItem` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_with_primary_item(
        const dnv_vista_sdk_local_id_query_builder_t* builder, const dnv_vista_sdk_gmod_path_t* primaryItem);

    /**
     * @brief Set the primary item query from an existing GmodPathQuery
     * @param builder Handle obtained from this API
     * @param primaryItem GmodPathQuery to use
     * @return New owned handle with the primary item set, or NULL if `builder`/`primaryItem` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_query_builder_t*
    dnv_vista_sdk_local_id_query_builder_with_primary_item_query(
        const dnv_vista_sdk_local_id_query_builder_t* builder, const dnv_vista_sdk_gmod_path_query_t* primaryItem);

    /**
     * @brief Set the secondary item query from a GmodPath (exact match)
     * @param builder Handle obtained from this API
     * @param secondaryItem GmodPath to match
     * @return New owned handle with the secondary item set, or NULL if `builder`/`secondaryItem` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_query_builder_t*
    dnv_vista_sdk_local_id_query_builder_with_secondary_item(
        const dnv_vista_sdk_local_id_query_builder_t* builder, const dnv_vista_sdk_gmod_path_t* secondaryItem);

    /**
     * @brief Set the secondary item query from an existing GmodPathQuery
     * @param builder Handle obtained from this API
     * @param secondaryItem GmodPathQuery to use
     * @return New owned handle with the secondary item set, or NULL if `builder`/`secondaryItem` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_query_builder_t*
    dnv_vista_sdk_local_id_query_builder_with_secondary_item_query(
        const dnv_vista_sdk_local_id_query_builder_t* builder, const dnv_vista_sdk_gmod_path_query_t* secondaryItem);

    /**
     * @brief Match any LocalId regardless of secondary item presence
     * @param builder Handle obtained from this API
     * @return New owned handle with the secondary requirement cleared, or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_query_builder_t*
    dnv_vista_sdk_local_id_query_builder_with_any_secondary_item(const dnv_vista_sdk_local_id_query_builder_t* builder);

    /**
     * @brief Match only LocalIds without a secondary item
     * @param builder Handle obtained from this API
     * @return New owned handle with the secondary requirement set to "must not have",
     *         or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_query_builder_t*
    dnv_vista_sdk_local_id_query_builder_without_secondary_item(const dnv_vista_sdk_local_id_query_builder_t* builder);

    /**
     * @brief Set the metadata tags query
     * @param builder Handle obtained from this API
     * @param tags MetadataTagsQuery to use
     * @return New owned handle with the tags set, or NULL if `builder`/`tags` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_with_tags(
        const dnv_vista_sdk_local_id_query_builder_t* builder, const dnv_vista_sdk_metadata_tags_query_t* tags);

    /**
     * @brief Remove all location requirements from primary and secondary items
     * @details Only affects items configured as a Path-variant GmodPathQuery (not Nodes)
     * @param builder Handle obtained from this API
     * @return New owned handle with locations cleared, or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_query_builder_t* dnv_vista_sdk_local_id_query_builder_without_locations(
        const dnv_vista_sdk_local_id_query_builder_t* builder);

    /**
     * @brief Get the primary item path, if configured as a Path-variant query
     * @param builder Handle obtained from this API
     * @return Borrowed pointer, valid as long as `builder` is valid, or NULL if
     *         `builder` is NULL, unset, or configured as a Nodes-variant query
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_query_builder_primary_item(
        const dnv_vista_sdk_local_id_query_builder_t* builder);

    /**
     * @brief Get the secondary item path, if configured as a Path-variant query
     * @param builder Handle obtained from this API
     * @return Borrowed pointer, valid as long as `builder` is valid, or NULL if
     *         `builder` is NULL, unset, or configured as a Nodes-variant query
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_query_builder_secondary_item(
        const dnv_vista_sdk_local_id_query_builder_t* builder);

    /**
     * @brief Build the immutable query from the builder's current state
     * @param builder Handle obtained from this API
     * @return Owned handle, must be released with dnv_vista_sdk_local_id_query_free,
     *         or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_query_t* dnv_vista_sdk_local_id_query_builder_build(
        const dnv_vista_sdk_local_id_query_builder_t* builder);

#ifdef __cplusplus
}
#endif
