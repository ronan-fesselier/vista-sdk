/**
 * @file parsing_errors.h
 * @brief C API for dnv::vista::sdk::ParsingErrors
 * @details `dnv_vista_sdk_parsing_errors_t*` is owned by value and must be released
 *          with dnv_vista_sdk_parsing_errors_free
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_parsing_errors dnv_vista_sdk_parsing_errors_t;

    /**
     * @brief Release a ParsingErrors handle
     * @param errors Handle obtained from this API, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_parsing_errors_free(dnv_vista_sdk_parsing_errors_t* errors);

    /**
     * @brief Get the number of errors
     * @param errors Handle obtained from this API
     * @return Error count, or 0 if `errors` is NULL
     */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_parsing_errors_count(const dnv_vista_sdk_parsing_errors_t* errors);

    /**
     * @brief Check if any errors exist
     * @param errors Handle obtained from this API
     * @return 1 if the collection is non-empty, 0 otherwise or if `errors` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_parsing_errors_has_errors(const dnv_vista_sdk_parsing_errors_t* errors);

    /**
     * @brief Check if a specific error type exists
     * @param errors Handle obtained from this API
     * @param type Error type identifier to search for
     * @return 1 if at least one entry with that type is present, 0 otherwise or if
     *         `errors`/`type` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_parsing_errors_has_error_type(
        const dnv_vista_sdk_parsing_errors_t* errors, const char* type);

    /**
     * @brief Get the error type at a given index
     * @param errors Handle obtained from this API
     * @param index Zero-based index, must be < dnv_vista_sdk_parsing_errors_count(errors)
     * @return Null-terminated type string, owned by `errors` - valid as long as
     *         `errors` is valid, or NULL if `errors` is NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_parsing_errors_type_at(
        const dnv_vista_sdk_parsing_errors_t* errors, size_t index);

    /**
     * @brief Get the error message at a given index
     * @param errors Handle obtained from this API
     * @param index Zero-based index, must be < dnv_vista_sdk_parsing_errors_count(errors)
     * @return Null-terminated message string, owned by `errors` - valid as long as
     *         `errors` is valid, or NULL if `errors` is NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_parsing_errors_message_at(
        const dnv_vista_sdk_parsing_errors_t* errors, size_t index);

    /**
     * @brief Format errors as a human-readable string
     * @param errors Handle obtained from this API
     * @return Owned, null-terminated string ("Success" if empty), must be released
     *         with dnv_vista_sdk_parsing_errors_string_free, or NULL if `errors` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_parsing_errors_to_string(const dnv_vista_sdk_parsing_errors_t* errors);

    /**
     * @brief Release a string obtained from this API
     * @param str String obtained from dnv_vista_sdk_parsing_errors_to_string, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_parsing_errors_string_free(char* str);

#ifdef __cplusplus
}
#endif
