@enum _FfiDocumentKind::Int32 begin
    _DocKindNull = 0
    _DocKindBoolean = 1
    _DocKindInteger = 2
    _DocKindDouble = 3
    _DocKindString = 4
    _DocKindArray = 5
    _DocKindObject = 6
end

function ffi_sd_null()
    ccall((:dnv_vista_sdk_serializable_document_null, VISTA_LIB), Ptr{Cvoid}, ())
end

function ffi_sd_from_boolean(value::Cint)
    ccall(
        (:dnv_vista_sdk_serializable_document_from_boolean, VISTA_LIB),
        Ptr{Cvoid},
        (Cint,),
        value,
    )
end

function ffi_sd_from_integer(value::Int64)
    ccall(
        (:dnv_vista_sdk_serializable_document_from_integer, VISTA_LIB),
        Ptr{Cvoid},
        (Int64,),
        value,
    )
end

function ffi_sd_from_double(value::Cdouble)
    ccall(
        (:dnv_vista_sdk_serializable_document_from_double, VISTA_LIB),
        Ptr{Cvoid},
        (Cdouble,),
        value,
    )
end

function ffi_sd_from_string(value::Cstring)
    ccall(
        (:dnv_vista_sdk_serializable_document_from_string, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring,),
        value,
    )
end

function ffi_sd_array()
    ccall((:dnv_vista_sdk_serializable_document_array, VISTA_LIB), Ptr{Cvoid}, ())
end

function ffi_sd_object()
    ccall((:dnv_vista_sdk_serializable_document_object, VISTA_LIB), Ptr{Cvoid}, ())
end

function ffi_sd_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_serializable_document_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_sd_clone(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_clone, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_equals(a::Ptr{Cvoid}, b::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_equals, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        a,
        b,
    )
end

function ffi_sd_kind(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_kind, VISTA_LIB),
        _FfiDocumentKind,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_is_null(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_is_null, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_is_boolean(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_is_boolean, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_is_integer(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_is_integer, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_is_double(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_is_double, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_is_string(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_is_string, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_is_array(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_is_array, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_is_object(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_is_object, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_as_boolean(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_as_boolean, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_as_integer(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_as_integer, VISTA_LIB),
        Int64,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_as_double(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_as_double, VISTA_LIB),
        Cdouble,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_as_string(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_as_string, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_array_size(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_array_size, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_array_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_serializable_document_array_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_sd_object_size(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_object_size, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_sd_object_key_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_serializable_document_object_key_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_sd_object_value_at(ptr::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_serializable_document_object_value_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

function ffi_sd_find(ptr::Ptr{Cvoid}, key::Cstring)
    ccall(
        (:dnv_vista_sdk_serializable_document_find, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring),
        ptr,
        key,
    )
end

function ffi_sd_contains(ptr::Ptr{Cvoid}, key::Cstring)
    ccall(
        (:dnv_vista_sdk_serializable_document_contains, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Cstring),
        ptr,
        key,
    )
end

function ffi_sd_set(ptr::Ptr{Cvoid}, key::Cstring, value::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_set, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring, Ptr{Cvoid}),
        ptr,
        key,
        value,
    )
end

function ffi_sd_push_back(ptr::Ptr{Cvoid}, value::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_serializable_document_push_back, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        ptr,
        value,
    )
end
