use std::ffi::c_int;

use crate::ffi::core::local_id::dnv_vista_sdk_local_id_t;

use super::metadata_tags_query_builder::dnv_vista_sdk_metadata_tags_query_builder_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_metadata_tags_query_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_metadata_tags_query_free(
        query: *mut dnv_vista_sdk_metadata_tags_query_t,
    );
    pub(crate) fn dnv_vista_sdk_metadata_tags_query_match(
        query: *const dnv_vista_sdk_metadata_tags_query_t,
        local_id: *const dnv_vista_sdk_local_id_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_metadata_tags_query_builder(
        query: *const dnv_vista_sdk_metadata_tags_query_t,
    ) -> *const dnv_vista_sdk_metadata_tags_query_builder_t;
}
