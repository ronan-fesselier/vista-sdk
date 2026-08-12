/**
 * @file iso19848.h
 * @brief C API for dnv::vista::sdk::transport::{ISO19848, ISO19848Version, DataChannelTypeName(s),
 *        FormatDataType(s), Value}
 * @details `dnv_vista_sdk_iso19848_t*` is a borrowed pointer to the singleton - never freed.
 *          `dnv_vista_sdk_iso19848_data_channel_type_names_t*`/`dnv_vista_sdk_iso19848_format_data_types_t*`/
 *          `dnv_vista_sdk_iso19848_data_channel_type_name_t*`/`dnv_vista_sdk_iso19848_format_data_type_t*`/
 *          `dnv_vista_sdk_iso19848_value_t*` are owned and must be released with their respective `_free`.
 *          The `match()` templates on Value/FormatDataType are not exposed - `_type()` plus the typed
 *          accessors cover the same runtime dispatch.
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "../types/datetime/date_time_offset.h"
#include "../types/decimal/decimal.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_iso19848 dnv_vista_sdk_iso19848_t;
    typedef struct dnv_vista_sdk_iso19848_data_channel_type_name dnv_vista_sdk_iso19848_data_channel_type_name_t;
    typedef struct dnv_vista_sdk_iso19848_data_channel_type_names dnv_vista_sdk_iso19848_data_channel_type_names_t;
    typedef struct dnv_vista_sdk_iso19848_format_data_type dnv_vista_sdk_iso19848_format_data_type_t;
    typedef struct dnv_vista_sdk_iso19848_format_data_types dnv_vista_sdk_iso19848_format_data_types_t;
    typedef struct dnv_vista_sdk_iso19848_value dnv_vista_sdk_iso19848_value_t;

    /** @brief Mirrors dnv::vista::sdk::transport::ISO19848Version */
    typedef enum
    {
        DNV_VISTA_SDK_ISO19848_VERSION_V2018 = 0,
        DNV_VISTA_SDK_ISO19848_VERSION_V2024
    } dnv_vista_sdk_iso19848_version_t;

    /** @brief Mirrors dnv::vista::sdk::transport::Value::Type */
    typedef enum
    {
        DNV_VISTA_SDK_ISO19848_VALUE_TYPE_DECIMAL = 0,
        DNV_VISTA_SDK_ISO19848_VALUE_TYPE_INTEGER,
        DNV_VISTA_SDK_ISO19848_VALUE_TYPE_BOOLEAN,
        DNV_VISTA_SDK_ISO19848_VALUE_TYPE_STRING,
        DNV_VISTA_SDK_ISO19848_VALUE_TYPE_DATE_TIME
    } dnv_vista_sdk_iso19848_value_type_t;

    //=========================================================================
    // ISO19848 singleton
    //=========================================================================

    /**
     * @brief Get the ISO19848 singleton instance
     * @return Borrowed pointer, valid for the lifetime of the program
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_iso19848_t* dnv_vista_sdk_iso19848_instance(void);

    /** @brief Number of available ISO 19848 versions */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_iso19848_version_count(const dnv_vista_sdk_iso19848_t* iso19848);

    /**
     * @brief Get an available ISO 19848 version by index
     * @param iso19848 Instance obtained from dnv_vista_sdk_iso19848_instance
     * @param index Zero-based index, must be < dnv_vista_sdk_iso19848_version_count(iso19848)
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_iso19848_version_t
    dnv_vista_sdk_iso19848_version_at(const dnv_vista_sdk_iso19848_t* iso19848, size_t index);

    /** @brief Most recent ISO 19848 version */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_iso19848_version_t
    dnv_vista_sdk_iso19848_latest(const dnv_vista_sdk_iso19848_t* iso19848);

    /**
     * @brief Get the data channel type names for a specific version
     * @return Owned handle, must be released with dnv_vista_sdk_iso19848_data_channel_type_names_free,
     *         or NULL if `iso19848` is NULL or `version` has no defined type names
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_iso19848_data_channel_type_names_t*
    dnv_vista_sdk_iso19848_data_channel_type_names(
        const dnv_vista_sdk_iso19848_t* iso19848, dnv_vista_sdk_iso19848_version_t version);

    /**
     * @brief Get the format data types for a specific version
     * @return Owned handle, must be released with dnv_vista_sdk_iso19848_format_data_types_free,
     *         or NULL if `iso19848` is NULL or `version` has no defined format data types
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_iso19848_format_data_types_t* dnv_vista_sdk_iso19848_format_data_types(
        const dnv_vista_sdk_iso19848_t* iso19848, dnv_vista_sdk_iso19848_version_t version);

    //=========================================================================
    // DataChannelTypeName / DataChannelTypeNames
    //=========================================================================

    /** @brief Release a DataChannelTypeName handle, may be NULL (no-op) */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_iso19848_data_channel_type_name_free(
        dnv_vista_sdk_iso19848_data_channel_type_name_t* typeName);

    /** @brief Borrowed, valid as long as `typeName` is valid */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_iso19848_data_channel_type_name_type(
        const dnv_vista_sdk_iso19848_data_channel_type_name_t* typeName);

    /** @brief Borrowed, valid as long as `typeName` is valid */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_iso19848_data_channel_type_name_description(
        const dnv_vista_sdk_iso19848_data_channel_type_name_t* typeName);

    /** @brief Release a DataChannelTypeNames handle, may be NULL (no-op) */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_iso19848_data_channel_type_names_free(
        dnv_vista_sdk_iso19848_data_channel_type_names_t* typeNames);

    /**
     * @brief Look up a DataChannelTypeName by its type identifier
     * @return Owned handle, must be released with dnv_vista_sdk_iso19848_data_channel_type_name_free,
     *         or NULL if not found or if `typeNames`/`type` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_iso19848_data_channel_type_name_t*
    dnv_vista_sdk_iso19848_data_channel_type_names_from_string(
        const dnv_vista_sdk_iso19848_data_channel_type_names_t* typeNames, const char* type);

    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_iso19848_data_channel_type_names_count(
        const dnv_vista_sdk_iso19848_data_channel_type_names_t* typeNames);

    /**
     * @brief Get an element by index
     * @param typeNames Collection to read from
     * @param index Zero-based index, must be < dnv_vista_sdk_iso19848_data_channel_type_names_count(typeNames)
     * @return Borrowed pointer, valid as long as `typeNames` is valid, or NULL if out of range
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_iso19848_data_channel_type_name_t*
    dnv_vista_sdk_iso19848_data_channel_type_names_at(
        const dnv_vista_sdk_iso19848_data_channel_type_names_t* typeNames, size_t index);

    //=========================================================================
    // FormatDataType / FormatDataTypes
    //=========================================================================

    /** @brief Release a FormatDataType handle, may be NULL (no-op) */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_iso19848_format_data_type_free(
        dnv_vista_sdk_iso19848_format_data_type_t* formatDataType);

    /** @brief Borrowed, valid as long as `formatDataType` is valid */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_iso19848_format_data_type_type(
        const dnv_vista_sdk_iso19848_format_data_type_t* formatDataType);

    /** @brief Borrowed, valid as long as `formatDataType` is valid */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_iso19848_format_data_type_description(
        const dnv_vista_sdk_iso19848_format_data_type_t* formatDataType);

    /**
     * @brief Validate and parse a string value against this format type
     * @param formatDataType Format type to validate against, must not be NULL
     * @param value String value to validate, must not be NULL
     * @param result Set to the parsed Value on success (owned, must be released with
     *               dnv_vista_sdk_iso19848_value_free), untouched on failure
     * @return 1 if validation succeeded, 0 otherwise (sets the last error message)
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_iso19848_format_data_type_validate(
        const dnv_vista_sdk_iso19848_format_data_type_t* formatDataType,
        const char* value,
        dnv_vista_sdk_iso19848_value_t** result);

    /** @brief Release a FormatDataTypes handle, may be NULL (no-op) */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_iso19848_format_data_types_free(
        dnv_vista_sdk_iso19848_format_data_types_t* formatDataTypes);

    /**
     * @brief Look up a FormatDataType by its type identifier
     * @return Owned handle, must be released with dnv_vista_sdk_iso19848_format_data_type_free,
     *         or NULL if not found or if `formatDataTypes`/`type` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_iso19848_format_data_type_t* dnv_vista_sdk_iso19848_format_data_types_from_string(
        const dnv_vista_sdk_iso19848_format_data_types_t* formatDataTypes, const char* type);

    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_iso19848_format_data_types_count(const dnv_vista_sdk_iso19848_format_data_types_t* formatDataTypes);

    /**
     * @brief Get an element by index
     * @param formatDataTypes Collection to read from
     * @param index Zero-based index, must be < dnv_vista_sdk_iso19848_format_data_types_count(formatDataTypes)
     * @return Borrowed pointer, valid as long as `formatDataTypes` is valid, or NULL if out of range
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_iso19848_format_data_type_t* dnv_vista_sdk_iso19848_format_data_types_at(
        const dnv_vista_sdk_iso19848_format_data_types_t* formatDataTypes, size_t index);

    //=========================================================================
    // Value
    //=========================================================================

    DNV_VISTA_SDK_C_API dnv_vista_sdk_iso19848_value_t* dnv_vista_sdk_iso19848_value_from_string(const char* value);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_iso19848_value_t* dnv_vista_sdk_iso19848_value_from_integer(int64_t value);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_iso19848_value_t* dnv_vista_sdk_iso19848_value_from_boolean(int value);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_iso19848_value_t* dnv_vista_sdk_iso19848_value_from_decimal(
        dnv_vista_sdk_decimal_t value);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_iso19848_value_t* dnv_vista_sdk_iso19848_value_from_date_time(
        dnv_vista_sdk_date_time_offset_t value);

    /** @brief Release a Value handle, may be NULL (no-op) */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_iso19848_value_free(dnv_vista_sdk_iso19848_value_t* value);

    /** @brief Get the type as an enum for readable switch statements */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_iso19848_value_type_t
    dnv_vista_sdk_iso19848_value_type(const dnv_vista_sdk_iso19848_value_t* value);

    /**
     * @brief Get the String value, if this Value holds one
     * @return Borrowed, valid as long as `value` is valid, or NULL if `value` is NULL or holds another type
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_iso19848_value_string(const dnv_vista_sdk_iso19848_value_t* value);

    /**
     * @brief Get the Boolean value, if this Value holds one
     * @param value Value to read from
     * @param result Set to the value on success, untouched on failure
     * @return 1 if `value` holds a Boolean, 0 otherwise
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_iso19848_value_boolean(
        const dnv_vista_sdk_iso19848_value_t* value, int* result);

    /**
     * @brief Get the Integer value, if this Value holds one
     * @param value Value to read from
     * @param result Set to the value on success, untouched on failure
     * @return 1 if `value` holds an Integer, 0 otherwise
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_iso19848_value_integer(
        const dnv_vista_sdk_iso19848_value_t* value, int64_t* result);

    /**
     * @brief Get the Decimal value, if this Value holds one
     * @param value Value to read from
     * @param result Set to the value on success, untouched on failure
     * @return 1 if `value` holds a Decimal, 0 otherwise
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_iso19848_value_decimal(
        const dnv_vista_sdk_iso19848_value_t* value, dnv_vista_sdk_decimal_t* result);

    /**
     * @brief Get the DateTime value, if this Value holds one
     * @param value Value to read from
     * @param result Set to the value on success, untouched on failure
     * @return 1 if `value` holds a DateTime, 0 otherwise
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_iso19848_value_date_time(
        const dnv_vista_sdk_iso19848_value_t* value, dnv_vista_sdk_date_time_offset_t* result);

    /**
     * @brief Convert to string representation
     * @return Owned, null-terminated string, must be released with dnv_vista_sdk_iso19848_value_string_free,
     *         or NULL if `value` is NULL
     */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_iso19848_value_to_string(const dnv_vista_sdk_iso19848_value_t* value);

    /**
     * @brief Release a string obtained from this API
     * @param str String obtained from dnv_vista_sdk_iso19848_value_to_string, may be NULL (no-op)
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_iso19848_value_string_free(char* str);

#ifdef __cplusplus
}
#endif
