struct MetadataTagRef
    _ptr::Ptr{Cvoid}
end

"""
    MetadataTag

A metadata tag combining a [`CodebookName`](@ref) and a string value.

Tags appear as `"prefix-value"` (standard) or `"prefix~value"` (custom)
in VIS Local ID string format.
"""
mutable struct MetadataTag
    _ref::MetadataTagRef

    function MetadataTag(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("MetadataTag: NULL pointer")
        t = new(MetadataTagRef(ptr))
        finalizer(t) do x
            ffi_metadata_tag_free(x._ref._ptr)
            x._ref = MetadataTagRef(C_NULL)
        end
        t
    end
end

function _ptr(t::MetadataTagRef)
    t._ptr
end

function _ptr(t::MetadataTag)
    t._ref._ptr
end

function name(t::MetadataTagRef)
    CodebookName(ffi_metadata_tag_name(t._ptr))
end

function name(t::MetadataTag)
    name(t._ref)
end

function value(t::MetadataTagRef)
    ptr = ffi_metadata_tag_value(t._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_metadata_tag_value returned NULL")
    unsafe_string(ptr)
end

function value(t::MetadataTag)
    value(t._ref)
end

function is_custom(t::MetadataTagRef)
    ffi_metadata_tag_is_custom(t._ptr) != 0
end

function is_custom(t::MetadataTag)
    is_custom(t._ref)
end

function Base.show(io::IO, t::MetadataTagRef)
    ptr = ffi_metadata_tag_to_string(t._ptr)
    ptr == C_NULL && (print(io, "<MetadataTag NULL>"); return)
    p = Ptr{UInt8}(ptr)
    s = unsafe_string(p)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), p)
    print(io, s)
end

function Base.show(io::IO, t::MetadataTag)
    show(io, t._ref)
end
