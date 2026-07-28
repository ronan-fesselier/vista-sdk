#include "error_internal.h"

namespace
{
    thread_local std::string g_lastErrorMessage;
    thread_local dnv_vista_sdk_error_kind_t g_lastErrorKind{ DNV_VISTA_SDK_ERROR_NONE };
} // namespace

namespace dnv::vista::sdk::c
{
    void setLastError(std::string_view message, dnv_vista_sdk_error_kind_t kind)
    {
        g_lastErrorMessage.assign(message);
        g_lastErrorKind = kind;
    }
} // namespace dnv::vista::sdk::c

const char* dnv_vista_sdk_last_error_message(void)
{
    return g_lastErrorMessage.c_str();
}

dnv_vista_sdk_error_kind_t dnv_vista_sdk_last_error_kind(void)
{
    return g_lastErrorKind;
}

void dnv_vista_sdk_clear_error(void)
{
    g_lastErrorMessage.clear();
    g_lastErrorKind = DNV_VISTA_SDK_ERROR_NONE;
}
