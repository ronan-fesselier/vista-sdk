#include "dnv/vista/sdk/c/transport/iso19848.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromDataChannelTypeName;
using dnv::vista::sdk::c::fromDataChannelTypeNames;
using dnv::vista::sdk::c::fromDecimal;
using dnv::vista::sdk::c::fromFormatDataType;
using dnv::vista::sdk::c::fromFormatDataTypes;
using dnv::vista::sdk::c::fromIso19848Value;
using dnv::vista::sdk::c::fromIso19848Version;
using dnv::vista::sdk::c::fromDateTimeOffset;
using dnv::vista::sdk::c::toDataChannelTypeName;
using dnv::vista::sdk::c::toDataChannelTypeNames;
using dnv::vista::sdk::c::toDecimal;
using dnv::vista::sdk::c::toDateTimeOffset;
using dnv::vista::sdk::c::toFormatDataType;
using dnv::vista::sdk::c::toFormatDataTypes;
using dnv::vista::sdk::c::toIso19848;
using dnv::vista::sdk::c::toIso19848Value;
using dnv::vista::sdk::c::toIso19848Version;
using dnv::vista::sdk::c::toOwnedCString;

//=============================================================================
// ISO19848 singleton
//=============================================================================

const dnv_vista_sdk_iso19848_t* dnv_vista_sdk_iso19848_instance(void)
{
    return reinterpret_cast<const dnv_vista_sdk_iso19848_t*>(&transport::ISO19848::instance());
}

size_t dnv_vista_sdk_iso19848_version_count(const dnv_vista_sdk_iso19848_t* iso19848)
{
    if (iso19848 == nullptr)
    {
        c::setLastError("iso19848 must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toIso19848(iso19848)->versions().size();
}

dnv_vista_sdk_iso19848_version_t dnv_vista_sdk_iso19848_version_at(
    const dnv_vista_sdk_iso19848_t* iso19848, size_t index)
{
    if (iso19848 == nullptr)
    {
        c::setLastError("iso19848 must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return DNV_VISTA_SDK_ISO19848_VERSION_V2018;
    }

    const auto& versions = toIso19848(iso19848)->versions();
    if (index >= versions.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return DNV_VISTA_SDK_ISO19848_VERSION_V2018;
    }

    return fromIso19848Version(versions[index]);
}

dnv_vista_sdk_iso19848_version_t dnv_vista_sdk_iso19848_latest(const dnv_vista_sdk_iso19848_t* iso19848)
{
    if (iso19848 == nullptr)
    {
        c::setLastError("iso19848 must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return DNV_VISTA_SDK_ISO19848_VERSION_V2018;
    }

    return fromIso19848Version(toIso19848(iso19848)->latest());
}

dnv_vista_sdk_iso19848_data_channel_type_names_t* dnv_vista_sdk_iso19848_data_channel_type_names(
    const dnv_vista_sdk_iso19848_t* iso19848, dnv_vista_sdk_iso19848_version_t version)
{
    if (iso19848 == nullptr)
    {
        c::setLastError("iso19848 must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_iso19848_data_channel_type_names_t*>(
        [&]() -> dnv_vista_sdk_iso19848_data_channel_type_names_t* {
            return fromDataChannelTypeNames(const_cast<transport::ISO19848*>(toIso19848(iso19848))
                                                ->dataChannelTypeNames(toIso19848Version(version)));
        });
}

dnv_vista_sdk_iso19848_format_data_types_t* dnv_vista_sdk_iso19848_format_data_types(
    const dnv_vista_sdk_iso19848_t* iso19848, dnv_vista_sdk_iso19848_version_t version)
{
    if (iso19848 == nullptr)
    {
        c::setLastError("iso19848 must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return c::cApiTryCatch<dnv_vista_sdk_iso19848_format_data_types_t*>(
        [&]() -> dnv_vista_sdk_iso19848_format_data_types_t* {
            return fromFormatDataTypes(
                const_cast<transport::ISO19848*>(toIso19848(iso19848))->formatDataTypes(toIso19848Version(version)));
        });
}

//=============================================================================
// DataChannelTypeName / DataChannelTypeNames
//=============================================================================

void dnv_vista_sdk_iso19848_data_channel_type_name_free(dnv_vista_sdk_iso19848_data_channel_type_name_t* typeName)
{
    delete reinterpret_cast<transport::DataChannelTypeName*>(typeName);
}

const char* dnv_vista_sdk_iso19848_data_channel_type_name_type(
    const dnv_vista_sdk_iso19848_data_channel_type_name_t* typeName)
{
    if (typeName == nullptr)
    {
        c::setLastError("typeName must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toDataChannelTypeName(typeName)->type.c_str();
}

const char* dnv_vista_sdk_iso19848_data_channel_type_name_description(
    const dnv_vista_sdk_iso19848_data_channel_type_name_t* typeName)
{
    if (typeName == nullptr)
    {
        c::setLastError("typeName must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toDataChannelTypeName(typeName)->description.c_str();
}

void dnv_vista_sdk_iso19848_data_channel_type_names_free(dnv_vista_sdk_iso19848_data_channel_type_names_t* typeNames)
{
    delete reinterpret_cast<transport::DataChannelTypeNames*>(typeNames);
}

dnv_vista_sdk_iso19848_data_channel_type_name_t* dnv_vista_sdk_iso19848_data_channel_type_names_from_string(
    const dnv_vista_sdk_iso19848_data_channel_type_names_t* typeNames, const char* type)
{
    if (typeNames == nullptr || type == nullptr)
    {
        c::setLastError("typeNames and type must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    auto result = toDataChannelTypeNames(typeNames)->fromString(type);
    if (!result || !result.value().has_value())
    {
        c::setLastError("unknown data channel type name", DNV_VISTA_SDK_ERROR_DOMAIN);
        return nullptr;
    }

    return fromDataChannelTypeName(transport::DataChannelTypeName{ *result.value() });
}

size_t dnv_vista_sdk_iso19848_data_channel_type_names_count(
    const dnv_vista_sdk_iso19848_data_channel_type_names_t* typeNames)
{
    if (typeNames == nullptr)
    {
        return 0;
    }

    return toDataChannelTypeNames(typeNames)->size();
}

const dnv_vista_sdk_iso19848_data_channel_type_name_t* dnv_vista_sdk_iso19848_data_channel_type_names_at(
    const dnv_vista_sdk_iso19848_data_channel_type_names_t* typeNames, size_t index)
{
    if (typeNames == nullptr)
    {
        c::setLastError("typeNames must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto* names = toDataChannelTypeNames(typeNames);
    if (index >= names->size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_iso19848_data_channel_type_name_t*>(&*(names->begin() + index));
}

//=============================================================================
// FormatDataType / FormatDataTypes
//=============================================================================

void dnv_vista_sdk_iso19848_format_data_type_free(dnv_vista_sdk_iso19848_format_data_type_t* formatDataType)
{
    delete reinterpret_cast<transport::FormatDataType*>(formatDataType);
}

const char* dnv_vista_sdk_iso19848_format_data_type_type(
    const dnv_vista_sdk_iso19848_format_data_type_t* formatDataType)
{
    if (formatDataType == nullptr)
    {
        c::setLastError("formatDataType must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toFormatDataType(formatDataType)->type.c_str();
}

const char* dnv_vista_sdk_iso19848_format_data_type_description(
    const dnv_vista_sdk_iso19848_format_data_type_t* formatDataType)
{
    if (formatDataType == nullptr)
    {
        c::setLastError("formatDataType must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toFormatDataType(formatDataType)->description.c_str();
}

int dnv_vista_sdk_iso19848_format_data_type_validate(
    const dnv_vista_sdk_iso19848_format_data_type_t* formatDataType,
    const char* value,
    dnv_vista_sdk_iso19848_value_t** result)
{
    if (formatDataType == nullptr || value == nullptr || result == nullptr)
    {
        c::setLastError("formatDataType, value and result must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    auto validation = toFormatDataType(formatDataType)->validate(value);
    if (!validation)
    {
        c::setLastError(
            validation.errors().empty() ? "validation failed" : validation.errors()[0], DNV_VISTA_SDK_ERROR_DOMAIN);
        return 0;
    }

    *result = fromIso19848Value(transport::Value{ *validation.value() });
    return 1;
}

void dnv_vista_sdk_iso19848_format_data_types_free(dnv_vista_sdk_iso19848_format_data_types_t* formatDataTypes)
{
    delete reinterpret_cast<transport::FormatDataTypes*>(formatDataTypes);
}

dnv_vista_sdk_iso19848_format_data_type_t* dnv_vista_sdk_iso19848_format_data_types_from_string(
    const dnv_vista_sdk_iso19848_format_data_types_t* formatDataTypes, const char* type)
{
    if (formatDataTypes == nullptr || type == nullptr)
    {
        c::setLastError("formatDataTypes and type must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    auto result = toFormatDataTypes(formatDataTypes)->fromString(type);
    if (!result || !result.value().has_value())
    {
        c::setLastError("unknown format data type", DNV_VISTA_SDK_ERROR_DOMAIN);
        return nullptr;
    }

    return fromFormatDataType(transport::FormatDataType{ *result.value() });
}

size_t dnv_vista_sdk_iso19848_format_data_types_count(const dnv_vista_sdk_iso19848_format_data_types_t* formatDataTypes)
{
    if (formatDataTypes == nullptr)
    {
        return 0;
    }

    return toFormatDataTypes(formatDataTypes)->size();
}

const dnv_vista_sdk_iso19848_format_data_type_t* dnv_vista_sdk_iso19848_format_data_types_at(
    const dnv_vista_sdk_iso19848_format_data_types_t* formatDataTypes, size_t index)
{
    if (formatDataTypes == nullptr)
    {
        c::setLastError("formatDataTypes must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto* types = toFormatDataTypes(formatDataTypes);
    if (index >= types->size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return reinterpret_cast<const dnv_vista_sdk_iso19848_format_data_type_t*>(&*(types->begin() + index));
}

//=============================================================================
// Value
//=============================================================================

dnv_vista_sdk_iso19848_value_t* dnv_vista_sdk_iso19848_value_from_string(const char* value)
{
    if (value == nullptr)
    {
        c::setLastError("value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromIso19848Value(transport::Value{ transport::Value::String{ std::string{ value } } });
}

dnv_vista_sdk_iso19848_value_t* dnv_vista_sdk_iso19848_value_from_integer(int64_t value)
{
    return fromIso19848Value(transport::Value{ transport::Value::Integer{ value } });
}

dnv_vista_sdk_iso19848_value_t* dnv_vista_sdk_iso19848_value_from_boolean(int value)
{
    return fromIso19848Value(transport::Value{ transport::Value::Boolean{ value != 0 } });
}

dnv_vista_sdk_iso19848_value_t* dnv_vista_sdk_iso19848_value_from_decimal(dnv_vista_sdk_decimal_t value)
{
    return fromIso19848Value(transport::Value{ transport::Value::Decimal{ toDecimal(value) } });
}

dnv_vista_sdk_iso19848_value_t* dnv_vista_sdk_iso19848_value_from_date_time(dnv_vista_sdk_date_time_offset_t value)
{
    return fromIso19848Value(transport::Value{ transport::Value::DateTime{ toDateTimeOffset(value) } });
}

void dnv_vista_sdk_iso19848_value_free(dnv_vista_sdk_iso19848_value_t* value)
{
    delete reinterpret_cast<transport::Value*>(value);
}

dnv_vista_sdk_iso19848_value_type_t dnv_vista_sdk_iso19848_value_type(const dnv_vista_sdk_iso19848_value_t* value)
{
    if (value == nullptr)
    {
        c::setLastError("value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return DNV_VISTA_SDK_ISO19848_VALUE_TYPE_STRING;
    }

    return static_cast<dnv_vista_sdk_iso19848_value_type_t>(toIso19848Value(value)->type());
}

const char* dnv_vista_sdk_iso19848_value_string(const dnv_vista_sdk_iso19848_value_t* value)
{
    if (value == nullptr)
    {
        c::setLastError("value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto str = toIso19848Value(value)->string();
    if (!str.has_value())
    {
        return nullptr;
    }

    return str->data();
}

int dnv_vista_sdk_iso19848_value_boolean(const dnv_vista_sdk_iso19848_value_t* value, int* result)
{
    if (value == nullptr || result == nullptr)
    {
        c::setLastError("value and result must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    const auto b = toIso19848Value(value)->boolean();
    if (!b.has_value())
    {
        return 0;
    }

    *result = *b ? 1 : 0;
    return 1;
}

int dnv_vista_sdk_iso19848_value_integer(const dnv_vista_sdk_iso19848_value_t* value, int64_t* result)
{
    if (value == nullptr || result == nullptr)
    {
        c::setLastError("value and result must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    const auto i = toIso19848Value(value)->integer();
    if (!i.has_value())
    {
        return 0;
    }

    *result = *i;
    return 1;
}

int dnv_vista_sdk_iso19848_value_decimal(const dnv_vista_sdk_iso19848_value_t* value, dnv_vista_sdk_decimal_t* result)
{
    if (value == nullptr || result == nullptr)
    {
        c::setLastError("value and result must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    const auto d = toIso19848Value(value)->decimal();
    if (!d.has_value())
    {
        return 0;
    }

    *result = fromDecimal(*d);
    return 1;
}

int dnv_vista_sdk_iso19848_value_date_time(
    const dnv_vista_sdk_iso19848_value_t* value, dnv_vista_sdk_date_time_offset_t* result)
{
    if (value == nullptr || result == nullptr)
    {
        c::setLastError("value and result must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    const auto dt = toIso19848Value(value)->dateTime();
    if (!dt.has_value())
    {
        return 0;
    }

    *result = fromDateTimeOffset(*dt);
    return 1;
}

char* dnv_vista_sdk_iso19848_value_to_string(const dnv_vista_sdk_iso19848_value_t* value)
{
    if (value == nullptr)
    {
        c::setLastError("value must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toOwnedCString(toIso19848Value(value)->toString());
}
