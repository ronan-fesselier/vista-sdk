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

    /** @brief Mirrors the C++ exception hierarchy, maps each catch branch to a distinct code */
    typedef enum
    {
        DNV_VISTA_SDK_ERROR_NONE = 0,
        DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT,
        DNV_VISTA_SDK_ERROR_OUT_OF_RANGE,
        DNV_VISTA_SDK_ERROR_DOMAIN,
        DNV_VISTA_SDK_ERROR_OVERFLOW,
        DNV_VISTA_SDK_ERROR_RUNTIME
    } dnv_vista_sdk_error_kind_t;

    /**
     * @brief Get the message for the last error on the calling thread
     * @return Null-terminated message, empty string if no error occurred yet
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_last_error_message(void);

    /**
     * @brief Get the kind code for the last error on the calling thread
     * @return Error kind, DNV_VISTA_SDK_ERROR_NONE if no error occurred
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_error_kind_t dnv_vista_sdk_last_error_kind(void);

    /**
     * @brief Clear the last error on the calling thread
     * @details Resets both the message and the kind to their initial state
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_clear_error(void);

#ifdef __cplusplus
}
#endif
