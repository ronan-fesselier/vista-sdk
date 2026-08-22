use std::ffi::CString;
use std::fmt;
use std::ops::{Add, AddAssign, Div, Mul, Neg, Sub, SubAssign};

use crate::ffi::types::decimal as ffi;

/// Rounding mode used by [`Decimal::round`].
#[repr(C)]
pub enum RoundingMode {
    ToNearest = 0,
    ToNearestTiesAway,
    ToZero,
    ToPositiveInfinity,
    ToNegativeInfinity,
}

/// A fixed-point decimal number with up to 28 significant digits.
///
/// Mirrors `System.Decimal` / `dnv::vista::sdk::Decimal` - passed by value, no heap allocation.
/// Use [`Decimal::from_str`] for exact construction; [`Decimal::from_f64`] may lose precision.
#[repr(C)]
#[derive(Clone, Copy)]
pub struct Decimal {
    flags: u32,
    mantissa: [u32; 3],
}

impl Decimal {
    /// Zero.
    #[inline]
    pub fn zero() -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_decimal_zero() })
    }

    /// Smallest positive value (1 × 10⁻²⁸).
    #[inline]
    pub fn min() -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_decimal_min() })
    }

    /// Maximum value.
    #[inline]
    pub fn max() -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_decimal_max() })
    }

    /// Most negative value.
    #[inline]
    pub fn lowest() -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_decimal_lowest() })
    }

    /// Constructs from a `f64`. NaN/Infinity become zero. Limited to ~15–17 significant digits.
    #[inline]
    pub fn from_f64(value: f64) -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_decimal_from_double(value) })
    }

    /// Constructs from an `i64`.
    #[inline]
    pub fn from_i64(value: i64) -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_decimal_from_int64(value) })
    }

    /// Constructs from a `u64`.
    #[inline]
    pub fn from_u64(value: u64) -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_decimal_from_uint64(value) })
    }

    /// Parses a decimal string (e.g. `"123.45"`, `"-0.001"`, `"1.23e5"`).
    ///
    /// Returns `None` if the string is invalid.
    pub fn from_str(s: &str) -> Option<Self> {
        let c = CString::new(s).ok()?;
        let mut result = ffi::dnv_vista_sdk_decimal_t {
            flags: 0,
            mantissa: [0; 3],
        };
        let ok = unsafe { ffi::dnv_vista_sdk_decimal_from_string(c.as_ptr(), &mut result) };
        if ok != 0 {
            Some(Self::from_ffi(result))
        } else {
            None
        }
    }

    /// Number of decimal places in the stored value (0–28).
    #[inline]
    pub fn scale(self) -> u8 {
        unsafe { ffi::dnv_vista_sdk_decimal_scale(self.into_ffi()) }
    }

    /// Number of significant decimal places, excluding trailing zeros (0–28).
    #[inline]
    pub fn decimal_places_count(self) -> u8 {
        unsafe { ffi::dnv_vista_sdk_decimal_decimal_places_count(self.into_ffi()) }
    }

    /// Number of significant digits, excluding sign and decimal point (at least 1).
    #[inline]
    pub fn total_digits_count(self) -> u32 {
        unsafe { ffi::dnv_vista_sdk_decimal_total_digits_count(self.into_ffi()) }
    }

    /// Converts to `f64`. May lose precision beyond ~15–17 significant digits.
    #[inline]
    pub fn to_f64(self) -> f64 {
        unsafe { ffi::dnv_vista_sdk_decimal_to_double(self.into_ffi()) }
    }

    /// Returns the internal 32-bit representation as `[mantissa[0], mantissa[1], mantissa[2], flags]`.
    #[inline]
    pub fn to_bits(self) -> [i32; 4] {
        let mut bits = [0i32; 4];
        unsafe { ffi::dnv_vista_sdk_decimal_to_bits(self.into_ffi(), bits.as_mut_ptr()) };
        bits
    }

    /// Negates this value.
    #[inline]
    pub fn negate(self) -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_decimal_negate(self.into_ffi()) })
    }

    /// Absolute value.
    #[inline]
    pub fn abs(self) -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_decimal_abs(self.into_ffi()) })
    }

    /// Ceiling (toward +∞).
    #[inline]
    pub fn ceil(self) -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_decimal_ceil(self.into_ffi()) })
    }

    /// Floor (toward −∞).
    #[inline]
    pub fn floor(self) -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_decimal_floor(self.into_ffi()) })
    }

    /// Truncation (toward zero).
    #[inline]
    pub fn trunc(self) -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_decimal_trunc(self.into_ffi()) })
    }

    /// Rounds to `decimal_places` using the given rounding mode.
    #[inline]
    pub fn round(self, decimal_places: i32, mode: RoundingMode) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_decimal_round(
                self.into_ffi(),
                decimal_places,
                std::mem::transmute(mode),
            )
        })
    }

    /// Square root. Returns zero and sets the last error if `self` is negative.
    #[inline]
    pub fn sqrt(self) -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_decimal_sqrt(self.into_ffi()) })
    }

    /// Compares to another `Decimal`. Returns `Ordering`.
    #[inline]
    fn compare(self, other: Self) -> std::cmp::Ordering {
        unsafe { ffi::dnv_vista_sdk_decimal_compare(self.into_ffi(), other.into_ffi()) }.cmp(&0)
    }

    #[inline]
    pub(crate) fn into_ffi(self) -> ffi::dnv_vista_sdk_decimal_t {
        ffi::dnv_vista_sdk_decimal_t {
            flags: self.flags,
            mantissa: self.mantissa,
        }
    }

    #[inline]
    pub(crate) fn from_ffi(f: ffi::dnv_vista_sdk_decimal_t) -> Self {
        Self {
            flags: f.flags,
            mantissa: f.mantissa,
        }
    }
}

impl PartialEq for Decimal {
    #[inline]
    fn eq(&self, other: &Self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_decimal_equals(self.into_ffi(), other.into_ffi()) }) != 0
    }
}

impl Eq for Decimal {}

impl PartialOrd for Decimal {
    #[inline]
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for Decimal {
    #[inline]
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.compare(*other)
    }
}

impl Add for Decimal {
    type Output = Self;
    #[inline]
    fn add(self, rhs: Self) -> Self {
        Self::from_ffi(unsafe { ffi::dnv_vista_sdk_decimal_add(self.into_ffi(), rhs.into_ffi()) })
    }
}

impl Sub for Decimal {
    type Output = Self;
    #[inline]
    fn sub(self, rhs: Self) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_decimal_subtract(self.into_ffi(), rhs.into_ffi())
        })
    }
}

impl Mul for Decimal {
    type Output = Self;
    #[inline]
    fn mul(self, rhs: Self) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_decimal_multiply(self.into_ffi(), rhs.into_ffi())
        })
    }
}

impl Div for Decimal {
    type Output = Self;
    #[inline]
    fn div(self, rhs: Self) -> Self {
        Self::from_ffi(unsafe {
            ffi::dnv_vista_sdk_decimal_divide(self.into_ffi(), rhs.into_ffi())
        })
    }
}

impl Neg for Decimal {
    type Output = Self;
    #[inline]
    fn neg(self) -> Self {
        self.negate()
    }
}

impl AddAssign for Decimal {
    #[inline]
    fn add_assign(&mut self, rhs: Self) {
        *self = *self + rhs;
    }
}

impl SubAssign for Decimal {
    #[inline]
    fn sub_assign(&mut self, rhs: Self) {
        *self = *self - rhs;
    }
}

impl fmt::Display for Decimal {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let ptr = unsafe { ffi::dnv_vista_sdk_decimal_to_string(self.into_ffi()) };
        if ptr.is_null() {
            return f.write_str("(null)");
        }
        let s = unsafe { std::ffi::CStr::from_ptr(ptr).to_string_lossy().into_owned() };
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        f.write_str(&s)
    }
}

impl fmt::Debug for Decimal {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "Decimal({})", self)
    }
}
