/**
 * @file gmod_path_query.h
 * @brief C API for dnv::vista::sdk::GmodPathQuery
 * @details `dnv_vista_sdk_gmod_path_query_t*` is owned by value and must be released
 *          with dnv_vista_sdk_gmod_path_query_free
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "dnv/vista/sdk/c/core/gmod_path.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_gmod_path_query dnv_vista_sdk_gmod_path_query_t;

    /**
     * @brief Release a GmodPathQuery handle
     * @param query Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_gmod_path_query_free(dnv_vista_sdk_gmod_path_query_t* query);

    /**
     * @brief Test whether a GmodPath matches this query
     * @param query Handle obtained from this API
     * @param path GmodPath to test
     * @return 1 if `path` matches the query criteria, 0 otherwise or if any argument is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_path_query_match(
        const dnv_vista_sdk_gmod_path_query_t* query, const dnv_vista_sdk_gmod_path_t* path);

#ifdef __cplusplus
}
#endif
