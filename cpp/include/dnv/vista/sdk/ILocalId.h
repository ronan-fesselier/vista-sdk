#pragma once

namespace dnv::vista::sdk
{
	enum class VisVersion;
	class MetadataTag;
	class GmodPath;
	class ParsingErrors;
	/**
	 * @brief Interface for Local ID
	 *
	 * Provides the core properties and methods for working with Local IDs
	 * in the VIS (Vessel Information Structure) system.
	 */
	template <typename T>
	class ILocalId
	{
	public:
		virtual ~ILocalId() = default;

		/**
		 * @brief Get the VIS version
		 * @return The VIS version
		 */
		virtual VisVersion GetVisVersion() const = 0;

		/**
		 * @brief Check if verbose mode is enabled
		 * @return True if verbose mode is enabled
		 */
		virtual bool GetVerboseMode() const = 0;

		/**
		 * @brief Get the primary item
		 * @return The primary item (GmodPath)
		 */
		virtual const GmodPath& GetPrimaryItem() const = 0;

		/**
		 * @brief Get the secondary item
		 * @return The secondary item, if present
		 */
		virtual std::optional<GmodPath> GetSecondaryItem() const = 0;

		/**
		 * @brief Check if the Local ID has any custom tags
		 * @return True if custom tags exist
		 */
		virtual bool HasCustomTag() const = 0;

		/**
		 * @brief Get all metadata tags
		 * @return A vector of metadata tags
		 */
		virtual std::vector<MetadataTag> GetMetadataTags() const = 0;

		/**
		 * @brief Convert the Local ID to a string representation
		 * @return The string representation of the Local ID
		 */
		virtual std::string ToString() const = 0;

	public:
		/**
		 * @brief Parse a string into a Local ID
		 * @param localIdStr The string to parse
		 * @return The parsed Local ID
		 * @throws std::invalid_argument If parsing fails
		 */
		static T Parse( const std::string& localIdStr );

		/**
		 * @brief Try to parse a string into a Local ID
		 * @param localIdStr The string to parse
		 * @param errors Output parameter for parsing errors
		 * @param localId Output parameter for the parsed Local ID
		 * @return True if parsing succeeded, false otherwise
		 */
		static bool TryParse( const std::string& localIdStr, ParsingErrors& errors, std::optional<T>& localId );
	};
}
