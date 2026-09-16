function ffi_metadata_tag_free(tag::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_metadata_tag_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), tag)
end

function ffi_metadata_tag_name(tag::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_metadata_tag_name, VISTA_LIB), Cint, (Ptr{Cvoid},), tag)
end

function ffi_metadata_tag_value(tag::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_metadata_tag_value, VISTA_LIB), Cstring, (Ptr{Cvoid},), tag)
end

function ffi_metadata_tag_prefix(tag::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_metadata_tag_prefix, VISTA_LIB), Cchar, (Ptr{Cvoid},), tag)
end

function ffi_metadata_tag_is_custom(tag::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_metadata_tag_is_custom, VISTA_LIB), Cint, (Ptr{Cvoid},), tag)
end

function ffi_metadata_tag_to_string(tag::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_metadata_tag_to_string, VISTA_LIB),
        Ptr{Cchar},
        (Ptr{Cvoid},),
        tag,
    )
end
