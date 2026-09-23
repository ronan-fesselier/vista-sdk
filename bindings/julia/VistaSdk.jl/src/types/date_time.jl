import Dates

struct DateTime
    _ticks::Int64
end

function _ffi(dt::DateTime)
    _FfiDateTime(dt._ticks)
end

function _from_ffi(ffi::_FfiDateTime)
    DateTime(ffi.ticks)
end

function DateTime(ticks::Integer)
    _from_ffi(ffi_date_time_from_ticks(Int64(ticks)))
end

"""
    DateTime(year, month, day) -> DateTime

Construct a `DateTime` from calendar components (no time-of-day, i.e. midnight).

Returns a sentinel (`typemin(DateTime)`) if the components are invalid (e.g. `month=13`),
instead of throwing.
"""
function DateTime(year::Integer, month::Integer, day::Integer)
    _from_ffi(ffi_date_time_from_date(Int32(year), Int32(month), Int32(day)))
end

"""
    DateTime(year, month, day, hour, minute, second) -> DateTime

Construct a `DateTime` from calendar and time-of-day components.

Returns a sentinel (`typemin(DateTime)`) if the components are invalid, instead of
throwing.
"""
function DateTime(
    year::Integer,
    month::Integer,
    day::Integer,
    hour::Integer,
    minute::Integer,
    second::Integer,
)
    _from_ffi(
        ffi_date_time_from_date_time(
            Int32(year),
            Int32(month),
            Int32(day),
            Int32(hour),
            Int32(minute),
            Int32(second),
        ),
    )
end

"""
    DateTime(year, month, day, hour, minute, second, millisecond) -> DateTime

Construct a `DateTime` from calendar, time-of-day, and millisecond components.

Returns a sentinel (`typemin(DateTime)`) if the components are invalid, instead of
throwing.
"""
function DateTime(
    year::Integer,
    month::Integer,
    day::Integer,
    hour::Integer,
    minute::Integer,
    second::Integer,
    millisecond::Integer,
)
    _from_ffi(
        ffi_date_time_from_date_time_millis(
            Int32(year),
            Int32(month),
            Int32(day),
            Int32(hour),
            Int32(minute),
            Int32(second),
            Int32(millisecond),
        ),
    )
end

function DateTime(dt::Dates.DateTime)
    _from_ffi(
        ffi_date_time_from_date_time_millis(
            Int32(Dates.year(dt)),
            Int32(Dates.month(dt)),
            Int32(Dates.day(dt)),
            Int32(Dates.hour(dt)),
            Int32(Dates.minute(dt)),
            Int32(Dates.second(dt)),
            Int32(Dates.millisecond(dt)),
        ),
    )
end

function from_epoch_seconds(::Base.Type{DateTime}, seconds::Integer)
    _from_ffi(ffi_date_time_from_epoch_seconds(Int64(seconds)))
end

function from_epoch_millis(::Base.Type{DateTime}, ms::Integer)
    _from_ffi(ffi_date_time_from_epoch_milliseconds(Int64(ms)))
end

function utc_now(::Base.Type{DateTime})
    _from_ffi(ffi_date_time_utc_now())
end

function Base.typemin(::Base.Type{DateTime})
    _from_ffi(ffi_date_time_min())
end

function Base.typemax(::Base.Type{DateTime})
    _from_ffi(ffi_date_time_max())
end

function epoch(::Base.Type{DateTime})
    _from_ffi(ffi_date_time_epoch())
end

function is_leap_year(::Base.Type{DateTime}, year::Integer)
    ffi_date_time_is_leap_year(Int32(year)) != 0
end

function days_in_month(::Base.Type{DateTime}, year::Integer, month::Integer)
    Int(ffi_date_time_days_in_month(Int32(year), Int32(month)))
end

function ticks(dt::DateTime)
    dt._ticks
end

function year(dt::DateTime)
    Int(ffi_date_time_year(_ffi(dt)))
end

function month(dt::DateTime)
    Int(ffi_date_time_month(_ffi(dt)))
end

function day(dt::DateTime)
    Int(ffi_date_time_day(_ffi(dt)))
end

function hour(dt::DateTime)
    Int(ffi_date_time_hour(_ffi(dt)))
end

function minute(dt::DateTime)
    Int(ffi_date_time_minute(_ffi(dt)))
end

function second(dt::DateTime)
    Int(ffi_date_time_second(_ffi(dt)))
end

function millisecond(dt::DateTime)
    Int(ffi_date_time_millisecond(_ffi(dt)))
end

function microsecond(dt::DateTime)
    Int(ffi_date_time_microsecond(_ffi(dt)))
end

function nanosecond(dt::DateTime)
    Int(ffi_date_time_nanosecond(_ffi(dt)))
end

function day_of_week(dt::DateTime)
    Int(ffi_date_time_day_of_week(_ffi(dt)))
end

function day_of_year(dt::DateTime)
    Int(ffi_date_time_day_of_year(_ffi(dt)))
end

function to_epoch_seconds(dt::DateTime)
    ffi_date_time_to_epoch_seconds(_ffi(dt))
end

function to_epoch_millis(dt::DateTime)
    ffi_date_time_to_epoch_milliseconds(_ffi(dt))
end

function date(dt::DateTime)
    _from_ffi(ffi_date_time_date(_ffi(dt)))
end

function time_of_day(dt::DateTime)
    _from_ffi(ffi_date_time_time_of_day(_ffi(dt)))
end

function is_valid(dt::DateTime)
    ffi_date_time_is_valid(_ffi(dt)) != 0
end

function add_days(dt::DateTime, d::Real)
    _from_ffi(ffi_date_time_add_days(_ffi(dt), Float64(d)))
end

function add_hours(dt::DateTime, h::Real)
    _from_ffi(ffi_date_time_add_hours(_ffi(dt), Float64(h)))
end

function add_minutes(dt::DateTime, m::Real)
    _from_ffi(ffi_date_time_add_minutes(_ffi(dt), Float64(m)))
end

function add_seconds(dt::DateTime, s::Real)
    _from_ffi(ffi_date_time_add_seconds(_ffi(dt), Float64(s)))
end

function add_milliseconds(dt::DateTime, ms::Real)
    _from_ffi(ffi_date_time_add_milliseconds(_ffi(dt), Float64(ms)))
end

function add_months(dt::DateTime, m::Integer)
    _from_ffi(ffi_date_time_add_months(_ffi(dt), Int32(m)))
end

function add_years(dt::DateTime, y::Integer)
    _from_ffi(ffi_date_time_add_years(_ffi(dt), Int32(y)))
end

function Base.:+(dt::DateTime, ts::TimeSpan)
    _from_ffi(ffi_date_time_add_time_span(_ffi(dt), _ffi(ts)))
end

function Base.:+(dt::DateTime, p::Dates.Period)
    dt + TimeSpan(p)
end

function Base.:-(dt::DateTime, ts::TimeSpan)
    _from_ffi(ffi_date_time_subtract_time_span(_ffi(dt), _ffi(ts)))
end

function Base.:-(dt::DateTime, p::Dates.Period)
    dt - TimeSpan(p)
end

function Base.:-(a::DateTime, b::DateTime)
    _from_ffi(ffi_date_time_subtract(_ffi(a), _ffi(b)))
end

function Base.:(==)(a::DateTime, b::DateTime)
    a._ticks == b._ticks
end

function Base.isless(a::DateTime, b::DateTime)
    a._ticks < b._ticks
end

function Base.hash(dt::DateTime, h::UInt)
    hash(dt._ticks, h)
end

function to_string(dt::DateTime, format::DateTimeFormat = Iso8601)
    p = ffi_date_time_to_string(_ffi(dt), format)
    p == C_NULL && return ""
    s = unsafe_string(p)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), p)
    s
end

function Base.string(dt::DateTime)
    to_string(dt, Iso8601)
end

function Base.show(io::IO, dt::DateTime)
    print(io, "DateTime(", Base.string(dt), ")")
end

function Base.parse(::Base.Type{DateTime}, s::AbstractString)
    cs = String(s)
    result = Ref(_FfiDateTime(Int64(0)))
    ok = GC.@preserve cs ffi_date_time_from_string(Base.unsafe_convert(Cstring, cs), result)
    ok != 0 || throw(last_error())
    _from_ffi(result[])
end

function Base.convert(::Base.Type{Dates.DateTime}, dt::DateTime)
    Dates.DateTime(
        year(dt),
        month(dt),
        day(dt),
        hour(dt),
        minute(dt),
        second(dt),
        millisecond(dt),
    )
end

struct DateTimeOffset
    _dt::DateTime
    _offset::TimeSpan
    DateTimeOffset(dt::DateTime, offset::TimeSpan, ::Val{:raw}) = new(dt, offset)
end

function _ffi(dto::DateTimeOffset)
    _FfiDateTimeOffset(dto._dt._ticks, dto._offset._ticks)
end

function _from_ffi(ffi::_FfiDateTimeOffset)
    DateTimeOffset(DateTime(ffi.ticks), TimeSpan(ffi.offset_ticks), Val(:raw))
end

function DateTimeOffset(dt::DateTime, offset::TimeSpan)
    _from_ffi(ffi_date_time_offset_create(_ffi(dt), _ffi(offset)))
end

function DateTimeOffset(dt::DateTime)
    _from_ffi(ffi_date_time_offset_from_date_time(_ffi(dt)))
end

function DateTimeOffset(ticks::Integer, offset::TimeSpan)
    _from_ffi(ffi_date_time_offset_from_ticks(Int64(ticks), _ffi(offset)))
end

function DateTimeOffset(dt::Dates.DateTime, offset_minutes::Integer = 0)
    DateTimeOffset(DateTime(dt), from_minutes(TimeSpan, Float64(offset_minutes)))
end

function from_epoch_seconds(::Base.Type{DateTimeOffset}, seconds::Integer)
    _from_ffi(ffi_date_time_offset_from_epoch_seconds(Int64(seconds)))
end

function from_epoch_millis(::Base.Type{DateTimeOffset}, ms::Integer)
    _from_ffi(ffi_date_time_offset_from_epoch_milliseconds(Int64(ms)))
end

function from_filetime(::Base.Type{DateTimeOffset}, filetime::Integer)
    _from_ffi(ffi_date_time_offset_from_filetime(Int64(filetime)))
end

function now(::Base.Type{DateTimeOffset})
    _from_ffi(ffi_date_time_offset_now())
end

function utc_now(::Base.Type{DateTimeOffset})
    _from_ffi(ffi_date_time_offset_utc_now())
end

function today(::Base.Type{DateTimeOffset})
    _from_ffi(ffi_date_time_offset_today())
end

function Base.typemin(::Base.Type{DateTimeOffset})
    _from_ffi(ffi_date_time_offset_min())
end

function Base.typemax(::Base.Type{DateTimeOffset})
    _from_ffi(ffi_date_time_offset_max())
end

function epoch(::Base.Type{DateTimeOffset})
    _from_ffi(ffi_date_time_offset_epoch())
end

function date_time(dto::DateTimeOffset)
    _from_ffi(ffi_date_time_offset_date_time(_ffi(dto)))
end

function offset(dto::DateTimeOffset)
    _from_ffi(ffi_date_time_offset_offset(_ffi(dto)))
end

function utc_date_time(dto::DateTimeOffset)
    _from_ffi(ffi_date_time_offset_utc_date_time(_ffi(dto)))
end

function local_date_time(dto::DateTimeOffset)
    _from_ffi(ffi_date_time_offset_local_date_time(_ffi(dto)))
end

function ticks(dto::DateTimeOffset)
    ffi_date_time_offset_ticks(_ffi(dto))
end

function utc_ticks(dto::DateTimeOffset)
    ffi_date_time_offset_utc_ticks(_ffi(dto))
end

function year(dto::DateTimeOffset)
    Int(ffi_date_time_offset_year(_ffi(dto)))
end

function month(dto::DateTimeOffset)
    Int(ffi_date_time_offset_month(_ffi(dto)))
end

function day(dto::DateTimeOffset)
    Int(ffi_date_time_offset_day(_ffi(dto)))
end

function hour(dto::DateTimeOffset)
    Int(ffi_date_time_offset_hour(_ffi(dto)))
end

function minute(dto::DateTimeOffset)
    Int(ffi_date_time_offset_minute(_ffi(dto)))
end

function second(dto::DateTimeOffset)
    Int(ffi_date_time_offset_second(_ffi(dto)))
end

function millisecond(dto::DateTimeOffset)
    Int(ffi_date_time_offset_millisecond(_ffi(dto)))
end

function microsecond(dto::DateTimeOffset)
    Int(ffi_date_time_offset_microsecond(_ffi(dto)))
end

function nanosecond(dto::DateTimeOffset)
    Int(ffi_date_time_offset_nanosecond(_ffi(dto)))
end

function day_of_week(dto::DateTimeOffset)
    Int(ffi_date_time_offset_day_of_week(_ffi(dto)))
end

function day_of_year(dto::DateTimeOffset)
    Int(ffi_date_time_offset_day_of_year(_ffi(dto)))
end

function total_offset_minutes(dto::DateTimeOffset)
    Int(ffi_date_time_offset_total_offset_minutes(_ffi(dto)))
end

function to_epoch_seconds(dto::DateTimeOffset)
    ffi_date_time_offset_to_epoch_seconds(_ffi(dto))
end

function to_epoch_millis(dto::DateTimeOffset)
    ffi_date_time_offset_to_epoch_milliseconds(_ffi(dto))
end

function date(dto::DateTimeOffset)
    _from_ffi(ffi_date_time_offset_date(_ffi(dto)))
end

function time_of_day(dto::DateTimeOffset)
    _from_ffi(ffi_date_time_offset_time_of_day(_ffi(dto)))
end

function to_offset(dto::DateTimeOffset, new_offset::TimeSpan)
    _from_ffi(ffi_date_time_offset_to_offset(_ffi(dto), _ffi(new_offset)))
end

function to_universal_time(dto::DateTimeOffset)
    _from_ffi(ffi_date_time_offset_to_universal_time(_ffi(dto)))
end

function to_local_time(dto::DateTimeOffset)
    _from_ffi(ffi_date_time_offset_to_local_time(_ffi(dto)))
end

function to_filetime(dto::DateTimeOffset)
    ffi_date_time_offset_to_filetime(_ffi(dto))
end

function is_valid(dto::DateTimeOffset)
    ffi_date_time_offset_is_valid(_ffi(dto)) != 0
end

function equals_exact(a::DateTimeOffset, b::DateTimeOffset)
    ffi_date_time_offset_equals_exact(_ffi(a), _ffi(b)) != 0
end

function add_days(dto::DateTimeOffset, d::Real)
    _from_ffi(ffi_date_time_offset_add_days(_ffi(dto), Float64(d)))
end

function add_hours(dto::DateTimeOffset, h::Real)
    _from_ffi(ffi_date_time_offset_add_hours(_ffi(dto), Float64(h)))
end

function add_minutes(dto::DateTimeOffset, m::Real)
    _from_ffi(ffi_date_time_offset_add_minutes(_ffi(dto), Float64(m)))
end

function add_seconds(dto::DateTimeOffset, s::Real)
    _from_ffi(ffi_date_time_offset_add_seconds(_ffi(dto), Float64(s)))
end

function add_milliseconds(dto::DateTimeOffset, ms::Real)
    _from_ffi(ffi_date_time_offset_add_milliseconds(_ffi(dto), Float64(ms)))
end

function add_months(dto::DateTimeOffset, m::Integer)
    _from_ffi(ffi_date_time_offset_add_months(_ffi(dto), Int32(m)))
end

function add_years(dto::DateTimeOffset, y::Integer)
    _from_ffi(ffi_date_time_offset_add_years(_ffi(dto), Int32(y)))
end

function Base.:+(dto::DateTimeOffset, ts::TimeSpan)
    _from_ffi(ffi_date_time_offset_add_time_span(_ffi(dto), _ffi(ts)))
end

function Base.:+(dto::DateTimeOffset, p::Dates.Period)
    dto + TimeSpan(p)
end

function Base.:-(dto::DateTimeOffset, ts::TimeSpan)
    _from_ffi(ffi_date_time_offset_subtract_time_span(_ffi(dto), _ffi(ts)))
end

function Base.:-(dto::DateTimeOffset, p::Dates.Period)
    dto - TimeSpan(p)
end

function Base.:-(a::DateTimeOffset, b::DateTimeOffset)
    _from_ffi(ffi_date_time_offset_subtract(_ffi(a), _ffi(b)))
end

function Base.:(==)(a::DateTimeOffset, b::DateTimeOffset)
    ffi_date_time_offset_equals(_ffi(a), _ffi(b)) != 0
end

function Base.isless(a::DateTimeOffset, b::DateTimeOffset)
    ffi_date_time_offset_compare(_ffi(a), _ffi(b)) < 0
end

function Base.hash(dto::DateTimeOffset, h::UInt)
    hash((dto._dt._ticks, dto._offset._ticks), h)
end

function to_string(dto::DateTimeOffset, format::DateTimeFormat = Iso8601)
    p = ffi_date_time_offset_to_string(_ffi(dto), format)
    p == C_NULL && return ""
    s = unsafe_string(p)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), p)
    s
end

function Base.string(dto::DateTimeOffset)
    to_string(dto, Iso8601)
end

function Base.show(io::IO, dto::DateTimeOffset)
    print(io, "DateTimeOffset(", Base.string(dto), ")")
end

function Base.parse(::Base.Type{DateTimeOffset}, s::AbstractString)
    cs = String(s)
    result = Ref(_FfiDateTimeOffset(Int64(0), Int64(0)))
    ok = GC.@preserve cs ffi_date_time_offset_from_string(
        Base.unsafe_convert(Cstring, cs),
        result,
    )
    ok != 0 || throw(last_error())
    _from_ffi(result[])
end

function Base.convert(::Base.Type{Dates.DateTime}, dto::DateTimeOffset)
    dt = utc_date_time(dto)
    Base.convert(Dates.DateTime, dt)
end
