function ffi_location_builder_create(locations::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_location_builder_create, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        locations,
    )
end

function ffi_location_builder_free(builder::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_location_builder_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), builder)
end

function ffi_location_builder_version(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_location_builder_version, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_location_builder_number(builder::Ptr{Cvoid}, out::Ref{Cint})
    ccall(
        (:dnv_vista_sdk_location_builder_number, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cint}),
        builder,
        out,
    )
end

function ffi_location_builder_side(builder::Ptr{Cvoid}, out::Ref{Cchar})
    ccall(
        (:dnv_vista_sdk_location_builder_side, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cchar}),
        builder,
        out,
    )
end

function ffi_location_builder_vertical(builder::Ptr{Cvoid}, out::Ref{Cchar})
    ccall(
        (:dnv_vista_sdk_location_builder_vertical, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cchar}),
        builder,
        out,
    )
end

function ffi_location_builder_transverse(builder::Ptr{Cvoid}, out::Ref{Cchar})
    ccall(
        (:dnv_vista_sdk_location_builder_transverse, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cchar}),
        builder,
        out,
    )
end

function ffi_location_builder_longitudinal(builder::Ptr{Cvoid}, out::Ref{Cchar})
    ccall(
        (:dnv_vista_sdk_location_builder_longitudinal, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cchar}),
        builder,
        out,
    )
end

function ffi_location_builder_with_number(builder::Ptr{Cvoid}, number::Cint)
    ccall(
        (:dnv_vista_sdk_location_builder_with_number, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cint),
        builder,
        number,
    )
end

function ffi_location_builder_without_number(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_location_builder_without_number, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_location_builder_with_side(builder::Ptr{Cvoid}, side::Cchar)
    ccall(
        (:dnv_vista_sdk_location_builder_with_side, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cchar),
        builder,
        side,
    )
end

function ffi_location_builder_without_side(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_location_builder_without_side, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_location_builder_with_vertical(builder::Ptr{Cvoid}, vertical::Cchar)
    ccall(
        (:dnv_vista_sdk_location_builder_with_vertical, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cchar),
        builder,
        vertical,
    )
end

function ffi_location_builder_without_vertical(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_location_builder_without_vertical, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_location_builder_with_transverse(builder::Ptr{Cvoid}, transverse::Cchar)
    ccall(
        (:dnv_vista_sdk_location_builder_with_transverse, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cchar),
        builder,
        transverse,
    )
end

function ffi_location_builder_without_transverse(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_location_builder_without_transverse, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_location_builder_with_longitudinal(builder::Ptr{Cvoid}, longitudinal::Cchar)
    ccall(
        (:dnv_vista_sdk_location_builder_with_longitudinal, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cchar),
        builder,
        longitudinal,
    )
end

function ffi_location_builder_without_longitudinal(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_location_builder_without_longitudinal, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_location_builder_with_code(builder::Ptr{Cvoid}, code::Cchar)
    ccall(
        (:dnv_vista_sdk_location_builder_with_code, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cchar),
        builder,
        code,
    )
end

function ffi_location_builder_with_location(builder::Ptr{Cvoid}, location::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_location_builder_with_location, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        builder,
        location,
    )
end

function ffi_location_builder_without_value(builder::Ptr{Cvoid}, group::LocationGroup)
    ccall(
        (:dnv_vista_sdk_location_builder_without_value, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cint),
        builder,
        Cint(Integer(group)),
    )
end

function ffi_location_builder_build(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_location_builder_build, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_location_builder_to_string(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_location_builder_to_string, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end
