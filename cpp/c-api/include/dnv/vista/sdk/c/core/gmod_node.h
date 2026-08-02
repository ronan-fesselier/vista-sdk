/**
 * @file gmod_node.h
 * @brief C API for dnv::vista::sdk::GmodNode
 * @details `dnv_vista_sdk_gmod_node_t*` is a borrowed pointer, owned by the parent
 *          Gmod - never freed independently
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "gmod_node_metadata.h"
#include "location.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_gmod_node dnv_vista_sdk_gmod_node_t;

    /**
     * @brief Get the VIS version string for a node
     * @param node Handle obtained from this API
     * @return Null-terminated version string, owned by the library - valid for the
     *         lifetime of the program, or NULL if `node` is NULL
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_gmod_node_version(const dnv_vista_sdk_gmod_node_t* node);

    /**
     * @brief Get the node code
     * @param node Handle obtained from this API
     * @return Null-terminated code string, owned by `node` - valid as long as `node`
     *         is valid, or NULL if `node` is NULL
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_gmod_node_code(const dnv_vista_sdk_gmod_node_t* node);

    /**
     * @brief Get the node's optional location
     * @param node Handle obtained from this API
     * @return Borrowed pointer, valid as long as `node` is valid, or NULL if `node`
     *         is NULL or has no location set
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_location_t* dnv_vista_sdk_gmod_node_location(
        const dnv_vista_sdk_gmod_node_t* node);

    /**
     * @brief Get the node's metadata
     * @param node Handle obtained from this API
     * @return Borrowed pointer, valid as long as `node` is valid, or NULL if `node` is NULL
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_node_metadata_t* dnv_vista_sdk_gmod_node_metadata(
        const dnv_vista_sdk_gmod_node_t* node);

    /**
     * @brief Get the number of children
     * @param node Handle obtained from this API
     * @return Child count, or 0 if `node` is NULL
     */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_gmod_node_child_count(const dnv_vista_sdk_gmod_node_t* node);

    /**
     * @brief Get a child node by index
     * @param node Handle obtained from this API
     * @param index Zero-based index, must be < dnv_vista_sdk_gmod_node_child_count(node)
     * @return Borrowed pointer, valid as long as the owning Gmod is valid, or NULL if
     *         `node` is NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_node_child_at(
        const dnv_vista_sdk_gmod_node_t* node, size_t index);

    /**
     * @brief Get the number of parents
     * @param node Handle obtained from this API
     * @return Parent count, or 0 if `node` is NULL
     */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_gmod_node_parent_count(const dnv_vista_sdk_gmod_node_t* node);

    /**
     * @brief Get a parent node by index
     * @param node Handle obtained from this API
     * @param index Zero-based index, must be < dnv_vista_sdk_gmod_node_parent_count(node)
     * @return Borrowed pointer, valid as long as the owning Gmod is valid, or NULL if
     *         `node` is NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_node_parent_at(
        const dnv_vista_sdk_gmod_node_t* node, size_t index);

    /**
     * @brief Get the product type child if this is a function node
     * @param node Handle obtained from this API
     * @return Borrowed pointer, valid as long as the owning Gmod is valid, or NULL if
     *         `node` is NULL or not applicable
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_node_product_type(
        const dnv_vista_sdk_gmod_node_t* node);

    /**
     * @brief Get the product selection child if this is a function node
     * @param node Handle obtained from this API
     * @return Borrowed pointer, valid as long as the owning Gmod is valid, or NULL if
     *         `node` is NULL or not applicable
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_node_product_selection(
        const dnv_vista_sdk_gmod_node_t* node);

    /** @brief True if category is ASSET/PRODUCT FUNCTION and type is COMPOSITION, 0 if `node` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_node_is_function_composition(const dnv_vista_sdk_gmod_node_t* node);

    /** @brief True if the node can be used in mapping, 0 if `node` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_node_is_mappable(const dnv_vista_sdk_gmod_node_t* node);

    /** @brief True if category is PRODUCT and type is SELECTION, 0 if `node` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_node_is_product_selection(const dnv_vista_sdk_gmod_node_t* node);

    /** @brief True if category is PRODUCT and type is TYPE, 0 if `node` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_node_is_product_type(const dnv_vista_sdk_gmod_node_t* node);

    /** @brief True if category is ASSET, 0 if `node` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_node_is_asset(const dnv_vista_sdk_gmod_node_t* node);

    /** @brief True if ASSET FUNCTION LEAF or PRODUCT FUNCTION LEAF, 0 if `node` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_node_is_leaf_node(const dnv_vista_sdk_gmod_node_t* node);

    /** @brief True if category is not PRODUCT or ASSET, 0 if `node` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_node_is_function_node(const dnv_vista_sdk_gmod_node_t* node);

    /** @brief True if category is ASSET FUNCTION, 0 if `node` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_node_is_asset_function_node(const dnv_vista_sdk_gmod_node_t* node);

    /** @brief True if code is "VE", 0 if `node` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_node_is_root(const dnv_vista_sdk_gmod_node_t* node);

    /**
     * @brief Check if another node is a child of this node
     * @param node Handle obtained from this API
     * @param other Handle to compare against
     * @return 1 if `other` is a child of `node`, 0 otherwise or if either is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_node_is_child(
        const dnv_vista_sdk_gmod_node_t* node, const dnv_vista_sdk_gmod_node_t* other);

    /**
     * @brief Check if a node code is a child of this node
     * @param node Handle obtained from this API
     * @param code Node code to check
     * @return 1 if a child has this code, 0 otherwise or if `node`/`code` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_gmod_node_is_child_code(
        const dnv_vista_sdk_gmod_node_t* node, const char* code);

    /**
     * @brief Convert to string representation ("Code" or "Code-Location")
     * @param node Handle obtained from this API
     * @return Owned, null-terminated string, must be released with
     *         dnv_vista_sdk_gmod_node_string_free, or NULL if `node` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_gmod_node_to_string(const dnv_vista_sdk_gmod_node_t* node);

    /**
     * @brief Release a string obtained from this API
     * @param str String obtained from dnv_vista_sdk_gmod_node_to_string, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_gmod_node_string_free(char* str);

#ifdef __cplusplus
}
#endif
