/**
 * @file codebook_name.h
 * @brief C API for dnv::vista::sdk::CodebookName and dnv::vista::sdk::CodebookNames
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** @brief Mirrors dnv::vista::sdk::CodebookName - same underlying values */
    typedef enum
    {
        DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY = 1,
        DNV_VISTA_SDK_CODEBOOK_NAME_CONTENT,
        DNV_VISTA_SDK_CODEBOOK_NAME_CALCULATION,
        DNV_VISTA_SDK_CODEBOOK_NAME_STATE,
        DNV_VISTA_SDK_CODEBOOK_NAME_COMMAND,
        DNV_VISTA_SDK_CODEBOOK_NAME_TYPE,
        DNV_VISTA_SDK_CODEBOOK_NAME_FUNCTIONAL_SERVICES,
        DNV_VISTA_SDK_CODEBOOK_NAME_MAINTENANCE_CATEGORY,
        DNV_VISTA_SDK_CODEBOOK_NAME_ACTIVITY_TYPE,
        DNV_VISTA_SDK_CODEBOOK_NAME_POSITION,
        DNV_VISTA_SDK_CODEBOOK_NAME_DETAIL
    } dnv_vista_sdk_codebook_name_t;

    /**
     * @brief Convert a string prefix to a codebook name
     * @param prefix Prefix string (e.g. "qty", "cnt", "calc")
     * @param outName Set to the corresponding codebook name on success
     * @return 1 on success, 0 if `prefix` is unrecognized or either argument is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_codebook_names_from_prefix(
        const char* prefix, dnv_vista_sdk_codebook_name_t* outName);

    /**
     * @brief Convert a codebook name to its string prefix
     * @param name Codebook name
     * @return Null-terminated prefix string (e.g. "qty" for Quantity), owned by the
     *         library - valid for the lifetime of the program, or NULL if `name` is invalid
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_codebook_names_to_prefix(dnv_vista_sdk_codebook_name_t name);

    /**
     * @brief Convert a codebook name to its human-readable name
     * @param name Codebook name
     * @return Null-terminated string (e.g. "Quantity"), owned by the library - valid
     *         for the lifetime of the program, or NULL if `name` is invalid
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_codebook_names_to_string(dnv_vista_sdk_codebook_name_t name);

#ifdef __cplusplus
}
#endif
