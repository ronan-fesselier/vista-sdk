struct GmodPathQueryBuilderRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

mutable struct GmodPathQueryBuilder
    _ref::GmodPathQueryBuilderRef
    function GmodPathQueryBuilder(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("GmodPathQueryBuilder: NULL pointer")
        t = new(GmodPathQueryBuilderRef(ptr, nothing))
        finalizer(t) do x
            ffi_gmod_path_query_builder_free(x._ref._ptr)
            x._ref = GmodPathQueryBuilderRef(C_NULL, nothing)
        end
        t
    end
end

function _ptr(b::GmodPathQueryBuilderRef)
    b._ptr
end

function _ptr(b::GmodPathQueryBuilder)
    b._ref._ptr
end

mutable struct GmodPathQuery
    _ptr::Ptr{Cvoid}
    function GmodPathQuery(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("GmodPathQuery: NULL pointer")
        t = new(ptr)
        finalizer(t) do x
            ffi_gmod_path_query_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    create(::Type{GmodPathQueryBuilder}) -> GmodPathQueryBuilder

Create an empty Nodes-variant builder.
"""
function create(::Base.Type{GmodPathQueryBuilder})
    ptr = ffi_gmod_path_query_builder_create()
    ptr == C_NULL && error("dnv_vista_sdk_gmod_path_query_builder_create returned NULL")
    GmodPathQueryBuilder(ptr)
end

"""
    from_path(::Type{GmodPathQueryBuilder}, path) -> GmodPathQueryBuilder

Create a Path-variant builder from an existing path. Throws [`VistaError`](@ref) on failure.
"""
function from_path(::Base.Type{GmodPathQueryBuilder}, path)
    ptr = ffi_gmod_path_query_builder_from(_gmod_path_ptr(path))
    ptr == C_NULL && throw(last_error())
    GmodPathQueryBuilder(ptr)
end

"""
    path(b) -> Union{GmodPathRef, Nothing}

Return the base path this builder was created from (Path variant only).
"""
function path(b::GmodPathQueryBuilderRef)
    ptr = ffi_gmod_path_query_builder_path(b._ptr)
    ptr == C_NULL ? nothing : GmodPathRef(ptr, b)
end

function path(b::GmodPathQueryBuilder)
    path(b._ref)
end

"""
    path_with_node_all_locations(b, code, match_all_locations) -> GmodPathQueryBuilder

Return a new builder with the node selected by `code` configured to match
any location individualization (Path variant only). Throws [`VistaError`](@ref) on failure.
"""
function path_with_node_all_locations(
    b::GmodPathQueryBuilderRef,
    code::AbstractString,
    match_all_locations::Bool,
)
    s = String(code)
    ptr = GC.@preserve s ffi_gmod_path_query_builder_path_with_node_all_locations(
        b._ptr,
        Base.unsafe_convert(Cstring, s),
        Cint(match_all_locations),
    )
    ptr == C_NULL && throw(last_error())
    GmodPathQueryBuilder(ptr)
end

function path_with_node_all_locations(
    b::GmodPathQueryBuilder,
    code::AbstractString,
    match_all_locations::Bool,
)
    path_with_node_all_locations(b._ref, code, match_all_locations)
end

"""
    path_with_node_locations(b, code, locs) -> GmodPathQueryBuilder

Return a new builder with the node selected by `code` configured to match
the given locations (Path variant only). Throws [`VistaError`](@ref) on failure.
"""
function path_with_node_locations(
    b::GmodPathQueryBuilderRef,
    code::AbstractString,
    locs::AbstractVector,
)
    s = String(code)
    ptrs = [l._ptr for l in locs]
    ptr = GC.@preserve s ptrs ffi_gmod_path_query_builder_path_with_node_locations(
        b._ptr,
        Base.unsafe_convert(Cstring, s),
        isempty(ptrs) ? Ptr{Ptr{Cvoid}}(C_NULL) : pointer(ptrs),
        Csize_t(length(ptrs)),
    )
    ptr == C_NULL && throw(last_error())
    GmodPathQueryBuilder(ptr)
end

function path_with_node_locations(
    b::GmodPathQueryBuilder,
    code::AbstractString,
    locs::AbstractVector,
)
    path_with_node_locations(b._ref, code, locs)
end

"""
    with_any_node_before(b, code) -> GmodPathQueryBuilder

Return a new builder with all nodes before `code` ignored (Path variant only).
Throws [`VistaError`](@ref) if `code` is not in the path.
"""
function with_any_node_before(b::GmodPathQueryBuilderRef, code::AbstractString)
    s = String(code)
    ptr = GC.@preserve s ffi_gmod_path_query_builder_with_any_node_before(
        b._ptr,
        Base.unsafe_convert(Cstring, s),
    )
    ptr == C_NULL && throw(last_error())
    GmodPathQueryBuilder(ptr)
end

function with_any_node_before(b::GmodPathQueryBuilder, code::AbstractString)
    with_any_node_before(b._ref, code)
end

"""
    with_any_node_after(b, code) -> GmodPathQueryBuilder

Return a new builder with all nodes after `code` ignored (Path variant only).
Throws [`VistaError`](@ref) if `code` is not in the path.
"""
function with_any_node_after(b::GmodPathQueryBuilderRef, code::AbstractString)
    s = String(code)
    ptr = GC.@preserve s ffi_gmod_path_query_builder_with_any_node_after(
        b._ptr,
        Base.unsafe_convert(Cstring, s),
    )
    ptr == C_NULL && throw(last_error())
    GmodPathQueryBuilder(ptr)
end

function with_any_node_after(b::GmodPathQueryBuilder, code::AbstractString)
    with_any_node_after(b._ref, code)
end

"""
    without_locations(b::GmodPathQueryBuilderRef) -> GmodPathQueryBuilder

Return a new builder with all location individualizations ignored (Path variant only).
Throws [`VistaError`](@ref) on failure.
"""
function without_locations(b::GmodPathQueryBuilderRef)
    ptr = ffi_gmod_path_query_builder_without_locations(b._ptr)
    ptr == C_NULL && throw(last_error())
    GmodPathQueryBuilder(ptr)
end

function without_locations(b::GmodPathQueryBuilder)
    without_locations(b._ref)
end

"""
    with_node_all_locations(b, node, match_all_locations) -> GmodPathQueryBuilder

Return a new builder with `node` added, configured to match any location
individualization (Nodes variant only). Throws [`VistaError`](@ref) on failure.
"""
function with_node_all_locations(
    b::GmodPathQueryBuilderRef,
    node,
    match_all_locations::Bool,
)
    ptr = ffi_gmod_path_query_builder_with_node_all_locations(
        b._ptr,
        _ptr(node),
        Cint(match_all_locations),
    )
    ptr == C_NULL && throw(last_error())
    GmodPathQueryBuilder(ptr)
end

function with_node_all_locations(b::GmodPathQueryBuilder, node, match_all_locations::Bool)
    with_node_all_locations(b._ref, node, match_all_locations)
end

"""
    with_node_locations(b, node, locs) -> GmodPathQueryBuilder

Return a new builder with `node` added, configured to match the given locations
(Nodes variant only). Throws [`VistaError`](@ref) on failure.
"""
function with_node_locations(b::GmodPathQueryBuilderRef, node, locs::AbstractVector)
    ptrs = [l._ptr for l in locs]
    ptr = ffi_gmod_path_query_builder_with_node_locations(
        b._ptr,
        _ptr(node),
        isempty(ptrs) ? Ptr{Ptr{Cvoid}}(C_NULL) : pointer(ptrs),
        Csize_t(length(ptrs)),
    )
    ptr == C_NULL && throw(last_error())
    GmodPathQueryBuilder(ptr)
end

function with_node_locations(b::GmodPathQueryBuilder, node, locs::AbstractVector)
    with_node_locations(b._ref, node, locs)
end

"""
    build(b) -> GmodPathQuery

Construct the immutable [`GmodPathQuery`].
"""
function build(b::GmodPathQueryBuilderRef)
    ptr = ffi_gmod_path_query_builder_build(b._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_gmod_path_query_builder_build returned NULL")
    GmodPathQuery(ptr)
end

function build(b::GmodPathQueryBuilder)
    build(b._ref)
end

"""
    is_match(q::GmodPathQuery, path) -> Bool

Whether `path` satisfies all constraints in `q`.
"""
function is_match(q::GmodPathQuery, path)
    ffi_gmod_path_query_match(q._ptr, _gmod_path_ptr(path)) != 0
end
