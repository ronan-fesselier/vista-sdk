/**
 * @file gmod_path.h
 * @brief C API for dnv::vista::sdk::GmodPath
 * @details `dnv_vista_sdk_gmod_path_t*` is owned by value and must be released with
 *          dnv_vista_sdk_gmod_path_free. FullPathRange/Iterator are not exposed - use
 *          length()/at() index-based access, or to_full_path_string() for the same
 *          traversal result already formatted
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "gmod.h"
#include "gmod_individualizable_set.h"
#include "gmod_node.h"
#include "locations.h"
#include "parsing_errors.h"
#include "vis.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_gmod_path dnv_vista_sdk_gmod_path_t;

    /**
     * @brief Release a GmodPath handle
     * @param path Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_gmod_path_free(dnv_vista_sdk_gmod_path_t* path);

    /**
     * @brief Parse a short path string, resolving Gmod/Locations from a VIS version
     * @param item Short path string (e.g. "411.1/C101.72/I101")
     * @param visVersion VIS version string (e.g. "3-11a")
     * @return Owned handle, must be released with dnv_vista_sdk_gmod_path_free, or
     *         NULL if `item`/`visVersion` is invalid or NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_path_from_short_path_version(
        const char* item, const char* visVersion);

    /**
     * @brief Parse a short path string against an explicit Gmod/Locations
     * @param item Short path string
     * @param gmod Handle obtained from dnv_vista_sdk_vis_gmod
     * @param locations Handle obtained from dnv_vista_sdk_vis_locations
     * @return Owned handle, must be released with dnv_vista_sdk_gmod_path_free, or
     *         NULL if `item` is invalid or any argument is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_path_from_short_path(
        const char* item, const dnv_vista_sdk_gmod_t* gmod, const dnv_vista_sdk_locations_t* locations);

    /**
     * @brief Parse a short path string, collecting detailed error information on failure
     * @param item Short path string
     * @param gmod Handle obtained from dnv_vista_sdk_vis_gmod
     * @param locations Handle obtained from dnv_vista_sdk_vis_locations
     * @param outErrors Set to an owned ParsingErrors handle (must be released with
     *                  dnv_vista_sdk_parsing_errors_free), even on success (empty in
     *                  that case). Left untouched if any argument is NULL
     * @return Owned handle, must be released with dnv_vista_sdk_gmod_path_free, or
     *         NULL if `item` is invalid or any argument is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_path_from_short_path_with_errors(
        const char* item,
        const dnv_vista_sdk_gmod_t* gmod,
        const dnv_vista_sdk_locations_t* locations,
        dnv_vista_sdk_parsing_errors_t** outErrors);

    /**
     * @brief Parse a full path string (starting with root, e.g. "VE/400a/410/411/411i/411.1")
     * @param fullPathStr Full path string
     * @param gmod Handle obtained from dnv_vista_sdk_vis_gmod
     * @param locations Handle obtained from dnv_vista_sdk_vis_locations
     * @return Owned handle, must be released with dnv_vista_sdk_gmod_path_free, or
     *         NULL if `fullPathStr` is invalid or any argument is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_path_from_full_path(
        const char* fullPathStr, const dnv_vista_sdk_gmod_t* gmod, const dnv_vista_sdk_locations_t* locations);

    /**
     * @brief Parse a full path string, collecting detailed error information on failure
     * @param fullPathStr Full path string
     * @param gmod Handle obtained from dnv_vista_sdk_vis_gmod
     * @param locations Handle obtained from dnv_vista_sdk_vis_locations
     * @param outErrors Set to an owned ParsingErrors handle (must be released with
     *                  dnv_vista_sdk_parsing_errors_free), even on success (empty in
     *                  that case). Left untouched if any argument is NULL
     * @return Owned handle, must be released with dnv_vista_sdk_gmod_path_free, or
     *         NULL if `fullPathStr` is invalid or any argument is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_path_from_full_path_with_errors(
        const char* fullPathStr,
        const dnv_vista_sdk_gmod_t* gmod,
        const dnv_vista_sdk_locations_t* locations,
        dnv_vista_sdk_parsing_errors_t** outErrors);

    /**
     * @brief Get the VIS version string for a path
     * @param path Handle obtained from this API
     * @return Null-terminated version string, owned by the library - valid for the
     *         lifetime of the program, or NULL if `path` is NULL
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_gmod_path_version(const dnv_vista_sdk_gmod_path_t* path);

    /**
     * @brief Get the target node at the end of the path
     * @param path Handle obtained from this API
     * @return Borrowed pointer, valid as long as `path` is valid, or NULL if `path` is NULL
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_path_node(
        const dnv_vista_sdk_gmod_path_t* path);

    /**
     * @brief Get the total number of nodes in the path (parents + target node)
     * @param path Handle obtained from this API
     * @return Node count, or 0 if `path` is NULL
     */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_gmod_path_length(const dnv_vista_sdk_gmod_path_t* path);

    /**
     * @brief Access the node at a given depth (0 = root, length-1 = target node)
     * @param path Handle obtained from this API
     * @param index Zero-based depth, must be < dnv_vista_sdk_gmod_path_length(path)
     * @return Borrowed pointer, valid as long as `path` is valid, or NULL if `path`
     *         is NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_path_at(
        const dnv_vista_sdk_gmod_path_t* path, size_t index);

    /** @brief True if the target node is mappable to equipment, 0 if `path` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_path_is_mappable(const dnv_vista_sdk_gmod_path_t* path);

    /** @brief True if the path contains any individualizable nodes, 0 if `path` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_path_is_individualizable(const dnv_vista_sdk_gmod_path_t* path);

    /**
     * @brief Create a copy of this path with all locations removed
     * @param path Handle obtained from this API
     * @return Owned handle, must be released with dnv_vista_sdk_gmod_path_free, or
     *         NULL if `path` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_path_without_locations(
        const dnv_vista_sdk_gmod_path_t* path);

    /**
     * @brief Get the normal assignment name for a node at a given depth
     * @param path Handle obtained from this API
     * @param nodeDepth Zero-based depth to query
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_string_free, or NULL if `path` is NULL or not
     *         found at this depth
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_gmod_path_normal_assignment_name(
        const dnv_vista_sdk_gmod_path_t* path, size_t nodeDepth);

    /**
     * @brief Get the number of individualizable sets in this path
     * @param path Handle obtained from this API
     * @return Set count, or 0 if `path` is NULL
     */
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_gmod_path_individualizable_set_count(const dnv_vista_sdk_gmod_path_t* path);

    /**
     * @brief Get an individualizable set by index
     * @param path Handle obtained from this API
     * @param index Zero-based index, must be < dnv_vista_sdk_gmod_path_individualizable_set_count(path)
     * @return Owned handle, must be released with dnv_vista_sdk_gmod_individualizable_set_free,
     *         or NULL if `path` is NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_individualizable_set_t* dnv_vista_sdk_gmod_path_individualizable_set_at(
        const dnv_vista_sdk_gmod_path_t* path, size_t index);

    /**
     * @brief Get the number of function nodes with a common name in this path
     * @param path Handle obtained from this API
     * @return Entry count, or 0 if `path` is NULL
     */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_gmod_path_common_name_count(const dnv_vista_sdk_gmod_path_t* path);

    /**
     * @brief Get the depth of a common name entry by index
     * @param path Handle obtained from this API
     * @param index Zero-based index, must be < dnv_vista_sdk_gmod_path_common_name_count(path)
     * @param outDepth Set to the node depth on success
     * @return 1 on success, 0 if `path`/`outDepth` is NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_path_common_name_depth_at(
        const dnv_vista_sdk_gmod_path_t* path, size_t index, size_t* outDepth);

    /**
     * @brief Get the name of a common name entry by index
     * @param path Handle obtained from this API
     * @param index Zero-based index, must be < dnv_vista_sdk_gmod_path_common_name_count(path)
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_string_free, or NULL if `path` is NULL or
     *         `index` is out of range
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_gmod_path_common_name_at(
        const dnv_vista_sdk_gmod_path_t* path, size_t index);

    /**
     * @brief Convert path to short string representation (leaf nodes only, '/'-separated)
     * @param path Handle obtained from this API
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_string_free, or NULL if `path` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_gmod_path_to_string(const dnv_vista_sdk_gmod_path_t* path);

    /**
     * @brief Convert path to full string representation (all nodes, '/'-separated)
     * @param path Handle obtained from this API
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_string_free, or NULL if `path` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_gmod_path_to_full_path_string(const dnv_vista_sdk_gmod_path_t* path);

    /**
     * @brief Convert path to diagnostic dump string (codes, names, common names, normal assignments)
     * @param path Handle obtained from this API
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_string_free, or NULL if `path` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_gmod_path_to_string_dump(const dnv_vista_sdk_gmod_path_t* path);

#ifdef __cplusplus
}
#endif
