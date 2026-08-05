#include "dnv/vista/sdk/c/core/vis.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromGmodNode;
using dnv::vista::sdk::c::fromGmodPath;
using dnv::vista::sdk::c::fromLocalId;
using dnv::vista::sdk::c::fromLocalIdBuilder;
using dnv::vista::sdk::c::toGmodNode;
using dnv::vista::sdk::c::toGmodPath;
using dnv::vista::sdk::c::toLocalId;
using dnv::vista::sdk::c::toLocalIdBuilder;
using dnv::vista::sdk::c::toVis;

const dnv_vista_sdk_vis_t* dnv_vista_sdk_vis_instance(void)
{
    return reinterpret_cast<const dnv_vista_sdk_vis_t*>(&VIS::instance());
}

size_t dnv_vista_sdk_vis_version_count(const dnv_vista_sdk_vis_t* vis)
{
    if (vis == nullptr)
    {
        c::setLastError("vis must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toVis(vis)->versions().size();
}

const char* dnv_vista_sdk_vis_version_at(const dnv_vista_sdk_vis_t* vis, size_t index)
{
    if (vis == nullptr)
    {
        c::setLastError("vis must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto versions = toVis(vis)->versions();
    if (index >= versions.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return VisVersions::toString(versions[index]).data();
}

const char* dnv_vista_sdk_vis_latest(const dnv_vista_sdk_vis_t* vis)
{
    if (vis == nullptr)
    {
        c::setLastError("vis must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return VisVersions::toString(toVis(vis)->latest()).data();
}

const dnv_vista_sdk_gmod_t* dnv_vista_sdk_vis_gmod(const dnv_vista_sdk_vis_t* vis, const char* visVersion)
{
    if (vis == nullptr || visVersion == nullptr)
    {
        c::setLastError("vis and visVersion must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto version = VisVersions::fromString(visVersion);
    if (!version.has_value())
    {
        c::setLastError("unrecognized VIS version", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_gmod_t*>(&toVis(vis)->gmod(*version));
}

const dnv_vista_sdk_codebooks_t* dnv_vista_sdk_vis_codebooks(const dnv_vista_sdk_vis_t* vis, const char* visVersion)
{
    if (vis == nullptr || visVersion == nullptr)
    {
        c::setLastError("vis and visVersion must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto version = VisVersions::fromString(visVersion);
    if (!version.has_value())
    {
        c::setLastError("unrecognized VIS version", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_codebooks_t*>(&toVis(vis)->codebooks(*version));
}

const dnv_vista_sdk_locations_t* dnv_vista_sdk_vis_locations(const dnv_vista_sdk_vis_t* vis, const char* visVersion)
{
    if (vis == nullptr || visVersion == nullptr)
    {
        c::setLastError("vis and visVersion must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto version = VisVersions::fromString(visVersion);
    if (!version.has_value())
    {
        c::setLastError("unrecognized VIS version", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_locations_t*>(&toVis(vis)->locations(*version));
}

dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_vis_convert_node(
    const dnv_vista_sdk_vis_t* vis,
    const char* sourceVersion,
    const dnv_vista_sdk_gmod_node_t* sourceNode,
    const char* targetVersion)
{
    if (vis == nullptr || sourceVersion == nullptr || sourceNode == nullptr || targetVersion == nullptr)
    {
        c::setLastError(
            "vis, sourceVersion, sourceNode and targetVersion must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto source = VisVersions::fromString(sourceVersion);
    const auto target = VisVersions::fromString(targetVersion);
    if (!source.has_value() || !target.has_value())
    {
        c::setLastError("unrecognized VIS version", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    auto node = toVis(vis)->convertNode(*source, *toGmodNode(sourceNode), *target);
    if (!node.has_value())
    {
        c::setLastError("conversion failed", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
    }

    return fromGmodNode(std::move(node));
}

dnv_vista_sdk_gmod_path_t* dnv_vista_sdk_vis_convert_path(
    const dnv_vista_sdk_vis_t* vis,
    const char* sourceVersion,
    const dnv_vista_sdk_gmod_path_t* sourcePath,
    const char* targetVersion)
{
    if (vis == nullptr || sourceVersion == nullptr || sourcePath == nullptr || targetVersion == nullptr)
    {
        c::setLastError(
            "vis, sourceVersion, sourcePath and targetVersion must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto source = VisVersions::fromString(sourceVersion);
    const auto target = VisVersions::fromString(targetVersion);
    if (!source.has_value() || !target.has_value())
    {
        c::setLastError("unrecognized VIS version", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    auto path = toVis(vis)->convertPath(*source, *toGmodPath(sourcePath), *target);
    if (!path.has_value())
    {
        c::setLastError("conversion failed", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
    }

    return fromGmodPath(std::move(path));
}

dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_vis_convert_local_id_builder(
    const dnv_vista_sdk_vis_t* vis, const dnv_vista_sdk_local_id_builder_t* sourceLocalId, const char* targetVersion)
{
    if (vis == nullptr || sourceLocalId == nullptr || targetVersion == nullptr)
    {
        c::setLastError("vis, sourceLocalId and targetVersion must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto target = VisVersions::fromString(targetVersion);
    if (!target.has_value())
    {
        c::setLastError("unrecognized VIS version", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_local_id_builder_t*>([&]() -> dnv_vista_sdk_local_id_builder_t* {
        auto localId = toVis(vis)->convertLocalId(*toLocalIdBuilder(sourceLocalId), *target);
        if (!localId.has_value())
        {
            c::setLastError("conversion failed", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
            return nullptr;
        }

        return fromLocalIdBuilder(std::move(*localId));
    });
}

dnv_vista_sdk_local_id_t* dnv_vista_sdk_vis_convert_local_id(
    const dnv_vista_sdk_vis_t* vis, const dnv_vista_sdk_local_id_t* sourceLocalId, const char* targetVersion)
{
    if (vis == nullptr || sourceLocalId == nullptr || targetVersion == nullptr)
    {
        c::setLastError("vis, sourceLocalId and targetVersion must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto target = VisVersions::fromString(targetVersion);
    if (!target.has_value())
    {
        c::setLastError("unrecognized VIS version", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_local_id_t*>([&]() -> dnv_vista_sdk_local_id_t* {
        auto localId = toVis(vis)->convertLocalId(*toLocalId(sourceLocalId), *target);
        if (!localId.has_value())
        {
            c::setLastError("conversion failed", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
            return nullptr;
        }

        return fromLocalId(std::move(localId));
    });
}
