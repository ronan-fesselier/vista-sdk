#include "dnv/vista/sdk/c/core/local_id_mqtt.h"

#include "local_id_internal.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromGmodPathRef;
using dnv::vista::sdk::c::fromLocalIdBuilderRef;
using dnv::vista::sdk::c::fromTagRef;
using dnv::vista::sdk::c::selectMetadataTag;
using dnv::vista::sdk::c::toCodebookName;
using dnv::vista::sdk::c::toLocalIdBuilder;
using dnv::vista::sdk::c::toLocalIdMqtt;
using dnv::vista::sdk::c::toOwnedCString;

dnv_vista_sdk_local_id_mqtt_t* dnv_vista_sdk_local_id_mqtt_create(const dnv_vista_sdk_local_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    try
    {
        return reinterpret_cast<dnv_vista_sdk_local_id_mqtt_t*>(new mqtt::LocalId{ *toLocalIdBuilder(builder) });
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

void dnv_vista_sdk_local_id_mqtt_free(dnv_vista_sdk_local_id_mqtt_t* localId)
{
    delete reinterpret_cast<mqtt::LocalId*>(localId);
}

const char* dnv_vista_sdk_local_id_mqtt_version(const dnv_vista_sdk_local_id_mqtt_t* localId)
{
    if (localId == nullptr)
    {
        c::setLastErrorMessage("localId must not be null");
        return nullptr;
    }

    return VisVersions::toString(toLocalIdMqtt(localId)->version()).data();
}

const dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_mqtt_primary_item(const dnv_vista_sdk_local_id_mqtt_t* localId)
{
    if (localId == nullptr)
    {
        c::setLastErrorMessage("localId must not be null");
        return nullptr;
    }

    return fromGmodPathRef(toLocalIdMqtt(localId)->primaryItem());
}

const dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_local_id_mqtt_secondary_item(
    const dnv_vista_sdk_local_id_mqtt_t* localId)
{
    if (localId == nullptr)
    {
        c::setLastErrorMessage("localId must not be null");
        return nullptr;
    }

    const auto& path = toLocalIdMqtt(localId)->secondaryItem();
    if (!path.has_value())
    {
        return nullptr;
    }

    return fromGmodPathRef(*path);
}

const dnv_vista_sdk_metadata_tag_t* dnv_vista_sdk_local_id_mqtt_metadata_tag(
    const dnv_vista_sdk_local_id_mqtt_t* localId, dnv_vista_sdk_codebook_name_t name)
{
    if (localId == nullptr)
    {
        c::setLastErrorMessage("localId must not be null");
        return nullptr;
    }

    const auto* tag = selectMetadataTag(*toLocalIdMqtt(localId), toCodebookName(name));
    if (tag == nullptr)
    {
        c::setLastErrorMessage("invalid codebook name");
        return nullptr;
    }

    if (!tag->has_value())
    {
        return nullptr;
    }

    return fromTagRef(**tag);
}

const dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_local_id_mqtt_builder(
    const dnv_vista_sdk_local_id_mqtt_t* localId)
{
    if (localId == nullptr)
    {
        c::setLastErrorMessage("localId must not be null");
        return nullptr;
    }

    return fromLocalIdBuilderRef(toLocalIdMqtt(localId)->builder());
}

char* dnv_vista_sdk_local_id_mqtt_to_string(const dnv_vista_sdk_local_id_mqtt_t* localId)
{
    if (localId == nullptr)
    {
        c::setLastErrorMessage("localId must not be null");
        return nullptr;
    }

    return toOwnedCString(toLocalIdMqtt(localId)->toString());
}

void dnv_vista_sdk_local_id_mqtt_string_free(char* str)
{
    delete[] str;
}
