/**
 * @file gmod_individualizable_set.h
 * @brief C API for dnv::vista::sdk::GmodIndividualizableSet
 * @details `dnv_vista_sdk_gmod_individualizable_set_t*` is owned by value and must be
 *          released with dnv_vista_sdk_gmod_individualizable_set_free. Mirrors the
 *          C++ type's build-once semantics: after
 *          dnv_vista_sdk_gmod_individualizable_set_build() succeeds, further calls to
 *          this or nodes()/location() fail (matching the C++ std::runtime_error on
 *          an already-built set)
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "gmod_node.h"
#include "location.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_gmod_individualizable_set dnv_vista_sdk_gmod_individualizable_set_t;
    typedef struct dnv_vista_sdk_gmod_path dnv_vista_sdk_gmod_path_t;

    /**
     * @brief Construct an individualizable set from node indices in a path
     * @param nodeIndices Array of indices of nodes in the path that form this set
     * @param nodeIndicesCount Number of elements in `nodeIndices`
     * @param sourcePath Source path containing the nodes (copied)
     * @return Owned handle, must be released with
     *         dnv_vista_sdk_gmod_individualizable_set_free, or NULL if `nodeIndices`/
     *         `sourcePath` is NULL or set validation fails
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_individualizable_set_t* dnv_vista_sdk_gmod_individualizable_set_create(
        const int* nodeIndices, size_t nodeIndicesCount, const dnv_vista_sdk_gmod_path_t* sourcePath);

    /**
     * @brief Release an individualizable set handle
     * @param set Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_gmod_individualizable_set_free(
        dnv_vista_sdk_gmod_individualizable_set_t* set);

    /**
     * @brief Build and return the modified path (can only be called once)
     * @param set Handle obtained from this API - consumed by this call, must not be
     *            used again (still requires dnv_vista_sdk_gmod_individualizable_set_free)
     * @return Owned handle, must be released with dnv_vista_sdk_gmod_path_free, or
     *         NULL if `set` is NULL or already built
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_individualizable_set_build(
        dnv_vista_sdk_gmod_individualizable_set_t* set);

    /**
     * @brief Get the number of nodes in this set
     * @param set Handle obtained from this API
     * @return Node count, or 0 if `set` is NULL or already built
     */
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_gmod_individualizable_set_node_count(const dnv_vista_sdk_gmod_individualizable_set_t* set);

    /**
     * @brief Get a node in this set by index (a copy, distinct from the source path's nodes)
     * @param set Handle obtained from this API
     * @param index Zero-based index, must be <
     *              dnv_vista_sdk_gmod_individualizable_set_node_count(set)
     * @return Owned handle, must be released with dnv_vista_sdk_gmod_individualizable_set_node_free,
     *         or NULL if `set` is NULL, already built, or `index` is out of range
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_individualizable_set_node_at(
        const dnv_vista_sdk_gmod_individualizable_set_t* set, size_t index);

    /**
     * @brief Release a node obtained from dnv_vista_sdk_gmod_individualizable_set_node_at
     * @param node Handle obtained from that function, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_gmod_individualizable_set_node_free(dnv_vista_sdk_gmod_node_t* node);

    /**
     * @brief Get the number of node indices in this set
     * @param set Handle obtained from this API
     * @return Index count, or 0 if `set` is NULL
     */
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_gmod_individualizable_set_index_count(const dnv_vista_sdk_gmod_individualizable_set_t* set);

    /**
     * @brief Get a node index by position
     * @param set Handle obtained from this API
     * @param position Zero-based position, must be <
     *                 dnv_vista_sdk_gmod_individualizable_set_index_count(set)
     * @param outIndex Set to the node index on success
     * @return 1 on success, 0 if `set`/`outIndex` is NULL or `position` is out of range
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_individualizable_set_index_at(
        const dnv_vista_sdk_gmod_individualizable_set_t* set, size_t position, int* outIndex);

    /**
     * @brief Get the current location assigned to this set
     * @param set Handle obtained from this API
     * @return Owned handle (a copy), must be released with dnv_vista_sdk_location_free,
     *         or NULL if `set` is NULL, already built, or no location is set
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_location_t* dnv_vista_sdk_gmod_individualizable_set_location(
        const dnv_vista_sdk_gmod_individualizable_set_t* set);

    /**
     * @brief Convert set to string representation (leaf nodes separated by '/')
     * @param set Handle obtained from this API
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_gmod_individualizable_set_string_free, or NULL if `set`
     *         is NULL or already built
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_gmod_individualizable_set_to_string(
        const dnv_vista_sdk_gmod_individualizable_set_t* set);

    /**
     * @brief Release a string obtained from this API
     * @param str String obtained from dnv_vista_sdk_gmod_individualizable_set_to_string,
     *            may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_gmod_individualizable_set_string_free(char* str);

#ifdef __cplusplus
}
#endif
