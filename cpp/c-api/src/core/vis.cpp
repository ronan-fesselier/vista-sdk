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
        c::setLastErrorMessage("vis must not be null");
        return 0;
    }

    return toVis(vis)->versions().size();
}

const char* dnv_vista_sdk_vis_version_at(const dnv_vista_sdk_vis_t* vis, size_t index)
{
    if (vis == nullptr)
    {
        c::setLastErrorMessage("vis must not be null");
        return nullptr;
    }

    const auto versions = toVis(vis)->versions();
    if (index >= versions.size())
    {
        c::setLastErrorMessage("index out of range");
        return nullptr;
    }

    return VisVersions::toString(versions[index]).data();
}

const char* dnv_vista_sdk_vis_latest(const dnv_vista_sdk_vis_t* vis)
{
    if (vis == nullptr)
    {
        c::setLastErrorMessage("vis must not be null");
        return nullptr;
    }

    return VisVersions::toString(toVis(vis)->latest()).data();
}

const dnv_vista_sdk_gmod_t* dnv_vista_sdk_vis_gmod(const dnv_vista_sdk_vis_t* vis, const char* visVersion)
{
    if (vis == nullptr || visVersion == nullptr)
    {
        c::setLastErrorMessage("vis and visVersion must not be null");
        return nullptr;
    }

    const auto version = VisVersions::fromString(visVersion);
    if (!version.has_value())
    {
        c::setLastErrorMessage("unrecognized VIS version");
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_gmod_t*>(&toVis(vis)->gmod(*version));
}

const dnv_vista_sdk_codebooks_t* dnv_vista_sdk_vis_codebooks(const dnv_vista_sdk_vis_t* vis, const char* visVersion)
{
    if (vis == nullptr || visVersion == nullptr)
    {
        c::setLastErrorMessage("vis and visVersion must not be null");
        return nullptr;
    }

    const auto version = VisVersions::fromString(visVersion);
    if (!version.has_value())
    {
        c::setLastErrorMessage("unrecognized VIS version");
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_codebooks_t*>(&toVis(vis)->codebooks(*version));
}

const dnv_vista_sdk_locations_t* dnv_vista_sdk_vis_locations(const dnv_vista_sdk_vis_t* vis, const char* visVersion)
{
    if (vis == nullptr || visVersion == nullptr)
    {
        c::setLastErrorMessage("vis and visVersion must not be null");
        return nullptr;
    }

    const auto version = VisVersions::fromString(visVersion);
    if (!version.has_value())
    {
        c::setLastErrorMessage("unrecognized VIS version");
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
        c::setLastErrorMessage("vis, sourceVersion, sourceNode and targetVersion must not be null");
        return nullptr;
    }

    const auto source = VisVersions::fromString(sourceVersion);
    const auto target = VisVersions::fromString(targetVersion);
    if (!source.has_value() || !target.has_value())
    {
        c::setLastErrorMessage("unrecognized VIS version");
        return nullptr;
    }

    auto node = toVis(vis)->convertNode(*source, *toGmodNode(sourceNode), *target);
    if (!node.has_value())
    {
        c::setLastErrorMessage("conversion failed");
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
        c::setLastErrorMessage("vis, sourceVersion, sourcePath and targetVersion must not be null");
        return nullptr;
    }

    const auto source = VisVersions::fromString(sourceVersion);
    const auto target = VisVersions::fromString(targetVersion);
    if (!source.has_value() || !target.has_value())
    {
        c::setLastErrorMessage("unrecognized VIS version");
        return nullptr;
    }

    auto path = toVis(vis)->convertPath(*source, *toGmodPath(sourcePath), *target);
    if (!path.has_value())
    {
        c::setLastErrorMessage("conversion failed");
    }

    return fromGmodPath(std::move(path));
}

dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_vis_convert_local_id_builder(
    const dnv_vista_sdk_vis_t* vis, const dnv_vista_sdk_local_id_builder_t* sourceLocalId, const char* targetVersion)
{
    if (vis == nullptr || sourceLocalId == nullptr || targetVersion == nullptr)
    {
        c::setLastErrorMessage("vis, sourceLocalId and targetVersion must not be null");
        return nullptr;
    }

    const auto target = VisVersions::fromString(targetVersion);
    if (!target.has_value())
    {
        c::setLastErrorMessage("unrecognized VIS version");
        return nullptr;
    }

    try
    {
        auto localId = toVis(vis)->convertLocalId(*toLocalIdBuilder(sourceLocalId), *target);
        if (!localId.has_value())
        {
            c::setLastErrorMessage("conversion failed");
            return nullptr;
        }

        return fromLocalIdBuilder(std::move(*localId));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

dnv_vista_sdk_local_id_t* dnv_vista_sdk_vis_convert_local_id(
    const dnv_vista_sdk_vis_t* vis, const dnv_vista_sdk_local_id_t* sourceLocalId, const char* targetVersion)
{
    if (vis == nullptr || sourceLocalId == nullptr || targetVersion == nullptr)
    {
        c::setLastErrorMessage("vis, sourceLocalId and targetVersion must not be null");
        return nullptr;
    }

    const auto target = VisVersions::fromString(targetVersion);
    if (!target.has_value())
    {
        c::setLastErrorMessage("unrecognized VIS version");
        return nullptr;
    }

    try
    {
        auto localId = toVis(vis)->convertLocalId(*toLocalId(sourceLocalId), *target);
        if (!localId.has_value())
        {
            c::setLastErrorMessage("conversion failed");
            return nullptr;
        }

        return fromLocalId(std::move(localId));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}
