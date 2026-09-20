"""
    LocalIdBuilderRef

Borrowed reference to a [`LocalIdBuilder`](@ref). The `_owner` field prevents GC
collection of the owning object while this ref is live.
"""
struct LocalIdBuilderRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    LocalIdBuilder

Immutable builder for constructing a [`LocalId`](@ref). Each `with_*`/`without_*`
call returns a new owned builder. Freed when garbage-collected.
"""
mutable struct LocalIdBuilder
    _ref::LocalIdBuilderRef
    function LocalIdBuilder(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("LocalIdBuilder: NULL pointer")
        t = new(LocalIdBuilderRef(ptr, nothing))
        finalizer(t) do x
            ffi_local_id_builder_free(x._ref._ptr)
            x._ref = LocalIdBuilderRef(C_NULL, nothing)
        end
        t
    end
end

function _ptr(b::LocalIdBuilderRef)
    b._ptr
end

function _ptr(b::LocalIdBuilder)
    b._ref._ptr
end

"""
    naming_rule(::Type{LocalIdBuilder}) -> String

Return the naming rule string for all local IDs (e.g. `"dnv-v2"`).
"""
function naming_rule(::Base.Type{LocalIdBuilder})
    ptr = ffi_local_id_builder_naming_rule()
    ptr == C_NULL && error("dnv_vista_sdk_local_id_builder_naming_rule returned NULL")
    unsafe_string(ptr)
end

"""
    create(::Type{LocalIdBuilder}, ver::VisVersion) -> LocalIdBuilder

Create an empty builder for the given VIS version.
"""
function create(::Base.Type{LocalIdBuilder}, ver::VisVersion)
    s = string(ver)
    ptr = GC.@preserve s ffi_local_id_builder_create(Base.unsafe_convert(Cstring, s))
    ptr == C_NULL && error("dnv_vista_sdk_local_id_builder_create returned NULL")
    LocalIdBuilder(ptr)
end

"""
    version(b::Union{LocalIdBuilderRef,LocalIdBuilder}) -> Union{VisVersion,Nothing}

Return the VIS version set on the builder, or `nothing` if unset.
"""
function version(b::LocalIdBuilderRef)
    ptr = ffi_local_id_builder_version(b._ptr)
    ptr == C_NULL ? nothing : Base.parse(VisVersion, unsafe_string(ptr))
end

function version(b::LocalIdBuilder)
    version(b._ref)
end

"""
    is_verbose_mode(b::Union{LocalIdBuilderRef,LocalIdBuilder}) -> Bool
"""
is_verbose_mode(b::LocalIdBuilderRef) = ffi_local_id_builder_is_verbose_mode(b._ptr) != 0
is_verbose_mode(b::LocalIdBuilder) = is_verbose_mode(b._ref)

"""
    is_valid(b::Union{LocalIdBuilderRef,LocalIdBuilder}) -> Bool

Return `true` if the builder has enough state to produce a valid [`LocalId`](@ref).
"""
is_valid(b::LocalIdBuilderRef) = ffi_local_id_builder_is_valid(b._ptr) != 0
is_valid(b::LocalIdBuilder) = is_valid(b._ref)

"""
    is_empty(b::Union{LocalIdBuilderRef,LocalIdBuilder}) -> Bool
"""
is_empty(b::LocalIdBuilderRef) = ffi_local_id_builder_is_empty(b._ptr) != 0
is_empty(b::LocalIdBuilder) = is_empty(b._ref)

"""
    is_empty_metadata(b::Union{LocalIdBuilderRef,LocalIdBuilder}) -> Bool
"""
is_empty_metadata(b::LocalIdBuilderRef) =
    ffi_local_id_builder_is_empty_metadata(b._ptr) != 0
is_empty_metadata(b::LocalIdBuilder) = is_empty_metadata(b._ref)

"""
    has_custom_tag(b::Union{LocalIdBuilderRef,LocalIdBuilder}) -> Bool
"""
has_custom_tag(b::LocalIdBuilderRef) = ffi_local_id_builder_has_custom_tag(b._ptr) != 0
has_custom_tag(b::LocalIdBuilder) = has_custom_tag(b._ref)

"""
    primary_item(b::Union{LocalIdBuilderRef,LocalIdBuilder}) -> Union{GmodPath,Nothing}

Return the primary item path, or `nothing` if unset.
"""
function primary_item(b::LocalIdBuilderRef)
    ptr = ffi_local_id_builder_primary_item(b._ptr)
    ptr == C_NULL ? nothing : GmodPath(ptr)
end

function primary_item(b::LocalIdBuilder)
    primary_item(b._ref)
end

"""
    secondary_item(b::Union{LocalIdBuilderRef,LocalIdBuilder}) -> Union{GmodPath,Nothing}

Return the secondary item path, or `nothing` if unset.
"""
function secondary_item(b::LocalIdBuilderRef)
    ptr = ffi_local_id_builder_secondary_item(b._ptr)
    ptr == C_NULL ? nothing : GmodPath(ptr)
end

function secondary_item(b::LocalIdBuilder)
    secondary_item(b._ref)
end

"""
    metadata_tag(b::Union{LocalIdBuilderRef,LocalIdBuilder}, cb_name::CodebookName) -> Union{MetadataTag,Nothing}

Return the metadata tag for the given codebook, or `nothing` if unset.
"""
function metadata_tag(b::LocalIdBuilderRef, cb_name::CodebookName)
    ptr = ffi_local_id_builder_metadata_tag(b._ptr, Cint(Integer(cb_name)))
    ptr == C_NULL ? nothing : MetadataTag(ptr)
end

function metadata_tag(b::LocalIdBuilder, cb_name::CodebookName)
    metadata_tag(b._ref, cb_name)
end

"""
    quantity(b) / content(b) / calculation(b) / state(b) / command(b) / tag_type(b) / position(b) / detail(b)

Convenience accessors for each metadata tag codebook. Return `nothing` if the tag is unset.
"""
quantity(b::Union{LocalIdBuilderRef,LocalIdBuilder}) = metadata_tag(b, Quantity)
content(b::Union{LocalIdBuilderRef,LocalIdBuilder}) = metadata_tag(b, Content)
calculation(b::Union{LocalIdBuilderRef,LocalIdBuilder}) = metadata_tag(b, Calculation)
state(b::Union{LocalIdBuilderRef,LocalIdBuilder}) = metadata_tag(b, State)
command(b::Union{LocalIdBuilderRef,LocalIdBuilder}) = metadata_tag(b, Command)
tag_type(b::Union{LocalIdBuilderRef,LocalIdBuilder}) = metadata_tag(b, Type)
position(b::Union{LocalIdBuilderRef,LocalIdBuilder}) = metadata_tag(b, Position)
detail(b::Union{LocalIdBuilderRef,LocalIdBuilder}) = metadata_tag(b, Detail)

"""
    with_vis_version(b, ver::VisVersion) -> LocalIdBuilder

Return a new builder with the VIS version set to `ver`.
"""
function with_vis_version(b::LocalIdBuilderRef, ver::VisVersion)
    s = string(ver)
    ptr = GC.@preserve s ffi_local_id_builder_with_vis_version(
        b._ptr,
        Base.unsafe_convert(Cstring, s),
    )
    ptr == C_NULL && throw(last_error())
    LocalIdBuilder(ptr)
end

function with_vis_version(b::LocalIdBuilder, ver::VisVersion)
    with_vis_version(b._ref, ver)
end

"""
    without_vis_version(b) -> LocalIdBuilder

Return a new builder with the VIS version cleared.
"""
function without_vis_version(b::LocalIdBuilderRef)
    ptr = ffi_local_id_builder_without_vis_version(b._ptr)
    ptr == C_NULL &&
        error("dnv_vista_sdk_local_id_builder_without_vis_version returned NULL")
    LocalIdBuilder(ptr)
end

function without_vis_version(b::LocalIdBuilder)
    without_vis_version(b._ref)
end

"""
    with_primary_item(b, path::GmodPath) -> LocalIdBuilder

Return a new builder with the primary item set to `path`.
"""
function with_primary_item(b::LocalIdBuilderRef, path::GmodPath)
    ptr = GC.@preserve path ffi_local_id_builder_with_primary_item(b._ptr, path._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_local_id_builder_with_primary_item returned NULL")
    LocalIdBuilder(ptr)
end

function with_primary_item(b::LocalIdBuilder, path::GmodPath)
    with_primary_item(b._ref, path)
end

"""
    without_primary_item(b) -> LocalIdBuilder

Return a new builder with the primary item cleared.
"""
function without_primary_item(b::LocalIdBuilderRef)
    ptr = ffi_local_id_builder_without_primary_item(b._ptr)
    ptr == C_NULL &&
        error("dnv_vista_sdk_local_id_builder_without_primary_item returned NULL")
    LocalIdBuilder(ptr)
end

function without_primary_item(b::LocalIdBuilder)
    without_primary_item(b._ref)
end

"""
    with_secondary_item(b, path::GmodPath) -> LocalIdBuilder

Return a new builder with the secondary item set to `path`.
"""
function with_secondary_item(b::LocalIdBuilderRef, path::GmodPath)
    ptr = GC.@preserve path ffi_local_id_builder_with_secondary_item(b._ptr, path._ptr)
    ptr == C_NULL &&
        error("dnv_vista_sdk_local_id_builder_with_secondary_item returned NULL")
    LocalIdBuilder(ptr)
end

function with_secondary_item(b::LocalIdBuilder, path::GmodPath)
    with_secondary_item(b._ref, path)
end

"""
    without_secondary_item(b) -> LocalIdBuilder

Return a new builder with the secondary item cleared.
"""
function without_secondary_item(b::LocalIdBuilderRef)
    ptr = ffi_local_id_builder_without_secondary_item(b._ptr)
    ptr == C_NULL &&
        error("dnv_vista_sdk_local_id_builder_without_secondary_item returned NULL")
    LocalIdBuilder(ptr)
end

function without_secondary_item(b::LocalIdBuilder)
    without_secondary_item(b._ref)
end

"""
    with_metadata_tag(b, tag::MetadataTag) -> LocalIdBuilder

Return a new builder with `tag` added to the metadata.
"""
function with_metadata_tag(b::LocalIdBuilderRef, tag::MetadataTag)
    ptr = GC.@preserve tag ffi_local_id_builder_with_metadata_tag(b._ptr, tag._ref._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_local_id_builder_with_metadata_tag returned NULL")
    LocalIdBuilder(ptr)
end

function with_metadata_tag(b::LocalIdBuilder, tag::MetadataTag)
    with_metadata_tag(b._ref, tag)
end

"""
    without_metadata_tag(b, cb_name::CodebookName) -> LocalIdBuilder

Return a new builder with the metadata tag for `cb_name` removed.
"""
function without_metadata_tag(b::LocalIdBuilderRef, cb_name::CodebookName)
    ptr = ffi_local_id_builder_without_metadata_tag(b._ptr, Cint(Integer(cb_name)))
    ptr == C_NULL &&
        error("dnv_vista_sdk_local_id_builder_without_metadata_tag returned NULL")
    LocalIdBuilder(ptr)
end

function without_metadata_tag(b::LocalIdBuilder, cb_name::CodebookName)
    without_metadata_tag(b._ref, cb_name)
end

"""
    with_verbose_mode(b, verbose::Bool) -> LocalIdBuilder

Return a new builder with verbose mode set.
"""
function with_verbose_mode(b::LocalIdBuilderRef, verbose::Bool)
    ptr = ffi_local_id_builder_with_verbose_mode(b._ptr, Cint(verbose))
    ptr == C_NULL && error("dnv_vista_sdk_local_id_builder_with_verbose_mode returned NULL")
    LocalIdBuilder(ptr)
end

function with_verbose_mode(b::LocalIdBuilder, verbose::Bool)
    with_verbose_mode(b._ref, verbose)
end

"""
    build(b::Union{LocalIdBuilderRef,LocalIdBuilder}) -> LocalId

Build and return a [`LocalId`](@ref). Throws if the builder state is invalid.
"""
function build(b::LocalIdBuilderRef)
    ptr = ffi_local_id_builder_build(b._ptr)
    ptr == C_NULL && throw(last_error())
    LocalId(ptr)
end

function build(b::LocalIdBuilder)
    build(b._ref)
end

function Base.:(==)(a::LocalIdBuilderRef, b::LocalIdBuilderRef)
    ffi_local_id_builder_equals(a._ptr, b._ptr) != 0
end

function Base.:(==)(a::LocalIdBuilder, b::LocalIdBuilder)
    a._ref == b._ref
end

function Base.show(io::IO, b::LocalIdBuilderRef)
    ptr = ffi_local_id_builder_to_string(b._ptr)
    ptr == C_NULL && (print(io, "<LocalIdBuilder NULL>"); return)
    s = unsafe_string(ptr)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), ptr)
    print(io, s)
end

function Base.show(io::IO, b::LocalIdBuilder)
    show(io, b._ref)
end

"""
    from_string(::Type{LocalIdBuilder}, s::AbstractString) -> Union{LocalId,Nothing}

Parse a local ID string and return a [`LocalId`](@ref), or `nothing` on failure.
"""
function from_string(::Base.Type{LocalIdBuilder}, s::AbstractString)
    str = String(s)
    ptr =
        GC.@preserve str ffi_local_id_builder_from_string(Base.unsafe_convert(Cstring, str))
    ptr == C_NULL ? nothing : LocalId(ptr)
end

"""
    from_string_with_errors(::Type{LocalIdBuilder}, s::AbstractString) -> Tuple{Union{LocalId,Nothing}, ParsingErrors}

Parse a local ID string and return `(local_id, errors)`. `local_id` is `nothing` on failure.
"""
function from_string_with_errors(::Base.Type{LocalIdBuilder}, s::AbstractString)
    str = String(s)
    out_errors = Ref{Ptr{Cvoid}}(C_NULL)
    ptr = GC.@preserve str ffi_local_id_builder_from_string_with_errors(
        Base.unsafe_convert(Cstring, str),
        out_errors,
    )
    local_id = ptr == C_NULL ? nothing : LocalId(ptr)
    errors =
        out_errors[] == C_NULL ? error("from_string_with_errors: out_errors is NULL") :
        ParsingErrors(out_errors[])
    (local_id, errors)
end
