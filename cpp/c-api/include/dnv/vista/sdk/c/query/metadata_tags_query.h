/**
 * @file metadata_tags_query.h
 * @brief C API for dnv::vista::sdk::MetadataTagsQuery
 * @details `dnv_vista_sdk_metadata_tags_query_t*` is owned by value and must be
 *          released with dnv_vista_sdk_metadata_tags_query_free
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "dnv/vista/sdk/c/core/local_id.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_metadata_tags_query dnv_vista_sdk_metadata_tags_query_t;
    typedef struct dnv_vista_sdk_metadata_tags_query_builder dnv_vista_sdk_metadata_tags_query_builder_t;

    /**
     * @brief Release a MetadataTagsQuery handle
     * @param query Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_metadata_tags_query_free(dnv_vista_sdk_metadata_tags_query_t* query);

    /**
     * @brief Test whether a LocalId matches this query
     * @param query Handle obtained from this API
     * @param localId LocalId to test
     * @return 1 if `localId` matches the query criteria, 0 otherwise or if any argument is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_metadata_tags_query_match(
        const dnv_vista_sdk_metadata_tags_query_t* query, const dnv_vista_sdk_local_id_t* localId);

    /**
     * @brief Get the builder this query was created from
     * @param query Handle obtained from this API
     * @return Borrowed pointer, valid as long as `query` is valid, or NULL if `query` is NULL
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_metadata_tags_query_builder_t* dnv_vista_sdk_metadata_tags_query_builder(
        const dnv_vista_sdk_metadata_tags_query_t* query);

#ifdef __cplusplus
}
#endif
