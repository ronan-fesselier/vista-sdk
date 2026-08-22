use std::ffi::{c_char, c_int};

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Copy)]
pub(crate) struct dnv_vista_sdk_decimal_t {
    pub(crate) flags: u32,
    pub(crate) mantissa: [u32; 3],
}

#[allow(non_camel_case_types, dead_code)]
#[repr(C)]
pub(crate) enum dnv_vista_sdk_decimal_rounding_mode_t {
    ToNearest = 0,
    ToNearestTiesAway,
    ToZero,
    ToPositiveInfinity,
    ToNegativeInfinity,
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_decimal_zero() -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_from_double(value: f64) -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_from_int64(value: i64) -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_from_uint64(value: u64) -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_from_string(
        str: *const c_char,
        result: *mut dnv_vista_sdk_decimal_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_decimal_scale(d: dnv_vista_sdk_decimal_t) -> u8;
    pub(crate) fn dnv_vista_sdk_decimal_add(
        a: dnv_vista_sdk_decimal_t,
        b: dnv_vista_sdk_decimal_t,
    ) -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_subtract(
        a: dnv_vista_sdk_decimal_t,
        b: dnv_vista_sdk_decimal_t,
    ) -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_multiply(
        a: dnv_vista_sdk_decimal_t,
        b: dnv_vista_sdk_decimal_t,
    ) -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_divide(
        a: dnv_vista_sdk_decimal_t,
        b: dnv_vista_sdk_decimal_t,
    ) -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_negate(
        d: dnv_vista_sdk_decimal_t,
    ) -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_abs(d: dnv_vista_sdk_decimal_t) -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_ceil(d: dnv_vista_sdk_decimal_t)
        -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_floor(
        d: dnv_vista_sdk_decimal_t,
    ) -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_trunc(
        d: dnv_vista_sdk_decimal_t,
    ) -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_round(
        d: dnv_vista_sdk_decimal_t,
        decimal_places_count: i32,
        mode: dnv_vista_sdk_decimal_rounding_mode_t,
    ) -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_sqrt(d: dnv_vista_sdk_decimal_t)
        -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_compare(
        a: dnv_vista_sdk_decimal_t,
        b: dnv_vista_sdk_decimal_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_decimal_equals(
        a: dnv_vista_sdk_decimal_t,
        b: dnv_vista_sdk_decimal_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_decimal_to_double(d: dnv_vista_sdk_decimal_t) -> f64;
    pub(crate) fn dnv_vista_sdk_decimal_to_bits(d: dnv_vista_sdk_decimal_t, bits: *mut i32);
    pub(crate) fn dnv_vista_sdk_decimal_decimal_places_count(d: dnv_vista_sdk_decimal_t) -> u8;
    pub(crate) fn dnv_vista_sdk_decimal_total_digits_count(d: dnv_vista_sdk_decimal_t) -> u32;
    pub(crate) fn dnv_vista_sdk_decimal_min() -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_max() -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_lowest() -> dnv_vista_sdk_decimal_t;
    pub(crate) fn dnv_vista_sdk_decimal_to_string(d: dnv_vista_sdk_decimal_t) -> *mut c_char;
}
