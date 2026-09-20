"""
    UniversalIdRef

Borrowed reference to a [`UniversalId`](@ref). Valid only for the lifetime of
the owning object. The `_owner` field keeps the Julia owner reachable so the GC
cannot run its finalizer while this ref is live.
"""
struct UniversalIdRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    UniversalId

An immutable, validated ISO 19848 universal ID combining a ship identifier and a
[`LocalId`](@ref). Freed when garbage-collected.
"""
mutable struct UniversalId
    _ptr::Ptr{Cvoid}
    function UniversalId(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("UniversalId: NULL pointer")
        t = new(ptr)
        finalizer(t) do x
            ffi_universal_id_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    naming_entity(::Type{UniversalId}) -> String

ISO 19848 Annex C naming entity (`"data.dnv.com"`).
"""
function naming_entity(::Base.Type{UniversalId})
    ptr = ffi_universal_id_naming_entity()
    ptr == C_NULL && error("dnv_vista_sdk_universal_id_naming_entity returned NULL")
    unsafe_string(ptr)
end

function imo_number(id::UniversalId)
    b = builder(id)
    imo_number(b)
end

function local_id(id::UniversalId)
    ptr = ffi_universal_id_local_id(id._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_universal_id_local_id returned NULL")
    LocalIdRef(ptr, id)
end

function builder(id::UniversalId)
    ptr = ffi_universal_id_builder(id._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_universal_id_builder returned NULL")
    UniversalIdBuilderRef(ptr, id)
end

function Base.:(==)(a::UniversalId, b::UniversalId)
    ffi_universal_id_equals(a._ptr, b._ptr) != 0
end

function Base.show(io::IO, id::UniversalId)
    ptr = ffi_universal_id_to_string(id._ptr)
    ptr == C_NULL && return print(io, "<UniversalId NULL>")
    s = unsafe_string(ptr)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), ptr)
    print(io, s)
end

Base.string(id::UniversalId) = sprint(show, id)

"""
    from_string(::Type{UniversalId}, s) -> Union{UniversalId, Nothing}

Parse a UniversalId string. Returns `nothing` on invalid input.
"""
function from_string(::Base.Type{UniversalId}, s::AbstractString)
    str = String(s)
    ptr = GC.@preserve str ffi_universal_id_from_string(Base.unsafe_convert(Cstring, str))
    ptr == C_NULL ? nothing : UniversalId(ptr)
end
