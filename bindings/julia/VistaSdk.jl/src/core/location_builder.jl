"""
    LocationBuilder

Builder for constructing a [`Location`](@ref) one component at a time.
Owns a C resource. Freed when garbage-collected.

Each `with_*`/`without_*` method returns a new independent builder. Use [`build`](@ref)
to produce the final [`Location`](@ref).

# Examples
```julia
locs = locations(vis(), latest(vis()))
loc = build(
    with_number(LocationBuilder(locs), 11) |>
    b -> with_side(b, 'P') |>
    b -> with_transverse(b, 'I')
)
string(loc)  # "11IP"
```
"""
mutable struct LocationBuilder
    _ptr::Ptr{Cvoid}
    function LocationBuilder(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("LocationBuilder: NULL pointer")
        t = new(ptr)
        finalizer(t) do x
            ffi_location_builder_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    LocationBuilder(locs::Locations) -> LocationBuilder

Create a new empty builder for `locs`.
"""
function LocationBuilder(locs::Locations)
    ptr = ffi_location_builder_create(locs._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_location_builder_create returned NULL")
    LocationBuilder(ptr)
end

"""
    version(b::LocationBuilder) -> VisVersion

Return the VIS version this builder validates against.
"""
function version(b::LocationBuilder)
    ptr = ffi_location_builder_version(b._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_location_builder_version returned NULL")
    parse(VisVersion, unsafe_string(ptr))
end

"""
    number(b::LocationBuilder) -> Union{Int,Nothing}

Return the number component, or `nothing` if not set.
"""
function number(b::LocationBuilder)
    out = Ref{Cint}(0)
    ok = ffi_location_builder_number(b._ptr, out)
    ok != 0 ? Int(out[]) : nothing
end

"""
    side(b::LocationBuilder) -> Union{Char,Nothing}

Return the side component, or `nothing` if not set.
"""
function side(b::LocationBuilder)
    out = Ref{Cchar}(0)
    ok = ffi_location_builder_side(b._ptr, out)
    ok != 0 ? Char(UInt8(out[])) : nothing
end

"""
    vertical(b::LocationBuilder) -> Union{Char,Nothing}

Return the vertical component, or `nothing` if not set.
"""
function vertical(b::LocationBuilder)
    out = Ref{Cchar}(0)
    ok = ffi_location_builder_vertical(b._ptr, out)
    ok != 0 ? Char(UInt8(out[])) : nothing
end

"""
    transverse(b::LocationBuilder) -> Union{Char,Nothing}

Return the transverse component, or `nothing` if not set.
"""
function transverse(b::LocationBuilder)
    out = Ref{Cchar}(0)
    ok = ffi_location_builder_transverse(b._ptr, out)
    ok != 0 ? Char(UInt8(out[])) : nothing
end

"""
    longitudinal(b::LocationBuilder) -> Union{Char,Nothing}

Return the longitudinal component, or `nothing` if not set.
"""
function longitudinal(b::LocationBuilder)
    out = Ref{Cchar}(0)
    ok = ffi_location_builder_longitudinal(b._ptr, out)
    ok != 0 ? Char(UInt8(out[])) : nothing
end

"""
    with_number(b::LocationBuilder, n::Integer) -> LocationBuilder

Return a new builder with the number component set to `n`. Throws [`VistaError`](@ref) if invalid.
"""
function with_number(b::LocationBuilder, n::Integer)
    ptr = ffi_location_builder_with_number(b._ptr, Cint(n))
    ptr == C_NULL && throw(last_error())
    LocationBuilder(ptr)
end

"""
    without_number(b::LocationBuilder) -> LocationBuilder

Return a new builder with the number component removed.
"""
function without_number(b::LocationBuilder)
    ptr = ffi_location_builder_without_number(b._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_location_builder_without_number returned NULL")
    LocationBuilder(ptr)
end

"""
    with_side(b::LocationBuilder, c::Char) -> LocationBuilder

Return a new builder with the side component set. Throws [`VistaError`](@ref) if invalid.
"""
function with_side(b::LocationBuilder, c::Char)
    ptr = ffi_location_builder_with_side(b._ptr, Cchar(UInt8(c)))
    ptr == C_NULL && throw(last_error())
    LocationBuilder(ptr)
end

"""
    without_side(b::LocationBuilder) -> LocationBuilder
"""
function without_side(b::LocationBuilder)
    ptr = ffi_location_builder_without_side(b._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_location_builder_without_side returned NULL")
    LocationBuilder(ptr)
end

"""
    with_vertical(b::LocationBuilder, c::Char) -> LocationBuilder

Return a new builder with the vertical component set. Throws [`VistaError`](@ref) if invalid.
"""
function with_vertical(b::LocationBuilder, c::Char)
    ptr = ffi_location_builder_with_vertical(b._ptr, Cchar(UInt8(c)))
    ptr == C_NULL && throw(last_error())
    LocationBuilder(ptr)
end

"""
    without_vertical(b::LocationBuilder) -> LocationBuilder
"""
function without_vertical(b::LocationBuilder)
    ptr = ffi_location_builder_without_vertical(b._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_location_builder_without_vertical returned NULL")
    LocationBuilder(ptr)
end

"""
    with_transverse(b::LocationBuilder, c::Char) -> LocationBuilder

Return a new builder with the transverse component set. Throws [`VistaError`](@ref) if invalid.
"""
function with_transverse(b::LocationBuilder, c::Char)
    ptr = ffi_location_builder_with_transverse(b._ptr, Cchar(UInt8(c)))
    ptr == C_NULL && throw(last_error())
    LocationBuilder(ptr)
end

"""
    without_transverse(b::LocationBuilder) -> LocationBuilder
"""
function without_transverse(b::LocationBuilder)
    ptr = ffi_location_builder_without_transverse(b._ptr)
    ptr == C_NULL &&
        error("dnv_vista_sdk_location_builder_without_transverse returned NULL")
    LocationBuilder(ptr)
end

"""
    with_longitudinal(b::LocationBuilder, c::Char) -> LocationBuilder

Return a new builder with the longitudinal component set. Throws [`VistaError`](@ref) if invalid.
"""
function with_longitudinal(b::LocationBuilder, c::Char)
    ptr = ffi_location_builder_with_longitudinal(b._ptr, Cchar(UInt8(c)))
    ptr == C_NULL && throw(last_error())
    LocationBuilder(ptr)
end

"""
    without_longitudinal(b::LocationBuilder) -> LocationBuilder
"""
function without_longitudinal(b::LocationBuilder)
    ptr = ffi_location_builder_without_longitudinal(b._ptr)
    ptr == C_NULL &&
        error("dnv_vista_sdk_location_builder_without_longitudinal returned NULL")
    LocationBuilder(ptr)
end

"""
    with_code(b::LocationBuilder, c::Char) -> LocationBuilder

Return a new builder with `c` set, auto-detecting its group. Throws [`VistaError`](@ref) if invalid.
"""
function with_code(b::LocationBuilder, c::Char)
    ptr = ffi_location_builder_with_code(b._ptr, Cchar(UInt8(c)))
    ptr == C_NULL && throw(last_error())
    LocationBuilder(ptr)
end

"""
    with_location(b::LocationBuilder, loc::Location) -> LocationBuilder

Return a new builder with all components set from `loc`.
"""
function with_location(b::LocationBuilder, loc::Location)
    ptr = ffi_location_builder_with_location(b._ptr, loc._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_location_builder_with_location returned NULL")
    LocationBuilder(ptr)
end

"""
    without_value(b::LocationBuilder, grp::LocationGroup) -> LocationBuilder

Return a new builder with the component for `grp` removed.
"""
function without_value(b::LocationBuilder, grp::LocationGroup)
    ptr = ffi_location_builder_without_value(b._ptr, grp)
    ptr == C_NULL && error("dnv_vista_sdk_location_builder_without_value returned NULL")
    LocationBuilder(ptr)
end

"""
    build(b::LocationBuilder) -> Location

Build the final [`Location`](@ref) from the current state.
"""
function build(b::LocationBuilder)
    ptr = ffi_location_builder_build(b._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_location_builder_build returned NULL")
    Location(ptr)
end

function Base.show(io::IO, b::LocationBuilder)
    ptr = ffi_location_builder_to_string(b._ptr)
    ptr == C_NULL && return print(io, "LocationBuilder()")
    p = Ptr{UInt8}(ptr)
    s = unsafe_string(p)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), p)
    print(io, s)
end
