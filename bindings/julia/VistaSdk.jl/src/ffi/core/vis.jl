function ffi_vis_instance()
    ccall((:dnv_vista_sdk_vis_instance, VISTA_LIB), Ptr{Cvoid}, ())
end

function ffi_vis_version_count(vis::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_vis_version_count, VISTA_LIB), Csize_t, (Ptr{Cvoid},), vis)
end

function ffi_vis_version_at(vis::Ptr{Cvoid}, index::Integer)
    ccall(
        (:dnv_vista_sdk_vis_version_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        vis,
        index,
    )
end

function ffi_vis_latest(vis::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_vis_latest, VISTA_LIB), Cstring, (Ptr{Cvoid},), vis)
end

function ffi_vis_locations(vis::Ptr{Cvoid}, version::Cstring)
    ccall(
        (:dnv_vista_sdk_vis_locations, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring),
        vis,
        version,
    )
end

function ffi_vis_codebooks(vis::Ptr{Cvoid}, version::Cstring)
    ccall(
        (:dnv_vista_sdk_vis_codebooks, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring),
        vis,
        version,
    )
end
