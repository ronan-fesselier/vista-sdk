use std::ffi::c_char;

use super::location::dnv_vista_sdk_location_t;
use super::location_group::dnv_vista_sdk_location_group_t;
use super::parsing_errors::dnv_vista_sdk_parsing_errors_t;
use super::relative_location::dnv_vista_sdk_relative_location_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_locations_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_locations_version(
        locations: *const dnv_vista_sdk_locations_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_locations_relative_location_count(
        locations: *const dnv_vista_sdk_locations_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_locations_relative_location_at(
        locations: *const dnv_vista_sdk_locations_t,
        index: usize,
    ) -> *const dnv_vista_sdk_relative_location_t;
    pub(crate) fn dnv_vista_sdk_locations_group_count(
        locations: *const dnv_vista_sdk_locations_t,
        group: dnv_vista_sdk_location_group_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_locations_group_at(
        locations: *const dnv_vista_sdk_locations_t,
        group: dnv_vista_sdk_location_group_t,
        index: usize,
    ) -> *const dnv_vista_sdk_relative_location_t;
    pub(crate) fn dnv_vista_sdk_locations_parse(
        locations: *const dnv_vista_sdk_locations_t,
        location_str: *const c_char,
    ) -> *mut dnv_vista_sdk_location_t;
    pub(crate) fn dnv_vista_sdk_locations_parse_with_errors(
        locations: *const dnv_vista_sdk_locations_t,
        location_str: *const c_char,
        out_errors: *mut *mut dnv_vista_sdk_parsing_errors_t,
    ) -> *mut dnv_vista_sdk_location_t;
}
