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
     * @brief Result returned by a traverse handler to control traversal flow
     */
    typedef enum
    {
        DNV_VISTA_SDK_TRAVERSAL_STOP,         ///< Stop traversal immediately
        DNV_VISTA_SDK_TRAVERSAL_SKIP_SUBTREE, ///< Skip children of current node
        DNV_VISTA_SDK_TRAVERSAL_CONTINUE,     ///< Continue normal traversal
    } dnv_vista_sdk_traversal_result_t;

    /**
     * @brief Callback type for Gmod tree traversal
     * @param parents Array of borrowed node pointers from root to current node (not including current)
     * @param parent_count Number of entries in `parents`
     * @param node Current node being visited (borrowed, same lifetime as `gmod`)
     * @param userdata Caller-supplied context pointer, passed through unchanged
     * @return Traversal control result
     */
    typedef dnv_vista_sdk_traversal_result_t (*dnv_vista_sdk_traverse_handler_t)(
        const dnv_vista_sdk_gmod_node_t* const* parents,
        size_t parent_count,
        const dnv_vista_sdk_gmod_node_t* node,
        void* userdata);

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

    /**
     * @brief Traverse the Gmod tree depth-first from the root node
     * @param gmod Handle obtained from dnv_vista_sdk_vis_gmod
     * @param handler Callback invoked for each node; must not be NULL
     * @param max_traversal_occurrence Maximum times a node may appear in the current path (1 = default)
     * @param userdata Caller-supplied pointer forwarded to every handler call (may be NULL)
     * @return 1 if traversal completed, 0 if stopped early or if `gmod` / `handler` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_traverse(
        const dnv_vista_sdk_gmod_t* gmod,
        dnv_vista_sdk_traverse_handler_t handler,
        int max_traversal_occurrence,
        void* userdata);

#ifdef __cplusplus
}
#endif
