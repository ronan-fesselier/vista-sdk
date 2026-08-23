use std::ffi::{c_char, c_int};

use crate::ffi::core::imo_number::dnv_vista_sdk_imo_number_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_ship_id_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_ship_id_from_imo_number(
        imo_number: *const dnv_vista_sdk_imo_number_t,
    ) -> *mut dnv_vista_sdk_ship_id_t;
    pub(crate) fn dnv_vista_sdk_ship_id_from_other_id(
        other_id: *const c_char,
    ) -> *mut dnv_vista_sdk_ship_id_t;
    pub(crate) fn dnv_vista_sdk_ship_id_from_string(
        value: *const c_char,
    ) -> *mut dnv_vista_sdk_ship_id_t;
    pub(crate) fn dnv_vista_sdk_ship_id_free(ship_id: *mut dnv_vista_sdk_ship_id_t);
    pub(crate) fn dnv_vista_sdk_ship_id_is_imo_number(
        ship_id: *const dnv_vista_sdk_ship_id_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_ship_id_imo_number(
        ship_id: *const dnv_vista_sdk_ship_id_t,
    ) -> *mut dnv_vista_sdk_imo_number_t;
    pub(crate) fn dnv_vista_sdk_ship_id_other_id(
        ship_id: *const dnv_vista_sdk_ship_id_t,
    ) -> *const c_char;
}
