/**
 * @file gmod.h
 * @brief C API for dnv::vista::sdk::Gmod
 * @details `dnv_vista_sdk_gmod_t*` is a borrowed pointer into the VIS singleton's
 *          cache - never freed, same lifetime rules as Codebooks/Locations
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "gmod_node.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_gmod dnv_vista_sdk_gmod_t;

    /**
     * @brief Get the VIS version string for a Gmod instance
     * @param gmod Handle obtained from dnv_vista_sdk_vis_gmod
     * @return Null-terminated version string, owned by the library - valid for the
     *         lifetime of the program, or NULL if `gmod` is NULL
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_gmod_version(const dnv_vista_sdk_gmod_t* gmod);

    /**
     * @brief Get the root node of the tree (code "VE")
     * @param gmod Handle obtained from dnv_vista_sdk_vis_gmod
     * @return Borrowed pointer, valid as long as `gmod` is valid, or NULL if `gmod` is NULL
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_root_node(const dnv_vista_sdk_gmod_t* gmod);

    /**
     * @brief Look up a node by code
     * @param gmod Handle obtained from dnv_vista_sdk_vis_gmod
     * @param code Node code (e.g. "411.1", "VE")
     * @return Borrowed pointer, valid as long as `gmod` is valid, or NULL if `gmod` is
     *         NULL or no node has this code
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_get_node(
        const dnv_vista_sdk_gmod_t* gmod, const char* code);

    /**
     * @brief Get the total number of nodes in this Gmod
     * @param gmod Handle obtained from dnv_vista_sdk_vis_gmod
     * @return Node count, or 0 if `gmod` is NULL
     */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_gmod_node_count(const dnv_vista_sdk_gmod_t* gmod);

    /**
     * @brief Get a node by iteration index (unspecified order - hash map based)
     * @param gmod Handle obtained from dnv_vista_sdk_vis_gmod
     * @param index Zero-based index, must be < dnv_vista_sdk_gmod_node_count(gmod)
     * @return Borrowed pointer, valid as long as `gmod` is valid, or NULL if `gmod` is
     *         NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_node_at(
        const dnv_vista_sdk_gmod_t* gmod, size_t index);

#ifdef __cplusplus
}
#endif
