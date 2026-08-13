/**
 * @file showcase.c
 * @brief Demonstrates usage of the vista-sdk C API across its main modules
 * @details This sample shows how to use the VIS singleton, browse Codebooks, walk the
 *          Gmod tree, parse a GmodPath, parse a LocalId, and validate an ISO 19848
 *          value through the C API
 */

#include <dnv/VistaSDK_c_api_short.h>

#include <stdio.h>

static int onTabularDataPoint(
    date_time_offset_t timeStamp,
    const dcl_data_channel_t* dataChannel,
    const iso19848_value_t* value,
    const char* quality,
    const char** outErrorMessage,
    void* userdata)
{
    (void)timeStamp;
    (void)outErrorMessage;
    (void)userdata;

    char* valueStr = iso19848_value_to_string(value);
    const dcl_property_t* property = dcl_data_channel_property(dataChannel);
    const dcl_unit_t* unit = dcl_property_unit(property);

    printf("  Validating tabular data point:\n");
    printf("    Value  : %s\n", valueStr);
    if (unit != NULL)
    {
        printf("    Unit   : %s\n", dcl_unit_unit_symbol(unit));
    }
    if (quality != NULL)
    {
        printf("    Quality: %s\n", quality);
    }

    iso19848_value_string_free(valueStr);
    return 1;
}

static int onEventDataPoint(
    date_time_offset_t timeStamp,
    const dcl_data_channel_t* dataChannel,
    const iso19848_value_t* value,
    const char* quality,
    const char** outErrorMessage,
    void* userdata)
{
    (void)dataChannel;
    (void)outErrorMessage;
    (void)userdata;

    char* valueStr = iso19848_value_to_string(value);
    char* tsStr = date_time_offset_to_string(timeStamp, DATE_TIME_FORMAT_ISO8601);

    printf("  Validating event data point:\n");
    printf("    Value    : %s\n", valueStr);
    printf("    Timestamp: %s\n", tsStr);
    if (quality != NULL)
    {
        printf("    Quality  : %s\n", quality);
    }

    date_time_offset_string_free(tsStr);
    iso19848_value_string_free(valueStr);
    return 1;
}

int main(void)
{
    printf("=== vista-sdk C API Sample ===\n\n");

    const vis_t* vis = vis_instance();
    const char* latest = vis_latest(vis);

    {
        printf("1. VIS: Version management\n");
        printf("-------------------------------------------------------\n");

        size_t versionCount = vis_version_count(vis);

        printf("Latest VIS version: %s\n", latest);
        printf("Available versions (%zu):\n", versionCount);
        for (size_t i = 0; i < versionCount; ++i)
        {
            printf("  - %s\n", vis_version_at(vis, i));
        }
        printf("\n");
    }

    const codebooks_t* codebooks = vis_codebooks(vis, latest);

    {
        printf("2. Codebooks: Accessing a codebook and its standard values\n");
        printf("-------------------------------------------------------\n");

        const codebook_t* quantity = codebooks_at(codebooks, CODEBOOK_NAME_QUANTITY);

        printf("Codebook: %s\n", codebook_names_to_string(CODEBOOK_NAME_QUANTITY));
        printf("Has 'temperature'? %s\n", codebook_has_standard_value(quantity, "temperature") ? "true" : "false");
        printf("Has 'invalid_qty'? %s\n", codebook_has_standard_value(quantity, "invalid_qty") ? "true" : "false");
        printf("\n");
    }

    {
        printf("3. MetadataTag: Creating a tag from a standard value\n");
        printf("-------------------------------------------------------\n");

        const codebook_t* quantity = codebooks_at(codebooks, CODEBOOK_NAME_QUANTITY);
        metadata_tag_t* tempTag = codebook_create_tag(quantity, "temperature");

        if (tempTag != NULL)
        {
            char* tagStr = metadata_tag_to_string(tempTag);

            printf("Temperature tag created:\n");
            printf("  Value:       %s\n", metadata_tag_value(tempTag));
            printf("  Is Custom:   %s\n", metadata_tag_is_custom(tempTag) ? "true" : "false");
            printf("  String repr: %s\n", tagStr);

            metadata_tag_string_free(tagStr);
            metadata_tag_free(tempTag);
        }
        printf("\n");
    }

    const gmod_t* gmod = vis_gmod(vis, latest);

    {
        printf("4. Gmod: Looking up a node and its hierarchy\n");
        printf("-------------------------------------------------------\n");

        const gmod_node_t* node = gmod_get_node(gmod, "411.1");

        if (node != NULL)
        {
            char* nodeStr = gmod_node_to_string(node);

            printf("Node:       %s\n", nodeStr);
            printf("Is leaf?    %s\n", gmod_node_is_leaf_node(node) ? "true" : "false");
            printf("Is mappable? %s\n", gmod_node_is_mappable(node) ? "true" : "false");
            printf("Parents:    %zu\n", gmod_node_parent_count(node));
            printf("Children:   %zu\n", gmod_node_child_count(node));

            gmod_node_string_free(nodeStr);
        }
        printf("\n");
    }

    {
        printf("5. GmodPath: Parsing a short path\n");
        printf("-------------------------------------------------------\n");

        const locations_t* locations = vis_locations(vis, latest);
        gmod_path_t* path = gmod_path_from_short_path("411.1", gmod, locations);

        if (path != NULL)
        {
            char* pathStr = gmod_path_to_string(path);
            char* fullPathStr = gmod_path_to_full_path_string(path);

            printf("Short path: %s\n", pathStr);
            printf("Full path:  %s\n", fullPathStr);
            printf("Length:     %zu node(s)\n", gmod_path_length(path));
            printf("Mappable?   %s\n", gmod_path_is_mappable(path) ? "true" : "false");

            gmod_path_string_free(pathStr);
            gmod_path_string_free(fullPathStr);
            gmod_path_free(path);
        }
        else
        {
            printf("Parse failed: %s\n", last_error_message());
        }
        printf("\n");
    }

    {
        printf("6. LocalId: Parsing and inspecting a LocalId string\n");
        printf("-------------------------------------------------------\n");

        const char* localIdStr = "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature";
        local_id_t* localId = local_id_from_string(localIdStr);

        if (localId != NULL)
        {
            const gmod_path_t* primaryItem = local_id_primary_item(localId);
            char* primaryItemStr = gmod_path_to_string(primaryItem);
            const metadata_tag_t* quantityTag = local_id_metadata_tag(localId, CODEBOOK_NAME_QUANTITY);

            printf("Parsing:      %s\n", localIdStr);
            printf("VIS Version:  %s\n", local_id_version(localId));
            printf("Primary item: %s\n", primaryItemStr);
            if (quantityTag != NULL)
            {
                printf("Quantity:     %s\n", metadata_tag_value(quantityTag));
            }

            gmod_path_string_free(primaryItemStr);
            local_id_free(localId);
        }
        else
        {
            printf("Parse failed: %s\n", last_error_message());
        }

        const char* invalidLocalIdStr = "/dnv-v2/vis-3-4a/not-a-valid-path/meta/qty-temperature";
        parsing_errors_t* parseErrors = NULL;
        local_id_t* invalidLocalId = local_id_from_string_with_errors(invalidLocalIdStr, &parseErrors);

        if (invalidLocalId != NULL)
        {
            local_id_free(invalidLocalId);
        }
        else
        {
            printf("Parsing:      %s\n", invalidLocalIdStr);
            printf("Parse errors (%zu):\n", parsing_errors_count(parseErrors));
            for (size_t i = 0; i < parsing_errors_count(parseErrors); ++i)
            {
                printf(
                    "  [%s] %s\n", parsing_errors_type_at(parseErrors, i), parsing_errors_message_at(parseErrors, i));
            }
        }

        parsing_errors_free(parseErrors);
        printf("\n");
    }

    {
        printf("7. ISO19848: Validating a value against a FormatDataType\n");
        printf("-------------------------------------------------------\n");

        const iso19848_t* iso19848 = iso19848_instance();
        iso19848_version_t isoVersion = iso19848_latest(iso19848);
        iso19848_format_data_types_t* formatDataTypes = iso19848_format_data_types(iso19848, isoVersion);
        iso19848_format_data_type_t* decimalType = iso19848_format_data_types_from_string(formatDataTypes, "Decimal");

        if (decimalType != NULL)
        {
            iso19848_value_t* value = NULL;
            const char* testValue = "23.5";

            if (iso19848_format_data_type_validate(decimalType, testValue, &value))
            {
                char* valueStr = iso19848_value_to_string(value);

                printf("'%s' as Decimal: valid -> %s\n", testValue, valueStr);

                iso19848_value_string_free(valueStr);
                iso19848_value_free(value);
            }
            else
            {
                printf("'%s' as Decimal: invalid -> %s\n", testValue, last_error_message());
            }

            iso19848_format_data_type_free(decimalType);
        }

        iso19848_format_data_types_free(formatDataTypes);
        printf("\n");
    }

    {
        printf("8. VIS versioning: Converting a GmodPath between versions\n");
        printf("-------------------------------------------------------\n");

        const char* sourceVersion = "3-4a";
        const gmod_t* sourceGmod = vis_gmod(vis, sourceVersion);
        const locations_t* sourceLocations = vis_locations(vis, sourceVersion);
        gmod_path_t* sourcePath = gmod_path_from_short_path("511.331/C221", sourceGmod, sourceLocations);

        if (sourcePath != NULL)
        {
            char* sourcePathStr = gmod_path_to_string(sourcePath);
            printf("Source (%s): %s (depth: %zu)\n", sourceVersion, sourcePathStr, gmod_path_length(sourcePath));
            gmod_path_string_free(sourcePathStr);

            gmod_path_t* targetPath = vis_convert_path(vis, sourceVersion, sourcePath, latest);

            if (targetPath != NULL)
            {
                char* targetPathStr = gmod_path_to_string(targetPath);
                printf("Target (%s): %s (depth: %zu)\n", latest, targetPathStr, gmod_path_length(targetPath));

                gmod_path_string_free(targetPathStr);
                gmod_path_free(targetPath);
            }
            else
            {
                printf("Conversion failed: %s\n", last_error_message());
            }

            gmod_path_free(sourcePath);
        }
        printf("\n");
    }

    {
        printf("9. VIS versioning: Converting a LocalId between versions\n");
        printf("-------------------------------------------------------\n");

        const char* sourceLocalIdStr = "/dnv-v2/vis-3-4a/411.1/C101/sec/411.1/C101.64i/S201/meta/cnt-condensate";
        local_id_t* sourceLocalId = local_id_from_string(sourceLocalIdStr);

        if (sourceLocalId != NULL)
        {
            printf("Source: %s\n", sourceLocalIdStr);

            local_id_t* targetLocalId = vis_convert_local_id(vis, sourceLocalId, "3-5a");

            if (targetLocalId != NULL)
            {
                char* targetLocalIdStr = local_id_to_string(targetLocalId);
                printf("Target: %s\n", targetLocalIdStr);

                local_id_string_free(targetLocalIdStr);
                local_id_free(targetLocalId);
            }
            else
            {
                printf("Conversion failed: %s\n", last_error_message());
            }

            local_id_free(sourceLocalId);
        }
        printf("\n");
    }

    {
        printf("10. GmodPathQuery: Matching a base path while ignoring locations\n");
        printf("-------------------------------------------------------\n");

        const locations_t* locations = vis_locations(vis, latest);
        gmod_path_t* basePath = gmod_path_from_short_path("411.1/C101.31", gmod, locations);
        gmod_path_t* matchPath = gmod_path_from_short_path("411.1/C101.31-2", gmod, locations);
        gmod_path_t* mismatchPath = gmod_path_from_short_path("411.1/C101.63", gmod, locations);

        if (basePath != NULL && matchPath != NULL && mismatchPath != NULL)
        {
            gmod_path_query_builder_t* builder = gmod_path_query_builder_from(basePath);
            gmod_path_query_builder_t* withoutLocations = gmod_path_query_builder_without_locations(builder);
            gmod_path_query_t* query = gmod_path_query_builder_build(withoutLocations);

            printf("Query: match '411.1/C101.31' ignoring locations\n");
            printf("  411.1/C101.31-2 matches: %s\n", gmod_path_query_match(query, matchPath) ? "true" : "false");
            printf(
                "  411.1/C101.63   matches: %s (different node)\n",
                gmod_path_query_match(query, mismatchPath) ? "true" : "false");

            gmod_path_query_free(query);
            gmod_path_query_builder_free(withoutLocations);
            gmod_path_query_builder_free(builder);
        }

        if (mismatchPath != NULL)
            gmod_path_free(mismatchPath);
        if (matchPath != NULL)
            gmod_path_free(matchPath);
        if (basePath != NULL)
            gmod_path_free(basePath);
        printf("\n");
    }

    {
        printf("11. LocalIdQuery: Combining a primary-item path query with a tags query\n");
        printf("-------------------------------------------------------\n");

        local_id_t* localId1 = local_id_from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature");
        local_id_t* localId2 = local_id_from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-pressure");

        const gmod_node_t* node411 = gmod_get_node(gmod, "411.1");
        const gmod_node_t* nodeC101 = gmod_get_node(gmod, "C101");

        if (localId1 != NULL && localId2 != NULL && node411 != NULL && nodeC101 != NULL)
        {
            gmod_path_query_builder_t* pathBuilder = gmod_path_query_builder_create();
            gmod_path_query_builder_t* pathBuilder2 =
                gmod_path_query_builder_with_node_all_locations(pathBuilder, node411, 1);
            gmod_path_query_builder_t* pathBuilder3 =
                gmod_path_query_builder_with_node_all_locations(pathBuilder2, nodeC101, 1);
            gmod_path_query_t* pathQuery = gmod_path_query_builder_build(pathBuilder3);

            metadata_tags_query_builder_t* tagsBuilder = metadata_tags_query_builder_create();
            metadata_tags_query_builder_t* tagsBuilder2 =
                metadata_tags_query_builder_with_tag(tagsBuilder, CODEBOOK_NAME_QUANTITY, "temperature");
            metadata_tags_query_t* tagsQuery = metadata_tags_query_builder_build(tagsBuilder2);

            local_id_query_builder_t* localIdBuilder = local_id_query_builder_create();
            local_id_query_builder_t* localIdBuilder2 =
                local_id_query_builder_with_primary_item_query(localIdBuilder, pathQuery);
            local_id_query_builder_t* localIdBuilder3 = local_id_query_builder_with_tags(localIdBuilder2, tagsQuery);
            local_id_query_t* query = local_id_query_builder_build(localIdBuilder3);

            printf("Query: primary='411.1/C101' (any location) AND qty-temperature\n");
            printf("  qty-temperature matches: %s\n", local_id_query_match(query, localId1) ? "true" : "false");
            printf(
                "  qty-pressure    matches: %s (wrong tag)\n",
                local_id_query_match(query, localId2) ? "true" : "false");

            local_id_query_free(query);
            local_id_query_builder_free(localIdBuilder3);
            local_id_query_builder_free(localIdBuilder2);
            local_id_query_builder_free(localIdBuilder);
            metadata_tags_query_free(tagsQuery);
            metadata_tags_query_builder_free(tagsBuilder2);
            metadata_tags_query_builder_free(tagsBuilder);
            gmod_path_query_free(pathQuery);
            gmod_path_query_builder_free(pathBuilder3);
            gmod_path_query_builder_free(pathBuilder2);
            gmod_path_query_builder_free(pathBuilder);
        }

        if (localId2 != NULL)
            local_id_free(localId2);
        if (localId1 != NULL)
            local_id_free(localId1);
        printf("\n");
    }

    dcl_list_package_t* dclListPackage = NULL;

    {
        printf("12. DataChannelList: Building a package and serializing to JSON\n");
        printf("-------------------------------------------------------\n");

        const locations_t* locations = vis_locations(vis, latest);

        ship_id_t* shipId = ship_id_from_string("IMO8027781");

        date_time_offset_t configTimestamp;
        date_time_offset_from_string("2026-09-01T10:00:00Z", &configTimestamp);
        dcl_configuration_reference_t* configRef =
            dcl_configuration_reference_create("vessel-config-2026-v1", configTimestamp);

        dcl_header_t* header = dcl_header_create(shipId, configRef);

        // Channels 1 & 2: GPS latitude and longitude (tabular, continuous measures)
        gmod_path_t* latItem = gmod_path_from_short_path("710.1/F211.11", gmod, locations);
        gmod_path_t* lonItem = gmod_path_from_short_path("710.1/F211.12", gmod, locations);
        metadata_tag_t* latQtyTag = codebook_create_tag(codebooks_at(codebooks, CODEBOOK_NAME_QUANTITY), "latitude");
        metadata_tag_t* lonQtyTag = codebook_create_tag(codebooks_at(codebooks, CODEBOOK_NAME_QUANTITY), "longitude");

        local_id_builder_t* latBuilder = local_id_builder_create(latest);
        local_id_builder_t* latBuilder2 = local_id_builder_with_primary_item(latBuilder, latItem);
        local_id_builder_t* latBuilder3 = local_id_builder_with_metadata_tag(latBuilder2, latQtyTag);
        local_id_t* latLocalId = local_id_builder_build(latBuilder3);

        local_id_builder_t* lonBuilder = local_id_builder_create(latest);
        local_id_builder_t* lonBuilder2 = local_id_builder_with_primary_item(lonBuilder, lonItem);
        local_id_builder_t* lonBuilder3 = local_id_builder_with_metadata_tag(lonBuilder2, lonQtyTag);
        local_id_t* lonLocalId = local_id_builder_build(lonBuilder3);

        dcl_data_channel_type_t* instType = dcl_data_channel_type_create("Inst");
        dcl_format_t* decimalFormat = dcl_format_create("Decimal");
        dcl_unit_t* degUnit = dcl_unit_create("deg");

        dcl_channel_id_t* latChannelId = dcl_channel_id_create(latLocalId);
        dcl_channel_id_set_short_id(latChannelId, "GPSLatitude");
        dcl_range_t* latRange = dcl_range_create(-90.0, 90.0);
        dcl_property_t* latProperty = dcl_property_create(instType, decimalFormat);
        dcl_property_set_range(latProperty, latRange);
        dcl_property_set_unit(latProperty, degUnit);
        dcl_data_channel_t* latChannel = dcl_data_channel_create(latChannelId, latProperty);

        dcl_channel_id_t* lonChannelId = dcl_channel_id_create(lonLocalId);
        dcl_channel_id_set_short_id(lonChannelId, "GPSLongitude");
        dcl_range_t* lonRange = dcl_range_create(-180.0, 180.0);
        dcl_property_t* lonProperty = dcl_property_create(instType, decimalFormat);
        dcl_property_set_range(lonProperty, lonRange);
        dcl_property_set_unit(lonProperty, degUnit);
        dcl_data_channel_t* lonChannel = dcl_data_channel_create(lonChannelId, lonProperty);

        // Channel 3: turbocharger overspeed alarm (event, discrete state)
        gmod_path_t* turboItem = gmod_path_from_short_path("411.1/C101", gmod, locations);
        metadata_tag_t* stateTag = codebook_create_tag(codebooks_at(codebooks, CODEBOOK_NAME_STATE), "overspeed");

        local_id_builder_t* alarmBuilder = local_id_builder_create(latest);
        local_id_builder_t* alarmBuilder2 = local_id_builder_with_primary_item(alarmBuilder, turboItem);
        local_id_builder_t* alarmBuilder3 = local_id_builder_with_metadata_tag(alarmBuilder2, stateTag);
        local_id_t* alarmLocalId = local_id_builder_build(alarmBuilder3);

        dcl_channel_id_t* alarmChannelId = dcl_channel_id_create(alarmLocalId);
        dcl_channel_id_set_short_id(alarmChannelId, "TurboOverspeedAlarm");

        dcl_data_channel_type_t* eventType = dcl_data_channel_type_create("Inst");
        dcl_format_t* stringFormat = dcl_format_create("String");

        dcl_property_t* alarmProperty = dcl_property_create(eventType, stringFormat);

        dcl_data_channel_t* alarmChannel = dcl_data_channel_create(alarmChannelId, alarmProperty);

        dcl_data_channel_list_t* dataChannelList = dcl_data_channel_list_create();
        dcl_data_channel_list_add(dataChannelList, latChannel);
        dcl_data_channel_list_add(dataChannelList, lonChannel);
        dcl_data_channel_list_add(dataChannelList, alarmChannel);

        dcl_package_t* package = dcl_package_create(header, dataChannelList);
        dclListPackage = dcl_list_package_create(package);

        printf("Created DataChannelList package:\n");
        printf("  Ship ID      : %s\n", ship_id_to_string(shipId));
        printf("  Channel count: %zu\n", dcl_data_channel_list_size(dataChannelList));

        char* json = dcl_list_package_to_json(dclListPackage, 1);
        printf("\nJSON output:\n%s\n", json);
        dcl_json_string_free(json);

        dcl_package_free(package);
        dcl_data_channel_list_free(dataChannelList);
        dcl_data_channel_free(alarmChannel);
        dcl_property_free(alarmProperty);
        dcl_format_free(stringFormat);
        dcl_data_channel_type_free(eventType);
        dcl_channel_id_free(alarmChannelId);
        local_id_free(alarmLocalId);
        local_id_builder_free(alarmBuilder3);
        local_id_builder_free(alarmBuilder2);
        local_id_builder_free(alarmBuilder);
        metadata_tag_free(stateTag);
        gmod_path_free(turboItem);
        dcl_data_channel_free(lonChannel);
        dcl_property_free(lonProperty);
        dcl_range_free(lonRange);
        dcl_channel_id_free(lonChannelId);
        dcl_data_channel_free(latChannel);
        dcl_property_free(latProperty);
        dcl_range_free(latRange);
        dcl_channel_id_free(latChannelId);
        dcl_unit_free(degUnit);
        dcl_format_free(decimalFormat);
        dcl_data_channel_type_free(instType);
        local_id_free(lonLocalId);
        local_id_builder_free(lonBuilder3);
        local_id_builder_free(lonBuilder2);
        local_id_builder_free(lonBuilder);
        metadata_tag_free(lonQtyTag);
        gmod_path_free(lonItem);
        local_id_free(latLocalId);
        local_id_builder_free(latBuilder3);
        local_id_builder_free(latBuilder2);
        local_id_builder_free(latBuilder);
        metadata_tag_free(latQtyTag);
        gmod_path_free(latItem);
        dcl_header_free(header);
        dcl_configuration_reference_free(configRef);
        ship_id_free(shipId);
        printf("\n");
    }

    {
        printf("13. TimeSeriesData: Building a package and cross-validating against the DataChannelList\n");
        printf("-------------------------------------------------------\n");

        ship_id_t* shipId = ship_id_from_string("IMO8027781");
        tsd_header_t* header = tsd_header_create(shipId);

        date_time_offset_t configTimestamp;
        date_time_offset_from_string("2026-09-01T00:00:00Z", &configTimestamp);
        tsd_config_ref_t* dataConfig = tsd_config_ref_create("vessel-config-2026-v1", configTimestamp);

        // Tabular: GPS latitude and longitude sample
        tsd_channel_id_t* latChannelId = tsd_channel_id_from_string("GPSLatitude");
        tsd_channel_id_t* lonChannelId = tsd_channel_id_from_string("GPSLongitude");

        date_time_offset_t sampleTimestamp;
        date_time_offset_from_string("2026-09-01T10:00:00Z", &sampleTimestamp);
        const char* values[] = { "45.123456", "5.987654" };
        tsd_tabular_data_set_t* dataSet = tsd_tabular_data_set_create(sampleTimestamp, values, 2);

        const tsd_channel_id_t* channelIds[] = { latChannelId, lonChannelId };
        const tsd_tabular_data_set_t* dataSets[] = { dataSet };
        tsd_tabular_data_t* tabularData = tsd_tabular_data_create(channelIds, 2, dataSets, 1);

        // Event: turbocharger overspeed alarm trigger
        tsd_channel_id_t* alarmChannelId = tsd_channel_id_from_string("TurboOverspeedAlarm");

        date_time_offset_t eventTimestamp;
        date_time_offset_from_string("2026-09-01T10:00:42Z", &eventTimestamp);
        tsd_event_data_set_t* eventDataSet = tsd_event_data_set_create(eventTimestamp, alarmChannelId, "alarm.active");

        tsd_event_data_t* eventData = tsd_event_data_create();
        const tsd_event_data_set_t* eventDataSets[] = { eventDataSet };
        tsd_event_data_set_data_set(eventData, eventDataSets, 1);

        tsd_time_series_data_t* timeSeriesData = tsd_time_series_data_create();
        tsd_time_series_data_set_data_configuration(timeSeriesData, dataConfig);
        const tsd_tabular_data_t* tabularDataEntries[] = { tabularData };
        tsd_time_series_data_set_tabular_data(timeSeriesData, tabularDataEntries, 1);
        tsd_time_series_data_set_event_data(timeSeriesData, eventData);

        const tsd_time_series_data_t* timeSeriesDataEntries[] = { timeSeriesData };
        tsd_package_t* package = tsd_package_create(header, timeSeriesDataEntries, 1);
        tsd_data_package_t* dataPackage = tsd_data_package_create(package);

        printf("Created TimeSeriesData package:\n");
        printf("  Ship ID       : %s\n", ship_id_to_string(shipId));
        printf("  Tabular tables: %zu\n", tsd_time_series_data_tabular_data_count(timeSeriesData));
        printf(
            "  Event data    : %s\n", tsd_time_series_data_event_data(timeSeriesData) != NULL ? "present" : "absent");

        if (dclListPackage != NULL)
        {
            printf("\nCross-validating against the DataChannelList built in section 12:\n");
            int valid = tsd_time_series_data_validate(
                timeSeriesData, dclListPackage, onTabularDataPoint, NULL, onEventDataPoint, NULL);
            printf("Cross-validation result: %s\n", valid ? "valid" : "invalid");
            if (!valid)
            {
                printf("  Error: %s\n", last_error_message());
            }
        }

        char* json = tsd_data_package_to_json(dataPackage, 1);
        printf("\nJSON output:\n%s\n", json);
        tsd_json_string_free(json);

        tsd_data_package_free(dataPackage);
        tsd_package_free(package);
        tsd_time_series_data_free(timeSeriesData);
        tsd_tabular_data_free(tabularData);
        tsd_tabular_data_set_free(dataSet);
        tsd_event_data_free(eventData);
        tsd_event_data_set_free(eventDataSet);
        tsd_channel_id_free(alarmChannelId);
        tsd_channel_id_free(lonChannelId);
        tsd_channel_id_free(latChannelId);
        tsd_config_ref_free(dataConfig);
        tsd_header_free(header);
        ship_id_free(shipId);
        printf("\n");
    }

    if (dclListPackage != NULL)
    {
        dcl_list_package_free(dclListPackage);
    }

    return 0;
}
