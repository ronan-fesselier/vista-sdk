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
     * @brief Get the Codebooks for a VIS version
     * @param vis Instance obtained from dnv_vista_sdk_vis_instance
     * @param visVersion VIS version string (e.g. "3-11a")
     * @return Borrowed pointer valid for the program's lifetime, or NULL if `vis` is
     *         NULL or `visVersion` is not a recognized VIS version
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_codebooks_t* dnv_vista_sdk_vis_codebooks(
        const dnv_vista_sdk_vis_t* vis, const char* visVersion);

#ifdef __cplusplus
}
#endif
