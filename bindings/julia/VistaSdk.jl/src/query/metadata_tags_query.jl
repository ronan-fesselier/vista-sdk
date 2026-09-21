struct MetadataTagsQueryBuilderRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

mutable struct MetadataTagsQueryBuilder
    _ref::MetadataTagsQueryBuilderRef
    function MetadataTagsQueryBuilder(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("MetadataTagsQueryBuilder: NULL pointer")
        t = new(MetadataTagsQueryBuilderRef(ptr, nothing))
        finalizer(t) do x
            ffi_metadata_tags_query_builder_free(x._ref._ptr)
            x._ref = MetadataTagsQueryBuilderRef(C_NULL, nothing)
        end
        t
    end
end

function _ptr(b::MetadataTagsQueryBuilderRef)
    b._ptr
end

function _ptr(b::MetadataTagsQueryBuilder)
    b._ref._ptr
end

mutable struct MetadataTagsQuery
    _ptr::Ptr{Cvoid}
    function MetadataTagsQuery(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("MetadataTagsQuery: NULL pointer")
        t = new(ptr)
        finalizer(t) do x
            ffi_metadata_tags_query_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    create(::Type{MetadataTagsQueryBuilder}) -> MetadataTagsQueryBuilder

Create an empty builder. The built query matches any LocalId.
"""
function create(::Base.Type{MetadataTagsQueryBuilder})
    ptr = ffi_metadata_tags_query_builder_create()
    ptr == C_NULL && error("dnv_vista_sdk_metadata_tags_query_builder_create returned NULL")
    MetadataTagsQueryBuilder(ptr)
end

"""
    from_local_id(::Type{MetadataTagsQueryBuilder}, lid, allow_other_tags::Bool) -> MetadataTagsQueryBuilder

Create a builder pre-populated with all metadata tags from `lid`.
"""
function from_local_id(::Base.Type{MetadataTagsQueryBuilder}, lid, allow_other_tags::Bool)
    ptr = ffi_metadata_tags_query_builder_from(_ptr(lid), Cint(allow_other_tags))
    ptr == C_NULL && error("dnv_vista_sdk_metadata_tags_query_builder_from returned NULL")
    MetadataTagsQueryBuilder(ptr)
end

"""
    with_tag(b, name::CodebookName, value::AbstractString) -> MetadataTagsQueryBuilder

Return a new builder requiring that codebook slot `name` equals `value`.
"""
function with_tag(b::MetadataTagsQueryBuilderRef, name::CodebookName, value::AbstractString)
    s = String(value)
    ptr = GC.@preserve s ffi_metadata_tags_query_builder_with_tag(
        b._ptr,
        Cint(Int32(name)),
        Base.unsafe_convert(Cstring, s),
    )
    ptr == C_NULL &&
        error("dnv_vista_sdk_metadata_tags_query_builder_with_tag returned NULL")
    MetadataTagsQueryBuilder(ptr)
end

function with_tag(b::MetadataTagsQueryBuilder, name::CodebookName, value::AbstractString)
    with_tag(b._ref, name, value)
end

"""
    with_metadata_tag(b, tag) -> MetadataTagsQueryBuilder

Return a new builder requiring that `tag`'s codebook slot equals `tag`'s value.
"""
function with_metadata_tag(b::MetadataTagsQueryBuilderRef, tag)
    ptr = ffi_metadata_tags_query_builder_with_metadata_tag(b._ptr, _ptr(tag))
    ptr == C_NULL &&
        error("dnv_vista_sdk_metadata_tags_query_builder_with_metadata_tag returned NULL")
    MetadataTagsQueryBuilder(ptr)
end

function with_metadata_tag(b::MetadataTagsQueryBuilder, tag)
    with_metadata_tag(b._ref, tag)
end

"""
    with_allow_other_tags(b, allow::Bool) -> MetadataTagsQueryBuilder

Return a new builder with the allow-other-tags flag set to `allow`.

When `false`, a LocalId must carry exactly the tags specified.
"""
function with_allow_other_tags(b::MetadataTagsQueryBuilderRef, allow::Bool)
    ptr = ffi_metadata_tags_query_builder_with_allow_other_tags(b._ptr, Cint(allow))
    ptr == C_NULL && error(
        "dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags returned NULL",
    )
    MetadataTagsQueryBuilder(ptr)
end

function with_allow_other_tags(b::MetadataTagsQueryBuilder, allow::Bool)
    with_allow_other_tags(b._ref, allow)
end

"""
    build(b) -> MetadataTagsQuery

Construct the immutable [`MetadataTagsQuery`].
"""
function build(b::MetadataTagsQueryBuilderRef)
    ptr = ffi_metadata_tags_query_builder_build(b._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_metadata_tags_query_builder_build returned NULL")
    MetadataTagsQuery(ptr)
end

function build(b::MetadataTagsQueryBuilder)
    build(b._ref)
end

"""
    is_match(q::MetadataTagsQuery, lid) -> Bool

Whether `lid` satisfies all constraints in `q`.
"""
function is_match(q::MetadataTagsQuery, lid)
    ffi_metadata_tags_query_match(q._ptr, _ptr(lid)) != 0
end

"""
    builder(q::MetadataTagsQuery) -> MetadataTagsQueryBuilderRef

Return the builder state underlying `q`. The ref is valid for `q`'s lifetime.
"""
function builder(q::MetadataTagsQuery)
    ptr = ffi_metadata_tags_query_builder(q._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_metadata_tags_query_builder returned NULL")
    MetadataTagsQueryBuilderRef(ptr, q)
end
