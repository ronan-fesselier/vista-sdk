#pragma once

#include "dnv/vista/sdk/c/core/vis.h"

#include <dnv/VistaSDK.h>

namespace dnv::vista::sdk::c
{
    inline const VIS* toVis(const dnv_vista_sdk_vis_t* vis)
    {
        return reinterpret_cast<const VIS*>(vis);
    }
} // namespace dnv::vista::sdk::c
