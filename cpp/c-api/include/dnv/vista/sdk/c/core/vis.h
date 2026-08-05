/**
 * @file vis.h
 * @brief C API for dnv::vista::sdk::VIS
 * @details `dnv_vista_sdk_vis_t*` is a borrowed pointer to the singleton - never freed
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_codebooks dnv_vista_sdk_codebooks_t;
    typedef struct dnv_vista_sdk_gmod dnv_vista_sdk_gmod_t;
    typedef struct dnv_vista_sdk_gmod_node dnv_vista_sdk_gmod_node_t;
    typedef struct dnv_vista_sdk_gmod_path dnv_vista_sdk_gmod_path_t;
    typedef struct dnv_vista_sdk_local_id dnv_vista_sdk_local_id_t;
    typedef struct dnv_vista_sdk_local_id_builder dnv_vista_sdk_local_id_builder_t;
    typedef struct dnv_vista_sdk_locations dnv_vista_sdk_locations_t;
    typedef struct dnv_vista_sdk_vis dnv_vista_sdk_vis_t;

    /**
     * @brief Get the VIS singleton instance
     * @return Borrowed pointer, valid for the lifetime of the program
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_vis_t* dnv_vista_sdk_vis_instance(void);

    /**
     * @brief Get the number of available VIS versions
     * @param vis Instance obtained from dnv_vista_sdk_vis_instance
     * @return Version count, or 0 if `vis` is NULL
     */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_vis_version_count(const dnv_vista_sdk_vis_t* vis);

    /**
     * @brief Get a VIS version string by index
     * @param vis Instance obtained from dnv_vista_sdk_vis_instance
     * @param index Zero-based index, must be < dnv_vista_sdk_vis_version_count(vis)
     * @return Null-terminated version string, owned by the library - valid for the
     *         lifetime of the program, or NULL if `vis` is NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_vis_version_at(const dnv_vista_sdk_vis_t* vis, size_t index);

    /**
     * @brief Get the latest VIS version
     * @param vis Instance obtained from dnv_vista_sdk_vis_instance
     * @return Null-terminated version string (e.g. "3-11a"), owned by the library -
     *         valid for the lifetime of the program, or NULL if `vis` is NULL
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_vis_latest(const dnv_vista_sdk_vis_t* vis);

    /**
     * @brief Get the Gmod for a VIS version
     * @param vis Instance obtained from dnv_vista_sdk_vis_instance
     * @param visVersion VIS version string (e.g. "3-11a")
     * @return Borrowed pointer valid for the program's lifetime, or NULL if `vis` is
     *         NULL or `visVersion` is not a recognized VIS version
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_gmod_t* dnv_vista_sdk_vis_gmod(
        const dnv_vista_sdk_vis_t* vis, const char* visVersion);

    /**
     * @brief Get the Codebooks for a VIS version
     * @param vis Instance obtained from dnv_vista_sdk_vis_instance
     * @param visVersion VIS version string (e.g. "3-11a")
     * @return Borrowed pointer valid for the program's lifetime, or NULL if `vis` is
     *         NULL or `visVersion` is not a recognized VIS version
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_codebooks_t* dnv_vista_sdk_vis_codebooks(
        const dnv_vista_sdk_vis_t* vis, const char* visVersion);

    /**
     * @brief Get the Locations for a VIS version
     * @param vis Instance obtained from dnv_vista_sdk_vis_instance
     * @param visVersion VIS version string (e.g. "3-11a")
     * @return Borrowed pointer valid for the program's lifetime, or NULL if `vis` is
     *         NULL or `visVersion` is not a recognized VIS version
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_locations_t* dnv_vista_sdk_vis_locations(
        const dnv_vista_sdk_vis_t* vis, const char* visVersion);

    /**
     * @brief Convert a Gmod node from one VIS version to another
     * @param vis Instance obtained from dnv_vista_sdk_vis_instance
     * @param sourceVersion VIS version of `sourceNode`
     * @param sourceNode Node to convert
     * @param targetVersion Target VIS version
     * @return Owned handle, must be released with dnv_vista_sdk_gmod_node_free, or
     *         NULL if any argument is NULL/invalid or the conversion failed
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_vis_convert_node(
        const dnv_vista_sdk_vis_t* vis,
        const char* sourceVersion,
        const dnv_vista_sdk_gmod_node_t* sourceNode,
        const char* targetVersion);

    /**
     * @brief Convert a Gmod path from one VIS version to another
     * @param vis Instance obtained from dnv_vista_sdk_vis_instance
     * @param sourceVersion VIS version of `sourcePath`
     * @param sourcePath Path to convert
     * @param targetVersion Target VIS version
     * @return Owned handle, must be released with dnv_vista_sdk_gmod_path_free, or
     *         NULL if any argument is NULL/invalid or the conversion failed
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_vis_convert_path(
        const dnv_vista_sdk_vis_t* vis,
        const char* sourceVersion,
        const dnv_vista_sdk_gmod_path_t* sourcePath,
        const char* targetVersion);

    /**
     * @brief Convert a LocalIdBuilder from one VIS version to another
     * @param vis Instance obtained from dnv_vista_sdk_vis_instance
     * @param sourceLocalId Builder to convert (its VIS version is used as the source version)
     * @param targetVersion Target VIS version
     * @return Owned handle, must be released with dnv_vista_sdk_local_id_builder_free, or
     *         NULL if any argument is NULL/invalid or the conversion failed
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_vis_convert_local_id_builder(
        const dnv_vista_sdk_vis_t* vis,
        const dnv_vista_sdk_local_id_builder_t* sourceLocalId,
        const char* targetVersion);

    /**
     * @brief Convert a LocalId from one VIS version to another
     * @param vis Instance obtained from dnv_vista_sdk_vis_instance
     * @param sourceLocalId LocalId to convert (its VIS version is used as the source version)
     * @param targetVersion Target VIS version
     * @return Owned handle, must be released with dnv_vista_sdk_local_id_free, or
     *         NULL if any argument is NULL/invalid or the conversion failed
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_local_id_t* dnv_vista_sdk_vis_convert_local_id(
        const dnv_vista_sdk_vis_t* vis, const dnv_vista_sdk_local_id_t* sourceLocalId, const char* targetVersion);

#ifdef __cplusplus
}
#endif
