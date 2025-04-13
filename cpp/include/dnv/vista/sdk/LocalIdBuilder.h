#pragma once

#include "ILocalIdBuilder.h"
#include "LocalIdItems.h"
#include "MetadataTag.h"

namespace dnv::vista::sdk
{
	enum class VisVersion;
	enum class CodebookName;
	class GmodPath;
	class ParsingErrors;
	class LocalId;
	class LocalIdParsingErrorBuilder;

	/**
	 * @brief Builder class for LocalId objects, implementing the ILocalIdBuilder interface.
	 */
	class LocalIdBuilder : public ILocalIdBuilder<LocalIdBuilder, LocalId>
	{
	public:
		/** @brief Naming rule constant */
		static const std::string NamingRule;

		/** @brief Used codebooks array */
		static const std::vector<CodebookName> UsedCodebooks;

	private:
		std::optional<VisVersion> m_visVersion;
		bool m_verboseMode = false;
		LocalIdItems m_items;
		std::optional<MetadataTag> m_quantity;
		std::optional<MetadataTag> m_content;
		std::optional<MetadataTag> m_calculation;
		std::optional<MetadataTag> m_state;
		std::optional<MetadataTag> m_command;
		std::optional<MetadataTag> m_type;
		std::optional<MetadataTag> m_position;
		std::optional<MetadataTag> m_detail;

	public:
		/**
		 * @brief Default constructor
		 */
		LocalIdBuilder();

		/**
		 * @brief Copy constructor
		 */
		LocalIdBuilder( const LocalIdBuilder& other );

		/**
		 * @brief Get the VIS version
		 * @return Optional VIS version
		 */
		std::optional<VisVersion> GetVisVersion() const override;

		/**
		 * @brief Get verbose mode flag
		 * @return true if verbose mode is enabled
		 */
		bool GetVerboseMode() const override;

		/**
		 * @brief Get items
		 * @return LocalIdItems
		 */
		const LocalIdItems& GetItems() const;

		/**
		 * @brief Get primary item
		 * @return Optional primary item GmodPath
		 */
		const std::optional<GmodPath>& GetPrimaryItem() const override;

		/**
		 * @brief Get secondary item
		 * @return Optional secondary item GmodPath
		 */
		const std::optional<GmodPath>& GetSecondaryItem() const override;

		/**
		 * @brief Get quantity tag
		 * @return Optional quantity metadata tag
		 */
		const std::optional<MetadataTag>& GetQuantity() const;

		/**
		 * @brief Get content tag
		 * @return Optional content metadata tag
		 */
		const std::optional<MetadataTag>& GetContent() const;

		/**
		 * @brief Get calculation tag
		 * @return Optional calculation metadata tag
		 */
		const std::optional<MetadataTag>& GetCalculation() const;

		/**
		 * @brief Get state tag
		 * @return Optional state metadata tag
		 */
		const std::optional<MetadataTag>& GetState() const;

		/**
		 * @brief Get command tag
		 * @return Optional command metadata tag
		 */
		const std::optional<MetadataTag>& GetCommand() const;

		/**
		 * @brief Get type tag
		 * @return Optional type metadata tag
		 */
		const std::optional<MetadataTag>& GetType() const;

		/**
		 * @brief Get position tag
		 * @return Optional position metadata tag
		 */
		const std::optional<MetadataTag>& GetPosition() const;

		/**
		 * @brief Get detail tag
		 * @return Optional detail metadata tag
		 */
		const std::optional<MetadataTag>& GetDetail() const;

		/**
		 * @brief Check if has custom tag
		 * @return true if has custom tag
		 */
		bool HasCustomTag() const override;

		/**
		 * @brief Get metadata tags
		 * @return List of metadata tags
		 */
		std::vector<MetadataTag> GetMetadataTags() const override;

		/**
		 * @brief Check if valid
		 * @return true if valid
		 */
		bool IsValid() const override;

		/**
		 * @brief Check if empty
		 * @return true if empty
		 */
		bool IsEmpty() const override;

		/**
		 * @brief Check if metadata is empty
		 * @return true if metadata is empty
		 */
		bool IsEmptyMetadata() const;

		/**
		 * @brief Set VIS version from string
		 * @param visVersion VIS version string
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithVisVersion( const std::string& visVersion ) override;

		/**
		 * @brief Set VIS version from enum
		 * @param version VIS version enum
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithVisVersion( VisVersion version ) override;

		/**
		 * @brief Try to set VIS version from optional
		 * @param version Optional VIS version
		 * @return Builder for method chaining
		 */
		LocalIdBuilder TryWithVisVersion( const std::optional<VisVersion>& version ) override;

		/**
		 * @brief Try to set VIS version from string
		 * @param visVersionStr Optional VIS version string
		 * @param[out] succeeded true if operation succeeded
		 * @return Builder for method chaining
		 */
		LocalIdBuilder TryWithVisVersion( const std::optional<std::string>& visVersionStr, bool& succeeded ) override;

		/**
		 * @brief Remove VIS version
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithoutVisVersion() override;

		/**
		 * @brief Set verbose mode
		 * @param verboseMode Verbose mode flag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithVerboseMode( bool verboseMode ) override;

		/**
		 * @brief Set primary item
		 * @param item GmodPath item
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithPrimaryItem( const GmodPath& item ) override;

		/**
		 * @brief Try to set primary item from optional
		 * @param item Optional GmodPath
		 * @return Builder for method chaining
		 */
		LocalIdBuilder TryWithPrimaryItem( const std::optional<GmodPath>& item ) override;

		/**
		 * @brief Try to set primary item from optional
		 * @param item Optional GmodPath
		 * @param[out] succeeded true if operation succeeded
		 * @return Builder for method chaining
		 */
		LocalIdBuilder TryWithPrimaryItem( const std::optional<GmodPath>& item, bool& succeeded ) override;

		/**
		 * @brief Remove primary item
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithoutPrimaryItem() override;

		/**
		 * @brief Set secondary item
		 * @param item GmodPath item
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithSecondaryItem( const GmodPath& item ) override;

		/**
		 * @brief Try to set secondary item from optional
		 * @param item Optional GmodPath
		 * @return Builder for method chaining
		 */
		LocalIdBuilder TryWithSecondaryItem( const std::optional<GmodPath>& item ) override;

		/**
		 * @brief Try to set secondary item from optional
		 * @param item Optional GmodPath
		 * @param[out] succeeded true if operation succeeded
		 * @return Builder for method chaining
		 */
		LocalIdBuilder TryWithSecondaryItem( const std::optional<GmodPath>& item, bool& succeeded ) override;

		/**
		 * @brief Remove secondary item
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithoutSecondaryItem() override;

		/**
		 * @brief Add metadata tag
		 * @param metadataTag MetadataTag to add
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithMetadataTag( const MetadataTag& metadataTag ) override;

		/**
		 * @brief Try to add metadata tag from optional
		 * @param metadataTag Optional MetadataTag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder TryWithMetadataTag( const std::optional<MetadataTag>& metadataTag ) override;

		/**
		 * @brief Try to add metadata tag from optional
		 * @param metadataTag Optional MetadataTag
		 * @param[out] succeeded true if operation succeeded
		 * @return Builder for method chaining
		 */
		LocalIdBuilder TryWithMetadataTag( const std::optional<MetadataTag>& metadataTag, bool& succeeded ) override;

		/**
		 * @brief Remove metadata tag by name
		 * @param name CodebookName of tag to remove
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithoutMetadataTag( CodebookName name ) override;

		/**
		 * @brief Remove quantity tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithoutQuantity();

		/**
		 * @brief Remove content tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithoutContent();

		/**
		 * @brief Remove calculation tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithoutCalculation();

		/**
		 * @brief Remove state tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithoutState();

		/**
		 * @brief Remove command tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithoutCommand();

		/**
		 * @brief Remove type tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithoutType();

		/**
		 * @brief Remove position tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithoutPosition();

		/**
		 * @brief Remove detail tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithoutDetail();

	public:
		/**
		 * @brief Set quantity tag
		 * @param quantity MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder
		WithQuantity( const MetadataTag& quantity );

		/**
		 * @brief Set content tag
		 * @param content MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithContent( const MetadataTag& content );

		/**
		 * @brief Set calculation tag
		 * @param calculation MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithCalculation( const MetadataTag& calculation );

		/**
		 * @brief Set state tag
		 * @param state MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithState( const MetadataTag& state );

		/**
		 * @brief Set command tag
		 * @param command MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithCommand( const MetadataTag& command );

		/**
		 * @brief Set type tag
		 * @param type MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithType( const MetadataTag& type );

		/**
		 * @brief Set position tag
		 * @param position MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithPosition( const MetadataTag& position );

		/**
		 * @brief Set detail tag
		 * @param detail MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder WithDetail( const MetadataTag& detail );

	public:
		/**
		 * @brief Create a new LocalIdBuilder with the specified VIS version
		 * @param version The VIS version to use
		 * @return A new LocalIdBuilder instance
		 */
		static LocalIdBuilder Create( VisVersion version );

		/**
		 * @brief Build a LocalId from this builder
		 * @return The built LocalId
		 * @throws std::invalid_argument If the builder is in an invalid state
		 */
		LocalId Build() const override;

		/**
		 * @brief Static method to parse a LocalId string
		 * @param localIdStr String to parse
		 * @return LocalIdBuilder representing the parsed string
		 * @throws std::invalid_argument If parsing fails
		 */
		static LocalIdBuilder Parse( const std::string& localIdStr );

		static bool TryParse( const std::string& localIdStr, ParsingErrors& errors, std::optional<LocalIdBuilder>& localId );

		/**
		 * @brief Static method to try parsing a LocalId string with detailed errors
		 * @param localIdStr String to parse
		 * @param[out] errors Object to receive parsing errors
		 * @param[out] localId Optional to receive the parsed builder
		 * @return true if parsing succeeded
		 */
		static bool TryParseInternal( const std::string& localIdStr, LocalIdParsingErrorBuilder& errorBuilder, std::optional<LocalIdBuilder>& localIdBuilder );

		/**
		 * @brief Equality operator
		 * @param other Builder to compare with
		 * @return true if equal
		 */
		bool operator==( const LocalIdBuilder& other ) const;

		/**
		 * @brief Inequality operator
		 * @param other Builder to compare with
		 * @return true if not equal
		 */
		bool operator!=( const LocalIdBuilder& other ) const;

		/**
		 * @brief Get hash code
		 * @return Hash code for this builder
		 */
		size_t GetHashCode() const;

		/**
		 * @brief Convert to string
		 * @param builder Stream to write to
		 */
		void ToString( std::stringstream& builder ) const;

		/**
		 * @brief Convert to string
		 * @return String representation
		 */
		std::string ToString() const override;
	};
}
