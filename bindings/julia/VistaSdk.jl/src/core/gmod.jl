"""
    Gmod

The DNV Generic Product Model for a given VIS version. Borrowed from [`Vis`](@ref).
Do not store beyond the lifetime of the owning `Vis` instance.
"""
struct Gmod
    _ptr::Ptr{Cvoid}
end

"""
    version(g::Gmod) -> VisVersion

Return the VIS version of this product model.
"""
function version(g::Gmod)
    ptr = ffi_gmod_version(g._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_gmod_version returned NULL")
    Base.parse(VisVersion, unsafe_string(ptr))
end

"""
    root_node(g::Gmod) -> GmodNodeRef

Return a borrowed reference to the root node of the product model.
"""
function root_node(g::Gmod)
    ptr = ffi_gmod_root_node(g._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_gmod_root_node returned NULL")
    GmodNodeRef(ptr)
end

"""
    get_node(g::Gmod, c::AbstractString) -> GmodNodeRef

Return a borrowed reference to the node with code `c`. Throws if not found.
"""
function get_node(g::Gmod, c::AbstractString)
    ptr = GC.@preserve c ffi_gmod_get_node(g._ptr, Base.unsafe_convert(Cstring, c))
    ptr == C_NULL && throw(last_error())
    GmodNodeRef(ptr)
end

"""
    node_count(g::Gmod) -> Int

Return the total number of nodes in the product model.
"""
function node_count(g::Gmod)
    Int(ffi_gmod_node_count(g._ptr))
end

"""
    node_at(g::Gmod, index::Int) -> Union{GmodNodeRef,Nothing}

Return a borrowed reference to the node at 1-based `index`, or `nothing` if out of range.
"""
function node_at(g::Gmod, index::Int)
    ptr = ffi_gmod_node_at(g._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : GmodNodeRef(ptr)
end

Base.length(g::Gmod) = node_count(g)

function Base.iterate(g::Gmod, i::Int = 1)
    i > node_count(g) && return nothing
    (node_at(g, i), i + 1)
end
