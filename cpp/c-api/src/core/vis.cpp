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
