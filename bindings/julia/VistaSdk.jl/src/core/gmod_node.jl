"""
    GmodNodeRef

Borrowed reference to a node inside a [`Gmod`](@ref). Valid for the lifetime of the
owning `Gmod`. Do not store beyond the scope where the `Gmod` is alive.
"""
struct GmodNodeRef
    _ptr::Ptr{Cvoid}
end

"""
    GmodNode

Owned copy of a VIS generic product model node. Freed when garbage-collected.
"""
mutable struct GmodNode
    _ref::GmodNodeRef
    function GmodNode(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("GmodNode: NULL pointer")
        t = new(GmodNodeRef(ptr))
        finalizer(t) do x
            ffi_gmod_node_free(x._ref._ptr)
            x._ref = GmodNodeRef(C_NULL)
        end
        t
    end
end

function _ptr(n::GmodNodeRef)
    n._ptr
end

function _ptr(n::GmodNode)
    n._ref._ptr
end

"""
    version(n::Union{GmodNodeRef,GmodNode}) -> VisVersion

Return the VIS version this node belongs to.
"""
function version(n::GmodNodeRef)
    ptr = ffi_gmod_node_version(n._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_gmod_node_version returned NULL")
    Base.parse(VisVersion, unsafe_string(ptr))
end

function version(n::GmodNode)
    version(n._ref)
end

"""
    code(n::Union{GmodNodeRef,GmodNode}) -> String

Return the node code string (e.g. `"411.1"`).
"""
function code(n::GmodNodeRef)
    ptr = ffi_gmod_node_code(n._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_gmod_node_code returned NULL")
    unsafe_string(ptr)
end

function code(n::GmodNode)
    code(n._ref)
end

"""
    location(n::Union{GmodNodeRef,GmodNode}) -> Union{String,Nothing}

Return the location value string, or `nothing` if the node has no location.
"""
function location(n::GmodNodeRef)
    loc_ptr = ffi_gmod_node_location(n._ptr)
    loc_ptr == C_NULL && return nothing
    val_ptr = ffi_location_value(loc_ptr)
    val_ptr == C_NULL && error("dnv_vista_sdk_location_value returned NULL")
    unsafe_string(val_ptr)
end

function location(n::GmodNode)
    location(n._ref)
end

"""
    metadata(n::Union{GmodNodeRef,GmodNode}) -> GmodNodeMetadata

Return a borrowed view of the node metadata.
"""
function metadata(n::GmodNodeRef)
    ptr = ffi_gmod_node_metadata(n._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_gmod_node_metadata returned NULL")
    GmodNodeMetadata(ptr)
end

function metadata(n::GmodNode)
    metadata(n._ref)
end

"""
    child_count(n::Union{GmodNodeRef,GmodNode}) -> Int

Return the number of direct children.
"""
function child_count(n::GmodNodeRef)
    Int(ffi_gmod_node_child_count(n._ptr))
end

function child_count(n::GmodNode)
    child_count(n._ref)
end

"""
    child_at(n::Union{GmodNodeRef,GmodNode}, index::Int) -> Union{GmodNodeRef,Nothing}

Return the child at 1-based `index`, or `nothing` if out of range.
"""
function child_at(n::GmodNodeRef, index::Int)
    ptr = ffi_gmod_node_child_at(n._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : GmodNodeRef(ptr)
end

function child_at(n::GmodNode, index::Int)
    child_at(n._ref, index)
end

"""
    parent_count(n::Union{GmodNodeRef,GmodNode}) -> Int

Return the number of direct parents.
"""
function parent_count(n::GmodNodeRef)
    Int(ffi_gmod_node_parent_count(n._ptr))
end

function parent_count(n::GmodNode)
    parent_count(n._ref)
end

"""
    parent_at(n::Union{GmodNodeRef,GmodNode}, index::Int) -> Union{GmodNodeRef,Nothing}

Return the parent at 1-based `index`, or `nothing` if out of range.
"""
function parent_at(n::GmodNodeRef, index::Int)
    ptr = ffi_gmod_node_parent_at(n._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : GmodNodeRef(ptr)
end

function parent_at(n::GmodNode, index::Int)
    parent_at(n._ref, index)
end

"""
    children(n::Union{GmodNodeRef,GmodNode}) -> Vector{GmodNodeRef}

Return all direct children.
"""
function children(n::GmodNodeRef)
    [child_at(n, i) for i = 1:child_count(n)]
end

function children(n::GmodNode)
    GC.@preserve n children(n._ref)
end

"""
    parents(n::Union{GmodNodeRef,GmodNode}) -> Vector{GmodNodeRef}

Return all direct parents.
"""
function parents(n::GmodNodeRef)
    [parent_at(n, i) for i = 1:parent_count(n)]
end

function parents(n::GmodNode)
    GC.@preserve n parents(n._ref)
end

"""
    product_type(n::Union{GmodNodeRef,GmodNode}) -> Union{GmodNodeRef,Nothing}

Return the product type node, or `nothing` if absent.
"""
function product_type(n::GmodNodeRef)
    ptr = ffi_gmod_node_product_type(n._ptr)
    ptr == C_NULL ? nothing : GmodNodeRef(ptr)
end

function product_type(n::GmodNode)
    product_type(n._ref)
end

"""
    product_selection(n::Union{GmodNodeRef,GmodNode}) -> Union{GmodNodeRef,Nothing}

Return the product selection node, or `nothing` if absent.
"""
function product_selection(n::GmodNodeRef)
    ptr = ffi_gmod_node_product_selection(n._ptr)
    ptr == C_NULL ? nothing : GmodNodeRef(ptr)
end

function product_selection(n::GmodNode)
    product_selection(n._ref)
end

"""
    is_function_composition(n::Union{GmodNodeRef,GmodNode}) -> Bool
"""
is_function_composition(n::GmodNodeRef) = ffi_gmod_node_is_function_composition(n._ptr) != 0
is_function_composition(n::GmodNode) = is_function_composition(n._ref)

"""
    is_mappable(n::Union{GmodNodeRef,GmodNode}) -> Bool
"""
is_mappable(n::GmodNodeRef) = ffi_gmod_node_is_mappable(n._ptr) != 0
is_mappable(n::GmodNode) = is_mappable(n._ref)

"""
    is_product_selection(n::Union{GmodNodeRef,GmodNode}) -> Bool
"""
is_product_selection(n::GmodNodeRef) = ffi_gmod_node_is_product_selection(n._ptr) != 0
is_product_selection(n::GmodNode) = is_product_selection(n._ref)

"""
    is_product_type(n::Union{GmodNodeRef,GmodNode}) -> Bool
"""
is_product_type(n::GmodNodeRef) = ffi_gmod_node_is_product_type(n._ptr) != 0
is_product_type(n::GmodNode) = is_product_type(n._ref)

"""
    is_asset(n::Union{GmodNodeRef,GmodNode}) -> Bool
"""
is_asset(n::GmodNodeRef) = ffi_gmod_node_is_asset(n._ptr) != 0
is_asset(n::GmodNode) = is_asset(n._ref)

"""
    is_leaf_node(n::Union{GmodNodeRef,GmodNode}) -> Bool
"""
is_leaf_node(n::GmodNodeRef) = ffi_gmod_node_is_leaf_node(n._ptr) != 0
is_leaf_node(n::GmodNode) = is_leaf_node(n._ref)

"""
    is_function_node(n::Union{GmodNodeRef,GmodNode}) -> Bool
"""
is_function_node(n::GmodNodeRef) = ffi_gmod_node_is_function_node(n._ptr) != 0
is_function_node(n::GmodNode) = is_function_node(n._ref)

"""
    is_asset_function_node(n::Union{GmodNodeRef,GmodNode}) -> Bool
"""
is_asset_function_node(n::GmodNodeRef) = ffi_gmod_node_is_asset_function_node(n._ptr) != 0
is_asset_function_node(n::GmodNode) = is_asset_function_node(n._ref)

"""
    is_root(n::Union{GmodNodeRef,GmodNode}) -> Bool
"""
is_root(n::GmodNodeRef) = ffi_gmod_node_is_root(n._ptr) != 0
is_root(n::GmodNode) = is_root(n._ref)

"""
    is_child(n, other) -> Bool

Return `true` if `n` is a direct child of `other`.
"""
function is_child(n::GmodNodeRef, other::GmodNodeRef)
    ffi_gmod_node_is_child(n._ptr, other._ptr) != 0
end

function is_child(n::GmodNode, other::GmodNodeRef)
    is_child(n._ref, other)
end

function is_child(n::GmodNodeRef, other::GmodNode)
    is_child(n, other._ref)
end

function is_child(n::GmodNode, other::GmodNode)
    is_child(n._ref, other._ref)
end

"""
    is_child_code(n::Union{GmodNodeRef,GmodNode}, c::AbstractString) -> Bool

Return `true` if any direct child of `n` has code `c`.
"""
function is_child_code(n::GmodNodeRef, c::AbstractString)
    GC.@preserve c ffi_gmod_node_is_child_code(n._ptr, Base.unsafe_convert(Cstring, c)) != 0
end

function is_child_code(n::GmodNode, c::AbstractString)
    is_child_code(n._ref, c)
end

function Base.show(io::IO, n::GmodNodeRef)
    ptr = ffi_gmod_node_to_string(n._ptr)
    ptr == C_NULL && return print(io, "GmodNode()")
    p = Ptr{UInt8}(ptr)
    s = unsafe_string(p)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), p)
    print(io, s)
end

function Base.show(io::IO, n::GmodNode)
    show(io, n._ref)
end
