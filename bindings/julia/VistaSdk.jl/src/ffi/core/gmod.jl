function ffi_gmod_version(gmod::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_version, VISTA_LIB), Cstring, (Ptr{Cvoid},), gmod)
end

function ffi_gmod_root_node(gmod::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_root_node, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), gmod)
end

function ffi_gmod_get_node(gmod::Ptr{Cvoid}, code::Cstring)
    ccall(
        (:dnv_vista_sdk_gmod_get_node, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring),
        gmod,
        code,
    )
end

function ffi_gmod_node_count(gmod::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_count, VISTA_LIB), Csize_t, (Ptr{Cvoid},), gmod)
end

function ffi_gmod_node_at(gmod::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_gmod_node_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        gmod,
        index,
    )
end

function ffi_gmod_traverse(
    gmod::Ptr{Cvoid},
    handler::Ptr{Cvoid},
    max_occurrence::Cint,
    userdata::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_gmod_traverse, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}, Cint, Ptr{Cvoid}),
        gmod,
        handler,
        max_occurrence,
        userdata,
    )
end
