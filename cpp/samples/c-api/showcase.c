/**
 * @file showcase.c
 * @brief Demonstrates usage of the vista-sdk C API across its main modules
 * @details End-to-end walkthrough of the C API: VIS versioning, Codebooks, MetadataTag,
 *          ImoNumber, Gmod, GmodPath, LocalId, LocalId MQTT, UniversalId, ISO 19848
 *          value validation, GmodPath/LocalId version conversion, GmodPathQuery,
 *          LocalIdQuery, DataChannelList, and TimeSeriesData with business-rule callbacks
 */

#include <dnv/VistaSDK_c_api_short.h>

#include <stdio.h>

static int onTabularDataPoint(
    vista_date_time_offset_t timeStamp,
    const vista_dcl_data_channel_t* dataChannel,
    const vista_iso19848_value_t* value,
    const char* quality,
    const char** outErrorMessage,
    void* userdata)
{
    (void)timeStamp;
    (void)userdata;

    char* valueStr = vista_iso19848_value_to_string(value);
    const vista_dcl_property_t* property = vista_dcl_data_channel_property(dataChannel);
    const vista_dcl_unit_t* unit = vista_dcl_property_unit(property);
    const vista_dcl_range_t* range = vista_dcl_property_range(property);

    printf("  Tabular data point:\n");
    printf("    Value  : %s", valueStr);
    if (unit != NULL)
    {
        printf(" %s", vista_dcl_unit_unit_symbol(unit));
    }
    printf("\n");
    if (quality != NULL)
    {
        printf("    Quality: %s\n", quality);
    }

    if (range != NULL)
    {
        vista_decimal_t decVal;
        if (vista_decimal_from_string(valueStr, &decVal))
        {
            vista_decimal_t high = vista_decimal_from_double(vista_dcl_range_high(range));
            vista_decimal_t threshold = vista_decimal_multiply(high, vista_decimal_from_double(0.9));
            if (vista_decimal_compare(decVal, threshold) > 0)
            {
                const char* priority = vista_dcl_property_alert_priority(property);
                printf(
                    "    [!] ALARM (%s): value approaching upper limit of %.1f\n",
                    priority != NULL ? priority : "unclassified",
                    vista_dcl_range_high(range));
                if (outErrorMessage != NULL)
                {
                    *outErrorMessage = "value approaching upper limit";
                }
                vista_string_free(valueStr);
                return 0;
            }
        }
    }

    vista_string_free(valueStr);
    return 1;
}

static int onEventDataPoint(
    vista_date_time_offset_t timeStamp,
    const vista_dcl_data_channel_t* dataChannel,
    const vista_iso19848_value_t* value,
    const char* quality,
    const char** outErrorMessage,
    void* userdata)
{
    (void)dataChannel;
    (void)outErrorMessage;
    (void)userdata;

    char* valueStr = vista_iso19848_value_to_string(value);
    char* tsStr = vista_date_time_offset_to_string(timeStamp, VISTA_DATE_TIME_FORMAT_ISO8601);

    printf("  Event data point:\n");
    printf("    Value    : %s\n", valueStr);
    printf("    Timestamp: %s\n", tsStr);
    if (quality != NULL)
    {
        printf("    Quality  : %s\n", quality);
    }

    vista_string_free(tsStr);
    vista_string_free(valueStr);
    return 1;
}

int main(void)
{
    printf("=== vista-sdk C API Sample ===\n\n");

    const vista_vis_t* vis = vista_vis_instance();
    const char* latest = vista_vis_latest(vis);

    {
        printf("1. VIS: Version management\n");
        printf("-------------------------------------------------------\n");

        printf("SDK version: %s\n", dnv_vista_sdk_version());

        size_t versionCount = vista_vis_version_count(vis);

        printf("Latest VIS version: %s\n", latest);
        printf("Available versions (%zu):\n", versionCount);
        for (size_t i = 0; i < versionCount; ++i)
        {
            printf("  - %s\n", vista_vis_version_at(vis, i));
        }
        printf("\n");
    }

    const vista_codebooks_t* codebooks = vista_vis_codebooks(vis, latest);

    {
        printf("2. Codebooks: Accessing a codebook and its standard values\n");
        printf("-------------------------------------------------------\n");

        const vista_codebook_t* quantity = vista_codebooks_at(codebooks, VISTA_CODEBOOK_NAME_QUANTITY);

        printf("Codebook: %s\n", vista_codebook_names_to_string(VISTA_CODEBOOK_NAME_QUANTITY));
        printf(
            "Has 'temperature'? %s\n", vista_codebook_has_standard_value(quantity, "temperature") ? "true" : "false");
        printf(
            "Has 'invalid_qty'? %s\n", vista_codebook_has_standard_value(quantity, "invalid_qty") ? "true" : "false");
        printf("\n");
    }

    {
        printf("3. MetadataTag: Creating a tag from a standard value\n");
        printf("-------------------------------------------------------\n");

        const vista_codebook_t* quantity = vista_codebooks_at(codebooks, VISTA_CODEBOOK_NAME_QUANTITY);
        vista_metadata_tag_t* tempTag = vista_codebook_create_tag(quantity, "temperature");

        if (tempTag != NULL)
        {
            char* tagStr = vista_metadata_tag_to_string(tempTag);

            printf("Temperature tag created:\n");
            printf("  Value:       %s\n", vista_metadata_tag_value(tempTag));
            printf("  Is Custom:   %s\n", vista_metadata_tag_is_custom(tempTag) ? "true" : "false");
            printf("  String repr: %s\n", tagStr);

            vista_string_free(tagStr);
            vista_metadata_tag_free(tempTag);
        }
        printf("\n");
    }

    {
        printf("4. ImoNumber: Parsing and validating an IMO ship number\n");
        printf("-------------------------------------------------------\n");

        vista_imo_number_t* imo = vista_imo_number_from_string("IMO9074729");

        if (imo != NULL)
        {
            char* imoStr = vista_imo_number_to_string(imo);
            printf("Parsed: %s\n", imoStr);
            vista_string_free(imoStr);
            vista_imo_number_free(imo);
        }

        printf("Is 9074729 valid? %s\n", vista_imo_number_is_valid(9074729) ? "true" : "false");
        printf("Is 1234507 valid? %s (bad checksum)\n", vista_imo_number_is_valid(1234507) ? "true" : "false");
        printf("\n");
    }

    const vista_gmod_t* gmod = vista_vis_gmod(vis, latest);

    {
        printf("5. Gmod: Looking up a node and its hierarchy\n");
        printf("-------------------------------------------------------\n");

        const vista_gmod_node_t* node = vista_gmod_get_node(gmod, "411.1");

        if (node != NULL)
        {
            char* nodeStr = vista_gmod_node_to_string(node);

            printf("Node:       %s\n", nodeStr);
            printf("Is leaf?    %s\n", vista_gmod_node_is_leaf_node(node) ? "true" : "false");
            printf("Is mappable? %s\n", vista_gmod_node_is_mappable(node) ? "true" : "false");
            printf("Parents:    %zu\n", vista_gmod_node_parent_count(node));
            printf("Children:   %zu\n", vista_gmod_node_child_count(node));

            vista_string_free(nodeStr);
        }
        printf("\n");
    }

    {
        printf("6. GmodPath: Parsing a short path\n");
        printf("-------------------------------------------------------\n");

        const vista_locations_t* locations = vista_vis_locations(vis, latest);
        vista_gmod_path_t* path = vista_gmod_path_from_short_path("411.1", gmod, locations);

        if (path != NULL)
        {
            char* pathStr = vista_gmod_path_to_string(path);
            char* fullPathStr = vista_gmod_path_to_full_path_string(path);

            printf("Short path: %s\n", pathStr);
            printf("Full path:  %s\n", fullPathStr);
            printf("Length:     %zu node(s)\n", vista_gmod_path_length(path));
            printf("Mappable?   %s\n", vista_gmod_path_is_mappable(path) ? "true" : "false");

            vista_string_free(pathStr);
            vista_string_free(fullPathStr);
            vista_gmod_path_free(path);
        }
        else
        {
            printf("Parse failed: %s\n", vista_last_error_message());
        }
        printf("\n");
    }

    {
        printf("7. LocalId: Parsing and inspecting a LocalId string\n");
        printf("-------------------------------------------------------\n");

        const char* localIdStr = "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature";
        vista_local_id_t* localId = vista_local_id_from_string(localIdStr);

        if (localId != NULL)
        {
            const vista_gmod_path_t* primaryItem = vista_local_id_primary_item(localId);
            char* primaryItemStr = vista_gmod_path_to_string(primaryItem);
            const vista_metadata_tag_t* quantityTag =
                vista_local_id_metadata_tag(localId, VISTA_CODEBOOK_NAME_QUANTITY);

            printf("Parsing:      %s\n", localIdStr);
            printf("VIS Version:  %s\n", vista_local_id_version(localId));
            printf("Primary item: %s\n", primaryItemStr);
            if (quantityTag != NULL)
            {
                printf("Quantity:     %s\n", vista_metadata_tag_value(quantityTag));
            }

            vista_string_free(primaryItemStr);
            vista_local_id_free(localId);
        }
        else
        {
            printf("Parse failed: %s\n", vista_last_error_message());
        }

        printf("\n");

        const char* invalidLocalIdStr = "//vis-3-4a/not-a-valid-path/meta/qty-temperature";
        vista_parsing_errors_t* parseErrors = NULL;
        vista_local_id_t* invalidLocalId = vista_local_id_from_string_with_errors(invalidLocalIdStr, &parseErrors);

        if (invalidLocalId != NULL)
        {
            vista_local_id_free(invalidLocalId);
        }
        else
        {
            printf("Parsing:      %s\n", invalidLocalIdStr);
            printf("Parse errors (%zu):\n", vista_parsing_errors_count(parseErrors));
            for (size_t i = 0; i < vista_parsing_errors_count(parseErrors); ++i)
            {
                printf(
                    "  [%s] %s\n",
                    vista_parsing_errors_type_at(parseErrors, i),
                    vista_parsing_errors_message_at(parseErrors, i));
            }
        }

        vista_parsing_errors_free(parseErrors);
        printf("\n");
    }

    {
        printf("8. LocalId MQTT: MQTT-compatible topic formatting\n");
        printf("-------------------------------------------------------\n");

        const vista_locations_t* locations = vista_vis_locations(vis, latest);
        vista_gmod_path_t* primaryItem = vista_gmod_path_from_short_path("411.1/C101.31-2", gmod, locations);
        const vista_codebook_t* qtyBook = vista_codebooks_at(codebooks, VISTA_CODEBOOK_NAME_QUANTITY);
        vista_metadata_tag_t* tempTag = vista_codebook_create_tag(qtyBook, "temperature");

        if (primaryItem != NULL && tempTag != NULL)
        {
            vista_local_id_builder_t* builder = vista_local_id_builder_create(latest);
            vista_local_id_builder_t* builder2 = vista_local_id_builder_with_primary_item(builder, primaryItem);
            vista_local_id_builder_t* builder3 = vista_local_id_builder_with_metadata_tag(builder2, tempTag);

            vista_local_id_mqtt_t* mqttId = vista_local_id_mqtt_create(builder3);

            if (mqttId != NULL)
            {
                char* stdStr = vista_local_id_builder_to_string(builder3);
                char* mqttStr = vista_local_id_mqtt_to_string(mqttId);

                printf("Standard : %s\n", stdStr);
                printf("MQTT     : %s\n", mqttStr);
                printf("Difference: '/' replaced by '+' as MQTT wildcard separator\n");

                vista_string_free(stdStr);
                vista_string_free(mqttStr);
                vista_local_id_mqtt_free(mqttId);
            }

            vista_local_id_builder_free(builder3);
            vista_local_id_builder_free(builder2);
            vista_local_id_builder_free(builder);
        }

        if (tempTag != NULL)
        {
            vista_metadata_tag_free(tempTag);
        }
        if (primaryItem != NULL)
        {
            vista_gmod_path_free(primaryItem);
        }
        printf("\n");
    }

    {
        printf("9. UniversalId: Globally unique sensor identifier\n");
        printf("-------------------------------------------------------\n");

        vista_imo_number_t* imo = vista_imo_number_from_string("IMO9074729");
        const vista_locations_t* locations = vista_vis_locations(vis, latest);
        vista_gmod_path_t* primaryItem = vista_gmod_path_from_short_path("411.1/C101.31-2", gmod, locations);
        const vista_codebook_t* qtyBook = vista_codebooks_at(codebooks, VISTA_CODEBOOK_NAME_QUANTITY);
        vista_metadata_tag_t* tempTag = vista_codebook_create_tag(qtyBook, "temperature");

        if (imo != NULL && primaryItem != NULL && tempTag != NULL)
        {
            vista_local_id_builder_t* localIdBuilder = vista_local_id_builder_create(latest);
            vista_local_id_builder_t* localIdBuilder2 =
                vista_local_id_builder_with_primary_item(localIdBuilder, primaryItem);
            vista_local_id_builder_t* localIdBuilder3 =
                vista_local_id_builder_with_metadata_tag(localIdBuilder2, tempTag);

            vista_universal_id_builder_t* uidBuilder = vista_universal_id_builder_create(latest);
            vista_universal_id_builder_t* uidBuilder2 = vista_universal_id_builder_with_imo_number(uidBuilder, imo);
            vista_universal_id_builder_t* uidBuilder3 =
                vista_universal_id_builder_with_local_id(uidBuilder2, localIdBuilder3);

            vista_universal_id_t* uid = vista_universal_id_builder_build(uidBuilder3);

            if (uid != NULL)
            {
                char* uidStr = vista_universal_id_to_string(uid);
                const vista_imo_number_t* uidImo = vista_universal_id_imo_number(uid);
                char* imoStr = vista_imo_number_to_string(uidImo);

                printf("UniversalId: %s\n", uidStr);
                printf("  Naming entity: %s\n", vista_universal_id_naming_entity());
                printf("  IMO Number   : %s\n", imoStr);

                vista_string_free(imoStr);
                vista_string_free(uidStr);
                vista_universal_id_free(uid);
            }

            vista_universal_id_builder_free(uidBuilder3);
            vista_universal_id_builder_free(uidBuilder2);
            vista_universal_id_builder_free(uidBuilder);
            vista_local_id_builder_free(localIdBuilder3);
            vista_local_id_builder_free(localIdBuilder2);
            vista_local_id_builder_free(localIdBuilder);
        }

        if (tempTag != NULL)
        {
            vista_metadata_tag_free(tempTag);
        }
        if (primaryItem != NULL)
        {
            vista_gmod_path_free(primaryItem);
        }
        if (imo != NULL)
        {
            vista_imo_number_free(imo);
        }
        printf("\n");
    }

    {
        printf("10. ISO19848: Validating a value against a FormatDataType\n");
        printf("-------------------------------------------------------\n");

        const vista_iso19848_t* iso19848 = vista_iso19848_instance();
        vista_iso19848_version_t isoVersion = vista_iso19848_latest(iso19848);
        vista_iso19848_format_data_types_t* formatDataTypes = vista_iso19848_format_data_types(iso19848, isoVersion);
        vista_iso19848_format_data_type_t* decimalType =
            vista_iso19848_format_data_types_from_string(formatDataTypes, "Decimal");

        if (decimalType != NULL)
        {
            vista_iso19848_value_t* value = NULL;
            const char* testValue = "23.5";

            if (vista_iso19848_format_data_type_validate(decimalType, testValue, &value))
            {
                char* valueStr = vista_iso19848_value_to_string(value);

                printf("'%s' as Decimal: valid -> %s\n", testValue, valueStr);

                vista_string_free(valueStr);
                vista_iso19848_value_free(value);
            }
            else
            {
                printf("'%s' as Decimal: invalid -> %s\n", testValue, vista_last_error_message());
            }

            const char* badValue = "not-a-number";
            vista_clear_error();
            if (!vista_iso19848_format_data_type_validate(decimalType, badValue, &value))
            {
                printf("'%s' as Decimal: invalid\n", badValue);
                printf("  message: %s\n", vista_last_error_message());
                printf("  kind   : %d (DOMAIN=%d)\n", vista_last_error_kind(), VISTA_ERROR_DOMAIN);
            }

            vista_iso19848_format_data_type_free(decimalType);
        }

        vista_iso19848_format_data_types_free(formatDataTypes);
        printf("\n");
    }

    {
        printf("11. VIS versioning: Converting a GmodPath between versions\n");
        printf("-------------------------------------------------------\n");

        const char* sourceVersion = "3-4a";
        const vista_gmod_t* sourceGmod = vista_vis_gmod(vis, sourceVersion);
        const vista_locations_t* sourceLocations = vista_vis_locations(vis, sourceVersion);
        vista_gmod_path_t* sourcePath = vista_gmod_path_from_short_path("511.331/C221", sourceGmod, sourceLocations);

        if (sourcePath != NULL)
        {
            char* sourcePathStr = vista_gmod_path_to_string(sourcePath);
            printf("Source (%s): %s (depth: %zu)\n", sourceVersion, sourcePathStr, vista_gmod_path_length(sourcePath));
            vista_string_free(sourcePathStr);

            vista_gmod_path_t* targetPath = vista_vis_convert_path(vis, sourceVersion, sourcePath, latest);

            if (targetPath != NULL)
            {
                char* targetPathStr = vista_gmod_path_to_string(targetPath);
                printf("Target (%s): %s (depth: %zu)\n", latest, targetPathStr, vista_gmod_path_length(targetPath));

                vista_string_free(targetPathStr);
                vista_gmod_path_free(targetPath);
            }
            else
            {
                printf("Conversion failed: %s\n", vista_last_error_message());
            }

            vista_gmod_path_free(sourcePath);
        }
        printf("\n");
    }

    {
        printf("12. VIS versioning: Converting a LocalId between versions\n");
        printf("-------------------------------------------------------\n");

        const char* sourceLocalIdStr = "/dnv-v2/vis-3-4a/411.1/C101/sec/411.1/C101.64i/S201/meta/cnt-condensate";
        vista_local_id_t* sourceLocalId = vista_local_id_from_string(sourceLocalIdStr);

        if (sourceLocalId != NULL)
        {
            printf("Source: %s\n", sourceLocalIdStr);

            vista_local_id_t* targetLocalId = vista_vis_convert_local_id(vis, sourceLocalId, "3-5a");

            if (targetLocalId != NULL)
            {
                char* targetLocalIdStr = vista_local_id_to_string(targetLocalId);
                printf("Target: %s\n", targetLocalIdStr);

                vista_string_free(targetLocalIdStr);
                vista_local_id_free(targetLocalId);
            }
            else
            {
                printf("Conversion failed: %s\n", vista_last_error_message());
            }

            vista_local_id_free(sourceLocalId);
        }
        printf("\n");
    }

    {
        printf("13. GmodPathQuery: Matching a base path while ignoring locations\n");
        printf("-------------------------------------------------------\n");

        const vista_locations_t* locations = vista_vis_locations(vis, latest);
        vista_gmod_path_t* basePath = vista_gmod_path_from_short_path("411.1/C101.31", gmod, locations);
        vista_gmod_path_t* matchPath = vista_gmod_path_from_short_path("411.1/C101.31-2", gmod, locations);
        vista_gmod_path_t* mismatchPath = vista_gmod_path_from_short_path("411.1/C101.63", gmod, locations);

        if (basePath != NULL && matchPath != NULL && mismatchPath != NULL)
        {
            vista_gmod_path_query_builder_t* builder = vista_gmod_path_query_builder_from(basePath);
            vista_gmod_path_query_builder_t* withoutLocations =
                vista_gmod_path_query_builder_without_locations(builder);
            vista_gmod_path_query_t* query = vista_gmod_path_query_builder_build(withoutLocations);

            printf("Query: match '411.1/C101.31' ignoring locations\n");
            printf("  411.1/C101.31-2 matches: %s\n", vista_gmod_path_query_match(query, matchPath) ? "true" : "false");
            printf(
                "  411.1/C101.63   matches: %s (different node)\n",
                vista_gmod_path_query_match(query, mismatchPath) ? "true" : "false");

            vista_gmod_path_query_free(query);
            vista_gmod_path_query_builder_free(withoutLocations);
            vista_gmod_path_query_builder_free(builder);
        }

        if (mismatchPath != NULL)
        {
            vista_gmod_path_free(mismatchPath);
        }
        if (matchPath != NULL)
        {
            vista_gmod_path_free(matchPath);
        }
        if (basePath != NULL)
        {
            vista_gmod_path_free(basePath);
        }
        printf("\n");
    }

    {
        printf("14. LocalIdQuery: Combining a primary-item path query with a tags query\n");
        printf("-------------------------------------------------------\n");

        vista_local_id_t* localId1 =
            vista_local_id_from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature");
        vista_local_id_t* localId2 = vista_local_id_from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-pressure");

        const vista_gmod_node_t* node411 = vista_gmod_get_node(gmod, "411.1");
        const vista_gmod_node_t* nodeC101 = vista_gmod_get_node(gmod, "C101");

        if (localId1 != NULL && localId2 != NULL && node411 != NULL && nodeC101 != NULL)
        {
            vista_gmod_path_query_builder_t* pathBuilder = vista_gmod_path_query_builder_create();
            vista_gmod_path_query_builder_t* pathBuilder2 =
                vista_gmod_path_query_builder_with_node_all_locations(pathBuilder, node411, 1);
            vista_gmod_path_query_builder_t* pathBuilder3 =
                vista_gmod_path_query_builder_with_node_all_locations(pathBuilder2, nodeC101, 1);
            vista_gmod_path_query_t* pathQuery = vista_gmod_path_query_builder_build(pathBuilder3);

            vista_metadata_tags_query_builder_t* tagsBuilder = vista_metadata_tags_query_builder_create();
            vista_metadata_tags_query_builder_t* tagsBuilder2 =
                vista_metadata_tags_query_builder_with_tag(tagsBuilder, VISTA_CODEBOOK_NAME_QUANTITY, "temperature");
            vista_metadata_tags_query_t* tagsQuery = vista_metadata_tags_query_builder_build(tagsBuilder2);

            vista_local_id_query_builder_t* localIdBuilder = vista_local_id_query_builder_create();
            vista_local_id_query_builder_t* localIdBuilder2 =
                vista_local_id_query_builder_with_primary_item_query(localIdBuilder, pathQuery);
            vista_local_id_query_builder_t* localIdBuilder3 =
                vista_local_id_query_builder_with_tags(localIdBuilder2, tagsQuery);
            vista_local_id_query_t* query = vista_local_id_query_builder_build(localIdBuilder3);

            printf("Query: primary='411.1/C101' (any location) AND qty-temperature\n");
            printf("  qty-temperature matches: %s\n", vista_local_id_query_match(query, localId1) ? "true" : "false");
            printf(
                "  qty-pressure    matches: %s (wrong tag)\n",
                vista_local_id_query_match(query, localId2) ? "true" : "false");

            vista_local_id_query_free(query);
            vista_local_id_query_builder_free(localIdBuilder3);
            vista_local_id_query_builder_free(localIdBuilder2);
            vista_local_id_query_builder_free(localIdBuilder);
            vista_metadata_tags_query_free(tagsQuery);
            vista_metadata_tags_query_builder_free(tagsBuilder2);
            vista_metadata_tags_query_builder_free(tagsBuilder);
            vista_gmod_path_query_free(pathQuery);
            vista_gmod_path_query_builder_free(pathBuilder3);
            vista_gmod_path_query_builder_free(pathBuilder2);
            vista_gmod_path_query_builder_free(pathBuilder);
        }

        if (localId2 != NULL)
        {
            vista_local_id_free(localId2);
        }
        if (localId1 != NULL)
        {
            vista_local_id_free(localId1);
        }
        printf("\n");
    }

    vista_dcl_list_package_t* dclListPackage = NULL;

    {
        printf("15. DataChannelList: Building a package and serializing to JSON\n");
        printf("-------------------------------------------------------\n");

        const vista_locations_t* locations = vista_vis_locations(vis, latest);

        vista_ship_id_t* shipId = vista_ship_id_from_string("IMO8027781");

        vista_date_time_offset_t configTimestamp;
        vista_date_time_offset_from_string("2026-08-13T10:00:00Z", &configTimestamp);
        vista_dcl_configuration_reference_t* configRef =
            vista_dcl_configuration_reference_create("vessel-config-2026-v1", configTimestamp);

        vista_dcl_header_t* header = vista_dcl_header_create(shipId, configRef);

        // Channels 1 & 2: GPS latitude and longitude (tabular, continuous measures)
        vista_gmod_path_t* latItem = vista_gmod_path_from_short_path("710.1/F211.11", gmod, locations);
        vista_gmod_path_t* lonItem = vista_gmod_path_from_short_path("710.1/F211.12", gmod, locations);
        vista_metadata_tag_t* latQtyTag =
            vista_codebook_create_tag(vista_codebooks_at(codebooks, VISTA_CODEBOOK_NAME_QUANTITY), "latitude");
        vista_metadata_tag_t* lonQtyTag =
            vista_codebook_create_tag(vista_codebooks_at(codebooks, VISTA_CODEBOOK_NAME_QUANTITY), "longitude");

        vista_local_id_builder_t* latBuilder = vista_local_id_builder_create(latest);
        vista_local_id_builder_t* latBuilder2 = vista_local_id_builder_with_primary_item(latBuilder, latItem);
        vista_local_id_builder_t* latBuilder3 = vista_local_id_builder_with_metadata_tag(latBuilder2, latQtyTag);
        vista_local_id_t* latLocalId = vista_local_id_builder_build(latBuilder3);

        vista_local_id_builder_t* lonBuilder = vista_local_id_builder_create(latest);
        vista_local_id_builder_t* lonBuilder2 = vista_local_id_builder_with_primary_item(lonBuilder, lonItem);
        vista_local_id_builder_t* lonBuilder3 = vista_local_id_builder_with_metadata_tag(lonBuilder2, lonQtyTag);
        vista_local_id_t* lonLocalId = vista_local_id_builder_build(lonBuilder3);

        vista_dcl_data_channel_type_t* instType = vista_dcl_data_channel_type_create("Inst");
        vista_dcl_format_t* decimalFormat = vista_dcl_format_create("Decimal");
        vista_dcl_unit_t* degUnit = vista_dcl_unit_create("deg");

        vista_dcl_channel_id_t* latChannelId = vista_dcl_channel_id_create(latLocalId);
        vista_dcl_channel_id_set_short_id(latChannelId, "GPSLatitude");
        vista_dcl_range_t* latRange = vista_dcl_range_create(-90.0, 90.0);
        vista_dcl_property_t* latProperty = vista_dcl_property_create(instType, decimalFormat);
        vista_dcl_property_set_range(latProperty, latRange);
        vista_dcl_property_set_unit(latProperty, degUnit);
        vista_dcl_data_channel_t* latChannel = vista_dcl_data_channel_create(latChannelId, latProperty);

        vista_dcl_channel_id_t* lonChannelId = vista_dcl_channel_id_create(lonLocalId);
        vista_dcl_channel_id_set_short_id(lonChannelId, "GPSLongitude");
        vista_dcl_range_t* lonRange = vista_dcl_range_create(-180.0, 180.0);
        vista_dcl_property_t* lonProperty = vista_dcl_property_create(instType, decimalFormat);
        vista_dcl_property_set_range(lonProperty, lonRange);
        vista_dcl_property_set_unit(lonProperty, degUnit);
        vista_dcl_data_channel_t* lonChannel = vista_dcl_data_channel_create(lonChannelId, lonProperty);

        // Channel 3: main engine exhaust temperature (tabular, machinery)
        vista_gmod_path_t* exhaustItem = vista_gmod_path_from_short_path("411.1/C101", gmod, locations);
        vista_metadata_tag_t* tempQtyTag =
            vista_codebook_create_tag(vista_codebooks_at(codebooks, VISTA_CODEBOOK_NAME_QUANTITY), "temperature");

        vista_local_id_builder_t* exhaustBuilder = vista_local_id_builder_create(latest);
        vista_local_id_builder_t* exhaustBuilder2 =
            vista_local_id_builder_with_primary_item(exhaustBuilder, exhaustItem);
        vista_local_id_builder_t* exhaustBuilder3 =
            vista_local_id_builder_with_metadata_tag(exhaustBuilder2, tempQtyTag);
        vista_local_id_t* exhaustLocalId = vista_local_id_builder_build(exhaustBuilder3);

        vista_dcl_channel_id_t* exhaustChannelId = vista_dcl_channel_id_create(exhaustLocalId);
        vista_dcl_channel_id_set_short_id(exhaustChannelId, "ExhaustTemp");

        vista_dcl_range_t* exhaustRange = vista_dcl_range_create(0.0, 450.0);
        vista_dcl_unit_t* celsiusUnit = vista_dcl_unit_create("deg C");
        vista_dcl_property_t* exhaustProperty = vista_dcl_property_create(instType, decimalFormat);
        vista_dcl_property_set_range(exhaustProperty, exhaustRange);
        vista_dcl_property_set_unit(exhaustProperty, celsiusUnit);
        vista_dcl_property_set_alert_priority(exhaustProperty, "high-temperature");
        vista_dcl_data_channel_t* exhaustChannel = vista_dcl_data_channel_create(exhaustChannelId, exhaustProperty);

        // Channel 4: turbocharger overspeed alarm (event, discrete state)
        vista_gmod_path_t* alarmItem = vista_gmod_path_from_short_path("411.1/C101", gmod, locations);
        vista_metadata_tag_t* stateTag =
            vista_codebook_create_tag(vista_codebooks_at(codebooks, VISTA_CODEBOOK_NAME_STATE), "overspeed");

        vista_local_id_builder_t* alarmBuilder = vista_local_id_builder_create(latest);
        vista_local_id_builder_t* alarmBuilder2 = vista_local_id_builder_with_primary_item(alarmBuilder, alarmItem);
        vista_local_id_builder_t* alarmBuilder3 = vista_local_id_builder_with_metadata_tag(alarmBuilder2, stateTag);
        vista_local_id_t* alarmLocalId = vista_local_id_builder_build(alarmBuilder3);

        vista_dcl_channel_id_t* alarmChannelId = vista_dcl_channel_id_create(alarmLocalId);
        vista_dcl_channel_id_set_short_id(alarmChannelId, "TurboOverspeedAlarm");

        vista_dcl_data_channel_type_t* eventType = vista_dcl_data_channel_type_create("Inst");
        vista_dcl_format_t* stringFormat = vista_dcl_format_create("String");

        vista_dcl_property_t* alarmProperty = vista_dcl_property_create(eventType, stringFormat);

        vista_dcl_data_channel_t* alarmChannel = vista_dcl_data_channel_create(alarmChannelId, alarmProperty);

        vista_dcl_data_channel_list_t* dataChannelList = vista_dcl_data_channel_list_create();
        vista_dcl_data_channel_list_add(dataChannelList, latChannel);
        vista_dcl_data_channel_list_add(dataChannelList, lonChannel);
        vista_dcl_data_channel_list_add(dataChannelList, exhaustChannel);
        vista_dcl_data_channel_list_add(dataChannelList, alarmChannel);

        vista_dcl_package_t* package = vista_dcl_package_create(header, dataChannelList);
        dclListPackage = vista_dcl_list_package_create(package);

        printf("Created DataChannelList package:\n");
        printf("  Ship ID      : %s\n", vista_ship_id_to_string(shipId));
        printf("  Channel count: %zu\n", vista_dcl_data_channel_list_size(dataChannelList));

        char* json = vista_dcl_list_package_to_json(dclListPackage, 1);
        printf("\nJSON output:\n%s\n", json);
        vista_string_free(json);

        vista_dcl_package_free(package);
        vista_dcl_data_channel_list_free(dataChannelList);
        vista_dcl_data_channel_free(alarmChannel);
        vista_dcl_property_free(alarmProperty);
        vista_dcl_format_free(stringFormat);
        vista_dcl_data_channel_type_free(eventType);
        vista_dcl_channel_id_free(alarmChannelId);
        vista_local_id_free(alarmLocalId);
        vista_local_id_builder_free(alarmBuilder3);
        vista_local_id_builder_free(alarmBuilder2);
        vista_local_id_builder_free(alarmBuilder);
        vista_metadata_tag_free(stateTag);
        vista_gmod_path_free(alarmItem);
        vista_dcl_data_channel_free(exhaustChannel);
        vista_dcl_property_free(exhaustProperty);
        vista_dcl_range_free(exhaustRange);
        vista_dcl_unit_free(celsiusUnit);
        vista_dcl_channel_id_free(exhaustChannelId);
        vista_local_id_free(exhaustLocalId);
        vista_local_id_builder_free(exhaustBuilder3);
        vista_local_id_builder_free(exhaustBuilder2);
        vista_local_id_builder_free(exhaustBuilder);
        vista_metadata_tag_free(tempQtyTag);
        vista_gmod_path_free(exhaustItem);
        vista_dcl_data_channel_free(lonChannel);
        vista_dcl_property_free(lonProperty);
        vista_dcl_range_free(lonRange);
        vista_dcl_channel_id_free(lonChannelId);
        vista_dcl_data_channel_free(latChannel);
        vista_dcl_property_free(latProperty);
        vista_dcl_range_free(latRange);
        vista_dcl_channel_id_free(latChannelId);
        vista_dcl_unit_free(degUnit);
        vista_dcl_format_free(decimalFormat);
        vista_dcl_data_channel_type_free(instType);
        vista_local_id_free(lonLocalId);
        vista_local_id_builder_free(lonBuilder3);
        vista_local_id_builder_free(lonBuilder2);
        vista_local_id_builder_free(lonBuilder);
        vista_metadata_tag_free(lonQtyTag);
        vista_gmod_path_free(lonItem);
        vista_local_id_free(latLocalId);
        vista_local_id_builder_free(latBuilder3);
        vista_local_id_builder_free(latBuilder2);
        vista_local_id_builder_free(latBuilder);
        vista_metadata_tag_free(latQtyTag);
        vista_gmod_path_free(latItem);
        vista_dcl_header_free(header);
        vista_dcl_configuration_reference_free(configRef);
        vista_ship_id_free(shipId);
        printf("\n");
    }

    {
        printf("16. TimeSeriesData: Building a package and cross-validating against the DataChannelList\n");
        printf("-------------------------------------------------------\n");

        vista_ship_id_t* shipId = vista_ship_id_from_string("IMO8027781");
        vista_tsd_header_t* header = vista_tsd_header_create(shipId);

        vista_date_time_offset_t configTimestamp;
        vista_date_time_offset_from_string("2026-08-13T00:00:00Z", &configTimestamp);
        vista_tsd_config_ref_t* dataConfig = vista_tsd_config_ref_create("vessel-config-2026-v1", configTimestamp);

        vista_tsd_channel_id_t* latChannelId = vista_tsd_channel_id_from_string("GPSLatitude");
        vista_tsd_channel_id_t* lonChannelId = vista_tsd_channel_id_from_string("GPSLongitude");
        vista_tsd_channel_id_t* exhaustChannelId = vista_tsd_channel_id_from_string("ExhaustTemp");

        vista_date_time_offset_t sampleTimestamp;
        vista_date_time_offset_from_string("2026-08-13T10:00:00Z", &sampleTimestamp);
        const char* values[] = { "45.123456", "5.987654", "420.0" };
        vista_tsd_tabular_data_set_t* dataSet = vista_tsd_tabular_data_set_create(sampleTimestamp, values, 3);

        const vista_tsd_channel_id_t* channelIds[] = { latChannelId, lonChannelId, exhaustChannelId };
        const vista_tsd_tabular_data_set_t* dataSets[] = { dataSet };
        vista_tsd_tabular_data_t* tabularData = vista_tsd_tabular_data_create(channelIds, 3, dataSets, 1);

        // Event: turbocharger overspeed alarm trigger
        vista_tsd_channel_id_t* alarmChannelId = vista_tsd_channel_id_from_string("TurboOverspeedAlarm");

        vista_date_time_offset_t eventTimestamp;
        vista_date_time_offset_from_string("2026-08-13T10:00:42Z", &eventTimestamp);
        vista_tsd_event_data_set_t* eventDataSet =
            vista_tsd_event_data_set_create(eventTimestamp, alarmChannelId, "alarm.active");

        vista_tsd_event_data_t* eventData = vista_tsd_event_data_create();
        const vista_tsd_event_data_set_t* eventDataSets[] = { eventDataSet };
        vista_tsd_event_data_set_data_set(eventData, eventDataSets, 1);

        vista_tsd_time_series_data_t* timeSeriesData = vista_tsd_time_series_data_create();
        vista_tsd_time_series_data_set_data_configuration(timeSeriesData, dataConfig);
        const vista_tsd_tabular_data_t* tabularDataEntries[] = { tabularData };
        vista_tsd_time_series_data_set_tabular_data(timeSeriesData, tabularDataEntries, 1);
        vista_tsd_time_series_data_set_event_data(timeSeriesData, eventData);

        const vista_tsd_time_series_data_t* timeSeriesDataEntries[] = { timeSeriesData };
        vista_tsd_package_t* package = vista_tsd_package_create(header, timeSeriesDataEntries, 1);
        vista_tsd_data_package_t* dataPackage = vista_tsd_data_package_create(package);

        printf("Created TimeSeriesData package:\n");
        printf("  Ship ID       : %s\n", vista_ship_id_to_string(shipId));
        printf("  Tabular tables: %zu\n", vista_tsd_time_series_data_tabular_data_count(timeSeriesData));
        printf(
            "  Event data    : %s\n",
            vista_tsd_time_series_data_event_data(timeSeriesData) != NULL ? "present" : "absent");

        if (dclListPackage != NULL)
        {
            printf("\nCross-validating against the DataChannelList built in section 12:\n");
            int valid = vista_tsd_time_series_data_validate(
                timeSeriesData, dclListPackage, onTabularDataPoint, NULL, onEventDataPoint, NULL);
            printf("Cross-validation result: %s\n", valid ? "valid" : "invalid");
            if (!valid)
            {
                printf("  Error: %s\n", vista_last_error_message());
            }
        }

        char* json = vista_tsd_data_package_to_json(dataPackage, 1);
        printf("\nJSON output:\n%s\n", json);
        vista_string_free(json);

        vista_tsd_data_package_free(dataPackage);
        vista_tsd_package_free(package);
        vista_tsd_time_series_data_free(timeSeriesData);
        vista_tsd_tabular_data_free(tabularData);
        vista_tsd_tabular_data_set_free(dataSet);
        vista_tsd_event_data_free(eventData);
        vista_tsd_event_data_set_free(eventDataSet);
        vista_tsd_channel_id_free(alarmChannelId);
        vista_tsd_channel_id_free(exhaustChannelId);
        vista_tsd_channel_id_free(lonChannelId);
        vista_tsd_channel_id_free(latChannelId);
        vista_tsd_config_ref_free(dataConfig);
        vista_tsd_header_free(header);
        vista_ship_id_free(shipId);
        printf("\n");
    }

    if (dclListPackage != NULL)
    {
        vista_dcl_list_package_free(dclListPackage);
    }

    return 0;
}
