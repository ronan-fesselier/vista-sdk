#include "dnv/vista/sdk/c/core/vis.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
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
