function ffi_gmod_path_query_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_path_query_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_gmod_path_query_match(query::Ptr{Cvoid}, path::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_path_query_match, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        query,
        path,
    )
end
