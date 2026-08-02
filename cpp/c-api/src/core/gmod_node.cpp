#include "dnv/vista/sdk/c/core/gmod_node.h"

#include "../cast_internal.h"
#include "../error_internal.h"

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromGmodNode;
using dnv::vista::sdk::c::fromLocationRef;
using dnv::vista::sdk::c::fromMetadata;
using dnv::vista::sdk::c::toGmodNode;
using dnv::vista::sdk::c::toOwnedCString;

const char* dnv_vista_sdk_gmod_node_version(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return VisVersions::toString(toGmodNode(node)->version()).data();
}

const char* dnv_vista_sdk_gmod_node_code(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toGmodNode(node)->code().data();
}

const dnv_vista_sdk_location_t* dnv_vista_sdk_gmod_node_location(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& location = toGmodNode(node)->location();
    if (!location.has_value())
    {
        return nullptr;
    }

    return fromLocationRef(*location);
}

const dnv_vista_sdk_gmod_node_metadata_t* dnv_vista_sdk_gmod_node_metadata(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return fromMetadata(toGmodNode(node)->metadata());
}

size_t dnv_vista_sdk_gmod_node_child_count(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodNode(node)->children().size();
}

const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_node_child_at(const dnv_vista_sdk_gmod_node_t* node, size_t index)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& children = toGmodNode(node)->children();
    if (index >= children.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return fromGmodNode(children[index]);
}

size_t dnv_vista_sdk_gmod_node_parent_count(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodNode(node)->parents().size();
}

const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_node_parent_at(const dnv_vista_sdk_gmod_node_t* node, size_t index)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto& parents = toGmodNode(node)->parents();
    if (index >= parents.size())
    {
        c::setLastError("index out of range", DNV_VISTA_SDK_ERROR_OUT_OF_RANGE);
        return nullptr;
    }

    return fromGmodNode(parents[index]);
}

const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_node_product_type(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto productType = toGmodNode(node)->productType();
    if (!productType.has_value())
    {
        return nullptr;
    }

    return fromGmodNode(*productType);
}

const dnv_vista_sdk_gmod_node_t* dnv_vista_sdk_gmod_node_product_selection(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    const auto productSelection = toGmodNode(node)->productSelection();
    if (!productSelection.has_value())
    {
        return nullptr;
    }

    return fromGmodNode(*productSelection);
}

int dnv_vista_sdk_gmod_node_is_function_composition(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodNode(node)->isFunctionComposition() ? 1 : 0;
}

int dnv_vista_sdk_gmod_node_is_mappable(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodNode(node)->isMappable() ? 1 : 0;
}

int dnv_vista_sdk_gmod_node_is_product_selection(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodNode(node)->isProductSelection() ? 1 : 0;
}

int dnv_vista_sdk_gmod_node_is_product_type(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodNode(node)->isProductType() ? 1 : 0;
}

int dnv_vista_sdk_gmod_node_is_asset(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodNode(node)->isAsset() ? 1 : 0;
}

int dnv_vista_sdk_gmod_node_is_leaf_node(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodNode(node)->isLeafNode() ? 1 : 0;
}

int dnv_vista_sdk_gmod_node_is_function_node(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodNode(node)->isFunctionNode() ? 1 : 0;
}

int dnv_vista_sdk_gmod_node_is_asset_function_node(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodNode(node)->isAssetFunctionNode() ? 1 : 0;
}

int dnv_vista_sdk_gmod_node_is_root(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodNode(node)->isRoot() ? 1 : 0;
}

int dnv_vista_sdk_gmod_node_is_child(const dnv_vista_sdk_gmod_node_t* node, const dnv_vista_sdk_gmod_node_t* other)
{
    if (node == nullptr || other == nullptr)
    {
        c::setLastError("node and other must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodNode(node)->isChild(*toGmodNode(other)) ? 1 : 0;
}

int dnv_vista_sdk_gmod_node_is_child_code(const dnv_vista_sdk_gmod_node_t* node, const char* code)
{
    if (node == nullptr || code == nullptr)
    {
        c::setLastError("node and code must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return 0;
    }

    return toGmodNode(node)->isChild(code) ? 1 : 0;
}

char* dnv_vista_sdk_gmod_node_to_string(const dnv_vista_sdk_gmod_node_t* node)
{
    if (node == nullptr)
    {
        c::setLastError("node must not be null", DNV_VISTA_SDK_ERROR_INVALID_ARGUMENT);
        return nullptr;
    }

    return toOwnedCString(toGmodNode(node)->toString());
}
