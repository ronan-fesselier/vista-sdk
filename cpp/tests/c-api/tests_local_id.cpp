#include <doctest/doctest.h>

#include <dnv/vista/sdk/c/core/local_id.h>
#include <dnv/vista/sdk/c/core/local_id_builder.h>
#include <dnv/vista/sdk/c/error.h>

#include <string_view>

namespace
{
    constexpr const char* validLocalIdStr =
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet";
} // namespace

TEST_SUITE("c-api::local_id")
{
    TEST_CASE("dnv_vista_sdk_local_id_naming_rule - is dnv-v2")
    {
        CHECK(std::string_view{ dnv_vista_sdk_local_id_naming_rule() } == "dnv-v2");
    }

    TEST_CASE("dnv_vista_sdk_local_id_from_string - valid string returns non-null")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);

        REQUIRE(localId != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_local_id_version(localId) } == "3-4a");

        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("dnv_vista_sdk_local_id_from_string - invalid string returns null and sets last error")
    {
        CHECK(dnv_vista_sdk_local_id_from_string("not-a-local-id") == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("dnv_vista_sdk_local_id_from_string - null returns null")
    {
        CHECK(dnv_vista_sdk_local_id_from_string(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_local_id_primary_item - non-null")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);

        CHECK(dnv_vista_sdk_local_id_primary_item(localId) != nullptr);

        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("dnv_vista_sdk_local_id_secondary_item - unset for this LocalId")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);

        CHECK(dnv_vista_sdk_local_id_secondary_item(localId) == nullptr);

        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("dnv_vista_sdk_local_id_metadata_tag - quantity/content/position are set")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);

        const auto* quantity = dnv_vista_sdk_local_id_metadata_tag(localId, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY);
        REQUIRE(quantity != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_metadata_tag_value(quantity) } == "temperature");

        const auto* content = dnv_vista_sdk_local_id_metadata_tag(localId, DNV_VISTA_SDK_CODEBOOK_NAME_CONTENT);
        REQUIRE(content != nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_metadata_tag_value(content) } == "exhaust.gas");

        const auto* state = dnv_vista_sdk_local_id_metadata_tag(localId, DNV_VISTA_SDK_CODEBOOK_NAME_STATE);
        CHECK(state == nullptr);

        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("dnv_vista_sdk_local_id_builder - returns a valid builder handle")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);

        const auto* builder = dnv_vista_sdk_local_id_builder(localId);
        REQUIRE(builder != nullptr);
        CHECK(dnv_vista_sdk_local_id_builder_is_valid(builder) == 1);

        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("dnv_vista_sdk_local_id_to_string - round-trips the input string")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);

        char* str = dnv_vista_sdk_local_id_to_string(localId);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == validLocalIdStr);

        dnv_vista_sdk_local_id_string_free(str);
        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("dnv_vista_sdk_local_id_free - null is a no-op")
    {
        dnv_vista_sdk_local_id_free(nullptr);
    }

    TEST_CASE("null localId handle - accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_local_id_version(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_primary_item(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_secondary_item(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_is_verbose_mode(nullptr) == 0);
        CHECK(dnv_vista_sdk_local_id_has_custom_tag(nullptr) == 0);
        CHECK(dnv_vista_sdk_local_id_metadata_tag(nullptr, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY) == nullptr);
        CHECK(dnv_vista_sdk_local_id_builder(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_to_string(nullptr) == nullptr);
    }

    TEST_CASE("local_id_from_string_with_errors - valid string yields no errors")
    {
        const char* str = "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature";
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;

        auto* localId = dnv_vista_sdk_local_id_from_string_with_errors(str, &errors);

        REQUIRE(localId != nullptr);
        REQUIRE(errors != nullptr);
        CHECK(dnv_vista_sdk_parsing_errors_has_errors(errors) == 0);
        CHECK(dnv_vista_sdk_parsing_errors_count(errors) == 0);

        dnv_vista_sdk_local_id_free(localId);
        dnv_vista_sdk_parsing_errors_free(errors);
    }

    TEST_CASE("local_id_from_string_with_errors - invalid string yields errors and null")
    {
        const char* str = "/dnv-v2/vis-3-4a/not-a-valid-path/meta/qty-temperature";
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;

        auto* localId = dnv_vista_sdk_local_id_from_string_with_errors(str, &errors);

        CHECK(localId == nullptr);
        REQUIRE(errors != nullptr);
        CHECK(dnv_vista_sdk_parsing_errors_has_errors(errors) == 1);
        CHECK(dnv_vista_sdk_parsing_errors_count(errors) > 0);

        const char* msg = dnv_vista_sdk_parsing_errors_message_at(errors, 0);
        REQUIRE(msg != nullptr);
        CHECK(std::string_view{ msg }.size() > 0);

        dnv_vista_sdk_parsing_errors_free(errors);
    }

    TEST_CASE("local_id_from_string_with_errors - null string returns null and sets last error")
    {
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;

        auto* localId = dnv_vista_sdk_local_id_from_string_with_errors(nullptr, &errors);

        CHECK(localId == nullptr);
        CHECK(errors == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("local_id_from_string_with_errors - null outErrors returns null and sets last error")
    {
        const char* str = "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature";

        auto* localId = dnv_vista_sdk_local_id_from_string_with_errors(str, nullptr);

        CHECK(localId == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);
    }

    TEST_CASE("parsing_errors - type_at and message_at on invalid parse")
    {
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;
        dnv_vista_sdk_local_id_from_string_with_errors("not-a-local-id", &errors);

        REQUIRE(errors != nullptr);
        size_t count = dnv_vista_sdk_parsing_errors_count(errors);
        REQUIRE(count > 0);

        for (size_t i = 0; i < count; ++i)
        {
            const char* type = dnv_vista_sdk_parsing_errors_type_at(errors, i);
            const char* msg = dnv_vista_sdk_parsing_errors_message_at(errors, i);
            CHECK(type != nullptr);
            CHECK(msg != nullptr);
            CHECK(std::string_view{ type }.size() > 0);
            CHECK(std::string_view{ msg }.size() > 0);
        }

        dnv_vista_sdk_parsing_errors_free(errors);
    }

    TEST_CASE("parsing_errors - to_string on invalid parse contains content")
    {
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;
        dnv_vista_sdk_local_id_from_string_with_errors("not-a-local-id", &errors);

        REQUIRE(errors != nullptr);
        char* str = dnv_vista_sdk_parsing_errors_to_string(errors);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } != "Success");

        dnv_vista_sdk_parsing_errors_string_free(str);
        dnv_vista_sdk_parsing_errors_free(errors);
    }

    TEST_CASE("parsing_errors - to_string on valid parse returns Success")
    {
        const char* localIdStr = "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature";
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;
        auto* localId = dnv_vista_sdk_local_id_from_string_with_errors(localIdStr, &errors);

        REQUIRE(errors != nullptr);
        char* str = dnv_vista_sdk_parsing_errors_to_string(errors);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str } == "Success");

        dnv_vista_sdk_parsing_errors_string_free(str);
        dnv_vista_sdk_parsing_errors_free(errors);
        dnv_vista_sdk_local_id_free(localId);
    }

    TEST_CASE("parsing_errors - out of range index returns null")
    {
        dnv_vista_sdk_parsing_errors_t* errors = nullptr;
        dnv_vista_sdk_local_id_from_string_with_errors("not-a-local-id", &errors);

        REQUIRE(errors != nullptr);
        size_t count = dnv_vista_sdk_parsing_errors_count(errors);

        CHECK(dnv_vista_sdk_parsing_errors_type_at(errors, count) == nullptr);
        CHECK(dnv_vista_sdk_parsing_errors_message_at(errors, count) == nullptr);

        dnv_vista_sdk_parsing_errors_free(errors);
    }
}
