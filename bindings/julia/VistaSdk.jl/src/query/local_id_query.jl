struct LocalIdQueryBuilderRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

mutable struct LocalIdQueryBuilder
    _ref::LocalIdQueryBuilderRef
    function LocalIdQueryBuilder(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("LocalIdQueryBuilder: NULL pointer")
        t = new(LocalIdQueryBuilderRef(ptr, nothing))
        finalizer(t) do x
            ffi_local_id_query_builder_free(x._ref._ptr)
            x._ref = LocalIdQueryBuilderRef(C_NULL, nothing)
        end
        t
    end
end

function _ptr(b::LocalIdQueryBuilderRef)
    b._ptr
end

function _ptr(b::LocalIdQueryBuilder)
    b._ref._ptr
end

mutable struct LocalIdQuery
    _ptr::Ptr{Cvoid}
    function LocalIdQuery(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("LocalIdQuery: NULL pointer")
        t = new(ptr)
        finalizer(t) do x
            ffi_local_id_query_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    create(::Type{LocalIdQueryBuilder}) -> LocalIdQueryBuilder

Create an empty builder. The built query matches all LocalIds.
"""
function create(::Base.Type{LocalIdQueryBuilder})
    ptr = ffi_local_id_query_builder_create()
    ptr == C_NULL && error("dnv_vista_sdk_local_id_query_builder_create returned NULL")
    LocalIdQueryBuilder(ptr)
end

"""
    from_local_id(::Type{LocalIdQueryBuilder}, lid) -> LocalIdQueryBuilder

Create a builder configured to match `lid` exactly. Throws [`VistaError`](@ref) on failure.
"""
function from_local_id(::Base.Type{LocalIdQueryBuilder}, lid)
    ptr = ffi_local_id_query_builder_from(_ptr(lid))
    ptr == C_NULL && throw(last_error())
    LocalIdQueryBuilder(ptr)
end

"""
    from_string(::Type{LocalIdQueryBuilder}, s) -> LocalIdQueryBuilder

Create a builder configured to match the given LocalId string exactly.
Throws [`VistaError`](@ref) on failure.
"""
function from_string(::Base.Type{LocalIdQueryBuilder}, s::AbstractString)
    cs = String(s)
    ptr = GC.@preserve cs ffi_local_id_query_builder_from_string(
        Base.unsafe_convert(Cstring, cs),
    )
    ptr == C_NULL && throw(last_error())
    LocalIdQueryBuilder(ptr)
end

"""
    with_primary_item(b, path) -> LocalIdQueryBuilder

Return a new builder with the primary item matched exactly. Throws [`VistaError`](@ref) on failure.
"""
function with_primary_item(b::LocalIdQueryBuilderRef, path)
    ptr = ffi_local_id_query_builder_with_primary_item(b._ptr, _gmod_path_ptr(path))
    ptr == C_NULL && throw(last_error())
    LocalIdQueryBuilder(ptr)
end

function with_primary_item(b::LocalIdQueryBuilder, path)
    with_primary_item(b._ref, path)
end

"""
    with_primary_item_query(b, q::GmodPathQuery) -> LocalIdQueryBuilder

Return a new builder with the primary item matched by `q`. Throws [`VistaError`](@ref) on failure.
"""
function with_primary_item_query(b::LocalIdQueryBuilderRef, q::GmodPathQuery)
    ptr = ffi_local_id_query_builder_with_primary_item_query(b._ptr, q._ptr)
    ptr == C_NULL && throw(last_error())
    LocalIdQueryBuilder(ptr)
end

function with_primary_item_query(b::LocalIdQueryBuilder, q::GmodPathQuery)
    with_primary_item_query(b._ref, q)
end

"""
    with_secondary_item(b, path) -> LocalIdQueryBuilder

Return a new builder with the secondary item matched exactly. Throws [`VistaError`](@ref) on failure.
"""
function with_secondary_item(b::LocalIdQueryBuilderRef, path)
    ptr = ffi_local_id_query_builder_with_secondary_item(b._ptr, _gmod_path_ptr(path))
    ptr == C_NULL && throw(last_error())
    LocalIdQueryBuilder(ptr)
end

function with_secondary_item(b::LocalIdQueryBuilder, path)
    with_secondary_item(b._ref, path)
end

"""
    with_secondary_item_query(b, q::GmodPathQuery) -> LocalIdQueryBuilder

Return a new builder with the secondary item matched by `q`. Throws [`VistaError`](@ref) on failure.
"""
function with_secondary_item_query(b::LocalIdQueryBuilderRef, q::GmodPathQuery)
    ptr = ffi_local_id_query_builder_with_secondary_item_query(b._ptr, q._ptr)
    ptr == C_NULL && throw(last_error())
    LocalIdQueryBuilder(ptr)
end

function with_secondary_item_query(b::LocalIdQueryBuilder, q::GmodPathQuery)
    with_secondary_item_query(b._ref, q)
end

"""
    with_primary_item_nodes_builder(b, nb) -> LocalIdQueryBuilder

Return a new builder with the primary item matched by a Nodes-variant query builder.
Throws [`VistaError`](@ref) if `nb` is a Path-variant builder.
"""
function with_primary_item_nodes_builder(b::LocalIdQueryBuilderRef, nb)
    ptr = ffi_local_id_query_builder_with_primary_item_nodes_builder(b._ptr, _ptr(nb))
    ptr == C_NULL && throw(last_error())
    LocalIdQueryBuilder(ptr)
end

function with_primary_item_nodes_builder(b::LocalIdQueryBuilder, nb)
    with_primary_item_nodes_builder(b._ref, nb)
end

"""
    with_primary_item_path_builder(b, pb) -> LocalIdQueryBuilder

Return a new builder with the primary item matched by a Path-variant query builder.
Throws [`VistaError`](@ref) if `pb` is a Nodes-variant builder.
"""
function with_primary_item_path_builder(b::LocalIdQueryBuilderRef, pb)
    ptr = ffi_local_id_query_builder_with_primary_item_path_builder(b._ptr, _ptr(pb))
    ptr == C_NULL && throw(last_error())
    LocalIdQueryBuilder(ptr)
end

function with_primary_item_path_builder(b::LocalIdQueryBuilder, pb)
    with_primary_item_path_builder(b._ref, pb)
end

"""
    with_any_secondary_item(b) -> LocalIdQueryBuilder

Return a new builder matching any LocalId regardless of secondary item presence.
Throws [`VistaError`](@ref) on failure.
"""
function with_any_secondary_item(b::LocalIdQueryBuilderRef)
    ptr = ffi_local_id_query_builder_with_any_secondary_item(b._ptr)
    ptr == C_NULL && throw(last_error())
    LocalIdQueryBuilder(ptr)
end

function with_any_secondary_item(b::LocalIdQueryBuilder)
    with_any_secondary_item(b._ref)
end

"""
    without_secondary_item(b) -> LocalIdQueryBuilder

Return a new builder matching only LocalIds without a secondary item.
Throws [`VistaError`](@ref) on failure.
"""
function without_secondary_item(b::LocalIdQueryBuilderRef)
    ptr = ffi_local_id_query_builder_without_secondary_item(b._ptr)
    ptr == C_NULL && throw(last_error())
    LocalIdQueryBuilder(ptr)
end

function without_secondary_item(b::LocalIdQueryBuilder)
    without_secondary_item(b._ref)
end

"""
    with_secondary_item_nodes_builder(b, nb) -> LocalIdQueryBuilder

Return a new builder with the secondary item matched by a Nodes-variant query builder.
Throws [`VistaError`](@ref) if `nb` is a Path-variant builder.
"""
function with_secondary_item_nodes_builder(b::LocalIdQueryBuilderRef, nb)
    ptr = ffi_local_id_query_builder_with_secondary_item_nodes_builder(b._ptr, _ptr(nb))
    ptr == C_NULL && throw(last_error())
    LocalIdQueryBuilder(ptr)
end

function with_secondary_item_nodes_builder(b::LocalIdQueryBuilder, nb)
    with_secondary_item_nodes_builder(b._ref, nb)
end

"""
    with_secondary_item_path_builder(b, pb) -> LocalIdQueryBuilder

Return a new builder with the secondary item matched by a Path-variant query builder.
Throws [`VistaError`](@ref) if `pb` is a Nodes-variant builder.
"""
function with_secondary_item_path_builder(b::LocalIdQueryBuilderRef, pb)
    ptr = ffi_local_id_query_builder_with_secondary_item_path_builder(b._ptr, _ptr(pb))
    ptr == C_NULL && throw(last_error())
    LocalIdQueryBuilder(ptr)
end

function with_secondary_item_path_builder(b::LocalIdQueryBuilder, pb)
    with_secondary_item_path_builder(b._ref, pb)
end

"""
    with_tags(b, q::MetadataTagsQuery) -> LocalIdQueryBuilder

Return a new builder with the given metadata tags query. Throws [`VistaError`](@ref) on failure.
"""
function with_tags(b::LocalIdQueryBuilderRef, q::MetadataTagsQuery)
    ptr = ffi_local_id_query_builder_with_tags(b._ptr, q._ptr)
    ptr == C_NULL && throw(last_error())
    LocalIdQueryBuilder(ptr)
end

function with_tags(b::LocalIdQueryBuilder, q::MetadataTagsQuery)
    with_tags(b._ref, q)
end

"""
    without_locations(b::LocalIdQueryBuilderRef) -> LocalIdQueryBuilder

Return a new builder with all location requirements removed from primary and secondary items.
Throws [`VistaError`](@ref) on failure.
"""
function without_locations(b::LocalIdQueryBuilderRef)
    ptr = ffi_local_id_query_builder_without_locations(b._ptr)
    ptr == C_NULL && throw(last_error())
    LocalIdQueryBuilder(ptr)
end

function without_locations(b::LocalIdQueryBuilder)
    without_locations(b._ref)
end

"""
    primary_item(b) -> Union{GmodPathRef, Nothing}

Return the primary item path if configured as an exact path, otherwise `nothing`.
"""
function primary_item(b::LocalIdQueryBuilderRef)
    ptr = ffi_local_id_query_builder_primary_item(b._ptr)
    ptr == C_NULL ? nothing : GmodPathRef(ptr, b)
end

function primary_item(b::LocalIdQueryBuilder)
    primary_item(b._ref)
end

"""
    secondary_item(b) -> Union{GmodPathRef, Nothing}

Return the secondary item path if configured as an exact path, otherwise `nothing`.
"""
function secondary_item(b::LocalIdQueryBuilderRef)
    ptr = ffi_local_id_query_builder_secondary_item(b._ptr)
    ptr == C_NULL ? nothing : GmodPathRef(ptr, b)
end

function secondary_item(b::LocalIdQueryBuilder)
    secondary_item(b._ref)
end

"""
    tags_builder(b) -> Union{MetadataTagsQueryBuilderRef, Nothing}

Return the current tags query builder, or `nothing` if no tags query is configured.
"""
function tags_builder(b::LocalIdQueryBuilderRef)
    ptr = ffi_local_id_query_builder_tags_builder(b._ptr)
    ptr == C_NULL ? nothing : MetadataTagsQueryBuilderRef(ptr, b)
end

function tags_builder(b::LocalIdQueryBuilder)
    tags_builder(b._ref)
end

"""
    build(b) -> LocalIdQuery

Construct the immutable [`LocalIdQuery`].
"""
function build(b::LocalIdQueryBuilderRef)
    ptr = ffi_local_id_query_builder_build(b._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_local_id_query_builder_build returned NULL")
    LocalIdQuery(ptr)
end

function build(b::LocalIdQueryBuilder)
    build(b._ref)
end

"""
    is_match(q::LocalIdQuery, lid) -> Bool

Whether `lid` satisfies all constraints in `q`.
"""
function is_match(q::LocalIdQuery, lid)
    ffi_local_id_query_match(q._ptr, _ptr(lid)) != 0
end

"""
    is_match_str(q::LocalIdQuery, s::AbstractString) -> Bool

Whether the LocalId string `s` satisfies all constraints in `q`.
"""
function is_match_str(q::LocalIdQuery, s::AbstractString)
    cs = String(s)
    GC.@preserve cs ffi_local_id_query_match_string(
        q._ptr,
        Base.unsafe_convert(Cstring, cs),
    ) != 0
end
