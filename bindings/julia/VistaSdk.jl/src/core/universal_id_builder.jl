"""
    UniversalIdBuilderRef

Borrowed reference to a [`UniversalIdBuilder`](@ref). Valid only for the
lifetime of the owning object. The `_owner` field keeps the Julia owner
reachable so the GC cannot run its finalizer while this ref is live.
"""
struct UniversalIdBuilderRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    UniversalIdBuilder

Immutable builder for constructing a [`UniversalId`](@ref). Each `with_*`/`without_*`
call returns a new owned builder. Freed when garbage-collected.
"""
mutable struct UniversalIdBuilder
    _ref::UniversalIdBuilderRef
    function UniversalIdBuilder(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("UniversalIdBuilder: NULL pointer")
        t = new(UniversalIdBuilderRef(ptr, nothing))
        finalizer(t) do x
            ffi_universal_id_builder_free(x._ref._ptr)
            x._ref = UniversalIdBuilderRef(C_NULL, nothing)
        end
        t
    end
end

function _ptr(b::UniversalIdBuilderRef)
    b._ptr
end

function _ptr(b::UniversalIdBuilder)
    b._ref._ptr
end

"""
    naming_entity(::Type{UniversalIdBuilder}) -> String

ISO 19848 Annex C naming entity (`"data.dnv.com"`).
"""
function naming_entity(::Base.Type{UniversalIdBuilder})
    ptr = ffi_universal_id_builder_naming_entity()
    ptr == C_NULL && error("dnv_vista_sdk_universal_id_builder_naming_entity returned NULL")
    unsafe_string(ptr)
end

"""
    create(::Type{UniversalIdBuilder}, ver::VisVersion) -> UniversalIdBuilder

Create a new empty builder for the given VIS version.
"""
function create(::Base.Type{UniversalIdBuilder}, ver::VisVersion)
    s = string(ver)
    ptr = GC.@preserve s ffi_universal_id_builder_create(Base.unsafe_convert(Cstring, s))
    ptr == C_NULL && error("dnv_vista_sdk_universal_id_builder_create returned NULL")
    UniversalIdBuilder(ptr)
end

function imo_number(b::UniversalIdBuilderRef)
    ptr = ffi_universal_id_builder_imo_number(b._ptr)
    ptr == C_NULL && return nothing
    v = ffi_imo_number_value(ptr)
    ffi_imo_number_free(ptr)
    ImoNumber(UInt32(v))
end

function imo_number(b::UniversalIdBuilder)
    imo_number(b._ref)
end

function local_id(b::UniversalIdBuilderRef)
    ptr = ffi_universal_id_builder_local_id(b._ptr)
    ptr == C_NULL && return nothing
    LocalIdBuilder(ptr)
end

function local_id(b::UniversalIdBuilder)
    local_id(b._ref)
end

function is_valid(b::UniversalIdBuilderRef)
    ffi_universal_id_builder_is_valid(b._ptr) != 0
end

function is_valid(b::UniversalIdBuilder)
    is_valid(b._ref)
end

function with_imo_number(b::UniversalIdBuilderRef, imo::ImoNumber)
    imo_ptr = ffi_imo_number_create(Cint(imo._value))
    imo_ptr == C_NULL && error("dnv_vista_sdk_imo_number_create returned NULL")
    ptr = ffi_universal_id_builder_with_imo_number(b._ptr, imo_ptr)
    ffi_imo_number_free(imo_ptr)
    ptr == C_NULL &&
        error("dnv_vista_sdk_universal_id_builder_with_imo_number returned NULL")
    UniversalIdBuilder(ptr)
end

function with_imo_number(b::UniversalIdBuilder, imo::ImoNumber)
    with_imo_number(b._ref, imo)
end

function without_imo_number(b::UniversalIdBuilderRef)
    ptr = ffi_universal_id_builder_without_imo_number(b._ptr)
    ptr == C_NULL &&
        error("dnv_vista_sdk_universal_id_builder_without_imo_number returned NULL")
    UniversalIdBuilder(ptr)
end

function without_imo_number(b::UniversalIdBuilder)
    without_imo_number(b._ref)
end

function with_local_id(b::UniversalIdBuilderRef, lb::LocalIdBuilderRef)
    ptr = ffi_universal_id_builder_with_local_id(b._ptr, lb._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_universal_id_builder_with_local_id returned NULL")
    UniversalIdBuilder(ptr)
end

function with_local_id(b::UniversalIdBuilder, lb::Union{LocalIdBuilderRef,LocalIdBuilder})
    with_local_id(b._ref, lb isa LocalIdBuilder ? lb._ref : lb)
end

function with_local_id(b::UniversalIdBuilderRef, lb::LocalIdBuilder)
    with_local_id(b, lb._ref)
end

function without_local_id(b::UniversalIdBuilderRef)
    ptr = ffi_universal_id_builder_without_local_id(b._ptr)
    ptr == C_NULL &&
        error("dnv_vista_sdk_universal_id_builder_without_local_id returned NULL")
    UniversalIdBuilder(ptr)
end

function without_local_id(b::UniversalIdBuilder)
    without_local_id(b._ref)
end

function build(b::UniversalIdBuilderRef)
    ptr = ffi_universal_id_builder_build(b._ptr)
    ptr == C_NULL && throw(last_error())
    UniversalId(ptr)
end

function build(b::UniversalIdBuilder)
    build(b._ref)
end

function Base.:(==)(a::UniversalIdBuilderRef, b::UniversalIdBuilderRef)
    ffi_universal_id_builder_equals(a._ptr, b._ptr) != 0
end

function Base.:(==)(a::UniversalIdBuilder, b::UniversalIdBuilder)
    a._ref == b._ref
end

function Base.show(io::IO, b::UniversalIdBuilderRef)
    ptr = ffi_universal_id_builder_to_string(b._ptr)
    ptr == C_NULL && (print(io, "<UniversalIdBuilder NULL>"); return)
    s = unsafe_string(ptr)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), ptr)
    print(io, s)
end

function Base.show(io::IO, b::UniversalIdBuilder)
    show(io, b._ref)
end

"""
    from_string(::Type{UniversalIdBuilder}, s) -> Union{UniversalId, Nothing}

Parse a UniversalId string via the builder. Returns `nothing` on invalid input.
"""
function from_string(::Base.Type{UniversalIdBuilder}, s::AbstractString)
    str = String(s)
    ptr = GC.@preserve str ffi_universal_id_builder_from_string(
        Base.unsafe_convert(Cstring, str),
    )
    ptr == C_NULL ? nothing : UniversalId(ptr)
end

"""
    from_string_with_errors(::Type{UniversalIdBuilder}, s) -> (Union{UniversalId,Nothing}, ParsingErrors)

Parse a UniversalId string, returning any parse errors alongside the result.
"""
function from_string_with_errors(::Base.Type{UniversalIdBuilder}, s::AbstractString)
    str = String(s)
    out_errors = Ref{Ptr{Cvoid}}(C_NULL)
    ptr = GC.@preserve str ffi_universal_id_builder_from_string_with_errors(
        Base.unsafe_convert(Cstring, str),
        out_errors,
    )
    universal_id = ptr == C_NULL ? nothing : UniversalId(ptr)
    errors =
        out_errors[] == C_NULL ? error("from_string_with_errors: out_errors is NULL") :
        ParsingErrors(out_errors[])
    (universal_id, errors)
end
