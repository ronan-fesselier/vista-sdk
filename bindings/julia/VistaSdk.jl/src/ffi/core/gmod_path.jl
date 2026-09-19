function ffi_gmod_path_free(path::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_path_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), path)
end

function ffi_gmod_path_from_short_path_version(item::Cstring, vis_version::Cstring)
    ccall(
        (:dnv_vista_sdk_gmod_path_from_short_path_version, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring, Cstring),
        item,
        vis_version,
    )
end

function ffi_gmod_path_from_short_path(
    item::Cstring,
    gmod::Ptr{Cvoid},
    locations::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_gmod_path_from_short_path, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring, Ptr{Cvoid}, Ptr{Cvoid}),
        item,
        gmod,
        locations,
    )
end

function ffi_gmod_path_from_short_path_with_errors(
    item::Cstring,
    gmod::Ptr{Cvoid},
    locations::Ptr{Cvoid},
    out_errors::Ref{Ptr{Cvoid}},
)
    ccall(
        (:dnv_vista_sdk_gmod_path_from_short_path_with_errors, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring, Ptr{Cvoid}, Ptr{Cvoid}, Ptr{Ptr{Cvoid}}),
        item,
        gmod,
        locations,
        out_errors,
    )
end

function ffi_gmod_path_from_full_path(
    full_path_str::Cstring,
    gmod::Ptr{Cvoid},
    locations::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_gmod_path_from_full_path, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring, Ptr{Cvoid}, Ptr{Cvoid}),
        full_path_str,
        gmod,
        locations,
    )
end

function ffi_gmod_path_from_full_path_with_errors(
    full_path_str::Cstring,
    gmod::Ptr{Cvoid},
    locations::Ptr{Cvoid},
    out_errors::Ref{Ptr{Cvoid}},
)
    ccall(
        (:dnv_vista_sdk_gmod_path_from_full_path_with_errors, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring, Ptr{Cvoid}, Ptr{Cvoid}, Ptr{Ptr{Cvoid}}),
        full_path_str,
        gmod,
        locations,
        out_errors,
    )
end

function ffi_gmod_path_version(path::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_path_version, VISTA_LIB), Cstring, (Ptr{Cvoid},), path)
end

function ffi_gmod_path_node(path::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_path_node, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), path)
end

function ffi_gmod_path_length(path::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_path_length, VISTA_LIB), Csize_t, (Ptr{Cvoid},), path)
end

function ffi_gmod_path_at(path::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_gmod_path_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        path,
        index,
    )
end

function ffi_gmod_path_equals(a::Ptr{Cvoid}, b::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_path_equals, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        a,
        b,
    )
end

function ffi_gmod_path_is_mappable(path::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_path_is_mappable, VISTA_LIB), Cint, (Ptr{Cvoid},), path)
end

function ffi_gmod_path_is_individualizable(path::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_path_is_individualizable, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        path,
    )
end

function ffi_gmod_path_without_locations(path::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_path_without_locations, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        path,
    )
end

function ffi_gmod_path_normal_assignment_name(path::Ptr{Cvoid}, node_depth::Csize_t)
    ccall(
        (:dnv_vista_sdk_gmod_path_normal_assignment_name, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid}, Csize_t),
        path,
        node_depth,
    )
end

function ffi_gmod_path_individualizable_set_count(path::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_path_individualizable_set_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        path,
    )
end

function ffi_gmod_path_individualizable_set_at(path::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_gmod_path_individualizable_set_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        path,
        index,
    )
end

function ffi_gmod_path_common_name_count(path::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_path_common_name_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        path,
    )
end

function ffi_gmod_path_common_name_depth_at(
    path::Ptr{Cvoid},
    index::Csize_t,
    out_depth::Ref{Csize_t},
)
    ccall(
        (:dnv_vista_sdk_gmod_path_common_name_depth_at, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Csize_t, Ptr{Csize_t}),
        path,
        index,
        out_depth,
    )
end

function ffi_gmod_path_common_name_at(path::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_gmod_path_common_name_at, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid}, Csize_t),
        path,
        index,
    )
end

function ffi_gmod_path_to_string(path::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_path_to_string, VISTA_LIB), Ptr{UInt8}, (Ptr{Cvoid},), path)
end

function ffi_gmod_path_to_full_path_string(path::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_path_to_full_path_string, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid},),
        path,
    )
end

function ffi_gmod_path_to_string_dump(path::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_path_to_string_dump, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid},),
        path,
    )
end
