#pragma once

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
	};
}
