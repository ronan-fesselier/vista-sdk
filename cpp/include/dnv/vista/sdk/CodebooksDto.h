#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <rapidjson/document.h>

namespace dnv::vista::sdk
{
	/**
	 * @brief Data transfer object for a single codebook
	 *
	 * Represents serialized information about a codebook from the ISO 19848 standard.
	 * Each codebook contains a name identifier and a collection of values organized by groups.
	 */
	struct CodebookDto
	{
		/** @brief Name identifier of the codebook (e.g., "positions", "quantities") */
		std::string name;

		/** @brief Map of group names to their corresponding values */
		std::unordered_map<std::string, std::vector<std::string>> values;

		/**
		 * @brief Deserialize a CodebookDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @return The deserialized CodebookDto
		 */
		static CodebookDto FromJson( const rapidjson::Value& json )
		{
			CodebookDto dto;

			dto.name = json["name"].GetString();

			if ( json.HasMember( "values" ) && json["values"].IsObject() )
			{
				for ( auto it = json["values"].MemberBegin(); it != json["values"].MemberEnd(); ++it )
				{
					std::vector<std::string> groupValues;
					for ( const auto& value : it->value.GetArray() )
					{
						groupValues.push_back( value.GetString() );
					}
					dto.values[it->name.GetString()] = std::move( groupValues );
				}
			}

			return dto;
		}
	};

	/**
	 * @brief Data transfer object for a collection of codebooks
	 *
	 * Represents a complete set of codebooks for a specific VIS version,
	 * used for serialization to and from JSON format.
	 */
	struct CodebooksDto
	{
		/** @brief VIS version string (e.g., "3.8a") */
		std::string visVersion;

		/** @brief Collection of codebook DTOs contained in this version */
		std::vector<CodebookDto> items;

		/**
		 * @brief Deserialize a CodebooksDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @return The deserialized CodebooksDto
		 */
		static CodebooksDto FromJson( const rapidjson::Value& json )
		{
			CodebooksDto dto;

			dto.visVersion = json["visRelease"].GetString();

			if ( json.HasMember( "items" ) && json["items"].IsArray() )
			{
				for ( const auto& item : json["items"].GetArray() )
				{
					dto.items.push_back( CodebookDto::FromJson( item ) );
				}
			}

			return dto;
		}
	};
}
