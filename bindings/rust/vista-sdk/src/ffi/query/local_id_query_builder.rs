use std::ffi::c_char;

use crate::ffi::core::gmod_path::dnv_vista_sdk_gmod_path_t;
use crate::ffi::core::local_id::dnv_vista_sdk_local_id_t;

use super::gmod_path_query::dnv_vista_sdk_gmod_path_query_t;
use super::local_id_query::dnv_vista_sdk_local_id_query_t;
use super::metadata_tags_query::dnv_vista_sdk_metadata_tags_query_t;

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_local_id_query_builder_t {
    _opaque: [u8; 0],
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_create(
    ) -> *mut dnv_vista_sdk_local_id_query_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_from(
        local_id: *const dnv_vista_sdk_local_id_t,
    ) -> *mut dnv_vista_sdk_local_id_query_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_from_string(
        local_id_str: *const c_char,
    ) -> *mut dnv_vista_sdk_local_id_query_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_free(
        builder: *mut dnv_vista_sdk_local_id_query_builder_t,
    );
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_with_primary_item(
        builder: *const dnv_vista_sdk_local_id_query_builder_t,
        primary_item: *const dnv_vista_sdk_gmod_path_t,
    ) -> *mut dnv_vista_sdk_local_id_query_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_with_primary_item_query(
        builder: *const dnv_vista_sdk_local_id_query_builder_t,
        primary_item: *const dnv_vista_sdk_gmod_path_query_t,
    ) -> *mut dnv_vista_sdk_local_id_query_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_with_secondary_item(
        builder: *const dnv_vista_sdk_local_id_query_builder_t,
        secondary_item: *const dnv_vista_sdk_gmod_path_t,
    ) -> *mut dnv_vista_sdk_local_id_query_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_with_secondary_item_query(
        builder: *const dnv_vista_sdk_local_id_query_builder_t,
        secondary_item: *const dnv_vista_sdk_gmod_path_query_t,
    ) -> *mut dnv_vista_sdk_local_id_query_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_with_any_secondary_item(
        builder: *const dnv_vista_sdk_local_id_query_builder_t,
    ) -> *mut dnv_vista_sdk_local_id_query_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_without_secondary_item(
        builder: *const dnv_vista_sdk_local_id_query_builder_t,
    ) -> *mut dnv_vista_sdk_local_id_query_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_with_tags(
        builder: *const dnv_vista_sdk_local_id_query_builder_t,
        tags: *const dnv_vista_sdk_metadata_tags_query_t,
    ) -> *mut dnv_vista_sdk_local_id_query_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_without_locations(
        builder: *const dnv_vista_sdk_local_id_query_builder_t,
    ) -> *mut dnv_vista_sdk_local_id_query_builder_t;
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_primary_item(
        builder: *const dnv_vista_sdk_local_id_query_builder_t,
    ) -> *const dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_secondary_item(
        builder: *const dnv_vista_sdk_local_id_query_builder_t,
    ) -> *const dnv_vista_sdk_gmod_path_t;
    pub(crate) fn dnv_vista_sdk_local_id_query_builder_build(
        builder: *const dnv_vista_sdk_local_id_query_builder_t,
    ) -> *mut dnv_vista_sdk_local_id_query_t;
}
