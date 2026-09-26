"""
    ShipIdImo

A [`ShipId`](@ref) variant holding a validated IMO vessel number.
"""
struct ShipIdImo
    imo::ImoNumber
end

"""
    ShipIdOther

A [`ShipId`](@ref) variant holding an alternative ship identifier string.
"""
struct ShipIdOther
    id::String
end

"""
    ShipId

ISO 19848 ship identifier: either a validated IMO number ([`ShipIdImo`](@ref))
or an alternative string identifier ([`ShipIdOther`](@ref)).

Use [`from_imo_number`](@ref), [`from_other_id`](@ref), or [`from_string`](@ref)
(also `Base.parse`) to construct.
"""
const ShipId = Union{ShipIdImo,ShipIdOther}

"""
    from_imo_number(imo::ImoNumber) -> ShipId

Construct a `ShipId` from a validated IMO number.
"""
function from_imo_number(imo::ImoNumber)
    ShipIdImo(imo)
end

"""
    from_other_id(s::AbstractString) -> ShipId

Construct a `ShipId` from an alternative identifier string.
Throws [`VistaError`](@ref) if `s` is empty.
"""
function from_other_id(s::AbstractString)
    isempty(s) && throw(VistaError(InvalidArgument, "other_id must not be empty"))
    ptr = GC.@preserve s ffi_ship_id_from_other_id(Base.unsafe_convert(Cstring, s))
    if ptr == C_NULL
        throw(last_error())
    end
    ffi_ship_id_free(ptr)
    ShipIdOther(String(s))
end

"""
    from_string(::Type{ShipId}, s::AbstractString) -> Union{ShipId,Nothing}

Parse a `ShipId` from its string representation.
Strings starting with `"IMO"` (case-insensitive) followed by a valid IMO number
produce a [`ShipIdImo`](@ref); all other non-empty strings produce [`ShipIdOther`](@ref).
Returns `nothing` if `s` is empty or whitespace-only.
"""
function from_string(::Core.Type{ShipId}, s::AbstractString)
    ptr = GC.@preserve s ffi_ship_id_from_string(Base.unsafe_convert(Cstring, s))
    ptr == C_NULL && return nothing
    result = if ffi_ship_id_is_imo_number(ptr) != 0
        imo_ptr = ffi_ship_id_imo_number(ptr)
        v = ffi_imo_number_value(imo_ptr)
        ffi_imo_number_free(imo_ptr)
        ShipIdImo(ImoNumber(UInt32(v)))
    else
        raw = ffi_ship_id_other_id(ptr)
        ShipIdOther(unsafe_string(raw))
    end
    ffi_ship_id_free(ptr)
    result
end

"""
    Base.parse(::Type{ShipId}, s::AbstractString) -> ShipId

Parse a `ShipId` from its string representation.
Throws `ArgumentError` if `s` is empty or whitespace-only.
"""
function Base.parse(::Core.Type{ShipId}, s::AbstractString)
    r = from_string(ShipId, s)
    r === nothing &&
        throw(ArgumentError("cannot parse ShipId from empty or whitespace string"))
    r
end

"""
    imo_number(s::ShipId) -> Union{ImoNumber,Nothing}

Return the `ImoNumber` if `s` is a [`ShipIdImo`](@ref), otherwise `nothing`.
"""
imo_number(s::ShipIdImo) = s.imo
imo_number(::ShipIdOther) = nothing

"""
    other_id(s::ShipId) -> Union{String,Nothing}

Return the alternative identifier if `s` is a [`ShipIdOther`](@ref), otherwise `nothing`.
"""
other_id(::ShipIdImo) = nothing
other_id(s::ShipIdOther) = s.id

"""
    is_imo_number(s::ShipId) -> Bool

Return `true` if `s` holds an IMO number.
"""
is_imo_number(::ShipIdImo) = true
is_imo_number(::ShipIdOther) = false

"""
    is_other_id(s::ShipId) -> Bool

Return `true` if `s` holds an alternative identifier.
"""
is_other_id(::ShipIdImo) = false
is_other_id(::ShipIdOther) = true

Base.show(io::IO, s::ShipIdImo) = print(io, "IMO", s.imo._value)
Base.show(io::IO, s::ShipIdOther) = print(io, s.id)
Base.string(s::ShipIdImo) = string("IMO", s.imo._value)
Base.string(s::ShipIdOther) = s.id

Base.:(==)(a::ShipIdImo, b::ShipIdImo) = a.imo == b.imo
Base.:(==)(a::ShipIdOther, b::ShipIdOther) = a.id == b.id
Base.:(==)(::ShipIdImo, ::ShipIdOther) = false
Base.:(==)(::ShipIdOther, ::ShipIdImo) = false

Base.hash(s::ShipIdImo, h::UInt) = hash((:ShipIdImo, s.imo), h)
Base.hash(s::ShipIdOther, h::UInt) = hash((:ShipIdOther, s.id), h)
