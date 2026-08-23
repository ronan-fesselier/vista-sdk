use std::ffi::{c_char, c_int};

#[allow(non_camel_case_types)]
#[repr(C)]
pub(crate) struct dnv_vista_sdk_serializable_document_t {
    _opaque: [u8; 0],
}

#[allow(non_camel_case_types, dead_code)]
#[repr(C)]
pub(crate) enum dnv_vista_sdk_serializable_document_kind_t {
    Null = 0,
    Boolean,
    Integer,
    Double,
    String,
    Array,
    Object,
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_serializable_document_null(
    ) -> *mut dnv_vista_sdk_serializable_document_t;
    pub(crate) fn dnv_vista_sdk_serializable_document_from_boolean(
        value: c_int,
    ) -> *mut dnv_vista_sdk_serializable_document_t;
    pub(crate) fn dnv_vista_sdk_serializable_document_from_integer(
        value: i64,
    ) -> *mut dnv_vista_sdk_serializable_document_t;
    pub(crate) fn dnv_vista_sdk_serializable_document_from_double(
        value: f64,
    ) -> *mut dnv_vista_sdk_serializable_document_t;
    pub(crate) fn dnv_vista_sdk_serializable_document_from_string(
        value: *const c_char,
    ) -> *mut dnv_vista_sdk_serializable_document_t;
    pub(crate) fn dnv_vista_sdk_serializable_document_array(
    ) -> *mut dnv_vista_sdk_serializable_document_t;
    pub(crate) fn dnv_vista_sdk_serializable_document_object(
    ) -> *mut dnv_vista_sdk_serializable_document_t;
    pub(crate) fn dnv_vista_sdk_serializable_document_free(
        doc: *mut dnv_vista_sdk_serializable_document_t,
    );
    pub(crate) fn dnv_vista_sdk_serializable_document_clone(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> *mut dnv_vista_sdk_serializable_document_t;
    pub(crate) fn dnv_vista_sdk_serializable_document_equals(
        a: *const dnv_vista_sdk_serializable_document_t,
        b: *const dnv_vista_sdk_serializable_document_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_serializable_document_kind(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> dnv_vista_sdk_serializable_document_kind_t;
    pub(crate) fn dnv_vista_sdk_serializable_document_is_null(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_serializable_document_is_boolean(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_serializable_document_is_integer(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_serializable_document_is_double(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_serializable_document_is_string(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_serializable_document_is_array(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_serializable_document_is_object(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_serializable_document_as_boolean(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_serializable_document_as_integer(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> i64;
    pub(crate) fn dnv_vista_sdk_serializable_document_as_double(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> f64;
    pub(crate) fn dnv_vista_sdk_serializable_document_as_string(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_serializable_document_array_size(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_serializable_document_array_at(
        doc: *const dnv_vista_sdk_serializable_document_t,
        index: usize,
    ) -> *const dnv_vista_sdk_serializable_document_t;
    pub(crate) fn dnv_vista_sdk_serializable_document_object_size(
        doc: *const dnv_vista_sdk_serializable_document_t,
    ) -> usize;
    pub(crate) fn dnv_vista_sdk_serializable_document_object_key_at(
        doc: *const dnv_vista_sdk_serializable_document_t,
        index: usize,
    ) -> *const c_char;
    pub(crate) fn dnv_vista_sdk_serializable_document_object_value_at(
        doc: *const dnv_vista_sdk_serializable_document_t,
        index: usize,
    ) -> *const dnv_vista_sdk_serializable_document_t;
    pub(crate) fn dnv_vista_sdk_serializable_document_find(
        doc: *const dnv_vista_sdk_serializable_document_t,
        key: *const c_char,
    ) -> *const dnv_vista_sdk_serializable_document_t;
    pub(crate) fn dnv_vista_sdk_serializable_document_contains(
        doc: *const dnv_vista_sdk_serializable_document_t,
        key: *const c_char,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_serializable_document_set(
        doc: *mut dnv_vista_sdk_serializable_document_t,
        key: *const c_char,
        value: *mut dnv_vista_sdk_serializable_document_t,
    );
    pub(crate) fn dnv_vista_sdk_serializable_document_push_back(
        doc: *mut dnv_vista_sdk_serializable_document_t,
        value: *mut dnv_vista_sdk_serializable_document_t,
    );
}
