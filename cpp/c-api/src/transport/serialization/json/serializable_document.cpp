#include "dnv/vista/sdk/c/transport/serialization/json/serializable_document.h"

#include "../../../cast_internal.h"
#include "../../../error_internal.h"

#include <memory>

using namespace dnv::vista::sdk;
using dnv::vista::sdk::c::fromSerializableDocument;
using dnv::vista::sdk::c::fromSerializableDocumentRef;
using dnv::vista::sdk::c::toSerializableDocument;

using Doc = transport::serialization::json::SerializableDocument;

static_assert(static_cast<int>(Doc::Kind::Null) == DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_NULL);
static_assert(static_cast<int>(Doc::Kind::Boolean) == DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_BOOLEAN);
static_assert(static_cast<int>(Doc::Kind::Integer) == DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_INTEGER);
static_assert(static_cast<int>(Doc::Kind::Double) == DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_DOUBLE);
static_assert(static_cast<int>(Doc::Kind::String) == DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_STRING);
static_assert(static_cast<int>(Doc::Kind::Array) == DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_ARRAY);
static_assert(static_cast<int>(Doc::Kind::Object) == DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_OBJECT);

dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_null(void)
{
    return fromSerializableDocument(Doc{});
}

dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_from_boolean(int value)
{
    return fromSerializableDocument(Doc{ value != 0 });
}

dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_from_integer(int64_t value)
{
    return fromSerializableDocument(Doc{ value });
}

dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_from_double(double value)
{
    return fromSerializableDocument(Doc{ value });
}

dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_from_string(const char* value)
{
    if (value == nullptr)
    {
        c::setLastErrorMessage("value must not be null");
        return nullptr;
    }

    return fromSerializableDocument(Doc{ std::string{ value } });
}

dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_array(void)
{
    return fromSerializableDocument(Doc::array());
}

dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_object(void)
{
    return fromSerializableDocument(Doc::object());
}

void dnv_vista_sdk_serializable_document_free(dnv_vista_sdk_serializable_document_t* doc)
{
    delete reinterpret_cast<Doc*>(doc);
}

dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_clone(
    const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr)
    {
        c::setLastErrorMessage("doc must not be null");
        return nullptr;
    }

    return fromSerializableDocument(Doc{ *toSerializableDocument(doc) });
}

int dnv_vista_sdk_serializable_document_equals(
    const dnv_vista_sdk_serializable_document_t* a, const dnv_vista_sdk_serializable_document_t* b)
{
    if (a == nullptr || b == nullptr)
    {
        c::setLastErrorMessage("a and b must not be null");
        return 0;
    }

    return *toSerializableDocument(a) == *toSerializableDocument(b) ? 1 : 0;
}

dnv_vista_sdk_serializable_document_kind_t dnv_vista_sdk_serializable_document_kind(
    const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr)
    {
        c::setLastErrorMessage("doc must not be null");
        return DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_NULL;
    }

    return static_cast<dnv_vista_sdk_serializable_document_kind_t>(toSerializableDocument(doc)->kind());
}

int dnv_vista_sdk_serializable_document_is_null(const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr)
    {
        c::setLastErrorMessage("doc must not be null");
        return 0;
    }

    return toSerializableDocument(doc)->isNull() ? 1 : 0;
}

int dnv_vista_sdk_serializable_document_is_boolean(const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr)
    {
        c::setLastErrorMessage("doc must not be null");
        return 0;
    }

    return toSerializableDocument(doc)->isBoolean() ? 1 : 0;
}

int dnv_vista_sdk_serializable_document_is_integer(const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr)
    {
        c::setLastErrorMessage("doc must not be null");
        return 0;
    }

    return toSerializableDocument(doc)->isInteger() ? 1 : 0;
}

int dnv_vista_sdk_serializable_document_is_double(const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr)
    {
        c::setLastErrorMessage("doc must not be null");
        return 0;
    }

    return toSerializableDocument(doc)->isDouble() ? 1 : 0;
}

int dnv_vista_sdk_serializable_document_is_string(const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr)
    {
        c::setLastErrorMessage("doc must not be null");
        return 0;
    }

    return toSerializableDocument(doc)->isString() ? 1 : 0;
}

int dnv_vista_sdk_serializable_document_is_array(const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr)
    {
        c::setLastErrorMessage("doc must not be null");
        return 0;
    }

    return toSerializableDocument(doc)->isArray() ? 1 : 0;
}

int dnv_vista_sdk_serializable_document_is_object(const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr)
    {
        c::setLastErrorMessage("doc must not be null");
        return 0;
    }

    return toSerializableDocument(doc)->isObject() ? 1 : 0;
}

int dnv_vista_sdk_serializable_document_as_boolean(const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr)
    {
        c::setLastErrorMessage("doc must not be null");
        return 0;
    }

    try
    {
        return toSerializableDocument(doc)->asBoolean() ? 1 : 0;
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return 0;
    }
}

int64_t dnv_vista_sdk_serializable_document_as_integer(const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr)
    {
        c::setLastErrorMessage("doc must not be null");
        return 0;
    }

    try
    {
        return toSerializableDocument(doc)->asInteger();
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return 0;
    }
}

double dnv_vista_sdk_serializable_document_as_double(const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr)
    {
        c::setLastErrorMessage("doc must not be null");
        return 0.0;
    }

    try
    {
        return toSerializableDocument(doc)->asDouble();
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return 0.0;
    }
}

const char* dnv_vista_sdk_serializable_document_as_string(const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr)
    {
        c::setLastErrorMessage("doc must not be null");
        return nullptr;
    }

    try
    {
        return toSerializableDocument(doc)->asString().c_str();
    }
    catch (const std::exception& e)
    {
        c::setLastErrorMessage(e.what());
        return nullptr;
    }
}

size_t dnv_vista_sdk_serializable_document_array_size(const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr || !toSerializableDocument(doc)->isArray())
    {
        return 0;
    }

    return toSerializableDocument(doc)->asArray().size();
}

const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_array_at(
    const dnv_vista_sdk_serializable_document_t* doc, size_t index)
{
    if (doc == nullptr || !toSerializableDocument(doc)->isArray())
    {
        c::setLastErrorMessage("doc must not be null and must be an array");
        return nullptr;
    }

    const auto& arr = toSerializableDocument(doc)->asArray();
    if (index >= arr.size())
    {
        c::setLastErrorMessage("index out of range");
        return nullptr;
    }

    return fromSerializableDocumentRef(arr[index]);
}

size_t dnv_vista_sdk_serializable_document_object_size(const dnv_vista_sdk_serializable_document_t* doc)
{
    if (doc == nullptr || !toSerializableDocument(doc)->isObject())
    {
        return 0;
    }

    return toSerializableDocument(doc)->asObject().size();
}

const char* dnv_vista_sdk_serializable_document_object_key_at(
    const dnv_vista_sdk_serializable_document_t* doc, size_t index)
{
    if (doc == nullptr || !toSerializableDocument(doc)->isObject())
    {
        c::setLastErrorMessage("doc must not be null and must be an object");
        return nullptr;
    }

    const auto& obj = toSerializableDocument(doc)->asObject();
    if (index >= obj.size())
    {
        c::setLastErrorMessage("index out of range");
        return nullptr;
    }

    return obj[index].first.c_str();
}

const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_object_value_at(
    const dnv_vista_sdk_serializable_document_t* doc, size_t index)
{
    if (doc == nullptr || !toSerializableDocument(doc)->isObject())
    {
        c::setLastErrorMessage("doc must not be null and must be an object");
        return nullptr;
    }

    const auto& obj = toSerializableDocument(doc)->asObject();
    if (index >= obj.size())
    {
        c::setLastErrorMessage("index out of range");
        return nullptr;
    }

    return fromSerializableDocumentRef(obj[index].second);
}

const dnv_vista_sdk_serializable_document_t* dnv_vista_sdk_serializable_document_find(
    const dnv_vista_sdk_serializable_document_t* doc, const char* key)
{
    if (doc == nullptr || key == nullptr)
    {
        c::setLastErrorMessage("doc and key must not be null");
        return nullptr;
    }

    const auto* found = toSerializableDocument(doc)->find(key);
    if (found == nullptr)
    {
        return nullptr;
    }

    return fromSerializableDocumentRef(*found);
}

int dnv_vista_sdk_serializable_document_contains(const dnv_vista_sdk_serializable_document_t* doc, const char* key)
{
    if (doc == nullptr || key == nullptr)
    {
        return 0;
    }

    return toSerializableDocument(doc)->contains(key) ? 1 : 0;
}

void dnv_vista_sdk_serializable_document_set(
    dnv_vista_sdk_serializable_document_t* doc, const char* key, dnv_vista_sdk_serializable_document_t* value)
{
    if (doc == nullptr || key == nullptr || value == nullptr)
    {
        c::setLastErrorMessage("doc, key and value must not be null");
        return;
    }

    std::unique_ptr<Doc> owned{ reinterpret_cast<Doc*>(value) };
    reinterpret_cast<Doc*>(doc)->set(key, std::move(*owned));
}

void dnv_vista_sdk_serializable_document_push_back(
    dnv_vista_sdk_serializable_document_t* doc, dnv_vista_sdk_serializable_document_t* value)
{
    if (doc == nullptr || value == nullptr)
    {
        c::setLastErrorMessage("doc and value must not be null");
        return;
    }

    std::unique_ptr<Doc> owned{ reinterpret_cast<Doc*>(value) };
    reinterpret_cast<Doc*>(doc)->push_back(std::move(*owned));
}
