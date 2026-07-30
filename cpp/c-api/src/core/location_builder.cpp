#include "dnv/vista/sdk/c/core/location_builder.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromBuilder;
using dnv::vista::sdk::c::toBuilder;
using dnv::vista::sdk::c::toLocation;
using dnv::vista::sdk::c::toLocationGroup;
using dnv::vista::sdk::c::toLocations;
using dnv::vista::sdk::c::toOwnedCString;

dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_create(const dnv_vista_sdk_locations_t* locations)
{
    if (locations == nullptr)
    {
        c::setLastErrorMessage("locations must not be null");
        return nullptr;
    }

    return fromBuilder(LocationBuilder::create(*toLocations(locations)));
}

void dnv_vista_sdk_location_builder_free(dnv_vista_sdk_location_builder_t* builder)
{
    delete reinterpret_cast<LocationBuilder*>(builder);
}

dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_with_number(
    const dnv_vista_sdk_location_builder_t* builder, int number)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    try
    {
        return fromBuilder(toBuilder(builder)->withNumber(number));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_without_number(
    const dnv_vista_sdk_location_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromBuilder(toBuilder(builder)->withoutNumber());
}

dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_with_side(
    const dnv_vista_sdk_location_builder_t* builder, char side)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    try
    {
        return fromBuilder(toBuilder(builder)->withSide(side));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_without_side(
    const dnv_vista_sdk_location_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromBuilder(toBuilder(builder)->withoutSide());
}

dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_with_vertical(
    const dnv_vista_sdk_location_builder_t* builder, char vertical)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    try
    {
        return fromBuilder(toBuilder(builder)->withVertical(vertical));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_without_vertical(
    const dnv_vista_sdk_location_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromBuilder(toBuilder(builder)->withoutVertical());
}

dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_with_transverse(
    const dnv_vista_sdk_location_builder_t* builder, char transverse)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    try
    {
        return fromBuilder(toBuilder(builder)->withTransverse(transverse));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_without_transverse(
    const dnv_vista_sdk_location_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromBuilder(toBuilder(builder)->withoutTransverse());
}

dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_with_longitudinal(
    const dnv_vista_sdk_location_builder_t* builder, char longitudinal)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    try
    {
        return fromBuilder(toBuilder(builder)->withLongitudinal(longitudinal));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_without_longitudinal(
    const dnv_vista_sdk_location_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromBuilder(toBuilder(builder)->withoutLongitudinal());
}

dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_with_location(
    const dnv_vista_sdk_location_builder_t* builder, const dnv_vista_sdk_location_t* location)
{
    if (builder == nullptr || location == nullptr)
    {
        c::setLastErrorMessage("builder and location must not be null");
        return nullptr;
    }

    return fromBuilder(toBuilder(builder)->withLocation(*toLocation(location)));
}

dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_without_value(
    const dnv_vista_sdk_location_builder_t* builder, dnv_vista_sdk_location_group_t group)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return fromBuilder(toBuilder(builder)->withoutValue(toLocationGroup(group)));
}

int dnv_vista_sdk_location_builder_number(const dnv_vista_sdk_location_builder_t* builder, int* outNumber)
{
    if (builder == nullptr || outNumber == nullptr)
    {
        c::setLastErrorMessage("builder and outNumber must not be null");
        return 0;
    }

    const auto number = toBuilder(builder)->number();
    if (!number.has_value())
    {
        return 0;
    }

    *outNumber = *number;
    return 1;
}

int dnv_vista_sdk_location_builder_side(const dnv_vista_sdk_location_builder_t* builder, char* outSide)
{
    if (builder == nullptr || outSide == nullptr)
    {
        c::setLastErrorMessage("builder and outSide must not be null");
        return 0;
    }

    const auto side = toBuilder(builder)->side();
    if (!side.has_value())
    {
        return 0;
    }

    *outSide = *side;
    return 1;
}

int dnv_vista_sdk_location_builder_vertical(const dnv_vista_sdk_location_builder_t* builder, char* outVertical)
{
    if (builder == nullptr || outVertical == nullptr)
    {
        c::setLastErrorMessage("builder and outVertical must not be null");
        return 0;
    }

    const auto vertical = toBuilder(builder)->vertical();
    if (!vertical.has_value())
    {
        return 0;
    }

    *outVertical = *vertical;
    return 1;
}

int dnv_vista_sdk_location_builder_transverse(const dnv_vista_sdk_location_builder_t* builder, char* outTransverse)
{
    if (builder == nullptr || outTransverse == nullptr)
    {
        c::setLastErrorMessage("builder and outTransverse must not be null");
        return 0;
    }

    const auto transverse = toBuilder(builder)->transverse();
    if (!transverse.has_value())
    {
        return 0;
    }

    *outTransverse = *transverse;
    return 1;
}

int dnv_vista_sdk_location_builder_longitudinal(const dnv_vista_sdk_location_builder_t* builder, char* outLongitudinal)
{
    if (builder == nullptr || outLongitudinal == nullptr)
    {
        c::setLastErrorMessage("builder and outLongitudinal must not be null");
        return 0;
    }

    const auto longitudinal = toBuilder(builder)->longitudinal();
    if (!longitudinal.has_value())
    {
        return 0;
    }

    *outLongitudinal = *longitudinal;
    return 1;
}

dnv_vista_sdk_location_builder_t* dnv_vista_sdk_location_builder_with_code(
    const dnv_vista_sdk_location_builder_t* builder, char code)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    try
    {
        return fromBuilder(toBuilder(builder)->withCode(code));
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

dnv_vista_sdk_location_t* dnv_vista_sdk_location_builder_build(const dnv_vista_sdk_location_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return reinterpret_cast<dnv_vista_sdk_location_t*>(new Location{ toBuilder(builder)->build() });
}

char* dnv_vista_sdk_location_builder_to_string(const dnv_vista_sdk_location_builder_t* builder)
{
    if (builder == nullptr)
    {
        c::setLastErrorMessage("builder must not be null");
        return nullptr;
    }

    return toOwnedCString(toBuilder(builder)->toString());
}

void dnv_vista_sdk_location_builder_string_free(char* str)
{
    delete[] str;
}
