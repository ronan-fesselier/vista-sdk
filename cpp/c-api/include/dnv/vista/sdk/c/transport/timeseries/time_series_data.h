/**
 * @file time_series_data.h
 * @brief C API for dnv::vista::sdk::transport::timeseries (ISO 19848 TimeSeriesData domain model)
 * @details All types in this header are owned by value and must be released with their
 *          respective `_free` function, following the standard opaque-pointer pattern.
 *
 *          Prefixed `dnv_vista_sdk_tsd_*`. Two name collisions with earlier modules are
 *          resolved by this prefix alone:
 *          - `timeseries::TimeSpan` (Table 25: a DateTimeOffset start/end interval) is a
 *            different type from `dnv::vista::sdk::TimeSpan` (time_span.h, a duration in
 *            100ns ticks) - `dnv_vista_sdk_tsd_time_span_t` vs `dnv_vista_sdk_time_span_t`.
 *            Unlike the latter, `tsd_time_span` is NOT a trivial POD struct (it embeds two
 *            DateTimeOffset with non-trivial construction) and follows the normal opaque-
 *            pointer + _free pattern instead
 *          - `timeseries::ConfigurationReference` (Table 26: id + timeStamp) is a different
 *            type from `datachannel::ConfigurationReference` (Table 11: id + timeStamp +
 *            optional version, data_channel.h) - `dnv_vista_sdk_tsd_config_ref_t` vs
 *            `dnv_vista_sdk_dcl_configuration_reference_t`
 *          - `timeseries::Package`/`TimeSeriesDataPackage` vs `datachannel::Package`/
 *            `DataChannelListPackage` are likewise disambiguated by the `tsd_`/`dcl_` prefix
 *
 *          `TimeSeriesData::validate(dcPackage, onTabularData, onEventData)` is exposed via
 *          dnv_vista_sdk_tsd_time_series_data_validate() using a C callback per data stream
 *          (dnv_vista_sdk_tsd_validate_callback_t). The C++ implementation calls each callback
 *          once per data point and never stops early on a failed callback - it always visits
 *          every point and aggregates failures into the final ValidateResult. The callback
 *          return value (1 = valid, 0 = invalid) is what drives that aggregation, so it must
 *          be supplied honestly. See dnv_vista_sdk_tsd_validate_callback_t for the
 *          outErrorMessage lifetime contract
 */

#pragma once

#include <dnv/vista/sdk/c/Export.h>

#include "../../types/datetime/date_time_offset.h"
#include "../serialization/json/serializable_document.h"
#include "../datachannel/data_channel.h"
#include "../ship_id.h"

#include "data_channel_id.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct dnv_vista_sdk_tsd_time_span dnv_vista_sdk_tsd_time_span_t;
    typedef struct dnv_vista_sdk_tsd_config_ref dnv_vista_sdk_tsd_config_ref_t;
    typedef struct dnv_vista_sdk_tsd_header dnv_vista_sdk_tsd_header_t;
    typedef struct dnv_vista_sdk_tsd_tabular_data_set dnv_vista_sdk_tsd_tabular_data_set_t;
    typedef struct dnv_vista_sdk_tsd_tabular_data dnv_vista_sdk_tsd_tabular_data_t;
    typedef struct dnv_vista_sdk_tsd_event_data_set dnv_vista_sdk_tsd_event_data_set_t;
    typedef struct dnv_vista_sdk_tsd_event_data dnv_vista_sdk_tsd_event_data_t;
    typedef struct dnv_vista_sdk_tsd_time_series_data dnv_vista_sdk_tsd_time_series_data_t;
    typedef struct dnv_vista_sdk_tsd_package dnv_vista_sdk_tsd_package_t;
    typedef struct dnv_vista_sdk_tsd_data_package dnv_vista_sdk_tsd_data_package_t;

    /*=====================================================================
     * TimeSpan - Table 25
     *===================================================================*/

    /**
     * @brief Construct a TimeSpan from start/end timestamps
     * @return Owned handle, or NULL (with the last error message set) if `start` is after `end`
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_time_span_t* dnv_vista_sdk_tsd_time_span_create(
        dnv_vista_sdk_date_time_offset_t start, dnv_vista_sdk_date_time_offset_t end);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_time_span_free(dnv_vista_sdk_tsd_time_span_t* timeSpan);

    /** @brief Time stamp of the oldest Data Set */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_tsd_time_span_start(const dnv_vista_sdk_tsd_time_span_t* timeSpan);
    /** @brief Time stamp of the newest Data Set */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_tsd_time_span_end(const dnv_vista_sdk_tsd_time_span_t* timeSpan);
    /** @brief Sets the last error message and leaves `timeSpan` unchanged if `start` is after the current end */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_time_span_set_start(
        dnv_vista_sdk_tsd_time_span_t* timeSpan, dnv_vista_sdk_date_time_offset_t start);
    /** @brief Sets the last error message and leaves `timeSpan` unchanged if `end` is before the current start */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_time_span_set_end(
        dnv_vista_sdk_tsd_time_span_t* timeSpan, dnv_vista_sdk_date_time_offset_t end);

    /*=====================================================================
     * ConfigurationReference - Table 26
     *===================================================================*/

    /**
     * @brief Construct a ConfigurationReference
     * @return Owned handle, or NULL if `id` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_config_ref_t* dnv_vista_sdk_tsd_config_ref_create(
        const char* id, dnv_vista_sdk_date_time_offset_t timeStamp);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_config_ref_free(dnv_vista_sdk_tsd_config_ref_t* configRef);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_config_ref_id(const dnv_vista_sdk_tsd_config_ref_t* configRef);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_config_ref_set_id(
        dnv_vista_sdk_tsd_config_ref_t* configRef, const char* id);

    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_tsd_config_ref_timestamp(const dnv_vista_sdk_tsd_config_ref_t* configRef);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_config_ref_set_timestamp(
        dnv_vista_sdk_tsd_config_ref_t* configRef, dnv_vista_sdk_date_time_offset_t timeStamp);

    /*=====================================================================
     * Header - Table 24
     *===================================================================*/

    /**
     * @brief Construct a Header
     * @details Copies `shipId` - it remains owned by the caller
     * @return Owned handle, or NULL if `shipId` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_header_t* dnv_vista_sdk_tsd_header_create(
        const dnv_vista_sdk_ship_id_t* shipId);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_header_free(dnv_vista_sdk_tsd_header_t* header);

    /** @brief Borrowed pointer, valid as long as `header` is valid */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_ship_id_t* dnv_vista_sdk_tsd_header_ship_id(
        const dnv_vista_sdk_tsd_header_t* header);
    /** @brief Copies `shipId` into `header`. `shipId` remains owned by the caller */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_header_set_ship_id(
        dnv_vista_sdk_tsd_header_t* header, const dnv_vista_sdk_ship_id_t* shipId);

    /** @brief Borrowed pointer, valid as long as `header` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_tsd_time_span_t* dnv_vista_sdk_tsd_header_time_span(
        const dnv_vista_sdk_tsd_header_t* header);
    /** @brief Copies `timeSpan` into `header`. `timeSpan` remains owned by the caller */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_header_set_time_span(
        dnv_vista_sdk_tsd_header_t* header, const dnv_vista_sdk_tsd_time_span_t* timeSpan);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_header_clear_time_span(dnv_vista_sdk_tsd_header_t* header);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_header_has_date_created(const dnv_vista_sdk_tsd_header_t* header);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_tsd_header_date_created(const dnv_vista_sdk_tsd_header_t* header);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_header_set_date_created(
        dnv_vista_sdk_tsd_header_t* header, dnv_vista_sdk_date_time_offset_t dateCreated);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_header_clear_date_created(dnv_vista_sdk_tsd_header_t* header);

    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_header_has_date_modified(const dnv_vista_sdk_tsd_header_t* header);
    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_tsd_header_date_modified(const dnv_vista_sdk_tsd_header_t* header);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_header_set_date_modified(
        dnv_vista_sdk_tsd_header_t* header, dnv_vista_sdk_date_time_offset_t dateModified);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_header_clear_date_modified(dnv_vista_sdk_tsd_header_t* header);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_header_author(const dnv_vista_sdk_tsd_header_t* header);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_header_set_author(
        dnv_vista_sdk_tsd_header_t* header, const char* author);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_header_clear_author(dnv_vista_sdk_tsd_header_t* header);

    /** @brief Number of entries in the system configuration list, or 0 if `header` is NULL or unset */
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_tsd_header_system_configuration_count(const dnv_vista_sdk_tsd_header_t* header);
    /**
     * @brief Get a system configuration entry by index
     * @return Borrowed pointer, valid as long as `header` is valid, or NULL if `header`
     *         is NULL, unset, or `index` is out of range
     */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_tsd_config_ref_t* dnv_vista_sdk_tsd_header_system_configuration_at(
        const dnv_vista_sdk_tsd_header_t* header, size_t index);
    /**
     * @brief Set the system configuration list
     * @param header Header to modify
     * @param entries Array of handles, copied into `header`, may be NULL if `count` is 0
     * @param count Number of entries in `entries`
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_header_set_system_configuration(
        dnv_vista_sdk_tsd_header_t* header, const dnv_vista_sdk_tsd_config_ref_t* const* entries, size_t count);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_header_clear_system_configuration(dnv_vista_sdk_tsd_header_t* header);

    /** @brief Borrowed pointer, valid as long as `header` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_tsd_header_custom_headers(
        const dnv_vista_sdk_tsd_header_t* header);
    /** @brief Takes ownership of `value` - do not free it separately after this call */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_header_set_custom_headers(
        dnv_vista_sdk_tsd_header_t* header, dnv_vista_sdk_serializable_document_t* value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_header_clear_custom_headers(dnv_vista_sdk_tsd_header_t* header);

    /*=====================================================================
     * TabularDataSet - Table 30
     *===================================================================*/

    /**
     * @brief Construct a TabularDataSet
     * @param timeStamp Timestamp of this data set
     * @param values Array of null-terminated strings, copied into the data set
     * @param valueCount Number of entries in `values`
     * @return Owned handle, or NULL if `values` is NULL while `valueCount` is nonzero
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_tabular_data_set_t* dnv_vista_sdk_tsd_tabular_data_set_create(
        dnv_vista_sdk_date_time_offset_t timeStamp, const char* const* values, size_t valueCount);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_tabular_data_set_free(dnv_vista_sdk_tsd_tabular_data_set_t* dataSet);

    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_tsd_tabular_data_set_time_stamp(const dnv_vista_sdk_tsd_tabular_data_set_t* dataSet);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_tabular_data_set_set_time_stamp(
        dnv_vista_sdk_tsd_tabular_data_set_t* dataSet, dnv_vista_sdk_date_time_offset_t timeStamp);

    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_tsd_tabular_data_set_value_count(const dnv_vista_sdk_tsd_tabular_data_set_t* dataSet);
    /** @return Null-terminated string, owned by `dataSet`, or NULL if `index` is out of range */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_tabular_data_set_value_at(
        const dnv_vista_sdk_tsd_tabular_data_set_t* dataSet, size_t index);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_tabular_data_set_set_values(
        dnv_vista_sdk_tsd_tabular_data_set_t* dataSet, const char* const* values, size_t count);

    /** @brief Number of quality entries, or 0 if `dataSet` is NULL or unset */
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_tsd_tabular_data_set_quality_count(const dnv_vista_sdk_tsd_tabular_data_set_t* dataSet);
    /** @return Null-terminated string, owned by `dataSet`, or NULL if unset or `index` out of range */
    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_tabular_data_set_quality_at(
        const dnv_vista_sdk_tsd_tabular_data_set_t* dataSet, size_t index);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_tabular_data_set_set_quality(
        dnv_vista_sdk_tsd_tabular_data_set_t* dataSet, const char* const* quality, size_t count);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_tabular_data_set_clear_quality(
        dnv_vista_sdk_tsd_tabular_data_set_t* dataSet);

    /*=====================================================================
     * TabularData - Table 28
     *===================================================================*/

    /**
     * @brief Construct TabularData
     * @details Copies `dataChannelIds`/`dataSets` - both arrays remain owned by the caller
     * @return Owned handle, or NULL if `dataChannelIds`/`dataSets` is NULL while the
     *         corresponding count is nonzero
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_tabular_data_t* dnv_vista_sdk_tsd_tabular_data_create(
        const dnv_vista_sdk_tsd_channel_id_t* const* dataChannelIds,
        size_t dataChannelIdCount,
        const dnv_vista_sdk_tsd_tabular_data_set_t* const* dataSets,
        size_t dataSetCount);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_tabular_data_free(dnv_vista_sdk_tsd_tabular_data_t* tabularData);

    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_tsd_tabular_data_channel_id_count(const dnv_vista_sdk_tsd_tabular_data_t* tabularData);
    /** @return Borrowed pointer, or NULL if `index` is out of range */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_tsd_channel_id_t* dnv_vista_sdk_tsd_tabular_data_channel_id_at(
        const dnv_vista_sdk_tsd_tabular_data_t* tabularData, size_t index);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_tabular_data_set_channel_ids(
        dnv_vista_sdk_tsd_tabular_data_t* tabularData,
        const dnv_vista_sdk_tsd_channel_id_t* const* dataChannelIds,
        size_t count);

    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_tsd_tabular_data_data_set_count(const dnv_vista_sdk_tsd_tabular_data_t* tabularData);
    /** @return Borrowed pointer, or NULL if `index` is out of range */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_tsd_tabular_data_set_t* dnv_vista_sdk_tsd_tabular_data_data_set_at(
        const dnv_vista_sdk_tsd_tabular_data_t* tabularData, size_t index);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_tabular_data_set_data_sets(
        dnv_vista_sdk_tsd_tabular_data_t* tabularData,
        const dnv_vista_sdk_tsd_tabular_data_set_t* const* dataSets,
        size_t count);

    /**
     * @brief Validate that data channel/data set counts are consistent
     * @return 1 if valid, 0 otherwise (with the first validation error set via
     *         dnv_vista_sdk_last_error_message()), or if `tabularData` is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_tabular_data_validate(
        const dnv_vista_sdk_tsd_tabular_data_t* tabularData);

    /*=====================================================================
     * EventDataSet - Table 31
     *===================================================================*/

    /**
     * @brief Construct an EventDataSet
     * @details Copies `dataChannelId` - it remains owned by the caller
     * @return Owned handle, or NULL if `dataChannelId`/`value` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_event_data_set_t* dnv_vista_sdk_tsd_event_data_set_create(
        dnv_vista_sdk_date_time_offset_t timeStamp,
        const dnv_vista_sdk_tsd_channel_id_t* dataChannelId,
        const char* value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_event_data_set_free(dnv_vista_sdk_tsd_event_data_set_t* dataSet);

    DNV_VISTA_SDK_C_API dnv_vista_sdk_date_time_offset_t
    dnv_vista_sdk_tsd_event_data_set_time_stamp(const dnv_vista_sdk_tsd_event_data_set_t* dataSet);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_event_data_set_set_time_stamp(
        dnv_vista_sdk_tsd_event_data_set_t* dataSet, dnv_vista_sdk_date_time_offset_t timeStamp);

    /** @brief Borrowed pointer, valid as long as `dataSet` is valid */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_tsd_channel_id_t* dnv_vista_sdk_tsd_event_data_set_data_channel_id(
        const dnv_vista_sdk_tsd_event_data_set_t* dataSet);
    /** @brief Copies `dataChannelId` into `dataSet`. `dataChannelId` remains owned by the caller */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_event_data_set_set_data_channel_id(
        dnv_vista_sdk_tsd_event_data_set_t* dataSet, const dnv_vista_sdk_tsd_channel_id_t* dataChannelId);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_event_data_set_value(
        const dnv_vista_sdk_tsd_event_data_set_t* dataSet);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_event_data_set_set_value(
        dnv_vista_sdk_tsd_event_data_set_t* dataSet, const char* value);

    DNV_VISTA_SDK_C_API const char* dnv_vista_sdk_tsd_event_data_set_quality(
        const dnv_vista_sdk_tsd_event_data_set_t* dataSet);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_event_data_set_set_quality(
        dnv_vista_sdk_tsd_event_data_set_t* dataSet, const char* quality);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_event_data_set_clear_quality(
        dnv_vista_sdk_tsd_event_data_set_t* dataSet);

    /*=====================================================================
     * EventData - Table 29
     *===================================================================*/

    /** @brief Construct an empty EventData (no data sets) */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_event_data_t* dnv_vista_sdk_tsd_event_data_create(void);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_event_data_free(dnv_vista_sdk_tsd_event_data_t* eventData);

    /** @brief Number of data sets, or 0 if `eventData` is NULL or unset */
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_tsd_event_data_data_set_count(const dnv_vista_sdk_tsd_event_data_t* eventData);
    /** @return Borrowed pointer, or NULL if unset or `index` out of range */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_tsd_event_data_set_t* dnv_vista_sdk_tsd_event_data_data_set_at(
        const dnv_vista_sdk_tsd_event_data_t* eventData, size_t index);
    /**
     * @brief Set the data set list
     * @param eventData EventData to modify
     * @param dataSets Array of handles, copied into `eventData`, may be NULL if `count` is 0
     * @param count Number of entries in `dataSets`
     */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_event_data_set_data_set(
        dnv_vista_sdk_tsd_event_data_t* eventData,
        const dnv_vista_sdk_tsd_event_data_set_t* const* dataSets,
        size_t count);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_event_data_clear_data_set(dnv_vista_sdk_tsd_event_data_t* eventData);

    /*=====================================================================
     * TimeSeriesData - Table 27
     *===================================================================*/

    /** @brief Construct an empty TimeSeriesData (all fields unset) */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_time_series_data_t* dnv_vista_sdk_tsd_time_series_data_create(void);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_time_series_data_free(
        dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData);

    /** @brief Borrowed pointer, valid as long as `timeSeriesData` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_tsd_config_ref_t* dnv_vista_sdk_tsd_time_series_data_data_configuration(
        const dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData);
    /** @brief Copies `configRef` into `timeSeriesData`. `configRef` remains owned by the caller */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_time_series_data_set_data_configuration(
        dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData, const dnv_vista_sdk_tsd_config_ref_t* configRef);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_time_series_data_clear_data_configuration(
        dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData);

    /** @brief Number of tabular data entries, or 0 if `timeSeriesData` is NULL or unset */
    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_tsd_time_series_data_tabular_data_count(const dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData);
    /** @return Borrowed pointer, or NULL if unset or `index` out of range */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_tsd_tabular_data_t* dnv_vista_sdk_tsd_time_series_data_tabular_data_at(
        const dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData, size_t index);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_time_series_data_set_tabular_data(
        dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData,
        const dnv_vista_sdk_tsd_tabular_data_t* const* entries,
        size_t count);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_time_series_data_clear_tabular_data(
        dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData);

    /** @brief Borrowed pointer, valid as long as `timeSeriesData` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_tsd_event_data_t* dnv_vista_sdk_tsd_time_series_data_event_data(
        const dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData);
    /** @brief Copies `eventData` into `timeSeriesData`. `eventData` remains owned by the caller */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_time_series_data_set_event_data(
        dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData, const dnv_vista_sdk_tsd_event_data_t* eventData);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_time_series_data_clear_event_data(
        dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData);

    /** @brief Borrowed pointer, valid as long as `timeSeriesData` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_serializable_document_t*
    dnv_vista_sdk_tsd_time_series_data_custom_data_kinds(const dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData);
    /** @brief Takes ownership of `value` - do not free it separately after this call */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_time_series_data_set_custom_data_kinds(
        dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData, dnv_vista_sdk_serializable_document_t* value);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_time_series_data_clear_custom_data_kinds(
        dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData);

    /**
     * @brief Per-data-point custom validation callback for dnv_vista_sdk_tsd_time_series_data_validate
     * @param timeStamp Timestamp of the data point being validated
     * @param dataChannel Borrowed pointer to the resolved DataChannel, valid only for the
     *                     duration of the callback
     * @param value Borrowed pointer to the parsed Value, valid only for the duration of the callback
     * @param quality Optional quality indicator, may be NULL
     * @param outErrorMessage Optional out-param: if the callback returns 0, it may set
     *                         `*outErrorMessage` to a description of the failure. The pointed-to
     *                         string is only read by the caller before the callback returns
     *                         control - it is never read afterwards and never freed by the
     *                         caller, so a string literal or a thread-local/static buffer is a
     *                         valid choice. May be left untouched (defaults to a generic message)
     * @param userdata Opaque pointer forwarded from dnv_vista_sdk_tsd_time_series_data_validate
     * @return 1 if the data point passes custom validation, 0 otherwise
     * @details The C++ implementation calls this callback once per data point (tabular value or
     *          event) and never stops early on a 0 return - it always visits every point and
     *          aggregates every failure into the final validation result
     */
    typedef int (*dnv_vista_sdk_tsd_validate_callback_t)(
        dnv_vista_sdk_date_time_offset_t timeStamp,
        const dnv_vista_sdk_dcl_data_channel_t* dataChannel,
        const dnv_vista_sdk_iso19848_value_t* value,
        const char* quality,
        const char** outErrorMessage,
        void* userdata);

    /**
     * @brief Validate this TimeSeriesData against a DataChannelList package
     * @param timeSeriesData Must not be NULL
     * @param dcPackage DataChannelList package to validate against, must not be NULL
     * @param onTabularData Callback invoked for each tabular data point, must not be NULL
     * @param tabularUserdata Opaque pointer forwarded to `onTabularData`
     * @param onEventData Callback invoked for each event data point, must not be NULL
     * @param eventUserdata Opaque pointer forwarded to `onEventData`
     * @return 1 if valid, 0 otherwise (with the first validation error set via
     *         dnv_vista_sdk_last_error_message()), or if any required argument is NULL
     */
    DNV_VISTA_SDK_C_API int dnv_vista_sdk_tsd_time_series_data_validate(
        const dnv_vista_sdk_tsd_time_series_data_t* timeSeriesData,
        const dnv_vista_sdk_dcl_list_package_t* dcPackage,
        dnv_vista_sdk_tsd_validate_callback_t onTabularData,
        void* tabularUserdata,
        dnv_vista_sdk_tsd_validate_callback_t onEventData,
        void* eventUserdata);

    /*=====================================================================
     * Package - Table 23
     *===================================================================*/

    /**
     * @brief Construct a Package
     * @details Copies `timeSeriesData` entries - the array remains owned by the caller.
     *          `header` may be NULL (an unset, optional Header)
     * @return Owned handle, or NULL if `timeSeriesData` is NULL while `count` is nonzero
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_package_t* dnv_vista_sdk_tsd_package_create(
        const dnv_vista_sdk_tsd_header_t* header,
        const dnv_vista_sdk_tsd_time_series_data_t* const* timeSeriesData,
        size_t count);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_package_free(dnv_vista_sdk_tsd_package_t* package);

    /** @brief Borrowed pointer, valid as long as `package` is valid, or NULL if unset */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_tsd_header_t* dnv_vista_sdk_tsd_package_header(
        const dnv_vista_sdk_tsd_package_t* package);
    /** @brief Copies `header` into `package`. `header` remains owned by the caller. NULL clears it */
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_package_set_header(
        dnv_vista_sdk_tsd_package_t* package, const dnv_vista_sdk_tsd_header_t* header);

    DNV_VISTA_SDK_C_API size_t
    dnv_vista_sdk_tsd_package_time_series_data_count(const dnv_vista_sdk_tsd_package_t* package);
    /** @return Borrowed pointer, or NULL if `index` is out of range */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_tsd_time_series_data_t* dnv_vista_sdk_tsd_package_time_series_data_at(
        const dnv_vista_sdk_tsd_package_t* package, size_t index);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_package_set_time_series_data(
        dnv_vista_sdk_tsd_package_t* package, const dnv_vista_sdk_tsd_time_series_data_t* const* entries, size_t count);

    /*=====================================================================
     * TimeSeriesDataPackage
     *===================================================================*/

    /**
     * @brief Construct a TimeSeriesDataPackage
     * @details Copies `package` - it remains owned by the caller
     * @return Owned handle, or NULL if `package` is NULL
     */
    DNV_VISTA_SDK_C_API dnv_vista_sdk_tsd_data_package_t* dnv_vista_sdk_tsd_data_package_create(
        const dnv_vista_sdk_tsd_package_t* package);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_data_package_free(dnv_vista_sdk_tsd_data_package_t* dataPackage);

    /** @brief Borrowed pointer, valid as long as `dataPackage` is valid */
    DNV_VISTA_SDK_C_API const dnv_vista_sdk_tsd_package_t* dnv_vista_sdk_tsd_data_package_package(
        const dnv_vista_sdk_tsd_data_package_t* dataPackage);
    DNV_VISTA_SDK_C_API void dnv_vista_sdk_tsd_data_package_set_package(
        dnv_vista_sdk_tsd_data_package_t* dataPackage, const dnv_vista_sdk_tsd_package_t* package);

#ifdef __cplusplus
}
#endif
