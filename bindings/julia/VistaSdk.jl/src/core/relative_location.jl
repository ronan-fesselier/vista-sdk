"""
    RelativeLocation

A single relative location entry: a character code with a name and optional definition.

Borrowed from a [`Locations`](@ref) instance — valid for the lifetime of the program.
"""
struct RelativeLocation
    _ptr::Ptr{Cvoid}
end

"""
    code(rl::RelativeLocation) -> Char

Return the single-character location code.
"""
function code(rl::RelativeLocation)
    Char(UInt8(ffi_relative_location_code(rl._ptr)))
end

"""
    name(rl::RelativeLocation) -> String

Return the human-readable name.
"""
function name(rl::RelativeLocation)
    ptr = ffi_relative_location_name(rl._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_relative_location_name returned NULL")
    unsafe_string(ptr)
end

"""
    definition(rl::RelativeLocation) -> Union{String,Nothing}

Return the optional definition text, or `nothing` if not set.
"""
function definition(rl::RelativeLocation)
    ptr = ffi_relative_location_definition(rl._ptr)
    ptr == C_NULL ? nothing : unsafe_string(ptr)
end

"""
    location_value(rl::RelativeLocation) -> String

Return the canonical location string for this relative location.
"""
function location_value(rl::RelativeLocation)
    loc_ptr = ffi_relative_location_location(rl._ptr)
    loc_ptr == C_NULL && error("dnv_vista_sdk_relative_location_location returned NULL")
    val_ptr = ffi_location_value(loc_ptr)
    val_ptr == C_NULL && error("dnv_vista_sdk_location_value returned NULL")
    unsafe_string(val_ptr)
end

Base.show(io::IO, rl::RelativeLocation) = print(io, location_value(rl))
