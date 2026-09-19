"""
    GmodPathRef

Borrowed reference to a [`GmodPath`](@ref). Valid only for the lifetime of the
owning object (e.g. a `LocalId` or `MqttLocalId`). The `_owner` field keeps the
Julia owner reachable so the GC cannot run its finalizer while this ref is live.
"""
struct GmodPathRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    GmodPath

An owned VIS generic product model path. Freed when garbage-collected.
"""
mutable struct GmodPath
    _ptr::Ptr{Cvoid}
    function GmodPath(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("GmodPath: NULL pointer")
        t = new(ptr)
        finalizer(t) do x
            ffi_gmod_path_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    from_short_path(item::AbstractString, g::Gmod, locs::Locations) -> Union{GmodPath,Nothing}

Parse a short path string (e.g. `"411.1/C101.63-1"`) and return an owned path,
or `nothing` if parsing fails.
"""
function from_short_path(item::AbstractString, g::Gmod, locs::Locations)
    s = String(item)
    ptr = GC.@preserve s ffi_gmod_path_from_short_path(
        Base.unsafe_convert(Cstring, s),
        g._ptr,
        locs._ptr,
    )
    ptr == C_NULL ? nothing : GmodPath(ptr)
end

"""
    from_short_path(item::AbstractString, version::VisVersion) -> Union{GmodPath,Nothing}

Parse a short path string for the given VIS version without a pre-loaded `Gmod`.
Returns `nothing` if parsing fails.
"""
function from_short_path(item::AbstractString, version::VisVersion)
    s = String(item)
    vs = string(version)
    ptr = GC.@preserve s vs ffi_gmod_path_from_short_path_version(
        Base.unsafe_convert(Cstring, s),
        Base.unsafe_convert(Cstring, vs),
    )
    ptr == C_NULL ? nothing : GmodPath(ptr)
end

"""
    from_short_path_with_errors(item::AbstractString, g::Gmod, locs::Locations) -> Tuple{Union{GmodPath,Nothing}, ParsingErrors}

Parse a short path string and return `(path, errors)`. `path` is `nothing` on failure.
"""
function from_short_path_with_errors(item::AbstractString, g::Gmod, locs::Locations)
    s = String(item)
    out_errors = Ref{Ptr{Cvoid}}(C_NULL)
    ptr = GC.@preserve s ffi_gmod_path_from_short_path_with_errors(
        Base.unsafe_convert(Cstring, s),
        g._ptr,
        locs._ptr,
        out_errors,
    )
    path = ptr == C_NULL ? nothing : GmodPath(ptr)
    errors = out_errors[] == C_NULL ? ParsingErrors() : ParsingErrors(out_errors[])
    (path, errors)
end

"""
    from_full_path(full_path_str::AbstractString, g::Gmod, locs::Locations) -> Union{GmodPath,Nothing}

Parse a full path string and return an owned path, or `nothing` if parsing fails.
"""
function from_full_path(full_path_str::AbstractString, g::Gmod, locs::Locations)
    s = String(full_path_str)
    ptr = GC.@preserve s ffi_gmod_path_from_full_path(
        Base.unsafe_convert(Cstring, s),
        g._ptr,
        locs._ptr,
    )
    ptr == C_NULL ? nothing : GmodPath(ptr)
end

"""
    from_full_path_with_errors(full_path_str::AbstractString, g::Gmod, locs::Locations) -> Tuple{Union{GmodPath,Nothing}, ParsingErrors}

Parse a full path string and return `(path, errors)`. `path` is `nothing` on failure.
"""
function from_full_path_with_errors(full_path_str::AbstractString, g::Gmod, locs::Locations)
    s = String(full_path_str)
    out_errors = Ref{Ptr{Cvoid}}(C_NULL)
    ptr = GC.@preserve s ffi_gmod_path_from_full_path_with_errors(
        Base.unsafe_convert(Cstring, s),
        g._ptr,
        locs._ptr,
        out_errors,
    )
    path = ptr == C_NULL ? nothing : GmodPath(ptr)
    errors = out_errors[] == C_NULL ? ParsingErrors() : ParsingErrors(out_errors[])
    (path, errors)
end

const AnyGmodPath = Union{GmodPathRef,GmodPath}

function _gmod_path_ptr(p::GmodPathRef)
    p._ptr
end

function _gmod_path_ptr(p::GmodPath)
    p._ptr
end

"""
    version(p::AnyGmodPath) -> VisVersion

Return the VIS version of the path.
"""
function version(p::AnyGmodPath)
    ptr = ffi_gmod_path_version(_gmod_path_ptr(p))
    ptr == C_NULL && error("dnv_vista_sdk_gmod_path_version returned NULL")
    Base.parse(VisVersion, unsafe_string(ptr))
end

"""
    node(p::AnyGmodPath) -> GmodNodeRef

Return a borrowed reference to the leaf node of the path.
"""
function node(p::AnyGmodPath)
    ptr = ffi_gmod_path_node(_gmod_path_ptr(p))
    ptr == C_NULL && error("dnv_vista_sdk_gmod_path_node returned NULL")
    GmodNodeRef(ptr)
end

function Base.length(p::AnyGmodPath)
    Int(ffi_gmod_path_length(_gmod_path_ptr(p)))
end

function Base.getindex(p::AnyGmodPath, i::Int)
    (i < 1 || i > length(p)) && throw(BoundsError(p, i))
    ptr = ffi_gmod_path_at(_gmod_path_ptr(p), Csize_t(i - 1))
    ptr == C_NULL && throw(BoundsError(p, i))
    GmodNodeRef(ptr)
end

function Base.iterate(p::AnyGmodPath, i::Int = 1)
    i > length(p) && return nothing
    (p[i], i + 1)
end

"""
    parents(p::AnyGmodPath) -> Vector{GmodNodeRef}

Return all nodes in the path except the leaf (i.e. the ancestor chain).
"""
function parents(p::AnyGmodPath)
    n = length(p)
    n <= 1 && return GmodNodeRef[]
    [p[i] for i = 1:(n-1)]
end

"""
    is_mappable(p::AnyGmodPath) -> Bool
"""
is_mappable(p::AnyGmodPath) = ffi_gmod_path_is_mappable(_gmod_path_ptr(p)) != 0
"""
    is_individualizable(p::AnyGmodPath) -> Bool
"""
is_individualizable(p::AnyGmodPath) =
    ffi_gmod_path_is_individualizable(_gmod_path_ptr(p)) != 0

"""
    without_locations(p::AnyGmodPath) -> GmodPath

Return a copy of the path with all location codes stripped.
"""
function without_locations(p::AnyGmodPath)
    ptr = ffi_gmod_path_without_locations(_gmod_path_ptr(p))
    ptr == C_NULL && error("dnv_vista_sdk_gmod_path_without_locations returned NULL")
    GmodPath(ptr)
end

"""
    normal_assignment_name(p::AnyGmodPath, node_depth::Int) -> Union{String,Nothing}

Return the normal assignment name at `node_depth`, or `nothing` if absent.
"""
function normal_assignment_name(p::AnyGmodPath, node_depth::Int)
    ptr = ffi_gmod_path_normal_assignment_name(_gmod_path_ptr(p), Csize_t(node_depth))
    ptr == C_NULL && return nothing
    p2 = Ptr{UInt8}(ptr)
    s = unsafe_string(p2)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), p2)
    s
end

"""
    individualizable_set_count(p::AnyGmodPath) -> Int

Return the number of individualizable sets in the path.
"""
function individualizable_set_count(p::AnyGmodPath)
    Int(ffi_gmod_path_individualizable_set_count(_gmod_path_ptr(p)))
end

"""
    individualizable_set_at(p::AnyGmodPath, index::Int) -> Union{GmodIndividualizableSet,Nothing}

Return the individualizable set at 1-based `index`, or `nothing` if out of range.
"""
function individualizable_set_at(p::AnyGmodPath, index::Int)
    ptr = ffi_gmod_path_individualizable_set_at(_gmod_path_ptr(p), Csize_t(index - 1))
    ptr == C_NULL ? nothing : GmodIndividualizableSet(ptr)
end

"""
    common_name_count(p::AnyGmodPath) -> Int

Return the number of common names attached to nodes along the path.
"""
function common_name_count(p::AnyGmodPath)
    Int(ffi_gmod_path_common_name_count(_gmod_path_ptr(p)))
end

"""
    common_names(p::AnyGmodPath) -> Vector{Tuple{Int,String}}

Return all `(depth, name)` pairs for common names along the path.
"""
function common_names(p::AnyGmodPath)
    n = common_name_count(p)
    result = Tuple{Int,String}[]
    for i = 0:(n-1)
        out = Ref{Csize_t}(0)
        ok = ffi_gmod_path_common_name_depth_at(_gmod_path_ptr(p), Csize_t(i), out)
        ok == 0 && continue
        ptr = ffi_gmod_path_common_name_at(_gmod_path_ptr(p), Csize_t(i))
        ptr == C_NULL && continue
        p2 = Ptr{UInt8}(ptr)
        s = unsafe_string(p2)
        ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), p2)
        push!(result, (Int(out[]), s))
    end
    result
end

"""
    to_full_path_string(p::AnyGmodPath) -> String

Return the full path string representation (with all location annotations).
"""
function to_full_path_string(p::AnyGmodPath)
    ptr = ffi_gmod_path_to_full_path_string(_gmod_path_ptr(p))
    ptr == C_NULL && error("dnv_vista_sdk_gmod_path_to_full_path_string returned NULL")
    p2 = Ptr{UInt8}(ptr)
    s = unsafe_string(p2)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), p2)
    s
end

"""
    to_string_dump(p::AnyGmodPath) -> String

Return a debug dump string of the path structure.
"""
function to_string_dump(p::AnyGmodPath)
    ptr = ffi_gmod_path_to_string_dump(_gmod_path_ptr(p))
    ptr == C_NULL && error("dnv_vista_sdk_gmod_path_to_string_dump returned NULL")
    p2 = Ptr{UInt8}(ptr)
    s = unsafe_string(p2)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), p2)
    s
end

function Base.:(==)(a::AnyGmodPath, b::AnyGmodPath)
    ffi_gmod_path_equals(_gmod_path_ptr(a), _gmod_path_ptr(b)) != 0
end

function Base.show(io::IO, p::AnyGmodPath)
    ptr = ffi_gmod_path_to_string(_gmod_path_ptr(p))
    ptr == C_NULL && return print(io, "GmodPath()")
    p2 = Ptr{UInt8}(ptr)
    s = unsafe_string(p2)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), p2)
    print(io, s)
end

Base.string(p::AnyGmodPath) = sprint(show, p)
