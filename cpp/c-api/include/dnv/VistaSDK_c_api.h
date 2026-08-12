/**
 * @file VistaSDK_c_api.h
 * @brief Umbrella header for the DNV Vista SDK C API
 * @details Single include for the complete Vista SDK C API: VIS core (Gmod, Codebooks,
 *          Locations, identifiers, queries), value types (DateTime, Decimal), and ISO 19848
 *          transport layer (DataChannelList, TimeSeriesData, primitives, JSON serialization)
 */

#pragma once

#include <dnv/vista/sdk/c/version.h>

#include "vista/sdk/c/core/codebook_name.h"
#include "vista/sdk/c/core/codebook.h"
#include "vista/sdk/c/core/codebooks.h"
#include "vista/sdk/c/core/gmod_individualizable_set.h"
#include "vista/sdk/c/core/gmod_node_metadata.h"
#include "vista/sdk/c/core/gmod_node.h"
#include "vista/sdk/c/core/gmod_path.h"
#include "vista/sdk/c/core/gmod.h"
#include "vista/sdk/c/core/imo_number.h"
#include "vista/sdk/c/core/local_id_builder.h"
#include "vista/sdk/c/core/local_id_mqtt.h"
#include "vista/sdk/c/core/local_id.h"
#include "vista/sdk/c/core/location_builder.h"
#include "vista/sdk/c/core/location_group.h"
#include "vista/sdk/c/core/location.h"
#include "vista/sdk/c/core/locations.h"
#include "vista/sdk/c/core/metadata_tag.h"
#include "vista/sdk/c/core/parsing_errors.h"
#include "vista/sdk/c/core/relative_location.h"
#include "vista/sdk/c/core/universal_id_builder.h"
#include "vista/sdk/c/core/universal_id.h"
#include "vista/sdk/c/core/vis.h"

#include "vista/sdk/c/query/gmod_path_query_builder.h"
#include "vista/sdk/c/query/gmod_path_query.h"
#include "vista/sdk/c/query/local_id_query_builder.h"
#include "vista/sdk/c/query/local_id_query.h"
#include "vista/sdk/c/query/metadata_tags_query_builder.h"
#include "vista/sdk/c/query/metadata_tags_query.h"

#include "vista/sdk/c/transport/serialization/json/datachannel/data_channel_json.h"
#include "vista/sdk/c/transport/serialization/json/timeseries/time_series_data_json.h"
#include "vista/sdk/c/transport/serialization/json/serializable_document.h"
#include "vista/sdk/c/transport/datachannel/data_channel.h"
#include "vista/sdk/c/transport/timeseries/data_channel_id.h"
#include "vista/sdk/c/transport/timeseries/time_series_data.h"
#include "vista/sdk/c/transport/iso19848.h"
#include "vista/sdk/c/transport/ship_id.h"

#include "vista/sdk/c/types/datetime/date_time_offset.h"
#include "vista/sdk/c/types/datetime/date_time.h"
#include "vista/sdk/c/types/datetime/time_span.h"
#include "vista/sdk/c/types/decimal/decimal.h"

#include "vista/sdk/c/common.h"
#include "vista/sdk/c/error.h"
