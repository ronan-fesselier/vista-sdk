struct _FfiDateTime
    ticks::Int64
end

struct _FfiDateTimeOffset
    ticks::Int64          # DateTime part
    offset_ticks::Int64   # TimeSpan offset part
end

@enum DateTimeFormat::Int32 begin
    Iso8601 = 0
    Iso8601Precise = 1
    Iso8601PreciseTrimmed = 2
    Iso8601Millis = 3
    Iso8601Micros = 4
    Iso8601Extended = 5
    Iso8601Basic = 6
    Iso8601Date = 7
    Iso8601Time = 8
    UnixSeconds = 9
    UnixMilliseconds = 10
end

function ffi_date_time_from_ticks(ticks::Int64)
    ccall((:dnv_vista_sdk_date_time_from_ticks, VISTA_LIB), _FfiDateTime, (Int64,), ticks)
end

function ffi_date_time_from_date(year::Int32, month::Int32, day::Int32)
    ccall(
        (:dnv_vista_sdk_date_time_from_date, VISTA_LIB),
        _FfiDateTime,
        (Int32, Int32, Int32),
        year,
        month,
        day,
    )
end

function ffi_date_time_from_date_time(
    year::Int32,
    month::Int32,
    day::Int32,
    hour::Int32,
    minute::Int32,
    second::Int32,
)
    ccall(
        (:dnv_vista_sdk_date_time_from_date_time, VISTA_LIB),
        _FfiDateTime,
        (Int32, Int32, Int32, Int32, Int32, Int32),
        year,
        month,
        day,
        hour,
        minute,
        second,
    )
end

function ffi_date_time_from_date_time_millis(
    year::Int32,
    month::Int32,
    day::Int32,
    hour::Int32,
    minute::Int32,
    second::Int32,
    millisecond::Int32,
)
    ccall(
        (:dnv_vista_sdk_date_time_from_date_time_millis, VISTA_LIB),
        _FfiDateTime,
        (Int32, Int32, Int32, Int32, Int32, Int32, Int32),
        year,
        month,
        day,
        hour,
        minute,
        second,
        millisecond,
    )
end

function ffi_date_time_from_string(str::Cstring, result::Ref{_FfiDateTime})
    ccall(
        (:dnv_vista_sdk_date_time_from_string, VISTA_LIB),
        Cint,
        (Cstring, Ptr{_FfiDateTime}),
        str,
        result,
    )
end

function ffi_date_time_from_epoch_seconds(seconds::Int64)
    ccall(
        (:dnv_vista_sdk_date_time_from_epoch_seconds, VISTA_LIB),
        _FfiDateTime,
        (Int64,),
        seconds,
    )
end

function ffi_date_time_from_epoch_milliseconds(ms::Int64)
    ccall(
        (:dnv_vista_sdk_date_time_from_epoch_milliseconds, VISTA_LIB),
        _FfiDateTime,
        (Int64,),
        ms,
    )
end

function ffi_date_time_utc_now()
    ccall((:dnv_vista_sdk_date_time_utc_now, VISTA_LIB), _FfiDateTime, ())
end

function ffi_date_time_min()
    ccall((:dnv_vista_sdk_date_time_min, VISTA_LIB), _FfiDateTime, ())
end

function ffi_date_time_max()
    ccall((:dnv_vista_sdk_date_time_max, VISTA_LIB), _FfiDateTime, ())
end

function ffi_date_time_epoch()
    ccall((:dnv_vista_sdk_date_time_epoch, VISTA_LIB), _FfiDateTime, ())
end

function ffi_date_time_year(dt::_FfiDateTime)
    ccall((:dnv_vista_sdk_date_time_year, VISTA_LIB), Int32, (_FfiDateTime,), dt)
end

function ffi_date_time_month(dt::_FfiDateTime)
    ccall((:dnv_vista_sdk_date_time_month, VISTA_LIB), Int32, (_FfiDateTime,), dt)
end

function ffi_date_time_day(dt::_FfiDateTime)
    ccall((:dnv_vista_sdk_date_time_day, VISTA_LIB), Int32, (_FfiDateTime,), dt)
end

function ffi_date_time_hour(dt::_FfiDateTime)
    ccall((:dnv_vista_sdk_date_time_hour, VISTA_LIB), Int32, (_FfiDateTime,), dt)
end

function ffi_date_time_minute(dt::_FfiDateTime)
    ccall((:dnv_vista_sdk_date_time_minute, VISTA_LIB), Int32, (_FfiDateTime,), dt)
end

function ffi_date_time_second(dt::_FfiDateTime)
    ccall((:dnv_vista_sdk_date_time_second, VISTA_LIB), Int32, (_FfiDateTime,), dt)
end

function ffi_date_time_millisecond(dt::_FfiDateTime)
    ccall((:dnv_vista_sdk_date_time_millisecond, VISTA_LIB), Int32, (_FfiDateTime,), dt)
end

function ffi_date_time_microsecond(dt::_FfiDateTime)
    ccall((:dnv_vista_sdk_date_time_microsecond, VISTA_LIB), Int32, (_FfiDateTime,), dt)
end

function ffi_date_time_nanosecond(dt::_FfiDateTime)
    ccall((:dnv_vista_sdk_date_time_nanosecond, VISTA_LIB), Int32, (_FfiDateTime,), dt)
end

function ffi_date_time_day_of_week(dt::_FfiDateTime)
    ccall((:dnv_vista_sdk_date_time_day_of_week, VISTA_LIB), Int32, (_FfiDateTime,), dt)
end

function ffi_date_time_day_of_year(dt::_FfiDateTime)
    ccall((:dnv_vista_sdk_date_time_day_of_year, VISTA_LIB), Int32, (_FfiDateTime,), dt)
end

function ffi_date_time_to_epoch_seconds(dt::_FfiDateTime)
    ccall(
        (:dnv_vista_sdk_date_time_to_epoch_seconds, VISTA_LIB),
        Int64,
        (_FfiDateTime,),
        dt,
    )
end

function ffi_date_time_to_epoch_milliseconds(dt::_FfiDateTime)
    ccall(
        (:dnv_vista_sdk_date_time_to_epoch_milliseconds, VISTA_LIB),
        Int64,
        (_FfiDateTime,),
        dt,
    )
end

function ffi_date_time_date(dt::_FfiDateTime)
    ccall((:dnv_vista_sdk_date_time_date, VISTA_LIB), _FfiDateTime, (_FfiDateTime,), dt)
end

function ffi_date_time_time_of_day(dt::_FfiDateTime)
    ccall(
        (:dnv_vista_sdk_date_time_time_of_day, VISTA_LIB),
        _FfiTimeSpan,
        (_FfiDateTime,),
        dt,
    )
end

function ffi_date_time_add_days(dt::_FfiDateTime, days::Float64)
    ccall(
        (:dnv_vista_sdk_date_time_add_days, VISTA_LIB),
        _FfiDateTime,
        (_FfiDateTime, Float64),
        dt,
        days,
    )
end

function ffi_date_time_add_hours(dt::_FfiDateTime, hours::Float64)
    ccall(
        (:dnv_vista_sdk_date_time_add_hours, VISTA_LIB),
        _FfiDateTime,
        (_FfiDateTime, Float64),
        dt,
        hours,
    )
end

function ffi_date_time_add_minutes(dt::_FfiDateTime, minutes::Float64)
    ccall(
        (:dnv_vista_sdk_date_time_add_minutes, VISTA_LIB),
        _FfiDateTime,
        (_FfiDateTime, Float64),
        dt,
        minutes,
    )
end

function ffi_date_time_add_seconds(dt::_FfiDateTime, seconds::Float64)
    ccall(
        (:dnv_vista_sdk_date_time_add_seconds, VISTA_LIB),
        _FfiDateTime,
        (_FfiDateTime, Float64),
        dt,
        seconds,
    )
end

function ffi_date_time_add_milliseconds(dt::_FfiDateTime, ms::Float64)
    ccall(
        (:dnv_vista_sdk_date_time_add_milliseconds, VISTA_LIB),
        _FfiDateTime,
        (_FfiDateTime, Float64),
        dt,
        ms,
    )
end

function ffi_date_time_add_months(dt::_FfiDateTime, months::Int32)
    ccall(
        (:dnv_vista_sdk_date_time_add_months, VISTA_LIB),
        _FfiDateTime,
        (_FfiDateTime, Int32),
        dt,
        months,
    )
end

function ffi_date_time_add_years(dt::_FfiDateTime, years::Int32)
    ccall(
        (:dnv_vista_sdk_date_time_add_years, VISTA_LIB),
        _FfiDateTime,
        (_FfiDateTime, Int32),
        dt,
        years,
    )
end

function ffi_date_time_add_time_span(dt::_FfiDateTime, ts::_FfiTimeSpan)
    ccall(
        (:dnv_vista_sdk_date_time_add_time_span, VISTA_LIB),
        _FfiDateTime,
        (_FfiDateTime, _FfiTimeSpan),
        dt,
        ts,
    )
end

function ffi_date_time_subtract_time_span(dt::_FfiDateTime, ts::_FfiTimeSpan)
    ccall(
        (:dnv_vista_sdk_date_time_subtract_time_span, VISTA_LIB),
        _FfiDateTime,
        (_FfiDateTime, _FfiTimeSpan),
        dt,
        ts,
    )
end

function ffi_date_time_subtract(a::_FfiDateTime, b::_FfiDateTime)
    ccall(
        (:dnv_vista_sdk_date_time_subtract, VISTA_LIB),
        _FfiTimeSpan,
        (_FfiDateTime, _FfiDateTime),
        a,
        b,
    )
end

function ffi_date_time_is_valid(dt::_FfiDateTime)
    ccall((:dnv_vista_sdk_date_time_is_valid, VISTA_LIB), Cint, (_FfiDateTime,), dt)
end

function ffi_date_time_is_leap_year(year::Int32)
    ccall((:dnv_vista_sdk_date_time_is_leap_year, VISTA_LIB), Cint, (Int32,), year)
end

function ffi_date_time_days_in_month(year::Int32, month::Int32)
    ccall(
        (:dnv_vista_sdk_date_time_days_in_month, VISTA_LIB),
        Int32,
        (Int32, Int32),
        year,
        month,
    )
end

function ffi_date_time_to_string(dt::_FfiDateTime, format::DateTimeFormat)
    ccall(
        (:dnv_vista_sdk_date_time_to_string, VISTA_LIB),
        Ptr{UInt8},
        (_FfiDateTime, Int32),
        dt,
        Int32(format),
    )
end

# DateTimeOffset FFI

function ffi_date_time_offset_create(dt::_FfiDateTime, offset::_FfiTimeSpan)
    ccall(
        (:dnv_vista_sdk_date_time_offset_create, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTime, _FfiTimeSpan),
        dt,
        offset,
    )
end

function ffi_date_time_offset_from_date_time(dt::_FfiDateTime)
    ccall(
        (:dnv_vista_sdk_date_time_offset_from_date_time, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTime,),
        dt,
    )
end

function ffi_date_time_offset_from_ticks(ticks::Int64, offset::_FfiTimeSpan)
    ccall(
        (:dnv_vista_sdk_date_time_offset_from_ticks, VISTA_LIB),
        _FfiDateTimeOffset,
        (Int64, _FfiTimeSpan),
        ticks,
        offset,
    )
end

function ffi_date_time_offset_from_string(str::Cstring, result::Ref{_FfiDateTimeOffset})
    ccall(
        (:dnv_vista_sdk_date_time_offset_from_string, VISTA_LIB),
        Cint,
        (Cstring, Ptr{_FfiDateTimeOffset}),
        str,
        result,
    )
end

function ffi_date_time_offset_from_epoch_seconds(seconds::Int64)
    ccall(
        (:dnv_vista_sdk_date_time_offset_from_epoch_seconds, VISTA_LIB),
        _FfiDateTimeOffset,
        (Int64,),
        seconds,
    )
end

function ffi_date_time_offset_from_epoch_milliseconds(ms::Int64)
    ccall(
        (:dnv_vista_sdk_date_time_offset_from_epoch_milliseconds, VISTA_LIB),
        _FfiDateTimeOffset,
        (Int64,),
        ms,
    )
end

function ffi_date_time_offset_from_filetime(filetime::Int64)
    ccall(
        (:dnv_vista_sdk_date_time_offset_from_filetime, VISTA_LIB),
        _FfiDateTimeOffset,
        (Int64,),
        filetime,
    )
end

function ffi_date_time_offset_now()
    ccall((:dnv_vista_sdk_date_time_offset_now, VISTA_LIB), _FfiDateTimeOffset, ())
end

function ffi_date_time_offset_utc_now()
    ccall((:dnv_vista_sdk_date_time_offset_utc_now, VISTA_LIB), _FfiDateTimeOffset, ())
end

function ffi_date_time_offset_today()
    ccall((:dnv_vista_sdk_date_time_offset_today, VISTA_LIB), _FfiDateTimeOffset, ())
end

function ffi_date_time_offset_min()
    ccall((:dnv_vista_sdk_date_time_offset_min, VISTA_LIB), _FfiDateTimeOffset, ())
end

function ffi_date_time_offset_max()
    ccall((:dnv_vista_sdk_date_time_offset_max, VISTA_LIB), _FfiDateTimeOffset, ())
end

function ffi_date_time_offset_epoch()
    ccall((:dnv_vista_sdk_date_time_offset_epoch, VISTA_LIB), _FfiDateTimeOffset, ())
end

function ffi_date_time_offset_date_time(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_date_time, VISTA_LIB),
        _FfiDateTime,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_offset(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_offset, VISTA_LIB),
        _FfiTimeSpan,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_utc_date_time(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_utc_date_time, VISTA_LIB),
        _FfiDateTime,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_local_date_time(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_local_date_time, VISTA_LIB),
        _FfiDateTime,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_ticks(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_ticks, VISTA_LIB),
        Int64,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_utc_ticks(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_utc_ticks, VISTA_LIB),
        Int64,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_year(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_year, VISTA_LIB),
        Int32,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_month(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_month, VISTA_LIB),
        Int32,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_day(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_day, VISTA_LIB),
        Int32,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_hour(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_hour, VISTA_LIB),
        Int32,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_minute(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_minute, VISTA_LIB),
        Int32,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_second(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_second, VISTA_LIB),
        Int32,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_millisecond(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_millisecond, VISTA_LIB),
        Int32,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_microsecond(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_microsecond, VISTA_LIB),
        Int32,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_nanosecond(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_nanosecond, VISTA_LIB),
        Int32,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_day_of_week(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_day_of_week, VISTA_LIB),
        Int32,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_day_of_year(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_day_of_year, VISTA_LIB),
        Int32,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_total_offset_minutes(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_total_offset_minutes, VISTA_LIB),
        Int32,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_to_epoch_seconds(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_to_epoch_seconds, VISTA_LIB),
        Int64,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_to_epoch_milliseconds(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_to_epoch_milliseconds, VISTA_LIB),
        Int64,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_date(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_date, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_time_of_day(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_time_of_day, VISTA_LIB),
        _FfiTimeSpan,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_to_offset(dto::_FfiDateTimeOffset, new_offset::_FfiTimeSpan)
    ccall(
        (:dnv_vista_sdk_date_time_offset_to_offset, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTimeOffset, _FfiTimeSpan),
        dto,
        new_offset,
    )
end

function ffi_date_time_offset_to_universal_time(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_to_universal_time, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_to_local_time(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_to_local_time, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_to_filetime(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_to_filetime, VISTA_LIB),
        Int64,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_add_time_span(dto::_FfiDateTimeOffset, ts::_FfiTimeSpan)
    ccall(
        (:dnv_vista_sdk_date_time_offset_add_time_span, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTimeOffset, _FfiTimeSpan),
        dto,
        ts,
    )
end

function ffi_date_time_offset_add_days(dto::_FfiDateTimeOffset, days::Float64)
    ccall(
        (:dnv_vista_sdk_date_time_offset_add_days, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTimeOffset, Float64),
        dto,
        days,
    )
end

function ffi_date_time_offset_add_hours(dto::_FfiDateTimeOffset, hours::Float64)
    ccall(
        (:dnv_vista_sdk_date_time_offset_add_hours, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTimeOffset, Float64),
        dto,
        hours,
    )
end

function ffi_date_time_offset_add_minutes(dto::_FfiDateTimeOffset, minutes::Float64)
    ccall(
        (:dnv_vista_sdk_date_time_offset_add_minutes, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTimeOffset, Float64),
        dto,
        minutes,
    )
end

function ffi_date_time_offset_add_seconds(dto::_FfiDateTimeOffset, seconds::Float64)
    ccall(
        (:dnv_vista_sdk_date_time_offset_add_seconds, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTimeOffset, Float64),
        dto,
        seconds,
    )
end

function ffi_date_time_offset_add_milliseconds(dto::_FfiDateTimeOffset, ms::Float64)
    ccall(
        (:dnv_vista_sdk_date_time_offset_add_milliseconds, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTimeOffset, Float64),
        dto,
        ms,
    )
end

function ffi_date_time_offset_add_months(dto::_FfiDateTimeOffset, months::Int32)
    ccall(
        (:dnv_vista_sdk_date_time_offset_add_months, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTimeOffset, Int32),
        dto,
        months,
    )
end

function ffi_date_time_offset_add_years(dto::_FfiDateTimeOffset, years::Int32)
    ccall(
        (:dnv_vista_sdk_date_time_offset_add_years, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTimeOffset, Int32),
        dto,
        years,
    )
end

function ffi_date_time_offset_add_ticks(dto::_FfiDateTimeOffset, ticks::Int64)
    ccall(
        (:dnv_vista_sdk_date_time_offset_add_ticks, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTimeOffset, Int64),
        dto,
        ticks,
    )
end

function ffi_date_time_offset_subtract(a::_FfiDateTimeOffset, b::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_subtract, VISTA_LIB),
        _FfiTimeSpan,
        (_FfiDateTimeOffset, _FfiDateTimeOffset),
        a,
        b,
    )
end

function ffi_date_time_offset_subtract_time_span(dto::_FfiDateTimeOffset, ts::_FfiTimeSpan)
    ccall(
        (:dnv_vista_sdk_date_time_offset_subtract_time_span, VISTA_LIB),
        _FfiDateTimeOffset,
        (_FfiDateTimeOffset, _FfiTimeSpan),
        dto,
        ts,
    )
end

function ffi_date_time_offset_equals(a::_FfiDateTimeOffset, b::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_equals, VISTA_LIB),
        Cint,
        (_FfiDateTimeOffset, _FfiDateTimeOffset),
        a,
        b,
    )
end

function ffi_date_time_offset_equals_exact(a::_FfiDateTimeOffset, b::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_equals_exact, VISTA_LIB),
        Cint,
        (_FfiDateTimeOffset, _FfiDateTimeOffset),
        a,
        b,
    )
end

function ffi_date_time_offset_compare(a::_FfiDateTimeOffset, b::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_compare, VISTA_LIB),
        Cint,
        (_FfiDateTimeOffset, _FfiDateTimeOffset),
        a,
        b,
    )
end

function ffi_date_time_offset_is_valid(dto::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_date_time_offset_is_valid, VISTA_LIB),
        Cint,
        (_FfiDateTimeOffset,),
        dto,
    )
end

function ffi_date_time_offset_to_string(dto::_FfiDateTimeOffset, format::DateTimeFormat)
    ccall(
        (:dnv_vista_sdk_date_time_offset_to_string, VISTA_LIB),
        Ptr{UInt8},
        (_FfiDateTimeOffset, Int32),
        dto,
        Int32(format),
    )
end
