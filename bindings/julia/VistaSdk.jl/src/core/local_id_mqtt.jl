"""
    MqttLocalId

An MQTT-formatted local ID derived from a [`LocalIdBuilder`](@ref). Freed when garbage-collected.
"""
mutable struct MqttLocalId
    _ptr::Ptr{Cvoid}
    function MqttLocalId(ptr::Ptr{Cvoid})
        ptr == C_NULL && error("MqttLocalId: NULL pointer")
        t = new(ptr)
        finalizer(t) do x
            ffi_local_id_mqtt_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    create(::Type{MqttLocalId}, b::Union{LocalIdBuilderRef,LocalIdBuilder}) -> Union{MqttLocalId,Nothing}

Build an MQTT local ID from `b`, or return `nothing` if the builder state is invalid.
"""
function create(::Base.Type{MqttLocalId}, b::LocalIdBuilderRef)
    ptr = ffi_local_id_mqtt_create(b._ptr)
    ptr == C_NULL ? nothing : MqttLocalId(ptr)
end

function create(::Base.Type{MqttLocalId}, b::LocalIdBuilder)
    create(MqttLocalId, b._ref)
end

"""
    version(m::MqttLocalId) -> VisVersion

Return the VIS version of the MQTT local ID.
"""
function version(m::MqttLocalId)
    ptr = ffi_local_id_mqtt_version(m._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_local_id_mqtt_version returned NULL")
    Base.parse(VisVersion, unsafe_string(ptr))
end

"""
    primary_item(m::MqttLocalId) -> GmodPathRef

Return a borrowed reference to the primary item path.
"""
function primary_item(m::MqttLocalId)
    ptr = ffi_local_id_mqtt_primary_item(m._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_local_id_mqtt_primary_item returned NULL")
    GmodPathRef(ptr, m)
end

"""
    secondary_item(m::MqttLocalId) -> Union{GmodPathRef,Nothing}

Return a borrowed reference to the secondary item path, or `nothing` if absent.
"""
function secondary_item(m::MqttLocalId)
    ptr = ffi_local_id_mqtt_secondary_item(m._ptr)
    ptr == C_NULL ? nothing : GmodPathRef(ptr, m)
end

"""
    metadata_tag(m::MqttLocalId, cb_name::CodebookName) -> Union{MetadataTagRef,Nothing}

Return the metadata tag for the given codebook, or `nothing` if absent.
"""
function metadata_tag(m::MqttLocalId, cb_name::CodebookName)
    ptr = ffi_local_id_mqtt_metadata_tag(m._ptr, Cint(Integer(cb_name)))
    ptr == C_NULL ? nothing : MetadataTagRef(ptr)
end

"""
    quantity(m) / content(m) / calculation(m) / state(m) / command(m) / tag_type(m) / position(m) / detail(m)

Convenience accessors for each metadata tag codebook. Return `nothing` if the tag is absent.
"""
quantity(m::MqttLocalId) = metadata_tag(m, Quantity)
content(m::MqttLocalId) = metadata_tag(m, Content)
calculation(m::MqttLocalId) = metadata_tag(m, Calculation)
state(m::MqttLocalId) = metadata_tag(m, State)
command(m::MqttLocalId) = metadata_tag(m, Command)
tag_type(m::MqttLocalId) = metadata_tag(m, Type)
position(m::MqttLocalId) = metadata_tag(m, Position)
detail(m::MqttLocalId) = metadata_tag(m, Detail)

"""
    builder(m::MqttLocalId) -> LocalIdBuilderRef

Return a borrowed builder pre-populated from this MQTT local ID.
"""
function builder(m::MqttLocalId)
    ptr = ffi_local_id_mqtt_builder(m._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_local_id_mqtt_builder returned NULL")
    LocalIdBuilderRef(ptr, m)
end

function Base.:(==)(a::MqttLocalId, b::MqttLocalId)
    builder(a) == builder(b)
end

function Base.show(io::IO, m::MqttLocalId)
    ptr = ffi_local_id_mqtt_to_string(m._ptr)
    ptr == C_NULL && (print(io, "<MqttLocalId NULL>"); return)
    s = unsafe_string(ptr)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), ptr)
    print(io, s)
end
