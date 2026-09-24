"""
    RoundingMode

Rounding mode used by `round`.

# Variants
- `ToNearest`            : round to nearest; ties go to the nearest even digit (banker's rounding)
- `ToNearestTiesAway`    : round to nearest; ties go away from zero
- `ToZero`               : round toward zero (truncate)
- `ToPositiveInfinity`   : round toward positive infinity (ceiling)
- `ToNegativeInfinity`   : round toward negative infinity (floor)
"""
@enum RoundingMode::Int32 begin
    ToNearest = 0
    ToNearestTiesAway = 1
    ToZero = 2
    ToPositiveInfinity = 3
    ToNegativeInfinity = 4
end

"""
    Decimal

A fixed-point decimal number with up to 28 significant digits.

Use `Base.parse(Decimal, "1.23")` for exact construction. `Decimal(::Float64)` may lose
precision beyond ~15-17 significant digits.
"""
struct Decimal
    _flags::UInt32
    _mantissa_0::UInt32
    _mantissa_1::UInt32
    _mantissa_2::UInt32
end

function _ffi(d::Decimal)
    _FfiDecimal(d._flags, d._mantissa_0, d._mantissa_1, d._mantissa_2)
end

function _from_ffi(f::_FfiDecimal)
    Decimal(f.flags, f.mantissa_0, f.mantissa_1, f.mantissa_2)
end

function Decimal(value::Float64)
    _from_ffi(ffi_decimal_from_double(value))
end

function Decimal(value::Integer)
    if value >= 0
        _from_ffi(ffi_decimal_from_uint64(UInt64(value)))
    else
        _from_ffi(ffi_decimal_from_int64(Int64(value)))
    end
end

function Base.zero(::Base.Type{Decimal})
    _from_ffi(ffi_decimal_zero())
end

function Base.typemin(::Base.Type{Decimal})
    _from_ffi(ffi_decimal_min())
end

function Base.typemax(::Base.Type{Decimal})
    _from_ffi(ffi_decimal_max())
end

function lowest(::Base.Type{Decimal})
    _from_ffi(ffi_decimal_lowest())
end

function scale(d::Decimal)
    Int(ffi_decimal_scale(_ffi(d)))
end

function decimal_places_count(d::Decimal)
    Int(ffi_decimal_decimal_places_count(_ffi(d)))
end

function total_digits_count(d::Decimal)
    Int(ffi_decimal_total_digits_count(_ffi(d)))
end

function to_f64(d::Decimal)
    ffi_decimal_to_double(_ffi(d))
end

function to_bits(d::Decimal)
    bits = zeros(Int32, 4)
    GC.@preserve bits ffi_decimal_to_bits(_ffi(d), pointer(bits))
    (bits[1], bits[2], bits[3], bits[4])
end

function Base.abs(d::Decimal)
    _from_ffi(ffi_decimal_abs(_ffi(d)))
end

function Base.ceil(d::Decimal)
    _from_ffi(ffi_decimal_ceil(_ffi(d)))
end

function Base.floor(d::Decimal)
    _from_ffi(ffi_decimal_floor(_ffi(d)))
end

function Base.trunc(d::Decimal)
    _from_ffi(ffi_decimal_trunc(_ffi(d)))
end

"""
    sqrt(d::Decimal) -> Decimal

Return the square root of `d`. Throws [`VistaError`](@ref) if `d` is negative.
"""
function Base.sqrt(d::Decimal)
    clear_error()
    result = _from_ffi(ffi_decimal_sqrt(_ffi(d)))
    last_error().kind == ErrorNone || throw(last_error())
    result
end

function Base.round(d::Decimal, decimal_places::Integer, mode::RoundingMode = ToNearest)
    ffi_mode = _FfiDecimalRoundingMode(Int32(mode))
    _from_ffi(ffi_decimal_round(_ffi(d), Int32(decimal_places), ffi_mode))
end

function Base.:+(a::Decimal, b::Decimal)
    _from_ffi(ffi_decimal_add(_ffi(a), _ffi(b)))
end

function Base.:-(a::Decimal, b::Decimal)
    _from_ffi(ffi_decimal_subtract(_ffi(a), _ffi(b)))
end

function Base.:*(a::Decimal, b::Decimal)
    _from_ffi(ffi_decimal_multiply(_ffi(a), _ffi(b)))
end

"""
    /(a::Decimal, b::Decimal) -> Decimal

Divide `a` by `b`. Throws [`VistaError`](@ref) if `b` is zero.
"""
function Base.:/(a::Decimal, b::Decimal)
    clear_error()
    result = _from_ffi(ffi_decimal_divide(_ffi(a), _ffi(b)))
    last_error().kind == ErrorNone || throw(last_error())
    result
end

function Base.:-(d::Decimal)
    _from_ffi(ffi_decimal_negate(_ffi(d)))
end

function Base.:(==)(a::Decimal, b::Decimal)
    ffi_decimal_equals(_ffi(a), _ffi(b)) != 0
end

function Base.isless(a::Decimal, b::Decimal)
    ffi_decimal_compare(_ffi(a), _ffi(b)) < 0
end

function Base.hash(d::Decimal, h::UInt)
    hash((d._flags, d._mantissa_0, d._mantissa_1, d._mantissa_2), h)
end

function Base.string(d::Decimal)
    p = ffi_decimal_to_string(_ffi(d))
    p == C_NULL && return "(null)"
    s = unsafe_string(p)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), p)
    s
end

function Base.show(io::IO, d::Decimal)
    print(io, "Decimal(", Base.string(d), ")")
end

function Base.parse(::Base.Type{Decimal}, s::AbstractString)
    cs = String(s)
    result = Ref(_FfiDecimal(UInt32(0), UInt32(0), UInt32(0), UInt32(0)))
    ok = GC.@preserve cs ffi_decimal_from_string(Base.unsafe_convert(Cstring, cs), result)
    ok != 0 || throw(last_error())
    _from_ffi(result[])
end
