#pragma once

#include <string>
#include <vector>
#include <rapidjson/document.h>

namespace dnv::vista::sdk
{
	/**
	 * @brief Data Transfer Object (DTO) for a single data channel type name
	 *
	 * Represents a type name and its description as defined in ISO 19848.
	 */
	struct DataChannelTypeNameDto
	{
		/**
		 * @brief Constructor
		 * @param type The type name
		 * @param description The description of the type
		 */
		DataChannelTypeNameDto(
			const std::string& type,
			const std::string& description );

		/**
		 * @brief Default constructor
		 */
		DataChannelTypeNameDto() = default;

		/**
		 * @brief Type name
		 */
		std::string type;

		/**
		 * @brief Description of the type
		 */
		std::string description;

		/**
		 * @brief Deserialize from JSON
		 * @param json JSON value to deserialize from
		 * @return Deserialized DTO
		 */
		static DataChannelTypeNameDto FromJson( const rapidjson::Value& json )
		{
			DataChannelTypeNameDto dto;

			if ( json.HasMember( "type" ) && json["type"].IsString() )
				dto.type = json["type"].GetString();

			if ( json.HasMember( "description" ) && json["description"].IsString() )
				dto.description = json["description"].GetString();

			return dto;
		}
	};

	/**
	 * @brief Data Transfer Object (DTO) for a collection of data channel type names
	 *
	 * Represents a collection of data channel type names and their descriptions.
	 */
	struct DataChannelTypeNamesDto
	{
		/**
		 * @brief Default constructor
		 */
		DataChannelTypeNamesDto() = default;

		/**
		 * @brief Collection of data channel type name values
		 */
		std::vector<DataChannelTypeNameDto> values;

		/**
		 * @brief Constructor
		 * @param values A collection of data channel type name values
		 */
		explicit DataChannelTypeNamesDto(
			const std::vector<DataChannelTypeNameDto>& values );

		/**
		 * @brief Deserialize from JSON
		 * @param json JSON value to deserialize from
		 * @return Deserialized DTO
		 */
		static DataChannelTypeNamesDto FromJson( const rapidjson::Value& json )
		{
			DataChannelTypeNamesDto dto;

			if ( json.HasMember( "values" ) && json["values"].IsArray() )
			{
				for ( const auto& item : json["values"].GetArray() )
				{
					try
					{
						dto.values.push_back( DataChannelTypeNameDto::FromJson( item ) );
					}
					catch ( const std::exception& e )
					{
						SPDLOG_ERROR( "Warning: Skipping malformed data channel type name: {}", e.what() );
					}
				}
			}

			return dto;
		}
	};

	/**
	 * @brief Data Transfer Object (DTO) for a single format data type
	 *
	 * Represents a format data type and its description as defined in ISO 19848.
	 */
	struct FormatDataTypeDto
	{
		/**
		 * @brief Constructor
		 * @param type The type name
		 * @param description The description of the type
		 */
		FormatDataTypeDto(
			const std::string& type,
			const std::string& description );

		/**
		 * @brief Default constructor
		 */
		FormatDataTypeDto() = default;

		/**
		 * @brief Type name
		 */
		std::string type;

		/**
		 * @brief Description of the type
		 */
		std::string description;

		/**
		 * @brief Deserialize from JSON
		 * @param json JSON value to deserialize from
		 * @return Deserialized DTO
		 */
		static FormatDataTypeDto FromJson( const rapidjson::Value& json )
		{
			FormatDataTypeDto dto;

			if ( json.HasMember( "type" ) && json["type"].IsString() )
				dto.type = json["type"].GetString();

			if ( json.HasMember( "description" ) && json["description"].IsString() )
				dto.description = json["description"].GetString();

			return dto;
		}
	};

	/**
	 * @brief Data Transfer Object (DTO) for a collection of format data types
	 *
	 * Represents a collection of format data types and their descriptions.
	 */
	struct FormatDataTypesDto
	{
		/**
		 * @brief Constructor
		 * @param values A collection of format data type values
		 */
		explicit FormatDataTypesDto(
			const std::vector<FormatDataTypeDto>& values );

		/**
		 * @brief Default constructor
		 */
		FormatDataTypesDto() = default;

		/**
		 * @brief Collection of format data type values
		 */
		std::vector<FormatDataTypeDto> values;

		/**
		 * @brief Deserialize from JSON
		 * @param json JSON value to deserialize from
		 * @return Deserialized DTO
		 */
		static FormatDataTypesDto FromJson( const rapidjson::Value& json )
		{
			FormatDataTypesDto dto;

			if ( json.HasMember( "values" ) && json["values"].IsArray() )
			{
				for ( const auto& item : json["values"].GetArray() )
				{
					try
					{
						dto.values.push_back( FormatDataTypeDto::FromJson( item ) );
					}
					catch ( const std::exception& e )
					{
						SPDLOG_ERROR( "Warning: Skipping malformed format data type: {}", e.what() );
					}
				}
			}

			return dto;
		}
	};
}
