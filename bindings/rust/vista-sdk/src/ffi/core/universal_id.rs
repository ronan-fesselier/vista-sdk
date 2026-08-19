use std::ffi::{c_char, c_int};

use super::local_id::dnv_vista_sdk_local_id_t;
use super::universal_id_builder::dnv_vista_sdk_universal_id_builder_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_universal_id_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_universal_id_naming_entity() -> *const c_char;
    pub(crate) fn dnv_vista_sdk_universal_id_free(universal_id: *mut dnv_vista_sdk_universal_id_t);
    pub(crate) fn dnv_vista_sdk_universal_id_from_string(
        universal_id_str: *const c_char,
    ) -> *mut dnv_vista_sdk_universal_id_t;
    pub(crate) fn dnv_vista_sdk_universal_id_local_id(
        universal_id: *const dnv_vista_sdk_universal_id_t,
    ) -> *const dnv_vista_sdk_local_id_t;
    pub(crate) fn dnv_vista_sdk_universal_id_builder(
        universal_id: *const dnv_vista_sdk_universal_id_t,
    ) -> *const dnv_vista_sdk_universal_id_builder_t;
    pub(crate) fn dnv_vista_sdk_universal_id_equals(
        a: *const dnv_vista_sdk_universal_id_t,
        b: *const dnv_vista_sdk_universal_id_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_universal_id_to_string(
        universal_id: *const dnv_vista_sdk_universal_id_t,
    ) -> *mut c_char;
}
