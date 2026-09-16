@enum ErrorKind::Int32 begin
    ErrorNone = 0
    InvalidArgument = 1
    OutOfRange = 2
    Domain = 3
    Overflow = 4
    Runtime = 5
end

export ErrorKind

function ffi_last_error_kind()
    ccall((:dnv_vista_sdk_last_error_kind, VISTA_LIB), Cint, ())
end

function ffi_last_error_message()
    ccall((:dnv_vista_sdk_last_error_message, VISTA_LIB), Cstring, ())
end

function ffi_clear_error()
    ccall((:dnv_vista_sdk_clear_error, VISTA_LIB), Cvoid, ())
end
