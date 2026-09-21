function ffi_metadata_tags_query_builder_create()
    ccall((:dnv_vista_sdk_metadata_tags_query_builder_create, VISTA_LIB), Ptr{Cvoid}, ())
end

function ffi_metadata_tags_query_builder_from(local_id::Ptr{Cvoid}, allow_other_tags::Cint)
    ccall(
        (:dnv_vista_sdk_metadata_tags_query_builder_from, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cint),
        local_id,
        allow_other_tags,
    )
end

function ffi_metadata_tags_query_builder_free(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_metadata_tags_query_builder_free, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_metadata_tags_query_builder_with_tag(
    builder::Ptr{Cvoid},
    name::Cint,
    value::Cstring,
)
    ccall(
        (:dnv_vista_sdk_metadata_tags_query_builder_with_tag, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cint, Cstring),
        builder,
        name,
        value,
    )
end

function ffi_metadata_tags_query_builder_with_metadata_tag(
    builder::Ptr{Cvoid},
    tag::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_metadata_tags_query_builder_with_metadata_tag, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        builder,
        tag,
    )
end

function ffi_metadata_tags_query_builder_with_allow_other_tags(
    builder::Ptr{Cvoid},
    allow_others::Cint,
)
    ccall(
        (:dnv_vista_sdk_metadata_tags_query_builder_with_allow_other_tags, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cint),
        builder,
        allow_others,
    )
end

function ffi_metadata_tags_query_builder_build(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_metadata_tags_query_builder_build, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end
