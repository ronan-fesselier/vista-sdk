#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/transport/serialization/json/serializable_document.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

TEST_SUITE("c-api::serializable_document")
{
    TEST_CASE("dnv_vista_sdk_serializable_document_null - default kind is Null")
    {
        auto* doc = dnv_vista_sdk_serializable_document_null();
        REQUIRE(doc != nullptr);

        CHECK(dnv_vista_sdk_serializable_document_kind(doc) == DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_NULL);
        CHECK(dnv_vista_sdk_serializable_document_is_null(doc) == 1);
        CHECK(dnv_vista_sdk_serializable_document_is_boolean(doc) == 0);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_from_boolean - construction and as_boolean")
    {
        auto* doc = dnv_vista_sdk_serializable_document_from_boolean(1);
        REQUIRE(doc != nullptr);

        CHECK(dnv_vista_sdk_serializable_document_is_boolean(doc) == 1);
        CHECK(dnv_vista_sdk_serializable_document_as_boolean(doc) == 1);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_from_integer - negative value round-trips")
    {
        auto* doc = dnv_vista_sdk_serializable_document_from_integer(-42);
        REQUIRE(doc != nullptr);

        CHECK(dnv_vista_sdk_serializable_document_is_integer(doc) == 1);
        CHECK(dnv_vista_sdk_serializable_document_as_integer(doc) == -42);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_from_double - round-trips")
    {
        auto* doc = dnv_vista_sdk_serializable_document_from_double(3.14);
        REQUIRE(doc != nullptr);

        CHECK(dnv_vista_sdk_serializable_document_is_double(doc) == 1);
        CHECK(dnv_vista_sdk_serializable_document_as_double(doc) == doctest::Approx(3.14));

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_from_string - round-trips")
    {
        auto* doc = dnv_vista_sdk_serializable_document_from_string("test");
        REQUIRE(doc != nullptr);

        CHECK(dnv_vista_sdk_serializable_document_is_string(doc) == 1);
        const auto* str = dnv_vista_sdk_serializable_document_as_string(doc);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "test");

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_from_string - null returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_serializable_document_from_string(nullptr) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_array - static factory produces empty array")
    {
        auto* doc = dnv_vista_sdk_serializable_document_array();
        REQUIRE(doc != nullptr);

        CHECK(dnv_vista_sdk_serializable_document_is_array(doc) == 1);
        CHECK(dnv_vista_sdk_serializable_document_array_size(doc) == 0);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_object - static factory produces empty object")
    {
        auto* doc = dnv_vista_sdk_serializable_document_object();
        REQUIRE(doc != nullptr);

        CHECK(dnv_vista_sdk_serializable_document_is_object(doc) == 1);
        CHECK(dnv_vista_sdk_serializable_document_object_size(doc) == 0);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_as_boolean - throws-equivalent sets last error and returns 0")
    {
        auto* doc = dnv_vista_sdk_serializable_document_from_integer(1);
        REQUIRE(doc != nullptr);

        CHECK(dnv_vista_sdk_serializable_document_as_boolean(doc) == 0);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_push_back - appends elements, takes ownership")
    {
        auto* doc = dnv_vista_sdk_serializable_document_array();
        REQUIRE(doc != nullptr);

        dnv_vista_sdk_serializable_document_push_back(doc, dnv_vista_sdk_serializable_document_from_integer(10));
        dnv_vista_sdk_serializable_document_push_back(doc, dnv_vista_sdk_serializable_document_from_integer(20));

        CHECK(dnv_vista_sdk_serializable_document_array_size(doc) == 2);

        const auto* first = dnv_vista_sdk_serializable_document_array_at(doc, 0);
        REQUIRE(first != nullptr);
        CHECK(dnv_vista_sdk_serializable_document_as_integer(first) == 10);

        const auto* second = dnv_vista_sdk_serializable_document_array_at(doc, 1);
        REQUIRE(second != nullptr);
        CHECK(dnv_vista_sdk_serializable_document_as_integer(second) == 20);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_push_back - on non-array resets to array")
    {
        auto* doc = dnv_vista_sdk_serializable_document_from_boolean(1);
        REQUIRE(doc != nullptr);

        dnv_vista_sdk_serializable_document_push_back(doc, dnv_vista_sdk_serializable_document_from_integer(1));

        CHECK(dnv_vista_sdk_serializable_document_is_array(doc) == 1);
        CHECK(dnv_vista_sdk_serializable_document_array_size(doc) == 1);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_array_at - out of range returns null and sets last error")
    {
        auto* doc = dnv_vista_sdk_serializable_document_array();
        REQUIRE(doc != nullptr);

        CHECK(dnv_vista_sdk_serializable_document_array_at(doc, 0) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_set/find - set and lookup by key")
    {
        auto* doc = dnv_vista_sdk_serializable_document_object();
        REQUIRE(doc != nullptr);

        dnv_vista_sdk_serializable_document_set(doc, "key", dnv_vista_sdk_serializable_document_from_string("value"));

        const auto* found = dnv_vista_sdk_serializable_document_find(doc, "key");
        REQUIRE(found != nullptr);
        const auto* str = dnv_vista_sdk_serializable_document_as_string(found);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "value");

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_contains")
    {
        auto* doc = dnv_vista_sdk_serializable_document_object();
        REQUIRE(doc != nullptr);

        dnv_vista_sdk_serializable_document_set(doc, "x", dnv_vista_sdk_serializable_document_from_integer(1));

        CHECK(dnv_vista_sdk_serializable_document_contains(doc, "x") == 1);
        CHECK(dnv_vista_sdk_serializable_document_contains(doc, "y") == 0);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_find - missing key returns null")
    {
        auto* doc = dnv_vista_sdk_serializable_document_object();
        REQUIRE(doc != nullptr);

        CHECK(dnv_vista_sdk_serializable_document_find(doc, "missing") == nullptr);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_find - non-object returns null")
    {
        auto* doc = dnv_vista_sdk_serializable_document_from_integer(1);
        REQUIRE(doc != nullptr);

        CHECK(dnv_vista_sdk_serializable_document_find(doc, "key") == nullptr);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_set - replaces existing key in place")
    {
        auto* doc = dnv_vista_sdk_serializable_document_object();
        REQUIRE(doc != nullptr);

        dnv_vista_sdk_serializable_document_set(doc, "k", dnv_vista_sdk_serializable_document_from_integer(1));
        dnv_vista_sdk_serializable_document_set(doc, "k", dnv_vista_sdk_serializable_document_from_integer(2));

        CHECK(dnv_vista_sdk_serializable_document_object_size(doc) == 1);
        const auto* found = dnv_vista_sdk_serializable_document_find(doc, "k");
        REQUIRE(found != nullptr);
        CHECK(dnv_vista_sdk_serializable_document_as_integer(found) == 2);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_set - preserves insertion order")
    {
        auto* doc = dnv_vista_sdk_serializable_document_object();
        REQUIRE(doc != nullptr);

        dnv_vista_sdk_serializable_document_set(doc, "a", dnv_vista_sdk_serializable_document_from_integer(1));
        dnv_vista_sdk_serializable_document_set(doc, "b", dnv_vista_sdk_serializable_document_from_integer(2));
        dnv_vista_sdk_serializable_document_set(doc, "c", dnv_vista_sdk_serializable_document_from_integer(3));

        REQUIRE(dnv_vista_sdk_serializable_document_object_size(doc) == 3);
        CHECK(std::string_view{ dnv_vista_sdk_serializable_document_object_key_at(doc, 0) } == "a");
        CHECK(std::string_view{ dnv_vista_sdk_serializable_document_object_key_at(doc, 1) } == "b");
        CHECK(std::string_view{ dnv_vista_sdk_serializable_document_object_key_at(doc, 2) } == "c");

        const auto* valueAt1 = dnv_vista_sdk_serializable_document_object_value_at(doc, 1);
        REQUIRE(valueAt1 != nullptr);
        CHECK(dnv_vista_sdk_serializable_document_as_integer(valueAt1) == 2);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_set - on non-object resets to object")
    {
        auto* doc = dnv_vista_sdk_serializable_document_from_boolean(1);
        REQUIRE(doc != nullptr);

        dnv_vista_sdk_serializable_document_set(doc, "key", dnv_vista_sdk_serializable_document_from_boolean(1));

        CHECK(dnv_vista_sdk_serializable_document_is_object(doc) == 1);

        dnv_vista_sdk_serializable_document_free(doc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_equals - same primitive values")
    {
        auto* a = dnv_vista_sdk_serializable_document_from_integer(42);
        auto* b = dnv_vista_sdk_serializable_document_from_integer(42);
        auto* c = dnv_vista_sdk_serializable_document_from_integer(1);

        CHECK(dnv_vista_sdk_serializable_document_equals(a, b) == 1);
        CHECK(dnv_vista_sdk_serializable_document_equals(a, c) == 0);

        dnv_vista_sdk_serializable_document_free(a);
        dnv_vista_sdk_serializable_document_free(b);
        dnv_vista_sdk_serializable_document_free(c);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_equals - different types are not equal")
    {
        auto* nullDoc = dnv_vista_sdk_serializable_document_null();
        auto* falseDoc = dnv_vista_sdk_serializable_document_from_boolean(0);

        CHECK(dnv_vista_sdk_serializable_document_equals(nullDoc, falseDoc) == 0);

        dnv_vista_sdk_serializable_document_free(nullDoc);
        dnv_vista_sdk_serializable_document_free(falseDoc);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_clone - deep copy is independent")
    {
        auto* doc = dnv_vista_sdk_serializable_document_object();
        REQUIRE(doc != nullptr);
        dnv_vista_sdk_serializable_document_set(doc, "x", dnv_vista_sdk_serializable_document_from_integer(1));

        auto* clone = dnv_vista_sdk_serializable_document_clone(doc);
        REQUIRE(clone != nullptr);
        CHECK(dnv_vista_sdk_serializable_document_equals(doc, clone) == 1);

        dnv_vista_sdk_serializable_document_set(doc, "x", dnv_vista_sdk_serializable_document_from_integer(2));
        CHECK(dnv_vista_sdk_serializable_document_equals(doc, clone) == 0);

        dnv_vista_sdk_serializable_document_free(doc);
        dnv_vista_sdk_serializable_document_free(clone);
    }

    TEST_CASE("dnv_vista_sdk_serializable_document_free - null is a no-op")
    {
        dnv_vista_sdk_serializable_document_free(nullptr);
    }

    TEST_CASE("null doc handle - accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_serializable_document_kind(nullptr) == DNV_VISTA_SDK_SERIALIZABLE_DOCUMENT_KIND_NULL);
        CHECK(dnv_vista_sdk_serializable_document_is_null(nullptr) == 0);
        CHECK(dnv_vista_sdk_serializable_document_as_boolean(nullptr) == 0);
        CHECK(dnv_vista_sdk_serializable_document_as_integer(nullptr) == 0);
        CHECK(dnv_vista_sdk_serializable_document_as_double(nullptr) == 0.0);
        CHECK(dnv_vista_sdk_serializable_document_as_string(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_serializable_document_array_size(nullptr) == 0);
        CHECK(dnv_vista_sdk_serializable_document_array_at(nullptr, 0) == nullptr);
        CHECK(dnv_vista_sdk_serializable_document_object_size(nullptr) == 0);
        CHECK(dnv_vista_sdk_serializable_document_find(nullptr, "x") == nullptr);
        CHECK(dnv_vista_sdk_serializable_document_contains(nullptr, "x") == 0);
        CHECK(dnv_vista_sdk_serializable_document_clone(nullptr) == nullptr);
    }
}
