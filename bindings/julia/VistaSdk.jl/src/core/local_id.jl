"""
    LocalIdRef

Borrowed reference to a [`LocalId`](@ref). The `_owner` field prevents GC
collection of the owning object while this ref is live.
"""
struct LocalIdRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    LocalId

An immutable, validated VIS local ID (ISO 19848). Freed when garbage-collected.
"""
mutable struct LocalId
    _ref::LocalIdRef
    function LocalId(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("LocalId: NULL pointer")
        t = new(LocalIdRef(ptr, nothing))
        finalizer(t) do x
            ffi_local_id_free(x._ref._ptr)
            x._ref = LocalIdRef(C_NULL, nothing)
        end
        t
    end
end

function _ptr(id::LocalIdRef)
    id._ptr
end

function _ptr(id::LocalId)
    id._ref._ptr
end

"""
    naming_rule(::Type{LocalId}) -> String

Return the naming rule string for all local IDs (e.g. `"dnv-v2"`).
"""
function naming_rule(::Base.Type{LocalId})
    ptr = ffi_local_id_naming_rule()
    ptr == C_NULL && error("dnv_vista_sdk_local_id_naming_rule returned NULL")
    unsafe_string(ptr)
end

"""
    version(id::Union{LocalIdRef,LocalId}) -> VisVersion

Return the VIS version of the local ID.
"""
function version(id::LocalIdRef)
    ptr = ffi_local_id_version(id._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_local_id_version returned NULL")
    Base.parse(VisVersion, unsafe_string(ptr))
end

function version(id::LocalId)
    version(id._ref)
end

"""
    primary_item(id::Union{LocalIdRef,LocalId}) -> GmodPathRef

Return a borrowed reference to the primary item path.
"""
function primary_item(id::LocalIdRef, owner::Any)
    ptr = ffi_local_id_primary_item(id._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_local_id_primary_item returned NULL")
    GmodPathRef(ptr, owner)
end

function primary_item(id::LocalIdRef)
    primary_item(id, id)
end

function primary_item(id::LocalId)
    primary_item(id._ref, id)
end

"""
    secondary_item(id::Union{LocalIdRef,LocalId}) -> Union{GmodPathRef,Nothing}

Return a borrowed reference to the secondary item path, or `nothing` if absent.
"""
function secondary_item(id::LocalIdRef, owner::Any)
    ptr = ffi_local_id_secondary_item(id._ptr)
    ptr == C_NULL ? nothing : GmodPathRef(ptr, owner)
end

function secondary_item(id::LocalIdRef)
    secondary_item(id, id)
end

function secondary_item(id::LocalId)
    secondary_item(id._ref, id)
end

"""
    is_verbose_mode(id::Union{LocalIdRef,LocalId}) -> Bool
"""
function is_verbose_mode(id::LocalIdRef)
    ffi_local_id_is_verbose_mode(id._ptr) != 0
end

function is_verbose_mode(id::LocalId)
    is_verbose_mode(id._ref)
end

"""
    has_custom_tag(id::Union{LocalIdRef,LocalId}) -> Bool
"""
function has_custom_tag(id::LocalIdRef)
    ffi_local_id_has_custom_tag(id._ptr) != 0
end

function has_custom_tag(id::LocalId)
    has_custom_tag(id._ref)
end

"""
    metadata_tag(id::Union{LocalIdRef,LocalId}, cb_name::CodebookName) -> Union{MetadataTagRef,Nothing}

Return the metadata tag for the given codebook, or `nothing` if absent.
"""
function metadata_tag(id::LocalIdRef, cb_name::CodebookName)
    ptr = ffi_local_id_metadata_tag(id._ptr, Cint(Integer(cb_name)))
    ptr == C_NULL ? nothing : MetadataTagRef(ptr)
end

function metadata_tag(id::LocalId, cb_name::CodebookName)
    metadata_tag(id._ref, cb_name)
end

"""
    quantity(id) / content(id) / calculation(id) / state(id) / command(id) / tag_type(id) / position(id) / detail(id)

Convenience accessors for each metadata tag codebook. Return `nothing` if the tag is absent.
"""
quantity(id::Union{LocalIdRef,LocalId}) = metadata_tag(id, Quantity)
content(id::Union{LocalIdRef,LocalId}) = metadata_tag(id, Content)
calculation(id::Union{LocalIdRef,LocalId}) = metadata_tag(id, Calculation)
state(id::Union{LocalIdRef,LocalId}) = metadata_tag(id, State)
command(id::Union{LocalIdRef,LocalId}) = metadata_tag(id, Command)
tag_type(id::Union{LocalIdRef,LocalId}) = metadata_tag(id, Type)
position(id::Union{LocalIdRef,LocalId}) = metadata_tag(id, Position)
detail(id::Union{LocalIdRef,LocalId}) = metadata_tag(id, Detail)

"""
    metadata_tags(id::Union{LocalIdRef,LocalId}) -> Vector{MetadataTagRef}

Return all metadata tags that are set, in codebook order.
"""
function metadata_tags(id::Union{LocalIdRef,LocalId})
    filter(
        !isnothing,
        [
            quantity(id),
            content(id),
            calculation(id),
            state(id),
            command(id),
            tag_type(id),
            position(id),
            detail(id),
        ],
    )
end

"""
    builder(id::Union{LocalIdRef,LocalId}) -> LocalIdBuilderRef

Return a borrowed builder pre-populated from this local ID.
"""
function builder(id::LocalIdRef, owner::Any)
    ptr = ffi_local_id_builder(id._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_local_id_builder returned NULL")
    LocalIdBuilderRef(ptr, owner)
end

function builder(id::LocalIdRef)
    builder(id, id)
end

function builder(id::LocalId)
    builder(id._ref, id)
end

function Base.:(==)(a::LocalIdRef, b::LocalIdRef)
    ffi_local_id_equals(a._ptr, b._ptr) != 0
end

function Base.:(==)(a::LocalId, b::LocalId)
    a._ref == b._ref
end

function Base.show(io::IO, id::LocalIdRef)
    ptr = ffi_local_id_to_string(id._ptr)
    ptr == C_NULL && (print(io, "<LocalId NULL>"); return)
    s = unsafe_string(ptr)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), ptr)
    print(io, s)
end

function Base.show(io::IO, id::LocalId)
    show(io, id._ref)
end

"""
    from_string(::Type{LocalId}, s::AbstractString) -> Union{LocalId,Nothing}

Parse a local ID string and return a [`LocalId`](@ref), or `nothing` on failure.
"""
function from_string(::Base.Type{LocalId}, s::AbstractString)
    str = String(s)
    ptr = GC.@preserve str ffi_local_id_from_string(Base.unsafe_convert(Cstring, str))
    ptr == C_NULL ? nothing : LocalId(ptr)
end

"""
    from_string_with_errors(::Type{LocalId}, s::AbstractString) -> Tuple{Union{LocalId,Nothing}, ParsingErrors}

Parse a local ID string and return `(local_id, errors)`. `local_id` is `nothing` on failure.
"""
function from_string_with_errors(::Base.Type{LocalId}, s::AbstractString)
    str = String(s)
    out_errors = Ref{Ptr{Cvoid}}(C_NULL)
    ptr = GC.@preserve str ffi_local_id_from_string_with_errors(
        Base.unsafe_convert(Cstring, str),
        out_errors,
    )
    local_id = ptr == C_NULL ? nothing : LocalId(ptr)
    errors =
        out_errors[] == C_NULL ? error("from_string_with_errors: out_errors is NULL") :
        ParsingErrors(out_errors[])
    (local_id, errors)
end
