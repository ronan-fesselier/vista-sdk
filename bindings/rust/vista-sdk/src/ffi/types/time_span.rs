use std::ffi::c_char;

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Copy)]
pub(crate) struct dnv_vista_sdk_time_span_t {
    pub(crate) ticks: i64,
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_time_span_from_ticks(ticks: i64) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_time_span_from_string(
        str: *const c_char,
        result: *mut dnv_vista_sdk_time_span_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_time_span_from_days(days: f64) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_time_span_from_hours(hours: f64) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_time_span_from_minutes(minutes: f64) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_time_span_from_seconds(seconds: f64) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_time_span_from_milliseconds(
        milliseconds: f64,
    ) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_time_span_from_microseconds(
        microseconds: f64,
    ) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_time_span_days(ts: dnv_vista_sdk_time_span_t) -> f64;
    pub(crate) fn dnv_vista_sdk_time_span_hours(ts: dnv_vista_sdk_time_span_t) -> f64;
    pub(crate) fn dnv_vista_sdk_time_span_minutes(ts: dnv_vista_sdk_time_span_t) -> f64;
    pub(crate) fn dnv_vista_sdk_time_span_seconds(ts: dnv_vista_sdk_time_span_t) -> f64;
    pub(crate) fn dnv_vista_sdk_time_span_milliseconds(ts: dnv_vista_sdk_time_span_t) -> f64;
    pub(crate) fn dnv_vista_sdk_time_span_microseconds(ts: dnv_vista_sdk_time_span_t) -> f64;
    pub(crate) fn dnv_vista_sdk_time_span_nanoseconds(ts: dnv_vista_sdk_time_span_t) -> f64;
    pub(crate) fn dnv_vista_sdk_time_span_add(
        a: dnv_vista_sdk_time_span_t,
        b: dnv_vista_sdk_time_span_t,
    ) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_time_span_subtract(
        a: dnv_vista_sdk_time_span_t,
        b: dnv_vista_sdk_time_span_t,
    ) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_time_span_negate(
        ts: dnv_vista_sdk_time_span_t,
    ) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_time_span_multiply(
        ts: dnv_vista_sdk_time_span_t,
        multiplier: f64,
    ) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_time_span_divide(
        ts: dnv_vista_sdk_time_span_t,
        divisor: f64,
    ) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_time_span_ratio(
        a: dnv_vista_sdk_time_span_t,
        b: dnv_vista_sdk_time_span_t,
    ) -> f64;
    pub(crate) fn dnv_vista_sdk_time_span_to_string(ts: dnv_vista_sdk_time_span_t) -> *mut c_char;
}
