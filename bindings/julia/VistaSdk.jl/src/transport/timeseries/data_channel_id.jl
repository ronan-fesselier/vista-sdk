"""
    TsdChannelIdRef

Borrowed reference to a [`TsdChannelId`](@ref). Valid only for the lifetime of
the owning object. The `_owner` field keeps the Julia owner reachable so the GC
cannot run its finalizer while this ref is live.
"""
struct TsdChannelIdRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    TsdChannelId

ISO 19848 time series data channel identifier: either a validated VIS
[`LocalId`](@ref) or a plain short identifier string. Freed when garbage-collected.

Use [`from_string`](@ref) to construct.
"""
mutable struct TsdChannelId
    _ptr::Ptr{Cvoid}
    function TsdChannelId(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "TsdChannelId: NULL pointer"))
        t = new(ptr)
        finalizer(t) do x
            ffi_tsd_channel_id_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

function _ptr(id::TsdChannelId)
    id._ptr
end

function _ptr(id::TsdChannelIdRef)
    id._ptr
end

"""
    from_string(::Type{TsdChannelId}, s::AbstractString) -> Union{TsdChannelId,Nothing}

Parse a `TsdChannelId` from its string representation.
Tries to parse `s` as a [`LocalId`](@ref). On failure, stores `s` verbatim as a short
identifier. Returns `nothing` if `s` is empty.
"""
function from_string(::Base.Type{TsdChannelId}, s::AbstractString)
    isempty(s) && return nothing
    str = String(s)
    ptr = GC.@preserve str ffi_tsd_channel_id_from_string(Base.unsafe_convert(Cstring, str))
    ptr == C_NULL ? nothing : TsdChannelId(ptr)
end

"""
    is_local_id(id::TsdChannelId) -> Bool

Return `true` if `id` holds a validated VIS [`LocalId`](@ref).
"""
function is_local_id(id::TsdChannelId)
    ffi_tsd_channel_id_is_local_id(id._ptr) != 0
end

function is_local_id(id::TsdChannelIdRef)
    ffi_tsd_channel_id_is_local_id(id._ptr) != 0
end

"""
    is_short_id(id::TsdChannelId) -> Bool

Return `true` if `id` holds a plain short identifier string.
"""
function is_short_id(id::TsdChannelId)
    ffi_tsd_channel_id_is_short_id(id._ptr) != 0
end

function is_short_id(id::TsdChannelIdRef)
    ffi_tsd_channel_id_is_short_id(id._ptr) != 0
end

"""
    local_id(id::TsdChannelId) -> Union{LocalIdRef,Nothing}

Return the [`LocalId`](@ref) view if `id` holds one, or `nothing` if it holds a short id.
"""
function local_id(id::TsdChannelId)
    ptr = ffi_tsd_channel_id_local_id(id._ptr)
    ptr == C_NULL ? nothing : LocalIdRef(ptr, id)
end

function local_id(id::TsdChannelIdRef)
    ptr = ffi_tsd_channel_id_local_id(id._ptr)
    ptr == C_NULL ? nothing : LocalIdRef(ptr, id._owner)
end

"""
    short_id(id::TsdChannelId) -> Union{String,Nothing}

Return the short identifier string if `id` holds one, or `nothing` if it holds a
[`LocalId`](@ref).
"""
function short_id(id::TsdChannelId)
    cs = ffi_tsd_channel_id_short_id(id._ptr)
    cs == C_NULL ? nothing : unsafe_string(cs)
end

function short_id(id::TsdChannelIdRef)
    cs = ffi_tsd_channel_id_short_id(id._ptr)
    cs == C_NULL ? nothing : unsafe_string(cs)
end

function Base.:(==)(a::TsdChannelId, b::TsdChannelId)
    ffi_tsd_channel_id_equals(a._ptr, b._ptr) != 0
end

function Base.:(==)(a::TsdChannelIdRef, b::TsdChannelIdRef)
    ffi_tsd_channel_id_equals(a._ptr, b._ptr) != 0
end

function Base.show(io::IO, id::TsdChannelId)
    ptr = ffi_tsd_channel_id_to_string(id._ptr)
    ptr == C_NULL && (print(io, "<TsdChannelId NULL>"); return)
    s = unsafe_string(ptr)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), ptr)
    print(io, s)
end

function Base.show(io::IO, id::TsdChannelIdRef)
    ptr = ffi_tsd_channel_id_to_string(id._ptr)
    ptr == C_NULL && (print(io, "<TsdChannelId NULL>"); return)
    s = unsafe_string(ptr)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), ptr)
    print(io, s)
end

Base.string(id::TsdChannelId) = sprint(show, id)
Base.string(id::TsdChannelIdRef) = sprint(show, id)
