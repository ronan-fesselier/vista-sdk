function ffi_parsing_errors_create_empty()
    ccall((:dnv_vista_sdk_parsing_errors_create_empty, VISTA_LIB), Ptr{Cvoid}, ())
end

function ffi_parsing_errors_free(errors::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_parsing_errors_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), errors)
end

function ffi_parsing_errors_count(errors::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_parsing_errors_count, VISTA_LIB), Csize_t, (Ptr{Cvoid},), errors)
end

function ffi_parsing_errors_has_errors(errors::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_parsing_errors_has_errors, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        errors,
    )
end

function ffi_parsing_errors_has_error_type(errors::Ptr{Cvoid}, type::Cstring)
    ccall(
        (:dnv_vista_sdk_parsing_errors_has_error_type, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Cstring),
        errors,
        type,
    )
end

function ffi_parsing_errors_type_at(errors::Ptr{Cvoid}, index::Integer)
    ccall(
        (:dnv_vista_sdk_parsing_errors_type_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        errors,
        index,
    )
end

function ffi_parsing_errors_message_at(errors::Ptr{Cvoid}, index::Integer)
    ccall(
        (:dnv_vista_sdk_parsing_errors_message_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        errors,
        index,
    )
end

function ffi_parsing_errors_to_string(errors::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_parsing_errors_to_string, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        errors,
    )
end
