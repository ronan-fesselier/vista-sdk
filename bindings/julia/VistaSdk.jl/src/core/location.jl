"""
    Location

A validated, canonical VIS location string (e.g. `"11FIPU"`).

Owned — freed automatically when garbage collected.
"""
mutable struct Location
    _ptr::Ptr{Cvoid}
    function Location(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("Location: NULL pointer")
        t = new(ptr)
        finalizer(t) do x
            ffi_location_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    value(loc::Location) -> String

Return the canonical location string.
"""
function value(loc::Location)
    ptr = ffi_location_value(loc._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_location_value returned NULL")
    unsafe_string(ptr)
end

Base.show(io::IO, loc::Location) = print(io, value(loc))
Base.string(loc::Location) = value(loc)
