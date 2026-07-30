/**
 * @file codebooks.h
 * @brief C API for dnv::vista::sdk::Codebooks
 * @details `dnv_vista_sdk_codebooks_t*` is a borrowed pointer into the VIS singleton's
 *          cache - never freed, same lifetime rules as Gmod
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "codebook.h"
#include "codebook_name.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_codebooks dnv_vista_sdk_codebooks_t;

    /**
     * @brief Get the VIS version for these codebooks
     * @param codebooks Handle obtained from dnv_vista_sdk_vis_codebooks
     * @return Null-terminated version string (e.g. "3-4a"), owned by the VIS singleton,
     *         valid for the lifetime of the program, or NULL if `codebooks` is NULL
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_codebooks_version(const dnv_vista_sdk_codebooks_t* codebooks);

    /**
     * @brief Get a codebook by name
     * @param codebooks Handle obtained from dnv_vista_sdk_vis_codebooks
     * @param name Codebook name
     * @return Borrowed pointer, valid as long as `codebooks` is valid, or NULL if
     *         `codebooks` is NULL or `name` is invalid
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_codebook_t* dnv_vista_sdk_codebooks_at(
        const dnv_vista_sdk_codebooks_t* codebooks, dnv_vista_sdk_codebook_name_t name);

#ifdef __cplusplus
}
#endif
