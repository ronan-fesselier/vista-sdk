"""
    ParsingErrors

Collection of typed error messages from parsing operations.
Owns a C resource. Freed when garbage-collected.

# Examples
```julia
errors = ParsingErrors()
has_errors(errors)        # false
length(errors)            # 0
```
"""
mutable struct ParsingErrors
    _ptr::Ptr{Cvoid}
    function ParsingErrors(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("ParsingErrors: NULL pointer")
        t = new(ptr)
        finalizer(t) do x
            ffi_parsing_errors_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    ParsingErrors() -> ParsingErrors

Create an empty [`ParsingErrors`](@ref) collection.
"""
function ParsingErrors()
    ptr = ffi_parsing_errors_create_empty()
    ptr == C_NULL && error("dnv_vista_sdk_parsing_errors_create_empty returned NULL")
    ParsingErrors(ptr)
end

"""
    length(errors::ParsingErrors) -> Int

Return the number of errors.
"""
function Base.length(errors::ParsingErrors)
    Int(ffi_parsing_errors_count(errors._ptr))
end

"""
    has_errors(errors::ParsingErrors) -> Bool

Return `true` if the collection contains at least one error.
"""
function has_errors(errors::ParsingErrors)
    ffi_parsing_errors_has_errors(errors._ptr) != 0
end

"""
    has_error_type(errors::ParsingErrors, type::AbstractString) -> Bool

Return `true` if at least one error with the given type tag is present.
"""
function has_error_type(errors::ParsingErrors, type::AbstractString)
    GC.@preserve type ffi_parsing_errors_has_error_type(
        errors._ptr,
        Base.unsafe_convert(Cstring, type),
    ) != 0
end

"""
    Base.getindex(errors::ParsingErrors, i::Integer) -> NamedTuple

Return the error entry at 1-based index `i` as `(type, message)`.
"""
function Base.getindex(errors::ParsingErrors, i::Integer)
    (i < 1 || i > length(errors)) && throw(BoundsError(errors, i))
    idx = i - 1
    type_ptr = ffi_parsing_errors_type_at(errors._ptr, idx)
    type_ptr == C_NULL && throw(BoundsError(errors, i))
    msg_ptr = ffi_parsing_errors_message_at(errors._ptr, idx)
    (
        type = unsafe_string(type_ptr),
        message = msg_ptr == C_NULL ? "" : unsafe_string(msg_ptr),
    )
end

"""
    Base.iterate(errors::ParsingErrors)

Iterate over error entries as `(type, message)` named tuples.
"""
function Base.iterate(errors::ParsingErrors, i::Int = 1)
    i > length(errors) && return nothing
    (errors[i], i + 1)
end


function Base.show(io::IO, errors::ParsingErrors)
    ptr = ffi_parsing_errors_to_string(errors._ptr)
    ptr == C_NULL && return print(io, "ParsingErrors()")
    p = Ptr{UInt8}(ptr)
    s = unsafe_string(p)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), p)
    print(io, s)
end
