"""
    DclDtoPackage

ISO 19848 DataChannelList DTO package. Owns the C resource. Freed when garbage-collected.

Obtain via [`dcl_dto_from_json`](@ref).
"""
mutable struct DclDtoPackage
    _ptr::Ptr{Cvoid}

    function DclDtoPackage(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "null DclDtoPackage pointer"))
        p = new(ptr)
        finalizer(p) do x
            if x._ptr != C_NULL
                ffi_dcl_dto_package_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        p
    end
end

"""
    DclDtoPkgRef

Borrowed view of the DTO package (header + channel list). Do not store past the lifetime
of the owning [`DclDtoPackage`](@ref).
"""
struct DclDtoPkgRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDtoHeaderRef

Borrowed view of the DTO package header. Do not store past the lifetime of the owning
[`DclDtoPackage`](@ref).
"""
struct DclDtoHeaderRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDtoCfgRefRef

Borrowed view of the DTO configuration reference. Do not store past the lifetime of the
owning [`DclDtoPackage`](@ref).
"""
struct DclDtoCfgRefRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDtoVersionInfoRef

Borrowed view of the DTO version information block. Do not store past the lifetime of the
owning [`DclDtoPackage`](@ref).
"""
struct DclDtoVersionInfoRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDtoChannelListRef

Borrowed view of the DTO channel list. Do not store past the lifetime of the owning
[`DclDtoPackage`](@ref).
"""
struct DclDtoChannelListRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDtoChannelRef

Borrowed view of a DTO data channel (id + property). Do not store past the lifetime of the
owning [`DclDtoPackage`](@ref).
"""
struct DclDtoChannelRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDtoChannelIdRef

Borrowed view of a DTO channel identifier (local id string + optional short id and name
object). Do not store past the lifetime of the owning [`DclDtoPackage`](@ref).
"""
struct DclDtoChannelIdRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDtoPropertyRef

Borrowed view of a DTO channel property (type, format, optional range/unit/etc.). Do not
store past the lifetime of the owning [`DclDtoPackage`](@ref).
"""
struct DclDtoPropertyRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDtoChannelTypeRef

Borrowed view of a DTO channel type (type string, optional update cycle and calculation
period). Do not store past the lifetime of the owning [`DclDtoPackage`](@ref).
"""
struct DclDtoChannelTypeRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDtoFormatRef

Borrowed view of a DTO format (type string + optional restriction). Do not store past the
lifetime of the owning [`DclDtoPackage`](@ref).
"""
struct DclDtoFormatRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDtoRestrictionRef

Borrowed view of a DTO restriction object (XML Schema facets, string-typed). Do not store
past the lifetime of the owning [`DclDtoPackage`](@ref).
"""
struct DclDtoRestrictionRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDtoRangeRef

Borrowed view of a DTO range (low/high strings). Do not store past the lifetime of the
owning [`DclDtoPackage`](@ref).
"""
struct DclDtoRangeRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDtoUnitRef

Borrowed view of a DTO unit (symbol + optional quantity name). Do not store past the
lifetime of the owning [`DclDtoPackage`](@ref).
"""
struct DclDtoUnitRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDtoNameObjectRef

Borrowed view of a DTO name object (naming rule + optional custom document). Do not store
past the lifetime of the owning [`DclDtoPackage`](@ref).
"""
struct DclDtoNameObjectRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end


function enumeration_count(r::DclDtoRestrictionRef)
    Int(ffi_dcl_dto_restriction_enumeration_count(r._ptr))
end

function enumeration_at(r::DclDtoRestrictionRef, index::Int)
    cs = ffi_dcl_dto_restriction_enumeration_at(r._ptr, Csize_t(index - 1))
    cs == C_NULL ? nothing : unsafe_string(cs)
end

function set_enumeration!(r::DclDtoRestrictionRef, values::AbstractVector{<:AbstractString})
    strs = [Base.unsafe_convert(Cstring, s) for s in values]
    GC.@preserve values ffi_dcl_dto_restriction_set_enumeration(
        r._ptr,
        pointer(strs),
        Csize_t(length(strs)),
    )
end

function clear_enumeration!(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_clear_enumeration(r._ptr)
end

function fraction_digits(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_has_fraction_digits(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_restriction_get_fraction_digits(r._ptr)) : nothing
end

function set_fraction_digits!(r::DclDtoRestrictionRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_restriction_set_fraction_digits(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_fraction_digits!(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_clear_fraction_digits(r._ptr)
end

function length_(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_has_length(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_restriction_get_length(r._ptr)) : nothing
end

function set_length!(r::DclDtoRestrictionRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_restriction_set_length(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_length!(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_clear_length(r._ptr)
end

function max_exclusive(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_has_max_exclusive(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_restriction_get_max_exclusive(r._ptr)) : nothing
end

function set_max_exclusive!(r::DclDtoRestrictionRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_restriction_set_max_exclusive(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_max_exclusive!(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_clear_max_exclusive(r._ptr)
end

function max_inclusive(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_has_max_inclusive(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_restriction_get_max_inclusive(r._ptr)) : nothing
end

function set_max_inclusive!(r::DclDtoRestrictionRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_restriction_set_max_inclusive(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_max_inclusive!(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_clear_max_inclusive(r._ptr)
end

function max_length(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_has_max_length(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_restriction_get_max_length(r._ptr)) : nothing
end

function set_max_length!(r::DclDtoRestrictionRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_restriction_set_max_length(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_max_length!(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_clear_max_length(r._ptr)
end

function min_exclusive(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_has_min_exclusive(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_restriction_get_min_exclusive(r._ptr)) : nothing
end

function set_min_exclusive!(r::DclDtoRestrictionRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_restriction_set_min_exclusive(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_min_exclusive!(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_clear_min_exclusive(r._ptr)
end

function min_inclusive(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_has_min_inclusive(r._ptr) != 0 ?
    Float64(ffi_dcl_dto_restriction_get_min_inclusive(r._ptr)) : nothing
end

function set_min_inclusive!(r::DclDtoRestrictionRef, v::Float64)
    ffi_dcl_dto_restriction_set_min_inclusive(r._ptr, Cdouble(v))
end

function clear_min_inclusive!(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_clear_min_inclusive(r._ptr)
end

function min_length(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_has_min_length(r._ptr) != 0 ?
    UInt32(ffi_dcl_dto_restriction_get_min_length(r._ptr)) : nothing
end

function set_min_length!(r::DclDtoRestrictionRef, v::Integer)
    ffi_dcl_dto_restriction_set_min_length(r._ptr, UInt32(v))
end

function clear_min_length!(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_clear_min_length(r._ptr)
end

function pattern(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_has_pattern(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_restriction_get_pattern(r._ptr)) : nothing
end

function set_pattern!(r::DclDtoRestrictionRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_restriction_set_pattern(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_pattern!(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_clear_pattern(r._ptr)
end

function total_digits(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_has_total_digits(r._ptr) != 0 ?
    UInt32(ffi_dcl_dto_restriction_get_total_digits(r._ptr)) : nothing
end

function set_total_digits!(r::DclDtoRestrictionRef, v::Integer)
    ffi_dcl_dto_restriction_set_total_digits(r._ptr, UInt32(v))
end

function clear_total_digits!(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_clear_total_digits(r._ptr)
end

function white_space(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_has_white_space(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_restriction_get_white_space(r._ptr)) : nothing
end

function set_white_space!(r::DclDtoRestrictionRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_restriction_set_white_space(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_white_space!(r::DclDtoRestrictionRef)
    ffi_dcl_dto_restriction_clear_white_space(r._ptr)
end


function Base.getproperty(r::DclDtoRangeRef, s::Symbol)
    if s === :low
        Float64(ffi_dcl_dto_range_get_low(r._ptr))
    elseif s === :high
        Float64(ffi_dcl_dto_range_get_high(r._ptr))
    else
        getfield(r, s)
    end
end

function set_low!(r::DclDtoRangeRef, v::Real)
    ffi_dcl_dto_range_set_low(r._ptr, Cdouble(v))
end

function set_high!(r::DclDtoRangeRef, v::Real)
    ffi_dcl_dto_range_set_high(r._ptr, Cdouble(v))
end


function symbol(r::DclDtoUnitRef)
    unsafe_string(ffi_dcl_dto_unit_get_symbol(r._ptr))
end

function set_symbol!(r::DclDtoUnitRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_unit_set_symbol(r._ptr, Base.unsafe_convert(Cstring, v))
end

function quantity_name(r::DclDtoUnitRef)
    ffi_dcl_dto_unit_has_quantity_name(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_unit_get_quantity_name(r._ptr)) : nothing
end

function set_quantity_name!(r::DclDtoUnitRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_unit_set_quantity_name(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_quantity_name!(r::DclDtoUnitRef)
    ffi_dcl_dto_unit_clear_quantity_name(r._ptr)
end


function format_type(r::DclDtoFormatRef)
    unsafe_string(ffi_dcl_dto_format_get_type(r._ptr))
end

function set_format_type!(r::DclDtoFormatRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_format_set_type(r._ptr, Base.unsafe_convert(Cstring, v))
end

function restriction(r::DclDtoFormatRef)
    ffi_dcl_dto_format_has_restriction(r._ptr) != 0 ?
    DclDtoRestrictionRef(ffi_dcl_dto_format_get_restriction(r._ptr), r._owner) : nothing
end

function ensure_restriction!(r::DclDtoFormatRef)
    ffi_dcl_dto_format_ensure_restriction(r._ptr)
    DclDtoRestrictionRef(ffi_dcl_dto_format_get_restriction(r._ptr), r._owner)
end

function clear_restriction!(r::DclDtoFormatRef)
    ffi_dcl_dto_format_clear_restriction(r._ptr)
end


function channel_type(r::DclDtoChannelTypeRef)
    unsafe_string(ffi_dcl_dto_ch_type_get_type(r._ptr))
end

function set_channel_type!(r::DclDtoChannelTypeRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_ch_type_set_type(r._ptr, Base.unsafe_convert(Cstring, v))
end

function update_cycle(r::DclDtoChannelTypeRef)
    ffi_dcl_dto_ch_type_has_update_cycle(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_ch_type_get_update_cycle(r._ptr)) : nothing
end

function set_update_cycle!(r::DclDtoChannelTypeRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_ch_type_set_update_cycle(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_update_cycle!(r::DclDtoChannelTypeRef)
    ffi_dcl_dto_ch_type_clear_update_cycle(r._ptr)
end

function calculation_period(r::DclDtoChannelTypeRef)
    ffi_dcl_dto_ch_type_has_calculation_period(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_ch_type_get_calculation_period(r._ptr)) : nothing
end

function set_calculation_period!(r::DclDtoChannelTypeRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_ch_type_set_calculation_period(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_calculation_period!(r::DclDtoChannelTypeRef)
    ffi_dcl_dto_ch_type_clear_calculation_period(r._ptr)
end


function channel_type(r::DclDtoPropertyRef)
    DclDtoChannelTypeRef(ffi_dcl_dto_property_get_ch_type(r._ptr), r._owner)
end

function format(r::DclDtoPropertyRef)
    DclDtoFormatRef(ffi_dcl_dto_property_get_format(r._ptr), r._owner)
end

function dcl_range(r::DclDtoPropertyRef)
    ffi_dcl_dto_property_has_range(r._ptr) != 0 ?
    DclDtoRangeRef(ffi_dcl_dto_property_get_range(r._ptr), r._owner) : nothing
end

function ensure_dcl_range!(r::DclDtoPropertyRef)
    ffi_dcl_dto_property_ensure_range(r._ptr)
    DclDtoRangeRef(ffi_dcl_dto_property_get_range(r._ptr), r._owner)
end

function clear_dcl_range!(r::DclDtoPropertyRef)
    ffi_dcl_dto_property_clear_range(r._ptr)
end

function unit(r::DclDtoPropertyRef)
    ffi_dcl_dto_property_has_unit(r._ptr) != 0 ?
    DclDtoUnitRef(ffi_dcl_dto_property_get_unit(r._ptr), r._owner) : nothing
end

function ensure_unit!(r::DclDtoPropertyRef)
    ffi_dcl_dto_property_ensure_unit(r._ptr)
    DclDtoUnitRef(ffi_dcl_dto_property_get_unit(r._ptr), r._owner)
end

function clear_unit!(r::DclDtoPropertyRef)
    ffi_dcl_dto_property_clear_unit(r._ptr)
end

function quality_coding(r::DclDtoPropertyRef)
    ffi_dcl_dto_property_has_quality_coding(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_property_get_quality_coding(r._ptr)) : nothing
end

function set_quality_coding!(r::DclDtoPropertyRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_property_set_quality_coding(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_quality_coding!(r::DclDtoPropertyRef)
    ffi_dcl_dto_property_clear_quality_coding(r._ptr)
end

function alert_priority(r::DclDtoPropertyRef)
    ffi_dcl_dto_property_has_alert_priority(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_property_get_alert_priority(r._ptr)) : nothing
end

function set_alert_priority!(r::DclDtoPropertyRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_property_set_alert_priority(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_alert_priority!(r::DclDtoPropertyRef)
    ffi_dcl_dto_property_clear_alert_priority(r._ptr)
end

function name(r::DclDtoPropertyRef)
    ffi_dcl_dto_property_has_name(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_property_get_name(r._ptr)) : nothing
end

function set_name!(r::DclDtoPropertyRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_property_set_name(r._ptr, Base.unsafe_convert(Cstring, v))
end

function clear_name!(r::DclDtoPropertyRef)
    ffi_dcl_dto_property_clear_name(r._ptr)
end

function remarks(r::DclDtoPropertyRef)
    ffi_dcl_dto_property_has_remarks(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_property_get_remarks(r._ptr)) : nothing
end

function set_remarks!(r::DclDtoPropertyRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_property_set_remarks(r._ptr, Base.unsafe_convert(Cstring, v))
end

function clear_remarks!(r::DclDtoPropertyRef)
    ffi_dcl_dto_property_clear_remarks(r._ptr)
end


function naming_rule(r::DclDtoNameObjectRef)
    unsafe_string(ffi_dcl_dto_name_obj_get_naming_rule(r._ptr))
end

function set_naming_rule!(r::DclDtoNameObjectRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_name_obj_set_naming_rule(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end


function local_id(r::DclDtoChannelIdRef)
    unsafe_string(ffi_dcl_dto_ch_id_get_local_id(r._ptr))
end

function set_local_id!(r::DclDtoChannelIdRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_ch_id_set_local_id(r._ptr, Base.unsafe_convert(Cstring, v))
end

function short_id(r::DclDtoChannelIdRef)
    ffi_dcl_dto_ch_id_has_short_id(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_ch_id_get_short_id(r._ptr)) : nothing
end

function set_short_id!(r::DclDtoChannelIdRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_ch_id_set_short_id(r._ptr, Base.unsafe_convert(Cstring, v))
end

function clear_short_id!(r::DclDtoChannelIdRef)
    ffi_dcl_dto_ch_id_clear_short_id(r._ptr)
end

function name_object(r::DclDtoChannelIdRef)
    ffi_dcl_dto_ch_id_has_name_object(r._ptr) != 0 ?
    DclDtoNameObjectRef(ffi_dcl_dto_ch_id_get_name_object(r._ptr), r._owner) : nothing
end

function ensure_name_object!(r::DclDtoChannelIdRef)
    ffi_dcl_dto_ch_id_ensure_name_object(r._ptr)
    DclDtoNameObjectRef(ffi_dcl_dto_ch_id_get_name_object(r._ptr), r._owner)
end

function clear_name_object!(r::DclDtoChannelIdRef)
    ffi_dcl_dto_ch_id_clear_name_object(r._ptr)
end


function channel_id(r::DclDtoChannelRef)
    DclDtoChannelIdRef(ffi_dcl_dto_channel_get_id(r._ptr), r._owner)
end

function property(r::DclDtoChannelRef)
    DclDtoPropertyRef(ffi_dcl_dto_channel_get_property(r._ptr), r._owner)
end


function Base.length(r::DclDtoChannelListRef)
    Int(ffi_dcl_dto_ch_list_count(r._ptr))
end

function Base.getindex(r::DclDtoChannelListRef, index::Int)
    ptr = ffi_dcl_dto_ch_list_at(r._ptr, Csize_t(index - 1))
    ptr == C_NULL && throw(BoundsError(r, index))
    DclDtoChannelRef(ptr, r._owner)
end

function Base.push!(r::DclDtoChannelListRef)
    ptr = ffi_dcl_dto_ch_list_push(r._ptr)
    ptr == C_NULL && throw(VistaError(InvalidState, "dcl_dto_ch_list_push returned NULL"))
    DclDtoChannelRef(ptr, r._owner)
end

function Base.deleteat!(r::DclDtoChannelListRef, index::Int)
    ffi_dcl_dto_ch_list_remove(r._ptr, Csize_t(index - 1))
end

function Base.iterate(r::DclDtoChannelListRef, i::Int = 1)
    i > length(r) && return nothing
    (r[i], i + 1)
end


function naming_rule(r::DclDtoVersionInfoRef)
    unsafe_string(ffi_dcl_dto_ver_info_get_naming_rule(r._ptr))
end

function set_naming_rule!(r::DclDtoVersionInfoRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_ver_info_set_naming_rule(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function naming_scheme_version(r::DclDtoVersionInfoRef)
    unsafe_string(ffi_dcl_dto_ver_info_get_naming_scheme_version(r._ptr))
end

function set_naming_scheme_version!(r::DclDtoVersionInfoRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_ver_info_set_naming_scheme_version(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function reference_url(r::DclDtoVersionInfoRef)
    ffi_dcl_dto_ver_info_has_reference_url(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_ver_info_get_reference_url(r._ptr)) : nothing
end

function set_reference_url!(r::DclDtoVersionInfoRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_ver_info_set_reference_url(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_reference_url!(r::DclDtoVersionInfoRef)
    ffi_dcl_dto_ver_info_clear_reference_url(r._ptr)
end


function cfg_id(r::DclDtoCfgRefRef)
    unsafe_string(ffi_dcl_dto_cfg_ref_get_id(r._ptr))
end

function set_cfg_id!(r::DclDtoCfgRefRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_cfg_ref_set_id(r._ptr, Base.unsafe_convert(Cstring, v))
end

function timestamp(r::DclDtoCfgRefRef)
    unsafe_string(ffi_dcl_dto_cfg_ref_get_timestamp(r._ptr))
end

function set_timestamp!(r::DclDtoCfgRefRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_cfg_ref_set_timestamp(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function version(r::DclDtoCfgRefRef)
    ffi_dcl_dto_cfg_ref_has_version(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_cfg_ref_get_version(r._ptr)) : nothing
end

function set_version!(r::DclDtoCfgRefRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_cfg_ref_set_version(r._ptr, Base.unsafe_convert(Cstring, v))
end

function clear_version!(r::DclDtoCfgRefRef)
    ffi_dcl_dto_cfg_ref_clear_version(r._ptr)
end


function ship_id(r::DclDtoHeaderRef)
    unsafe_string(ffi_dcl_dto_header_get_ship_id(r._ptr))
end

function set_ship_id!(r::DclDtoHeaderRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_header_set_ship_id(r._ptr, Base.unsafe_convert(Cstring, v))
end

function cfg_ref(r::DclDtoHeaderRef)
    DclDtoCfgRefRef(ffi_dcl_dto_header_get_cfg_ref(r._ptr), r._owner)
end

function version_info(r::DclDtoHeaderRef)
    ffi_dcl_dto_header_has_ver_info(r._ptr) != 0 ?
    DclDtoVersionInfoRef(ffi_dcl_dto_header_get_ver_info(r._ptr), r._owner) : nothing
end

function ensure_version_info!(r::DclDtoHeaderRef)
    ffi_dcl_dto_header_ensure_ver_info(r._ptr)
    DclDtoVersionInfoRef(ffi_dcl_dto_header_get_ver_info(r._ptr), r._owner)
end

function clear_version_info!(r::DclDtoHeaderRef)
    ffi_dcl_dto_header_clear_ver_info(r._ptr)
end

function author(r::DclDtoHeaderRef)
    ffi_dcl_dto_header_has_author(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_header_get_author(r._ptr)) : nothing
end

function set_author!(r::DclDtoHeaderRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_header_set_author(r._ptr, Base.unsafe_convert(Cstring, v))
end

function clear_author!(r::DclDtoHeaderRef)
    ffi_dcl_dto_header_clear_author(r._ptr)
end

function date_created(r::DclDtoHeaderRef)
    ffi_dcl_dto_header_has_date_created(r._ptr) != 0 ?
    unsafe_string(ffi_dcl_dto_header_get_date_created(r._ptr)) : nothing
end

function set_date_created!(r::DclDtoHeaderRef, v::AbstractString)
    GC.@preserve v ffi_dcl_dto_header_set_date_created(
        r._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

function clear_date_created!(r::DclDtoHeaderRef)
    ffi_dcl_dto_header_clear_date_created(r._ptr)
end

"""
    custom_headers(r::DclDtoHeaderRef) -> Union{SerializableDocumentRef,Nothing}

Return the custom headers extension point (`xs:any`), or `nothing` if unset.
"""
function custom_headers(r::DclDtoHeaderRef)
    ffi_dcl_dto_header_has_custom_headers(r._ptr) != 0 ?
    SerializableDocumentRef(ffi_dcl_dto_header_get_custom_headers(r._ptr), r._owner) :
    nothing
end

"""
    ensure_custom_headers!(r::DclDtoHeaderRef) -> SerializableDocumentRef

Ensure the custom headers extension point exists (creating an empty object if absent), and
return it.
"""
function ensure_custom_headers!(r::DclDtoHeaderRef)
    ffi_dcl_dto_header_ensure_custom_headers(r._ptr)
    SerializableDocumentRef(ffi_dcl_dto_header_get_custom_headers(r._ptr), r._owner)
end

"""
    clear_custom_headers!(r::DclDtoHeaderRef)

Remove the custom headers extension point.
"""
function clear_custom_headers!(r::DclDtoHeaderRef)
    ffi_dcl_dto_header_clear_custom_headers(r._ptr)
end

function header(r::DclDtoPkgRef)
    DclDtoHeaderRef(ffi_dcl_dto_pkg_get_header(r._ptr), r._owner)
end

function channel_list(r::DclDtoPkgRef)
    DclDtoChannelListRef(ffi_dcl_dto_pkg_get_channel_list(r._ptr), r._owner)
end


function pkg(p::DclDtoPackage)
    DclDtoPkgRef(ffi_dcl_dto_package_get_pkg(p._ptr), p)
end

"""
    dcl_to_dto(domain::DclListPackage) -> DclDtoPackage

Convert a [`DclListPackage`](@ref) domain object to its DTO representation.
"""
function dcl_to_dto(domain::DclListPackage)
    ptr = ffi_dcl_to_dto(domain._ptr)
    ptr == C_NULL && throw(VistaError(InvalidState, "dcl_to_dto returned NULL"))
    DclDtoPackage(ptr)
end

"""
    dcl_to_domain(dto::DclDtoPackage) -> DclListPackage

Convert a [`DclDtoPackage`](@ref) DTO to its domain representation.
"""
function dcl_to_domain(dto::DclDtoPackage)
    ptr = ffi_dcl_to_domain(dto._ptr)
    ptr == C_NULL && throw(VistaError(InvalidState, "dcl_to_domain returned NULL"))
    DclListPackage(ptr)
end
