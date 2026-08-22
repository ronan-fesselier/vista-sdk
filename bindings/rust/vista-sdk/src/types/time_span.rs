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
            // SAFETY: no pointer arguments. Returns a plain struct by value.
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_from_ticks(ticks) }.ticks,
        }
    }

    /// Constructs a `TimeSpan` from a number of days (fractional).
    #[inline]
    pub fn from_days(days: f64) -> Self {
        Self {
            // SAFETY: no pointer arguments. Returns a plain struct by value.
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_from_days(days) }.ticks,
        }
    }

    /// Constructs a `TimeSpan` from a number of hours (fractional).
    #[inline]
    pub fn from_hours(hours: f64) -> Self {
        Self {
            // SAFETY: no pointer arguments. Returns a plain struct by value.
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_from_hours(hours) }.ticks,
        }
    }

    /// Constructs a `TimeSpan` from a number of minutes (fractional).
    #[inline]
    pub fn from_minutes(minutes: f64) -> Self {
        Self {
            // SAFETY: no pointer arguments. Returns a plain struct by value.
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_from_minutes(minutes) }.ticks,
        }
    }

    /// Constructs a `TimeSpan` from a number of seconds (fractional).
    #[inline]
    pub fn from_seconds(seconds: f64) -> Self {
        Self {
            // SAFETY: no pointer arguments. Returns a plain struct by value.
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_from_seconds(seconds) }.ticks,
        }
    }

    /// Constructs a `TimeSpan` from a number of milliseconds (fractional).
    #[inline]
    pub fn from_millis(milliseconds: f64) -> Self {
        Self {
            // SAFETY: no pointer arguments. Returns a plain struct by value.
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_from_milliseconds(milliseconds) }.ticks,
        }
    }

    /// Constructs a `TimeSpan` from a number of microseconds (fractional).
    #[inline]
    pub fn from_micros(microseconds: f64) -> Self {
        Self {
            // SAFETY: no pointer arguments. Returns a plain struct by value.
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
        // SAFETY: no pointer arguments. self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_time_span_days(self.into_ffi()) }
    }

    /// Total duration expressed in hours (fractional).
    #[inline]
    pub fn hours(self) -> f64 {
        // SAFETY: no pointer arguments. self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_time_span_hours(self.into_ffi()) }
    }

    /// Total duration expressed in minutes (fractional).
    #[inline]
    pub fn minutes(self) -> f64 {
        // SAFETY: no pointer arguments. self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_time_span_minutes(self.into_ffi()) }
    }

    /// Total duration expressed in seconds (fractional).
    #[inline]
    pub fn seconds(self) -> f64 {
        // SAFETY: no pointer arguments. self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_time_span_seconds(self.into_ffi()) }
    }

    /// Total duration expressed in milliseconds (fractional).
    #[inline]
    pub fn millis(self) -> f64 {
        // SAFETY: no pointer arguments. self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_time_span_milliseconds(self.into_ffi()) }
    }

    /// Total duration expressed in microseconds (fractional).
    #[inline]
    pub fn micros(self) -> f64 {
        // SAFETY: no pointer arguments. self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_time_span_microseconds(self.into_ffi()) }
    }

    /// Total duration expressed in nanoseconds (fractional).
    #[inline]
    pub fn nanos(self) -> f64 {
        // SAFETY: no pointer arguments. self.into_ffi() is a plain struct passed by value.
        unsafe { ffi::dnv_vista_sdk_time_span_nanoseconds(self.into_ffi()) }
    }

    /// Negates this duration.
    #[inline]
    pub fn negate(self) -> Self {
        Self {
            // SAFETY: no pointer arguments. self.into_ffi() is a plain struct passed by value.
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_negate(self.into_ffi()) }.ticks,
        }
    }

    /// Multiplies this duration by a scalar.
    #[inline]
    pub fn multiply(self, multiplier: f64) -> Self {
        Self {
            // SAFETY: no pointer arguments. self.into_ffi() is a plain struct passed by value.
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_multiply(self.into_ffi(), multiplier) }
                .ticks,
        }
    }

    /// Divides this duration by a scalar.
    #[inline]
    pub fn divide(self, divisor: f64) -> Self {
        Self {
            // SAFETY: no pointer arguments. self.into_ffi() is a plain struct passed by value.
            ticks: unsafe { ffi::dnv_vista_sdk_time_span_divide(self.into_ffi(), divisor) }.ticks,
        }
    }

    /// Returns how many times `other` fits into `self`.
    #[inline]
    pub fn ratio(self, other: Self) -> f64 {
        // SAFETY: no pointer arguments. self.into_ffi()/other.into_ffi() are plain structs passed by value.
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
            // SAFETY: no pointer arguments. self.into_ffi()/rhs.into_ffi() are plain structs passed by value.
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
            // SAFETY: no pointer arguments. self.into_ffi()/rhs.into_ffi() are plain structs passed by value.
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

impl TimeSpan {
    /// Converts to a `std::time::Duration`.
    ///
    /// Returns `None` if this `TimeSpan` is negative (negative durations have no `Duration` equivalent).
    pub fn to_duration(self) -> Option<std::time::Duration> {
        if self.ticks < 0 {
            return None;
        }
        let nanos = self.ticks as u128 * 100;
        Some(std::time::Duration::from_nanos(nanos as u64))
    }

    /// Constructs a `TimeSpan` from a `std::time::Duration`.
    pub fn from_duration(d: std::time::Duration) -> Self {
        Self::from_ticks((d.as_nanos() / 100) as i64)
    }
}

impl std::str::FromStr for TimeSpan {
    type Err = crate::core::error::VistaError;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let c = CString::new(s).map_err(|_| crate::core::error::VistaError {
            kind: crate::core::error::ErrorKind::InvalidArgument,
            message: "value contains a NUL byte".to_string(),
        })?;
        let mut result = ffi::dnv_vista_sdk_time_span_t { ticks: 0 };
        // SAFETY: c is a valid NUL-terminated C string, result is a valid writable pointer to a local.
        let ok = unsafe { ffi::dnv_vista_sdk_time_span_from_string(c.as_ptr(), &mut result) };
        if ok != 0 {
            Ok(Self {
                ticks: result.ticks,
            })
        } else {
            Err(crate::core::error::last_error())
        }
    }
}

impl fmt::Display for TimeSpan {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        // SAFETY: no pointer arguments. self.into_ffi() is a plain struct passed by value. Returns an owned pointer or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_time_span_to_string(self.into_ffi()) };
        if ptr.is_null() {
            return f.write_str("(null)");
        }
        // SAFETY: ptr is non-null (checked above) and owned until freed below.
        let s = unsafe { std::ffi::CStr::from_ptr(ptr).to_string_lossy().into_owned() };
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        f.write_str(&s)
    }
}

impl fmt::Debug for TimeSpan {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "TimeSpan({})", self)
    }
}
