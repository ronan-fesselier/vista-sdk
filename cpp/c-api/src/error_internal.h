#pragma once

#include "dnv/vista/sdk/c/error.h"

#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

namespace dnv::vista::sdk::c
{
    void setLastError(std::string_view message, dnv_vista_sdk_error_kind_t kind);

    template <typename R, typename F>
    R cApiTryCatch(F&& f)
    {
        try
        {
            return f();
        }
        catch (const std::invalid_argument& e)
        {
            setLastError(e.what(), DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        }
        catch (const std::out_of_range& e)
        {
            setLastError(e.what(), DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        }
        catch (const std::domain_error& e)
        {
            setLastError(e.what(), DNV_VISTA_SDK_ERROR_DOMAIN);
        }
        catch (const std::overflow_error& e)
        {
            setLastError(e.what(), DNV_VISTA_SDK_ERROR_OVERFLOW);
        }
        catch (const std::exception& e)
        {
            setLastError(e.what(), DNV_VISTA_SDK_ERROR_RUNTIME);
        }
        if constexpr (!std::is_void_v<R>)
        {
            return R{};
        }
    }
} // namespace dnv::vista::sdk::c
