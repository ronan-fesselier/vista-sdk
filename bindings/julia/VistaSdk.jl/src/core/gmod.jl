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

"""
    TraversalHandlerResult

Return value for the callback passed to [`traverse`](@ref).

- `TraversalStop` - stop the traversal immediately.
- `TraversalSkipSubtree` - skip all children of the current node.
- `TraversalContinue` - continue normally.
"""
@enum TraversalHandlerResult::Int32 begin
    TraversalStop = 0
    TraversalSkipSubtree = 1
    TraversalContinue = 2
end

"""
    traverse(g::Gmod, handler::Function, max_occurrence::Int = 1) -> Bool

Traverse every node in the product model depth-first. `handler` receives
`(parents::Vector{GmodNodeRef}, node::GmodNodeRef)` and must return a
[`TraversalHandlerResult`](@ref). `max_occurrence` caps how many times a node
is visited when it appears in multiple paths. Returns `true` on success.
"""
function traverse(g::Gmod, handler::Function, max_occurrence::Int = 1)
    cb = @cfunction(
        (parents_ptr, parent_count, node_ptr, userdata) -> begin
            f = unsafe_pointer_to_objref(userdata)[]::Function
            parents = [GmodNodeRef(unsafe_load(parents_ptr, i)) for i = 1:parent_count]
            result = f(parents, GmodNodeRef(node_ptr))::TraversalHandlerResult
            Cint(Integer(result))
        end,
        Cint,
        (Ptr{Ptr{Cvoid}}, Csize_t, Ptr{Cvoid}, Ptr{Cvoid})
    )
    box = Ref{Function}(handler)
    GC.@preserve box begin
        ffi_gmod_traverse(g._ptr, cb, Cint(max_occurrence), pointer_from_objref(box)) != 0
    end
end
