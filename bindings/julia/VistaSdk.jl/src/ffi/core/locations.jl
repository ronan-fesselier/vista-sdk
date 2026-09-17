function ffi_locations_version(locations::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_locations_version, VISTA_LIB), Cstring, (Ptr{Cvoid},), locations)
end

function ffi_locations_relative_location_count(locations::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_locations_relative_location_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        locations,
    )
end

function ffi_locations_relative_location_at(locations::Ptr{Cvoid}, index::Integer)
    ccall(
        (:dnv_vista_sdk_locations_relative_location_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        locations,
        index,
    )
end

function ffi_locations_group_count(locations::Ptr{Cvoid}, group::LocationGroup)
    ccall(
        (:dnv_vista_sdk_locations_group_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid}, Cint),
        locations,
        Cint(Integer(group)),
    )
end

function ffi_locations_group_at(locations::Ptr{Cvoid}, group::LocationGroup, index::Integer)
    ccall(
        (:dnv_vista_sdk_locations_group_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cint, Csize_t),
        locations,
        Cint(Integer(group)),
        index,
    )
end

function ffi_locations_from_string(locations::Ptr{Cvoid}, location_str::Cstring)
    ccall(
        (:dnv_vista_sdk_locations_from_string, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring),
        locations,
        location_str,
    )
end

function ffi_locations_from_string_with_errors(
    locations::Ptr{Cvoid},
    location_str::Cstring,
    out_errors::Ref{Ptr{Cvoid}},
)
    ccall(
        (:dnv_vista_sdk_locations_from_string_with_errors, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring, Ptr{Ptr{Cvoid}}),
        locations,
        location_str,
        out_errors,
    )
end
