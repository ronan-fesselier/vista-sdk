/**
 * @file time_series_data_dto.h
 * @brief C API for TimeSeriesDataPackageDto and its nested DTO tree
 * @details Exposes the same DTO hierarchy as the C++ `jtsd::toDto`/`toDomain`.
 *          Lets a C consumer inspect and patch a TimeSeriesData package
 *          (author, custom headers, etc.) before serialization.
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "dnv/vista/sdk/c/transport/timeseries/time_series_data.h"
#include "dnv/vista/sdk/c/transport/serialization/json/serializable_document.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_tsd_dto_package dnv_vista_sdk_tsd_dto_package_t;
    typedef struct dnv_vista_sdk_tsd_dto_pkg dnv_vista_sdk_tsd_dto_pkg_t;
    typedef struct dnv_vista_sdk_tsd_dto_header dnv_vista_sdk_tsd_dto_header_t;
    typedef struct dnv_vista_sdk_tsd_dto_time_span dnv_vista_sdk_tsd_dto_time_span_t;
    typedef struct dnv_vista_sdk_tsd_dto_cfg_ref dnv_vista_sdk_tsd_dto_cfg_ref_t;
    typedef struct dnv_vista_sdk_tsd_dto_tsd dnv_vista_sdk_tsd_dto_tsd_t;
    typedef struct dnv_vista_sdk_tsd_dto_tabular dnv_vista_sdk_tsd_dto_tabular_t;
    typedef struct dnv_vista_sdk_tsd_dto_tab_set dnv_vista_sdk_tsd_dto_tab_set_t;
    typedef struct dnv_vista_sdk_tsd_dto_event dnv_vista_sdk_tsd_dto_event_t;
    typedef struct dnv_vista_sdk_tsd_dto_event_set dnv_vista_sdk_tsd_dto_event_set_t;

    /*=========================================================================
     * Root: TimeSeriesDataPackageDto
     *=======================================================================*/

    /** @brief Convert a domain package to a DTO. Caller owns the result. Free with dnv_vista_sdk_tsd_dto_package_free.
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_package_t* dnv_vista_sdk_tsd_to_dto(
        const dnv_vista_sdk_tsd_data_package_t* domain);

    /** @brief Convert a DTO back to a domain package. Returns NULL on failure. Caller owns the result. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_data_package_t* dnv_vista_sdk_tsd_to_domain(
        const dnv_vista_sdk_tsd_dto_package_t* dto);

    /** @brief Serialize a DTO to a JSON string. Caller frees with dnv_vista_sdk_string_free. */
    DNV_VISTA_SDK_C_API char* dnv_vista_sdk_tsd_dto_to_json(
        const dnv_vista_sdk_tsd_dto_package_t* dto, int prettyPrint);

    /** @brief Parse a JSON string into a DTO. Returns NULL on parse failure. Caller owns the result. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_package_t* dnv_vista_sdk_tsd_dto_from_json(const char* json);

    /** @brief Free a DTO package handle. All sub-handles derived from it become invalid. */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_package_free(dnv_vista_sdk_tsd_dto_package_t* p);

    /** @brief Non-owning view of the embedded PackageDto. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_pkg_t* dnv_vista_sdk_tsd_dto_package_get_pkg(
        dnv_vista_sdk_tsd_dto_package_t* p);

    /*=========================================================================
     * PackageDto
     *=======================================================================*/

    /** @brief Whether the optional HeaderDto is set. */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_dto_pkg_has_header(const dnv_vista_sdk_tsd_dto_pkg_t* pkg);

    /** @brief Non-owning view of the optional HeaderDto, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_header_t* dnv_vista_sdk_tsd_dto_pkg_get_header(
        dnv_vista_sdk_tsd_dto_pkg_t* pkg);

    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_pkg_ensure_header(dnv_vista_sdk_tsd_dto_pkg_t* pkg);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_pkg_clear_header(dnv_vista_sdk_tsd_dto_pkg_t* pkg);

    /** @brief Number of TimeSeriesData entries. */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_tsd_dto_pkg_tsd_count(const dnv_vista_sdk_tsd_dto_pkg_t* pkg);

    /**
     * @brief Non-owning view of TimeSeriesDataDto at index, or NULL if out of range. Do NOT free.
     * @warning Invalidated by a subsequent push or remove on the same pkg.
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_tsd_t* dnv_vista_sdk_tsd_dto_pkg_tsd_at(
        dnv_vista_sdk_tsd_dto_pkg_t* pkg, size_t index);

    /**
     * @brief Append a default-constructed TimeSeriesDataDto. Returns non-owning view. Do NOT free.
     * @warning Invalidates all views previously obtained from dnv_vista_sdk_tsd_dto_pkg_tsd_at.
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_tsd_t* dnv_vista_sdk_tsd_dto_pkg_tsd_push(
        dnv_vista_sdk_tsd_dto_pkg_t* pkg);

    /**
     * @brief Remove the TimeSeriesDataDto at index, shifting subsequent elements.
     * @warning Invalidates all views previously obtained from dnv_vista_sdk_tsd_dto_pkg_tsd_at.
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_pkg_tsd_remove(dnv_vista_sdk_tsd_dto_pkg_t* pkg, size_t index);

    /*=========================================================================
     * HeaderDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_header_get_ship_id(const dnv_vista_sdk_tsd_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_header_set_ship_id(dnv_vista_sdk_tsd_dto_header_t* h, const char* v);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_dto_header_has_time_span(const dnv_vista_sdk_tsd_dto_header_t* h);
    /** @brief Non-owning view, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_time_span_t* dnv_vista_sdk_tsd_dto_header_get_time_span(
        dnv_vista_sdk_tsd_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_header_ensure_time_span(dnv_vista_sdk_tsd_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_header_clear_time_span(dnv_vista_sdk_tsd_dto_header_t* h);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_dto_header_has_date_created(const dnv_vista_sdk_tsd_dto_header_t* h);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_header_get_date_created(
        const dnv_vista_sdk_tsd_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_header_set_date_created(
        dnv_vista_sdk_tsd_dto_header_t* h, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_header_clear_date_created(dnv_vista_sdk_tsd_dto_header_t* h);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_dto_header_has_date_modified(const dnv_vista_sdk_tsd_dto_header_t* h);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_header_get_date_modified(
        const dnv_vista_sdk_tsd_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_header_set_date_modified(
        dnv_vista_sdk_tsd_dto_header_t* h, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_header_clear_date_modified(dnv_vista_sdk_tsd_dto_header_t* h);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_dto_header_has_author(const dnv_vista_sdk_tsd_dto_header_t* h);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_header_get_author(const dnv_vista_sdk_tsd_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_header_set_author(dnv_vista_sdk_tsd_dto_header_t* h, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_header_clear_author(dnv_vista_sdk_tsd_dto_header_t* h);

    /** @brief Number of entries in the system configuration list. */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_tsd_dto_header_system_cfg_count(const dnv_vista_sdk_tsd_dto_header_t* h);

    /**
     * @brief Non-owning view of ConfigurationReferenceDto at index, or NULL if out of range. Do NOT free.
     * @warning Invalidated by a subsequent push or remove on the same header.
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_cfg_ref_t* dnv_vista_sdk_tsd_dto_header_system_cfg_at(
        dnv_vista_sdk_tsd_dto_header_t* h, size_t index);

    /**
     * @brief Append a default-constructed ConfigurationReferenceDto. Returns non-owning view. Do NOT free.
     * @warning Invalidates all views from dnv_vista_sdk_tsd_dto_header_system_cfg_at.
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_cfg_ref_t* dnv_vista_sdk_tsd_dto_header_system_cfg_push(
        dnv_vista_sdk_tsd_dto_header_t* h);

    /**
     * @brief Remove the ConfigurationReferenceDto at index, shifting subsequent elements.
     * @warning Invalidates all views from dnv_vista_sdk_tsd_dto_header_system_cfg_at.
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_header_system_cfg_remove(
        dnv_vista_sdk_tsd_dto_header_t* h, size_t index);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_dto_header_has_custom_headers(const dnv_vista_sdk_tsd_dto_header_t* h);
    /** @brief Non-owning view, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_tsd_dto_header_get_custom_headers(
        dnv_vista_sdk_tsd_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_header_ensure_custom_headers(dnv_vista_sdk_tsd_dto_header_t* h);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_header_clear_custom_headers(dnv_vista_sdk_tsd_dto_header_t* h);

    /*=========================================================================
     * TimeSpanDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_time_span_get_start(
        const dnv_vista_sdk_tsd_dto_time_span_t* ts);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_time_span_set_start(
        dnv_vista_sdk_tsd_dto_time_span_t* ts, const char* v);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_time_span_get_end(
        const dnv_vista_sdk_tsd_dto_time_span_t* ts);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_time_span_set_end(
        dnv_vista_sdk_tsd_dto_time_span_t* ts, const char* v);

    /*=========================================================================
     * ConfigurationReferenceDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_cfg_ref_get_id(const dnv_vista_sdk_tsd_dto_cfg_ref_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_cfg_ref_set_id(dnv_vista_sdk_tsd_dto_cfg_ref_t* r, const char* v);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_cfg_ref_get_timestamp(
        const dnv_vista_sdk_tsd_dto_cfg_ref_t* r);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_cfg_ref_set_timestamp(
        dnv_vista_sdk_tsd_dto_cfg_ref_t* r, const char* v);

    /*=========================================================================
     * TimeSeriesDataDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_dto_tsd_has_data_cfg(const dnv_vista_sdk_tsd_dto_tsd_t* tsd);
    /** @brief Non-owning view, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_cfg_ref_t* dnv_vista_sdk_tsd_dto_tsd_get_data_cfg(
        dnv_vista_sdk_tsd_dto_tsd_t* tsd);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tsd_ensure_data_cfg(dnv_vista_sdk_tsd_dto_tsd_t* tsd);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tsd_clear_data_cfg(dnv_vista_sdk_tsd_dto_tsd_t* tsd);

    /** @brief Number of TabularDataDto entries. */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_tsd_dto_tsd_tabular_count(const dnv_vista_sdk_tsd_dto_tsd_t* tsd);

    /**
     * @brief Non-owning view of TabularDataDto at index, or NULL if out of range. Do NOT free.
     * @warning Invalidated by a subsequent push or remove on the same tsd.
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_tabular_t* dnv_vista_sdk_tsd_dto_tsd_tabular_at(
        dnv_vista_sdk_tsd_dto_tsd_t* tsd, size_t index);

    /**
     * @brief Append a default-constructed TabularDataDto. Returns non-owning view. Do NOT free.
     * @warning Invalidates all views from dnv_vista_sdk_tsd_dto_tsd_tabular_at.
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_tabular_t* dnv_vista_sdk_tsd_dto_tsd_tabular_push(
        dnv_vista_sdk_tsd_dto_tsd_t* tsd);

    /**
     * @brief Remove the TabularDataDto at index, shifting subsequent elements.
     * @warning Invalidates all views from dnv_vista_sdk_tsd_dto_tsd_tabular_at.
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tsd_tabular_remove(dnv_vista_sdk_tsd_dto_tsd_t* tsd, size_t index);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_dto_tsd_has_event(const dnv_vista_sdk_tsd_dto_tsd_t* tsd);
    /** @brief Non-owning view, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_event_t* dnv_vista_sdk_tsd_dto_tsd_get_event(
        dnv_vista_sdk_tsd_dto_tsd_t* tsd);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tsd_ensure_event(dnv_vista_sdk_tsd_dto_tsd_t* tsd);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tsd_clear_event(dnv_vista_sdk_tsd_dto_tsd_t* tsd);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_dto_tsd_has_custom_data_kinds(const dnv_vista_sdk_tsd_dto_tsd_t* tsd);
    /** @brief Non-owning view, or NULL if absent. Do NOT free. */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_tsd_dto_tsd_get_custom_data_kinds(
        dnv_vista_sdk_tsd_dto_tsd_t* tsd);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tsd_ensure_custom_data_kinds(dnv_vista_sdk_tsd_dto_tsd_t* tsd);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tsd_clear_custom_data_kinds(dnv_vista_sdk_tsd_dto_tsd_t* tsd);

    /*=========================================================================
     * TabularDataDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_dto_tabular_has_number_of_data_set(
        const dnv_vista_sdk_tsd_dto_tabular_t* t);
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_tsd_dto_tabular_get_number_of_data_set(const dnv_vista_sdk_tsd_dto_tabular_t* t);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tabular_set_number_of_data_set(
        dnv_vista_sdk_tsd_dto_tabular_t* t, size_t v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tabular_clear_number_of_data_set(dnv_vista_sdk_tsd_dto_tabular_t* t);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_dto_tabular_has_number_of_data_channel(
        const dnv_vista_sdk_tsd_dto_tabular_t* t);
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_tsd_dto_tabular_get_number_of_data_channel(const dnv_vista_sdk_tsd_dto_tabular_t* t);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tabular_set_number_of_data_channel(
        dnv_vista_sdk_tsd_dto_tabular_t* t, size_t v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tabular_clear_number_of_data_channel(
        dnv_vista_sdk_tsd_dto_tabular_t* t);

    /** @brief Number of DataChannelID strings. */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_tsd_dto_tabular_channel_id_count(const dnv_vista_sdk_tsd_dto_tabular_t* t);
    /** @return Borrowed C string, or NULL if index is out of range. */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_tabular_channel_id_at(
        const dnv_vista_sdk_tsd_dto_tabular_t* t, size_t index);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tabular_set_channel_ids(
        dnv_vista_sdk_tsd_dto_tabular_t* t, const char* const* ids, size_t count);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tabular_clear_channel_ids(dnv_vista_sdk_tsd_dto_tabular_t* t);

    /** @brief Number of TabularDataSetDto entries. */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_tsd_dto_tabular_data_set_count(const dnv_vista_sdk_tsd_dto_tabular_t* t);

    /**
     * @brief Non-owning view of TabularDataSetDto at index, or NULL if out of range. Do NOT free.
     * @warning Invalidated by a subsequent push or remove on the same tabular.
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_tab_set_t* dnv_vista_sdk_tsd_dto_tabular_data_set_at(
        dnv_vista_sdk_tsd_dto_tabular_t* t, size_t index);

    /**
     * @brief Append a default-constructed TabularDataSetDto. Returns non-owning view. Do NOT free.
     * @warning Invalidates all views from dnv_vista_sdk_tsd_dto_tabular_data_set_at.
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_tab_set_t* dnv_vista_sdk_tsd_dto_tabular_data_set_push(
        dnv_vista_sdk_tsd_dto_tabular_t* t);

    /**
     * @brief Remove the TabularDataSetDto at index, shifting subsequent elements.
     * @warning Invalidates all views from dnv_vista_sdk_tsd_dto_tabular_data_set_at.
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tabular_data_set_remove(
        dnv_vista_sdk_tsd_dto_tabular_t* t, size_t index);

    /*=========================================================================
     * TabularDataSetDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_tab_set_get_timestamp(
        const dnv_vista_sdk_tsd_dto_tab_set_t* s);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tab_set_set_timestamp(
        dnv_vista_sdk_tsd_dto_tab_set_t* s, const char* v);

    /** @brief Number of value strings. */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_tsd_dto_tab_set_value_count(const dnv_vista_sdk_tsd_dto_tab_set_t* s);
    /** @return Borrowed C string, or NULL if index is out of range. */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_tab_set_value_at(
        const dnv_vista_sdk_tsd_dto_tab_set_t* s, size_t index);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tab_set_set_values(
        dnv_vista_sdk_tsd_dto_tab_set_t* s, const char* const* values, size_t count);

    /** @brief Number of quality strings, or 0 if absent. */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_tsd_dto_tab_set_quality_count(const dnv_vista_sdk_tsd_dto_tab_set_t* s);
    /** @return Borrowed C string, or NULL if absent or index is out of range. */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_tab_set_quality_at(
        const dnv_vista_sdk_tsd_dto_tab_set_t* s, size_t index);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tab_set_set_quality(
        dnv_vista_sdk_tsd_dto_tab_set_t* s, const char* const* quality, size_t count);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_tab_set_clear_quality(dnv_vista_sdk_tsd_dto_tab_set_t* s);

    /*=========================================================================
     * EventDataDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_dto_event_has_number_of_data_set(const dnv_vista_sdk_tsd_dto_event_t* e);
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_tsd_dto_event_get_number_of_data_set(const dnv_vista_sdk_tsd_dto_event_t* e);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_event_set_number_of_data_set(
        dnv_vista_sdk_tsd_dto_event_t* e, size_t v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_event_clear_number_of_data_set(dnv_vista_sdk_tsd_dto_event_t* e);

    /** @brief Number of EventDataSetDto entries. */
    DNV_VISTA_SDK_C_API size_t dnv_vista_sdk_tsd_dto_event_data_set_count(const dnv_vista_sdk_tsd_dto_event_t* e);

    /**
     * @brief Non-owning view of EventDataSetDto at index, or NULL if out of range. Do NOT free.
     * @warning Invalidated by a subsequent push or remove on the same event.
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_event_set_t* dnv_vista_sdk_tsd_dto_event_data_set_at(
        dnv_vista_sdk_tsd_dto_event_t* e, size_t index);

    /**
     * @brief Append a default-constructed EventDataSetDto. Returns non-owning view. Do NOT free.
     * @warning Invalidates all views from dnv_vista_sdk_tsd_dto_event_data_set_at.
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_dto_event_set_t* dnv_vista_sdk_tsd_dto_event_data_set_push(
        dnv_vista_sdk_tsd_dto_event_t* e);

    /**
     * @brief Remove the EventDataSetDto at index, shifting subsequent elements.
     * @warning Invalidates all views from dnv_vista_sdk_tsd_dto_event_data_set_at.
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_event_data_set_remove(
        dnv_vista_sdk_tsd_dto_event_t* e, size_t index);

    /*=========================================================================
     * EventDataSetDto
     *=======================================================================*/

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_event_set_get_timestamp(
        const dnv_vista_sdk_tsd_dto_event_set_t* s);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_event_set_set_timestamp(
        dnv_vista_sdk_tsd_dto_event_set_t* s, const char* v);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_event_set_get_data_channel_id(
        const dnv_vista_sdk_tsd_dto_event_set_t* s);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_event_set_set_data_channel_id(
        dnv_vista_sdk_tsd_dto_event_set_t* s, const char* v);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_event_set_get_value(
        const dnv_vista_sdk_tsd_dto_event_set_t* s);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_event_set_set_value(
        dnv_vista_sdk_tsd_dto_event_set_t* s, const char* v);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_dto_event_set_has_quality(const dnv_vista_sdk_tsd_dto_event_set_t* s);
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_dto_event_set_get_quality(
        const dnv_vista_sdk_tsd_dto_event_set_t* s);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_event_set_set_quality(
        dnv_vista_sdk_tsd_dto_event_set_t* s, const char* v);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_dto_event_set_clear_quality(dnv_vista_sdk_tsd_dto_event_set_t* s);

#ifdef __cplusplus
}
#endif
