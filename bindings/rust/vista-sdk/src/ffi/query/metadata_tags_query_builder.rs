use std::ffi::{c_char, c_int};

use crate::ffi::core::codebook_name::dnv_vista_sdk_codebook_name_t;
use crate::ffi::core::local_id::dnv_vista_sdk_local_id_t;
use crate::ffi::core::metadata_tag::dnv_vista_sdk_metadata_tag_t;

use super::metadata_tags_query::dnv_vista_sdk_metadata_tags_query_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_metadata_tags_query_builder_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_metadata_tags_query_builder_create(
    ) -> *mut dnv_vista_sdk_metadata_tags_query_builder_t;
    pub(crate) fn dnv_vista_sdk_metadata_tags_query_builder_from(
        local_id: *const dnv_vista_sdk_local_id_t,
        allow_other_tags: c_int,
    ) -> *mut dnv_vista_sdk_metadata_tags_query_builder_t;
    pub(crate) fn dnv_vista_sdk_metadata_tags_query_builder_free(
        builder: *mut dnv_vista_sdk_metadata_tags_query_builder_t,
    );
    pub(crate) fn dnv_vista_sdk_metadata_tags_query_builder_with_tag(
        builder: *const dnv_vista_sdk_metadata_tags_query_builder_t,
        name: dnv_vista_sdk_codebook_name_t,
        value: *const c_char,
    ) -> *mut dnv_vista_sdk_metadata_tags_query_builder_t;
    pub(crate) fn dnv_vista_sdk_metadata_tags_query_builder_with_metadata_tag(
        builder: *const dnv_vista_sdk_metadata_tags_query_builder_t,
        tag: *const dnv_vista_sdk_metadata_tag_t,
    ) -> *mut dnv_vista_sdk_metadata_tags_query_builder_t;
    pub(crate) fn dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags(
        builder: *const dnv_vista_sdk_metadata_tags_query_builder_t,
        allow_others: c_int,
    ) -> *mut dnv_vista_sdk_metadata_tags_query_builder_t;
    pub(crate) fn dnv_vista_sdk_metadata_tags_query_builder_build(
        builder: *const dnv_vista_sdk_metadata_tags_query_builder_t,
    ) -> *mut dnv_vista_sdk_metadata_tags_query_t;
}
