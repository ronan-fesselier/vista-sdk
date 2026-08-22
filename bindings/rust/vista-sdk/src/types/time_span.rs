use std::ffi::CString;
use std::fmt;
use std::ops::{Add, Mul, Neg, Sub};

use crate::ffi::types::time_span as ffi;

/// A duration expressed in 100-nanosecond ticks.
#[repr(C)]
#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct TimeSpan {
    ticks: i64,
}

impl TimeSpan {
    /// Constructs a `TimeSpan` from a raw 100-nanosecond tick count.
    #[inline]
    pub fn from_ticks(ticks: i64) -> Self {
        Self {
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_from_ticks(ticks) }.ticks,
        }
    }

    /// Parses an ISO 8601 duration string (e.g. `"PT1H30M"`) or a numeric seconds string.
    ///
    /// Returns `None` if the string is invalid.
    pub fn from_str(s: &str) -> Option<Self> {
        let c = CString::new(s).ok()?;
        let mut result = ffi::dnv_vista_sdk_time_span_t { ticks: 0 };
        let ok = unsafe { ffi::dnv_vista_sdk_time_span_from_string(c.as_ptr(), &mut result) };
        if ok != 0 {
            Some(Self {
                ticks: result.ticks,
            })
        } else {
            None
        }
    }

    /// Constructs a `TimeSpan` from a number of days (fractional).
    #[inline]
    pub fn from_days(days: f64) -> Self {
        Self {
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_from_days(days) }.ticks,
        }
    }

    /// Constructs a `TimeSpan` from a number of hours (fractional).
    #[inline]
    pub fn from_hours(hours: f64) -> Self {
        Self {
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_from_hours(hours) }.ticks,
        }
    }

    /// Constructs a `TimeSpan` from a number of minutes (fractional).
    #[inline]
    pub fn from_minutes(minutes: f64) -> Self {
        Self {
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_from_minutes(minutes) }.ticks,
        }
    }

    /// Constructs a `TimeSpan` from a number of seconds (fractional).
    #[inline]
    pub fn from_seconds(seconds: f64) -> Self {
        Self {
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_from_seconds(seconds) }.ticks,
        }
    }

    /// Constructs a `TimeSpan` from a number of milliseconds (fractional).
    #[inline]
    pub fn from_millis(milliseconds: f64) -> Self {
        Self {
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_from_milliseconds(milliseconds) }.ticks,
        }
    }

    /// Constructs a `TimeSpan` from a number of microseconds (fractional).
    #[inline]
    pub fn from_micros(microseconds: f64) -> Self {
        Self {
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_from_microseconds(microseconds) }.ticks,
        }
    }

    /// Returns the raw 100-nanosecond tick count.
    #[inline]
    pub fn ticks(self) -> i64 {
        self.ticks
    }

    /// Total duration expressed in days (fractional).
    #[inline]
    pub fn days(self) -> f64 {
        unsafe { ffi::dnv_vista_sdk_time_span_days(self.into_ffi()) }
    }

    /// Total duration expressed in hours (fractional).
    #[inline]
    pub fn hours(self) -> f64 {
        unsafe { ffi::dnv_vista_sdk_time_span_hours(self.into_ffi()) }
    }

    /// Total duration expressed in minutes (fractional).
    #[inline]
    pub fn minutes(self) -> f64 {
        unsafe { ffi::dnv_vista_sdk_time_span_minutes(self.into_ffi()) }
    }

    /// Total duration expressed in seconds (fractional).
    #[inline]
    pub fn seconds(self) -> f64 {
        unsafe { ffi::dnv_vista_sdk_time_span_seconds(self.into_ffi()) }
    }

    /// Total duration expressed in milliseconds (fractional).
    #[inline]
    pub fn millis(self) -> f64 {
        unsafe { ffi::dnv_vista_sdk_time_span_milliseconds(self.into_ffi()) }
    }

    /// Total duration expressed in microseconds (fractional).
    #[inline]
    pub fn micros(self) -> f64 {
        unsafe { ffi::dnv_vista_sdk_time_span_microseconds(self.into_ffi()) }
    }

    /// Total duration expressed in nanoseconds (fractional).
    #[inline]
    pub fn nanos(self) -> f64 {
        unsafe { ffi::dnv_vista_sdk_time_span_nanoseconds(self.into_ffi()) }
    }

    /// Negates this duration.
    #[inline]
    pub fn negate(self) -> Self {
        Self {
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_negate(self.into_ffi()) }.ticks,
        }
    }

    /// Multiplies this duration by a scalar.
    #[inline]
    pub fn multiply(self, multiplier: f64) -> Self {
        Self {
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_multiply(self.into_ffi(), multiplier) }
                .ticks,
        }
    }

    /// Divides this duration by a scalar.
    #[inline]
    pub fn divide(self, divisor: f64) -> Self {
        Self {
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_divide(self.into_ffi(), divisor) }.ticks,
        }
    }

    /// Returns how many times `other` fits into `self`.
    #[inline]
    pub fn ratio(self, other: Self) -> f64 {
        unsafe { ffi::dnv_vista_sdk_time_span_ratio(self.into_ffi(), other.into_ffi()) }
    }

    #[inline]
    pub(crate) fn into_ffi(self) -> ffi::dnv_vista_sdk_time_span_t {
        ffi::dnv_vista_sdk_time_span_t { ticks: self.ticks }
    }

    #[inline]
    pub(crate) fn from_ffi(ffi: ffi::dnv_vista_sdk_time_span_t) -> Self {
        Self { ticks: ffi.ticks }
    }
}

impl Add for TimeSpan {
    type Output = Self;
    #[inline]
    fn add(self, rhs: Self) -> Self {
        Self {
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_add(self.into_ffi(), rhs.into_ffi()) }
                .ticks,
        }
    }
}

impl Sub for TimeSpan {
    type Output = Self;
    #[inline]
    fn sub(self, rhs: Self) -> Self {
        Self {
            ticks: unsafe {
                ffi::dnv_vista_sdk_time_span_subtract(self.into_ffi(), rhs.into_ffi())
            }
            .ticks,
        }
    }
}

impl Neg for TimeSpan {
    type Output = Self;
    #[inline]
    fn neg(self) -> Self {
        self.negate()
    }
}

impl Mul<f64> for TimeSpan {
    type Output = Self;
    #[inline]
    fn mul(self, rhs: f64) -> Self {
        self.multiply(rhs)
    }
}

impl fmt::Display for TimeSpan {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let ptr = unsafe { ffi::dnv_vista_sdk_time_span_to_string(self.into_ffi()) };
        if ptr.is_null() {
            return f.write_str("(null)");
        }
        let s = unsafe { std::ffi::CStr::from_ptr(ptr).to_string_lossy().into_owned() };
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        f.write_str(&s)
    }
}

impl fmt::Debug for TimeSpan {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "TimeSpan({})", self)
    }
}
