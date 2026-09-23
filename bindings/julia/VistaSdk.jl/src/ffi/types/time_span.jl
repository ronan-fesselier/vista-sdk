struct _FfiTimeSpan
    ticks::Int64
end

function ffi_time_span_from_ticks(ticks::Int64)
    ccall((:dnv_vista_sdk_time_span_from_ticks, VISTA_LIB), _FfiTimeSpan, (Int64,), ticks)
end

function ffi_time_span_from_string(str::Cstring, result::Ref{_FfiTimeSpan})
    ccall(
        (:dnv_vista_sdk_time_span_from_string, VISTA_LIB),
        Cint,
        (Cstring, Ptr{_FfiTimeSpan}),
        str,
        result,
    )
end

function ffi_time_span_from_days(days::Float64)
    ccall((:dnv_vista_sdk_time_span_from_days, VISTA_LIB), _FfiTimeSpan, (Float64,), days)
end

function ffi_time_span_from_hours(hours::Float64)
    ccall((:dnv_vista_sdk_time_span_from_hours, VISTA_LIB), _FfiTimeSpan, (Float64,), hours)
end

function ffi_time_span_from_minutes(minutes::Float64)
    ccall(
        (:dnv_vista_sdk_time_span_from_minutes, VISTA_LIB),
        _FfiTimeSpan,
        (Float64,),
        minutes,
    )
end

function ffi_time_span_from_seconds(seconds::Float64)
    ccall(
        (:dnv_vista_sdk_time_span_from_seconds, VISTA_LIB),
        _FfiTimeSpan,
        (Float64,),
        seconds,
    )
end

function ffi_time_span_from_milliseconds(ms::Float64)
    ccall(
        (:dnv_vista_sdk_time_span_from_milliseconds, VISTA_LIB),
        _FfiTimeSpan,
        (Float64,),
        ms,
    )
end

function ffi_time_span_from_microseconds(us::Float64)
    ccall(
        (:dnv_vista_sdk_time_span_from_microseconds, VISTA_LIB),
        _FfiTimeSpan,
        (Float64,),
        us,
    )
end

function ffi_time_span_days(ts::_FfiTimeSpan)
    ccall((:dnv_vista_sdk_time_span_days, VISTA_LIB), Float64, (_FfiTimeSpan,), ts)
end

function ffi_time_span_hours(ts::_FfiTimeSpan)
    ccall((:dnv_vista_sdk_time_span_hours, VISTA_LIB), Float64, (_FfiTimeSpan,), ts)
end

function ffi_time_span_minutes(ts::_FfiTimeSpan)
    ccall((:dnv_vista_sdk_time_span_minutes, VISTA_LIB), Float64, (_FfiTimeSpan,), ts)
end

function ffi_time_span_seconds(ts::_FfiTimeSpan)
    ccall((:dnv_vista_sdk_time_span_seconds, VISTA_LIB), Float64, (_FfiTimeSpan,), ts)
end

function ffi_time_span_milliseconds(ts::_FfiTimeSpan)
    ccall((:dnv_vista_sdk_time_span_milliseconds, VISTA_LIB), Float64, (_FfiTimeSpan,), ts)
end

function ffi_time_span_microseconds(ts::_FfiTimeSpan)
    ccall((:dnv_vista_sdk_time_span_microseconds, VISTA_LIB), Float64, (_FfiTimeSpan,), ts)
end

function ffi_time_span_nanoseconds(ts::_FfiTimeSpan)
    ccall((:dnv_vista_sdk_time_span_nanoseconds, VISTA_LIB), Float64, (_FfiTimeSpan,), ts)
end

function ffi_time_span_add(a::_FfiTimeSpan, b::_FfiTimeSpan)
    ccall(
        (:dnv_vista_sdk_time_span_add, VISTA_LIB),
        _FfiTimeSpan,
        (_FfiTimeSpan, _FfiTimeSpan),
        a,
        b,
    )
end

function ffi_time_span_subtract(a::_FfiTimeSpan, b::_FfiTimeSpan)
    ccall(
        (:dnv_vista_sdk_time_span_subtract, VISTA_LIB),
        _FfiTimeSpan,
        (_FfiTimeSpan, _FfiTimeSpan),
        a,
        b,
    )
end

function ffi_time_span_negate(ts::_FfiTimeSpan)
    ccall((:dnv_vista_sdk_time_span_negate, VISTA_LIB), _FfiTimeSpan, (_FfiTimeSpan,), ts)
end

function ffi_time_span_multiply(ts::_FfiTimeSpan, multiplier::Float64)
    ccall(
        (:dnv_vista_sdk_time_span_multiply, VISTA_LIB),
        _FfiTimeSpan,
        (_FfiTimeSpan, Float64),
        ts,
        multiplier,
    )
end

function ffi_time_span_divide(ts::_FfiTimeSpan, divisor::Float64)
    ccall(
        (:dnv_vista_sdk_time_span_divide, VISTA_LIB),
        _FfiTimeSpan,
        (_FfiTimeSpan, Float64),
        ts,
        divisor,
    )
end

function ffi_time_span_ratio(a::_FfiTimeSpan, b::_FfiTimeSpan)
    ccall(
        (:dnv_vista_sdk_time_span_ratio, VISTA_LIB),
        Float64,
        (_FfiTimeSpan, _FfiTimeSpan),
        a,
        b,
    )
end

function ffi_time_span_to_string(ts::_FfiTimeSpan)
    ccall((:dnv_vista_sdk_time_span_to_string, VISTA_LIB), Ptr{UInt8}, (_FfiTimeSpan,), ts)
end
