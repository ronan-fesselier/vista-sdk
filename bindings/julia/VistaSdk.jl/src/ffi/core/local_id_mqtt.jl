function ffi_local_id_mqtt_create(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_mqtt_create, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_local_id_mqtt_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_mqtt_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_local_id_mqtt_version(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_mqtt_version, VISTA_LIB), Cstring, (Ptr{Cvoid},), ptr)
end

function ffi_local_id_mqtt_primary_item(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_mqtt_primary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_local_id_mqtt_secondary_item(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_mqtt_secondary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_local_id_mqtt_metadata_tag(ptr::Ptr{Cvoid}, name::Cint)
    ccall(
        (:dnv_vista_sdk_local_id_mqtt_metadata_tag, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cint),
        ptr,
        name,
    )
end

function ffi_local_id_mqtt_builder(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_mqtt_builder, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), ptr)
end

function ffi_local_id_mqtt_to_string(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_mqtt_to_string, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid},),
        ptr,
    )
end
