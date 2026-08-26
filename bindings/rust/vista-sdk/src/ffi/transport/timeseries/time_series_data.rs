use crate::ffi::transport::datachannel::data_channel::{
    dnv_vista_sdk_dcl_data_channel_t, dnv_vista_sdk_dcl_list_package_t,
};
use crate::ffi::transport::iso19848::dnv_vista_sdk_iso19848_value_t;
use crate::ffi::transport::serializable_document::dnv_vista_sdk_serializable_document_t;
use crate::ffi::transport::ship_id::dnv_vista_sdk_ship_id_t;
use crate::ffi::transport::timeseries::data_channel_id::dnv_vista_sdk_tsd_channel_id_t;
use crate::ffi::types::date_time_offset::dnv_vista_sdk_date_time_offset_t;

#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_time_span_t {
    _opaque: [u8; 0],
}
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_config_ref_t {
    _opaque: [u8; 0],
}
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_header_t {
    _opaque: [u8; 0],
}
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_tabular_data_set_t {
    _opaque: [u8; 0],
}
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_tabular_data_t {
    _opaque: [u8; 0],
}
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_event_data_set_t {
    _opaque: [u8; 0],
}
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_event_data_t {
    _opaque: [u8; 0],
}
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_time_series_data_t {
    _opaque: [u8; 0],
}
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_package_t {
    _opaque: [u8; 0],
}
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_data_package_t {
    _opaque: [u8; 0],
}

pub(crate) type DnvVistaSdkTsdValidateCallbackT = unsafe extern "C" fn(
    time_stamp: dnv_vista_sdk_date_time_offset_t,
    data_channel: *const dnv_vista_sdk_dcl_data_channel_t,
    value: *const dnv_vista_sdk_iso19848_value_t,
    quality: *const std::ffi::c_char,
    out_error_message: *mut *const std::ffi::c_char,
    userdata: *mut std::ffi::c_void,
) -> std::ffi::c_int;

extern "C" {
    // TimeSpan
    pub(crate) fn dnv_vista_sdk_tsd_time_span_create(
        start: dnv_vista_sdk_date_time_offset_t,
        end: dnv_vista_sdk_date_time_offset_t,
    ) -> *mut dnv_vista_sdk_tsd_time_span_t;
    pub(crate) fn dnv_vista_sdk_tsd_time_span_free(time_span: *mut dnv_vista_sdk_tsd_time_span_t);
    pub(crate) fn dnv_vista_sdk_tsd_time_span_start(
        time_span: *const dnv_vista_sdk_tsd_time_span_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_tsd_time_span_end(
        time_span: *const dnv_vista_sdk_tsd_time_span_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_tsd_time_span_set_start(
        time_span: *mut dnv_vista_sdk_tsd_time_span_t,
        start: dnv_vista_sdk_date_time_offset_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_time_span_set_end(
        time_span: *mut dnv_vista_sdk_tsd_time_span_t,
        end: dnv_vista_sdk_date_time_offset_t,
    );

    // ConfigurationReference
    pub(crate) fn dnv_vista_sdk_tsd_config_ref_create(
        id: *const std::ffi::c_char,
        time_stamp: dnv_vista_sdk_date_time_offset_t,
    ) -> *mut dnv_vista_sdk_tsd_config_ref_t;
    pub(crate) fn dnv_vista_sdk_tsd_config_ref_free(
        config_ref: *mut dnv_vista_sdk_tsd_config_ref_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_config_ref_id(
        config_ref: *const dnv_vista_sdk_tsd_config_ref_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_tsd_config_ref_set_id(
        config_ref: *mut dnv_vista_sdk_tsd_config_ref_t,
        id: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_tsd_config_ref_timestamp(
        config_ref: *const dnv_vista_sdk_tsd_config_ref_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_tsd_config_ref_set_timestamp(
        config_ref: *mut dnv_vista_sdk_tsd_config_ref_t,
        time_stamp: dnv_vista_sdk_date_time_offset_t,
    );

    // Header
    pub(crate) fn dnv_vista_sdk_tsd_header_create(
        ship_id: *const dnv_vista_sdk_ship_id_t,
    ) -> *mut dnv_vista_sdk_tsd_header_t;
    pub(crate) fn dnv_vista_sdk_tsd_header_free(header: *mut dnv_vista_sdk_tsd_header_t);
    pub(crate) fn dnv_vista_sdk_tsd_header_ship_id(
        header: *const dnv_vista_sdk_tsd_header_t,
    ) -> *const dnv_vista_sdk_ship_id_t;
    pub(crate) fn dnv_vista_sdk_tsd_header_set_ship_id(
        header: *mut dnv_vista_sdk_tsd_header_t,
        ship_id: *const dnv_vista_sdk_ship_id_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_header_time_span(
        header: *const dnv_vista_sdk_tsd_header_t,
    ) -> *const dnv_vista_sdk_tsd_time_span_t;
    pub(crate) fn dnv_vista_sdk_tsd_header_set_time_span(
        header: *mut dnv_vista_sdk_tsd_header_t,
        time_span: *const dnv_vista_sdk_tsd_time_span_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_header_clear_time_span(header: *mut dnv_vista_sdk_tsd_header_t);
    pub(crate) fn dnv_vista_sdk_tsd_header_has_date_created(
        header: *const dnv_vista_sdk_tsd_header_t,
    ) -> std::ffi::c_int;
    pub(crate) fn dnv_vista_sdk_tsd_header_date_created(
        header: *const dnv_vista_sdk_tsd_header_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_tsd_header_set_date_created(
        header: *mut dnv_vista_sdk_tsd_header_t,
        date_created: dnv_vista_sdk_date_time_offset_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_header_clear_date_created(
        header: *mut dnv_vista_sdk_tsd_header_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_header_has_date_modified(
        header: *const dnv_vista_sdk_tsd_header_t,
    ) -> std::ffi::c_int;
    pub(crate) fn dnv_vista_sdk_tsd_header_date_modified(
        header: *const dnv_vista_sdk_tsd_header_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_tsd_header_set_date_modified(
        header: *mut dnv_vista_sdk_tsd_header_t,
        date_modified: dnv_vista_sdk_date_time_offset_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_header_clear_date_modified(
        header: *mut dnv_vista_sdk_tsd_header_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_header_author(
        header: *const dnv_vista_sdk_tsd_header_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_tsd_header_set_author(
        header: *mut dnv_vista_sdk_tsd_header_t,
        author: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_tsd_header_clear_author(header: *mut dnv_vista_sdk_tsd_header_t);
    pub(crate) fn dnv_vista_sdk_tsd_header_system_configuration_count(
        header: *const dnv_vista_sdk_tsd_header_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_tsd_header_system_configuration_at(
        header: *const dnv_vista_sdk_tsd_header_t,
        index: usize,
    ) -> *const dnv_vista_sdk_tsd_config_ref_t;
    pub(crate) fn dnv_vista_sdk_tsd_header_set_system_configuration(
        header: *mut dnv_vista_sdk_tsd_header_t,
        entries: *const *const dnv_vista_sdk_tsd_config_ref_t,
        count: usize,
    );
    pub(crate) fn dnv_vista_sdk_tsd_header_clear_system_configuration(
        header: *mut dnv_vista_sdk_tsd_header_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_header_custom_headers(
        header: *const dnv_vista_sdk_tsd_header_t,
    ) -> *const dnv_vista_sdk_serializable_document_t;
    pub(crate) fn dnv_vista_sdk_tsd_header_set_custom_headers(
        header: *mut dnv_vista_sdk_tsd_header_t,
        value: *mut dnv_vista_sdk_serializable_document_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_header_clear_custom_headers(
        header: *mut dnv_vista_sdk_tsd_header_t,
    );

    // TabularDataSet
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_set_create(
        time_stamp: dnv_vista_sdk_date_time_offset_t,
        values: *const *const std::ffi::c_char,
        value_count: usize,
    ) -> *mut dnv_vista_sdk_tsd_tabular_data_set_t;
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_set_free(
        data_set: *mut dnv_vista_sdk_tsd_tabular_data_set_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_set_time_stamp(
        data_set: *const dnv_vista_sdk_tsd_tabular_data_set_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_set_value_count(
        data_set: *const dnv_vista_sdk_tsd_tabular_data_set_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_set_value_at(
        data_set: *const dnv_vista_sdk_tsd_tabular_data_set_t,
        index: usize,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_set_set_values(
        data_set: *mut dnv_vista_sdk_tsd_tabular_data_set_t,
        values: *const *const std::ffi::c_char,
        count: usize,
    );
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_set_quality_count(
        data_set: *const dnv_vista_sdk_tsd_tabular_data_set_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_set_quality_at(
        data_set: *const dnv_vista_sdk_tsd_tabular_data_set_t,
        index: usize,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_set_set_quality(
        data_set: *mut dnv_vista_sdk_tsd_tabular_data_set_t,
        quality: *const *const std::ffi::c_char,
        count: usize,
    );
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_set_clear_quality(
        data_set: *mut dnv_vista_sdk_tsd_tabular_data_set_t,
    );

    // TabularData
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_create(
        data_channel_ids: *const *const dnv_vista_sdk_tsd_channel_id_t,
        data_channel_id_count: usize,
        data_sets: *const *const dnv_vista_sdk_tsd_tabular_data_set_t,
        data_set_count: usize,
    ) -> *mut dnv_vista_sdk_tsd_tabular_data_t;
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_free(
        tabular_data: *mut dnv_vista_sdk_tsd_tabular_data_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_channel_id_count(
        tabular_data: *const dnv_vista_sdk_tsd_tabular_data_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_channel_id_at(
        tabular_data: *const dnv_vista_sdk_tsd_tabular_data_t,
        index: usize,
    ) -> *const dnv_vista_sdk_tsd_channel_id_t;
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_data_set_count(
        tabular_data: *const dnv_vista_sdk_tsd_tabular_data_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_data_set_at(
        tabular_data: *const dnv_vista_sdk_tsd_tabular_data_t,
        index: usize,
    ) -> *const dnv_vista_sdk_tsd_tabular_data_set_t;
    pub(crate) fn dnv_vista_sdk_tsd_tabular_data_validate(
        tabular_data: *const dnv_vista_sdk_tsd_tabular_data_t,
    ) -> std::ffi::c_int;

    // EventDataSet
    pub(crate) fn dnv_vista_sdk_tsd_event_data_set_create(
        time_stamp: dnv_vista_sdk_date_time_offset_t,
        data_channel_id: *const dnv_vista_sdk_tsd_channel_id_t,
        value: *const std::ffi::c_char,
    ) -> *mut dnv_vista_sdk_tsd_event_data_set_t;
    pub(crate) fn dnv_vista_sdk_tsd_event_data_set_free(
        data_set: *mut dnv_vista_sdk_tsd_event_data_set_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_event_data_set_time_stamp(
        data_set: *const dnv_vista_sdk_tsd_event_data_set_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_tsd_event_data_set_data_channel_id(
        data_set: *const dnv_vista_sdk_tsd_event_data_set_t,
    ) -> *const dnv_vista_sdk_tsd_channel_id_t;
    pub(crate) fn dnv_vista_sdk_tsd_event_data_set_value(
        data_set: *const dnv_vista_sdk_tsd_event_data_set_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_tsd_event_data_set_quality(
        data_set: *const dnv_vista_sdk_tsd_event_data_set_t,
    ) -> *const std::ffi::c_char;
    pub(crate) fn dnv_vista_sdk_tsd_event_data_set_set_quality(
        data_set: *mut dnv_vista_sdk_tsd_event_data_set_t,
        quality: *const std::ffi::c_char,
    );
    pub(crate) fn dnv_vista_sdk_tsd_event_data_set_clear_quality(
        data_set: *mut dnv_vista_sdk_tsd_event_data_set_t,
    );

    // EventData
    pub(crate) fn dnv_vista_sdk_tsd_event_data_create() -> *mut dnv_vista_sdk_tsd_event_data_t;
    pub(crate) fn dnv_vista_sdk_tsd_event_data_free(
        event_data: *mut dnv_vista_sdk_tsd_event_data_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_event_data_data_set_count(
        event_data: *const dnv_vista_sdk_tsd_event_data_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_tsd_event_data_data_set_at(
        event_data: *const dnv_vista_sdk_tsd_event_data_t,
        index: usize,
    ) -> *const dnv_vista_sdk_tsd_event_data_set_t;
    pub(crate) fn dnv_vista_sdk_tsd_event_data_set_data_set(
        event_data: *mut dnv_vista_sdk_tsd_event_data_t,
        data_sets: *const *const dnv_vista_sdk_tsd_event_data_set_t,
        count: usize,
    );
    pub(crate) fn dnv_vista_sdk_tsd_event_data_clear_data_set(
        event_data: *mut dnv_vista_sdk_tsd_event_data_t,
    );

    // TimeSeriesData
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_create(
    ) -> *mut dnv_vista_sdk_tsd_time_series_data_t;
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_free(
        time_series_data: *mut dnv_vista_sdk_tsd_time_series_data_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_data_configuration(
        time_series_data: *const dnv_vista_sdk_tsd_time_series_data_t,
    ) -> *const dnv_vista_sdk_tsd_config_ref_t;
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_set_data_configuration(
        time_series_data: *mut dnv_vista_sdk_tsd_time_series_data_t,
        config_ref: *const dnv_vista_sdk_tsd_config_ref_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_clear_data_configuration(
        time_series_data: *mut dnv_vista_sdk_tsd_time_series_data_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_tabular_data_count(
        time_series_data: *const dnv_vista_sdk_tsd_time_series_data_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_tabular_data_at(
        time_series_data: *const dnv_vista_sdk_tsd_time_series_data_t,
        index: usize,
    ) -> *const dnv_vista_sdk_tsd_tabular_data_t;
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_set_tabular_data(
        time_series_data: *mut dnv_vista_sdk_tsd_time_series_data_t,
        entries: *const *const dnv_vista_sdk_tsd_tabular_data_t,
        count: usize,
    );
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_clear_tabular_data(
        time_series_data: *mut dnv_vista_sdk_tsd_time_series_data_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_event_data(
        time_series_data: *const dnv_vista_sdk_tsd_time_series_data_t,
    ) -> *const dnv_vista_sdk_tsd_event_data_t;
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_set_event_data(
        time_series_data: *mut dnv_vista_sdk_tsd_time_series_data_t,
        event_data: *const dnv_vista_sdk_tsd_event_data_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_clear_event_data(
        time_series_data: *mut dnv_vista_sdk_tsd_time_series_data_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_set_custom_data_kinds(
        time_series_data: *mut dnv_vista_sdk_tsd_time_series_data_t,
        value: *mut dnv_vista_sdk_serializable_document_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_clear_custom_data_kinds(
        time_series_data: *mut dnv_vista_sdk_tsd_time_series_data_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_time_series_data_validate(
        time_series_data: *const dnv_vista_sdk_tsd_time_series_data_t,
        dc_package: *const dnv_vista_sdk_dcl_list_package_t,
        on_tabular_data: DnvVistaSdkTsdValidateCallbackT,
        tabular_userdata: *mut std::ffi::c_void,
        on_event_data: DnvVistaSdkTsdValidateCallbackT,
        event_userdata: *mut std::ffi::c_void,
    ) -> std::ffi::c_int;

    // Package
    pub(crate) fn dnv_vista_sdk_tsd_package_create(
        header: *const dnv_vista_sdk_tsd_header_t,
        time_series_data: *const *const dnv_vista_sdk_tsd_time_series_data_t,
        count: usize,
    ) -> *mut dnv_vista_sdk_tsd_package_t;
    pub(crate) fn dnv_vista_sdk_tsd_package_free(package: *mut dnv_vista_sdk_tsd_package_t);
    pub(crate) fn dnv_vista_sdk_tsd_package_header(
        package: *const dnv_vista_sdk_tsd_package_t,
    ) -> *const dnv_vista_sdk_tsd_header_t;
    pub(crate) fn dnv_vista_sdk_tsd_package_set_header(
        package: *mut dnv_vista_sdk_tsd_package_t,
        header: *const dnv_vista_sdk_tsd_header_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_package_time_series_data_count(
        package: *const dnv_vista_sdk_tsd_package_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_tsd_package_time_series_data_at(
        package: *const dnv_vista_sdk_tsd_package_t,
        index: usize,
    ) -> *const dnv_vista_sdk_tsd_time_series_data_t;

    // TimeSeriesDataPackage
    pub(crate) fn dnv_vista_sdk_tsd_data_package_create(
        package: *const dnv_vista_sdk_tsd_package_t,
    ) -> *mut dnv_vista_sdk_tsd_data_package_t;
    pub(crate) fn dnv_vista_sdk_tsd_data_package_free(
        data_package: *mut dnv_vista_sdk_tsd_data_package_t,
    );
    pub(crate) fn dnv_vista_sdk_tsd_data_package_package(
        data_package: *const dnv_vista_sdk_tsd_data_package_t,
    ) -> *const dnv_vista_sdk_tsd_package_t;

    // JSON
    pub(crate) fn dnv_vista_sdk_tsd_data_package_from_json(
        json: *const std::ffi::c_char,
    ) -> *mut dnv_vista_sdk_tsd_data_package_t;
    pub(crate) fn dnv_vista_sdk_tsd_data_package_to_json(
        data_package: *const dnv_vista_sdk_tsd_data_package_t,
        pretty_print: std::ffi::c_int,
    ) -> *mut std::ffi::c_char;
}
