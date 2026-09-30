"""
    TsdDtoTimeSpanRef

Borrowed view of a DTO time span (ISO 8601 string pair). Do not store past the lifetime
of the owning [`TsdDtoPackage`](@ref).
"""
struct TsdDtoTimeSpanRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

function start_time(r::TsdDtoTimeSpanRef)
    unsafe_string(ffi_tsd_dto_time_span_get_start(r._ptr))
end

function set_start_time!(r::TsdDtoTimeSpanRef, v::AbstractString)
    GC.@preserve v ffi_tsd_dto_time_span_set_start(r._ptr, Base.unsafe_convert(Cstring, v))
end

function end_time(r::TsdDtoTimeSpanRef)
    unsafe_string(ffi_tsd_dto_time_span_get_end(r._ptr))
end

function set_end_time!(r::TsdDtoTimeSpanRef, v::AbstractString)
    GC.@preserve v ffi_tsd_dto_time_span_set_end(r._ptr, Base.unsafe_convert(Cstring, v))
end

"""
    TsdDtoCfgRefRef

Borrowed view of a DTO configuration reference (id + timestamp strings). Do not store
past the lifetime of the owning [`TsdDtoPackage`](@ref).
"""
struct TsdDtoCfgRefRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

function cfg_id(r::TsdDtoCfgRefRef)
    unsafe_string(ffi_tsd_dto_cfg_ref_get_id(r._ptr))
end

function set_cfg_id!(r::TsdDtoCfgRefRef, v::AbstractString)
    GC.@preserve v ffi_tsd_dto_cfg_ref_set_id(r._ptr, Base.unsafe_convert(Cstring, v))
end

function timestamp(r::TsdDtoCfgRefRef)
    unsafe_string(ffi_tsd_dto_cfg_ref_get_timestamp(r._ptr))
end

function set_timestamp!(r::TsdDtoCfgRefRef, v::AbstractString)
    GC.@preserve v ffi_tsd_dto_cfg_ref_set_timestamp(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

"""
    TsdDtoTabSetRef

Borrowed view of a DTO tabular data set (timestamp + values + optional quality). Do not
store past the lifetime of the owning [`TsdDtoPackage`](@ref).
"""
struct TsdDtoTabSetRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

function timestamp(r::TsdDtoTabSetRef)
    unsafe_string(ffi_tsd_dto_tab_set_get_timestamp(r._ptr))
end

function set_timestamp!(r::TsdDtoTabSetRef, v::AbstractString)
    GC.@preserve v ffi_tsd_dto_tab_set_set_timestamp(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function values(r::TsdDtoTabSetRef)
    n = Int(ffi_tsd_dto_tab_set_value_count(r._ptr))
    [unsafe_string(ffi_tsd_dto_tab_set_value_at(r._ptr, Csize_t(i - 1))) for i = 1:n]
end

function set_values!(r::TsdDtoTabSetRef, vals::AbstractVector{<:AbstractString})
    strs = [Base.unsafe_convert(Cstring, s) for s in vals]
    GC.@preserve vals ffi_tsd_dto_tab_set_set_values(
        r._ptr,
        pointer(strs),
        Csize_t(length(strs)),
    )
end

function quality(r::TsdDtoTabSetRef)
    n = Int(ffi_tsd_dto_tab_set_quality_count(r._ptr))
    n == 0 ? nothing :
    [unsafe_string(ffi_tsd_dto_tab_set_quality_at(r._ptr, Csize_t(i - 1))) for i = 1:n]
end

function set_quality!(r::TsdDtoTabSetRef, q::AbstractVector{<:AbstractString})
    strs = [Base.unsafe_convert(Cstring, s) for s in q]
    GC.@preserve q ffi_tsd_dto_tab_set_set_quality(
        r._ptr,
        pointer(strs),
        Csize_t(length(strs)),
    )
end

function clear_quality!(r::TsdDtoTabSetRef)
    ffi_tsd_dto_tab_set_clear_quality(r._ptr)
end

"""
    TsdDtoTabularRef

Borrowed view of a DTO tabular data block. Do not store past the lifetime of the owning
[`TsdDtoPackage`](@ref).
"""
struct TsdDtoTabularRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

function number_of_data_set(r::TsdDtoTabularRef)
    ffi_tsd_dto_tabular_has_number_of_data_set(r._ptr) != 0 ?
    Int(ffi_tsd_dto_tabular_get_number_of_data_set(r._ptr)) : nothing
end

function set_number_of_data_set!(r::TsdDtoTabularRef, v::Integer)
    ffi_tsd_dto_tabular_set_number_of_data_set(r._ptr, Csize_t(v))
end

function clear_number_of_data_set!(r::TsdDtoTabularRef)
    ffi_tsd_dto_tabular_clear_number_of_data_set(r._ptr)
end

function number_of_data_channel(r::TsdDtoTabularRef)
    ffi_tsd_dto_tabular_has_number_of_data_channel(r._ptr) != 0 ?
    Int(ffi_tsd_dto_tabular_get_number_of_data_channel(r._ptr)) : nothing
end

function set_number_of_data_channel!(r::TsdDtoTabularRef, v::Integer)
    ffi_tsd_dto_tabular_set_number_of_data_channel(r._ptr, Csize_t(v))
end

function clear_number_of_data_channel!(r::TsdDtoTabularRef)
    ffi_tsd_dto_tabular_clear_number_of_data_channel(r._ptr)
end

function channel_ids(r::TsdDtoTabularRef)
    n = Int(ffi_tsd_dto_tabular_channel_id_count(r._ptr))
    [unsafe_string(ffi_tsd_dto_tabular_channel_id_at(r._ptr, Csize_t(i - 1))) for i = 1:n]
end

function set_channel_ids!(r::TsdDtoTabularRef, ids::AbstractVector{<:AbstractString})
    strs = [Base.unsafe_convert(Cstring, s) for s in ids]
    GC.@preserve ids ffi_tsd_dto_tabular_set_channel_ids(
        r._ptr,
        pointer(strs),
        Csize_t(length(strs)),
    )
end

function clear_channel_ids!(r::TsdDtoTabularRef)
    ffi_tsd_dto_tabular_clear_channel_ids(r._ptr)
end

function data_set_count(r::TsdDtoTabularRef)
    Int(ffi_tsd_dto_tabular_data_set_count(r._ptr))
end

function data_set_at(r::TsdDtoTabularRef, index::Int)
    ptr = ffi_tsd_dto_tabular_data_set_at(r._ptr, Csize_t(index - 1))
    ptr == C_NULL && return nothing
    TsdDtoTabSetRef(ptr, r._owner)
end

function data_set_push!(r::TsdDtoTabularRef)
    ptr = ffi_tsd_dto_tabular_data_set_push(r._ptr)
    ptr == C_NULL &&
        throw(VistaError(InvalidState, "tsd_dto_tabular_data_set_push returned NULL"))
    TsdDtoTabSetRef(ptr, r._owner)
end

function data_set_remove!(r::TsdDtoTabularRef, index::Int)
    ffi_tsd_dto_tabular_data_set_remove(r._ptr, Csize_t(index - 1))
end

"""
    TsdDtoEventSetRef

Borrowed view of a DTO event data set (timestamp + channel id + value + optional quality).
Do not store past the lifetime of the owning [`TsdDtoPackage`](@ref).
"""
struct TsdDtoEventSetRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

function timestamp(r::TsdDtoEventSetRef)
    unsafe_string(ffi_tsd_dto_event_set_get_timestamp(r._ptr))
end

function set_timestamp!(r::TsdDtoEventSetRef, v::AbstractString)
    GC.@preserve v ffi_tsd_dto_event_set_set_timestamp(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function channel_id(r::TsdDtoEventSetRef)
    unsafe_string(ffi_tsd_dto_event_set_get_data_channel_id(r._ptr))
end

function set_channel_id!(r::TsdDtoEventSetRef, v::AbstractString)
    GC.@preserve v ffi_tsd_dto_event_set_set_data_channel_id(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function value(r::TsdDtoEventSetRef)
    unsafe_string(ffi_tsd_dto_event_set_get_value(r._ptr))
end

function set_value!(r::TsdDtoEventSetRef, v::AbstractString)
    GC.@preserve v ffi_tsd_dto_event_set_set_value(r._ptr, Base.unsafe_convert(Cstring, v))
end

function quality(r::TsdDtoEventSetRef)
    ffi_tsd_dto_event_set_has_quality(r._ptr) != 0 ?
    unsafe_string(ffi_tsd_dto_event_set_get_quality(r._ptr)) : nothing
end

function set_quality!(r::TsdDtoEventSetRef, v::AbstractString)
    GC.@preserve v ffi_tsd_dto_event_set_set_quality(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_quality!(r::TsdDtoEventSetRef)
    ffi_tsd_dto_event_set_clear_quality(r._ptr)
end

"""
    TsdDtoEventRef

Borrowed view of a DTO event data block. Do not store past the lifetime of the owning
[`TsdDtoPackage`](@ref).
"""
struct TsdDtoEventRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

function number_of_data_set(r::TsdDtoEventRef)
    ffi_tsd_dto_event_has_number_of_data_set(r._ptr) != 0 ?
    Int(ffi_tsd_dto_event_get_number_of_data_set(r._ptr)) : nothing
end

function set_number_of_data_set!(r::TsdDtoEventRef, v::Integer)
    ffi_tsd_dto_event_set_number_of_data_set(r._ptr, Csize_t(v))
end

function clear_number_of_data_set!(r::TsdDtoEventRef)
    ffi_tsd_dto_event_clear_number_of_data_set(r._ptr)
end

function data_set_count(r::TsdDtoEventRef)
    Int(ffi_tsd_dto_event_data_set_count(r._ptr))
end

function data_set_at(r::TsdDtoEventRef, index::Int)
    ptr = ffi_tsd_dto_event_data_set_at(r._ptr, Csize_t(index - 1))
    ptr == C_NULL && return nothing
    TsdDtoEventSetRef(ptr, r._owner)
end

function data_set_push!(r::TsdDtoEventRef)
    ptr = ffi_tsd_dto_event_data_set_push(r._ptr)
    ptr == C_NULL &&
        throw(VistaError(InvalidState, "tsd_dto_event_data_set_push returned NULL"))
    TsdDtoEventSetRef(ptr, r._owner)
end

function data_set_remove!(r::TsdDtoEventRef, index::Int)
    ffi_tsd_dto_event_data_set_remove(r._ptr, Csize_t(index - 1))
end

"""
    TsdDtoTsdRef

Borrowed view of a DTO TimeSeriesData block. Do not store past the lifetime of the owning
[`TsdDtoPackage`](@ref).
"""
struct TsdDtoTsdRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

function data_cfg(r::TsdDtoTsdRef)
    ffi_tsd_dto_tsd_has_data_cfg(r._ptr) != 0 ?
    TsdDtoCfgRefRef(ffi_tsd_dto_tsd_get_data_cfg(r._ptr), r._owner) : nothing
end

function ensure_data_cfg!(r::TsdDtoTsdRef)
    ffi_tsd_dto_tsd_ensure_data_cfg(r._ptr)
    TsdDtoCfgRefRef(ffi_tsd_dto_tsd_get_data_cfg(r._ptr), r._owner)
end

function clear_data_cfg!(r::TsdDtoTsdRef)
    ffi_tsd_dto_tsd_clear_data_cfg(r._ptr)
end

function tabular_count(r::TsdDtoTsdRef)
    Int(ffi_tsd_dto_tsd_tabular_count(r._ptr))
end

function tabular_at(r::TsdDtoTsdRef, index::Int)
    ptr = ffi_tsd_dto_tsd_tabular_at(r._ptr, Csize_t(index - 1))
    ptr == C_NULL && return nothing
    TsdDtoTabularRef(ptr, r._owner)
end

function tabular_push!(r::TsdDtoTsdRef)
    ptr = ffi_tsd_dto_tsd_tabular_push(r._ptr)
    ptr == C_NULL &&
        throw(VistaError(InvalidState, "tsd_dto_tsd_tabular_push returned NULL"))
    TsdDtoTabularRef(ptr, r._owner)
end

function tabular_remove!(r::TsdDtoTsdRef, index::Int)
    ffi_tsd_dto_tsd_tabular_remove(r._ptr, Csize_t(index - 1))
end

function event(r::TsdDtoTsdRef)
    ffi_tsd_dto_tsd_has_event(r._ptr) != 0 ?
    TsdDtoEventRef(ffi_tsd_dto_tsd_get_event(r._ptr), r._owner) : nothing
end

function ensure_event!(r::TsdDtoTsdRef)
    ffi_tsd_dto_tsd_ensure_event(r._ptr)
    TsdDtoEventRef(ffi_tsd_dto_tsd_get_event(r._ptr), r._owner)
end

function clear_event!(r::TsdDtoTsdRef)
    ffi_tsd_dto_tsd_clear_event(r._ptr)
end

function custom_data_kinds(r::TsdDtoTsdRef)
    ffi_tsd_dto_tsd_has_custom_data_kinds(r._ptr) != 0 ?
    SerializableDocumentRef(ffi_tsd_dto_tsd_get_custom_data_kinds(r._ptr), r._owner) :
    nothing
end

function ensure_custom_data_kinds!(r::TsdDtoTsdRef)
    ffi_tsd_dto_tsd_ensure_custom_data_kinds(r._ptr)
    SerializableDocumentRef(ffi_tsd_dto_tsd_get_custom_data_kinds(r._ptr), r._owner)
end

function clear_custom_data_kinds!(r::TsdDtoTsdRef)
    ffi_tsd_dto_tsd_clear_custom_data_kinds(r._ptr)
end

"""
    TsdDtoHeaderRef

Borrowed view of the DTO package header. Do not store past the lifetime of the owning
[`TsdDtoPackage`](@ref).
"""
struct TsdDtoHeaderRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

function ship_id(r::TsdDtoHeaderRef)
    unsafe_string(ffi_tsd_dto_header_get_ship_id(r._ptr))
end

function set_ship_id!(r::TsdDtoHeaderRef, v::AbstractString)
    GC.@preserve v ffi_tsd_dto_header_set_ship_id(r._ptr, Base.unsafe_convert(Cstring, v))
end

function time_span(r::TsdDtoHeaderRef)
    ffi_tsd_dto_header_has_time_span(r._ptr) != 0 ?
    TsdDtoTimeSpanRef(ffi_tsd_dto_header_get_time_span(r._ptr), r._owner) : nothing
end

function ensure_time_span!(r::TsdDtoHeaderRef)
    ffi_tsd_dto_header_ensure_time_span(r._ptr)
    TsdDtoTimeSpanRef(ffi_tsd_dto_header_get_time_span(r._ptr), r._owner)
end

function clear_time_span!(r::TsdDtoHeaderRef)
    ffi_tsd_dto_header_clear_time_span(r._ptr)
end

function date_created(r::TsdDtoHeaderRef)
    ffi_tsd_dto_header_has_date_created(r._ptr) != 0 ?
    unsafe_string(ffi_tsd_dto_header_get_date_created(r._ptr)) : nothing
end

function set_date_created!(r::TsdDtoHeaderRef, v::AbstractString)
    GC.@preserve v ffi_tsd_dto_header_set_date_created(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_date_created!(r::TsdDtoHeaderRef)
    ffi_tsd_dto_header_clear_date_created(r._ptr)
end

function date_modified(r::TsdDtoHeaderRef)
    ffi_tsd_dto_header_has_date_modified(r._ptr) != 0 ?
    unsafe_string(ffi_tsd_dto_header_get_date_modified(r._ptr)) : nothing
end

function set_date_modified!(r::TsdDtoHeaderRef, v::AbstractString)
    GC.@preserve v ffi_tsd_dto_header_set_date_modified(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_date_modified!(r::TsdDtoHeaderRef)
    ffi_tsd_dto_header_clear_date_modified(r._ptr)
end

function author(r::TsdDtoHeaderRef)
    ffi_tsd_dto_header_has_author(r._ptr) != 0 ?
    unsafe_string(ffi_tsd_dto_header_get_author(r._ptr)) : nothing
end

function set_author!(r::TsdDtoHeaderRef, v::AbstractString)
    GC.@preserve v ffi_tsd_dto_header_set_author(r._ptr, Base.unsafe_convert(Cstring, v))
end

function clear_author!(r::TsdDtoHeaderRef)
    ffi_tsd_dto_header_clear_author(r._ptr)
end

function system_cfg_count(r::TsdDtoHeaderRef)
    Int(ffi_tsd_dto_header_system_cfg_count(r._ptr))
end

function system_cfg_at(r::TsdDtoHeaderRef, index::Int)
    ptr = ffi_tsd_dto_header_system_cfg_at(r._ptr, Csize_t(index - 1))
    ptr == C_NULL && return nothing
    TsdDtoCfgRefRef(ptr, r._owner)
end

function system_cfg_push!(r::TsdDtoHeaderRef)
    ptr = ffi_tsd_dto_header_system_cfg_push(r._ptr)
    ptr == C_NULL &&
        throw(VistaError(InvalidState, "tsd_dto_header_system_cfg_push returned NULL"))
    TsdDtoCfgRefRef(ptr, r._owner)
end

function system_cfg_remove!(r::TsdDtoHeaderRef, index::Int)
    ffi_tsd_dto_header_system_cfg_remove(r._ptr, Csize_t(index - 1))
end

function custom_headers(r::TsdDtoHeaderRef)
    ffi_tsd_dto_header_has_custom_headers(r._ptr) != 0 ?
    SerializableDocumentRef(ffi_tsd_dto_header_get_custom_headers(r._ptr), r._owner) :
    nothing
end

function ensure_custom_headers!(r::TsdDtoHeaderRef)
    ffi_tsd_dto_header_ensure_custom_headers(r._ptr)
    SerializableDocumentRef(ffi_tsd_dto_header_get_custom_headers(r._ptr), r._owner)
end

function clear_custom_headers!(r::TsdDtoHeaderRef)
    ffi_tsd_dto_header_clear_custom_headers(r._ptr)
end

"""
    TsdDtoPkgRef

Borrowed view of the DTO package (optional header + list of TimeSeriesData DTOs). Do not
store past the lifetime of the owning [`TsdDtoPackage`](@ref).
"""
struct TsdDtoPkgRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

function header(r::TsdDtoPkgRef)
    ffi_tsd_dto_pkg_has_header(r._ptr) != 0 ?
    TsdDtoHeaderRef(ffi_tsd_dto_pkg_get_header(r._ptr), r._owner) : nothing
end

function ensure_header!(r::TsdDtoPkgRef)
    ffi_tsd_dto_pkg_ensure_header(r._ptr)
    TsdDtoHeaderRef(ffi_tsd_dto_pkg_get_header(r._ptr), r._owner)
end

function clear_header!(r::TsdDtoPkgRef)
    ffi_tsd_dto_pkg_clear_header(r._ptr)
end

function tsd_count(r::TsdDtoPkgRef)
    Int(ffi_tsd_dto_pkg_tsd_count(r._ptr))
end

function tsd_at(r::TsdDtoPkgRef, index::Int)
    ptr = ffi_tsd_dto_pkg_tsd_at(r._ptr, Csize_t(index - 1))
    ptr == C_NULL && return nothing
    TsdDtoTsdRef(ptr, r._owner)
end

function tsd_push!(r::TsdDtoPkgRef)
    ptr = ffi_tsd_dto_pkg_tsd_push(r._ptr)
    ptr == C_NULL && throw(VistaError(InvalidState, "tsd_dto_pkg_tsd_push returned NULL"))
    TsdDtoTsdRef(ptr, r._owner)
end

function tsd_remove!(r::TsdDtoPkgRef, index::Int)
    ffi_tsd_dto_pkg_tsd_remove(r._ptr, Csize_t(index - 1))
end

"""
    TsdDtoPackage

ISO 19848 TimeSeriesData DTO package. Owns the C resource. Freed when garbage-collected.

Obtain via [`tsd_dto_from_json`](@ref) or [`tsd_to_dto`](@ref).
"""
mutable struct TsdDtoPackage
    _ptr::Ptr{Cvoid}

    function TsdDtoPackage(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "null TsdDtoPackage pointer"))
        p = new(ptr)
        finalizer(p) do x
            if x._ptr != C_NULL
                ffi_tsd_dto_package_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        p
    end
end

function pkg(p::TsdDtoPackage)
    TsdDtoPkgRef(ffi_tsd_dto_package_get_pkg(p._ptr), p)
end

"""
    tsd_to_dto(domain::TimeSeriesDataPackage) -> TsdDtoPackage

Convert a domain [`TimeSeriesDataPackage`](@ref) to its DTO representation.
"""
function tsd_to_dto(domain::TimeSeriesDataPackage)
    ptr = ffi_tsd_to_dto(domain._ptr)
    ptr == C_NULL && throw(VistaError(InvalidState, "tsd_to_dto returned NULL"))
    TsdDtoPackage(ptr)
end

"""
    tsd_to_domain(dto::TsdDtoPackage) -> TimeSeriesDataPackage

Convert a [`TsdDtoPackage`](@ref) back to a domain [`TimeSeriesDataPackage`](@ref).
Throws [`VistaError`](@ref) on failure.
"""
function tsd_to_domain(dto::TsdDtoPackage)
    ptr = ffi_tsd_to_domain(dto._ptr)
    ptr == C_NULL && throw(VistaError(InvalidState, "tsd_to_domain returned NULL"))
    TimeSeriesDataPackage(ptr)
end

"""
    tsd_dto_from_json(json::AbstractString) -> TsdDtoPackage

Parse an ISO 19848 TimeSeriesData JSON payload into a [`TsdDtoPackage`](@ref).
Throws [`VistaError`](@ref) if the input is invalid or cannot be parsed.
"""
function tsd_dto_from_json(json::AbstractString)
    s = String(json)
    ptr = GC.@preserve s ffi_tsd_dto_from_json(Base.unsafe_convert(Cstring, s))
    ptr == C_NULL && throw(VistaError(Runtime, "tsd_dto_from_json: invalid JSON"))
    TsdDtoPackage(ptr)
end

"""
    tsd_dto_to_json(p::TsdDtoPackage, pretty::Bool = false) -> String

Serialize a [`TsdDtoPackage`](@ref) to an ISO 19848 TimeSeriesData JSON string.
Pass `pretty = true` for indented output.
"""
function tsd_dto_to_json(p::TsdDtoPackage, pretty::Bool = false)
    ptr = ffi_tsd_dto_to_json(p._ptr, Cint(pretty ? 1 : 0))
    ptr == C_NULL && throw(VistaError(InvalidState, "tsd_dto_to_json returned NULL"))
    s = unsafe_string(ptr)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), ptr)
    s
end
