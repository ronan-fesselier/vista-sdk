/**
 * @file gmod_path_query_builder.h
 * @brief C API for dnv::vista::sdk::GmodPathQueryBuilder
 * @details `dnv_vista_sdk_gmod_path_query_builder_t*` is owned by value and must be
 *          released with dnv_vista_sdk_gmod_path_query_builder_free. Every with_x
 *          function returns a new, independent handle - it does not mutate the input.
 *
 *          Mirrors the two concrete C++ variants (GmodPathQueryBuilder::Path and
 *          GmodPathQueryBuilder::Nodes) through a single opaque type, since C has no
 *          polymorphism:
 *          - dnv_vista_sdk_gmod_path_query_builder_create() produces a Nodes-variant
 *            handle - only dnv_vista_sdk_gmod_path_query_builder_with_node applies
 *          - dnv_vista_sdk_gmod_path_query_builder_from() produces a Path-variant
 *            handle - dnv_vista_sdk_gmod_path_query_builder_path,
 *            _with_any_node_before/_after and _without_locations additionally apply
 *          Calling a Path-only function on a Nodes-variant handle (or vice versa)
 *          returns NULL and sets the last error message.
 *
 *          The C++ `select` callback (choosing a node from a
 *          std::unordered_map<std::string, const GmodNode*> by code) is replaced here
 *          by a plain `const char* code` lookup, since every real usage is a lookup by
 *          node code - this avoids exposing a C function-pointer/closure callback
 *          mechanism across the ABI boundary
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "../core/gmod_node.h"
#include "../core/gmod_path.h"
#include "../core/location.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_gmod_path_query_builder dnv_vista_sdk_gmod_path_query_builder_t;
    typedef struct dnv_vista_sdk_gmod_path_query dnv_vista_sdk_gmod_path_query_t;

    /**
     * @brief Create an empty Nodes-variant builder
     * @return Owned handle, must be released with dnv_vista_sdk_gmod_path_query_builder_free
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_query_builder_t* dnv_vista_sdk_gmod_path_query_builder_create(void);

    /**
     * @brief Create a Path-variant builder from an existing GmodPath
     * @param path GmodPath to use as base
     * @return Owned handle, must be released with dnv_vista_sdk_gmod_path_query_builder_free,
     *         or NULL if `path` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_query_builder_t* dnv_vista_sdk_gmod_path_query_builder_from(
        const dnv_vista_sdk_gmod_path_t* path);

    /**
     * @brief Release a GmodPathQueryBuilder handle
     * @param builder Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_gmod_path_query_builder_free(
        dnv_vista_sdk_gmod_path_query_builder_t* builder);

    /**
     * @brief Get the base GmodPath (Path variant only)
     * @param builder Handle obtained from dnv_vista_sdk_gmod_path_query_builder_from
     * @return Borrowed pointer, valid as long as `builder` is valid, or NULL if
     *         `builder` is NULL or not a Path-variant handle
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_gmod_path_query_builder_path(
        const dnv_vista_sdk_gmod_path_query_builder_t* builder);

    /**
     * @brief Configure a node (found by code among the path's set nodes) to match all
     *        locations (Path variant only)
     * @param builder Handle obtained from dnv_vista_sdk_gmod_path_query_builder_from
     * @param code Node code to select from the path's individualizable set nodes
     * @param matchAllLocations 1 to match any location individualization, 0 to match no location
     * @return New owned handle with the node configured, or NULL if `builder`/`code`
     *         is NULL, `builder` is not a Path-variant handle, or `code` is not
     *         found among the path's set nodes
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_query_builder_t*
    dnv_vista_sdk_gmod_path_query_builder_path_with_node_all_locations(
        const dnv_vista_sdk_gmod_path_query_builder_t* builder, const char* code, int matchAllLocations);

    /**
     * @brief Configure a node (found by code among the path's set nodes) with
     *        specific locations to match (Path variant only)
     * @param builder Handle obtained from dnv_vista_sdk_gmod_path_query_builder_from
     * @param code Node code to select from the path's individualizable set nodes
     * @param locations Array of locations to match, may be NULL if `locationCount` is 0
     * @param locationCount Number of entries in `locations`
     * @return New owned handle with the node configured, or NULL if `builder`/`code`
     *         is NULL, `builder` is not a Path-variant handle, or `code` is not
     *         found among the path's set nodes
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_query_builder_t*
    dnv_vista_sdk_gmod_path_query_builder_path_with_node_locations(
        const dnv_vista_sdk_gmod_path_query_builder_t* builder,
        const char* code,
        const dnv_vista_sdk_location_t* const* locations,
        size_t locationCount);

    /**
     * @brief Ignore all nodes before the node with the given code in the path
     *        (Path variant only)
     * @param builder Handle obtained from dnv_vista_sdk_gmod_path_query_builder_from
     * @param code Node code to select from the path's full node list
     * @return New owned handle with preceding nodes ignored, or NULL if `builder`/`code`
     *         is NULL, `builder` is not a Path-variant handle, or `code` is not in the path
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_query_builder_t*
    dnv_vista_sdk_gmod_path_query_builder_with_any_node_before(
        const dnv_vista_sdk_gmod_path_query_builder_t* builder, const char* code);

    /**
     * @brief Ignore all nodes after the node with the given code in the path
     *        (Path variant only)
     * @param builder Handle obtained from dnv_vista_sdk_gmod_path_query_builder_from
     * @param code Node code to select from the path's full node list
     * @return New owned handle with following nodes ignored, or NULL if `builder`/`code`
     *         is NULL, `builder` is not a Path-variant handle, or `code` is not in the path
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_query_builder_t*
    dnv_vista_sdk_gmod_path_query_builder_with_any_node_after(
        const dnv_vista_sdk_gmod_path_query_builder_t* builder, const char* code);

    /**
     * @brief Ignore all location individualizations (Path variant only)
     * @param builder Handle obtained from dnv_vista_sdk_gmod_path_query_builder_from
     * @return New owned handle with locations ignored, or NULL if `builder` is NULL
     *         or not a Path-variant handle
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_query_builder_t*
    dnv_vista_sdk_gmod_path_query_builder_without_locations(const dnv_vista_sdk_gmod_path_query_builder_t* builder);

    /**
     * @brief Add a node to match with location settings (Nodes variant only)
     * @param builder Handle obtained from dnv_vista_sdk_gmod_path_query_builder_create
     * @param node Node to match
     * @param matchAllLocations 1 to match any location individualization, 0 to match no location
     * @return New owned handle with the node added, or NULL if `builder`/`node` is
     *         NULL or `builder` is not a Nodes-variant handle
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_query_builder_t*
    dnv_vista_sdk_gmod_path_query_builder_with_node_all_locations(
        const dnv_vista_sdk_gmod_path_query_builder_t* builder,
        const dnv_vista_sdk_gmod_node_t* node,
        int matchAllLocations);

    /**
     * @brief Add a node to match with specific locations (Nodes variant only)
     * @param builder Handle obtained from dnv_vista_sdk_gmod_path_query_builder_create
     * @param node Node to match
     * @param locations Array of locations to match, may be NULL if `locationCount` is 0
     * @param locationCount Number of entries in `locations`
     * @return New owned handle with the node added, or NULL if `builder`/`node` is
     *         NULL or `builder` is not a Nodes-variant handle
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_query_builder_t*
    dnv_vista_sdk_gmod_path_query_builder_with_node_locations(
        const dnv_vista_sdk_gmod_path_query_builder_t* builder,
        const dnv_vista_sdk_gmod_node_t* node,
        const dnv_vista_sdk_location_t* const* locations,
        size_t locationCount);

    /**
     * @brief Build the immutable query from the builder's current state
     * @param builder Handle obtained from this API
     * @return Owned handle, must be released with dnv_vista_sdk_gmod_path_query_free,
     *         or NULL if `builder` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_query_t* dnv_vista_sdk_gmod_path_query_builder_build(
        const dnv_vista_sdk_gmod_path_query_builder_t* builder);

#ifdef __cplusplus
}
#endif
