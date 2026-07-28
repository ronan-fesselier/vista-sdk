#include "error_internal.h"

namespace
{
    thread_local std::string g_lastErrorMessage;
} // namespace

namespace dnv::vista::sdk::c
{
    void setLastErrorMessage(std::string_view message)
    {
        g_lastErrorMessage.assign(message);
    }
} // namespace dnv::vista::sdk::c

const char* dnv_vista_sdk_last_error_message(void)
{
    return g_lastErrorMessage.c_str();
}
