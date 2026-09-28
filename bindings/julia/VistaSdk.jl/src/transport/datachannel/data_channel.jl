@enum WhiteSpace::Int32 begin
    Preserve = 0
    Replace = 1
    Collapse = 2
end

function _ws_to_ffi(ws::WhiteSpace)
    if ws == Preserve
        _WsPreserve
    elseif ws == Replace
        _WsReplace
    else
        _WsCollapse
    end
end

function _ws_from_ffi(ws::_FfiWhiteSpace)
    if ws == _WsPreserve
        Preserve
    elseif ws == _WsReplace
        Replace
    else
        Collapse
    end
end

function _opt_str(ptr::Cstring)
    ptr == C_NULL ? nothing : unsafe_string(ptr)
end

function _req_str(ptr::Cstring)
    unsafe_string(ptr)
end

"""
    DclListPackage

ISO 19848 Table 11 root element wrapping a [`DclPackage`](@ref). The top-level
type for DataChannelList JSON documents. Owns a C resource, freed when
garbage-collected.
"""
mutable struct DclListPackage
    _ptr::Ptr{Cvoid}

    function DclListPackage(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(last_error())
        lp = new(ptr)
        finalizer(lp) do x
            if x._ptr != C_NULL
                ffi_dcl_list_package_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        lp
    end
end

"""
    DclPackage

ISO 19848 Table 12 top-level DataChannelList package: a header and a data
channel list. Owns a C resource, freed when garbage-collected.
"""
mutable struct DclPackage
    _ptr::Ptr{Cvoid}

    function DclPackage(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "null DclPackage pointer"))
        pkg = new(ptr)
        finalizer(pkg) do x
            if x._ptr != C_NULL
                ffi_dcl_package_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        pkg
    end
end

"""
    DclPackageRef

Borrowed, read-only view of a [`DclPackage`](@ref). Keeps a reference to the
owning object to prevent GC collection. Do not store this past the lifetime of
the owner.
"""
struct DclPackageRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclHeader

ISO 19848 Table 14 header of a DataChannelList package: ship ID, configuration
reference, optional version information, author, creation date, and custom
headers. Owns a C resource, freed when garbage-collected.
"""
mutable struct DclHeader
    _ptr::Ptr{Cvoid}

    function DclHeader(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "null DclHeader pointer"))
        h = new(ptr)
        finalizer(h) do x
            if x._ptr != C_NULL
                ffi_dcl_header_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        h
    end
end

"""
    DclHeaderRef

Borrowed, read-only view of a [`DclHeader`](@ref). Keeps a reference to the
owning object to prevent GC collection. Do not store this past the lifetime of
the owner.
"""
struct DclHeaderRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclConfigurationReference

Reference to a DataChannelList configuration (ISO 19848 ConfigurationReference):
an ID, an optional version, and a timestamp. Owns a C resource, freed when
garbage-collected.
"""
mutable struct DclConfigurationReference
    _ptr::Ptr{Cvoid}

    function DclConfigurationReference(ptr::Ptr{Cvoid})
        ptr == C_NULL &&
            throw(VistaError(InvalidArgument, "null DclConfigurationReference pointer"))
        cr = new(ptr)
        finalizer(cr) do x
            if x._ptr != C_NULL
                ffi_dcl_configuration_reference_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        cr
    end
end

"""
    DclConfigurationReferenceRef

Borrowed, read-only view of a [`DclConfigurationReference`](@ref). Keeps a
reference to the owning object to prevent GC collection. Do not store this past
the lifetime of the owner.
"""
struct DclConfigurationReferenceRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclVersionInformation

Naming scheme version information for a DataChannelList (naming rule, naming
scheme version, optional reference URL). Owns a C resource, freed when
garbage-collected.
"""
mutable struct DclVersionInformation
    _ptr::Ptr{Cvoid}

    function DclVersionInformation(ptr::Ptr{Cvoid})
        ptr == C_NULL &&
            throw(VistaError(InvalidArgument, "null DclVersionInformation pointer"))
        vi = new(ptr)
        finalizer(vi) do x
            if x._ptr != C_NULL
                ffi_dcl_version_information_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        vi
    end
end

"""
    DclVersionInformationRef

Borrowed, read-only view of a [`DclVersionInformation`](@ref). Keeps a reference
to the owning object to prevent GC collection. Do not store this past the
lifetime of the owner.
"""
struct DclVersionInformationRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDataChannelList

ISO 19848 Table 13 list of data channels. Owns a C resource, freed when
garbage-collected.
"""
mutable struct DclDataChannelList
    _ptr::Ptr{Cvoid}

    function DclDataChannelList(ptr::Ptr{Cvoid})
        ptr == C_NULL &&
            throw(VistaError(InvalidArgument, "null DclDataChannelList pointer"))
        l = new(ptr)
        finalizer(l) do x
            if x._ptr != C_NULL
                ffi_dcl_data_channel_list_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        l
    end
end

"""
    DclDataChannelListRef

Borrowed, read-only view of a [`DclDataChannelList`](@ref). Keeps a reference
to the owning object to prevent GC collection. Do not store this past the
lifetime of the owner.
"""
struct DclDataChannelListRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDataChannel

ISO 19848 Table 13 data channel: a channel ID paired with its property. Owns a
C resource, freed when garbage-collected.
"""
mutable struct DclDataChannel
    _ptr::Ptr{Cvoid}

    function DclDataChannel(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(last_error())
        dc = new(ptr)
        finalizer(dc) do x
            if x._ptr != C_NULL
                ffi_dcl_data_channel_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        dc
    end
end

"""
    DclDataChannelRef

Borrowed, read-only view of a [`DclDataChannel`](@ref). Keeps a reference to
the owning object to prevent GC collection. Do not store this past the lifetime
of the owner.
"""
struct DclDataChannelRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDataChannelId

ISO 19848 DataChannelID: a LocalID, optional short ID, and optional name object.
Owns a C resource, freed when garbage-collected.
"""
mutable struct DclDataChannelId
    _ptr::Ptr{Cvoid}

    function DclDataChannelId(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "null DclDataChannelId pointer"))
        cid = new(ptr)
        finalizer(cid) do x
            if x._ptr != C_NULL
                ffi_dcl_channel_id_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        cid
    end
end

"""
    DclDataChannelIdRef

Borrowed, read-only view of a [`DclDataChannelId`](@ref). Keeps a reference to
the owning object to prevent GC collection. Do not store this past the lifetime
of the owner.
"""
struct DclDataChannelIdRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclProperty

ISO 19848 Table 16 property of a data channel (type, format, optional range/unit,
quality coding, alert priority, name, remarks, and custom properties). Owns a C
resource. Freed when garbage-collected.
"""
mutable struct DclProperty
    _ptr::Ptr{Cvoid}

    function DclProperty(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "null DclProperty pointer"))
        p = new(ptr)
        finalizer(p) do x
            if x._ptr != C_NULL
                ffi_dcl_property_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        p
    end
end

"""
    DclPropertyRef

Borrowed, read-only view of a [`DclProperty`](@ref). Keeps a reference to the owning
object to prevent GC collection. Do not store this past the lifetime of the owner.
"""
struct DclPropertyRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclDataChannelType

ISO 19848 Table 18 data channel type descriptor (e.g. `"Inst"`, `"Average"`, `"Alert"`).
Owns a C resource. Freed when garbage-collected.
"""
mutable struct DclDataChannelType
    _ptr::Ptr{Cvoid}

    function DclDataChannelType(ptr::Ptr{Cvoid})
        ptr == C_NULL &&
            throw(VistaError(InvalidArgument, "null DclDataChannelType pointer"))
        t = new(ptr)
        finalizer(t) do x
            if x._ptr != C_NULL
                ffi_dcl_data_channel_type_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        t
    end
end

"""
    DclDataChannelTypeRef

Borrowed, read-only view of a [`DclDataChannelType`](@ref). Keeps a reference to the
owning object to prevent GC collection. Do not store this past the lifetime of the owner.
"""
struct DclDataChannelTypeRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclFormat

ISO 19848 Table 20 format descriptor (base type plus optional [`DclRestriction`](@ref))
for a data channel value. Owns a C resource. Freed when garbage-collected.
"""
mutable struct DclFormat
    _ptr::Ptr{Cvoid}

    function DclFormat(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "null DclFormat pointer"))
        f = new(ptr)
        finalizer(f) do x
            if x._ptr != C_NULL
                ffi_dcl_format_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        f
    end
end

"""
    DclFormatRef

Borrowed, read-only view of a [`DclFormat`](@ref). Keeps a reference to the owning
object to prevent GC collection. Do not store this past the lifetime of the owner.
"""
struct DclFormatRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclRestriction

ISO 19848 Table 19 restriction (XML Schema-style facets) applied to a [`DclFormat`](@ref)
value. Owns a C resource. Freed when garbage-collected.
"""
mutable struct DclRestriction
    _ptr::Ptr{Cvoid}

    function DclRestriction(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "null DclRestriction pointer"))
        r = new(ptr)
        finalizer(r) do x
            if x._ptr != C_NULL
                ffi_dcl_restriction_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        r
    end
end

"""
    DclRestrictionRef

Borrowed, read-only view of a [`DclRestriction`](@ref). Keeps a reference to the owning
object to prevent GC collection. Do not store this past the lifetime of the owner.
"""
struct DclRestrictionRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclRange

ISO 19848 inclusive low/high range for a Decimal [`DclFormat`](@ref) value. Owns a C
resource. Freed when garbage-collected.
"""
mutable struct DclRange
    _ptr::Ptr{Cvoid}

    function DclRange(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(last_error())
        r = new(ptr)
        finalizer(r) do x
            if x._ptr != C_NULL
                ffi_dcl_range_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        r
    end
end

"""
    DclRangeRef

Borrowed, read-only view of a [`DclRange`](@ref). Keeps a reference to the owning
object to prevent GC collection. Do not store this past the lifetime of the owner.
"""
struct DclRangeRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclUnit

ISO 19848 Table 21 unit of measurement (symbol, optional quantity name, and custom
elements extension point). Owns a C resource. Freed when garbage-collected.
"""
mutable struct DclUnit
    _ptr::Ptr{Cvoid}

    function DclUnit(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "null DclUnit pointer"))
        u = new(ptr)
        finalizer(u) do x
            if x._ptr != C_NULL
                ffi_dcl_unit_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        u
    end
end

"""
    DclUnitRef

Borrowed, read-only view of a [`DclUnit`](@ref). Keeps a reference to the owning
object to prevent GC collection. Do not store this past the lifetime of the owner.
"""
struct DclUnitRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    DclNameObject

ISO 19848 Table 17 name object (Annex C naming rule and optional custom name objects
extension point). Owns a C resource. Freed when garbage-collected.
"""
mutable struct DclNameObject
    _ptr::Ptr{Cvoid}

    function DclNameObject(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "null DclNameObject pointer"))
        no = new(ptr)
        finalizer(no) do x
            if x._ptr != C_NULL
                ffi_dcl_name_object_free(x._ptr)
                x._ptr = C_NULL
            end
        end
        no
    end
end

"""
    DclNameObjectRef

Borrowed, read-only view of a [`DclNameObject`](@ref). Keeps a reference to the owning
object to prevent GC collection. Do not store this past the lifetime of the owner.
"""
struct DclNameObjectRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end


"""
    DclRestriction() -> DclRestriction

Construct an empty restriction (no facets set).
"""
DclRestriction() = DclRestriction(ffi_dcl_restriction_create())

"""
    enumeration_count(r::DclRestriction) -> Int

Return the number of allowed enumeration values.
"""
function enumeration_count(r::DclRestriction)
    Int(ffi_dcl_restriction_enumeration_count(r._ptr))
end

"""
    enumeration_at(r::DclRestriction, index::Int) -> Union{String,Nothing}

Return the enumeration value at 1-based `index`, or `nothing` if out of range.
"""
function enumeration_at(r::DclRestriction, index::Int)
    ptr = ffi_dcl_restriction_enumeration_at(r._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : unsafe_string(ptr)
end

"""
    set_enumeration!(r::DclRestriction, values::AbstractVector{<:AbstractString})

Set the allowed enumeration values, replacing any previous list.
"""
function set_enumeration!(r::DclRestriction, values::AbstractVector{<:AbstractString})
    strs = [Base.unsafe_convert(Cstring, s) for s in values]
    GC.@preserve values ffi_dcl_restriction_set_enumeration(
        r._ptr,
        pointer(strs),
        Csize_t(length(strs)),
    )
end

"""
    clear_enumeration!(r::DclRestriction)

Remove the enumeration facet entirely.
"""
function clear_enumeration!(r::DclRestriction)
    ffi_dcl_restriction_clear_enumeration(r._ptr)
end

"""
    fraction_digits(r::DclRestriction) -> Union{UInt32,Nothing}

Return the fraction-digits facet, or `nothing` if unset.
"""
function fraction_digits(r::DclRestriction)
    ffi_dcl_restriction_has_fraction_digits(r._ptr) != 0 ?
    ffi_dcl_restriction_fraction_digits(r._ptr) : nothing
end

"""
    set_fraction_digits!(r::DclRestriction, value::Integer)

Set the maximum number of fraction digits allowed for a Decimal value.
"""
function set_fraction_digits!(r::DclRestriction, value::Integer)
    ffi_dcl_restriction_set_fraction_digits(r._ptr, UInt32(value))
end

"""
    clear_fraction_digits!(r::DclRestriction)

Remove the fraction-digits facet.
"""
function clear_fraction_digits!(r::DclRestriction)
    ffi_dcl_restriction_clear_fraction_digits(r._ptr)
end

"""
    length_(r::DclRestriction) -> Union{UInt32,Nothing}

Return the exact-length facet, or `nothing` if unset.
"""
function length_(r::DclRestriction)
    ffi_dcl_restriction_has_length(r._ptr) != 0 ? ffi_dcl_restriction_length(r._ptr) :
    nothing
end

"""
    set_length!(r::DclRestriction, value::Integer)

Set the exact required length (characters or list items) for a value.
"""
function set_length!(r::DclRestriction, value::Integer)
    ffi_dcl_restriction_set_length(r._ptr, UInt32(value))
end

"""
    clear_length!(r::DclRestriction)

Remove the exact-length facet.
"""
function clear_length!(r::DclRestriction)
    ffi_dcl_restriction_clear_length(r._ptr)
end

"""
    max_exclusive(r::DclRestriction) -> Union{Float64,Nothing}

Return the max-exclusive facet (upper bound, strictly less than), or `nothing` if unset.
"""
function max_exclusive(r::DclRestriction)
    ffi_dcl_restriction_has_max_exclusive(r._ptr) != 0 ?
    Float64(ffi_dcl_restriction_max_exclusive(r._ptr)) : nothing
end

"""
    set_max_exclusive!(r::DclRestriction, value::Real)

Set the upper bound a numeric value must be strictly less than.
"""
function set_max_exclusive!(r::DclRestriction, value::Real)
    ffi_dcl_restriction_set_max_exclusive(r._ptr, Cdouble(value))
end

"""
    clear_max_exclusive!(r::DclRestriction)

Remove the max-exclusive facet.
"""
function clear_max_exclusive!(r::DclRestriction)
    ffi_dcl_restriction_clear_max_exclusive(r._ptr)
end

"""
    max_inclusive(r::DclRestriction) -> Union{Float64,Nothing}

Return the max-inclusive facet (upper bound, less than or equal to), or `nothing` if unset.
"""
function max_inclusive(r::DclRestriction)
    ffi_dcl_restriction_has_max_inclusive(r._ptr) != 0 ?
    Float64(ffi_dcl_restriction_max_inclusive(r._ptr)) : nothing
end

"""
    set_max_inclusive!(r::DclRestriction, value::Real)

Set the upper bound a numeric value must be less than or equal to.
"""
function set_max_inclusive!(r::DclRestriction, value::Real)
    ffi_dcl_restriction_set_max_inclusive(r._ptr, Cdouble(value))
end

"""
    clear_max_inclusive!(r::DclRestriction)

Remove the max-inclusive facet.
"""
function clear_max_inclusive!(r::DclRestriction)
    ffi_dcl_restriction_clear_max_inclusive(r._ptr)
end

"""
    max_length(r::DclRestriction) -> Union{UInt32,Nothing}

Return the maximum allowed length facet, or `nothing` if unset.
"""
function max_length(r::DclRestriction)
    ffi_dcl_restriction_has_max_length(r._ptr) != 0 ?
    ffi_dcl_restriction_max_length(r._ptr) : nothing
end

"""
    set_max_length!(r::DclRestriction, value::Integer)

Set the maximum allowed length (characters or list items) for a value.
"""
function set_max_length!(r::DclRestriction, value::Integer)
    ffi_dcl_restriction_set_max_length(r._ptr, UInt32(value))
end

"""
    clear_max_length!(r::DclRestriction)

Remove the maximum-length facet.
"""
function clear_max_length!(r::DclRestriction)
    ffi_dcl_restriction_clear_max_length(r._ptr)
end

"""
    min_exclusive(r::DclRestriction) -> Union{Float64,Nothing}

Return the min-exclusive facet (lower bound, strictly greater than), or `nothing` if unset.
"""
function min_exclusive(r::DclRestriction)
    ffi_dcl_restriction_has_min_exclusive(r._ptr) != 0 ?
    Float64(ffi_dcl_restriction_min_exclusive(r._ptr)) : nothing
end

"""
    set_min_exclusive!(r::DclRestriction, value::Real)

Set the lower bound a numeric value must be strictly greater than.
"""
function set_min_exclusive!(r::DclRestriction, value::Real)
    ffi_dcl_restriction_set_min_exclusive(r._ptr, Cdouble(value))
end

"""
    clear_min_exclusive!(r::DclRestriction)

Remove the min-exclusive facet.
"""
function clear_min_exclusive!(r::DclRestriction)
    ffi_dcl_restriction_clear_min_exclusive(r._ptr)
end

"""
    min_inclusive(r::DclRestriction) -> Union{Float64,Nothing}

Return the min-inclusive facet (lower bound, greater than or equal to), or `nothing` if unset.
"""
function min_inclusive(r::DclRestriction)
    ffi_dcl_restriction_has_min_inclusive(r._ptr) != 0 ?
    Float64(ffi_dcl_restriction_min_inclusive(r._ptr)) : nothing
end

"""
    set_min_inclusive!(r::DclRestriction, value::Real)

Set the lower bound a numeric value must be greater than or equal to.
"""
function set_min_inclusive!(r::DclRestriction, value::Real)
    ffi_dcl_restriction_set_min_inclusive(r._ptr, Cdouble(value))
end

"""
    clear_min_inclusive!(r::DclRestriction)

Remove the min-inclusive facet.
"""
function clear_min_inclusive!(r::DclRestriction)
    ffi_dcl_restriction_clear_min_inclusive(r._ptr)
end

"""
    min_length(r::DclRestriction) -> Union{UInt32,Nothing}

Return the minimum allowed length facet, or `nothing` if unset.
"""
function min_length(r::DclRestriction)
    ffi_dcl_restriction_has_min_length(r._ptr) != 0 ?
    ffi_dcl_restriction_min_length(r._ptr) : nothing
end

"""
    set_min_length!(r::DclRestriction, value::Integer)

Set the minimum allowed length (characters or list items) for a value.
"""
function set_min_length!(r::DclRestriction, value::Integer)
    ffi_dcl_restriction_set_min_length(r._ptr, UInt32(value))
end

"""
    clear_min_length!(r::DclRestriction)

Remove the minimum-length facet.
"""
function clear_min_length!(r::DclRestriction)
    ffi_dcl_restriction_clear_min_length(r._ptr)
end

"""
    pattern(r::DclRestriction) -> Union{String,Nothing}

Return the regular-expression pattern facet, or `nothing` if unset.
"""
function pattern(r::DclRestriction)
    _opt_str(ffi_dcl_restriction_pattern(r._ptr))
end

"""
    set_pattern!(r::DclRestriction, value::AbstractString)

Set the regular-expression pattern a String value must match.
"""
function set_pattern!(r::DclRestriction, value::AbstractString)
    GC.@preserve value ffi_dcl_restriction_set_pattern(
        r._ptr,
        Base.unsafe_convert(Cstring, value),
    )
end

"""
    clear_pattern!(r::DclRestriction)

Remove the pattern facet.
"""
function clear_pattern!(r::DclRestriction)
    ffi_dcl_restriction_clear_pattern(r._ptr)
end

"""
    total_digits(r::DclRestriction) -> Union{UInt32,Nothing}

Return the total-digits facet, or `nothing` if unset.
"""
function total_digits(r::DclRestriction)
    ffi_dcl_restriction_has_total_digits(r._ptr) != 0 ?
    ffi_dcl_restriction_total_digits(r._ptr) : nothing
end

"""
    set_total_digits!(r::DclRestriction, value::Integer)

Set the exact number of significant digits allowed. Throws [`VistaError`](@ref) if
`value` is zero.
"""
function set_total_digits!(r::DclRestriction, value::Integer)
    value == 0 &&
        throw(VistaError(InvalidArgument, "total_digits must be greater than zero"))
    ffi_dcl_restriction_set_total_digits(r._ptr, UInt32(value))
end

"""
    clear_total_digits!(r::DclRestriction)

Remove the total-digits facet.
"""
function clear_total_digits!(r::DclRestriction)
    ffi_dcl_restriction_clear_total_digits(r._ptr)
end

"""
    white_space(r::DclRestriction) -> Union{WhiteSpace,Nothing}

Return the white-space handling facet, or `nothing` if unset.
"""
function white_space(r::DclRestriction)
    ffi_dcl_restriction_has_white_space(r._ptr) != 0 ?
    _ws_from_ffi(ffi_dcl_restriction_white_space(r._ptr)) : nothing
end

"""
    set_white_space!(r::DclRestriction, value::WhiteSpace)

Set how white space (line feeds, tabs, spaces, carriage returns) is handled.
"""
function set_white_space!(r::DclRestriction, value::WhiteSpace)
    ffi_dcl_restriction_set_white_space(r._ptr, _ws_to_ffi(value))
end

"""
    clear_white_space!(r::DclRestriction)

Remove the white-space facet.
"""
function clear_white_space!(r::DclRestriction)
    ffi_dcl_restriction_clear_white_space(r._ptr)
end

"""
    validate_value(r::DclRestriction, value::AbstractString, fmt::DclFormat) -> Bool

Return `true` if `value` satisfies every facet set on `r`, given the base type `fmt`.
"""
function validate_value(r::DclRestriction, value::AbstractString, fmt)
    GC.@preserve value ffi_dcl_restriction_validate_value(
        r._ptr,
        Base.unsafe_convert(Cstring, value),
        fmt._ptr,
    ) != 0
end


"""
    fraction_digits(r::DclRestrictionRef) -> Union{UInt32,Nothing}

Return the fraction-digits facet, or `nothing` if unset.
"""
function fraction_digits(r::DclRestrictionRef)
    ffi_dcl_restriction_has_fraction_digits(r._ptr) != 0 ?
    ffi_dcl_restriction_fraction_digits(r._ptr) : nothing
end

"""
    white_space(r::DclRestrictionRef) -> Union{WhiteSpace,Nothing}

Return the white-space handling facet, or `nothing` if unset.
"""
function white_space(r::DclRestrictionRef)
    ffi_dcl_restriction_has_white_space(r._ptr) != 0 ?
    _ws_from_ffi(ffi_dcl_restriction_white_space(r._ptr)) : nothing
end

"""
    pattern(r::DclRestrictionRef) -> Union{String,Nothing}

Return the regular-expression pattern facet, or `nothing` if unset.
"""
function pattern(r::DclRestrictionRef)
    _opt_str(ffi_dcl_restriction_pattern(r._ptr))
end

"""
    enumeration_count(r::DclRestrictionRef) -> Int

Return the number of allowed enumeration values.
"""
function enumeration_count(r::DclRestrictionRef)
    Int(ffi_dcl_restriction_enumeration_count(r._ptr))
end

"""
    enumeration_at(r::DclRestrictionRef, index::Int) -> Union{String,Nothing}

Return the enumeration value at 1-based `index`, or `nothing` if out of range.
"""
function enumeration_at(r::DclRestrictionRef, index::Int)
    ptr = ffi_dcl_restriction_enumeration_at(r._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : unsafe_string(ptr)
end

"""
    total_digits(r::DclRestrictionRef) -> Union{UInt32,Nothing}

Return the total-digits facet, or `nothing` if unset.
"""
function total_digits(r::DclRestrictionRef)
    ffi_dcl_restriction_has_total_digits(r._ptr) != 0 ?
    ffi_dcl_restriction_total_digits(r._ptr) : nothing
end

"""
    length_(r::DclRestrictionRef) -> Union{UInt32,Nothing}

Return the exact-length facet, or `nothing` if unset.
"""
function length_(r::DclRestrictionRef)
    ffi_dcl_restriction_has_length(r._ptr) != 0 ? ffi_dcl_restriction_length(r._ptr) :
    nothing
end

"""
    max_exclusive(r::DclRestrictionRef) -> Union{Float64,Nothing}

Return the max-exclusive facet, or `nothing` if unset.
"""
function max_exclusive(r::DclRestrictionRef)
    ffi_dcl_restriction_has_max_exclusive(r._ptr) != 0 ?
    Float64(ffi_dcl_restriction_max_exclusive(r._ptr)) : nothing
end

"""
    max_inclusive(r::DclRestrictionRef) -> Union{Float64,Nothing}

Return the max-inclusive facet, or `nothing` if unset.
"""
function max_inclusive(r::DclRestrictionRef)
    ffi_dcl_restriction_has_max_inclusive(r._ptr) != 0 ?
    Float64(ffi_dcl_restriction_max_inclusive(r._ptr)) : nothing
end

"""
    max_length(r::DclRestrictionRef) -> Union{UInt32,Nothing}

Return the maximum allowed length facet, or `nothing` if unset.
"""
function max_length(r::DclRestrictionRef)
    ffi_dcl_restriction_has_max_length(r._ptr) != 0 ?
    ffi_dcl_restriction_max_length(r._ptr) : nothing
end

"""
    min_exclusive(r::DclRestrictionRef) -> Union{Float64,Nothing}

Return the min-exclusive facet, or `nothing` if unset.
"""
function min_exclusive(r::DclRestrictionRef)
    ffi_dcl_restriction_has_min_exclusive(r._ptr) != 0 ?
    Float64(ffi_dcl_restriction_min_exclusive(r._ptr)) : nothing
end

"""
    min_inclusive(r::DclRestrictionRef) -> Union{Float64,Nothing}

Return the min-inclusive facet, or `nothing` if unset.
"""
function min_inclusive(r::DclRestrictionRef)
    ffi_dcl_restriction_has_min_inclusive(r._ptr) != 0 ?
    Float64(ffi_dcl_restriction_min_inclusive(r._ptr)) : nothing
end

"""
    min_length(r::DclRestrictionRef) -> Union{UInt32,Nothing}

Return the minimum allowed length facet, or `nothing` if unset.
"""
function min_length(r::DclRestrictionRef)
    ffi_dcl_restriction_has_min_length(r._ptr) != 0 ?
    ffi_dcl_restriction_min_length(r._ptr) : nothing
end


"""
    DclRange(low::Real, high::Real) -> DclRange

Construct a range `[low, high]`. Throws [`VistaError`](@ref) if `low > high`.
"""
function DclRange(low::Real, high::Real)
    ptr = ffi_dcl_range_create(Cdouble(low), Cdouble(high))
    DclRange(ptr)
end

"""
    low(r::DclRange) -> Float64

Return the lower bound.
"""
function low(r::DclRange)
    Float64(ffi_dcl_range_low(r._ptr))
end

"""
    high(r::DclRange) -> Float64

Return the upper bound.
"""
function high(r::DclRange)
    Float64(ffi_dcl_range_high(r._ptr))
end

"""
    set_low!(r::DclRange, value::Real)

Set the lower bound. Throws [`VistaError`](@ref) if `value > high(r)`.
"""
function set_low!(r::DclRange, value::Real)
    value > high(r) && throw(VistaError(InvalidArgument, "low must be less than high"))
    ffi_dcl_range_set_low(r._ptr, Cdouble(value))
end

"""
    set_high!(r::DclRange, value::Real)

Set the upper bound. Throws [`VistaError`](@ref) if `value < low(r)`.
"""
function set_high!(r::DclRange, value::Real)
    value < low(r) && throw(VistaError(InvalidArgument, "high must be greater than low"))
    ffi_dcl_range_set_high(r._ptr, Cdouble(value))
end


"""
    low(r::DclRangeRef) -> Float64

Return the lower bound.
"""
low(r::DclRangeRef) = Float64(ffi_dcl_range_low(r._ptr))

"""
    high(r::DclRangeRef) -> Float64

Return the upper bound.
"""
high(r::DclRangeRef) = Float64(ffi_dcl_range_high(r._ptr))


"""
    DclFormat(type_::AbstractString) -> DclFormat

Construct a format with base type `type_` (e.g. `"Decimal"`, `"String"`).
"""
function DclFormat(type_::AbstractString)
    ptr = GC.@preserve type_ ffi_dcl_format_create(Base.unsafe_convert(Cstring, type_))
    DclFormat(ptr)
end

"""
    type_(f::DclFormat) -> String

Return the base type name (e.g. `"Decimal"`).
"""
function type_(f::DclFormat)
    _req_str(ffi_dcl_format_type(f._ptr))
end

"""
    set_type_!(f::DclFormat, type_::AbstractString)

Set the base type name. Throws [`VistaError`](@ref) if `type_` is not a recognized
ISO 19848 format data type.
"""
function set_type_!(f::DclFormat, type_::AbstractString)
    ok = GC.@preserve type_ ffi_dcl_format_set_type(
        f._ptr,
        Base.unsafe_convert(Cstring, type_),
    ) != 0
    ok || throw(last_error())
end

"""
    restriction(f::DclFormat) -> Union{DclRestrictionRef,Nothing}

Return the restriction applied to this format, or `nothing` if unset.
"""
function restriction(f::DclFormat)
    ptr = ffi_dcl_format_restriction(f._ptr)
    ptr == C_NULL ? nothing : DclRestrictionRef(ptr, f)
end

"""
    set_restriction!(f::DclFormat, r::DclRestriction)

Set the restriction applied to this format.
"""
function set_restriction!(f::DclFormat, r::DclRestriction)
    ffi_dcl_format_set_restriction(f._ptr, r._ptr)
end

"""
    clear_restriction!(f::DclFormat)

Remove the restriction from this format.
"""
function clear_restriction!(f::DclFormat)
    ffi_dcl_format_clear_restriction(f._ptr)
end

"""
    validate_value(f::DclFormat, value::AbstractString) -> Bool

Return `true` if `value` is a valid instance of this format's base type (and
restriction, if any).
"""
function validate_value(f::DclFormat, value::AbstractString)
    GC.@preserve value ffi_dcl_format_validate_value(
        f._ptr,
        Base.unsafe_convert(Cstring, value),
        Ptr{Ptr{Cvoid}}(C_NULL),
    ) != 0
end


"""
    type_(f::DclFormatRef) -> String

Return the base type name (e.g. `"Decimal"`).
"""
function type_(f::DclFormatRef)
    _req_str(ffi_dcl_format_type(f._ptr))
end

"""
    restriction(f::DclFormatRef) -> Union{DclRestrictionRef,Nothing}

Return the restriction applied to this format, or `nothing` if unset.
"""
function restriction(f::DclFormatRef)
    ptr = ffi_dcl_format_restriction(f._ptr)
    ptr == C_NULL ? nothing : DclRestrictionRef(ptr, f._owner)
end


"""
    DclDataChannelType(type_::AbstractString) -> DclDataChannelType

Construct a data channel type (e.g. `"Inst"`). Throws [`VistaError`](@ref) if `type_`
is not a recognized ISO 19848 data channel type.
"""
function DclDataChannelType(type_::AbstractString)
    ptr = GC.@preserve type_ ffi_dcl_data_channel_type_create(
        Base.unsafe_convert(Cstring, type_),
    )
    DclDataChannelType(ptr)
end

"""
    type_(t::DclDataChannelType) -> String

Return the type name (e.g. `"Inst"`).
"""
function type_(t::DclDataChannelType)
    _req_str(ffi_dcl_data_channel_type_type(t._ptr))
end

"""
    set_type_!(t::DclDataChannelType, type_::AbstractString)

Set the type name. Throws [`VistaError`](@ref) if `type_` is not a recognized ISO 19848
data channel type.
"""
function set_type_!(t::DclDataChannelType, type_::AbstractString)
    ok = GC.@preserve type_ ffi_dcl_data_channel_type_set_type(
        t._ptr,
        Base.unsafe_convert(Cstring, type_),
    ) != 0
    ok || throw(last_error())
end

"""
    update_cycle(t::DclDataChannelType) -> Union{Float64,Nothing}

Return the update cycle in seconds, or `nothing` if unset.
"""
function update_cycle(t::DclDataChannelType)
    ffi_dcl_data_channel_type_has_update_cycle(t._ptr) != 0 ?
    Float64(ffi_dcl_data_channel_type_update_cycle(t._ptr)) : nothing
end

"""
    set_update_cycle!(t::DclDataChannelType, value::Real)

Set the update cycle in seconds.
"""
function set_update_cycle!(t::DclDataChannelType, value::Real)
    ffi_dcl_data_channel_type_set_update_cycle(t._ptr, Cdouble(value))
end

"""
    clear_update_cycle!(t::DclDataChannelType)

Remove the update cycle.
"""
function clear_update_cycle!(t::DclDataChannelType)
    ffi_dcl_data_channel_type_clear_update_cycle(t._ptr)
end

"""
    calculation_period(t::DclDataChannelType) -> Union{Float64,Nothing}

Return the calculation period in seconds, or `nothing` if unset.
"""
function calculation_period(t::DclDataChannelType)
    ffi_dcl_data_channel_type_has_calculation_period(t._ptr) != 0 ?
    Float64(ffi_dcl_data_channel_type_calculation_period(t._ptr)) : nothing
end

"""
    set_calculation_period!(t::DclDataChannelType, value::Real)

Set the calculation period in seconds.
"""
function set_calculation_period!(t::DclDataChannelType, value::Real)
    ffi_dcl_data_channel_type_set_calculation_period(t._ptr, Cdouble(value))
end

"""
    clear_calculation_period!(t::DclDataChannelType)

Remove the calculation period.
"""
function clear_calculation_period!(t::DclDataChannelType)
    ffi_dcl_data_channel_type_clear_calculation_period(t._ptr)
end

"""
    is_alert(t::DclDataChannelType) -> Bool

Return `true` if this type is `"Alert"`.
"""
is_alert(t::DclDataChannelType) = ffi_dcl_data_channel_type_is_alert(t._ptr) != 0


"""
    type_(t::DclDataChannelTypeRef) -> String

Return the type name (e.g. `"Inst"`).
"""
type_(t::DclDataChannelTypeRef) = _req_str(ffi_dcl_data_channel_type_type(t._ptr))

"""
    update_cycle(t::DclDataChannelTypeRef) -> Union{Float64,Nothing}

Return the update cycle in seconds, or `nothing` if unset.
"""
function update_cycle(t::DclDataChannelTypeRef)
    ffi_dcl_data_channel_type_has_update_cycle(t._ptr) != 0 ?
    Float64(ffi_dcl_data_channel_type_update_cycle(t._ptr)) : nothing
end


"""
    DclNameObject() -> DclNameObject

Construct a name object with the default Annex C naming rule.
"""
DclNameObject() = DclNameObject(ffi_dcl_name_object_create_default())

"""
    DclNameObject(naming_rule::AbstractString) -> DclNameObject

Construct a name object with the given naming rule.
"""
function DclNameObject(naming_rule::AbstractString)
    ptr = GC.@preserve naming_rule ffi_dcl_name_object_create(
        Base.unsafe_convert(Cstring, naming_rule),
    )
    DclNameObject(ptr)
end

"""
    naming_rule(no::DclNameObject) -> String

Return the naming rule.
"""
function naming_rule(no::DclNameObject)
    _req_str(ffi_dcl_name_object_naming_rule(no._ptr))
end

"""
    set_naming_rule!(no::DclNameObject, naming_rule::AbstractString)

Set the naming rule.
"""
function set_naming_rule!(no::DclNameObject, naming_rule::AbstractString)
    GC.@preserve naming_rule ffi_dcl_name_object_set_naming_rule(
        no._ptr,
        Base.unsafe_convert(Cstring, naming_rule),
    )
end

"""
    custom_name_objects(no::DclNameObject) -> Union{SerializableDocumentRef,Nothing}

Return the custom name objects extension point (`xs:any`), or `nothing` if unset.
"""
function custom_name_objects(no::DclNameObject)
    ptr = ffi_dcl_name_object_custom_name_objects(no._ptr)
    ptr == C_NULL ? nothing : SerializableDocumentRef(ptr, no)
end

"""
    set_custom_name_objects!(no::DclNameObject, doc::SerializableDocument)

Set the custom name objects extension point, taking ownership of `doc`.
"""
function set_custom_name_objects!(no::DclNameObject, doc::SerializableDocument)
    ffi_dcl_name_object_set_custom_name_objects(no._ptr, _transfer_ownership(doc))
end

"""
    clear_custom_name_objects!(no::DclNameObject)

Remove the custom name objects extension point.
"""
function clear_custom_name_objects!(no::DclNameObject)
    ffi_dcl_name_object_clear_custom_name_objects(no._ptr)
end


"""
    naming_rule(no::DclNameObjectRef) -> String

Return the naming rule.
"""
naming_rule(no::DclNameObjectRef) = _req_str(ffi_dcl_name_object_naming_rule(no._ptr))


"""
    DclUnit(unit_symbol::AbstractString) -> DclUnit

Construct a unit with the given symbol (e.g. `"deg"`, `"Cel"`).
"""
function DclUnit(unit_symbol::AbstractString)
    ptr = GC.@preserve unit_symbol ffi_dcl_unit_create(
        Base.unsafe_convert(Cstring, unit_symbol),
    )
    DclUnit(ptr)
end

"""
    unit_symbol(u::DclUnit) -> String

Return the unit symbol.
"""
function unit_symbol(u::DclUnit)
    _req_str(ffi_dcl_unit_unit_symbol(u._ptr))
end

"""
    set_unit_symbol!(u::DclUnit, unit_symbol::AbstractString)

Set the unit symbol.
"""
function set_unit_symbol!(u::DclUnit, unit_symbol::AbstractString)
    GC.@preserve unit_symbol ffi_dcl_unit_set_unit_symbol(
        u._ptr,
        Base.unsafe_convert(Cstring, unit_symbol),
    )
end

"""
    quantity_name(u::DclUnit) -> Union{String,Nothing}

Return the quantity name, or `nothing` if unset.
"""
function quantity_name(u::DclUnit)
    _opt_str(ffi_dcl_unit_quantity_name(u._ptr))
end

"""
    set_quantity_name!(u::DclUnit, quantity_name::AbstractString)

Set the quantity name.
"""
function set_quantity_name!(u::DclUnit, quantity_name::AbstractString)
    GC.@preserve quantity_name ffi_dcl_unit_set_quantity_name(
        u._ptr,
        Base.unsafe_convert(Cstring, quantity_name),
    )
end

"""
    clear_quantity_name!(u::DclUnit)

Remove the quantity name.
"""
function clear_quantity_name!(u::DclUnit)
    ffi_dcl_unit_clear_quantity_name(u._ptr)
end

"""
    set_custom_elements!(u::DclUnit, doc::SerializableDocument)

Set the custom elements extension point (`xs:any`), taking ownership of `doc`.
No getter is exposed by the C API for this field.
"""
function set_custom_elements!(u::DclUnit, doc::SerializableDocument)
    ffi_dcl_unit_set_custom_elements(u._ptr, _transfer_ownership(doc))
end

"""
    clear_custom_elements!(u::DclUnit)

Remove the custom elements extension point.
"""
function clear_custom_elements!(u::DclUnit)
    ffi_dcl_unit_clear_custom_elements(u._ptr)
end


"""
    unit_symbol(u::DclUnitRef) -> String

Return the unit symbol.
"""
unit_symbol(u::DclUnitRef) = _req_str(ffi_dcl_unit_unit_symbol(u._ptr))

"""
    quantity_name(u::DclUnitRef) -> Union{String,Nothing}

Return the quantity name, or `nothing` if unset.
"""
quantity_name(u::DclUnitRef) = _opt_str(ffi_dcl_unit_quantity_name(u._ptr))


"""
    DclProperty(dct::DclDataChannelType, fmt::DclFormat) -> DclProperty

Construct a property with the given data channel type and format.
"""
function DclProperty(dct::DclDataChannelType, fmt::DclFormat)
    ptr = ffi_dcl_property_create(dct._ptr, fmt._ptr)
    DclProperty(ptr)
end

"""
    data_channel_type(p::DclProperty) -> DclDataChannelTypeRef

Return the data channel type.
"""
function data_channel_type(p::DclProperty)
    DclDataChannelTypeRef(ffi_dcl_property_data_channel_type(p._ptr), p)
end

"""
    set_data_channel_type!(p::DclProperty, dct::DclDataChannelType)

Set the data channel type.
"""
function set_data_channel_type!(p::DclProperty, dct::DclDataChannelType)
    ffi_dcl_property_set_data_channel_type(p._ptr, dct._ptr)
end

"""
    format(p::DclProperty) -> DclFormatRef

Return the value format.
"""
function format(p::DclProperty)
    DclFormatRef(ffi_dcl_property_format(p._ptr), p)
end

"""
    set_format!(p::DclProperty, fmt::DclFormat)

Set the value format.
"""
function set_format!(p::DclProperty, fmt::DclFormat)
    ffi_dcl_property_set_format(p._ptr, fmt._ptr)
end

"""
    range(p::DclProperty) -> Union{DclRangeRef,Nothing}

Return the allowed value range, or `nothing` if unset. Required when `format` is
`"Decimal"`.
"""
function range(p::DclProperty)
    ptr = ffi_dcl_property_range(p._ptr)
    ptr == C_NULL ? nothing : DclRangeRef(ptr, p)
end

"""
    set_range!(p::DclProperty, r::DclRange)

Set the allowed value range.
"""
function set_range!(p::DclProperty, r::DclRange)
    ffi_dcl_property_set_range(p._ptr, r._ptr)
end

"""
    clear_range!(p::DclProperty)

Remove the value range.
"""
function clear_range!(p::DclProperty)
    ffi_dcl_property_clear_range(p._ptr)
end

"""
    unit(p::DclProperty) -> Union{DclUnitRef,Nothing}

Return the unit of measurement, or `nothing` if unset. Required when `format` is
`"Decimal"`.
"""
function unit(p::DclProperty)
    ptr = ffi_dcl_property_unit(p._ptr)
    ptr == C_NULL ? nothing : DclUnitRef(ptr, p)
end

"""
    set_unit!(p::DclProperty, u::DclUnit)

Set the unit of measurement.
"""
function set_unit!(p::DclProperty, u::DclUnit)
    ffi_dcl_property_set_unit(p._ptr, u._ptr)
end

"""
    clear_unit!(p::DclProperty)

Remove the unit of measurement.
"""
function clear_unit!(p::DclProperty)
    ffi_dcl_property_clear_unit(p._ptr)
end

"""
    quality_coding(p::DclProperty) -> Union{String,Nothing}

Return the IEC 61162 quality coding, or `nothing` if unset.
"""
function quality_coding(p::DclProperty)
    _opt_str(ffi_dcl_property_quality_coding(p._ptr))
end

"""
    set_quality_coding!(p::DclProperty, v::AbstractString)

Set the IEC 61162 quality coding.
"""
function set_quality_coding!(p::DclProperty, v::AbstractString)
    GC.@preserve v ffi_dcl_property_set_quality_coding(
        p._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

"""
    clear_quality_coding!(p::DclProperty)

Remove the quality coding.
"""
function clear_quality_coding!(p::DclProperty)
    ffi_dcl_property_clear_quality_coding(p._ptr)
end

"""
    alert_priority(p::DclProperty) -> Union{String,Nothing}

Return the alert priority, or `nothing` if unset. Required when `data_channel_type`
is `"Alert"`.
"""
function alert_priority(p::DclProperty)
    _opt_str(ffi_dcl_property_alert_priority(p._ptr))
end

"""
    set_alert_priority!(p::DclProperty, v::AbstractString)

Set the alert priority.
"""
function set_alert_priority!(p::DclProperty, v::AbstractString)
    GC.@preserve v ffi_dcl_property_set_alert_priority(
        p._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

"""
    clear_alert_priority!(p::DclProperty)

Remove the alert priority.
"""
function clear_alert_priority!(p::DclProperty)
    ffi_dcl_property_clear_alert_priority(p._ptr)
end

"""
    name(p::DclProperty) -> Union{String,Nothing}

Return the human-readable name, or `nothing` if unset.
"""
function name(p::DclProperty)
    _opt_str(ffi_dcl_property_name(p._ptr))
end

"""
    set_name!(p::DclProperty, v::AbstractString)

Set the human-readable name.
"""
function set_name!(p::DclProperty, v::AbstractString)
    GC.@preserve v ffi_dcl_property_set_name(p._ptr, Base.unsafe_convert(Cstring, v))
end

"""
    clear_name!(p::DclProperty)

Remove the name.
"""
function clear_name!(p::DclProperty)
    ffi_dcl_property_clear_name(p._ptr)
end

"""
    remarks(p::DclProperty) -> Union{String,Nothing}

Return free-text remarks, or `nothing` if unset.
"""
function remarks(p::DclProperty)
    _opt_str(ffi_dcl_property_remarks(p._ptr))
end

"""
    set_remarks!(p::DclProperty, v::AbstractString)

Set free-text remarks.
"""
function set_remarks!(p::DclProperty, v::AbstractString)
    GC.@preserve v ffi_dcl_property_set_remarks(p._ptr, Base.unsafe_convert(Cstring, v))
end

"""
    clear_remarks!(p::DclProperty)

Remove the remarks.
"""
function clear_remarks!(p::DclProperty)
    ffi_dcl_property_clear_remarks(p._ptr)
end

"""
    custom_properties(p::DclProperty) -> Union{SerializableDocumentRef,Nothing}

Return the custom properties extension point (`xs:any`), or `nothing` if unset.
"""
function custom_properties(p::DclProperty)
    ptr = ffi_dcl_property_custom_properties(p._ptr)
    ptr == C_NULL ? nothing : SerializableDocumentRef(ptr, p)
end

"""
    set_custom_properties!(p::DclProperty, doc::SerializableDocument)

Set the custom properties extension point, taking ownership of `doc`.
"""
function set_custom_properties!(p::DclProperty, doc::SerializableDocument)
    ffi_dcl_property_set_custom_properties(p._ptr, _transfer_ownership(doc))
end

"""
    clear_custom_properties!(p::DclProperty)

Remove the custom properties extension point.
"""
function clear_custom_properties!(p::DclProperty)
    ffi_dcl_property_clear_custom_properties(p._ptr)
end

"""
    validate(p::DclProperty) -> Bool

Return `true` if `p` satisfies the ISO 19848 cross-field constraints (range/unit
required for Decimal format, alert priority required for Alert type).
"""
validate(p::DclProperty) = ffi_dcl_property_validate(p._ptr) != 0


"""
    name(p::DclPropertyRef) -> Union{String,Nothing}

Return the human-readable name, or `nothing` if unset.
"""
function name(p::DclPropertyRef)
    _opt_str(ffi_dcl_property_name(p._ptr))
end

"""
    remarks(p::DclPropertyRef) -> Union{String,Nothing}

Return free-text remarks, or `nothing` if unset.
"""
function remarks(p::DclPropertyRef)
    _opt_str(ffi_dcl_property_remarks(p._ptr))
end

"""
    data_channel_type(p::DclPropertyRef) -> DclDataChannelTypeRef

Return the data channel type.
"""
function data_channel_type(p::DclPropertyRef)
    DclDataChannelTypeRef(ffi_dcl_property_data_channel_type(p._ptr), p._owner)
end

"""
    format(p::DclPropertyRef) -> DclFormatRef

Return the value format.
"""
function format(p::DclPropertyRef)
    DclFormatRef(ffi_dcl_property_format(p._ptr), p._owner)
end

"""
    range(p::DclPropertyRef) -> Union{DclRangeRef,Nothing}

Return the allowed value range, or `nothing` if unset.
"""
function range(p::DclPropertyRef)
    ptr = ffi_dcl_property_range(p._ptr)
    ptr == C_NULL ? nothing : DclRangeRef(ptr, p._owner)
end

"""
    unit(p::DclPropertyRef) -> Union{DclUnitRef,Nothing}

Return the unit of measurement, or `nothing` if unset.
"""
function unit(p::DclPropertyRef)
    ptr = ffi_dcl_property_unit(p._ptr)
    ptr == C_NULL ? nothing : DclUnitRef(ptr, p._owner)
end


"""
    DclConfigurationReference(id::AbstractString, timestamp::DateTimeOffset) -> DclConfigurationReference

Construct a configuration reference with the given ID and timestamp.
"""
function DclConfigurationReference(id::AbstractString, timestamp::DateTimeOffset)
    ptr = GC.@preserve id ffi_dcl_configuration_reference_create(
        Base.unsafe_convert(Cstring, id),
        _ffi(timestamp),
    )
    DclConfigurationReference(ptr)
end

"""
    id(cr::DclConfigurationReference) -> String

Return the configuration ID.
"""
function id(cr::DclConfigurationReference)
    _req_str(ffi_dcl_configuration_reference_id(cr._ptr))
end

"""
    set_id!(cr::DclConfigurationReference, id::AbstractString)

Set the configuration ID.
"""
function set_id!(cr::DclConfigurationReference, id::AbstractString)
    GC.@preserve id ffi_dcl_configuration_reference_set_id(
        cr._ptr,
        Base.unsafe_convert(Cstring, id),
    )
end

"""
    version(cr::DclConfigurationReference) -> Union{String,Nothing}

Return the configuration version, or `nothing` if unset.
"""
function version(cr::DclConfigurationReference)
    _opt_str(ffi_dcl_configuration_reference_version(cr._ptr))
end

"""
    set_version!(cr::DclConfigurationReference, version::AbstractString)

Set the configuration version.
"""
function set_version!(cr::DclConfigurationReference, version::AbstractString)
    GC.@preserve version ffi_dcl_configuration_reference_set_version(
        cr._ptr,
        Base.unsafe_convert(Cstring, version),
    )
end

"""
    clear_version!(cr::DclConfigurationReference)

Remove the configuration version.
"""
function clear_version!(cr::DclConfigurationReference)
    ffi_dcl_configuration_reference_clear_version(cr._ptr)
end

"""
    timestamp(cr::DclConfigurationReference) -> DateTimeOffset

Return the timestamp of the referenced configuration.
"""
function timestamp(cr::DclConfigurationReference)
    _from_ffi(ffi_dcl_configuration_reference_timestamp(cr._ptr))
end

"""
    set_timestamp!(cr::DclConfigurationReference, ts::DateTimeOffset)

Set the timestamp of the referenced configuration.
"""
function set_timestamp!(cr::DclConfigurationReference, ts::DateTimeOffset)
    ffi_dcl_configuration_reference_set_timestamp(cr._ptr, _ffi(ts))
end


"""
    id(cr::DclConfigurationReferenceRef) -> String

Return the configuration ID.
"""
id(cr::DclConfigurationReferenceRef) = _req_str(ffi_dcl_configuration_reference_id(cr._ptr))

"""
    timestamp(cr::DclConfigurationReferenceRef) -> DateTimeOffset

Return the timestamp of the referenced configuration.
"""
timestamp(cr::DclConfigurationReferenceRef) =
    _from_ffi(ffi_dcl_configuration_reference_timestamp(cr._ptr))


"""
    DclVersionInformation() -> DclVersionInformation

Construct version information with default values.
"""
DclVersionInformation() =
    DclVersionInformation(ffi_dcl_version_information_create_default())

"""
    DclVersionInformation(naming_rule::AbstractString, naming_scheme_version::AbstractString) -> DclVersionInformation

Construct version information with the given naming rule and naming scheme version.
"""
function DclVersionInformation(
    naming_rule::AbstractString,
    naming_scheme_version::AbstractString,
)
    ptr = GC.@preserve naming_rule naming_scheme_version ffi_dcl_version_information_create(
        Base.unsafe_convert(Cstring, naming_rule),
        Base.unsafe_convert(Cstring, naming_scheme_version),
    )
    DclVersionInformation(ptr)
end

"""
    naming_rule(vi::DclVersionInformation) -> String

Return the naming rule.
"""
function naming_rule(vi::DclVersionInformation)
    _req_str(ffi_dcl_version_information_naming_rule(vi._ptr))
end

"""
    set_naming_rule!(vi::DclVersionInformation, naming_rule::AbstractString)

Set the naming rule.
"""
function set_naming_rule!(vi::DclVersionInformation, naming_rule::AbstractString)
    GC.@preserve naming_rule ffi_dcl_version_information_set_naming_rule(
        vi._ptr,
        Base.unsafe_convert(Cstring, naming_rule),
    )
end

"""
    naming_scheme_version(vi::DclVersionInformation) -> String

Return the naming scheme version.
"""
function naming_scheme_version(vi::DclVersionInformation)
    _req_str(ffi_dcl_version_information_naming_scheme_version(vi._ptr))
end

"""
    set_naming_scheme_version!(vi::DclVersionInformation, v::AbstractString)

Set the naming scheme version.
"""
function set_naming_scheme_version!(vi::DclVersionInformation, v::AbstractString)
    GC.@preserve v ffi_dcl_version_information_set_naming_scheme_version(
        vi._ptr,
        Base.unsafe_convert(Cstring, v),
    )
end

"""
    reference_url(vi::DclVersionInformation) -> Union{String,Nothing}

Return the reference URL, or `nothing` if unset.
"""
function reference_url(vi::DclVersionInformation)
    _opt_str(ffi_dcl_version_information_reference_url(vi._ptr))
end

"""
    set_reference_url!(vi::DclVersionInformation, url::AbstractString)

Set the reference URL.
"""
function set_reference_url!(vi::DclVersionInformation, url::AbstractString)
    GC.@preserve url ffi_dcl_version_information_set_reference_url(
        vi._ptr,
        Base.unsafe_convert(Cstring, url),
    )
end

"""
    clear_reference_url!(vi::DclVersionInformation)

Remove the reference URL.
"""
function clear_reference_url!(vi::DclVersionInformation)
    ffi_dcl_version_information_clear_reference_url(vi._ptr)
end


"""
    naming_rule(vi::DclVersionInformationRef) -> String

Return the naming rule.
"""
naming_rule(vi::DclVersionInformationRef) =
    _req_str(ffi_dcl_version_information_naming_rule(vi._ptr))

"""
    naming_scheme_version(vi::DclVersionInformationRef) -> Union{String,Nothing}

Return the naming scheme version, or `nothing` if unset.
"""
function naming_scheme_version(vi::DclVersionInformationRef)
    _opt_str(ffi_dcl_version_information_naming_scheme_version(vi._ptr))
end

"""
    reference_url(vi::DclVersionInformationRef) -> Union{String,Nothing}

Return the reference URL, or `nothing` if unset.
"""
reference_url(vi::DclVersionInformationRef) =
    _opt_str(ffi_dcl_version_information_reference_url(vi._ptr))


"""
    DclDataChannelId(local_id::LocalIdRef) -> DclDataChannelId

Construct a DataChannelID from a borrowed LocalID.
"""
function DclDataChannelId(local_id::LocalIdRef)
    ptr = ffi_dcl_channel_id_create(_ptr(local_id))
    DclDataChannelId(ptr)
end

"""
    DclDataChannelId(local_id::LocalId) -> DclDataChannelId

Construct a DataChannelID from an owned LocalID.
"""
function DclDataChannelId(local_id::LocalId)
    ptr = ffi_dcl_channel_id_create(_ptr(local_id))
    DclDataChannelId(ptr)
end

"""
    local_id_string(cid::DclDataChannelId) -> Union{String,Nothing}

Return the string representation of the LocalID, or `nothing` if unset.
"""
function local_id_string(cid::DclDataChannelId)
    lid_ptr = ffi_dcl_channel_id_local_id(cid._ptr)
    lid_ptr == C_NULL && return nothing
    s_ptr = ffi_local_id_to_string(lid_ptr)
    s_ptr == C_NULL && return nothing
    s = unsafe_string(s_ptr)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), s_ptr)
    s
end

"""
    short_id(cid::DclDataChannelId) -> Union{String,Nothing}

Return the short ID, or `nothing` if unset.
"""
function short_id(cid::DclDataChannelId)
    _opt_str(ffi_dcl_channel_id_short_id(cid._ptr))
end

"""
    set_short_id!(cid::DclDataChannelId, short_id::AbstractString)

Set the short ID.
"""
function set_short_id!(cid::DclDataChannelId, short_id::AbstractString)
    GC.@preserve short_id ffi_dcl_channel_id_set_short_id(
        cid._ptr,
        Base.unsafe_convert(Cstring, short_id),
    )
end

"""
    clear_short_id!(cid::DclDataChannelId)

Remove the short ID.
"""
function clear_short_id!(cid::DclDataChannelId)
    ffi_dcl_channel_id_clear_short_id(cid._ptr)
end

"""
    set_local_id!(cid::DclDataChannelId, local_id::Union{LocalId,LocalIdRef})

Set the LocalID.
"""
function set_local_id!(cid::DclDataChannelId, local_id::Union{LocalId,LocalIdRef})
    ffi_dcl_channel_id_set_local_id(cid._ptr, _ptr(local_id))
end

"""
    name_object(cid::DclDataChannelId) -> Union{DclNameObjectRef,Nothing}

Return the name object, or `nothing` if unset.
"""
function name_object(cid::DclDataChannelId)
    ptr = ffi_dcl_channel_id_name_object(cid._ptr)
    ptr == C_NULL ? nothing : DclNameObjectRef(ptr, cid)
end

"""
    set_name_object!(cid::DclDataChannelId, no::DclNameObject)

Set the name object.
"""
function set_name_object!(cid::DclDataChannelId, no::DclNameObject)
    ffi_dcl_channel_id_set_name_object(cid._ptr, no._ptr)
end

"""
    clear_name_object!(cid::DclDataChannelId)

Remove the name object.
"""
function clear_name_object!(cid::DclDataChannelId)
    ffi_dcl_channel_id_clear_name_object(cid._ptr)
end


"""
    short_id(cid::DclDataChannelIdRef) -> Union{String,Nothing}

Return the short ID, or `nothing` if unset.
"""
function short_id(cid::DclDataChannelIdRef)
    _opt_str(ffi_dcl_channel_id_short_id(cid._ptr))
end

"""
    local_id_string(cid::DclDataChannelIdRef) -> Union{String,Nothing}

Return the string representation of the LocalID, or `nothing` if unset.
"""
function local_id_string(cid::DclDataChannelIdRef)
    lid_ptr = ffi_dcl_channel_id_local_id(cid._ptr)
    lid_ptr == C_NULL && return nothing
    s_ptr = ffi_local_id_to_string(lid_ptr)
    s_ptr == C_NULL && return nothing
    s = unsafe_string(s_ptr)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), s_ptr)
    s
end

"""
    name_object(cid::DclDataChannelIdRef) -> Union{DclNameObjectRef,Nothing}

Return the name object, or `nothing` if unset.
"""
function name_object(cid::DclDataChannelIdRef)
    ptr = ffi_dcl_channel_id_name_object(cid._ptr)
    ptr == C_NULL ? nothing : DclNameObjectRef(ptr, cid._owner)
end


function _ship_id_to_ptr(s::ShipIdImo)
    imo_ptr = ffi_imo_number_create(Cint(s.imo._value))
    ship_ptr = ffi_ship_id_from_imo_number(imo_ptr)
    ffi_imo_number_free(imo_ptr)
    ship_ptr
end

function _ship_id_to_ptr(s::ShipIdOther)
    GC.@preserve s ffi_ship_id_from_other_id(Base.unsafe_convert(Cstring, s.id))
end

function _ship_id_from_ptr(ptr::Ptr{Cvoid})
    if ffi_ship_id_is_imo_number(ptr) != 0
        imo_ptr = ffi_ship_id_imo_number(ptr)
        v = ffi_imo_number_value(imo_ptr)
        ffi_imo_number_free(imo_ptr)
        ShipIdImo(ImoNumber(UInt32(v)))
    else
        ShipIdOther(unsafe_string(ffi_ship_id_other_id(ptr)))
    end
end

"""
    DclHeader(ship_id::ShipId, data_channel_list_id::DclConfigurationReference) -> DclHeader

Construct a header with the given ship ID and configuration reference.
"""
function DclHeader(ship_id::ShipId, data_channel_list_id::DclConfigurationReference)
    ship_ptr = _ship_id_to_ptr(ship_id)
    ptr = ffi_dcl_header_create(ship_ptr, data_channel_list_id._ptr)
    ffi_ship_id_free(ship_ptr)
    DclHeader(ptr)
end

"""
    ship_id(h::DclHeader) -> Union{ShipId,Nothing}

Return the ship ID, or `nothing` if unset.
"""
function ship_id(h::DclHeader)
    ptr = ffi_dcl_header_ship_id(h._ptr)
    ptr == C_NULL ? nothing : _ship_id_from_ptr(ptr)
end

"""
    set_ship_id!(h::DclHeader, ship_id::ShipId)

Set the ship ID.
"""
function set_ship_id!(h::DclHeader, ship_id::ShipId)
    ship_ptr = _ship_id_to_ptr(ship_id)
    ffi_dcl_header_set_ship_id(h._ptr, ship_ptr)
    ffi_ship_id_free(ship_ptr)
end

"""
    data_channel_list_id(h::DclHeader) -> DclConfigurationReferenceRef

Return the configuration reference identifying this DataChannelList.
"""
function data_channel_list_id(h::DclHeader)
    DclConfigurationReferenceRef(ffi_dcl_header_data_channel_list_id(h._ptr), h)
end

"""
    set_data_channel_list_id!(h::DclHeader, cr::DclConfigurationReference)

Set the configuration reference identifying this DataChannelList.
"""
function set_data_channel_list_id!(h::DclHeader, cr::DclConfigurationReference)
    ffi_dcl_header_set_data_channel_list_id(h._ptr, cr._ptr)
end

"""
    version_information(h::DclHeader) -> Union{DclVersionInformationRef,Nothing}

Return the naming scheme version information, or `nothing` if unset.
"""
function version_information(h::DclHeader)
    ptr = ffi_dcl_header_version_information(h._ptr)
    ptr == C_NULL ? nothing : DclVersionInformationRef(ptr, h)
end

"""
    set_version_information!(h::DclHeader, vi::DclVersionInformation)

Set the naming scheme version information.
"""
function set_version_information!(h::DclHeader, vi::DclVersionInformation)
    ffi_dcl_header_set_version_information(h._ptr, vi._ptr)
end

"""
    clear_version_information!(h::DclHeader)

Remove the naming scheme version information.
"""
function clear_version_information!(h::DclHeader)
    ffi_dcl_header_clear_version_information(h._ptr)
end

"""
    author(h::DclHeader) -> Union{String,Nothing}

Return the author, or `nothing` if unset.
"""
function author(h::DclHeader)
    _opt_str(ffi_dcl_header_author(h._ptr))
end

"""
    set_author!(h::DclHeader, author::AbstractString)

Set the author.
"""
function set_author!(h::DclHeader, author::AbstractString)
    GC.@preserve author ffi_dcl_header_set_author(
        h._ptr,
        Base.unsafe_convert(Cstring, author),
    )
end

"""
    clear_author!(h::DclHeader)

Remove the author.
"""
function clear_author!(h::DclHeader)
    ffi_dcl_header_clear_author(h._ptr)
end

"""
    date_created(h::DclHeader) -> Union{DateTimeOffset,Nothing}

Return the creation date, or `nothing` if unset.
"""
function date_created(h::DclHeader)
    ffi_dcl_header_has_date_created(h._ptr) != 0 ?
    _from_ffi(ffi_dcl_header_date_created(h._ptr)) : nothing
end

"""
    set_date_created!(h::DclHeader, dt::DateTimeOffset)

Set the creation date.
"""
function set_date_created!(h::DclHeader, dt::DateTimeOffset)
    ffi_dcl_header_set_date_created(h._ptr, _ffi(dt))
end

"""
    clear_date_created!(h::DclHeader)

Remove the creation date.
"""
function clear_date_created!(h::DclHeader)
    ffi_dcl_header_clear_date_created(h._ptr)
end

"""
    custom_headers(h::DclHeader) -> Union{SerializableDocumentRef,Nothing}

Return the custom headers extension point (`xs:any`), or `nothing` if unset.
"""
function custom_headers(h::DclHeader)
    ptr = ffi_dcl_header_custom_headers(h._ptr)
    ptr == C_NULL ? nothing : SerializableDocumentRef(ptr, h)
end

"""
    set_custom_headers!(h::DclHeader, doc::SerializableDocument)

Set the custom headers extension point, taking ownership of `doc`.
"""
function set_custom_headers!(h::DclHeader, doc::SerializableDocument)
    ffi_dcl_header_set_custom_headers(h._ptr, _transfer_ownership(doc))
end

"""
    clear_custom_headers!(h::DclHeader)

Remove the custom headers extension point.
"""
function clear_custom_headers!(h::DclHeader)
    ffi_dcl_header_clear_custom_headers(h._ptr)
end


"""
    ship_id(h::DclHeaderRef) -> Union{ShipId,Nothing}

Return the ship ID, or `nothing` if unset.
"""
function ship_id(h::DclHeaderRef)
    ptr = ffi_dcl_header_ship_id(h._ptr)
    ptr == C_NULL ? nothing : _ship_id_from_ptr(ptr)
end

"""
    data_channel_list_id(h::DclHeaderRef) -> DclConfigurationReferenceRef

Return the configuration reference identifying this DataChannelList.
"""
function data_channel_list_id(h::DclHeaderRef)
    DclConfigurationReferenceRef(ffi_dcl_header_data_channel_list_id(h._ptr), h._owner)
end

"""
    version_information(h::DclHeaderRef) -> Union{DclVersionInformationRef,Nothing}

Return the naming scheme version information, or `nothing` if unset.
"""
function version_information(h::DclHeaderRef)
    ptr = ffi_dcl_header_version_information(h._ptr)
    ptr == C_NULL ? nothing : DclVersionInformationRef(ptr, h._owner)
end

"""
    author(h::DclHeaderRef) -> Union{String,Nothing}

Return the author, or `nothing` if unset.
"""
function author(h::DclHeaderRef)
    _opt_str(ffi_dcl_header_author(h._ptr))
end

"""
    date_created(h::DclHeaderRef) -> Union{DateTimeOffset,Nothing}

Return the creation date, or `nothing` if unset.
"""
function date_created(h::DclHeaderRef)
    ffi_dcl_header_has_date_created(h._ptr) != 0 ?
    _from_ffi(ffi_dcl_header_date_created(h._ptr)) : nothing
end

"""
    custom_headers(h::DclHeaderRef) -> Union{SerializableDocumentRef,Nothing}

Return the custom headers extension point (`xs:any`), or `nothing` if unset.
"""
function custom_headers(h::DclHeaderRef)
    ptr = ffi_dcl_header_custom_headers(h._ptr)
    ptr == C_NULL ? nothing : SerializableDocumentRef(ptr, h._owner)
end


"""
    DclDataChannel(channel_id::DclDataChannelId, property::DclProperty) -> DclDataChannel

Construct a data channel from a channel ID and a property.
"""
function DclDataChannel(channel_id::DclDataChannelId, property::DclProperty)
    ptr = ffi_dcl_data_channel_create(channel_id._ptr, property._ptr)
    DclDataChannel(ptr)
end

"""
    channel_id(dc::DclDataChannel) -> DclDataChannelIdRef

Return the channel ID.
"""
function channel_id(dc::DclDataChannel)
    DclDataChannelIdRef(ffi_dcl_data_channel_channel_id(dc._ptr), dc)
end

"""
    set_channel_id!(dc::DclDataChannel, cid::DclDataChannelId)

Set the channel ID.
"""
function set_channel_id!(dc::DclDataChannel, cid::DclDataChannelId)
    ffi_dcl_data_channel_set_channel_id(dc._ptr, cid._ptr)
end

"""
    property(dc::DclDataChannel) -> DclPropertyRef

Return the channel's property.
"""
function property(dc::DclDataChannel)
    DclPropertyRef(ffi_dcl_data_channel_property(dc._ptr), dc)
end

"""
    set_property!(dc::DclDataChannel, p::DclProperty)

Set the channel's property.

Throws [`VistaError`](@ref) if `p` is invalid.
"""
function set_property!(dc::DclDataChannel, p::DclProperty)
    ok = ffi_dcl_data_channel_set_property(dc._ptr, p._ptr) != 0
    ok || throw(last_error())
end


"""
    channel_id(dc::DclDataChannelRef) -> DclDataChannelIdRef

Return the channel ID.
"""
function channel_id(dc::DclDataChannelRef)
    DclDataChannelIdRef(ffi_dcl_data_channel_channel_id(dc._ptr), dc._owner)
end

"""
    property(dc::DclDataChannelRef) -> DclPropertyRef

Return the channel's property.
"""
function property(dc::DclDataChannelRef)
    DclPropertyRef(ffi_dcl_data_channel_property(dc._ptr), dc._owner)
end


"""
    DclDataChannelList() -> DclDataChannelList

Construct an empty data channel list.
"""
DclDataChannelList() = DclDataChannelList(ffi_dcl_data_channel_list_create())

"""
    length(l::DclDataChannelList) -> Int

Return the number of channels in the list.
"""
function Base.length(l::DclDataChannelList)
    Int(ffi_dcl_data_channel_list_size(l._ptr))
end

"""
    isempty(l::DclDataChannelList) -> Bool

Return `true` if the list has no channels.
"""
function Base.isempty(l::DclDataChannelList)
    length(l) == 0
end

"""
    getindex(l::DclDataChannelList, index::Int) -> DclDataChannelRef

Return the channel at `index` (1-based).

Throws `BoundsError` if `index` is out of range.
"""
function Base.getindex(l::DclDataChannelList, index::Int)
    ptr = ffi_dcl_data_channel_list_at(l._ptr, Csize_t(index - 1))
    ptr == C_NULL && throw(BoundsError(l, index))
    DclDataChannelRef(ptr, l)
end

"""
    from_short_id(l::DclDataChannelList, short_id::AbstractString) -> Union{DclDataChannelRef,Nothing}

Return the channel with the given short ID, or `nothing` if not found.
"""
function from_short_id(l::DclDataChannelList, short_id::AbstractString)
    ptr = GC.@preserve short_id ffi_dcl_data_channel_list_from_short_id(
        l._ptr,
        Base.unsafe_convert(Cstring, short_id),
    )
    ptr == C_NULL ? nothing : DclDataChannelRef(ptr, l)
end

"""
    from_local_id(l::DclDataChannelList, local_id::Union{LocalId,LocalIdRef}) -> Union{DclDataChannelRef,Nothing}

Return the channel with the given LocalID, or `nothing` if not found.
"""
function from_local_id(l::DclDataChannelList, local_id::Union{LocalId,LocalIdRef})
    ptr = ffi_dcl_data_channel_list_from_local_id(l._ptr, _ptr(local_id))
    ptr == C_NULL ? nothing : DclDataChannelRef(ptr, l)
end

"""
    add!(l::DclDataChannelList, dc::DclDataChannel) -> Bool

Append `dc` to the list. Returns `true` on success.
"""
function add!(l::DclDataChannelList, dc::DclDataChannel)
    ffi_dcl_data_channel_list_add(l._ptr, dc._ptr) != 0
end

"""
    remove!(l::DclDataChannelList, dc::DclDataChannel) -> Bool

Remove `dc` from the list. Returns `true` on success.
"""
function remove!(l::DclDataChannelList, dc::DclDataChannel)
    ffi_dcl_data_channel_list_remove(l._ptr, dc._ptr) != 0
end

"""
    empty!(l::DclDataChannelList) -> DclDataChannelList

Remove all channels from the list.
"""
function Base.empty!(l::DclDataChannelList)
    ffi_dcl_data_channel_list_clear(l._ptr)
    l
end


"""
    length(l::DclDataChannelListRef) -> Int

Return the number of channels in the list.
"""
function Base.length(l::DclDataChannelListRef)
    Int(ffi_dcl_data_channel_list_size(l._ptr))
end

"""
    isempty(l::DclDataChannelListRef) -> Bool

Return `true` if the list has no channels.
"""
Base.isempty(l::DclDataChannelListRef) = length(l) == 0

"""
    getindex(l::DclDataChannelListRef, index::Int) -> DclDataChannelRef

Return the channel at `index` (1-based).

Throws `BoundsError` if `index` is out of range.
"""
function Base.getindex(l::DclDataChannelListRef, index::Int)
    ptr = ffi_dcl_data_channel_list_at(l._ptr, Csize_t(index - 1))
    ptr == C_NULL && throw(BoundsError(l, index))
    DclDataChannelRef(ptr, l._owner)
end

"""
    from_short_id(l::DclDataChannelListRef, short_id::AbstractString) -> Union{DclDataChannelRef,Nothing}

Return the channel with the given short ID, or `nothing` if not found.
"""
function from_short_id(l::DclDataChannelListRef, short_id::AbstractString)
    ptr = GC.@preserve short_id ffi_dcl_data_channel_list_from_short_id(
        l._ptr,
        Base.unsafe_convert(Cstring, short_id),
    )
    ptr == C_NULL ? nothing : DclDataChannelRef(ptr, l._owner)
end

"""
    from_local_id(l::DclDataChannelListRef, local_id::Union{LocalId,LocalIdRef}) -> Union{DclDataChannelRef,Nothing}

Return the channel with the given LocalID, or `nothing` if not found.
"""
function from_local_id(l::DclDataChannelListRef, local_id::Union{LocalId,LocalIdRef})
    ptr = ffi_dcl_data_channel_list_from_local_id(l._ptr, _ptr(local_id))
    ptr == C_NULL ? nothing : DclDataChannelRef(ptr, l._owner)
end

"""
    DclPackage(header::DclHeader, data_channel_list::DclDataChannelList) -> DclPackage

Construct a package from a header and a data channel list.
"""
function DclPackage(header::DclHeader, data_channel_list::DclDataChannelList)
    ptr = ffi_dcl_package_create(header._ptr, data_channel_list._ptr)
    DclPackage(ptr)
end

"""
    header(pkg::DclPackage) -> Union{DclHeaderRef,Nothing}

Return the package header, or `nothing` if unset.
"""
function header(pkg::DclPackage)
    ptr = ffi_dcl_package_header(pkg._ptr)
    ptr == C_NULL ? nothing : DclHeaderRef(ptr, pkg)
end

"""
    set_header!(pkg::DclPackage, h::DclHeader)

Set the package header.
"""
function set_header!(pkg::DclPackage, h::DclHeader)
    ffi_dcl_package_set_header(pkg._ptr, h._ptr)
end

"""
    data_channel_list(pkg::DclPackage) -> Union{DclDataChannelListRef,Nothing}

Return the package's data channel list, or `nothing` if unset.
"""
function data_channel_list(pkg::DclPackage)
    ptr = ffi_dcl_package_data_channel_list(pkg._ptr)
    ptr == C_NULL ? nothing : DclDataChannelListRef(ptr, pkg)
end

"""
    set_data_channel_list!(pkg::DclPackage, l::DclDataChannelList)

Set the package's data channel list.
"""
function set_data_channel_list!(pkg::DclPackage, l::DclDataChannelList)
    ffi_dcl_package_set_data_channel_list(pkg._ptr, l._ptr)
end


"""
    header(pkg::DclPackageRef) -> Union{DclHeaderRef,Nothing}

Return the package header, or `nothing` if unset.
"""
function header(pkg::DclPackageRef)
    ptr = ffi_dcl_package_header(pkg._ptr)
    ptr == C_NULL ? nothing : DclHeaderRef(ptr, pkg._owner)
end

"""
    data_channel_list(pkg::DclPackageRef) -> Union{DclDataChannelListRef,Nothing}

Return the package's data channel list, or `nothing` if unset.
"""
function data_channel_list(pkg::DclPackageRef)
    ptr = ffi_dcl_package_data_channel_list(pkg._ptr)
    ptr == C_NULL ? nothing : DclDataChannelListRef(ptr, pkg._owner)
end


"""
    DclListPackage(pkg::DclPackage) -> DclListPackage

Construct a list package wrapping `pkg`.
"""
DclListPackage(pkg::DclPackage) = DclListPackage(ffi_dcl_list_package_create(pkg._ptr))

"""
    set_package!(lp::DclListPackage, pkg::DclPackage)

Set the wrapped package.
"""
function set_package!(lp::DclListPackage, pkg::DclPackage)
    ffi_dcl_list_package_set_package(lp._ptr, pkg._ptr)
end

"""
    package(lp::DclListPackage) -> Union{DclPackageRef,Nothing}

Return the wrapped package, or `nothing` if unset.
"""
function package(lp::DclListPackage)
    ptr = ffi_dcl_list_package_package(lp._ptr)
    ptr == C_NULL ? nothing : DclPackageRef(ptr, lp)
end

"""
    data_channel_list(lp::DclListPackage) -> Union{DclDataChannelListRef,Nothing}

Return the data channel list of the wrapped package, or `nothing` if unset.
"""
function data_channel_list(lp::DclListPackage)
    ptr = ffi_dcl_list_package_data_channel_list(lp._ptr)
    ptr == C_NULL ? nothing : DclDataChannelListRef(ptr, lp)
end

"""
    dcl_from_json(json::AbstractString) -> DclListPackage

Parse a DataChannelList JSON document.

Throws [`VistaError`](@ref) if `json` is not a valid DataChannelList document.
"""
function dcl_from_json(json::AbstractString)
    ptr =
        GC.@preserve json ffi_dcl_list_package_from_json(Base.unsafe_convert(Cstring, json))
    DclListPackage(ptr)
end

"""
    dcl_to_json(lp::DclListPackage; pretty::Bool = false) -> String

Serialize `lp` to a DataChannelList JSON document. Returns an empty string on
failure.
"""
function dcl_to_json(lp::DclListPackage; pretty::Bool = false)
    raw = ffi_dcl_list_package_to_json(lp._ptr, pretty ? Cint(1) : Cint(0))
    raw == C_NULL && return ""
    s = unsafe_string(raw)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), raw)
    s
end
