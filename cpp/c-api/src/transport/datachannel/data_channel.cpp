#include "dnv/vista/sdk/c/transport/datachannel/data_channel.h"

#include "../../cast_internal.h"
#include "../../error_internal.h"

#include <vector>

using namespace dnv::vista::sdk;
using namespace dnv::vista::sdk::c;

/*=====================================================================
 * Restriction
 *===================================================================*/

dnv_vista_sdk_dcl_restriction_t* dnv_vista_sdk_dcl_restriction_create(void)
{
    return fromRestriction(dcl::Restriction{});
}

void dnv_vista_sdk_dcl_restriction_free(dnv_vista_sdk_dcl_restriction_t* r)
{
    delete reinterpret_cast<dcl::Restriction*>(r);
}

size_t dnv_vista_sdk_dcl_restriction_enumeration_count(const dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->enumeration().has_value())
    {
        return 0;
    }

    return toRestriction(r)->enumeration()->size();
}

const char* dnv_vista_sdk_dcl_restriction_enumeration_at(const dnv_vista_sdk_dcl_restriction_t* r, size_t index)
{
    if (r == nullptr || !toRestriction(r)->enumeration().has_value())
    {
        c::setLastError("r must not be null and enumeration must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& enumeration = *toRestriction(r)->enumeration();
    if (index >= enumeration.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return enumeration[index].c_str();
}

void dnv_vista_sdk_dcl_restriction_set_enumeration(
    dnv_vista_sdk_dcl_restriction_t* r, const char* const* values, size_t count)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::vector<std::string> enumeration;
    enumeration.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        enumeration.emplace_back(values[i]);
    }

    toRestriction(r)->setEnumeration(std::move(enumeration));
}

void dnv_vista_sdk_dcl_restriction_clear_enumeration(dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setEnumeration(std::nullopt);
}

int dnv_vista_sdk_dcl_restriction_has_fraction_digits(const dnv_vista_sdk_dcl_restriction_t* r)
{
    return r != nullptr && toRestriction(r)->fractionDigits().has_value() ? 1 : 0;
}

uint32_t dnv_vista_sdk_dcl_restriction_fraction_digits(const dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->fractionDigits().has_value())
    {
        c::setLastError("r must not be null and fractionDigits must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return *toRestriction(r)->fractionDigits();
}

void dnv_vista_sdk_dcl_restriction_set_fraction_digits(dnv_vista_sdk_dcl_restriction_t* r, uint32_t value)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setFractionDigits(value);
}

void dnv_vista_sdk_dcl_restriction_clear_fraction_digits(dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setFractionDigits(std::nullopt);
}

int dnv_vista_sdk_dcl_restriction_has_length(const dnv_vista_sdk_dcl_restriction_t* r)
{
    return r != nullptr && toRestriction(r)->length().has_value() ? 1 : 0;
}

uint32_t dnv_vista_sdk_dcl_restriction_length(const dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->length().has_value())
    {
        c::setLastError("r must not be null and length must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return *toRestriction(r)->length();
}

void dnv_vista_sdk_dcl_restriction_set_length(dnv_vista_sdk_dcl_restriction_t* r, uint32_t value)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setLength(value);
}

void dnv_vista_sdk_dcl_restriction_clear_length(dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setLength(std::nullopt);
}

int dnv_vista_sdk_dcl_restriction_has_max_exclusive(const dnv_vista_sdk_dcl_restriction_t* r)
{
    return r != nullptr && toRestriction(r)->maxExclusive().has_value() ? 1 : 0;
}

double dnv_vista_sdk_dcl_restriction_max_exclusive(const dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->maxExclusive().has_value())
    {
        c::setLastError("r must not be null and maxExclusive must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0.0;
    }

    return *toRestriction(r)->maxExclusive();
}

void dnv_vista_sdk_dcl_restriction_set_max_exclusive(dnv_vista_sdk_dcl_restriction_t* r, double value)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setMaxExclusive(value);
}

void dnv_vista_sdk_dcl_restriction_clear_max_exclusive(dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setMaxExclusive(std::nullopt);
}

int dnv_vista_sdk_dcl_restriction_has_max_inclusive(const dnv_vista_sdk_dcl_restriction_t* r)
{
    return r != nullptr && toRestriction(r)->maxInclusive().has_value() ? 1 : 0;
}

double dnv_vista_sdk_dcl_restriction_max_inclusive(const dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->maxInclusive().has_value())
    {
        c::setLastError("r must not be null and maxInclusive must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0.0;
    }

    return *toRestriction(r)->maxInclusive();
}

void dnv_vista_sdk_dcl_restriction_set_max_inclusive(dnv_vista_sdk_dcl_restriction_t* r, double value)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setMaxInclusive(value);
}

void dnv_vista_sdk_dcl_restriction_clear_max_inclusive(dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setMaxInclusive(std::nullopt);
}

int dnv_vista_sdk_dcl_restriction_has_max_length(const dnv_vista_sdk_dcl_restriction_t* r)
{
    return r != nullptr && toRestriction(r)->maxLength().has_value() ? 1 : 0;
}

uint32_t dnv_vista_sdk_dcl_restriction_max_length(const dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->maxLength().has_value())
    {
        c::setLastError("r must not be null and maxLength must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return *toRestriction(r)->maxLength();
}

void dnv_vista_sdk_dcl_restriction_set_max_length(dnv_vista_sdk_dcl_restriction_t* r, uint32_t value)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setMaxLength(value);
}

void dnv_vista_sdk_dcl_restriction_clear_max_length(dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setMaxLength(std::nullopt);
}

int dnv_vista_sdk_dcl_restriction_has_min_exclusive(const dnv_vista_sdk_dcl_restriction_t* r)
{
    return r != nullptr && toRestriction(r)->minExclusive().has_value() ? 1 : 0;
}

double dnv_vista_sdk_dcl_restriction_min_exclusive(const dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->minExclusive().has_value())
    {
        c::setLastError("r must not be null and minExclusive must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0.0;
    }

    return *toRestriction(r)->minExclusive();
}

void dnv_vista_sdk_dcl_restriction_set_min_exclusive(dnv_vista_sdk_dcl_restriction_t* r, double value)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setMinExclusive(value);
}

void dnv_vista_sdk_dcl_restriction_clear_min_exclusive(dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setMinExclusive(std::nullopt);
}

int dnv_vista_sdk_dcl_restriction_has_min_inclusive(const dnv_vista_sdk_dcl_restriction_t* r)
{
    return r != nullptr && toRestriction(r)->minInclusive().has_value() ? 1 : 0;
}

double dnv_vista_sdk_dcl_restriction_min_inclusive(const dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->minInclusive().has_value())
    {
        c::setLastError("r must not be null and minInclusive must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0.0;
    }

    return *toRestriction(r)->minInclusive();
}

void dnv_vista_sdk_dcl_restriction_set_min_inclusive(dnv_vista_sdk_dcl_restriction_t* r, double value)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setMinInclusive(value);
}

void dnv_vista_sdk_dcl_restriction_clear_min_inclusive(dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setMinInclusive(std::nullopt);
}

int dnv_vista_sdk_dcl_restriction_has_min_length(const dnv_vista_sdk_dcl_restriction_t* r)
{
    return r != nullptr && toRestriction(r)->minLength().has_value() ? 1 : 0;
}

uint32_t dnv_vista_sdk_dcl_restriction_min_length(const dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->minLength().has_value())
    {
        c::setLastError("r must not be null and minLength must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return *toRestriction(r)->minLength();
}

void dnv_vista_sdk_dcl_restriction_set_min_length(dnv_vista_sdk_dcl_restriction_t* r, uint32_t value)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setMinLength(value);
}

void dnv_vista_sdk_dcl_restriction_clear_min_length(dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setMinLength(std::nullopt);
}

const char* dnv_vista_sdk_dcl_restriction_pattern(const dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->pattern().has_value())
    {
        return nullptr;
    }

    return toRestriction(r)->pattern()->c_str();
}

void dnv_vista_sdk_dcl_restriction_set_pattern(dnv_vista_sdk_dcl_restriction_t* r, const char* value)
{
    if (r == nullptr || value == nullptr)
    {
        c::setLastError("r and value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setPattern(std::string{ value });
}

void dnv_vista_sdk_dcl_restriction_clear_pattern(dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setPattern(std::nullopt);
}

int dnv_vista_sdk_dcl_restriction_has_total_digits(const dnv_vista_sdk_dcl_restriction_t* r)
{
    return r != nullptr && toRestriction(r)->totalDigits().has_value() ? 1 : 0;
}

uint32_t dnv_vista_sdk_dcl_restriction_total_digits(const dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->totalDigits().has_value())
    {
        c::setLastError("r must not be null and totalDigits must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return *toRestriction(r)->totalDigits();
}

void dnv_vista_sdk_dcl_restriction_set_total_digits(dnv_vista_sdk_dcl_restriction_t* r, uint32_t value)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    c::cApiTryCatch<void>([&]() -> void { toRestriction(r)->setTotalDigits(value); });
}

void dnv_vista_sdk_dcl_restriction_clear_total_digits(dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setTotalDigits(std::nullopt);
}

int dnv_vista_sdk_dcl_restriction_has_white_space(const dnv_vista_sdk_dcl_restriction_t* r)
{
    return r != nullptr && toRestriction(r)->whiteSpace().has_value() ? 1 : 0;
}

dnv_vista_sdk_dcl_white_space_t dnv_vista_sdk_dcl_restriction_white_space(const dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr || !toRestriction(r)->whiteSpace().has_value())
    {
        c::setLastError("r must not be null and whiteSpace must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return DNV_VISTA_SDK_DCL_WHITE_SPACE_PRESERVE;
    }

    return fromWhiteSpace(*toRestriction(r)->whiteSpace());
}

void dnv_vista_sdk_dcl_restriction_set_white_space(
    dnv_vista_sdk_dcl_restriction_t* r, dnv_vista_sdk_dcl_white_space_t value)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setWhiteSpace(toWhiteSpace(value));
}

void dnv_vista_sdk_dcl_restriction_clear_white_space(dnv_vista_sdk_dcl_restriction_t* r)
{
    if (r == nullptr)
    {
        c::setLastError("r must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toRestriction(r)->setWhiteSpace(std::nullopt);
}

int dnv_vista_sdk_dcl_restriction_validate_value(
    const dnv_vista_sdk_dcl_restriction_t* r, const char* value, const dnv_vista_sdk_dcl_format_t* format)
{
    if (r == nullptr || value == nullptr || format == nullptr)
    {
        c::setLastError("r, value and format must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    const auto result = toRestriction(r)->validateValue(value, *toFormat(format));
    if (!result)
    {
        c::setLastError(
            result.errors().empty() ? "validation failed" : result.errors().front(), DNV_VISTA_SDK_ERROR_DOMAIN);
        return 0;
    }

    return 1;
}

/*=====================================================================
 * Range
 *===================================================================*/

dnv_vista_sdk_dcl_range_t* dnv_vista_sdk_dcl_range_create(double low, double high)
{
    return c::cApiTryCatch<dnv_vista_sdk_dcl_range_t*>(
        [&]() -> dnv_vista_sdk_dcl_range_t* { return fromRange(dcl::Range{ low, high }); });
}

void dnv_vista_sdk_dcl_range_free(dnv_vista_sdk_dcl_range_t* range)
{
    delete reinterpret_cast<dcl::Range*>(range);
}

double dnv_vista_sdk_dcl_range_low(const dnv_vista_sdk_dcl_range_t* range)
{
    if (range == nullptr)
    {
        c::setLastError("range must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0.0;
    }

    return toRange(range)->low();
}

double dnv_vista_sdk_dcl_range_high(const dnv_vista_sdk_dcl_range_t* range)
{
    if (range == nullptr)
    {
        c::setLastError("range must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0.0;
    }

    return toRange(range)->high();
}

void dnv_vista_sdk_dcl_range_set_low(dnv_vista_sdk_dcl_range_t* range, double low)
{
    if (range == nullptr)
    {
        c::setLastError("range must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    c::cApiTryCatch<void>([&]() -> void { toRange(range)->setLow(low); });
}

void dnv_vista_sdk_dcl_range_set_high(dnv_vista_sdk_dcl_range_t* range, double high)
{
    if (range == nullptr)
    {
        c::setLastError("range must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    c::cApiTryCatch<void>([&]() -> void { toRange(range)->setHigh(high); });
}

/*=====================================================================
 * Format
 *===================================================================*/

dnv_vista_sdk_dcl_format_t* dnv_vista_sdk_dcl_format_create(const char* type)
{
    if (type == nullptr)
    {
        c::setLastError("type must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_dcl_format_t*>(
        [&]() -> dnv_vista_sdk_dcl_format_t* { return fromFormat(dcl::Format{ std::string{ type } }); });
}

void dnv_vista_sdk_dcl_format_free(dnv_vista_sdk_dcl_format_t* format)
{
    delete reinterpret_cast<dcl::Format*>(format);
}

const char* dnv_vista_sdk_dcl_format_type(const dnv_vista_sdk_dcl_format_t* format)
{
    if (format == nullptr)
    {
        c::setLastError("format must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toFormat(format)->type().c_str();
}

int dnv_vista_sdk_dcl_format_set_type(dnv_vista_sdk_dcl_format_t* format, const char* type)
{
    if (format == nullptr || type == nullptr)
    {
        c::setLastError("format and type must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return c::cApiTryCatch<int>([&]() -> int {
        toFormat(format)->setType(type);
        return 1;
    });
}

const dnv_vista_sdk_dcl_restriction_t* dnv_vista_sdk_dcl_format_restriction(const dnv_vista_sdk_dcl_format_t* format)
{
    if (format == nullptr || !toFormat(format)->restriction().has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_restriction_t*>(&*toFormat(format)->restriction());
}

void dnv_vista_sdk_dcl_format_set_restriction(
    dnv_vista_sdk_dcl_format_t* format, const dnv_vista_sdk_dcl_restriction_t* restriction)
{
    if (format == nullptr || restriction == nullptr)
    {
        c::setLastError("format and restriction must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toFormat(format)->setRestriction(*toRestriction(restriction));
}

void dnv_vista_sdk_dcl_format_clear_restriction(dnv_vista_sdk_dcl_format_t* format)
{
    if (format == nullptr)
    {
        c::setLastError("format must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toFormat(format)->setRestriction(std::nullopt);
}

int dnv_vista_sdk_dcl_format_validate_value(
    const dnv_vista_sdk_dcl_format_t* format, const char* value, dnv_vista_sdk_iso19848_value_t** parsedValue)
{
    if (format == nullptr || value == nullptr)
    {
        c::setLastError("format and value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    transport::Value parsed{ transport::Value::Boolean{ false } };
    const auto result = toFormat(format)->validateValue(value, parsed);
    if (!result)
    {
        c::setLastError(
            result.errors().empty() ? "validation failed" : result.errors().front(), DNV_VISTA_SDK_ERROR_DOMAIN);
        return 0;
    }

    if (parsedValue != nullptr)
    {
        *parsedValue = fromIso19848Value(std::move(parsed));
    }

    return 1;
}

/*=====================================================================
 * DataChannelType
 *===================================================================*/

dnv_vista_sdk_dcl_data_channel_type_t* dnv_vista_sdk_dcl_data_channel_type_create(const char* type)
{
    if (type == nullptr)
    {
        c::setLastError("type must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_dcl_data_channel_type_t*>([&]() -> dnv_vista_sdk_dcl_data_channel_type_t* {
        return fromDataChannelType(dcl::DataChannelType{ std::string{ type } });
    });
}

void dnv_vista_sdk_dcl_data_channel_type_free(dnv_vista_sdk_dcl_data_channel_type_t* dct)
{
    delete reinterpret_cast<dcl::DataChannelType*>(dct);
}

const char* dnv_vista_sdk_dcl_data_channel_type_type(const dnv_vista_sdk_dcl_data_channel_type_t* dct)
{
    if (dct == nullptr)
    {
        c::setLastError("dct must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toDataChannelType(dct)->type().c_str();
}

int dnv_vista_sdk_dcl_data_channel_type_set_type(dnv_vista_sdk_dcl_data_channel_type_t* dct, const char* type)
{
    if (dct == nullptr || type == nullptr)
    {
        c::setLastError("dct and type must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return c::cApiTryCatch<int>([&]() -> int {
        toDataChannelType(dct)->setType(type);
        return 1;
    });
}

int dnv_vista_sdk_dcl_data_channel_type_has_update_cycle(const dnv_vista_sdk_dcl_data_channel_type_t* dct)
{
    return dct != nullptr && toDataChannelType(dct)->updateCycle().has_value() ? 1 : 0;
}

double dnv_vista_sdk_dcl_data_channel_type_update_cycle(const dnv_vista_sdk_dcl_data_channel_type_t* dct)
{
    if (dct == nullptr || !toDataChannelType(dct)->updateCycle().has_value())
    {
        c::setLastError("dct must not be null and updateCycle must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0.0;
    }

    return *toDataChannelType(dct)->updateCycle();
}

void dnv_vista_sdk_dcl_data_channel_type_set_update_cycle(dnv_vista_sdk_dcl_data_channel_type_t* dct, double value)
{
    if (dct == nullptr)
    {
        c::setLastError("dct must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    c::cApiTryCatch<void>([&]() -> void { toDataChannelType(dct)->setUpdateCycle(value); });
}

void dnv_vista_sdk_dcl_data_channel_type_clear_update_cycle(dnv_vista_sdk_dcl_data_channel_type_t* dct)
{
    if (dct == nullptr)
    {
        c::setLastError("dct must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toDataChannelType(dct)->setUpdateCycle(std::nullopt);
}

int dnv_vista_sdk_dcl_data_channel_type_has_calculation_period(const dnv_vista_sdk_dcl_data_channel_type_t* dct)
{
    return dct != nullptr && toDataChannelType(dct)->calculationPeriod().has_value() ? 1 : 0;
}

double dnv_vista_sdk_dcl_data_channel_type_calculation_period(const dnv_vista_sdk_dcl_data_channel_type_t* dct)
{
    if (dct == nullptr || !toDataChannelType(dct)->calculationPeriod().has_value())
    {
        c::setLastError("dct must not be null and calculationPeriod must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0.0;
    }

    return *toDataChannelType(dct)->calculationPeriod();
}

void dnv_vista_sdk_dcl_data_channel_type_set_calculation_period(
    dnv_vista_sdk_dcl_data_channel_type_t* dct, double value)
{
    if (dct == nullptr)
    {
        c::setLastError("dct must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    c::cApiTryCatch<void>([&]() -> void { toDataChannelType(dct)->setCalculationPeriod(value); });
}

void dnv_vista_sdk_dcl_data_channel_type_clear_calculation_period(dnv_vista_sdk_dcl_data_channel_type_t* dct)
{
    if (dct == nullptr)
    {
        c::setLastError("dct must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toDataChannelType(dct)->setCalculationPeriod(std::nullopt);
}

int dnv_vista_sdk_dcl_data_channel_type_is_alert(const dnv_vista_sdk_dcl_data_channel_type_t* dct)
{
    return dct != nullptr && toDataChannelType(dct)->isAlert() ? 1 : 0;
}

/*=====================================================================
 * NameObject
 *===================================================================*/

dnv_vista_sdk_dcl_name_object_t* dnv_vista_sdk_dcl_name_object_create_default(void)
{
    return fromNameObject(dcl::NameObject{});
}

dnv_vista_sdk_dcl_name_object_t* dnv_vista_sdk_dcl_name_object_create(const char* namingRule)
{
    if (namingRule == nullptr)
    {
        c::setLastError("namingRule must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromNameObject(dcl::NameObject{ std::string{ namingRule } });
}

void dnv_vista_sdk_dcl_name_object_free(dnv_vista_sdk_dcl_name_object_t* nameObject)
{
    delete reinterpret_cast<dcl::NameObject*>(nameObject);
}

const char* dnv_vista_sdk_dcl_name_object_naming_rule(const dnv_vista_sdk_dcl_name_object_t* nameObject)
{
    if (nameObject == nullptr)
    {
        c::setLastError("nameObject must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toNameObject(nameObject)->namingRule().c_str();
}

void dnv_vista_sdk_dcl_name_object_set_naming_rule(dnv_vista_sdk_dcl_name_object_t* nameObject, const char* namingRule)
{
    if (nameObject == nullptr || namingRule == nullptr)
    {
        c::setLastError("nameObject and namingRule must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toNameObject(nameObject)->setNamingRule(namingRule);
}

const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_name_object_custom_name_objects(
    const dnv_vista_sdk_dcl_name_object_t* nameObject)
{
    if (nameObject == nullptr || !toNameObject(nameObject)->customNameObjects().has_value())
    {
        return nullptr;
    }

    return fromSerializableDocumentRef(*toNameObject(nameObject)->customNameObjects());
}

void dnv_vista_sdk_dcl_name_object_set_custom_name_objects(
    dnv_vista_sdk_dcl_name_object_t* nameObject, dnv_vista_sdk_serializable_document_t* value)
{
    if (nameObject == nullptr || value == nullptr)
    {
        c::setLastError("nameObject and value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::unique_ptr<transport::serialization::json::SerializableDocument> owned{
        reinterpret_cast<transport::serialization::json::SerializableDocument*>(value)
    };
    toNameObject(nameObject)->setCustomNameObjects(std::move(*owned));
}

void dnv_vista_sdk_dcl_name_object_clear_custom_name_objects(dnv_vista_sdk_dcl_name_object_t* nameObject)
{
    if (nameObject == nullptr)
    {
        c::setLastError("nameObject must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toNameObject(nameObject)->setCustomNameObjects(std::nullopt);
}

/*=====================================================================
 * Unit
 *===================================================================*/

dnv_vista_sdk_dcl_unit_t* dnv_vista_sdk_dcl_unit_create(const char* unitSymbol)
{
    if (unitSymbol == nullptr)
    {
        c::setLastError("unitSymbol must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromUnit(dcl::Unit{ std::string{ unitSymbol } });
}

void dnv_vista_sdk_dcl_unit_free(dnv_vista_sdk_dcl_unit_t* unit)
{
    delete reinterpret_cast<dcl::Unit*>(unit);
}

const char* dnv_vista_sdk_dcl_unit_unit_symbol(const dnv_vista_sdk_dcl_unit_t* unit)
{
    if (unit == nullptr)
    {
        c::setLastError("unit must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toUnit(unit)->unitSymbol().c_str();
}

void dnv_vista_sdk_dcl_unit_set_unit_symbol(dnv_vista_sdk_dcl_unit_t* unit, const char* unitSymbol)
{
    if (unit == nullptr || unitSymbol == nullptr)
    {
        c::setLastError("unit and unitSymbol must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toUnit(unit)->setUnitSymbol(unitSymbol);
}

const char* dnv_vista_sdk_dcl_unit_quantity_name(const dnv_vista_sdk_dcl_unit_t* unit)
{
    if (unit == nullptr || !toUnit(unit)->quantityName().has_value())
    {
        return nullptr;
    }

    return toUnit(unit)->quantityName()->c_str();
}

void dnv_vista_sdk_dcl_unit_set_quantity_name(dnv_vista_sdk_dcl_unit_t* unit, const char* quantityName)
{
    if (unit == nullptr || quantityName == nullptr)
    {
        c::setLastError("unit and quantityName must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toUnit(unit)->setQuantityName(std::string{ quantityName });
}

void dnv_vista_sdk_dcl_unit_clear_quantity_name(dnv_vista_sdk_dcl_unit_t* unit)
{
    if (unit == nullptr)
    {
        c::setLastError("unit must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toUnit(unit)->setQuantityName(std::nullopt);
}

const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_unit_custom_elements(
    const dnv_vista_sdk_dcl_unit_t* unit)
{
    if (unit == nullptr || !toUnit(unit)->customElements().has_value())
    {
        return nullptr;
    }

    return fromSerializableDocumentRef(*toUnit(unit)->customElements());
}

void dnv_vista_sdk_dcl_unit_set_custom_elements(
    dnv_vista_sdk_dcl_unit_t* unit, dnv_vista_sdk_serializable_document_t* value)
{
    if (unit == nullptr || value == nullptr)
    {
        c::setLastError("unit and value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::unique_ptr<transport::serialization::json::SerializableDocument> owned{
        reinterpret_cast<transport::serialization::json::SerializableDocument*>(value)
    };
    toUnit(unit)->setCustomElements(std::move(*owned));
}

void dnv_vista_sdk_dcl_unit_clear_custom_elements(dnv_vista_sdk_dcl_unit_t* unit)
{
    if (unit == nullptr)
    {
        c::setLastError("unit must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toUnit(unit)->setCustomElements(std::nullopt);
}

/*=====================================================================
 * Property
 *===================================================================*/

dnv_vista_sdk_dcl_property_t* dnv_vista_sdk_dcl_property_create(
    const dnv_vista_sdk_dcl_data_channel_type_t* dataChannelType, const dnv_vista_sdk_dcl_format_t* format)
{
    if (dataChannelType == nullptr || format == nullptr)
    {
        c::setLastError("dataChannelType and format must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromProperty(dcl::Property{ *toDataChannelType(dataChannelType), *toFormat(format) });
}

void dnv_vista_sdk_dcl_property_free(dnv_vista_sdk_dcl_property_t* property)
{
    delete reinterpret_cast<dcl::Property*>(property);
}

const dnv_vista_sdk_dcl_data_channel_type_t* dnv_vista_sdk_dcl_property_data_channel_type(
    const dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr)
    {
        c::setLastError("property must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_data_channel_type_t*>(&toProperty(property)->dataChannelType());
}

void dnv_vista_sdk_dcl_property_set_data_channel_type(
    dnv_vista_sdk_dcl_property_t* property, const dnv_vista_sdk_dcl_data_channel_type_t* dataChannelType)
{
    if (property == nullptr || dataChannelType == nullptr)
    {
        c::setLastError("property and dataChannelType must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setDataChannelType(*toDataChannelType(dataChannelType));
}

const dnv_vista_sdk_dcl_format_t* dnv_vista_sdk_dcl_property_format(const dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr)
    {
        c::setLastError("property must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_format_t*>(&toProperty(property)->format());
}

void dnv_vista_sdk_dcl_property_set_format(
    dnv_vista_sdk_dcl_property_t* property, const dnv_vista_sdk_dcl_format_t* format)
{
    if (property == nullptr || format == nullptr)
    {
        c::setLastError("property and format must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setFormat(*toFormat(format));
}

const dnv_vista_sdk_dcl_range_t* dnv_vista_sdk_dcl_property_range(const dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr || !toProperty(property)->range().has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_range_t*>(&*toProperty(property)->range());
}

void dnv_vista_sdk_dcl_property_set_range(
    dnv_vista_sdk_dcl_property_t* property, const dnv_vista_sdk_dcl_range_t* range)
{
    if (property == nullptr || range == nullptr)
    {
        c::setLastError("property and range must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setRange(*toRange(range));
}

void dnv_vista_sdk_dcl_property_clear_range(dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr)
    {
        c::setLastError("property must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setRange(std::nullopt);
}

const dnv_vista_sdk_dcl_unit_t* dnv_vista_sdk_dcl_property_unit(const dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr || !toProperty(property)->unit().has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_unit_t*>(&*toProperty(property)->unit());
}

void dnv_vista_sdk_dcl_property_set_unit(dnv_vista_sdk_dcl_property_t* property, const dnv_vista_sdk_dcl_unit_t* unit)
{
    if (property == nullptr || unit == nullptr)
    {
        c::setLastError("property and unit must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setUnit(*toUnit(unit));
}

void dnv_vista_sdk_dcl_property_clear_unit(dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr)
    {
        c::setLastError("property must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setUnit(std::nullopt);
}

const char* dnv_vista_sdk_dcl_property_quality_coding(const dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr || !toProperty(property)->qualityCoding().has_value())
    {
        return nullptr;
    }

    return toProperty(property)->qualityCoding()->c_str();
}

void dnv_vista_sdk_dcl_property_set_quality_coding(dnv_vista_sdk_dcl_property_t* property, const char* qualityCoding)
{
    if (property == nullptr || qualityCoding == nullptr)
    {
        c::setLastError("property and qualityCoding must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setQualityCoding(std::string{ qualityCoding });
}

void dnv_vista_sdk_dcl_property_clear_quality_coding(dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr)
    {
        c::setLastError("property must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setQualityCoding(std::nullopt);
}

const char* dnv_vista_sdk_dcl_property_alert_priority(const dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr || !toProperty(property)->alertPriority().has_value())
    {
        return nullptr;
    }

    return toProperty(property)->alertPriority()->c_str();
}

void dnv_vista_sdk_dcl_property_set_alert_priority(dnv_vista_sdk_dcl_property_t* property, const char* alertPriority)
{
    if (property == nullptr || alertPriority == nullptr)
    {
        c::setLastError("property and alertPriority must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setAlertPriority(std::string{ alertPriority });
}

void dnv_vista_sdk_dcl_property_clear_alert_priority(dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr)
    {
        c::setLastError("property must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setAlertPriority(std::nullopt);
}

const char* dnv_vista_sdk_dcl_property_name(const dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr || !toProperty(property)->name().has_value())
    {
        return nullptr;
    }

    return toProperty(property)->name()->c_str();
}

void dnv_vista_sdk_dcl_property_set_name(dnv_vista_sdk_dcl_property_t* property, const char* name)
{
    if (property == nullptr || name == nullptr)
    {
        c::setLastError("property and name must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setName(std::string{ name });
}

void dnv_vista_sdk_dcl_property_clear_name(dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr)
    {
        c::setLastError("property must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setName(std::nullopt);
}

const char* dnv_vista_sdk_dcl_property_remarks(const dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr || !toProperty(property)->remarks().has_value())
    {
        return nullptr;
    }

    return toProperty(property)->remarks()->c_str();
}

void dnv_vista_sdk_dcl_property_set_remarks(dnv_vista_sdk_dcl_property_t* property, const char* remarks)
{
    if (property == nullptr || remarks == nullptr)
    {
        c::setLastError("property and remarks must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setRemarks(std::string{ remarks });
}

void dnv_vista_sdk_dcl_property_clear_remarks(dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr)
    {
        c::setLastError("property must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setRemarks(std::nullopt);
}

const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_property_custom_properties(
    const dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr || !toProperty(property)->customProperties().has_value())
    {
        return nullptr;
    }

    return fromSerializableDocumentRef(*toProperty(property)->customProperties());
}

void dnv_vista_sdk_dcl_property_set_custom_properties(
    dnv_vista_sdk_dcl_property_t* property, dnv_vista_sdk_serializable_document_t* value)
{
    if (property == nullptr || value == nullptr)
    {
        c::setLastError("property and value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::unique_ptr<transport::serialization::json::SerializableDocument> owned{
        reinterpret_cast<transport::serialization::json::SerializableDocument*>(value)
    };
    toProperty(property)->setCustomProperties(std::move(*owned));
}

void dnv_vista_sdk_dcl_property_clear_custom_properties(dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr)
    {
        c::setLastError("property must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toProperty(property)->setCustomProperties(std::nullopt);
}

int dnv_vista_sdk_dcl_property_validate(const dnv_vista_sdk_dcl_property_t* property)
{
    if (property == nullptr)
    {
        c::setLastError("property must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    const auto result = toProperty(property)->validate();
    if (!result)
    {
        c::setLastError(
            result.errors().empty() ? "validation failed" : result.errors().front(), DNV_VISTA_SDK_ERROR_DOMAIN);
        return 0;
    }

    return 1;
}

/*=====================================================================
 * ConfigurationReference
 *===================================================================*/

dnv_vista_sdk_dcl_configuration_reference_t* dnv_vista_sdk_dcl_configuration_reference_create(
    const char* id, dnv_vista_sdk_date_time_offset_t timeStamp)
{
    if (id == nullptr)
    {
        c::setLastError("id must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromConfigurationReference(dcl::ConfigurationReference{ std::string{ id }, toDateTimeOffset(timeStamp) });
}

void dnv_vista_sdk_dcl_configuration_reference_free(dnv_vista_sdk_dcl_configuration_reference_t* configRef)
{
    delete reinterpret_cast<dcl::ConfigurationReference*>(configRef);
}

const char* dnv_vista_sdk_dcl_configuration_reference_id(const dnv_vista_sdk_dcl_configuration_reference_t* configRef)
{
    if (configRef == nullptr)
    {
        c::setLastError("configRef must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toConfigurationReference(configRef)->id().c_str();
}

void dnv_vista_sdk_dcl_configuration_reference_set_id(
    dnv_vista_sdk_dcl_configuration_reference_t* configRef, const char* id)
{
    if (configRef == nullptr || id == nullptr)
    {
        c::setLastError("configRef and id must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toConfigurationReference(configRef)->setId(id);
}

const char* dnv_vista_sdk_dcl_configuration_reference_version(
    const dnv_vista_sdk_dcl_configuration_reference_t* configRef)
{
    if (configRef == nullptr || !toConfigurationReference(configRef)->version().has_value())
    {
        return nullptr;
    }

    return toConfigurationReference(configRef)->version()->c_str();
}

void dnv_vista_sdk_dcl_configuration_reference_set_version(
    dnv_vista_sdk_dcl_configuration_reference_t* configRef, const char* version)
{
    if (configRef == nullptr || version == nullptr)
    {
        c::setLastError("configRef and version must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toConfigurationReference(configRef)->setVersion(std::string{ version });
}

void dnv_vista_sdk_dcl_configuration_reference_clear_version(dnv_vista_sdk_dcl_configuration_reference_t* configRef)
{
    if (configRef == nullptr)
    {
        c::setLastError("configRef must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toConfigurationReference(configRef)->setVersion(std::nullopt);
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_dcl_configuration_reference_timestamp(
    const dnv_vista_sdk_dcl_configuration_reference_t* configRef)
{
    if (configRef == nullptr)
    {
        c::setLastError("configRef must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return dnv_vista_sdk_date_time_offset_t{};
    }

    return fromDateTimeOffset(toConfigurationReference(configRef)->timeStamp());
}

void dnv_vista_sdk_dcl_configuration_reference_set_timestamp(
    dnv_vista_sdk_dcl_configuration_reference_t* configRef, dnv_vista_sdk_date_time_offset_t timeStamp)
{
    if (configRef == nullptr)
    {
        c::setLastError("configRef must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toConfigurationReference(configRef)->setTimeStamp(toDateTimeOffset(timeStamp));
}

/*=====================================================================
 * VersionInformation
 *===================================================================*/

dnv_vista_sdk_dcl_version_information_t* dnv_vista_sdk_dcl_version_information_create_default(void)
{
    return fromVersionInformation(dcl::VersionInformation{});
}

dnv_vista_sdk_dcl_version_information_t* dnv_vista_sdk_dcl_version_information_create(
    const char* namingRule, const char* namingSchemeVersion)
{
    if (namingRule == nullptr || namingSchemeVersion == nullptr)
    {
        c::setLastError("namingRule and namingSchemeVersion must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromVersionInformation(
        dcl::VersionInformation{ std::string{ namingRule }, std::string{ namingSchemeVersion } });
}

void dnv_vista_sdk_dcl_version_information_free(dnv_vista_sdk_dcl_version_information_t* versionInfo)
{
    delete reinterpret_cast<dcl::VersionInformation*>(versionInfo);
}

const char* dnv_vista_sdk_dcl_version_information_naming_rule(
    const dnv_vista_sdk_dcl_version_information_t* versionInfo)
{
    if (versionInfo == nullptr)
    {
        c::setLastError("versionInfo must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toVersionInformation(versionInfo)->namingRule().c_str();
}

void dnv_vista_sdk_dcl_version_information_set_naming_rule(
    dnv_vista_sdk_dcl_version_information_t* versionInfo, const char* namingRule)
{
    if (versionInfo == nullptr || namingRule == nullptr)
    {
        c::setLastError("versionInfo and namingRule must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toVersionInformation(versionInfo)->setNamingRule(namingRule);
}

const char* dnv_vista_sdk_dcl_version_information_naming_scheme_version(
    const dnv_vista_sdk_dcl_version_information_t* versionInfo)
{
    if (versionInfo == nullptr)
    {
        c::setLastError("versionInfo must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toVersionInformation(versionInfo)->namingSchemeVersion().c_str();
}

void dnv_vista_sdk_dcl_version_information_set_naming_scheme_version(
    dnv_vista_sdk_dcl_version_information_t* versionInfo, const char* namingSchemeVersion)
{
    if (versionInfo == nullptr || namingSchemeVersion == nullptr)
    {
        c::setLastError("versionInfo and namingSchemeVersion must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toVersionInformation(versionInfo)->setNamingSchemeVersion(namingSchemeVersion);
}

const char* dnv_vista_sdk_dcl_version_information_reference_url(
    const dnv_vista_sdk_dcl_version_information_t* versionInfo)
{
    if (versionInfo == nullptr || !toVersionInformation(versionInfo)->referenceUrl().has_value())
    {
        return nullptr;
    }

    return toVersionInformation(versionInfo)->referenceUrl()->c_str();
}

void dnv_vista_sdk_dcl_version_information_set_reference_url(
    dnv_vista_sdk_dcl_version_information_t* versionInfo, const char* referenceUrl)
{
    if (versionInfo == nullptr || referenceUrl == nullptr)
    {
        c::setLastError("versionInfo and referenceUrl must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toVersionInformation(versionInfo)->setReferenceUrl(std::string{ referenceUrl });
}

void dnv_vista_sdk_dcl_version_information_clear_reference_url(dnv_vista_sdk_dcl_version_information_t* versionInfo)
{
    if (versionInfo == nullptr)
    {
        c::setLastError("versionInfo must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toVersionInformation(versionInfo)->setReferenceUrl(std::nullopt);
}

/*=====================================================================
 * DataChannelId (datachannel)
 *===================================================================*/

dnv_vista_sdk_dcl_channel_id_t* dnv_vista_sdk_dcl_channel_id_create(const dnv_vista_sdk_local_id_t* localId)
{
    if (localId == nullptr)
    {
        c::setLastError("localId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromDclChannelId(dcl::DataChannelId{ *toLocalId(localId) });
}

void dnv_vista_sdk_dcl_channel_id_free(dnv_vista_sdk_dcl_channel_id_t* channelId)
{
    delete reinterpret_cast<dcl::DataChannelId*>(channelId);
}

const dnv_vista_sdk_local_id_t* dnv_vista_sdk_dcl_channel_id_local_id(const dnv_vista_sdk_dcl_channel_id_t* channelId)
{
    if (channelId == nullptr)
    {
        c::setLastError("channelId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromLocalIdRef(toDclChannelId(channelId)->localId());
}

void dnv_vista_sdk_dcl_channel_id_set_local_id(
    dnv_vista_sdk_dcl_channel_id_t* channelId, const dnv_vista_sdk_local_id_t* localId)
{
    if (channelId == nullptr || localId == nullptr)
    {
        c::setLastError("channelId and localId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toDclChannelId(channelId)->setLocalId(*toLocalId(localId));
}

const char* dnv_vista_sdk_dcl_channel_id_short_id(const dnv_vista_sdk_dcl_channel_id_t* channelId)
{
    if (channelId == nullptr || !toDclChannelId(channelId)->shortId().has_value())
    {
        return nullptr;
    }

    return toDclChannelId(channelId)->shortId()->c_str();
}

void dnv_vista_sdk_dcl_channel_id_set_short_id(dnv_vista_sdk_dcl_channel_id_t* channelId, const char* shortId)
{
    if (channelId == nullptr || shortId == nullptr)
    {
        c::setLastError("channelId and shortId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toDclChannelId(channelId)->setShortId(std::string{ shortId });
}

void dnv_vista_sdk_dcl_channel_id_clear_short_id(dnv_vista_sdk_dcl_channel_id_t* channelId)
{
    if (channelId == nullptr)
    {
        c::setLastError("channelId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toDclChannelId(channelId)->setShortId(std::nullopt);
}

const dnv_vista_sdk_dcl_name_object_t* dnv_vista_sdk_dcl_channel_id_name_object(
    const dnv_vista_sdk_dcl_channel_id_t* channelId)
{
    if (channelId == nullptr || !toDclChannelId(channelId)->nameObject().has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_name_object_t*>(&*toDclChannelId(channelId)->nameObject());
}

void dnv_vista_sdk_dcl_channel_id_set_name_object(
    dnv_vista_sdk_dcl_channel_id_t* channelId, const dnv_vista_sdk_dcl_name_object_t* nameObject)
{
    if (channelId == nullptr || nameObject == nullptr)
    {
        c::setLastError("channelId and nameObject must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toDclChannelId(channelId)->setNameObject(*toNameObject(nameObject));
}

void dnv_vista_sdk_dcl_channel_id_clear_name_object(dnv_vista_sdk_dcl_channel_id_t* channelId)
{
    if (channelId == nullptr)
    {
        c::setLastError("channelId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toDclChannelId(channelId)->setNameObject(std::nullopt);
}

/*=====================================================================
 * Header
 *===================================================================*/

dnv_vista_sdk_dcl_header_t* dnv_vista_sdk_dcl_header_create(
    const dnv_vista_sdk_ship_id_t* shipId, const dnv_vista_sdk_dcl_configuration_reference_t* dataChannelListId)
{
    if (shipId == nullptr || dataChannelListId == nullptr)
    {
        c::setLastError("shipId and dataChannelListId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromHeader(dcl::Header{ *toShipId(shipId), *toConfigurationReference(dataChannelListId) });
}

void dnv_vista_sdk_dcl_header_free(dnv_vista_sdk_dcl_header_t* header)
{
    delete reinterpret_cast<dcl::Header*>(header);
}

const dnv_vista_sdk_ship_id_t* dnv_vista_sdk_dcl_header_ship_id(const dnv_vista_sdk_dcl_header_t* header)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_ship_id_t*>(&toHeader(header)->shipId());
}

void dnv_vista_sdk_dcl_header_set_ship_id(dnv_vista_sdk_dcl_header_t* header, const dnv_vista_sdk_ship_id_t* shipId)
{
    if (header == nullptr || shipId == nullptr)
    {
        c::setLastError("header and shipId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toHeader(header)->setShipId(*toShipId(shipId));
}

const dnv_vista_sdk_dcl_configuration_reference_t* dnv_vista_sdk_dcl_header_data_channel_list_id(
    const dnv_vista_sdk_dcl_header_t* header)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_configuration_reference_t*>(&toHeader(header)->dataChannelListId());
}

void dnv_vista_sdk_dcl_header_set_data_channel_list_id(
    dnv_vista_sdk_dcl_header_t* header, const dnv_vista_sdk_dcl_configuration_reference_t* dataChannelListId)
{
    if (header == nullptr || dataChannelListId == nullptr)
    {
        c::setLastError("header and dataChannelListId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toHeader(header)->setDataChannelListId(*toConfigurationReference(dataChannelListId));
}

const dnv_vista_sdk_dcl_version_information_t* dnv_vista_sdk_dcl_header_version_information(
    const dnv_vista_sdk_dcl_header_t* header)
{
    if (header == nullptr || !toHeader(header)->versionInformation().has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_version_information_t*>(&*toHeader(header)->versionInformation());
}

void dnv_vista_sdk_dcl_header_set_version_information(
    dnv_vista_sdk_dcl_header_t* header, const dnv_vista_sdk_dcl_version_information_t* versionInformation)
{
    if (header == nullptr || versionInformation == nullptr)
    {
        c::setLastError("header and versionInformation must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toHeader(header)->setVersionInformation(*toVersionInformation(versionInformation));
}

void dnv_vista_sdk_dcl_header_clear_version_information(dnv_vista_sdk_dcl_header_t* header)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toHeader(header)->setVersionInformation(std::nullopt);
}

const char* dnv_vista_sdk_dcl_header_author(const dnv_vista_sdk_dcl_header_t* header)
{
    if (header == nullptr || !toHeader(header)->author().has_value())
    {
        return nullptr;
    }

    return toHeader(header)->author()->c_str();
}

void dnv_vista_sdk_dcl_header_set_author(dnv_vista_sdk_dcl_header_t* header, const char* author)
{
    if (header == nullptr || author == nullptr)
    {
        c::setLastError("header and author must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toHeader(header)->setAuthor(std::string{ author });
}

void dnv_vista_sdk_dcl_header_clear_author(dnv_vista_sdk_dcl_header_t* header)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toHeader(header)->setAuthor(std::nullopt);
}

int dnv_vista_sdk_dcl_header_has_date_created(const dnv_vista_sdk_dcl_header_t* header)
{
    return header != nullptr && toHeader(header)->dateCreated().has_value() ? 1 : 0;
}

dnv_vista_sdk_date_time_offset_t dnv_vista_sdk_dcl_header_date_created(const dnv_vista_sdk_dcl_header_t* header)
{
    if (header == nullptr || !toHeader(header)->dateCreated().has_value())
    {
        c::setLastError("header must not be null and dateCreated must be set", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return dnv_vista_sdk_date_time_offset_t{};
    }

    return fromDateTimeOffset(*toHeader(header)->dateCreated());
}

void dnv_vista_sdk_dcl_header_set_date_created(
    dnv_vista_sdk_dcl_header_t* header, dnv_vista_sdk_date_time_offset_t dateCreated)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toHeader(header)->setDateCreated(toDateTimeOffset(dateCreated));
}

void dnv_vista_sdk_dcl_header_clear_date_created(dnv_vista_sdk_dcl_header_t* header)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toHeader(header)->setDateCreated(std::nullopt);
}

const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_dcl_header_custom_headers(
    const dnv_vista_sdk_dcl_header_t* header)
{
    if (header == nullptr || !toHeader(header)->customHeaders().has_value())
    {
        return nullptr;
    }

    return fromSerializableDocumentRef(*toHeader(header)->customHeaders());
}

void dnv_vista_sdk_dcl_header_set_custom_headers(
    dnv_vista_sdk_dcl_header_t* header, dnv_vista_sdk_serializable_document_t* value)
{
    if (header == nullptr || value == nullptr)
    {
        c::setLastError("header and value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    std::unique_ptr<transport::serialization::json::SerializableDocument> owned{
        reinterpret_cast<transport::serialization::json::SerializableDocument*>(value)
    };
    toHeader(header)->setCustomHeaders(std::move(*owned));
}

void dnv_vista_sdk_dcl_header_clear_custom_headers(dnv_vista_sdk_dcl_header_t* header)
{
    if (header == nullptr)
    {
        c::setLastError("header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toHeader(header)->setCustomHeaders(std::nullopt);
}

/*=====================================================================
 * DataChannel
 *===================================================================*/

dnv_vista_sdk_dcl_data_channel_t* dnv_vista_sdk_dcl_data_channel_create(
    const dnv_vista_sdk_dcl_channel_id_t* channelId, const dnv_vista_sdk_dcl_property_t* property)
{
    if (channelId == nullptr || property == nullptr)
    {
        c::setLastError("channelId and property must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_dcl_data_channel_t*>([&]() -> dnv_vista_sdk_dcl_data_channel_t* {
        return fromDclDataChannel(dcl::DataChannel{ *toDclChannelId(channelId), *toProperty(property) });
    });
}

void dnv_vista_sdk_dcl_data_channel_free(dnv_vista_sdk_dcl_data_channel_t* dataChannel)
{
    delete reinterpret_cast<dcl::DataChannel*>(dataChannel);
}

const dnv_vista_sdk_dcl_channel_id_t* dnv_vista_sdk_dcl_data_channel_channel_id(
    const dnv_vista_sdk_dcl_data_channel_t* dataChannel)
{
    if (dataChannel == nullptr)
    {
        c::setLastError("dataChannel must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_channel_id_t*>(&toDclDataChannel(dataChannel)->dataChannelId());
}

void dnv_vista_sdk_dcl_data_channel_set_channel_id(
    dnv_vista_sdk_dcl_data_channel_t* dataChannel, const dnv_vista_sdk_dcl_channel_id_t* channelId)
{
    if (dataChannel == nullptr || channelId == nullptr)
    {
        c::setLastError("dataChannel and channelId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toDclDataChannel(dataChannel)->setDataChannelId(*toDclChannelId(channelId));
}

const dnv_vista_sdk_dcl_property_t* dnv_vista_sdk_dcl_data_channel_property(
    const dnv_vista_sdk_dcl_data_channel_t* dataChannel)
{
    if (dataChannel == nullptr)
    {
        c::setLastError("dataChannel must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_property_t*>(&toDclDataChannel(dataChannel)->property());
}

int dnv_vista_sdk_dcl_data_channel_set_property(
    dnv_vista_sdk_dcl_data_channel_t* dataChannel, const dnv_vista_sdk_dcl_property_t* property)
{
    if (dataChannel == nullptr || property == nullptr)
    {
        c::setLastError("dataChannel and property must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return c::cApiTryCatch<int>([&]() -> int {
        toDclDataChannel(dataChannel)->setProperty(*toProperty(property));
        return 1;
    });
}

/*=====================================================================
 * DataChannelList
 *===================================================================*/

dnv_vista_sdk_dcl_data_channel_list_t* dnv_vista_sdk_dcl_data_channel_list_create(void)
{
    return fromDataChannelList(dcl::DataChannelList{ {} });
}

void dnv_vista_sdk_dcl_data_channel_list_free(dnv_vista_sdk_dcl_data_channel_list_t* list)
{
    delete reinterpret_cast<dcl::DataChannelList*>(list);
}

size_t dnv_vista_sdk_dcl_data_channel_list_size(const dnv_vista_sdk_dcl_data_channel_list_t* list)
{
    if (list == nullptr)
    {
        return 0;
    }

    return toDataChannelList(list)->size();
}

const dnv_vista_sdk_dcl_data_channel_t* dnv_vista_sdk_dcl_data_channel_list_at(
    const dnv_vista_sdk_dcl_data_channel_list_t* list, size_t index)
{
    if (list == nullptr)
    {
        c::setLastError("list must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    if (index >= toDataChannelList(list)->size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_data_channel_t*>(&(*toDataChannelList(list))[index]);
}

const dnv_vista_sdk_dcl_data_channel_t* dnv_vista_sdk_dcl_data_channel_list_from_short_id(
    const dnv_vista_sdk_dcl_data_channel_list_t* list, const char* shortId)
{
    if (list == nullptr || shortId == nullptr)
    {
        c::setLastError("list and shortId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto found = toDataChannelList(list)->from(std::string_view{ shortId });
    if (!found.has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_data_channel_t*>(&found->get());
}

const dnv_vista_sdk_dcl_data_channel_t* dnv_vista_sdk_dcl_data_channel_list_from_local_id(
    const dnv_vista_sdk_dcl_data_channel_list_t* list, const dnv_vista_sdk_local_id_t* localId)
{
    if (list == nullptr || localId == nullptr)
    {
        c::setLastError("list and localId must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto found = toDataChannelList(list)->from(*toLocalId(localId));
    if (!found.has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_data_channel_t*>(&found->get());
}

int dnv_vista_sdk_dcl_data_channel_list_add(
    dnv_vista_sdk_dcl_data_channel_list_t* list, const dnv_vista_sdk_dcl_data_channel_t* dataChannel)
{
    if (list == nullptr || dataChannel == nullptr)
    {
        c::setLastError("list and dataChannel must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return c::cApiTryCatch<int>([&]() -> int {
        toDataChannelList(list)->add(*toDclDataChannel(dataChannel));
        return 1;
    });
}

int dnv_vista_sdk_dcl_data_channel_list_remove(
    dnv_vista_sdk_dcl_data_channel_list_t* list, const dnv_vista_sdk_dcl_data_channel_t* item)
{
    if (list == nullptr || item == nullptr)
    {
        return 0;
    }

    return toDataChannelList(list)->remove(*toDclDataChannel(item)) ? 1 : 0;
}

void dnv_vista_sdk_dcl_data_channel_list_clear(dnv_vista_sdk_dcl_data_channel_list_t* list)
{
    if (list == nullptr)
    {
        c::setLastError("list must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toDataChannelList(list)->clear();
}

/*=====================================================================
 * Package
 *===================================================================*/

dnv_vista_sdk_dcl_package_t* dnv_vista_sdk_dcl_package_create(
    const dnv_vista_sdk_dcl_header_t* header, const dnv_vista_sdk_dcl_data_channel_list_t* dataChannelList)
{
    if (header == nullptr || dataChannelList == nullptr)
    {
        c::setLastError("header and dataChannelList must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromDclPackage(dcl::Package{ *toHeader(header), *toDataChannelList(dataChannelList) });
}

void dnv_vista_sdk_dcl_package_free(dnv_vista_sdk_dcl_package_t* package)
{
    delete reinterpret_cast<dcl::Package*>(package);
}

const dnv_vista_sdk_dcl_header_t* dnv_vista_sdk_dcl_package_header(const dnv_vista_sdk_dcl_package_t* package)
{
    if (package == nullptr)
    {
        c::setLastError("package must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_header_t*>(&toDclPackage(package)->header());
}

void dnv_vista_sdk_dcl_package_set_header(
    dnv_vista_sdk_dcl_package_t* package, const dnv_vista_sdk_dcl_header_t* header)
{
    if (package == nullptr || header == nullptr)
    {
        c::setLastError("package and header must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toDclPackage(package)->setHeader(*toHeader(header));
}

const dnv_vista_sdk_dcl_data_channel_list_t* dnv_vista_sdk_dcl_package_data_channel_list(
    const dnv_vista_sdk_dcl_package_t* package)
{
    if (package == nullptr)
    {
        c::setLastError("package must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_data_channel_list_t*>(&toDclPackage(package)->dataChannelList());
}

void dnv_vista_sdk_dcl_package_set_data_channel_list(
    dnv_vista_sdk_dcl_package_t* package, const dnv_vista_sdk_dcl_data_channel_list_t* dataChannelList)
{
    if (package == nullptr || dataChannelList == nullptr)
    {
        c::setLastError("package and dataChannelList must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toDclPackage(package)->setDataChannelList(*toDataChannelList(dataChannelList));
}

/*=====================================================================
 * DataChannelListPackage
 *===================================================================*/

dnv_vista_sdk_dcl_list_package_t* dnv_vista_sdk_dcl_list_package_create(const dnv_vista_sdk_dcl_package_t* package)
{
    if (package == nullptr)
    {
        c::setLastError("package must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromDataChannelListPackage(dcl::DataChannelListPackage{ *toDclPackage(package) });
}

void dnv_vista_sdk_dcl_list_package_free(dnv_vista_sdk_dcl_list_package_t* listPackage)
{
    delete reinterpret_cast<dcl::DataChannelListPackage*>(listPackage);
}

const dnv_vista_sdk_dcl_package_t* dnv_vista_sdk_dcl_list_package_package(
    const dnv_vista_sdk_dcl_list_package_t* listPackage)
{
    if (listPackage == nullptr)
    {
        c::setLastError("listPackage must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_package_t*>(&toDataChannelListPackage(listPackage)->package());
}

void dnv_vista_sdk_dcl_list_package_set_package(
    dnv_vista_sdk_dcl_list_package_t* listPackage, const dnv_vista_sdk_dcl_package_t* package)
{
    if (listPackage == nullptr || package == nullptr)
    {
        c::setLastError("listPackage and package must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return;
    }

    toDataChannelListPackage(listPackage)->setPackage(*toDclPackage(package));
}

const dnv_vista_sdk_dcl_data_channel_list_t* dnv_vista_sdk_dcl_list_package_data_channel_list(
    const dnv_vista_sdk_dcl_list_package_t* listPackage)
{
    if (listPackage == nullptr)
    {
        c::setLastError("listPackage must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_dcl_data_channel_list_t*>(
        &toDataChannelListPackage(listPackage)->dataChannelList());
}
