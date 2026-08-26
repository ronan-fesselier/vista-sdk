use crate::ffi::transport::serializable_document::dnv_vista_sdk_serializable_document_t;
use crate::ffi::transport::timeseries::time_series_data::dnv_vista_sdk_tsd_data_package_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_dto_package {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_tsd_dto_package_t = dnv_vista_sdk_tsd_dto_package;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_dto_pkg {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_tsd_dto_pkg_t = dnv_vista_sdk_tsd_dto_pkg;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_dto_header {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_tsd_dto_header_t = dnv_vista_sdk_tsd_dto_header;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_dto_time_span {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_tsd_dto_time_span_t = dnv_vista_sdk_tsd_dto_time_span;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_dto_cfg_ref {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_tsd_dto_cfg_ref_t = dnv_vista_sdk_tsd_dto_cfg_ref;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_dto_tsd {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_tsd_dto_tsd_t = dnv_vista_sdk_tsd_dto_tsd;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_dto_tabular {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_tsd_dto_tabular_t = dnv_vista_sdk_tsd_dto_tabular;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_dto_tab_set {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_tsd_dto_tab_set_t = dnv_vista_sdk_tsd_dto_tab_set;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_dto_event {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_tsd_dto_event_t = dnv_vista_sdk_tsd_dto_event;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_tsd_dto_event_set {
    _opaque: [u8; 0],
}
#[allow(non_camel_case_types)]
pub(crate) type dnv_vista_sdk_tsd_dto_event_set_t = dnv_vista_sdk_tsd_dto_event_set;

extern "C" {
    // Root
    pub(crate) fn dnv_vista_sdk_tsd_to_dto(
        domain: *const dnv_vista_sdk_tsd_data_package_t,
    ) -> *mut dnv_vista_sdk_tsd_dto_package_t;

    pub(crate) fn dnv_vista_sdk_tsd_to_domain(
        dto: *const dnv_vista_sdk_tsd_dto_package_t,
    ) -> *mut dnv_vista_sdk_tsd_data_package_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_to_json(
        dto: *const dnv_vista_sdk_tsd_dto_package_t,
        pretty_print: std::ffi::c_int,
    ) -> *mut std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_from_json(
        json: *const std::ffi::c_char,
    ) -> *mut dnv_vista_sdk_tsd_dto_package_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_package_free(p: *mut dnv_vista_sdk_tsd_dto_package_t);

    pub(crate) fn dnv_vista_sdk_tsd_dto_package_get_pkg(
        p: *mut dnv_vista_sdk_tsd_dto_package_t,
    ) -> *mut dnv_vista_sdk_tsd_dto_pkg_t;

    // PackageDto
    pub(crate) fn dnv_vista_sdk_tsd_dto_pkg_has_header(
        pkg: *const dnv_vista_sdk_tsd_dto_pkg_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_tsd_dto_pkg_get_header(
        pkg: *mut dnv_vista_sdk_tsd_dto_pkg_t,
    ) -> *mut dnv_vista_sdk_tsd_dto_header_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_pkg_ensure_header(pkg: *mut dnv_vista_sdk_tsd_dto_pkg_t);

    pub(crate) fn dnv_vista_sdk_tsd_dto_pkg_clear_header(pkg: *mut dnv_vista_sdk_tsd_dto_pkg_t);

    pub(crate) fn dnv_vista_sdk_tsd_dto_pkg_tsd_count(
        pkg: *const dnv_vista_sdk_tsd_dto_pkg_t,
    ) -> usize;

    pub(crate) fn dnv_vista_sdk_tsd_dto_pkg_tsd_at(
        pkg: *mut dnv_vista_sdk_tsd_dto_pkg_t,
        index: usize,
    ) -> *mut dnv_vista_sdk_tsd_dto_tsd_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_pkg_tsd_push(
        pkg: *mut dnv_vista_sdk_tsd_dto_pkg_t,
    ) -> *mut dnv_vista_sdk_tsd_dto_tsd_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_pkg_tsd_remove(
        pkg: *mut dnv_vista_sdk_tsd_dto_pkg_t,
        index: usize,
    );

    // HeaderDto
    pub(crate) fn dnv_vista_sdk_tsd_dto_header_get_ship_id(
        h: *const dnv_vista_sdk_tsd_dto_header_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_set_ship_id(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_has_time_span(
        h: *const dnv_vista_sdk_tsd_dto_header_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_get_time_span(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
    ) -> *mut dnv_vista_sdk_tsd_dto_time_span_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_ensure_time_span(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_clear_time_span(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_has_date_created(
        h: *const dnv_vista_sdk_tsd_dto_header_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_get_date_created(
        h: *const dnv_vista_sdk_tsd_dto_header_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_set_date_created(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_clear_date_created(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_has_date_modified(
        h: *const dnv_vista_sdk_tsd_dto_header_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_get_date_modified(
        h: *const dnv_vista_sdk_tsd_dto_header_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_set_date_modified(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_clear_date_modified(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_has_author(
        h: *const dnv_vista_sdk_tsd_dto_header_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_get_author(
        h: *const dnv_vista_sdk_tsd_dto_header_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_set_author(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_clear_author(h: *mut dnv_vista_sdk_tsd_dto_header_t);

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_system_cfg_count(
        h: *const dnv_vista_sdk_tsd_dto_header_t,
    ) -> usize;

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_system_cfg_at(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
        index: usize,
    ) -> *mut dnv_vista_sdk_tsd_dto_cfg_ref_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_system_cfg_push(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
    ) -> *mut dnv_vista_sdk_tsd_dto_cfg_ref_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_system_cfg_remove(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
        index: usize,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_has_custom_headers(
        h: *const dnv_vista_sdk_tsd_dto_header_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_get_custom_headers(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
    ) -> *mut dnv_vista_sdk_serializable_document_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_ensure_custom_headers(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_header_clear_custom_headers(
        h: *mut dnv_vista_sdk_tsd_dto_header_t,
    );

    // TimeSpanDto
    pub(crate) fn dnv_vista_sdk_tsd_dto_time_span_get_start(
        ts: *const dnv_vista_sdk_tsd_dto_time_span_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_time_span_set_start(
        ts: *mut dnv_vista_sdk_tsd_dto_time_span_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_time_span_get_end(
        ts: *const dnv_vista_sdk_tsd_dto_time_span_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_time_span_set_end(
        ts: *mut dnv_vista_sdk_tsd_dto_time_span_t,
        v: *const std::ffi::c_char,
    );

    // ConfigurationReferenceDto
    pub(crate) fn dnv_vista_sdk_tsd_dto_cfg_ref_get_id(
        r: *const dnv_vista_sdk_tsd_dto_cfg_ref_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_cfg_ref_set_id(
        r: *mut dnv_vista_sdk_tsd_dto_cfg_ref_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_cfg_ref_get_timestamp(
        r: *const dnv_vista_sdk_tsd_dto_cfg_ref_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_cfg_ref_set_timestamp(
        r: *mut dnv_vista_sdk_tsd_dto_cfg_ref_t,
        v: *const std::ffi::c_char,
    );

    // TimeSeriesDataDto
    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_has_data_cfg(
        tsd: *const dnv_vista_sdk_tsd_dto_tsd_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_get_data_cfg(
        tsd: *mut dnv_vista_sdk_tsd_dto_tsd_t,
    ) -> *mut dnv_vista_sdk_tsd_dto_cfg_ref_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_ensure_data_cfg(tsd: *mut dnv_vista_sdk_tsd_dto_tsd_t);

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_clear_data_cfg(tsd: *mut dnv_vista_sdk_tsd_dto_tsd_t);

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_tabular_count(
        tsd: *const dnv_vista_sdk_tsd_dto_tsd_t,
    ) -> usize;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_tabular_at(
        tsd: *mut dnv_vista_sdk_tsd_dto_tsd_t,
        index: usize,
    ) -> *mut dnv_vista_sdk_tsd_dto_tabular_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_tabular_push(
        tsd: *mut dnv_vista_sdk_tsd_dto_tsd_t,
    ) -> *mut dnv_vista_sdk_tsd_dto_tabular_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_tabular_remove(
        tsd: *mut dnv_vista_sdk_tsd_dto_tsd_t,
        index: usize,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_has_event(
        tsd: *const dnv_vista_sdk_tsd_dto_tsd_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_get_event(
        tsd: *mut dnv_vista_sdk_tsd_dto_tsd_t,
    ) -> *mut dnv_vista_sdk_tsd_dto_event_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_ensure_event(tsd: *mut dnv_vista_sdk_tsd_dto_tsd_t);

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_clear_event(tsd: *mut dnv_vista_sdk_tsd_dto_tsd_t);

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_has_custom_data_kinds(
        tsd: *const dnv_vista_sdk_tsd_dto_tsd_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_get_custom_data_kinds(
        tsd: *mut dnv_vista_sdk_tsd_dto_tsd_t,
    ) -> *mut dnv_vista_sdk_serializable_document_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_ensure_custom_data_kinds(
        tsd: *mut dnv_vista_sdk_tsd_dto_tsd_t,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_tsd_clear_custom_data_kinds(
        tsd: *mut dnv_vista_sdk_tsd_dto_tsd_t,
    );

    // TabularDataDto
    pub(crate) fn dnv_vista_sdk_tsd_dto_tabular_channel_id_count(
        t: *const dnv_vista_sdk_tsd_dto_tabular_t,
    ) -> usize;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tabular_channel_id_at(
        t: *const dnv_vista_sdk_tsd_dto_tabular_t,
        index: usize,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tabular_set_channel_ids(
        t: *mut dnv_vista_sdk_tsd_dto_tabular_t,
        ids: *const *const std::ffi::c_char,
        count: usize,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_tabular_clear_channel_ids(
        t: *mut dnv_vista_sdk_tsd_dto_tabular_t,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_tabular_data_set_count(
        t: *const dnv_vista_sdk_tsd_dto_tabular_t,
    ) -> usize;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tabular_data_set_at(
        t: *mut dnv_vista_sdk_tsd_dto_tabular_t,
        index: usize,
    ) -> *mut dnv_vista_sdk_tsd_dto_tab_set_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tabular_data_set_push(
        t: *mut dnv_vista_sdk_tsd_dto_tabular_t,
    ) -> *mut dnv_vista_sdk_tsd_dto_tab_set_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tabular_data_set_remove(
        t: *mut dnv_vista_sdk_tsd_dto_tabular_t,
        index: usize,
    );

    // TabularDataSetDto
    pub(crate) fn dnv_vista_sdk_tsd_dto_tab_set_get_timestamp(
        s: *const dnv_vista_sdk_tsd_dto_tab_set_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tab_set_set_timestamp(
        s: *mut dnv_vista_sdk_tsd_dto_tab_set_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_tab_set_value_count(
        s: *const dnv_vista_sdk_tsd_dto_tab_set_t,
    ) -> usize;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tab_set_value_at(
        s: *const dnv_vista_sdk_tsd_dto_tab_set_t,
        index: usize,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tab_set_set_values(
        s: *mut dnv_vista_sdk_tsd_dto_tab_set_t,
        values: *const *const std::ffi::c_char,
        count: usize,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_tab_set_quality_count(
        s: *const dnv_vista_sdk_tsd_dto_tab_set_t,
    ) -> usize;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tab_set_quality_at(
        s: *const dnv_vista_sdk_tsd_dto_tab_set_t,
        index: usize,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_tab_set_set_quality(
        s: *mut dnv_vista_sdk_tsd_dto_tab_set_t,
        quality: *const *const std::ffi::c_char,
        count: usize,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_tab_set_clear_quality(
        s: *mut dnv_vista_sdk_tsd_dto_tab_set_t,
    );

    // EventDataDto
    pub(crate) fn dnv_vista_sdk_tsd_dto_event_data_set_count(
        e: *const dnv_vista_sdk_tsd_dto_event_t,
    ) -> usize;

    pub(crate) fn dnv_vista_sdk_tsd_dto_event_data_set_at(
        e: *mut dnv_vista_sdk_tsd_dto_event_t,
        index: usize,
    ) -> *mut dnv_vista_sdk_tsd_dto_event_set_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_event_data_set_push(
        e: *mut dnv_vista_sdk_tsd_dto_event_t,
    ) -> *mut dnv_vista_sdk_tsd_dto_event_set_t;

    pub(crate) fn dnv_vista_sdk_tsd_dto_event_data_set_remove(
        e: *mut dnv_vista_sdk_tsd_dto_event_t,
        index: usize,
    );

    // EventDataSetDto
    pub(crate) fn dnv_vista_sdk_tsd_dto_event_set_get_timestamp(
        s: *const dnv_vista_sdk_tsd_dto_event_set_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_event_set_set_timestamp(
        s: *mut dnv_vista_sdk_tsd_dto_event_set_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_event_set_get_data_channel_id(
        s: *const dnv_vista_sdk_tsd_dto_event_set_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_event_set_set_data_channel_id(
        s: *mut dnv_vista_sdk_tsd_dto_event_set_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_event_set_get_value(
        s: *const dnv_vista_sdk_tsd_dto_event_set_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_event_set_set_value(
        s: *mut dnv_vista_sdk_tsd_dto_event_set_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_event_set_has_quality(
        s: *const dnv_vista_sdk_tsd_dto_event_set_t,
    ) -> std::ffi::c_int;

    pub(crate) fn dnv_vista_sdk_tsd_dto_event_set_get_quality(
        s: *const dnv_vista_sdk_tsd_dto_event_set_t,
    ) -> *const std::ffi::c_char;

    pub(crate) fn dnv_vista_sdk_tsd_dto_event_set_set_quality(
        s: *mut dnv_vista_sdk_tsd_dto_event_set_t,
        v: *const std::ffi::c_char,
    );

    pub(crate) fn dnv_vista_sdk_tsd_dto_event_set_clear_quality(
        s: *mut dnv_vista_sdk_tsd_dto_event_set_t,
    );
}
