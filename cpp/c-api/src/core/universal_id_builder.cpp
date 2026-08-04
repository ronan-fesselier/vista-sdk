#include "dnv/vista/sdk/c/core/universal_id_builder.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromImoNumberRef;
using dnv::vista::sdk::c::fromLocalIdBuilderRef;
using dnv::vista::sdk::c::fromUniversalId;
using dnv::vista::sdk::c::fromUniversalIdBuilder;
using dnv::vista::sdk::c::fromUniversalIdValue;
using dnv::vista::sdk::c::toImoNumber;
using dnv::vista::sdk::c::toLocalIdBuilder;
using dnv::vista::sdk::c::toOwnedCString;
using dnv::vista::sdk::c::toUniversalIdBuilder;

const char* dnv_vista_sdk_universal_id_builder_naming_entity(void)
{
    return UniversalIdBuilder::namingEntity().data();
}

dnv_vista_sdk_universal_id_builder_t* dnv_vista_sdk_universal_id_builder_create(const char* visVersion)
{
    if (visVersion == nullptr)
    {
        c::setLastErrorMessage("visVersion must not be null");
        return nullptr;
    }

    const auto version = VisVersions::fromString(visVersion);
    if (!version.has_value())
    {
        c::setLastErrorMessage("unrecognized VIS version");
        return nullptr;
    }

    return fromUniversalIdBuilder(UniversalIdBuilder::create(*version));
}

void dnv_vista_sdk_universal_id_builder_free(dnv_vista_sdk_universal_id_builder_t* builder)
{
    delete reinterpret_cast<UniversalIdBuilder*>(builder);
}

dnv_vista_sdk_imo_number_t* dnv_vista_sdk_universal_id_builder_imo_number(
    const dnv_vista_sdk_universal_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    const auto& imoNumber = toUniversalIdBuilder(builder)->imoNumber();
    if (!imoNumber.has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<dnv_vista_sdk_imo_number_t*>(new ImoNumber{ *imoNumber });
}

dnv_vista_sdk_local_id_builder_t* dnv_vista_sdk_universal_id_builder_local_id(
    const dnv_vista_sdk_universal_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    const auto& localIdBuilder = toUniversalIdBuilder(builder)->localId();
    if (!localIdBuilder.has_value())
    {
        return nullptr;
    }

    return reinterpret_cast<dnv_vista_sdk_local_id_builder_t*>(new LocalIdBuilder{ *localIdBuilder });
}

int dnv_vista_sdk_universal_id_builder_is_valid(const dnv_vista_sdk_universal_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return 0;
    }

    return toUniversalIdBuilder(builder)->isValid() ? 1 : 0;
}

dnv_vista_sdk_universal_id_builder_t* dnv_vista_sdk_universal_id_builder_with_imo_number(
    const dnv_vista_sdk_universal_id_builder_t* builder, const dnv_vista_sdk_imo_number_t* imoNumber)
{
    if (builder == nullptr || imoNumber == nullptr)
    {
        c::setLastErrorMessage("builder and imoNumber must not be null");
        return nullptr;
    }

    return fromUniversalIdBuilder(toUniversalIdBuilder(builder)->withImoNumber(*toImoNumber(imoNumber)));
}

dnv_vista_sdk_universal_id_builder_t* dnv_vista_sdk_universal_id_builder_without_imo_number(
    const dnv_vista_sdk_universal_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromUniversalIdBuilder(toUniversalIdBuilder(builder)->withoutImoNumber());
}

dnv_vista_sdk_universal_id_builder_t* dnv_vista_sdk_universal_id_builder_with_local_id(
    const dnv_vista_sdk_universal_id_builder_t* builder, const dnv_vista_sdk_local_id_builder_t* localIdBuilder)
{
    if (builder == nullptr || localIdBuilder == nullptr)
    {
        c::setLastErrorMessage("builder and localIdBuilder must not be null");
        return nullptr;
    }

    return fromUniversalIdBuilder(toUniversalIdBuilder(builder)->withLocalId(*toLocalIdBuilder(localIdBuilder)));
}

dnv_vista_sdk_universal_id_builder_t* dnv_vista_sdk_universal_id_builder_without_local_id(
    const dnv_vista_sdk_universal_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromUniversalIdBuilder(toUniversalIdBuilder(builder)->withoutLocalId());
}

dnv_vista_sdk_universal_id_t* dnv_vista_sdk_universal_id_builder_build(
    const dnv_vista_sdk_universal_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    try
    {
        return fromUniversalIdValue(toUniversalIdBuilder(builder)->build());
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

char* dnv_vista_sdk_universal_id_builder_to_string(const dnv_vista_sdk_universal_id_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    try
    {
        return toOwnedCString(toUniversalIdBuilder(builder)->toString());
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

dnv_vista_sdk_universal_id_t* dnv_vista_sdk_universal_id_builder_from_string(const char* universalIdStr)
{
    if (universalIdStr == nullptr)
    {
        c::setLastErrorMessage("universalIdStr must not be null");
        return nullptr;
    }

    try
    {
        auto universalId = UniversalIdBuilder::fromString(universalIdStr);
        if (!universalId.has_value())
        {
            c::setLastErrorMessage("invalid UniversalId string");
        }

        return fromUniversalId(std::move(universalId));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

dnv_vista_sdk_universal_id_t* dnv_vista_sdk_universal_id_builder_from_string_with_errors(
    const char* universalIdStr, dnv_vista_sdk_parsing_errors_t** outErrors)
{
    if (universalIdStr == nullptr || outErrors == nullptr)
    {
        c::setLastErrorMessage("universalIdStr and outErrors must not be null");
        return nullptr;
    }

    try
    {
        ParsingErrors errors;
        auto universalId = UniversalIdBuilder::fromString(universalIdStr, errors);

        *outErrors = reinterpret_cast<dnv_vista_sdk_parsing_errors_t*>(new ParsingErrors{ std::move(errors) });

        if (!universalId.has_value())
        {
            c::setLastErrorMessage("invalid UniversalId string");
        }

        return fromUniversalId(std::move(universalId));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

void dnv_vista_sdk_universal_id_builder_string_free(char* str)
{
    delete[] str;
}
