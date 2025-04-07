#pragma once

#include "ILocalId.h"
#include "ParsingErrors.h"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace dnv::vista::sdk
{
	class GmodPath;
	class MetadataTag;
	enum class CodebookName;
	enum class VisVersion;

	/**
	 * @brief Interface for local ID builders with fluent builder pattern
	 *
	 * Defines the core properties and methods that all local ID builders must implement,
	 * including access to essential components like version, items, and metadata tags.
	 * Implementations should provide immutable builder pattern semantics for creating
	 * local IDs according to ISO 19848.
	 *
	 * @tparam TBuilder The concrete builder type (derived class, used for method chaining)
	 * @tparam TResult The result type produced by Build() (typically LocalId)
	 */
	template <typename TBuilder, typename TResult>
	class ILocalIdBuilder
	{
	public:
		/** @brief Virtual destructor */
		virtual ~ILocalIdBuilder() = default;

		/**
		 * @brief Get the VIS version associated with this local ID
		 * @return The VIS version if set, or std::nullopt if not set
		 */
		virtual std::optional<VisVersion> GetVisVersion() const = 0;

		/**
		 * @brief Check if verbose mode is enabled
		 * @return True if verbose mode is enabled, false otherwise
		 */
		virtual bool GetVerboseMode() const = 0;

		/**
		 * @brief Get the primary item path
		 * @return Reference to the optional primary item path
		 */
		virtual const std::optional<GmodPath>& GetPrimaryItem() const = 0;

		/**
		 * @brief Get the secondary item path
		 * @return Reference to the optional secondary item path
		 */
		virtual const std::optional<GmodPath>& GetSecondaryItem() const = 0;

		/**
		 * @brief Check if this local ID has any custom tags
		 * @return True if at least one custom tag exists, false otherwise
		 */
		virtual bool HasCustomTag() const = 0;

		/**
		 * @brief Get all metadata tags
		 * @return Vector of all metadata tags
		 */
		virtual std::vector<MetadataTag> GetMetadataTags() const = 0;

		/**
		 * @brief Check if this local ID is valid
		 * @return True if valid, false otherwise
		 */
		virtual bool IsValid() const = 0;

		/**
		 * @brief Check if this local ID is empty
		 * @return True if empty, false otherwise
		 */
		virtual bool IsEmpty() const = 0;

		/**
		 * @brief Convert to string representation
		 * @return String representation of the local ID
		 */
		virtual std::string ToString() const = 0;

	public:
		/**
		 * @brief Set the VIS version using a string
		 * @param visVersion The VIS version as a string
		 * @return New builder instance with the updated VIS version
		 * @throws std::invalid_argument If the visVersion is invalid
		 */
		virtual TBuilder WithVisVersion( const std::string& visVersion ) = 0;

		/**
		 * @brief Set the VIS version
		 * @param version The VIS version
		 * @return New builder instance with the updated VIS version
		 */
		virtual TBuilder WithVisVersion( VisVersion version ) = 0;

		/**
		 * @brief Try to set the VIS version
		 * @param version Optional VIS version
		 * @return New builder instance with the updated VIS version, or unchanged if version is nullopt
		 */
		virtual TBuilder TryWithVisVersion( const std::optional<VisVersion>& version ) = 0;

		/**
		 * @brief Try to set the VIS version from a string
		 * @param visVersionStr Optional VIS version string
		 * @param[out] succeeded Set to true if version was successfully set, false otherwise
		 * @return New builder instance with the updated VIS version, or unchanged if setting failed
		 */
		virtual TBuilder TryWithVisVersion( const std::optional<std::string>& visVersionStr, bool& succeeded ) = 0;

		/**
		 * @brief Remove the VIS version
		 * @return New builder instance with no VIS version
		 */
		virtual TBuilder WithoutVisVersion() = 0;

		/**
		 * @brief Set verbose mode
		 * @param verboseMode True to enable verbose mode, false otherwise
		 * @return New builder instance with the updated verbose mode setting
		 */
		virtual TBuilder WithVerboseMode( bool verboseMode ) = 0;

		/**
		 * @brief Set the primary item
		 * @param item The GMOD path to set as primary item
		 * @return New builder instance with the updated primary item
		 */
		virtual TBuilder WithPrimaryItem( const GmodPath& item ) = 0;

		/**
		 * @brief Try to set the primary item
		 * @param item Optional GMOD path to set as primary item
		 * @return New builder instance with the updated primary item, or unchanged if item is nullopt
		 */
		virtual TBuilder TryWithPrimaryItem( const std::optional<GmodPath>& item ) = 0;

		/**
		 * @brief Try to set the primary item
		 * @param item Optional GMOD path to set as primary item
		 * @param[out] succeeded Set to true if item was successfully set, false otherwise
		 * @return New builder instance with the updated primary item, or unchanged if setting failed
		 */
		virtual TBuilder TryWithPrimaryItem( const std::optional<GmodPath>& item, bool& succeeded ) = 0;

		/**
		 * @brief Remove the primary item
		 * @return New builder instance with no primary item
		 */
		virtual TBuilder WithoutPrimaryItem() = 0;

		/**
		 * @brief Set the secondary item
		 * @param item The GMOD path to set as secondary item
		 * @return New builder instance with the updated secondary item
		 */
		virtual TBuilder WithSecondaryItem( const GmodPath& item ) = 0;

		/**
		 * @brief Try to set the secondary item
		 * @param item Optional GMOD path to set as secondary item
		 * @return New builder instance with the updated secondary item, or unchanged if item is nullopt
		 */
		virtual TBuilder TryWithSecondaryItem( const std::optional<GmodPath>& item ) = 0;

		/**
		 * @brief Try to set the secondary item
		 * @param item Optional GMOD path to set as secondary item
		 * @param[out] succeeded Set to true if item was successfully set, false otherwise
		 * @return New builder instance with the updated secondary item, or unchanged if setting failed
		 */
		virtual TBuilder TryWithSecondaryItem( const std::optional<GmodPath>& item, bool& succeeded ) = 0;

		/**
		 * @brief Remove the secondary item
		 * @return New builder instance with no secondary item
		 */
		virtual TBuilder WithoutSecondaryItem() = 0;

		/**
		 * @brief Add a metadata tag
		 * @param metadataTag The metadata tag to add
		 * @return New builder instance with the added metadata tag
		 */
		virtual TBuilder WithMetadataTag( const MetadataTag& metadataTag ) = 0;

		/**
		 * @brief Try to add a metadata tag
		 * @param metadataTag Optional metadata tag to add
		 * @return New builder instance with the added metadata tag, or unchanged if tag is nullopt
		 */
		virtual TBuilder TryWithMetadataTag( const std::optional<MetadataTag>& metadataTag ) = 0;

		/**
		 * @brief Try to add a metadata tag
		 * @param metadataTag Optional metadata tag to add
		 * @param[out] succeeded Set to true if tag was successfully added, false otherwise
		 * @return New builder instance with the added metadata tag, or unchanged if adding failed
		 */
		virtual TBuilder TryWithMetadataTag( const std::optional<MetadataTag>& metadataTag, bool& succeeded ) = 0;

		/**
		 * @brief Remove a metadata tag by codebook name
		 * @param name The codebook name of the tag to remove
		 * @return New builder instance with the specified tag removed
		 */
		virtual TBuilder WithoutMetadataTag( CodebookName name ) = 0;

		/**
		 * @brief Build the final result object
		 * @return The constructed result object
		 * @throws std::invalid_argument If the builder state is invalid
		 */
		virtual TResult Build() const = 0;

		/**
		 * @brief Parse a local ID string into a builder
		 * @param localIdStr The local ID string to parse
		 * @return The builder representing the parsed local ID
		 * @throws std::invalid_argument If parsing fails
		 */
		static TBuilder Parse( const std::string& localIdStr )
		{
			ParsingErrors errors;
			std::optional<TBuilder> builder;
			if ( !TryParse( localIdStr, errors, builder ) )
			{
				SPDLOG_ERROR( "Failed to parse LocalId: {}", localIdStr );
				throw std::invalid_argument( "Failed to parse LocalId: " + errors.ToString() );
			}
			return builder.value();
		}

		/**
		 * @brief Try to parse a local ID string
		 * @param localIdStr The local ID string to parse
		 * @param[out] localId The optional builder to receive the result
		 * @return True if parsing succeeded, false otherwise
		 */
		static bool TryParse( const std::string& localIdStr, std::optional<TBuilder>& localId )
		{
			ParsingErrors errors;
			return TryParse( localIdStr, errors, localId );
		}

		/**
		 * @brief Try to parse a local ID string with detailed error information
		 * @param localIdStr The local ID string to parse
		 * @param[out] errors Object to receive parsing errors
		 * @param[out] localId The optional builder to receive the result
		 * @return True if parsing succeeded, false otherwise
		 */
		static bool TryParse( const std::string& localIdStr, ParsingErrors& errors, std::optional<TBuilder>& localId );
	};
}
