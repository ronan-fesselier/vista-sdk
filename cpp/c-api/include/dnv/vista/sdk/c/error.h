/**
 * @file error.h
 * @brief Last-error reporting for the C API
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Get the message for the last error on the calling thread
     * @return Null-terminated message, empty string if no error occurred yet
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_last_error_message(void);

#ifdef __cplusplus
}
#endif
