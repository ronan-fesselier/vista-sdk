/**
 * @file local_id_query.h
 * @brief C API for dnv::vista::sdk::LocalIdQuery
 * @details `dnv_vista_sdk_local_id_query_t*` is owned by value and must be released
 *          with dnv_vista_sdk_local_id_query_free
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "dnv/vista/sdk/c/core/local_id.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_local_id_query dnv_vista_sdk_local_id_query_t;

    /**
     * @brief Release a LocalIdQuery handle
     * @param query Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_local_id_query_free(dnv_vista_sdk_local_id_query_t* query);

    /**
     * @brief Test whether a LocalId matches this query
     * @param query Handle obtained from this API
     * @param localId LocalId to test
     * @return 1 if `localId` matches the query criteria, 0 otherwise or if any argument is NULL
     * @note If `localId`'s VIS version is older than the latest, automatic version conversion is attempted
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_local_id_query_match(
        const dnv_vista_sdk_local_id_query_t* query, const dnv_vista_sdk_local_id_t* localId);

    /**
     * @brief Test whether a LocalId string matches this query
     * @param query Handle obtained from this API
     * @param localIdStr LocalId string in dnv-v2 format
     * @return 1 if the parsed LocalId matches the query criteria, 0 if parsing fails,
     *         there is no match, or any argument is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_local_id_query_match_string(
        const dnv_vista_sdk_local_id_query_t* query, const char* localIdStr);

#ifdef __cplusplus
}
#endif
