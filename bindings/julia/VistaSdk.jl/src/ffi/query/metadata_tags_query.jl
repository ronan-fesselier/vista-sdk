function ffi_metadata_tags_query_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_metadata_tags_query_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_metadata_tags_query_match(query::Ptr{Cvoid}, local_id::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_metadata_tags_query_match, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        query,
        local_id,
    )
end

function ffi_metadata_tags_query_builder(query::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_metadata_tags_query_builder, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        query,
    )
end
