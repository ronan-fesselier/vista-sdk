"""
    DocumentKind

Discriminant for the JSON-like value kind of a [`SerializableDocument`](@ref).
"""
@enum DocumentKind::Int32 begin
    DocNull = 0
    DocBoolean = 1
    DocInteger = 2
    DocDouble = 3
    DocString = 4
    DocArray = 5
    DocObject = 6
end

"""
    SerializableDocument

An owned, mutable JSON-like document node.

Constructors: [`SerializableDocument()`](@ref) (null), [`SerializableDocument(::Bool)`](@ref),
[`SerializableDocument(::Int64)`](@ref), [`SerializableDocument(::Float64)`](@ref),
[`SerializableDocument(::AbstractString)`](@ref), [`sd_array()`](@ref), [`sd_object()`](@ref).
"""
mutable struct SerializableDocument
    _ptr::Ptr{Cvoid}

    function SerializableDocument(ptr::Ptr{Cvoid})
        ptr == C_NULL &&
            throw(VistaError(InvalidArgument, "null SerializableDocument pointer"))
        doc = new(ptr)
        finalizer(doc) do d
            if d._ptr != C_NULL
                ffi_sd_free(d._ptr)
                d._ptr = C_NULL
            end
        end
        doc
    end
end

function _transfer_ownership(doc::SerializableDocument)
    ptr = doc._ptr
    doc._ptr = C_NULL
    GC.@preserve doc finalize(doc)
    ptr
end

"""
    SerializableDocument() -> SerializableDocument

Construct a null document.
"""
SerializableDocument() = SerializableDocument(ffi_sd_null())

"""
    SerializableDocument(value::Bool) -> SerializableDocument
"""
SerializableDocument(value::Bool) =
    SerializableDocument(ffi_sd_from_boolean(value ? Cint(1) : Cint(0)))

"""
    SerializableDocument(value::Int64) -> SerializableDocument
"""
SerializableDocument(value::Int64) = SerializableDocument(ffi_sd_from_integer(value))

"""
    SerializableDocument(value::Float64) -> SerializableDocument
"""
SerializableDocument(value::Float64) =
    SerializableDocument(ffi_sd_from_double(Cdouble(value)))

"""
    SerializableDocument(value::Decimal) -> SerializableDocument
"""
SerializableDocument(value::Decimal) = SerializableDocument(Base.string(value))

"""
    SerializableDocument(value::AbstractString) -> SerializableDocument
"""
function SerializableDocument(value::AbstractString)
    ptr = GC.@preserve value ffi_sd_from_string(Base.unsafe_convert(Cstring, value))
    SerializableDocument(ptr)
end

"""
    sd_array() -> SerializableDocument

Construct an empty array document.
"""
sd_array() = SerializableDocument(ffi_sd_array())

"""
    sd_object() -> SerializableDocument

Construct an empty object document.
"""
sd_object() = SerializableDocument(ffi_sd_object())

"""
    SerializableDocumentRef

A borrowed, read-only view into a [`SerializableDocument`](@ref).
Keeps a reference to the owning document to prevent GC collection.
Do not store this past the lifetime of the owner.
"""
struct SerializableDocumentRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

_sd_ptr(d::SerializableDocument) = d._ptr
_sd_ptr(d::SerializableDocumentRef) = d._ptr

"""
    kind(doc) -> DocumentKind

Return the kind of the document node.
"""
function kind(doc::Union{SerializableDocument,SerializableDocumentRef})
    raw = ffi_sd_kind(_sd_ptr(doc))
    DocumentKind(Int32(raw))
end

"""
    is_null(doc) -> Bool
"""
is_null(doc::Union{SerializableDocument,SerializableDocumentRef}) =
    ffi_sd_is_null(_sd_ptr(doc)) != 0

"""
    is_boolean(doc) -> Bool
"""
is_boolean(doc::Union{SerializableDocument,SerializableDocumentRef}) =
    ffi_sd_is_boolean(_sd_ptr(doc)) != 0

"""
    is_integer(doc) -> Bool
"""
is_integer(doc::Union{SerializableDocument,SerializableDocumentRef}) =
    ffi_sd_is_integer(_sd_ptr(doc)) != 0

"""
    is_double(doc) -> Bool
"""
is_double(doc::Union{SerializableDocument,SerializableDocumentRef}) =
    ffi_sd_is_double(_sd_ptr(doc)) != 0

"""
    is_string(doc) -> Bool
"""
is_string(doc::Union{SerializableDocument,SerializableDocumentRef}) =
    ffi_sd_is_string(_sd_ptr(doc)) != 0

"""
    is_array(doc) -> Bool
"""
is_array(doc::Union{SerializableDocument,SerializableDocumentRef}) =
    ffi_sd_is_array(_sd_ptr(doc)) != 0

"""
    is_object(doc) -> Bool
"""
is_object(doc::Union{SerializableDocument,SerializableDocumentRef}) =
    ffi_sd_is_object(_sd_ptr(doc)) != 0

"""
    as_boolean(doc) -> Bool

Return the boolean value. Behaviour is undefined if kind is not Boolean.
"""
as_boolean(doc::Union{SerializableDocument,SerializableDocumentRef}) =
    ffi_sd_as_boolean(_sd_ptr(doc)) != 0

"""
    as_integer(doc) -> Int64

Return the integer value. Behaviour is undefined if kind is not Integer.
"""
as_integer(doc::Union{SerializableDocument,SerializableDocumentRef}) =
    ffi_sd_as_integer(_sd_ptr(doc))

"""
    as_double(doc) -> Float64

Return the double value. Behaviour is undefined if kind is not Double.
"""
as_double(doc::Union{SerializableDocument,SerializableDocumentRef}) =
    Float64(ffi_sd_as_double(_sd_ptr(doc)))

"""
    as_string(doc) -> String

Return the string value. Behaviour is undefined if kind is not String.
"""
as_string(doc::Union{SerializableDocument,SerializableDocumentRef}) =
    unsafe_string(ffi_sd_as_string(_sd_ptr(doc)))

"""
    array_size(doc) -> Int

Return the number of elements in an array document.
"""
array_size(doc::Union{SerializableDocument,SerializableDocumentRef}) =
    Int(ffi_sd_array_size(_sd_ptr(doc)))

"""
    array_at(doc, index::Int) -> SerializableDocumentRef

Return a borrowed view of the element at `index` (1-based).
"""
function array_at(doc::Union{SerializableDocument,SerializableDocumentRef}, index::Int)
    ptr = ffi_sd_array_at(_sd_ptr(doc), Csize_t(index - 1))
    ptr == C_NULL && throw(BoundsError(doc, index))
    owner = doc isa SerializableDocumentRef ? doc._owner : doc
    SerializableDocumentRef(ptr, owner)

end

"""
    push_back!(doc::SerializableDocument, value::SerializableDocument)

Append `value` to the array `doc`, transferring ownership of `value`.
`value` must not be used after this call.
"""
function push_back!(doc::SerializableDocument, value::SerializableDocument)
    val_ptr = _transfer_ownership(value)
    ffi_sd_push_back(doc._ptr, val_ptr)
    nothing
end

"""
    object_size(doc) -> Int

Return the number of key-value pairs in an object document.
"""
object_size(doc::Union{SerializableDocument,SerializableDocumentRef}) =
    Int(ffi_sd_object_size(_sd_ptr(doc)))

"""
    object_key_at(doc, index::Int) -> String

Return the key at `index` (1-based) in an object document.
"""
function object_key_at(doc::Union{SerializableDocument,SerializableDocumentRef}, index::Int)
    raw = ffi_sd_object_key_at(_sd_ptr(doc), Csize_t(index - 1))
    raw == C_NULL && throw(BoundsError(doc, index))
    unsafe_string(raw)
end

"""
    object_value_at(doc, index::Int) -> SerializableDocumentRef

Return a borrowed view of the value at `index` (1-based) in an object document.
"""
function object_value_at(
    doc::Union{SerializableDocument,SerializableDocumentRef},
    index::Int,
)
    ptr = ffi_sd_object_value_at(_sd_ptr(doc), Csize_t(index - 1))
    ptr == C_NULL && throw(BoundsError(doc, index))
    owner = doc isa SerializableDocumentRef ? doc._owner : doc
    SerializableDocumentRef(ptr, owner)
end

"""
    find(doc, key::AbstractString) -> Union{SerializableDocumentRef,Nothing}

Return a borrowed view of the value for `key`, or `nothing` if not present.
"""
function find(doc::Union{SerializableDocument,SerializableDocumentRef}, key::AbstractString)
    ptr = GC.@preserve key ffi_sd_find(_sd_ptr(doc), Base.unsafe_convert(Cstring, key))
    ptr == C_NULL && return nothing
    owner = doc isa SerializableDocumentRef ? doc._owner : doc
    SerializableDocumentRef(ptr, owner)
end

"""
    has_key(doc, key::AbstractString) -> Bool

Return `true` if `doc` has a key named `key`.
"""
function has_key(
    doc::Union{SerializableDocument,SerializableDocumentRef},
    key::AbstractString,
)
    GC.@preserve key ffi_sd_contains(_sd_ptr(doc), Base.unsafe_convert(Cstring, key)) != 0
end

"""
    set!(doc::SerializableDocument, key::AbstractString, value::SerializableDocument)

Set the value for `key` in the object `doc`, transferring ownership of `value`.
`value` must not be used after this call.
"""
function set!(doc::SerializableDocument, key::AbstractString, value::SerializableDocument)
    val_ptr = _transfer_ownership(value)
    GC.@preserve key ffi_sd_set(doc._ptr, Base.unsafe_convert(Cstring, key), val_ptr)
    nothing
end

function set!(
    doc::SerializableDocumentRef,
    key::AbstractString,
    value::SerializableDocument,
)
    val_ptr = _transfer_ownership(value)
    GC.@preserve key ffi_sd_set(doc._ptr, Base.unsafe_convert(Cstring, key), val_ptr)
    nothing
end

function Base.:(==)(
    a::Union{SerializableDocument,SerializableDocumentRef},
    b::Union{SerializableDocument,SerializableDocumentRef},
)
    ffi_sd_equals(_sd_ptr(a), _sd_ptr(b)) != 0
end

function Base.copy(doc::SerializableDocument)
    ptr = ffi_sd_clone(doc._ptr)
    SerializableDocument(ptr)
end
