import Dates

struct TimeSpan
    _ticks::Int64
end

function _ffi(ts::TimeSpan)
    _FfiTimeSpan(ts._ticks)
end

function _from_ffi(ffi::_FfiTimeSpan)
    TimeSpan(ffi.ticks)
end

function TimeSpan(ticks::Integer)
    _from_ffi(ffi_time_span_from_ticks(Int64(ticks)))
end

function TimeSpan(p::Dates.Period)
    _from_ffi(ffi_time_span_from_milliseconds(Float64(Dates.toms(p))))
end

function from_days(::Base.Type{TimeSpan}, days::Real)
    _from_ffi(ffi_time_span_from_days(Float64(days)))
end

function from_hours(::Base.Type{TimeSpan}, hours::Real)
    _from_ffi(ffi_time_span_from_hours(Float64(hours)))
end

function from_minutes(::Base.Type{TimeSpan}, minutes::Real)
    _from_ffi(ffi_time_span_from_minutes(Float64(minutes)))
end

function from_seconds(::Base.Type{TimeSpan}, seconds::Real)
    _from_ffi(ffi_time_span_from_seconds(Float64(seconds)))
end

function from_millis(::Base.Type{TimeSpan}, ms::Real)
    _from_ffi(ffi_time_span_from_milliseconds(Float64(ms)))
end

function from_micros(::Base.Type{TimeSpan}, us::Real)
    _from_ffi(ffi_time_span_from_microseconds(Float64(us)))
end

function ticks(ts::TimeSpan)
    ts._ticks
end

function days(ts::TimeSpan)
    ffi_time_span_days(_ffi(ts))
end

function hours(ts::TimeSpan)
    ffi_time_span_hours(_ffi(ts))
end

function minutes(ts::TimeSpan)
    ffi_time_span_minutes(_ffi(ts))
end

function seconds(ts::TimeSpan)
    ffi_time_span_seconds(_ffi(ts))
end

function millis(ts::TimeSpan)
    ffi_time_span_milliseconds(_ffi(ts))
end

function micros(ts::TimeSpan)
    ffi_time_span_microseconds(_ffi(ts))
end

function nanos(ts::TimeSpan)
    ffi_time_span_nanoseconds(_ffi(ts))
end

function negate(ts::TimeSpan)
    _from_ffi(ffi_time_span_negate(_ffi(ts)))
end

function divide(ts::TimeSpan, divisor::Real)
    _from_ffi(ffi_time_span_divide(_ffi(ts), Float64(divisor)))
end

function ratio(a::TimeSpan, b::TimeSpan)
    ffi_time_span_ratio(_ffi(a), _ffi(b))
end

function Base.:+(a::TimeSpan, b::TimeSpan)
    _from_ffi(ffi_time_span_add(_ffi(a), _ffi(b)))
end

function Base.:+(ts::TimeSpan, p::Dates.Period)
    ts + TimeSpan(p)
end

function Base.:+(p::Dates.Period, ts::TimeSpan)
    TimeSpan(p) + ts
end

function Base.:-(a::TimeSpan, b::TimeSpan)
    _from_ffi(ffi_time_span_subtract(_ffi(a), _ffi(b)))
end

function Base.:-(ts::TimeSpan, p::Dates.Period)
    ts - TimeSpan(p)
end

function Base.:-(ts::TimeSpan)
    negate(ts)
end

function Base.:*(ts::TimeSpan, multiplier::Real)
    _from_ffi(ffi_time_span_multiply(_ffi(ts), Float64(multiplier)))
end

function Base.:*(multiplier::Real, ts::TimeSpan)
    ts * multiplier
end

function Base.:/(ts::TimeSpan, divisor::Real)
    divide(ts, divisor)
end

function Base.:(==)(a::TimeSpan, b::TimeSpan)
    a._ticks == b._ticks
end

function Base.isless(a::TimeSpan, b::TimeSpan)
    a._ticks < b._ticks
end

function Base.hash(ts::TimeSpan, h::UInt)
    hash(ts._ticks, h)
end

function Base.string(ts::TimeSpan)
    p = ffi_time_span_to_string(_ffi(ts))
    p == C_NULL && return ""
    s = unsafe_string(p)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), p)
    s
end

function Base.show(io::IO, ts::TimeSpan)
    print(io, "TimeSpan(", Base.string(ts), ")")
end

function Base.parse(::Base.Type{TimeSpan}, s::AbstractString)
    cs = String(s)
    result = Ref(_FfiTimeSpan(Int64(0)))
    ok = GC.@preserve cs ffi_time_span_from_string(Base.unsafe_convert(Cstring, cs), result)
    ok != 0 || throw(last_error())
    _from_ffi(result[])
end

function Base.convert(::Base.Type{Dates.Millisecond}, ts::TimeSpan)
    Dates.Millisecond(round(Int64, millis(ts)))
end
