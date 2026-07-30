/**
 * @file common.h
 * @brief Common utilities shared across all C API modules
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Release a string returned by any C API function
     * @param str Pointer returned by a C API function, or NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_string_free(char* str);

#ifdef __cplusplus
}
#endif
