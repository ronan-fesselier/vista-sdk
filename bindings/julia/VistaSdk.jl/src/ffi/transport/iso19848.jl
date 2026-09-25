@enum _FfiIso19848ValueType::Int32 begin
    _ValueTypeDecimal = 0
    _ValueTypeInteger = 1
    _ValueTypeBoolean = 2
    _ValueTypeString = 3
    _ValueTypeDateTime = 4
end

function ffi_iso19848_instance()
    ccall((:dnv_vista_sdk_iso19848_instance, VISTA_LIB), Ptr{Cvoid}, ())
end

function ffi_iso19848_version_count(iso::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_iso19848_version_count, VISTA_LIB), Csize_t, (Ptr{Cvoid},), iso)
end

function ffi_iso19848_version_at(iso::Ptr{Cvoid}, index::Integer)
    ccall(
        (:dnv_vista_sdk_iso19848_version_at, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Csize_t),
        iso,
        index,
    )
end

function ffi_iso19848_latest(iso::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_iso19848_latest, VISTA_LIB), Cint, (Ptr{Cvoid},), iso)
end

function ffi_iso19848_data_channel_type_names(iso::Ptr{Cvoid}, version::Cint)
    ccall(
        (:dnv_vista_sdk_iso19848_data_channel_type_names, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cint),
        iso,
        version,
    )
end

function ffi_iso19848_format_data_types(iso::Ptr{Cvoid}, version::Cint)
    ccall(
        (:dnv_vista_sdk_iso19848_format_data_types, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cint),
        iso,
        version,
    )
end

# DataChannelTypeName

function ffi_iso19848_data_channel_type_name_free(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_iso19848_data_channel_type_name_free, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_iso19848_data_channel_type_name_type(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_iso19848_data_channel_type_name_type, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_iso19848_data_channel_type_name_description(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_iso19848_data_channel_type_name_description, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

# DataChannelTypeNames

function ffi_iso19848_data_channel_type_names_free(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_iso19848_data_channel_type_names_free, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_iso19848_data_channel_type_names_from_string(ptr::Ptr{Cvoid}, type_::Cstring)
    ccall(
        (:dnv_vista_sdk_iso19848_data_channel_type_names_from_string, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring),
        ptr,
        type_,
    )
end

function ffi_iso19848_data_channel_type_names_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_iso19848_data_channel_type_names_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_iso19848_data_channel_type_names_at(ptr::Ptr{Cvoid}, index::Integer)
    ccall(
        (:dnv_vista_sdk_iso19848_data_channel_type_names_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

# FormatDataType

function ffi_iso19848_format_data_type_free(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_iso19848_format_data_type_free, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_iso19848_format_data_type_type(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_iso19848_format_data_type_type, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_iso19848_format_data_type_description(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_iso19848_format_data_type_description, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_iso19848_format_data_type_validate(
    ptr::Ptr{Cvoid},
    value::Cstring,
    result::Ref{Ptr{Cvoid}},
)
    ccall(
        (:dnv_vista_sdk_iso19848_format_data_type_validate, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Cstring, Ref{Ptr{Cvoid}}),
        ptr,
        value,
        result,
    )
end

# FormatDataTypes

function ffi_iso19848_format_data_types_free(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_iso19848_format_data_types_free, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_iso19848_format_data_types_from_string(ptr::Ptr{Cvoid}, type_::Cstring)
    ccall(
        (:dnv_vista_sdk_iso19848_format_data_types_from_string, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring),
        ptr,
        type_,
    )
end

function ffi_iso19848_format_data_types_count(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_iso19848_format_data_types_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_iso19848_format_data_types_at(ptr::Ptr{Cvoid}, index::Integer)
    ccall(
        (:dnv_vista_sdk_iso19848_format_data_types_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        ptr,
        index,
    )
end

# Value

function ffi_iso19848_value_from_string(value::Cstring)
    ccall(
        (:dnv_vista_sdk_iso19848_value_from_string, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring,),
        value,
    )
end

function ffi_iso19848_value_from_integer(value::Int64)
    ccall(
        (:dnv_vista_sdk_iso19848_value_from_integer, VISTA_LIB),
        Ptr{Cvoid},
        (Int64,),
        value,
    )
end

function ffi_iso19848_value_from_boolean(value::Cint)
    ccall(
        (:dnv_vista_sdk_iso19848_value_from_boolean, VISTA_LIB),
        Ptr{Cvoid},
        (Cint,),
        value,
    )
end

function ffi_iso19848_value_from_decimal(value::_FfiDecimal)
    ccall(
        (:dnv_vista_sdk_iso19848_value_from_decimal, VISTA_LIB),
        Ptr{Cvoid},
        (_FfiDecimal,),
        value,
    )
end

function ffi_iso19848_value_from_date_time(value::_FfiDateTimeOffset)
    ccall(
        (:dnv_vista_sdk_iso19848_value_from_date_time, VISTA_LIB),
        Ptr{Cvoid},
        (_FfiDateTimeOffset,),
        value,
    )
end

function ffi_iso19848_value_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_iso19848_value_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_iso19848_value_type(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_iso19848_value_type, VISTA_LIB),
        _FfiIso19848ValueType,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_iso19848_value_string(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_iso19848_value_string, VISTA_LIB), Cstring, (Ptr{Cvoid},), ptr)
end

function ffi_iso19848_value_boolean(ptr::Ptr{Cvoid}, result::Ref{Cint})
    ccall(
        (:dnv_vista_sdk_iso19848_value_boolean, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ref{Cint}),
        ptr,
        result,
    )
end

function ffi_iso19848_value_integer(ptr::Ptr{Cvoid}, result::Ref{Int64})
    ccall(
        (:dnv_vista_sdk_iso19848_value_integer, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ref{Int64}),
        ptr,
        result,
    )
end

function ffi_iso19848_value_decimal(ptr::Ptr{Cvoid}, result::Ref{_FfiDecimal})
    ccall(
        (:dnv_vista_sdk_iso19848_value_decimal, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ref{_FfiDecimal}),
        ptr,
        result,
    )
end

function ffi_iso19848_value_date_time(ptr::Ptr{Cvoid}, result::Ref{_FfiDateTimeOffset})
    ccall(
        (:dnv_vista_sdk_iso19848_value_date_time, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ref{_FfiDateTimeOffset}),
        ptr,
        result,
    )
end

function ffi_iso19848_value_to_string(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_iso19848_value_to_string, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid},),
        ptr,
    )
end
