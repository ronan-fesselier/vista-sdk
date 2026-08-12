#include <doctest/doctest.h>

#include <dnv/VistaSDK_c_api.h>

#include <string_view>

namespace
{
    constexpr const char* validLocalIdStr =
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet";
} // namespace

TEST_SUITE("c-api::local_id_mqtt")
{
    TEST_CASE("dnv_vista_sdk_local_id_mqtt_create - valid builder returns non-null")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);
        const auto* builder = dnv_vista_sdk_local_id_builder(localId);
        REQUIRE(builder != nullptr);

        auto* mqttId = dnv_vista_sdk_local_id_mqtt_create(builder);
        REQUIRE(mqttId != nullptr);

        dnv_vista_sdk_local_id_free(localId);
        dnv_vista_sdk_local_id_mqtt_free(mqttId);
    }

    TEST_CASE("dnv_vista_sdk_local_id_mqtt_create - null builder returns null")
    {
        CHECK(dnv_vista_sdk_local_id_mqtt_create(nullptr) == nullptr);
    }

    TEST_CASE("dnv_vista_sdk_local_id_mqtt_create - empty builder throws and sets last error")
    {
        auto* builder = dnv_vista_sdk_local_id_builder_create("3-4a");
        REQUIRE(builder != nullptr);

        CHECK(dnv_vista_sdk_local_id_mqtt_create(builder) == nullptr);
        CHECK(std::string_view{ dnv_vista_sdk_last_error_message() }.size() > 0);

        dnv_vista_sdk_local_id_builder_free(builder);
    }

    TEST_CASE("dnv_vista_sdk_local_id_mqtt - accessors reflect the builder state")
    {
        auto* localId = dnv_vista_sdk_local_id_from_string(validLocalIdStr);
        REQUIRE(localId != nullptr);
        const auto* builder = dnv_vista_sdk_local_id_builder(localId);
        REQUIRE(builder != nullptr);

        auto* mqttId = dnv_vista_sdk_local_id_mqtt_create(builder);
        REQUIRE(mqttId != nullptr);

        CHECK(std::string_view{ dnv_vista_sdk_local_id_mqtt_version(mqttId) } == "3-4a");
        CHECK(dnv_vista_sdk_local_id_mqtt_primary_item(mqttId) != nullptr);
        CHECK(dnv_vista_sdk_local_id_mqtt_secondary_item(mqttId) == nullptr);
        CHECK(dnv_vista_sdk_local_id_mqtt_builder(mqttId) != nullptr);

        char* str = dnv_vista_sdk_local_id_mqtt_to_string(mqttId);
        REQUIRE(str != nullptr);
        CHECK(std::string_view{ str }.size() > 0);
        CHECK(std::string_view{ str }[0] != '/');

        dnv_vista_sdk_string_free(str);
        dnv_vista_sdk_local_id_free(localId);
        dnv_vista_sdk_local_id_mqtt_free(mqttId);
    }

    TEST_CASE("dnv_vista_sdk_local_id_mqtt_free - null is a no-op")
    {
        dnv_vista_sdk_local_id_mqtt_free(nullptr);
    }

    TEST_CASE("null localId handle - accessors fail gracefully")
    {
        CHECK(dnv_vista_sdk_local_id_mqtt_version(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_mqtt_primary_item(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_mqtt_secondary_item(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_mqtt_metadata_tag(nullptr, DNV_VISTA_SDK_CODEBOOK_NAME_QUANTITY) == nullptr);
        CHECK(dnv_vista_sdk_local_id_mqtt_builder(nullptr) == nullptr);
        CHECK(dnv_vista_sdk_local_id_mqtt_to_string(nullptr) == nullptr);
    }
}
