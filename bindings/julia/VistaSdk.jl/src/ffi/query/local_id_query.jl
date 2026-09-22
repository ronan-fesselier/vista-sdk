function ffi_local_id_query_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_query_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_local_id_query_match(query::Ptr{Cvoid}, local_id::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_query_match, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        query,
        local_id,
    )
end

function ffi_local_id_query_match_string(query::Ptr{Cvoid}, local_id_str::Cstring)
    ccall(
        (:dnv_vista_sdk_local_id_query_match_string, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Cstring),
        query,
        local_id_str,
    )
end
