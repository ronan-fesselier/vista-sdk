#pragma once

namespace dnv::vista::sdk
{
	/**
	 * @brief Data Transfer Object (DTO) for a single data channel type name
	 *
	 * Represents a type name and its description as defined in ISO 19848.
	 * Maps to C# record: DataChannelTypeNameDto(string Type, string Description)
	 */
	struct DataChannelTypeNameDto
	{
		/**
		 * @brief Default constructor
		 */
		DataChannelTypeNameDto() = default;

		/**
		 * @brief Constructor with parameters
		 * @param type The type name
		 * @param description The description of the type
		 */
		DataChannelTypeNameDto(
			std::string type,
			std::string description );

		/**
		 * @brief Deserialize from JSON
		 * @param json JSON value to deserialize from
		 * @return Deserialized DTO
		 */
		static DataChannelTypeNameDto FromJson( const rapidjson::Value& json );

		/**
		 * @brief Serialize to JSON
		 * @param allocator JSON allocator to use
		 * @return JSON value representation
		 */
		rapidjson::Value ToJson( rapidjson::Document::AllocatorType& allocator ) const;

		/**
		 * @brief Type name (JSON: "type")
		 */
		std::string type;

		/**
		 * @brief Description of the type (JSON: "description")
		 */
		std::string description;
	};

	/**
	 * @brief Data Transfer Object (DTO) for a collection of data channel type names
	 *
	 * Represents a collection of data channel type names and their descriptions.
	 * Maps to C# record: DataChannelTypeNamesDto(DataChannelTypeNameDto[] Values)
	 */
	struct DataChannelTypeNamesDto
	{
		/**
		 * @brief Default constructor
		 */
		DataChannelTypeNamesDto() = default;

		/**
		 * @brief Constructor with parameters
		 * @param values A collection of data channel type name values
		 */
		explicit DataChannelTypeNamesDto(
			std::vector<DataChannelTypeNameDto> values );

		/**
		 * @brief Deserialize from JSON
		 * @param json JSON value to deserialize from
		 * @return Deserialized DTO
		 */
		static DataChannelTypeNamesDto FromJson( const rapidjson::Value& json );

		/**
		 * @brief Serialize to JSON
		 * @param allocator JSON allocator to use
		 * @return JSON value representation
		 */
		rapidjson::Value ToJson( rapidjson::Document::AllocatorType& allocator ) const;

		/**
		 * @brief Collection of data channel type name values (JSON: "values")
		 */
		std::vector<DataChannelTypeNameDto> values;
	};

	/**
	 * @brief Data Transfer Object (DTO) for a single format data type
	 *
	 * Represents a format data type and its description as defined in ISO 19848.
	 * Maps to C# record: FormatDataTypeDto(string Type, string Description)
	 */
	struct FormatDataTypeDto
	{
		/**
		 * @brief Default constructor
		 */
		FormatDataTypeDto() = default;

		/**
		 * @brief Constructor with parameters
		 * @param type The type name
		 * @param description The description of the type
		 */
		FormatDataTypeDto(
			std::string type,
			std::string description );

		/**
		 * @brief Deserialize from JSON
		 * @param json JSON value to deserialize from
		 * @return Deserialized DTO
		 */
		static FormatDataTypeDto FromJson( const rapidjson::Value& json );

		/**
		 * @brief Serialize to JSON
		 * @param allocator JSON allocator to use
		 * @return JSON value representation
		 */
		rapidjson::Value ToJson( rapidjson::Document::AllocatorType& allocator ) const;
		/**
		 * @brief Type name (JSON: "type")
		 */
		std::string type;

		/**
		 * @brief Description of the type (JSON: "description")
		 */
		std::string description;
	};

	/**
	 * @brief Data Transfer Object (DTO) for a collection of format data types
	 *
	 * Represents a collection of format data types and their descriptions.
	 * Maps to C# record: FormatDataTypesDto(FormatDataTypeDto[] Values)
	 */
	struct FormatDataTypesDto
	{
		/**
		 * @brief Default constructor
		 */
		FormatDataTypesDto() = default;

		/**
		 * @brief Constructor with parameters
		 * @param values A collection of format data type values
		 */
		explicit FormatDataTypesDto(
			std::vector<FormatDataTypeDto> values );

		/**
		 * @brief Deserialize from JSON
		 * @param json JSON value to deserialize from
		 * @return Deserialized DTO
		 */
		static FormatDataTypesDto FromJson( const rapidjson::Value& json );

		/**
		 * @brief Serialize to JSON
		 * @param allocator JSON allocator to use
		 * @return JSON value representation
		 */
		rapidjson::Value ToJson( rapidjson::Document::AllocatorType& allocator ) const;

		/**
		 * @brief Collection of format data type values (JSON: "values")
		 */
		std::vector<FormatDataTypeDto> values;
	};
}
