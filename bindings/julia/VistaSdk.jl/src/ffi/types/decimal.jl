struct _FfiDecimal
    flags::UInt32
    mantissa_0::UInt32
    mantissa_1::UInt32
    mantissa_2::UInt32
end

@enum _FfiDecimalRoundingMode::Int32 begin
    _RoundToNearest = 0
    _RoundToNearestTiesAway = 1
    _RoundToZero = 2
    _RoundToPositiveInfinity = 3
    _RoundToNegativeInfinity = 4
end

function ffi_decimal_zero()
    ccall((:dnv_vista_sdk_decimal_zero, VISTA_LIB), _FfiDecimal, ())
end

function ffi_decimal_from_double(value::Float64)
    ccall((:dnv_vista_sdk_decimal_from_double, VISTA_LIB), _FfiDecimal, (Float64,), value)
end

function ffi_decimal_from_int64(value::Int64)
    ccall((:dnv_vista_sdk_decimal_from_int64, VISTA_LIB), _FfiDecimal, (Int64,), value)
end

function ffi_decimal_from_uint64(value::UInt64)
    ccall((:dnv_vista_sdk_decimal_from_uint64, VISTA_LIB), _FfiDecimal, (UInt64,), value)
end

function ffi_decimal_from_string(str::Cstring, result::Ref{_FfiDecimal})
    ccall(
        (:dnv_vista_sdk_decimal_from_string, VISTA_LIB),
        Cint,
        (Cstring, Ptr{_FfiDecimal}),
        str,
        result,
    )
end

function ffi_decimal_scale(d::_FfiDecimal)
    ccall((:dnv_vista_sdk_decimal_scale, VISTA_LIB), UInt8, (_FfiDecimal,), d)
end

function ffi_decimal_decimal_places_count(d::_FfiDecimal)
    ccall(
        (:dnv_vista_sdk_decimal_decimal_places_count, VISTA_LIB),
        UInt8,
        (_FfiDecimal,),
        d,
    )
end

function ffi_decimal_total_digits_count(d::_FfiDecimal)
    ccall((:dnv_vista_sdk_decimal_total_digits_count, VISTA_LIB), UInt32, (_FfiDecimal,), d)
end

function ffi_decimal_add(a::_FfiDecimal, b::_FfiDecimal)
    ccall(
        (:dnv_vista_sdk_decimal_add, VISTA_LIB),
        _FfiDecimal,
        (_FfiDecimal, _FfiDecimal),
        a,
        b,
    )
end

function ffi_decimal_subtract(a::_FfiDecimal, b::_FfiDecimal)
    ccall(
        (:dnv_vista_sdk_decimal_subtract, VISTA_LIB),
        _FfiDecimal,
        (_FfiDecimal, _FfiDecimal),
        a,
        b,
    )
end

function ffi_decimal_multiply(a::_FfiDecimal, b::_FfiDecimal)
    ccall(
        (:dnv_vista_sdk_decimal_multiply, VISTA_LIB),
        _FfiDecimal,
        (_FfiDecimal, _FfiDecimal),
        a,
        b,
    )
end

function ffi_decimal_divide(a::_FfiDecimal, b::_FfiDecimal)
    ccall(
        (:dnv_vista_sdk_decimal_divide, VISTA_LIB),
        _FfiDecimal,
        (_FfiDecimal, _FfiDecimal),
        a,
        b,
    )
end

function ffi_decimal_negate(d::_FfiDecimal)
    ccall((:dnv_vista_sdk_decimal_negate, VISTA_LIB), _FfiDecimal, (_FfiDecimal,), d)
end

function ffi_decimal_abs(d::_FfiDecimal)
    ccall((:dnv_vista_sdk_decimal_abs, VISTA_LIB), _FfiDecimal, (_FfiDecimal,), d)
end

function ffi_decimal_ceil(d::_FfiDecimal)
    ccall((:dnv_vista_sdk_decimal_ceil, VISTA_LIB), _FfiDecimal, (_FfiDecimal,), d)
end

function ffi_decimal_floor(d::_FfiDecimal)
    ccall((:dnv_vista_sdk_decimal_floor, VISTA_LIB), _FfiDecimal, (_FfiDecimal,), d)
end

function ffi_decimal_trunc(d::_FfiDecimal)
    ccall((:dnv_vista_sdk_decimal_trunc, VISTA_LIB), _FfiDecimal, (_FfiDecimal,), d)
end

function ffi_decimal_round(
    d::_FfiDecimal,
    decimal_places::Int32,
    mode::_FfiDecimalRoundingMode,
)
    ccall(
        (:dnv_vista_sdk_decimal_round, VISTA_LIB),
        _FfiDecimal,
        (_FfiDecimal, Int32, Int32),
        d,
        decimal_places,
        Int32(mode),
    )
end

function ffi_decimal_sqrt(d::_FfiDecimal)
    ccall((:dnv_vista_sdk_decimal_sqrt, VISTA_LIB), _FfiDecimal, (_FfiDecimal,), d)
end

function ffi_decimal_compare(a::_FfiDecimal, b::_FfiDecimal)
    ccall(
        (:dnv_vista_sdk_decimal_compare, VISTA_LIB),
        Cint,
        (_FfiDecimal, _FfiDecimal),
        a,
        b,
    )
end

function ffi_decimal_equals(a::_FfiDecimal, b::_FfiDecimal)
    ccall(
        (:dnv_vista_sdk_decimal_equals, VISTA_LIB),
        Cint,
        (_FfiDecimal, _FfiDecimal),
        a,
        b,
    )
end

function ffi_decimal_to_double(d::_FfiDecimal)
    ccall((:dnv_vista_sdk_decimal_to_double, VISTA_LIB), Float64, (_FfiDecimal,), d)
end

function ffi_decimal_to_bits(d::_FfiDecimal, bits::Ptr{Int32})
    ccall(
        (:dnv_vista_sdk_decimal_to_bits, VISTA_LIB),
        Cvoid,
        (_FfiDecimal, Ptr{Int32}),
        d,
        bits,
    )
end

function ffi_decimal_min()
    ccall((:dnv_vista_sdk_decimal_min, VISTA_LIB), _FfiDecimal, ())
end

function ffi_decimal_max()
    ccall((:dnv_vista_sdk_decimal_max, VISTA_LIB), _FfiDecimal, ())
end

function ffi_decimal_lowest()
    ccall((:dnv_vista_sdk_decimal_lowest, VISTA_LIB), _FfiDecimal, ())
end

function ffi_decimal_to_string(d::_FfiDecimal)
    ccall((:dnv_vista_sdk_decimal_to_string, VISTA_LIB), Ptr{UInt8}, (_FfiDecimal,), d)
end
