"""
    DataChannelTypeName

A data channel type name entry from the ISO 19848 specification.
"""
struct DataChannelTypeName
    type_::String
    description::String
end

Base.show(io::IO, t::DataChannelTypeName) = print(io, "DataChannelTypeName(", t.type_, ")")

"""
    DataChannelTypeNames

A collection of data channel type names for a specific ISO 19848 version.
"""
mutable struct DataChannelTypeNames
    _ptr::Ptr{Cvoid}

    function DataChannelTypeNames(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("DataChannelTypeNames: null pointer")
        obj = new(ptr)
        finalizer(obj) do o
            ffi_iso19848_data_channel_type_names_free(o._ptr)
        end
        obj
    end
end

function Base.length(names::DataChannelTypeNames)
    Int(ffi_iso19848_data_channel_type_names_count(names._ptr))
end

function _read_data_channel_type_name(ptr::Ptr{Cvoid})
    type_ = unsafe_string(ffi_iso19848_data_channel_type_name_type(ptr))
    description = unsafe_string(ffi_iso19848_data_channel_type_name_description(ptr))
    DataChannelTypeName(type_, description)
end

function Base.getindex(names::DataChannelTypeNames, index::Integer)
    (index < 1 || index > length(names)) && throw(BoundsError(names, index))
    ptr = ffi_iso19848_data_channel_type_names_at(names._ptr, index - 1)
    ptr == C_NULL && error("DataChannelTypeNames: null at index $index")
    _read_data_channel_type_name(ptr)
end

function find(names::DataChannelTypeNames, type_::AbstractString)
    GC.@preserve type_ begin
        ptr = ffi_iso19848_data_channel_type_names_from_string(
            names._ptr,
            Base.unsafe_convert(Cstring, type_),
        )
    end
    ptr == C_NULL && return nothing
    result = _read_data_channel_type_name(ptr)
    ffi_iso19848_data_channel_type_name_free(ptr)
    result
end

function Base.iterate(names::DataChannelTypeNames, i::Int = 1)
    i > length(names) && return nothing
    (names[i], i + 1)
end

"""
    FormatDataType

A format data type entry from the ISO 19848 specification (owned).
"""
mutable struct FormatDataType
    _ptr::Ptr{Cvoid}

    function FormatDataType(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("FormatDataType: null pointer")
        obj = new(ptr)
        finalizer(obj) do o
            ffi_iso19848_format_data_type_free(o._ptr)
        end
        obj
    end
end

function type_(fdt::FormatDataType)
    unsafe_string(ffi_iso19848_format_data_type_type(fdt._ptr))
end

function description(fdt::FormatDataType)
    unsafe_string(ffi_iso19848_format_data_type_description(fdt._ptr))
end

function validate(fdt::FormatDataType, value::AbstractString)
    result = Ref{Ptr{Cvoid}}(C_NULL)
    GC.@preserve value begin
        ok = ffi_iso19848_format_data_type_validate(
            fdt._ptr,
            Base.unsafe_convert(Cstring, value),
            result,
        )
    end
    (ok == 0 || result[] == C_NULL) && return nothing
    v = _read_iso19848_value(result[])
    ffi_iso19848_value_free(result[])
    v
end

Base.show(io::IO, fdt::FormatDataType) = print(io, "FormatDataType(", type_(fdt), ")")

"""
    FormatDataTypeRef

A borrowed view of a `FormatDataType` entry. Valid as long as the parent `FormatDataTypes` is alive.
"""
struct FormatDataTypeRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

function type_(fdt::FormatDataTypeRef)
    unsafe_string(ffi_iso19848_format_data_type_type(fdt._ptr))
end

function description(fdt::FormatDataTypeRef)
    unsafe_string(ffi_iso19848_format_data_type_description(fdt._ptr))
end

function validate(fdt::FormatDataTypeRef, value::AbstractString)
    result = Ref{Ptr{Cvoid}}(C_NULL)
    GC.@preserve value begin
        ok = ffi_iso19848_format_data_type_validate(
            fdt._ptr,
            Base.unsafe_convert(Cstring, value),
            result,
        )
    end
    (ok == 0 || result[] == C_NULL) && return nothing
    v = _read_iso19848_value(result[])
    ffi_iso19848_value_free(result[])
    v
end

Base.show(io::IO, fdt::FormatDataTypeRef) = print(io, "FormatDataTypeRef(", type_(fdt), ")")

"""
    FormatDataTypes

A collection of format data types for a specific ISO 19848 version.
"""
mutable struct FormatDataTypes
    _ptr::Ptr{Cvoid}

    function FormatDataTypes(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("FormatDataTypes: null pointer")
        obj = new(ptr)
        finalizer(obj) do o
            ffi_iso19848_format_data_types_free(o._ptr)
        end
        obj
    end
end

function Base.length(types::FormatDataTypes)
    Int(ffi_iso19848_format_data_types_count(types._ptr))
end

function Base.getindex(types::FormatDataTypes, index::Integer)
    (index < 1 || index > length(types)) && throw(BoundsError(types, index))
    ptr = ffi_iso19848_format_data_types_at(types._ptr, index - 1)
    ptr == C_NULL && error("FormatDataTypes: null at index $index")
    FormatDataTypeRef(ptr, types)
end

function find(types::FormatDataTypes, type_::AbstractString)
    GC.@preserve type_ begin
        ptr = ffi_iso19848_format_data_types_from_string(
            types._ptr,
            Base.unsafe_convert(Cstring, type_),
        )
    end
    ptr == C_NULL && return nothing
    FormatDataType(ptr)
end

function Base.iterate(types::FormatDataTypes, i::Int = 1)
    i > length(types) && return nothing
    (types[i], i + 1)
end

# Value

"""
    Iso19848Value

A discriminated union holding one ISO 19848 data value.
"""
const Iso19848Value = Union{Decimal,Int64,Bool,String,DateTimeOffset}

function _read_iso19848_value(ptr::Ptr{Cvoid})::Iso19848Value
    vtype = ffi_iso19848_value_type(ptr)
    if vtype == _ValueTypeDecimal
        raw = Ref{_FfiDecimal}(_FfiDecimal(0, 0, 0, 0))
        ffi_iso19848_value_decimal(ptr, raw)
        return _from_ffi(raw[])
    elseif vtype == _ValueTypeInteger
        raw = Ref{Int64}(0)
        ffi_iso19848_value_integer(ptr, raw)
        return raw[]
    elseif vtype == _ValueTypeBoolean
        raw = Ref{Cint}(0)
        ffi_iso19848_value_boolean(ptr, raw)
        return raw[] != 0
    elseif vtype == _ValueTypeString
        return unsafe_string(ffi_iso19848_value_string(ptr))
    else
        raw = Ref{_FfiDateTimeOffset}(_FfiDateTimeOffset(0, 0))
        ffi_iso19848_value_date_time(ptr, raw)
        return _from_ffi(raw[])
    end
end

function iso19848_value_from_string(value::AbstractString)::Union{Iso19848Value,Nothing}
    GC.@preserve value begin
        ptr = ffi_iso19848_value_from_string(Base.unsafe_convert(Cstring, value))
    end
    ptr == C_NULL && return nothing
    result = _read_iso19848_value(ptr)
    ffi_iso19848_value_free(ptr)
    result
end

function iso19848_value_from_integer(value::Integer)::Iso19848Value
    ptr = ffi_iso19848_value_from_integer(Int64(value))
    result = _read_iso19848_value(ptr)
    ffi_iso19848_value_free(ptr)
    result
end

function iso19848_value_from_boolean(value::Bool)::Iso19848Value
    ptr = ffi_iso19848_value_from_boolean(Cint(value))
    result = _read_iso19848_value(ptr)
    ffi_iso19848_value_free(ptr)
    result
end

function iso19848_value_from_decimal(value::Decimal)::Iso19848Value
    ptr = ffi_iso19848_value_from_decimal(_ffi(value))
    result = _read_iso19848_value(ptr)
    ffi_iso19848_value_free(ptr)
    result
end

function iso19848_value_from_date_time(value::DateTimeOffset)::Iso19848Value
    ptr = ffi_iso19848_value_from_date_time(_ffi(value))
    result = _read_iso19848_value(ptr)
    ffi_iso19848_value_free(ptr)
    result
end

function iso19848_value_to_string(value::Iso19848Value)::String
    if value isa String
        return value
    end
    ptr = if value isa Int64
        ffi_iso19848_value_from_integer(value)
    elseif value isa Bool
        ffi_iso19848_value_from_boolean(Cint(value))
    elseif value isa Decimal
        ffi_iso19848_value_from_decimal(_ffi(value))
    else
        ffi_iso19848_value_from_date_time(_ffi(value))
    end
    s_ptr = ffi_iso19848_value_to_string(ptr)
    ffi_iso19848_value_free(ptr)
    result = unsafe_string(s_ptr)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), s_ptr)
    result
end

"""
    Iso19848

The ISO 19848 singleton - entry point for accessing versioned type definitions.
"""
struct Iso19848
    _ptr::Ptr{Cvoid}
end

function Iso19848()
    ptr = ffi_iso19848_instance()
    ptr == C_NULL && error("iso19848 instance returned null")
    Iso19848(ptr)
end

function _ffi_version_to_julia(v::Integer)::Iso19848Version
    v == 0 ? V2018 : V2024
end

function _julia_version_to_ffi(v::Iso19848Version)::Cint
    v == V2018 ? Cint(0) : Cint(1)
end

function versions(iso::Iso19848)
    n = Int(ffi_iso19848_version_count(iso._ptr))
    [_ffi_version_to_julia(ffi_iso19848_version_at(iso._ptr, i - 1)) for i = 1:n]
end

function latest(iso::Iso19848)
    _ffi_version_to_julia(ffi_iso19848_latest(iso._ptr))
end

function data_channel_type_names(iso::Iso19848, version::Iso19848Version)
    ptr = ffi_iso19848_data_channel_type_names(iso._ptr, _julia_version_to_ffi(version))
    ptr == C_NULL && return nothing
    DataChannelTypeNames(ptr)
end

function format_data_types(iso::Iso19848, version::Iso19848Version)
    ptr = ffi_iso19848_format_data_types(iso._ptr, _julia_version_to_ffi(version))
    ptr == C_NULL && return nothing
    FormatDataTypes(ptr)
end

const ISO19848 = Iso19848()
