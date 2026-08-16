use std::ffi::{c_char, c_int};

use super::location::dnv_vista_sdk_location_t;
use super::location_group::dnv_vista_sdk_location_group_t;
use super::locations::dnv_vista_sdk_locations_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_location_builder_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_location_builder_create(
        locations: *const dnv_vista_sdk_locations_t,
    ) -> *mut dnv_vista_sdk_location_builder_t;
    pub(crate) fn dnv_vista_sdk_location_builder_free(
        builder: *mut dnv_vista_sdk_location_builder_t,
    );
    pub(crate) fn dnv_vista_sdk_location_builder_version(
        builder: *const dnv_vista_sdk_location_builder_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_location_builder_with_number(
        builder: *const dnv_vista_sdk_location_builder_t,
        number: c_int,
    ) -> *mut dnv_vista_sdk_location_builder_t;
    pub(crate) fn dnv_vista_sdk_location_builder_without_number(
        builder: *const dnv_vista_sdk_location_builder_t,
    ) -> *mut dnv_vista_sdk_location_builder_t;
    pub(crate) fn dnv_vista_sdk_location_builder_with_side(
        builder: *const dnv_vista_sdk_location_builder_t,
        side: c_char,
    ) -> *mut dnv_vista_sdk_location_builder_t;
    pub(crate) fn dnv_vista_sdk_location_builder_without_side(
        builder: *const dnv_vista_sdk_location_builder_t,
    ) -> *mut dnv_vista_sdk_location_builder_t;
    pub(crate) fn dnv_vista_sdk_location_builder_with_vertical(
        builder: *const dnv_vista_sdk_location_builder_t,
        vertical: c_char,
    ) -> *mut dnv_vista_sdk_location_builder_t;
    pub(crate) fn dnv_vista_sdk_location_builder_without_vertical(
        builder: *const dnv_vista_sdk_location_builder_t,
    ) -> *mut dnv_vista_sdk_location_builder_t;
    pub(crate) fn dnv_vista_sdk_location_builder_with_transverse(
        builder: *const dnv_vista_sdk_location_builder_t,
        transverse: c_char,
    ) -> *mut dnv_vista_sdk_location_builder_t;
    pub(crate) fn dnv_vista_sdk_location_builder_without_transverse(
        builder: *const dnv_vista_sdk_location_builder_t,
    ) -> *mut dnv_vista_sdk_location_builder_t;
    pub(crate) fn dnv_vista_sdk_location_builder_with_longitudinal(
        builder: *const dnv_vista_sdk_location_builder_t,
        longitudinal: c_char,
    ) -> *mut dnv_vista_sdk_location_builder_t;
    pub(crate) fn dnv_vista_sdk_location_builder_without_longitudinal(
        builder: *const dnv_vista_sdk_location_builder_t,
    ) -> *mut dnv_vista_sdk_location_builder_t;
    pub(crate) fn dnv_vista_sdk_location_builder_with_location(
        builder: *const dnv_vista_sdk_location_builder_t,
        location: *const dnv_vista_sdk_location_t,
    ) -> *mut dnv_vista_sdk_location_builder_t;
    pub(crate) fn dnv_vista_sdk_location_builder_without_value(
        builder: *const dnv_vista_sdk_location_builder_t,
        group: dnv_vista_sdk_location_group_t,
    ) -> *mut dnv_vista_sdk_location_builder_t;
    pub(crate) fn dnv_vista_sdk_location_builder_number(
        builder: *const dnv_vista_sdk_location_builder_t,
        out_number: *mut c_int,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_location_builder_side(
        builder: *const dnv_vista_sdk_location_builder_t,
        out_side: *mut c_char,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_location_builder_vertical(
        builder: *const dnv_vista_sdk_location_builder_t,
        out_vertical: *mut c_char,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_location_builder_transverse(
        builder: *const dnv_vista_sdk_location_builder_t,
        out_transverse: *mut c_char,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_location_builder_longitudinal(
        builder: *const dnv_vista_sdk_location_builder_t,
        out_longitudinal: *mut c_char,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_location_builder_with_code(
        builder: *const dnv_vista_sdk_location_builder_t,
        code: c_char,
    ) -> *mut dnv_vista_sdk_location_builder_t;
    pub(crate) fn dnv_vista_sdk_location_builder_build(
        builder: *const dnv_vista_sdk_location_builder_t,
    ) -> *mut dnv_vista_sdk_location_t;
    pub(crate) fn dnv_vista_sdk_location_builder_to_string(
        builder: *const dnv_vista_sdk_location_builder_t,
    ) -> *mut c_char;
}
