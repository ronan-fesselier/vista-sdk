#pragma once

#include "dnv/vista/sdk/c/error.h"

#include <string>
#include <string_view>

namespace dnv::vista::sdk::c
{
    void setLastErrorMessage(std::string_view message);
} // namespace dnv::vista::sdk::c
