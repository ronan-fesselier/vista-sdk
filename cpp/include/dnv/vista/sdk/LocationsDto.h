#pragma once

#include <string>
#include <vector>
#include <optional>
#include <rapidjson/document.h>

namespace dnv::vista::sdk
{
	/**
	 * @brief Data Transfer Object (DTO) for a relative location.
	 *
	 * Represents a relative location with a code, name, and optional definition.
	 */
	struct RelativeLocationsDto
	{
		char code;							   ///< The character code representing the location.
		std::string name;					   ///< The name of the location.
		std::optional<std::string> definition; ///< An optional definition of the location.

		/**
		 * @brief Deserialize a RelativeLocationsDto from a RapidJSON object.
		 * @param json The RapidJSON object to deserialize.
		 * @return The deserialized RelativeLocationsDto.
		 */
		static RelativeLocationsDto FromJson( const rapidjson::Value& json )
		{
			RelativeLocationsDto dto;

			dto.code = json["code"].GetString()[0]; // Assuming "code" is a single character.
			dto.name = json["name"].GetString();

			if ( json.HasMember( "definition" ) && json["definition"].IsString() )
			{
				dto.definition = json["definition"].GetString();
			}

			return dto;
		}
	};

	/**
	 * @brief Data Transfer Object (DTO) for a collection of locations.
	 *
	 * Represents a collection of relative locations and the VIS version they belong to.
	 */
	struct LocationsDto
	{
		std::string visVersion;					 ///< The VIS version string.
		std::vector<RelativeLocationsDto> items; ///< A vector of relative locations.

		/**
		 * @brief Deserialize a LocationsDto from a RapidJSON object.
		 * @param json The RapidJSON object to deserialize.
		 * @return The deserialized LocationsDto.
		 */
		static LocationsDto FromJson( const rapidjson::Value& json )
		{
			LocationsDto dto;

			if ( !json.HasMember( "visRelease" ) || !json["visRelease"].IsString() )
			{
				throw std::runtime_error( "Missing or invalid 'visRelease' field in Locations JSON." );
			}
			dto.visVersion = json["visRelease"].GetString();

			if ( json.HasMember( "items" ) && json["items"].IsArray() )
			{
				for ( const auto& item : json["items"].GetArray() )
				{
					dto.items.push_back( RelativeLocationsDto::FromJson( item ) );
				}
			}
			else
			{
				throw std::runtime_error( "Missing or invalid 'items' field in Locations JSON." );
			}

			return dto;
		}
	};
}
