"""
    Locations

Container for all valid relative locations for a specific VIS version.

Borrowed from the VIS singleton — valid for the lifetime of the program.
"""
struct Locations
    _ptr::Ptr{Cvoid}
end

"""
    version(locs::Locations) -> VisVersion

Return the VIS version these locations belong to.
"""
function version(locs::Locations)
    ptr = ffi_locations_version(locs._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_locations_version returned NULL")
    parse(VisVersion, unsafe_string(ptr))
end

"""
    Base.length(locs::Locations) -> Int

Return the number of relative locations.
"""
function Base.length(locs::Locations)
    Int(ffi_locations_relative_location_count(locs._ptr))
end

"""
    Base.getindex(locs::Locations, i::Integer) -> RelativeLocation

Return the relative location at 1-based index `i`.
"""
function Base.getindex(locs::Locations, i::Integer)
    (i < 1 || i > length(locs)) && throw(BoundsError(locs, i))
    ptr = ffi_locations_relative_location_at(locs._ptr, i - 1)
    ptr == C_NULL && throw(BoundsError(locs, i))
    RelativeLocation(ptr)
end

"""
    Base.iterate(locs::Locations)

Iterate over all [`RelativeLocation`](@ref) entries.
"""
function Base.iterate(locs::Locations, i::Int = 1)
    i > length(locs) && return nothing
    (locs[i], i + 1)
end

"""
    group(locs::Locations, grp::LocationGroup) -> Vector{RelativeLocation}

Return all relative locations belonging to `grp`.
"""
function group(locs::Locations, grp::LocationGroup)
    count = Int(ffi_locations_group_count(locs._ptr, grp))
    result = RelativeLocation[]
    for i = 0:(count-1)
        ptr = ffi_locations_group_at(locs._ptr, grp, i)
        ptr != C_NULL && push!(result, RelativeLocation(ptr))
    end
    result
end

"""
    Base.parse(::Type{Location}, locs::Locations, s::AbstractString) -> Union{Location,Nothing}

Parse a location string (e.g. `"11FIPU"`). Returns `nothing` if invalid.
"""
function Base.parse(::Base.Type{Location}, locs::Locations, s::AbstractString)
    ptr =
        GC.@preserve s ffi_locations_from_string(locs._ptr, Base.unsafe_convert(Cstring, s))
    ptr == C_NULL ? nothing : Location(ptr)
end

"""
    parse_with_errors(locs::Locations, s::AbstractString) -> Tuple{Union{Location,Nothing}, ParsingErrors}

Parse a location string, returning detailed error information on failure.
"""
function parse_with_errors(locs::Locations, s::AbstractString)
    out_errors = Ref{Ptr{Cvoid}}(C_NULL)
    ptr = GC.@preserve s ffi_locations_from_string_with_errors(
        locs._ptr,
        Base.unsafe_convert(Cstring, s),
        out_errors,
    )
    location = ptr == C_NULL ? nothing : Location(ptr)
    errors = out_errors[] == C_NULL ? ParsingErrors() : ParsingErrors(out_errors[])
    (location, errors)
end
