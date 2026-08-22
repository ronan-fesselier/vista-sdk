use std::ffi::{c_char, c_int};

use super::date_time::{dnv_vista_sdk_date_time_format_t, dnv_vista_sdk_date_time_t};
use super::time_span::dnv_vista_sdk_time_span_t;

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Copy)]
pub(crate) struct dnv_vista_sdk_date_time_offset_t {
    pub(crate) date_time: dnv_vista_sdk_date_time_t,
    pub(crate) offset: dnv_vista_sdk_time_span_t,
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_date_time_offset_create(
        date_time: dnv_vista_sdk_date_time_t,
        offset: dnv_vista_sdk_time_span_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_from_date_time(
        date_time: dnv_vista_sdk_date_time_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_from_ticks(
        ticks: i64,
        offset: dnv_vista_sdk_time_span_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_from_string(
        iso8601_string: *const c_char,
        result: *mut dnv_vista_sdk_date_time_offset_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_date_time_offset_date_time(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_offset(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_utc_date_time(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_local_date_time(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_ticks(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> i64;
    pub(crate) fn dnv_vista_sdk_date_time_offset_utc_ticks(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> i64;
    pub(crate) fn dnv_vista_sdk_date_time_offset_year(dto: dnv_vista_sdk_date_time_offset_t)
        -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_offset_month(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_offset_day(dto: dnv_vista_sdk_date_time_offset_t) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_offset_hour(dto: dnv_vista_sdk_date_time_offset_t)
        -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_offset_minute(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_offset_second(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_offset_millisecond(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_offset_microsecond(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_offset_nanosecond(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_offset_day_of_week(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_offset_day_of_year(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_offset_total_offset_minutes(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_offset_to_epoch_seconds(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> i64;
    pub(crate) fn dnv_vista_sdk_date_time_offset_to_epoch_milliseconds(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> i64;
    pub(crate) fn dnv_vista_sdk_date_time_offset_date(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_time_of_day(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_to_offset(
        dto: dnv_vista_sdk_date_time_offset_t,
        new_offset: dnv_vista_sdk_time_span_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_to_universal_time(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_to_local_time(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_to_filetime(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> i64;
    pub(crate) fn dnv_vista_sdk_date_time_offset_add_time_span(
        dto: dnv_vista_sdk_date_time_offset_t,
        value: dnv_vista_sdk_time_span_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_add_days(
        dto: dnv_vista_sdk_date_time_offset_t,
        days: f64,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_add_hours(
        dto: dnv_vista_sdk_date_time_offset_t,
        hours: f64,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_add_minutes(
        dto: dnv_vista_sdk_date_time_offset_t,
        minutes: f64,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_add_seconds(
        dto: dnv_vista_sdk_date_time_offset_t,
        seconds: f64,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_add_milliseconds(
        dto: dnv_vista_sdk_date_time_offset_t,
        milliseconds: f64,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_add_months(
        dto: dnv_vista_sdk_date_time_offset_t,
        months: i32,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_add_years(
        dto: dnv_vista_sdk_date_time_offset_t,
        years: i32,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_add_ticks(
        dto: dnv_vista_sdk_date_time_offset_t,
        ticks: i64,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_subtract(
        a: dnv_vista_sdk_date_time_offset_t,
        b: dnv_vista_sdk_date_time_offset_t,
    ) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_subtract_time_span(
        dto: dnv_vista_sdk_date_time_offset_t,
        value: dnv_vista_sdk_time_span_t,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_equals(
        a: dnv_vista_sdk_date_time_offset_t,
        b: dnv_vista_sdk_date_time_offset_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_date_time_offset_equals_exact(
        a: dnv_vista_sdk_date_time_offset_t,
        b: dnv_vista_sdk_date_time_offset_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_date_time_offset_compare(
        a: dnv_vista_sdk_date_time_offset_t,
        b: dnv_vista_sdk_date_time_offset_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_date_time_offset_is_valid(
        dto: dnv_vista_sdk_date_time_offset_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_date_time_offset_now() -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_utc_now() -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_today() -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_min() -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_max() -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_epoch() -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_from_epoch_seconds(
        seconds: i64,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_from_epoch_milliseconds(
        milliseconds: i64,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_from_filetime(
        filetime: i64,
    ) -> dnv_vista_sdk_date_time_offset_t;
    pub(crate) fn dnv_vista_sdk_date_time_offset_to_string(
        dto: dnv_vista_sdk_date_time_offset_t,
        format: dnv_vista_sdk_date_time_format_t,
    ) -> *mut c_char;
}
