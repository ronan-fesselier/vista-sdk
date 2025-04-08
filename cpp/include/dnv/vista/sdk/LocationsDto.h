#pragma once

namespace dnv::vista::sdk
{
	/**
	 * @brief Data Transfer Object (DTO) for a relative location.
	 *
	 * Represents a relative location with a code, name, and optional definition.
	 * Maps to C# record: RelativeLocationsDto(char Code, string Name, string? Definition)
	 */
	struct RelativeLocationsDto
	{
		/** @brief The character code representing the location (JSON: "code"). */
		char code;

		/** @brief The name of the location (JSON: "name"). */
		std::string name;

		/** @brief An optional definition of the location (JSON: "definition"). */
		std::optional<std::string> definition;

		/**
		 * @brief Default constructor
		 */
		RelativeLocationsDto() = default;

		/**
		 * @brief Constructor with parameters
		 *
		 * @param code The character code representing the location
		 * @param name The name of the location
		 * @param definition An optional definition of the location
		 */
		RelativeLocationsDto(
			char code,
			std::string name,
			std::optional<std::string> definition = std::nullopt )
			: code( code ),
			  name( std::move( name ) ),
			  definition( std::move( definition ) ) {}

		/**
		 * @brief Deserialize a RelativeLocationsDto from a RapidJSON object.
		 * @param json The RapidJSON object to deserialize.
		 * @return The deserialized RelativeLocationsDto.
		 */
		static RelativeLocationsDto FromJson( const rapidjson::Value& json );

		/**
		 * @brief Serialize this RelativeLocationsDto to a RapidJSON Value
		 * @param allocator The JSON value allocator to use
		 * @return The serialized JSON value
		 */
		rapidjson::Value ToJson( rapidjson::Document::AllocatorType& allocator ) const;
	};

	/**
	 * @brief Data Transfer Object (DTO) for a collection of locations.
	 *
	 * Represents a collection of relative locations and the VIS version they belong to.
	 * Maps to C# record: LocationsDto(string VisVersion, RelativeLocationsDto[] Items)
	 */
	struct LocationsDto
	{
		/** @brief The VIS version string (JSON: "visRelease"). */
		std::string visVersion;

		/** @brief A vector of relative locations (JSON: "items"). */
		std::vector<RelativeLocationsDto> items;

		/**
		 * @brief Default constructor
		 */
		LocationsDto() = default;

		/**
		 * @brief Constructor with parameters
		 *
		 * @param visVersion The VIS version string
		 * @param items A collection of relative locations
		 */
		LocationsDto(
			std::string visVersion,
			std::vector<RelativeLocationsDto> items )
			: visVersion( std::move( visVersion ) ),
			  items( std::move( items ) ) {}

		/**
		 * @brief Deserialize a LocationsDto from a RapidJSON object.
		 * @param json The RapidJSON object to deserialize.
		 * @return The deserialized LocationsDto.
		 */
		static LocationsDto FromJson( const rapidjson::Value& json );

		/**
		 * @brief Serialize this LocationsDto to a RapidJSON Value
		 * @param allocator The JSON value allocator to use
		 * @return The serialized JSON value
		 */
		rapidjson::Value ToJson( rapidjson::Document::AllocatorType& allocator ) const;
	};
}
