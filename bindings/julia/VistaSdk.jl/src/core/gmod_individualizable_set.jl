"""
    GmodIndividualizableSet

A set of [`GmodNode`](@ref) indices within a [`GmodPath`](@ref) that can be
individualized by assigning a location. Owns a C resource.
"""
mutable struct GmodIndividualizableSet
    _ptr::Ptr{Cvoid}
    function GmodIndividualizableSet(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("GmodIndividualizableSet: NULL pointer")
        t = new(ptr)
        finalizer(t) do x
            ffi_gmod_individualizable_set_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    GmodIndividualizableSet(node_indices::Vector{Int32}, source_path::GmodPath) -> GmodIndividualizableSet

Construct a set from a list of node indices and the source path. Throws on error.
"""
function GmodIndividualizableSet(node_indices::Vector{Int32}, source_path::GmodPath)
    indices = Cint.(node_indices)
    ptr = GC.@preserve indices ffi_gmod_individualizable_set_create(
        pointer(indices),
        Csize_t(length(indices)),
        source_path._ptr,
    )
    ptr == C_NULL && throw(last_error())
    GmodIndividualizableSet(ptr)
end

"""
    node_count(s::GmodIndividualizableSet) -> Int

Return the number of nodes in the set.
"""
function node_count(s::GmodIndividualizableSet)
    Int(ffi_gmod_individualizable_set_node_count(s._ptr))
end

"""
    node_at(s::GmodIndividualizableSet, index::Int) -> GmodNode

Return an owned copy of the node at 1-based `index`. Throws on error.
"""
function node_at(s::GmodIndividualizableSet, index::Int)
    ptr = ffi_gmod_individualizable_set_node_at(s._ptr, Csize_t(index - 1))
    ptr == C_NULL && throw(last_error())
    GmodNode(ptr)
end

"""
    index_count(s::GmodIndividualizableSet) -> Int

Return the number of path indices in the set.
"""
function index_count(s::GmodIndividualizableSet)
    Int(ffi_gmod_individualizable_set_index_count(s._ptr))
end

"""
    index_at(s::GmodIndividualizableSet, position::Int) -> Union{Int,Nothing}

Return the path index at 1-based `position`, or `nothing` if out of range.
"""
function index_at(s::GmodIndividualizableSet, position::Int)
    out = Ref{Cint}(0)
    ok = ffi_gmod_individualizable_set_index_at(s._ptr, Csize_t(position - 1), out)
    ok != 0 ? Int(out[]) : nothing
end

"""
    set_location(s::GmodIndividualizableSet) -> Union{Location,Nothing}

Return the assigned location, or `nothing` if none has been set.
"""
function set_location(s::GmodIndividualizableSet)
    ptr = ffi_gmod_individualizable_set_location(s._ptr)
    ptr == C_NULL ? nothing : Location(ptr)
end

"""
    build(s::GmodIndividualizableSet) -> GmodPath

Build and return the individualized path. Throws on error.
"""
function build(s::GmodIndividualizableSet)
    ptr = ffi_gmod_individualizable_set_build(s._ptr)
    ptr == C_NULL && throw(last_error())
    GmodPath(ptr)
end

function Base.show(io::IO, s::GmodIndividualizableSet)
    ptr = ffi_gmod_individualizable_set_to_string(s._ptr)
    ptr == C_NULL && return print(io, "GmodIndividualizableSet()")
    p = Ptr{UInt8}(ptr)
    str = unsafe_string(p)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), p)
    print(io, str)
end
