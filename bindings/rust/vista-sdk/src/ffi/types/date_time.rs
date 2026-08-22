use std::ffi::{c_char, c_int};

use super::time_span::dnv_vista_sdk_time_span_t;

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Copy)]
pub(crate) struct dnv_vista_sdk_date_time_t {
    pub(crate) ticks: i64,
}

#[allow(non_camel_case_types, dead_code)]
#[repr(C)]
pub(crate) enum dnv_vista_sdk_date_time_format_t {
    Iso8601 = 0,
    Iso8601Precise,
    Iso8601PreciseTrimmed,
    Iso8601Millis,
    Iso8601Micros,
    Iso8601Extended,
    Iso8601Basic,
    Iso8601Date,
    Iso8601Time,
    UnixSeconds,
    UnixMilliseconds,
}

extern "C" {
    pub(crate) fn dnv_vista_sdk_date_time_from_ticks(ticks: i64) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_from_date(
        year: i32,
        month: i32,
        day: i32,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_from_date_time(
        year: i32,
        month: i32,
        day: i32,
        hour: i32,
        minute: i32,
        second: i32,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_from_date_time_millis(
        year: i32,
        month: i32,
        day: i32,
        hour: i32,
        minute: i32,
        second: i32,
        millisecond: i32,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_from_string(
        iso8601_string: *const c_char,
        result: *mut dnv_vista_sdk_date_time_t,
    ) -> c_int;
    pub(crate) fn dnv_vista_sdk_date_time_year(dt: dnv_vista_sdk_date_time_t) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_month(dt: dnv_vista_sdk_date_time_t) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_day(dt: dnv_vista_sdk_date_time_t) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_hour(dt: dnv_vista_sdk_date_time_t) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_minute(dt: dnv_vista_sdk_date_time_t) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_second(dt: dnv_vista_sdk_date_time_t) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_millisecond(dt: dnv_vista_sdk_date_time_t) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_microsecond(dt: dnv_vista_sdk_date_time_t) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_nanosecond(dt: dnv_vista_sdk_date_time_t) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_day_of_week(dt: dnv_vista_sdk_date_time_t) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_day_of_year(dt: dnv_vista_sdk_date_time_t) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_to_epoch_seconds(dt: dnv_vista_sdk_date_time_t) -> i64;
    pub(crate) fn dnv_vista_sdk_date_time_to_epoch_milliseconds(
        dt: dnv_vista_sdk_date_time_t,
    ) -> i64;
    pub(crate) fn dnv_vista_sdk_date_time_date(
        dt: dnv_vista_sdk_date_time_t,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_time_of_day(
        dt: dnv_vista_sdk_date_time_t,
    ) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_date_time_add_days(
        dt: dnv_vista_sdk_date_time_t,
        days: f64,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_add_hours(
        dt: dnv_vista_sdk_date_time_t,
        hours: f64,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_add_minutes(
        dt: dnv_vista_sdk_date_time_t,
        minutes: f64,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_add_seconds(
        dt: dnv_vista_sdk_date_time_t,
        seconds: f64,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_add_milliseconds(
        dt: dnv_vista_sdk_date_time_t,
        milliseconds: f64,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_add_months(
        dt: dnv_vista_sdk_date_time_t,
        months: i32,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_add_years(
        dt: dnv_vista_sdk_date_time_t,
        years: i32,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_add_time_span(
        dt: dnv_vista_sdk_date_time_t,
        duration: dnv_vista_sdk_time_span_t,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_subtract_time_span(
        dt: dnv_vista_sdk_date_time_t,
        duration: dnv_vista_sdk_time_span_t,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_subtract(
        a: dnv_vista_sdk_date_time_t,
        b: dnv_vista_sdk_date_time_t,
    ) -> dnv_vista_sdk_time_span_t;
    pub(crate) fn dnv_vista_sdk_date_time_is_valid(dt: dnv_vista_sdk_date_time_t) -> c_int;
    pub(crate) fn dnv_vista_sdk_date_time_is_leap_year(year: i32) -> c_int;
    pub(crate) fn dnv_vista_sdk_date_time_days_in_month(year: i32, month: i32) -> i32;
    pub(crate) fn dnv_vista_sdk_date_time_utc_now() -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_min() -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_max() -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_epoch() -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_from_epoch_seconds(
        seconds: i64,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_from_epoch_milliseconds(
        milliseconds: i64,
    ) -> dnv_vista_sdk_date_time_t;
    pub(crate) fn dnv_vista_sdk_date_time_to_string(
        dt: dnv_vista_sdk_date_time_t,
        format: dnv_vista_sdk_date_time_format_t,
    ) -> *mut c_char;
}
