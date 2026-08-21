use std::ffi::{c_char, c_int};

use crate::ffi::core::local_id::dnv_vista_sdk_local_id_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_local_id_query_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_local_id_query_free(query: *mut dnv_vista_sdk_local_id_query_t);
    pub(crate) fn dnv_vista_sdk_local_id_query_match(
        query: *const dnv_vista_sdk_local_id_query_t,
        local_id: *const dnv_vista_sdk_local_id_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_local_id_query_match_string(
        query: *const dnv_vista_sdk_local_id_query_t,
        local_id_str: *const c_char,
    ) -> c_int;
}
