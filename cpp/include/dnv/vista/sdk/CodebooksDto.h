#pragma once

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
		/**
		 * @brief Default constructor
		 */
		CodebookDto() = default;

		/**
		 * @brief Constructor with parameters
		 * @param name The codebook name
		 * @param values The map of group names to values
		 */
		CodebookDto( std::string name, std::unordered_map<std::string, std::vector<std::string>> values );

		/**
		 * @brief Deserialize a CodebookDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @return The deserialized CodebookDto
		 */
		static CodebookDto fromJson( const rapidjson::Value& json );

		/**
		 * @brief Try to deserialize a CodebookDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @param dto Output parameter to receive the deserialized object
		 * @return True if deserialization was successful, false otherwise
		 */
		static bool tryFromJson( const rapidjson::Value& json, CodebookDto& dto );

		/**
		 * @brief Serialize this CodebookDto to a RapidJSON Value
		 * @param allocator The JSON value allocator to use
		 * @return The serialized JSON value
		 */
		rapidjson::Value toJson( rapidjson::Document::AllocatorType& allocator ) const;

		/** @brief Name identifier of the codebook (e.g., "positions", "quantities") */
		std::string name;

		/** @brief Map of group names to their corresponding values */
		std::unordered_map<std::string, std::vector<std::string>> values;
	};

	/**
	 * @brief Data transfer object for a collection of codebooks
	 *
	 * Represents a complete set of codebooks for a specific VIS version,
	 * used for serialization to and from JSON format.
	 */
	struct CodebooksDto
	{
		/**
		 * @brief Default constructor
		 */
		CodebooksDto() = default;

		/**
		 * @brief Constructor with parameters
		 * @param visVersion The VIS version
		 * @param items The collection of codebook DTOs
		 */
		CodebooksDto( std::string visVersion, std::vector<CodebookDto> items );

		/**
		 * @brief Deserialize a CodebooksDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @return The deserialized CodebooksDto
		 */
		static CodebooksDto fromJson( const rapidjson::Value& json );

		/**
		 * @brief Try to deserialize a CodebooksDto from a RapidJSON object
		 * @param json The RapidJSON object to deserialize
		 * @param dto Output parameter to receive the deserialized object
		 * @return True if deserialization was successful, false otherwise
		 */
		static bool tryFromJson( const rapidjson::Value& json, CodebooksDto& dto );

		/**
		 * @brief Serialize this CodebooksDto to a RapidJSON Value
		 * @param allocator The JSON value allocator to use
		 * @return The serialized JSON value
		 */
		rapidjson::Value toJson( rapidjson::Document::AllocatorType& allocator ) const;

		/** @brief VIS version string (e.g., "3.8a") */
		std::string visVersion;

		/** @brief Collection of codebook DTOs contained in this version */
		std::vector<CodebookDto> items;
	};
}
