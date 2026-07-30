#include "dnv/vista/sdk/c/core/parsing_errors.h"

#include "../cast_internal.h"
#include "../error_internal.h"

#include <iterator>

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::toErrors;
using dnv::vista::sdk::c::toOwnedCString;

void dnv_vista_sdk_parsing_errors_free(dnv_vista_sdk_parsing_errors_t* errors)
{
    delete reinterpret_cast<ParsingErrors*>(errors);
}

size_t dnv_vista_sdk_parsing_errors_count(const dnv_vista_sdk_parsing_errors_t* errors)
{
    if (errors == nullptr)
    {
        c::setLastErrorMessage("errors must not be null");
        return 0;
    }

    return toErrors(errors)->count();
}

int dnv_vista_sdk_parsing_errors_has_errors(const dnv_vista_sdk_parsing_errors_t* errors)
{
    if (errors == nullptr)
    {
        c::setLastErrorMessage("errors must not be null");
        return 0;
    }

    return toErrors(errors)->hasErrors() ? 1 : 0;
}

int dnv_vista_sdk_parsing_errors_has_error_type(const dnv_vista_sdk_parsing_errors_t* errors, const char* type)
{
    if (errors == nullptr || type == nullptr)
    {
        c::setLastErrorMessage("errors and type must not be null");
        return 0;
    }

    return toErrors(errors)->hasErrorType(type) ? 1 : 0;
}

const char* dnv_vista_sdk_parsing_errors_type_at(const dnv_vista_sdk_parsing_errors_t* errors, size_t index)
{
    if (errors == nullptr)
    {
        c::setLastErrorMessage("errors must not be null");
        return nullptr;
    }

    const auto* self = toErrors(errors);
    if (index >= self->count())
    {
        c::setLastErrorMessage("index out of range");
        return nullptr;
    }

    auto it = self->begin();
    std::advance(it, static_cast<std::ptrdiff_t>(index));
    return it->type.c_str();
}

const char* dnv_vista_sdk_parsing_errors_message_at(const dnv_vista_sdk_parsing_errors_t* errors, size_t index)
{
    if (errors == nullptr)
    {
        c::setLastErrorMessage("errors must not be null");
        return nullptr;
    }

    const auto* self = toErrors(errors);
    if (index >= self->count())
    {
        c::setLastErrorMessage("index out of range");
        return nullptr;
    }

    auto it = self->begin();
    std::advance(it, static_cast<std::ptrdiff_t>(index));
    return it->message.c_str();
}

char* dnv_vista_sdk_parsing_errors_to_string(const dnv_vista_sdk_parsing_errors_t* errors)
{
    if (errors == nullptr)
    {
        c::setLastErrorMessage("errors must not be null");
        return nullptr;
    }

    return toOwnedCString(toErrors(errors)->toString());
}

void dnv_vista_sdk_parsing_errors_string_free(char* str)
{
    delete[] str;
}
