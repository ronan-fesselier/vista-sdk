#pragma once

#include "CodebooksDto.h"

namespace dnv::vista::sdk
{
	class MetadataTag;

	enum class CodebookName;

	/**
	 * @brief Enumeration for validation results of position strings
	 */
	enum class PositionValidationResult
	{
		Invalid = 0,	 ///< Position is invalid
		InvalidOrder,	 ///< Position components are in invalid order
		InvalidGrouping, ///< Position components have invalid grouping

		Valid = 100, ///< Position is valid
		Custom = 101 ///< Position is valid but custom
	};

	/**
	 * @brief Utility class for working with position validation results
	 */
	class PositionValidationResults
	{
	public:
		/**
		 * @brief Convert a string to a PositionValidationResult enum
		 * @param name The name of the validation result
		 * @return The corresponding PositionValidationResult
		 * @throws std::invalid_argument If the name is unknown
		 */
		static PositionValidationResult FromString( const std::string& name );
	};

	/**
	 * @brief Container for standard values of a codebook
	 */
	class CodebookStandardValues
	{
	private:
		CodebookName m_name;
		std::unordered_set<std::string> m_standardValues;

	public:
		/**
		 * @brief Default constructor
		 */
		CodebookStandardValues() = default;

		/**
		 * @brief Construct with name and values
		 * @param name The codebook name
		 * @param standardValues The set of standard values
		 */
		CodebookStandardValues( CodebookName name, const std::unordered_set<std::string>& standardValues );

		/**
		 * @brief Get the number of standard values
		 * @return The count of standard values
		 */
		size_t Count() const;

		/**
		 * @brief Check if a value is contained in standard values
		 * @param tagValue The value to check
		 * @return True if the value is in standard values or is a numeric position
		 */
		bool Contains( const std::string& tagValue ) const;

		/**
		 * @brief Iterator type for traversing standard values
		 */
		using iterator = std::unordered_set<std::string>::const_iterator;

		/**
		 * @brief Get iterator to the beginning
		 * @return Iterator to the first standard value
		 */
		iterator begin() const;

		/**
		 * @brief Get iterator to the end
		 * @return Iterator past the last standard value
		 */
		iterator end() const;
	};

	/**
	 * @brief Container for groups of a codebook
	 */
	class CodebookGroups
	{
	private:
		std::unordered_set<std::string> m_groups;

	public:
		/**
		 * @brief Default constructor
		 */
		CodebookGroups() = default;

		/**
		 * @brief Construct with groups
		 * @param groups The set of groups
		 */
		CodebookGroups( const std::unordered_set<std::string>& groups );

		/**
		 * @brief Get the number of groups
		 * @return The count of groups
		 */
		size_t Count() const;

		/**
		 * @brief Check if a group is contained
		 * @param group The group to check
		 * @return True if the group exists
		 */
		bool Contains( const std::string& group ) const;

		/**
		 * @brief Iterator type for traversing groups
		 */
		using iterator = std::unordered_set<std::string>::const_iterator;

		/**
		 * @brief Get iterator to the beginning
		 * @return Iterator to the first group
		 */
		iterator begin() const;

		/**
		 * @brief Get iterator to the end
		 * @return Iterator past the last group
		 */
		iterator end() const;
	};

	/**
	 * @brief A codebook containing standard values and their groups
	 */
	class Codebook
	{
	private:
		CodebookName m_name;
		std::unordered_map<std::string, std::string> m_groupMap;
		CodebookStandardValues m_standardValues;
		CodebookGroups m_groups;
		std::unordered_map<std::string, std::vector<std::string>> m_rawData;

	public:
		/**
		 * @brief Default constructor
		 */
		Codebook() = default;

		/**
		 * @brief Construct from DTO
		 * @param dto The data transfer object
		 * @throws std::invalid_argument If the DTO has an unknown name
		 */
		Codebook( const CodebookDto& dto );

		/**
		 * @brief Get the codebook name
		 * @return The codebook name
		 */
		CodebookName GetName() const;

		/**
		 * @brief Get the groups
		 * @return Reference to the groups container
		 */
		const CodebookGroups& GetGroups() const;

		/**
		 * @brief Get the standard values
		 * @return Reference to the standard values container
		 */
		const CodebookStandardValues& GetStandardValues() const;

		/**
		 * @brief Get the raw data
		 * @return Map of groups to their values
		 */
		const std::unordered_map<std::string, std::vector<std::string>>& GetRawData() const;

		/**
		 * @brief Check if a group exists
		 * @param group The group to check
		 * @return True if the group exists
		 */
		bool HasGroup( const std::string& group ) const;

		/**
		 * @brief Check if a value is a standard value
		 * @param value The value to check
		 * @return True if the value is standard
		 */
		bool HasStandardValue( const std::string& value ) const;

		/**
		 * @brief Try to create a metadata tag
		 * @param value The tag value
		 * @return The metadata tag, or none if invalid
		 */
		std::optional<MetadataTag> TryCreateTag( const std::string_view value ) const;

		/**
		 * @brief Create a metadata tag
		 * @param value The tag value
		 * @return The metadata tag
		 * @throws std::invalid_argument If the value is invalid
		 */
		MetadataTag CreateTag( const std::string& value ) const;

		/**
		 * @brief Validate a position string
		 * @param position The position to validate
		 * @return The validation result
		 */
		PositionValidationResult ValidatePosition( const std::string& position ) const;
	};
}
