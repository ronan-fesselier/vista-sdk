/**
 * @file data_channel.h
 * @brief C API for dnv::vista::sdk::transport::datachannel (ISO 19848 DataChannelList domain model)
 * @details All types in this header are owned by value and must be released with their
 *          respective `_free` function, following the standard opaque-pointer pattern.
 *
 *          Prefixed `dnv_vista_sdk_dcl_*` to distinguish from the timeseries (`tsd_*`)
 *          domain model - `datachannel::DataChannelId` (Table 15, holds a LocalId plus
 *          optional ShortId/NameObject) is a different type from
 *          `timeseries::DataChannelId` (a discriminated union of LocalId/string), which
 *          gets its own `dnv_vista_sdk_tsd_*` wrapper in a later module.
 *
 *          `Format::validateValue`'s `Value&` out-parameter is not exposed here - `Value`
 *          (ISO19848.h) is not yet wrapped in the C API. Both `Restriction::validateValue`
 *          and `Format::validateValue` are exposed as plain int (1/0) plus the first error
 *          message via dnv_vista_sdk_last_error_message(), consistent with the rest of the
 *          C API's error handling convention - callers needing the full error list or the
 *          parsed Value must wait for a future ISO19848 primitives module
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "../../core/local_id.h"
#include "../../transport/serialization/json/serializable_document.h"
#include "../../transport/ship_id.h"
#include "../../types/datetime/date_time_offset.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_dcl_restriction dnv_vista_sdk_dcl_restriction_t;
    typedef struct dnv_vista_sdk_dcl_range dnv_vista_sdk_dcl_range_t;
    typedef struct dnv_vista_sdk_dcl_format dnv_vista_sdk_dcl_format_t;
    typedef struct dnv_vista_sdk_dcl_data_channel_type dnv_vista_sdk_dcl_data_channel_type_t;
    typedef struct dnv_vista_sdk_dcl_name_object dnv_vista_sdk_dcl_name_object_t;
    typedef struct dnv_vista_sdk_dcl_unit dnv_vista_sdk_dcl_unit_t;
    typedef struct dnv_vista_sdk_dcl_property dnv_vista_sdk_dcl_property_t;
    typedef struct dnv_vista_sdk_dcl_configuration_reference dnv_vista_sdk_dcl_configuration_reference_t;
    typedef struct dnv_vista_sdk_dcl_version_information dnv_vista_sdk_dcl_version_information_t;
    typedef struct dnv_vista_sdk_dcl_channel_id dnv_vista_sdk_dcl_channel_id_t;
    typedef struct dnv_vista_sdk_dcl_header dnv_vista_sdk_dcl_header_t;
    typedef struct dnv_vista_sdk_dcl_data_channel dnv_vista_sdk_dcl_data_channel_t;
    typedef struct dnv_vista_sdk_dcl_data_channel_list dnv_vista_sdk_dcl_data_channel_list_t;
    typedef struct dnv_vista_sdk_dcl_package dnv_vista_sdk_dcl_package_t;
    typedef struct dnv_vista_sdk_dcl_list_package dnv_vista_sdk_dcl_list_package_t;

    /** @brief Mirrors dnv::vista::sdk::transport::datachannel::Restriction::WhiteSpace */
    typedef enum
    {
        DNV_VISTA_SDK_DCL_WHITE_SPACE_PRESERVE = 0,
        DNV_VISTA_SDK_DCL_WHITE_SPACE_REPLACE,
        DNV_VISTA_SDK_DCL_WHITE_SPACE_COLLAPSE
    } dnv_vista_sdk_dcl_white_space_t;

    /*=====================================================================
     * Restriction - Table 19
     *===================================================================*/

    /** @brief Construct an empty Restriction (all fields unset) */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_restriction_t* dnv_vista_sdk_dcl_restriction_create(void);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_free(dnv_vista_sdk_dcl_restriction_t* r);

    /** @brief Number of entries in the enumeration, or 0 if `r` is NULL or unset */
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_dcl_restriction_enumeration_count(const dnv_vista_sdk_dcl_restriction_t* r);
    /**
     * @brief Get an enumeration entry by index
     * @return Null-terminated string, owned by `r`, or NULL if `r` is NULL, unset, or `index` out of range
     */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_restriction_enumeration_at(
        const dnv_vista_sdk_dcl_restriction_t* r, size_t index);
    /**
     * @brief Set the enumeration of acceptable values
     * @param r Restriction to modify, must not be NULL
     * @param values Array of null-terminated strings, may be NULL if `count` is 0
     * @param count Number of entries in `values`
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_set_enumeration(
        dnv_vista_sdk_dcl_restriction_t* r, const char* const* values, size_t count);
    /** @brief Clear the enumeration (unset) */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_clear_enumeration(dnv_vista_sdk_dcl_restriction_t* r);

    /** @brief 1 if fractionDigits is set, 0 otherwise */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_restriction_has_fraction_digits(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API uint32_t
    dnv_vista_sdk_dcl_restriction_fraction_digits(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_set_fraction_digits(
        dnv_vista_sdk_dcl_restriction_t* r, uint32_t value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_clear_fraction_digits(dnv_vista_sdk_dcl_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_restriction_has_length(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API uint32_t dnv_vista_sdk_dcl_restriction_length(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_set_length(
        dnv_vista_sdk_dcl_restriction_t* r, uint32_t value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_clear_length(dnv_vista_sdk_dcl_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_restriction_has_max_exclusive(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_restriction_max_exclusive(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_set_max_exclusive(
        dnv_vista_sdk_dcl_restriction_t* r, double value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_clear_max_exclusive(dnv_vista_sdk_dcl_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_restriction_has_max_inclusive(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_restriction_max_inclusive(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_set_max_inclusive(
        dnv_vista_sdk_dcl_restriction_t* r, double value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_clear_max_inclusive(dnv_vista_sdk_dcl_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_restriction_has_max_length(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API uint32_t dnv_vista_sdk_dcl_restriction_max_length(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_set_max_length(
        dnv_vista_sdk_dcl_restriction_t* r, uint32_t value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_clear_max_length(dnv_vista_sdk_dcl_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_restriction_has_min_exclusive(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_restriction_min_exclusive(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_set_min_exclusive(
        dnv_vista_sdk_dcl_restriction_t* r, double value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_clear_min_exclusive(dnv_vista_sdk_dcl_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_restriction_has_min_inclusive(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_restriction_min_inclusive(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_set_min_inclusive(
        dnv_vista_sdk_dcl_restriction_t* r, double value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_clear_min_inclusive(dnv_vista_sdk_dcl_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_restriction_has_min_length(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API uint32_t dnv_vista_sdk_dcl_restriction_min_length(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_set_min_length(
        dnv_vista_sdk_dcl_restriction_t* r, uint32_t value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_clear_min_length(dnv_vista_sdk_dcl_restriction_t* r);

    /** @brief Get the pattern string, or NULL if `r` is NULL or unset */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_restriction_pattern(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_set_pattern(
        dnv_vista_sdk_dcl_restriction_t* r, const char* value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_clear_pattern(dnv_vista_sdk_dcl_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_restriction_has_total_digits(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API uint32_t dnv_vista_sdk_dcl_restriction_total_digits(const dnv_vista_sdk_dcl_restriction_t* r);
    /** @brief Sets the last error message and leaves `r` unchanged if `value` is 0 */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_set_total_digits(
        dnv_vista_sdk_dcl_restriction_t* r, uint32_t value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_clear_total_digits(dnv_vista_sdk_dcl_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_restriction_has_white_space(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_white_space_t
    dnv_vista_sdk_dcl_restriction_white_space(const dnv_vista_sdk_dcl_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_set_white_space(
        dnv_vista_sdk_dcl_restriction_t* r, dnv_vista_sdk_dcl_white_space_t value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_restriction_clear_white_space(dnv_vista_sdk_dcl_restriction_t* r);

    /**
     * @brief Validate a string value against this restriction and a format
     * @return 1 if valid, 0 otherwise (with the first validation error set via
     *         dnv_vista_sdk_last_error_message()), or if any argument is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_restriction_validate_value(
        const dnv_vista_sdk_dcl_restriction_t* r, const char* value, const dnv_vista_sdk_dcl_format_t* format);

    /*=====================================================================
     * Range - Table 20
     *===================================================================*/

    /** @brief Construct a Range; sets the last error message and returns NULL if low >= high */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_range_t* dnv_vista_sdk_dcl_range_create(double low, double high);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_range_free(dnv_vista_sdk_dcl_range_t* range);

    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_range_low(const dnv_vista_sdk_dcl_range_t* range);
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_range_high(const dnv_vista_sdk_dcl_range_t* range);
    /** @brief Sets the last error message and leaves `range` unchanged if low > current high */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_range_set_low(dnv_vista_sdk_dcl_range_t* range, double low);
    /** @brief Sets the last error message and leaves `range` unchanged if high < current low */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_range_set_high(dnv_vista_sdk_dcl_range_t* range, double high);

    /*=====================================================================
     * Format - Table 18
     *===================================================================*/

    /**
     * @brief Construct a Format
     * @return Owned handle, or NULL if `type` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_format_t* dnv_vista_sdk_dcl_format_create(const char* type);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_format_free(dnv_vista_sdk_dcl_format_t* format);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_format_type(const dnv_vista_sdk_dcl_format_t* format);
    /**
     * @brief Set the format type
     * @return 1 on success, 0 (with the last error message set) if `type` is not a
     *         recognized ISO 19848 format type, `format`/`type` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_format_set_type(dnv_vista_sdk_dcl_format_t* format, const char* type);

    /** @brief Borrowed pointer, valid as long as `format` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_restriction_t* dnv_vista_sdk_dcl_format_restriction(
        const dnv_vista_sdk_dcl_format_t* format);
    /** @brief Copies `restriction` into `format`. `restriction` remains owned by the caller */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_format_set_restriction(
        dnv_vista_sdk_dcl_format_t* format, const dnv_vista_sdk_dcl_restriction_t* restriction);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_format_clear_restriction(dnv_vista_sdk_dcl_format_t* format);

    /**
     * @brief Validate a string value against this format (type and optional restriction)
     * @return 1 if valid, 0 otherwise (with the first validation error set via
     *         dnv_vista_sdk_last_error_message()), or if any argument is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_format_validate_value(
        const dnv_vista_sdk_dcl_format_t* format, const char* value);

    /*=====================================================================
     * DataChannelType - Table 17
     *===================================================================*/

    /**
     * @brief Construct a DataChannelType
     * @return Owned handle, or NULL if `type` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_data_channel_type_t* dnv_vista_sdk_dcl_data_channel_type_create(
        const char* type);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_data_channel_type_free(dnv_vista_sdk_dcl_data_channel_type_t* dct);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_data_channel_type_type(
        const dnv_vista_sdk_dcl_data_channel_type_t* dct);
    /**
     * @brief Set the data channel type
     * @return 1 on success, 0 (with the last error message set) if `type` is not a
     *         recognized ISO 19848 data channel type, `dct`/`type` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_data_channel_type_set_type(
        dnv_vista_sdk_dcl_data_channel_type_t* dct, const char* type);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_data_channel_type_has_update_cycle(
        const dnv_vista_sdk_dcl_data_channel_type_t* dct);
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_data_channel_type_update_cycle(
        const dnv_vista_sdk_dcl_data_channel_type_t* dct);
    /** @brief Sets the last error message and leaves `dct` unchanged if `value` is negative */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_data_channel_type_set_update_cycle(
        dnv_vista_sdk_dcl_data_channel_type_t* dct, double value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_data_channel_type_clear_update_cycle(
        dnv_vista_sdk_dcl_data_channel_type_t* dct);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_data_channel_type_has_calculation_period(
        const dnv_vista_sdk_dcl_data_channel_type_t* dct);
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_data_channel_type_calculation_period(
        const dnv_vista_sdk_dcl_data_channel_type_t* dct);
    /** @brief Sets the last error message and leaves `dct` unchanged if `value` is negative */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_data_channel_type_set_calculation_period(
        dnv_vista_sdk_dcl_data_channel_type_t* dct, double value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_data_channel_type_clear_calculation_period(
        dnv_vista_sdk_dcl_data_channel_type_t* dct);

    /** @brief 1 if type is "Alert", 0 otherwise or if `dct` is NULL */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_data_channel_type_is_alert(
        const dnv_vista_sdk_dcl_data_channel_type_t* dct);

    /*=====================================================================
     * NameObject - Table 22
     *===================================================================*/

    /** @brief Construct a NameObject with the default Annex C naming rule ("/dnv-v2") */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_name_object_t* dnv_vista_sdk_dcl_name_object_create_default(void);
    /**
     * @brief Construct a NameObject with an explicit naming rule
     * @return Owned handle, or NULL if `namingRule` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_name_object_t* dnv_vista_sdk_dcl_name_object_create(const char* namingRule);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_name_object_free(dnv_vista_sdk_dcl_name_object_t* nameObject);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_name_object_naming_rule(
        const dnv_vista_sdk_dcl_name_object_t* nameObject);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_name_object_set_naming_rule(
        dnv_vista_sdk_dcl_name_object_t* nameObject, const char* namingRule);

    /** @brief Borrowed pointer, valid as long as `nameObject` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_name_object_custom_name_objects(
        const dnv_vista_sdk_dcl_name_object_t* nameObject);
    /**
     * @brief Set the custom name objects extension point
     * @details Takes ownership of `value` - do not free it separately after this call
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_name_object_set_custom_name_objects(
        dnv_vista_sdk_dcl_name_object_t* nameObject, dnv_vista_sdk_serializable_document_t* value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_name_object_clear_custom_name_objects(
        dnv_vista_sdk_dcl_name_object_t* nameObject);

    /*=====================================================================
     * Unit - Table 21
     *===================================================================*/

    /**
     * @brief Construct a Unit
     * @return Owned handle, or NULL if `unitSymbol` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_unit_t* dnv_vista_sdk_dcl_unit_create(const char* unitSymbol);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_unit_free(dnv_vista_sdk_dcl_unit_t* unit);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_unit_unit_symbol(const dnv_vista_sdk_dcl_unit_t* unit);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_unit_set_unit_symbol(
        dnv_vista_sdk_dcl_unit_t* unit, const char* unitSymbol);

    /** @brief Get the quantity name, or NULL if `unit` is NULL or unset */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_unit_quantity_name(const dnv_vista_sdk_dcl_unit_t* unit);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_unit_set_quantity_name(
        dnv_vista_sdk_dcl_unit_t* unit, const char* quantityName);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_unit_clear_quantity_name(dnv_vista_sdk_dcl_unit_t* unit);

    /** @brief Borrowed pointer, valid as long as `unit` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_unit_custom_elements(
        const dnv_vista_sdk_dcl_unit_t* unit);
    /**
     * @brief Set the custom elements extension point
     * @details Takes ownership of `value` - do not free it separately after this call
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_unit_set_custom_elements(
        dnv_vista_sdk_dcl_unit_t* unit, dnv_vista_sdk_serializable_document_t* value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_unit_clear_custom_elements(dnv_vista_sdk_dcl_unit_t* unit);

    /*=====================================================================
     * Property - Table 16
     *===================================================================*/

    /**
     * @brief Construct a Property
     * @details Copies `dataChannelType` and `format` - both remain owned by the caller
     * @return Owned handle, or NULL if `dataChannelType`/`format` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_property_t* dnv_vista_sdk_dcl_property_create(
        const dnv_vista_sdk_dcl_data_channel_type_t* dataChannelType, const dnv_vista_sdk_dcl_format_t* format);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_free(dnv_vista_sdk_dcl_property_t* property);

    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_data_channel_type_t* dnv_vista_sdk_dcl_property_data_channel_type(
        const dnv_vista_sdk_dcl_property_t* property);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_set_data_channel_type(
        dnv_vista_sdk_dcl_property_t* property, const dnv_vista_sdk_dcl_data_channel_type_t* dataChannelType);

    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_format_t* dnv_vista_sdk_dcl_property_format(
        const dnv_vista_sdk_dcl_property_t* property);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_set_format(
        dnv_vista_sdk_dcl_property_t* property, const dnv_vista_sdk_dcl_format_t* format);

    /** @brief Borrowed pointer, valid as long as `property` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_range_t* dnv_vista_sdk_dcl_property_range(
        const dnv_vista_sdk_dcl_property_t* property);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_set_range(
        dnv_vista_sdk_dcl_property_t* property, const dnv_vista_sdk_dcl_range_t* range);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_clear_range(dnv_vista_sdk_dcl_property_t* property);

    /** @brief Borrowed pointer, valid as long as `property` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_unit_t* dnv_vista_sdk_dcl_property_unit(
        const dnv_vista_sdk_dcl_property_t* property);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_set_unit(
        dnv_vista_sdk_dcl_property_t* property, const dnv_vista_sdk_dcl_unit_t* unit);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_clear_unit(dnv_vista_sdk_dcl_property_t* property);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_property_quality_coding(
        const dnv_vista_sdk_dcl_property_t* property);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_set_quality_coding(
        dnv_vista_sdk_dcl_property_t* property, const char* qualityCoding);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_clear_quality_coding(dnv_vista_sdk_dcl_property_t* property);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_property_alert_priority(
        const dnv_vista_sdk_dcl_property_t* property);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_set_alert_priority(
        dnv_vista_sdk_dcl_property_t* property, const char* alertPriority);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_clear_alert_priority(dnv_vista_sdk_dcl_property_t* property);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_property_name(const dnv_vista_sdk_dcl_property_t* property);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_set_name(
        dnv_vista_sdk_dcl_property_t* property, const char* name);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_clear_name(dnv_vista_sdk_dcl_property_t* property);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_property_remarks(const dnv_vista_sdk_dcl_property_t* property);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_set_remarks(
        dnv_vista_sdk_dcl_property_t* property, const char* remarks);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_clear_remarks(dnv_vista_sdk_dcl_property_t* property);

    /** @brief Borrowed pointer, valid as long as `property` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_property_custom_properties(
        const dnv_vista_sdk_dcl_property_t* property);
    /**
     * @brief Set the custom properties extension point
     * @details Takes ownership of `value` - do not free it separately after this call
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_set_custom_properties(
        dnv_vista_sdk_dcl_property_t* property, dnv_vista_sdk_serializable_document_t* value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_property_clear_custom_properties(dnv_vista_sdk_dcl_property_t* property);

    /**
     * @brief Validate property consistency (Range/Unit for Decimal, AlertPriority for Alert)
     * @return 1 if valid, 0 otherwise (with the first validation error set via
     *         dnv_vista_sdk_last_error_message()), or if `property` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_property_validate(const dnv_vista_sdk_dcl_property_t* property);

    /*=====================================================================
     * ConfigurationReference - Table 11
     *===================================================================*/

    /**
     * @brief Construct a ConfigurationReference
     * @return Owned handle, or NULL if `id` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_configuration_reference_t* dnv_vista_sdk_dcl_configuration_reference_create(
        const char* id, dnv_vista_sdk_date_time_offset_t timeStamp);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_configuration_reference_free(
        dnv_vista_sdk_dcl_configuration_reference_t* configRef);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_configuration_reference_id(
        const dnv_vista_sdk_dcl_configuration_reference_t* configRef);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_configuration_reference_set_id(
        dnv_vista_sdk_dcl_configuration_reference_t* configRef, const char* id);

    /** @brief Get the version string, or NULL if `configRef` is NULL or unset */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_configuration_reference_version(
        const dnv_vista_sdk_dcl_configuration_reference_t* configRef);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_configuration_reference_set_version(
        dnv_vista_sdk_dcl_configuration_reference_t* configRef, const char* version);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_configuration_reference_clear_version(
        dnv_vista_sdk_dcl_configuration_reference_t* configRef);

    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_dcl_configuration_reference_timestamp(const dnv_vista_sdk_dcl_configuration_reference_t* configRef);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_configuration_reference_set_timestamp(
        dnv_vista_sdk_dcl_configuration_reference_t* configRef, dnv_vista_sdk_date_time_offset_t timeStamp);

    /*=====================================================================
     * VersionInformation - Table 12
     *===================================================================*/

    /** @brief Construct VersionInformation with default Annex C naming rule/scheme version/reference URL */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_version_information_t* dnv_vista_sdk_dcl_version_information_create_default(
        void);
    /**
     * @brief Construct VersionInformation with explicit fields
     * @return Owned handle, or NULL if `namingRule`/`namingSchemeVersion` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_version_information_t* dnv_vista_sdk_dcl_version_information_create(
        const char* namingRule, const char* namingSchemeVersion);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_version_information_free(
        dnv_vista_sdk_dcl_version_information_t* versionInfo);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_version_information_naming_rule(
        const dnv_vista_sdk_dcl_version_information_t* versionInfo);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_version_information_set_naming_rule(
        dnv_vista_sdk_dcl_version_information_t* versionInfo, const char* namingRule);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_version_information_naming_scheme_version(
        const dnv_vista_sdk_dcl_version_information_t* versionInfo);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_version_information_set_naming_scheme_version(
        dnv_vista_sdk_dcl_version_information_t* versionInfo, const char* namingSchemeVersion);

    /** @brief Get the reference URL, or NULL if `versionInfo` is NULL or unset */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_version_information_reference_url(
        const dnv_vista_sdk_dcl_version_information_t* versionInfo);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_version_information_set_reference_url(
        dnv_vista_sdk_dcl_version_information_t* versionInfo, const char* referenceUrl);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_version_information_clear_reference_url(
        dnv_vista_sdk_dcl_version_information_t* versionInfo);

    /*=====================================================================
     * DataChannelId - Table 15 (datachannel-specific.
     * See file @details for the distinction from timeseries::DataChannelId)
     *===================================================================*/

    /**
     * @brief Construct a DataChannelId
     * @details Copies `localId` - it remains owned by the caller
     * @return Owned handle, or NULL if `localId` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_channel_id_t* dnv_vista_sdk_dcl_channel_id_create(
        const dnv_vista_sdk_local_id_t* localId);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_channel_id_free(dnv_vista_sdk_dcl_channel_id_t* channelId);

    /** @brief Borrowed pointer, valid as long as `channelId` is valid */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_local_id_t* dnv_vista_sdk_dcl_channel_id_local_id(
        const dnv_vista_sdk_dcl_channel_id_t* channelId);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_channel_id_set_local_id(
        dnv_vista_sdk_dcl_channel_id_t* channelId, const dnv_vista_sdk_local_id_t* localId);

    /** @brief Get the short ID string, or NULL if `channelId` is NULL or unset */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_channel_id_short_id(
        const dnv_vista_sdk_dcl_channel_id_t* channelId);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_channel_id_set_short_id(
        dnv_vista_sdk_dcl_channel_id_t* channelId, const char* shortId);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_channel_id_clear_short_id(dnv_vista_sdk_dcl_channel_id_t* channelId);

    /** @brief Borrowed pointer, valid as long as `channelId` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_name_object_t* dnv_vista_sdk_dcl_channel_id_name_object(
        const dnv_vista_sdk_dcl_channel_id_t* channelId);
    /** @brief Copies `nameObject` into `channelId`. `nameObject` remains owned by the caller */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_channel_id_set_name_object(
        dnv_vista_sdk_dcl_channel_id_t* channelId, const dnv_vista_sdk_dcl_name_object_t* nameObject);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_channel_id_clear_name_object(dnv_vista_sdk_dcl_channel_id_t* channelId);

    /*=====================================================================
     * Header - Table 10
     *===================================================================*/

    /**
     * @brief Construct a Header
     * @details Copies `shipId` and `dataChannelListId` - both remain owned by the caller
     * @return Owned handle, or NULL if `shipId`/`dataChannelListId` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_header_t* dnv_vista_sdk_dcl_header_create(
        const dnv_vista_sdk_ship_id_t* shipId, const dnv_vista_sdk_dcl_configuration_reference_t* dataChannelListId);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_header_free(dnv_vista_sdk_dcl_header_t* header);

    /** @brief Borrowed pointer, valid as long as `header` is valid */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_ship_id_t* dnv_vista_sdk_dcl_header_ship_id(
        const dnv_vista_sdk_dcl_header_t* header);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_header_set_ship_id(
        dnv_vista_sdk_dcl_header_t* header, const dnv_vista_sdk_ship_id_t* shipId);

    /** @brief Borrowed pointer, valid as long as `header` is valid */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_configuration_reference_t*
    dnv_vista_sdk_dcl_header_data_channel_list_id(const dnv_vista_sdk_dcl_header_t* header);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_header_set_data_channel_list_id(
        dnv_vista_sdk_dcl_header_t* header, const dnv_vista_sdk_dcl_configuration_reference_t* dataChannelListId);

    /** @brief Borrowed pointer, valid as long as `header` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_version_information_t* dnv_vista_sdk_dcl_header_version_information(
        const dnv_vista_sdk_dcl_header_t* header);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_header_set_version_information(
        dnv_vista_sdk_dcl_header_t* header, const dnv_vista_sdk_dcl_version_information_t* versionInformation);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_header_clear_version_information(dnv_vista_sdk_dcl_header_t* header);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_header_author(const dnv_vista_sdk_dcl_header_t* header);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_header_set_author(
        dnv_vista_sdk_dcl_header_t* header, const char* author);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_header_clear_author(dnv_vista_sdk_dcl_header_t* header);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_header_has_date_created(const dnv_vista_sdk_dcl_header_t* header);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_dcl_header_date_created(const dnv_vista_sdk_dcl_header_t* header);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_header_set_date_created(
        dnv_vista_sdk_dcl_header_t* header, dnv_vista_sdk_date_time_offset_t dateCreated);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_header_clear_date_created(dnv_vista_sdk_dcl_header_t* header);

    /** @brief Borrowed pointer, valid as long as `header` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_header_custom_headers(
        const dnv_vista_sdk_dcl_header_t* header);
    /**
     * @brief Set the custom headers extension point
     * @details Takes ownership of `value` - do not free it separately after this call
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_header_set_custom_headers(
        dnv_vista_sdk_dcl_header_t* header, dnv_vista_sdk_serializable_document_t* value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_header_clear_custom_headers(dnv_vista_sdk_dcl_header_t* header);

    /*=====================================================================
     * DataChannel - Table 14
     *===================================================================*/

    /**
     * @brief Construct a DataChannel
     * @details Copies `channelId` and `property` - both remain owned by the caller
     * @return Owned handle, or NULL if `channelId`/`property` is NULL, or if
     *         `property`'s consistency validation fails (last error message set)
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_data_channel_t* dnv_vista_sdk_dcl_data_channel_create(
        const dnv_vista_sdk_dcl_channel_id_t* channelId, const dnv_vista_sdk_dcl_property_t* property);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_data_channel_free(dnv_vista_sdk_dcl_data_channel_t* dataChannel);

    /** @brief Borrowed pointer, valid as long as `dataChannel` is valid */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_channel_id_t* dnv_vista_sdk_dcl_data_channel_channel_id(
        const dnv_vista_sdk_dcl_data_channel_t* dataChannel);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_data_channel_set_channel_id(
        dnv_vista_sdk_dcl_data_channel_t* dataChannel, const dnv_vista_sdk_dcl_channel_id_t* channelId);

    /** @brief Borrowed pointer, valid as long as `dataChannel` is valid */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_property_t* dnv_vista_sdk_dcl_data_channel_property(
        const dnv_vista_sdk_dcl_data_channel_t* dataChannel);
    /**
     * @brief Set the property, with consistency validation
     * @return 1 on success, 0 (with the last error message set) if validation fails or
     *         any argument is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_data_channel_set_property(
        dnv_vista_sdk_dcl_data_channel_t* dataChannel, const dnv_vista_sdk_dcl_property_t* property);

    /*=====================================================================
     * DataChannelList - Table 13
     *===================================================================*/

    /** @brief Construct an empty DataChannelList */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_data_channel_list_t* dnv_vista_sdk_dcl_data_channel_list_create(void);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_data_channel_list_free(dnv_vista_sdk_dcl_data_channel_list_t* list);

    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_dcl_data_channel_list_size(const dnv_vista_sdk_dcl_data_channel_list_t* list);

    /**
     * @brief Get a data channel by insertion-order index
     * @return Borrowed pointer, valid as long as `list` is valid, or NULL if `list` is
     *         NULL or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_data_channel_t* dnv_vista_sdk_dcl_data_channel_list_at(
        const dnv_vista_sdk_dcl_data_channel_list_t* list, size_t index);
    /**
     * @brief Look up a data channel by short ID
     * @return Borrowed pointer, valid as long as `list` is valid, or NULL if not found
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_data_channel_t* dnv_vista_sdk_dcl_data_channel_list_from_short_id(
        const dnv_vista_sdk_dcl_data_channel_list_t* list, const char* shortId);
    /**
     * @brief Look up a data channel by LocalId
     * @return Borrowed pointer, valid as long as `list` is valid, or NULL if not found
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_data_channel_t* dnv_vista_sdk_dcl_data_channel_list_from_local_id(
        const dnv_vista_sdk_dcl_data_channel_list_t* list, const dnv_vista_sdk_local_id_t* localId);

    /**
     * @brief Add a data channel to the list, copying it
     * @return 1 on success, 0 (with the last error message set) if a data channel with
     *         the same LocalId/ShortId already exists, or if any argument is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_data_channel_list_add(
        dnv_vista_sdk_dcl_data_channel_list_t* list, const dnv_vista_sdk_dcl_data_channel_t* dataChannel);

    /**
     * @brief Remove a data channel matching `item`'s LocalId
     * @return 1 if removed, 0 if not found or any argument is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_data_channel_list_remove(
        dnv_vista_sdk_dcl_data_channel_list_t* list, const dnv_vista_sdk_dcl_data_channel_t* item);

    /** @brief Remove all data channels from the list */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_data_channel_list_clear(dnv_vista_sdk_dcl_data_channel_list_t* list);

    /*=====================================================================
     * Package - Table 9
     *===================================================================*/

    /**
     * @brief Construct a Package
     * @details Copies `header` and `dataChannelList` - both remain owned by the caller
     * @return Owned handle, or NULL if `header`/`dataChannelList` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_package_t* dnv_vista_sdk_dcl_package_create(
        const dnv_vista_sdk_dcl_header_t* header, const dnv_vista_sdk_dcl_data_channel_list_t* dataChannelList);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_package_free(dnv_vista_sdk_dcl_package_t* package);

    /** @brief Borrowed pointer, valid as long as `package` is valid */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_header_t* dnv_vista_sdk_dcl_package_header(
        const dnv_vista_sdk_dcl_package_t* package);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_package_set_header(
        dnv_vista_sdk_dcl_package_t* package, const dnv_vista_sdk_dcl_header_t* header);

    /** @brief Borrowed pointer, valid as long as `package` is valid */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_data_channel_list_t* dnv_vista_sdk_dcl_package_data_channel_list(
        const dnv_vista_sdk_dcl_package_t* package);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_package_set_data_channel_list(
        dnv_vista_sdk_dcl_package_t* package, const dnv_vista_sdk_dcl_data_channel_list_t* dataChannelList);

    /*=====================================================================
     * DataChannelListPackage
     *===================================================================*/

    /**
     * @brief Construct a DataChannelListPackage
     * @details Copies `package` - it remains owned by the caller
     * @return Owned handle, or NULL if `package` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_list_package_t* dnv_vista_sdk_dcl_list_package_create(
        const dnv_vista_sdk_dcl_package_t* package);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_list_package_free(dnv_vista_sdk_dcl_list_package_t* listPackage);

    /** @brief Borrowed pointer, valid as long as `listPackage` is valid */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_package_t* dnv_vista_sdk_dcl_list_package_package(
        const dnv_vista_sdk_dcl_list_package_t* listPackage);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_list_package_set_package(
        dnv_vista_sdk_dcl_list_package_t* listPackage, const dnv_vista_sdk_dcl_package_t* package);
    /** @brief Convenience accessor for the wrapped package's data channel list (borrowed) */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_dcl_data_channel_list_t* dnv_vista_sdk_dcl_list_package_data_channel_list(
        const dnv_vista_sdk_dcl_list_package_t* listPackage);

#ifdef __cplusplus
}
#endif
