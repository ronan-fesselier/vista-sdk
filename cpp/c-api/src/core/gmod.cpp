#include "dnv/vista/sdk/c/core/gmod.h"

#include "../cast_internal.h"
#include "../error_internal.h"

#include <iterator>

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromGmodNode;
using dnv::vista::sdk::c::toGmod;

const char* dnv_vista_sdk_gmod_version(const dnv_vista_sdk_gmod_t* gmod)
{
    if (gmod == nullptr)
    {
        c::setLastError("gmod must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return VisVersions::toString(toGmod(gmod)->version()).data();
}

const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_root_node(const dnv_vista_sdk_gmod_t* gmod)
{
    if (gmod == nullptr)
    {
        c::setLastError("gmod must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromGmodNode(&toGmod(gmod)->rootNode());
}

const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_get_node(const dnv_vista_sdk_gmod_t* gmod, const char* code)
{
    if (gmod == nullptr || code == nullptr)
    {
        c::setLastError("gmod and code must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto node = toGmod(gmod)->node(code);
    if (!node.has_value())
    {
        c::setLastError("no node with the given code", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return fromGmodNode(*node);
}

size_t dnv_vista_sdk_gmod_node_count(const dnv_vista_sdk_gmod_t* gmod)
{
    if (gmod == nullptr)
    {
        c::setLastError("gmod must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return static_cast<size_t>(std::distance(toGmod(gmod)->begin(), toGmod(gmod)->end()));
}

const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_node_at(const dnv_vista_sdk_gmod_t* gmod, size_t index)
{
    if (gmod == nullptr)
    {
        c::setLastError("gmod must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto* self = toGmod(gmod);
    auto it = self->begin();
    const auto end = self->end();

    for (size_t i = 0; it != end; ++it, ++i)
    {
        if (i == index)
        {
            return fromGmodNode(&it->second);
        }
    }

    c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
    return nullptr;
}
