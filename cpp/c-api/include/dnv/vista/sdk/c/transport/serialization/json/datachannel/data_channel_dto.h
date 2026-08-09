/**
 * @file data_channel_dto.h
 * @brief C API for DataChannelListPackageDto and its nested DTO tree
 * @details Exposes the same DTO hierarchy as the C++ `jdc::toDto`/`toDomain`.
 *          Lets a C consumer inspect and patch a DataChannelList package
 *          (author, custom headers, etc.) before serialization.
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "dnv/vista/sdk/c/transport/datachannel/data_channel.h"
#include "dnv/vista/sdk/c/transport/serialization/json/serializable_document.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_dcl_dto_package dnv_vista_sdk_dcl_dto_package_t;
    typedef struct dnv_vista_sdk_dcl_dto_pkg dnv_vista_sdk_dcl_dto_pkg_t;
    typedef struct dnv_vista_sdk_dcl_dto_header dnv_vista_sdk_dcl_dto_header_t;
    typedef struct dnv_vista_sdk_dcl_dto_cfg_ref dnv_vista_sdk_dcl_dto_cfg_ref_t;
    typedef struct dnv_vista_sdk_dcl_dto_ver_info dnv_vista_sdk_dcl_dto_ver_info_t;
    typedef struct dnv_vista_sdk_dcl_dto_ch_list dnv_vista_sdk_dcl_dto_ch_list_t;
    typedef struct dnv_vista_sdk_dcl_dto_channel dnv_vista_sdk_dcl_dto_channel_t;
    typedef struct dnv_vista_sdk_dcl_dto_ch_id dnv_vista_sdk_dcl_dto_ch_id_t;
    typedef struct dnv_vista_sdk_dcl_dto_name_obj dnv_vista_sdk_dcl_dto_name_obj_t;
    typedef struct dnv_vista_sdk_dcl_dto_property dnv_vista_sdk_dcl_dto_property_t;
    typedef struct dnv_vista_sdk_dcl_dto_ch_type dnv_vista_sdk_dcl_dto_ch_type_t;
    typedef struct dnv_vista_sdk_dcl_dto_format dnv_vista_sdk_dcl_dto_format_t;
    typedef struct dnv_vista_sdk_dcl_dto_restriction dnv_vista_sdk_dcl_dto_restriction_t;
    typedef struct dnv_vista_sdk_dcl_dto_range dnv_vista_sdk_dcl_dto_range_t;
    typedef struct dnv_vista_sdk_dcl_dto_unit dnv_vista_sdk_dcl_dto_unit_t;

    /*=========================================================================
     * Root: DataChannelListPackageDto
     *=======================================================================*/

    /** @brief Convert a domain package to a DTO. Caller owns the result. Free with dnv_vista_sdk_dcl_dto_package_free.
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_package_t* dnv_vista_sdk_dcl_to_dto(
        const dnv_vista_sdk_dcl_list_package_t* domain);

    /** @brief Convert a DTO back to a domain package. Returns NULL on construction failure. Caller owns the result. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_list_package_t* dnv_vista_sdk_dcl_to_domain(
        const dnv_vista_sdk_dcl_dto_package_t* dto);

    /** @brief Serialize a DTO to a JSON string. Caller frees with dnv_vista_sdk_string_free. */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_dcl_dto_to_json(
        const dnv_vista_sdk_dcl_dto_package_t* dto, int prettyPrint);

    /** @brief Parse a JSON string into a DTO. Returns NULL on parse failure. Caller owns the result. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_package_t* dnv_vista_sdk_dcl_dto_from_json(const char* json);

    /** @brief Free a DTO package handle. All sub-handles derived from it become invalid. */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_package_free(dnv_vista_sdk_dcl_dto_package_t* p);

    /** @brief Non-owning view of the embedded PackageDto. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_pkg_t* dnv_vista_sdk_dcl_dto_package_get_pkg(
        dnv_vista_sdk_dcl_dto_package_t* p);

    /*=========================================================================
     * PackageDto
     *=======================================================================*/

    /** @brief Non-owning view of the embedded HeaderDto. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_header_t* dnv_vista_sdk_dcl_dto_pkg_get_header(
        dnv_vista_sdk_dcl_dto_pkg_t* pkg);

    /** @brief Non-owning view of the embedded DataChannelListDto. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_ch_list_t* dnv_vista_sdk_dcl_dto_pkg_get_channel_list(
        dnv_vista_sdk_dcl_dto_pkg_t* pkg);

    /*=========================================================================
     * HeaderDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_header_get_ship_id(const dnv_vista_sdk_dcl_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_header_set_ship_id(dnv_vista_sdk_dcl_dto_header_t* h, const char* v);

    /** @brief Non-owning view of the embedded ConfigurationReferenceDto. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_cfg_ref_t* dnv_vista_sdk_dcl_dto_header_get_cfg_ref(
        dnv_vista_sdk_dcl_dto_header_t* h);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_header_has_ver_info(const dnv_vista_sdk_dcl_dto_header_t* h);
    /** @brief Non-owning view of the optional VersionInformationDto, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_ver_info_t* dnv_vista_sdk_dcl_dto_header_get_ver_info(
        dnv_vista_sdk_dcl_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_header_ensure_ver_info(dnv_vista_sdk_dcl_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_header_clear_ver_info(dnv_vista_sdk_dcl_dto_header_t* h);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_header_has_author(const dnv_vista_sdk_dcl_dto_header_t* h);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_header_get_author(const dnv_vista_sdk_dcl_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_header_set_author(dnv_vista_sdk_dcl_dto_header_t* h, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_header_clear_author(dnv_vista_sdk_dcl_dto_header_t* h);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_header_has_date_created(const dnv_vista_sdk_dcl_dto_header_t* h);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_header_get_date_created(
        const dnv_vista_sdk_dcl_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_header_set_date_created(
        dnv_vista_sdk_dcl_dto_header_t* h, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_header_clear_date_created(dnv_vista_sdk_dcl_dto_header_t* h);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_header_has_custom_headers(const dnv_vista_sdk_dcl_dto_header_t* h);
    /** @brief Non-owning view of the optional SerializableDocument, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_dto_header_get_custom_headers(
        dnv_vista_sdk_dcl_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_header_ensure_custom_headers(dnv_vista_sdk_dcl_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_header_clear_custom_headers(dnv_vista_sdk_dcl_dto_header_t* h);

    /*=========================================================================
     * ConfigurationReferenceDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_cfg_ref_get_id(const dnv_vista_sdk_dcl_dto_cfg_ref_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_cfg_ref_set_id(dnv_vista_sdk_dcl_dto_cfg_ref_t* r, const char* v);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_cfg_ref_get_timestamp(
        const dnv_vista_sdk_dcl_dto_cfg_ref_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_cfg_ref_set_timestamp(
        dnv_vista_sdk_dcl_dto_cfg_ref_t* r, const char* v);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_cfg_ref_has_version(const dnv_vista_sdk_dcl_dto_cfg_ref_t* r);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_cfg_ref_get_version(const dnv_vista_sdk_dcl_dto_cfg_ref_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_cfg_ref_set_version(
        dnv_vista_sdk_dcl_dto_cfg_ref_t* r, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_cfg_ref_clear_version(dnv_vista_sdk_dcl_dto_cfg_ref_t* r);

    /*=========================================================================
     * VersionInformationDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_ver_info_get_naming_rule(
        const dnv_vista_sdk_dcl_dto_ver_info_t* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ver_info_set_naming_rule(
        dnv_vista_sdk_dcl_dto_ver_info_t* v, const char* s);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_ver_info_get_naming_scheme_version(
        const dnv_vista_sdk_dcl_dto_ver_info_t* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ver_info_set_naming_scheme_version(
        dnv_vista_sdk_dcl_dto_ver_info_t* v, const char* s);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_ver_info_has_reference_url(const dnv_vista_sdk_dcl_dto_ver_info_t* v);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_ver_info_get_reference_url(
        const dnv_vista_sdk_dcl_dto_ver_info_t* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ver_info_set_reference_url(
        dnv_vista_sdk_dcl_dto_ver_info_t* v, const char* s);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ver_info_clear_reference_url(dnv_vista_sdk_dcl_dto_ver_info_t* v);

    /*=========================================================================
     * DataChannelListDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_dcl_dto_ch_list_count(const dnv_vista_sdk_dcl_dto_ch_list_t* list);

    /**
     * @brief Non-owning view of channel at index, or NULL if out of range. Do NOT free.
     * @warning The returned view (and any view derived from it) is invalidated by a subsequent
     *          call to dnv_vista_sdk_dcl_dto_ch_list_push or dnv_vista_sdk_dcl_dto_ch_list_remove
     *          on the same list, both of which may reallocate or shift the underlying storage.
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_channel_t* dnv_vista_sdk_dcl_dto_ch_list_at(
        dnv_vista_sdk_dcl_dto_ch_list_t* list, size_t index);

    /**
     * @brief Append a default-constructed DataChannelDto. Returns non-owning view. Do NOT free.
     * @warning Invalidates all views previously obtained from dnv_vista_sdk_dcl_dto_ch_list_at or
     *          dnv_vista_sdk_dcl_dto_ch_list_push on the same list (vector reallocation).
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_channel_t* dnv_vista_sdk_dcl_dto_ch_list_push(
        dnv_vista_sdk_dcl_dto_ch_list_t* list);

    /**
     * @brief Remove the channel at index, shifting subsequent elements.
     * @warning Invalidates all views previously obtained from dnv_vista_sdk_dcl_dto_ch_list_at or
     *          dnv_vista_sdk_dcl_dto_ch_list_push on the same list, including views at indices
     *          before the removed one (vector element relocation).
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ch_list_remove(dnv_vista_sdk_dcl_dto_ch_list_t* list, size_t index);

    /*=========================================================================
     * DataChannelDto
     *=======================================================================*/

    /** @brief Non-owning view of the embedded DataChannelIdDto. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_ch_id_t* dnv_vista_sdk_dcl_dto_channel_get_id(
        dnv_vista_sdk_dcl_dto_channel_t* ch);

    /** @brief Non-owning view of the embedded PropertyDto. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_property_t* dnv_vista_sdk_dcl_dto_channel_get_property(
        dnv_vista_sdk_dcl_dto_channel_t* ch);

    /*=========================================================================
     * DataChannelIdDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_ch_id_get_local_id(const dnv_vista_sdk_dcl_dto_ch_id_t* id);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ch_id_set_local_id(dnv_vista_sdk_dcl_dto_ch_id_t* id, const char* v);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_ch_id_has_short_id(const dnv_vista_sdk_dcl_dto_ch_id_t* id);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_ch_id_get_short_id(const dnv_vista_sdk_dcl_dto_ch_id_t* id);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ch_id_set_short_id(dnv_vista_sdk_dcl_dto_ch_id_t* id, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ch_id_clear_short_id(dnv_vista_sdk_dcl_dto_ch_id_t* id);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_ch_id_has_name_object(const dnv_vista_sdk_dcl_dto_ch_id_t* id);
    /** @brief Non-owning view, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_name_obj_t* dnv_vista_sdk_dcl_dto_ch_id_get_name_object(
        dnv_vista_sdk_dcl_dto_ch_id_t* id);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ch_id_ensure_name_object(dnv_vista_sdk_dcl_dto_ch_id_t* id);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ch_id_clear_name_object(dnv_vista_sdk_dcl_dto_ch_id_t* id);

    /*=========================================================================
     * NameObjectDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_name_obj_get_naming_rule(
        const dnv_vista_sdk_dcl_dto_name_obj_t* n);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_name_obj_set_naming_rule(
        dnv_vista_sdk_dcl_dto_name_obj_t* n, const char* v);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_name_obj_has_custom(const dnv_vista_sdk_dcl_dto_name_obj_t* n);
    /** @brief Non-owning view, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_dto_name_obj_get_custom(
        dnv_vista_sdk_dcl_dto_name_obj_t* n);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_name_obj_ensure_custom(dnv_vista_sdk_dcl_dto_name_obj_t* n);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_name_obj_clear_custom(dnv_vista_sdk_dcl_dto_name_obj_t* n);

    /*=========================================================================
     * PropertyDto
     *=======================================================================*/

    /** @brief Non-owning view of the embedded DataChannelTypeDto. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_ch_type_t* dnv_vista_sdk_dcl_dto_property_get_ch_type(
        dnv_vista_sdk_dcl_dto_property_t* p);

    /** @brief Non-owning view of the embedded FormatDto. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_format_t* dnv_vista_sdk_dcl_dto_property_get_format(
        dnv_vista_sdk_dcl_dto_property_t* p);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_property_has_range(const dnv_vista_sdk_dcl_dto_property_t* p);
    /** @brief Non-owning view, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_range_t* dnv_vista_sdk_dcl_dto_property_get_range(
        dnv_vista_sdk_dcl_dto_property_t* p);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_property_ensure_range(dnv_vista_sdk_dcl_dto_property_t* p);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_property_clear_range(dnv_vista_sdk_dcl_dto_property_t* p);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_property_has_unit(const dnv_vista_sdk_dcl_dto_property_t* p);
    /** @brief Non-owning view, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_unit_t* dnv_vista_sdk_dcl_dto_property_get_unit(
        dnv_vista_sdk_dcl_dto_property_t* p);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_property_ensure_unit(dnv_vista_sdk_dcl_dto_property_t* p);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_property_clear_unit(dnv_vista_sdk_dcl_dto_property_t* p);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_property_has_quality_coding(
        const dnv_vista_sdk_dcl_dto_property_t* p);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_property_get_quality_coding(
        const dnv_vista_sdk_dcl_dto_property_t* p);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_property_set_quality_coding(
        dnv_vista_sdk_dcl_dto_property_t* p, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_property_clear_quality_coding(dnv_vista_sdk_dcl_dto_property_t* p);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_property_has_alert_priority(
        const dnv_vista_sdk_dcl_dto_property_t* p);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_property_get_alert_priority(
        const dnv_vista_sdk_dcl_dto_property_t* p);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_property_set_alert_priority(
        dnv_vista_sdk_dcl_dto_property_t* p, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_property_clear_alert_priority(dnv_vista_sdk_dcl_dto_property_t* p);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_property_has_name(const dnv_vista_sdk_dcl_dto_property_t* p);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_property_get_name(const dnv_vista_sdk_dcl_dto_property_t* p);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_property_set_name(
        dnv_vista_sdk_dcl_dto_property_t* p, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_property_clear_name(dnv_vista_sdk_dcl_dto_property_t* p);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_property_has_remarks(const dnv_vista_sdk_dcl_dto_property_t* p);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_property_get_remarks(
        const dnv_vista_sdk_dcl_dto_property_t* p);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_property_set_remarks(
        dnv_vista_sdk_dcl_dto_property_t* p, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_property_clear_remarks(dnv_vista_sdk_dcl_dto_property_t* p);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_property_has_custom_properties(
        const dnv_vista_sdk_dcl_dto_property_t* p);
    /** @brief Non-owning view, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_dto_property_get_custom_properties(
        dnv_vista_sdk_dcl_dto_property_t* p);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_property_ensure_custom_properties(
        dnv_vista_sdk_dcl_dto_property_t* p);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_property_clear_custom_properties(
        dnv_vista_sdk_dcl_dto_property_t* p);

    /*=========================================================================
     * DataChannelTypeDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_ch_type_get_type(const dnv_vista_sdk_dcl_dto_ch_type_t* t);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ch_type_set_type(dnv_vista_sdk_dcl_dto_ch_type_t* t, const char* v);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_ch_type_has_update_cycle(const dnv_vista_sdk_dcl_dto_ch_type_t* t);
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_dto_ch_type_get_update_cycle(const dnv_vista_sdk_dcl_dto_ch_type_t* t);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ch_type_set_update_cycle(
        dnv_vista_sdk_dcl_dto_ch_type_t* t, double v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ch_type_clear_update_cycle(dnv_vista_sdk_dcl_dto_ch_type_t* t);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_ch_type_has_calculation_period(
        const dnv_vista_sdk_dcl_dto_ch_type_t* t);
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_dto_ch_type_get_calculation_period(
        const dnv_vista_sdk_dcl_dto_ch_type_t* t);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ch_type_set_calculation_period(
        dnv_vista_sdk_dcl_dto_ch_type_t* t, double v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_ch_type_clear_calculation_period(dnv_vista_sdk_dcl_dto_ch_type_t* t);

    /*=========================================================================
     * FormatDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_format_get_type(const dnv_vista_sdk_dcl_dto_format_t* f);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_format_set_type(dnv_vista_sdk_dcl_dto_format_t* f, const char* v);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_format_has_restriction(const dnv_vista_sdk_dcl_dto_format_t* f);
    /** @brief Non-owning view, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_dcl_dto_restriction_t* dnv_vista_sdk_dcl_dto_format_get_restriction(
        dnv_vista_sdk_dcl_dto_format_t* f);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_format_ensure_restriction(dnv_vista_sdk_dcl_dto_format_t* f);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_format_clear_restriction(dnv_vista_sdk_dcl_dto_format_t* f);

    /*=========================================================================
     * RestrictionDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_dcl_dto_restriction_enumeration_count(const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_restriction_enumeration_at(
        const dnv_vista_sdk_dcl_dto_restriction_t* r, size_t i);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_set_enumeration(
        dnv_vista_sdk_dcl_dto_restriction_t* r, const char* const* values, size_t count);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_clear_enumeration(
        dnv_vista_sdk_dcl_dto_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_restriction_has_fraction_digits(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API uint32_t
    dnv_vista_sdk_dcl_dto_restriction_get_fraction_digits(const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_set_fraction_digits(
        dnv_vista_sdk_dcl_dto_restriction_t* r, uint32_t v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_clear_fraction_digits(
        dnv_vista_sdk_dcl_dto_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_restriction_has_length(const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API uint32_t
    dnv_vista_sdk_dcl_dto_restriction_get_length(const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_set_length(
        dnv_vista_sdk_dcl_dto_restriction_t* r, uint32_t v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_clear_length(dnv_vista_sdk_dcl_dto_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_restriction_has_max_exclusive(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_dto_restriction_get_max_exclusive(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_set_max_exclusive(
        dnv_vista_sdk_dcl_dto_restriction_t* r, double v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_clear_max_exclusive(
        dnv_vista_sdk_dcl_dto_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_restriction_has_max_inclusive(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_dto_restriction_get_max_inclusive(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_set_max_inclusive(
        dnv_vista_sdk_dcl_dto_restriction_t* r, double v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_clear_max_inclusive(
        dnv_vista_sdk_dcl_dto_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_restriction_has_max_length(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API uint32_t
    dnv_vista_sdk_dcl_dto_restriction_get_max_length(const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_set_max_length(
        dnv_vista_sdk_dcl_dto_restriction_t* r, uint32_t v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_clear_max_length(dnv_vista_sdk_dcl_dto_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_restriction_has_min_exclusive(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_dto_restriction_get_min_exclusive(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_set_min_exclusive(
        dnv_vista_sdk_dcl_dto_restriction_t* r, double v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_clear_min_exclusive(
        dnv_vista_sdk_dcl_dto_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_restriction_has_min_inclusive(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_dto_restriction_get_min_inclusive(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_set_min_inclusive(
        dnv_vista_sdk_dcl_dto_restriction_t* r, double v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_clear_min_inclusive(
        dnv_vista_sdk_dcl_dto_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_restriction_has_min_length(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API uint32_t
    dnv_vista_sdk_dcl_dto_restriction_get_min_length(const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_set_min_length(
        dnv_vista_sdk_dcl_dto_restriction_t* r, uint32_t v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_clear_min_length(dnv_vista_sdk_dcl_dto_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_restriction_has_pattern(const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_restriction_get_pattern(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_set_pattern(
        dnv_vista_sdk_dcl_dto_restriction_t* r, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_clear_pattern(dnv_vista_sdk_dcl_dto_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_restriction_has_total_digits(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API uint32_t
    dnv_vista_sdk_dcl_dto_restriction_get_total_digits(const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_set_total_digits(
        dnv_vista_sdk_dcl_dto_restriction_t* r, uint32_t v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_clear_total_digits(
        dnv_vista_sdk_dcl_dto_restriction_t* r);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_restriction_has_white_space(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_restriction_get_white_space(
        const dnv_vista_sdk_dcl_dto_restriction_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_set_white_space(
        dnv_vista_sdk_dcl_dto_restriction_t* r, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_restriction_clear_white_space(
        dnv_vista_sdk_dcl_dto_restriction_t* r);

    /*=========================================================================
     * RangeDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_dto_range_get_low(const dnv_vista_sdk_dcl_dto_range_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_range_set_low(dnv_vista_sdk_dcl_dto_range_t* r, double v);
    DNV_VISTA_SDK_C_API double dnv_vista_sdk_dcl_dto_range_get_high(const dnv_vista_sdk_dcl_dto_range_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_range_set_high(dnv_vista_sdk_dcl_dto_range_t* r, double v);

    /*=========================================================================
     * UnitDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_unit_get_symbol(const dnv_vista_sdk_dcl_dto_unit_t* u);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_unit_set_symbol(dnv_vista_sdk_dcl_dto_unit_t* u, const char* v);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_unit_has_quantity_name(const dnv_vista_sdk_dcl_dto_unit_t* u);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_dcl_dto_unit_get_quantity_name(const dnv_vista_sdk_dcl_dto_unit_t* u);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_unit_set_quantity_name(
        dnv_vista_sdk_dcl_dto_unit_t* u, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_unit_clear_quantity_name(dnv_vista_sdk_dcl_dto_unit_t* u);
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_dcl_dto_unit_has_custom_elements(const dnv_vista_sdk_dcl_dto_unit_t* u);
    /** @brief Non-owning view, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_dto_unit_get_custom_elements(
        dnv_vista_sdk_dcl_dto_unit_t* u);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_unit_ensure_custom_elements(dnv_vista_sdk_dcl_dto_unit_t* u);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_dcl_dto_unit_clear_custom_elements(dnv_vista_sdk_dcl_dto_unit_t* u);

#ifdef __cplusplus
}
#endif
