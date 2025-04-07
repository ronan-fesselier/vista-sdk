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
		//-------------------------------------------------------------------------
		// Constants
		//-------------------------------------------------------------------------

		/** @brief Naming rule constant */
		static const std::string NamingRule;

		/** @brief Used codebooks array */
		static const std::vector<CodebookName> UsedCodebooks;

		//-------------------------------------------------------------------------
		// Constructors and Static Factories
		//-------------------------------------------------------------------------

		/**
		 * @brief Default constructor
		 */
		LocalIdBuilder();

		/**
		 * @brief Copy constructor
		 */
		LocalIdBuilder( const LocalIdBuilder& other );

		/**
		 * @brief Create a new LocalIdBuilder with the specified VIS version
		 * @param version The VIS version to use
		 * @return A new LocalIdBuilder instance
		 */
		static LocalIdBuilder create( VisVersion version );

		//-------------------------------------------------------------------------
		// Core Interface Implementation (ILocalIdBuilder)
		//-------------------------------------------------------------------------

		/**
		 * @brief Build a LocalId from this builder
		 * @return The built LocalId
		 * @throws std::invalid_argument If the builder is in an invalid state
		 */
		virtual LocalId build() const override;

		/**
		 * @brief Get the VIS version
		 * @return Optional VIS version
		 */
		virtual std::optional<VisVersion> getVisVersion() const override;

		/**
		 * @brief Get verbose mode flag
		 * @return true if verbose mode is enabled
		 */
		virtual bool getVerboseMode() const override;

		/**
		 * @brief Get primary item
		 * @return Optional primary item GmodPath
		 */
		virtual const std::optional<GmodPath>& getPrimaryItem() const override;

		/**
		 * @brief Get secondary item
		 * @return Optional secondary item GmodPath
		 */
		virtual const std::optional<GmodPath>& getSecondaryItem() const override;

		/**
		 * @brief Check if has custom tag
		 * @return true if has custom tag
		 */
		virtual bool hasCustomTag() const override;

		/**
		 * @brief Get metadata tags
		 * @return List of metadata tags
		 */
		virtual const std::vector<MetadataTag> getMetadataTags() const override;

		/**
		 * @brief Check if valid
		 * @return true if valid
		 */
		virtual bool isValid() const override;

		/**
		 * @brief Check if empty
		 * @return true if empty
		 */
		virtual bool isEmpty() const override;

		/**
		 * @brief Convert to string
		 * @return String representation
		 */
		virtual std::string toString() const override;

		//-------------------------------------------------------------------------
		// Accessors
		//-------------------------------------------------------------------------

		/**
		 * @brief Get items
		 * @return LocalIdItems
		 */
		const LocalIdItems& getItems() const;

		/**
		 * @brief Get quantity tag
		 * @return Optional quantity metadata tag
		 */
		const std::optional<MetadataTag>& getQuantity() const;

		/**
		 * @brief Get content tag
		 * @return Optional content metadata tag
		 */
		const std::optional<MetadataTag>& getContent() const;

		/**
		 * @brief Get calculation tag
		 * @return Optional calculation metadata tag
		 */
		const std::optional<MetadataTag>& getCalculation() const;

		/**
		 * @brief Get state tag
		 * @return Optional state metadata tag
		 */
		const std::optional<MetadataTag>& getState() const;

		/**
		 * @brief Get command tag
		 * @return Optional command metadata tag
		 */
		const std::optional<MetadataTag>& getCommand() const;

		/**
		 * @brief Get type tag
		 * @return Optional type metadata tag
		 */
		const std::optional<MetadataTag>& getType() const;

		/**
		 * @brief Get position tag
		 * @return Optional position metadata tag
		 */
		const std::optional<MetadataTag>& getPosition() const;

		/**
		 * @brief Get detail tag
		 * @return Optional detail metadata tag
		 */
		const std::optional<MetadataTag>& getDetail() const;

		/**
		 * @brief Check if metadata is empty
		 * @return true if metadata is empty
		 */
		bool isEmptyMetadata() const;

		//-------------------------------------------------------------------------
		// Builder Methods - Core Properties
		//-------------------------------------------------------------------------

		/**
		 * @brief Set VIS version from string
		 * @param visVersion VIS version string
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder withVisVersion( const std::string& visVersion ) override;

		/**
		 * @brief Set VIS version from enum
		 * @param version VIS version enum
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder withVisVersion( VisVersion version ) override;

		/**
		 * @brief Try to set VIS version from optional
		 * @param version Optional VIS version
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder tryWithVisVersion( const std::optional<VisVersion>& version ) override;

		/**
		 * @brief Try to set VIS version from string
		 * @param visVersionStr Optional VIS version string
		 * @param[out] succeeded true if operation succeeded
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder tryWithVisVersion( const std::optional<std::string>& visVersionStr, bool& succeeded ) override;

		/**
		 * @brief Remove VIS version
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder withoutVisVersion() override;

		/**
		 * @brief Set verbose mode
		 * @param verboseMode Verbose mode flag
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder withVerboseMode( bool verboseMode ) override;

		//-------------------------------------------------------------------------
		// Builder Methods - Items
		//-------------------------------------------------------------------------

		/**
		 * @brief Set primary item
		 * @param item GmodPath item
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder withPrimaryItem( const GmodPath& item ) override;

		/**
		 * @brief Try to set primary item from optional
		 * @param item Optional GmodPath
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder tryWithPrimaryItem( const std::optional<GmodPath>& item ) override;

		/**
		 * @brief Try to set primary item from optional
		 * @param item Optional GmodPath
		 * @param[out] succeeded true if operation succeeded
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder tryWithPrimaryItem( const std::optional<GmodPath>& item, bool& succeeded ) override;

		/**
		 * @brief Remove primary item
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder withoutPrimaryItem() override;

		/**
		 * @brief Set secondary item
		 * @param item GmodPath item
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder withSecondaryItem( const GmodPath& item ) override;

		/**
		 * @brief Try to set secondary item from optional
		 * @param item Optional GmodPath
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder tryWithSecondaryItem( const std::optional<GmodPath>& item ) override;

		/**
		 * @brief Try to set secondary item from optional
		 * @param item Optional GmodPath
		 * @param[out] succeeded true if operation succeeded
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder tryWithSecondaryItem( const std::optional<GmodPath>& item, bool& succeeded ) override;

		/**
		 * @brief Remove secondary item
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder withoutSecondaryItem() override;

		//-------------------------------------------------------------------------
		// Builder Methods - Metadata Tags
		//-------------------------------------------------------------------------

		/**
		 * @brief Add metadata tag
		 * @param metadataTag MetadataTag to add
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder withMetadataTag( const MetadataTag& metadataTag ) override;

		/**
		 * @brief Try to add metadata tag from optional
		 * @param metadataTag Optional MetadataTag
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder tryWithMetadataTag( const std::optional<MetadataTag>& metadataTag ) override;

		/**
		 * @brief Try to add metadata tag from optional
		 * @param metadataTag Optional MetadataTag
		 * @param[out] succeeded true if operation succeeded
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder tryWithMetadataTag( const std::optional<MetadataTag>& metadataTag, bool& succeeded ) override;

		/**
		 * @brief Remove metadata tag by name
		 * @param name CodebookName of tag to remove
		 * @return Builder for method chaining
		 */
		virtual LocalIdBuilder withoutMetadataTag( CodebookName name ) override;

		/**
		 * @brief Remove quantity tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withoutQuantity();

		/**
		 * @brief Remove content tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withoutContent();

		/**
		 * @brief Remove calculation tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withoutCalculation();

		/**
		 * @brief Remove state tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withoutState();

		/**
		 * @brief Remove command tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withoutCommand();

		/**
		 * @brief Remove type tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withoutType();

		/**
		 * @brief Remove position tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withoutPosition();

		/**
		 * @brief Remove detail tag
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withoutDetail();

		/**
		 * @brief Set quantity tag
		 * @param quantity MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withQuantity( const MetadataTag& quantity );

		/**
		 * @brief Set content tag
		 * @param content MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withContent( const MetadataTag& content );

		/**
		 * @brief Set calculation tag
		 * @param calculation MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withCalculation( const MetadataTag& calculation );

		/**
		 * @brief Set state tag
		 * @param state MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withState( const MetadataTag& state );

		/**
		 * @brief Set command tag
		 * @param command MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withCommand( const MetadataTag& command );

		/**
		 * @brief Set type tag
		 * @param type MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withType( const MetadataTag& type );

		/**
		 * @brief Set position tag
		 * @param position MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withPosition( const MetadataTag& position );

		/**
		 * @brief Set detail tag
		 * @param detail MetadataTag to set
		 * @return Builder for method chaining
		 */
		LocalIdBuilder withDetail( const MetadataTag& detail );

		//-------------------------------------------------------------------------
		// Static Parsing Methods
		//-------------------------------------------------------------------------

		/**
		 * @brief Static method to parse a LocalId string
		 * @param localIdStr String to parse
		 * @return LocalIdBuilder representing the parsed string
		 * @throws std::invalid_argument If parsing fails
		 */
		static LocalIdBuilder parse( const std::string& localIdStr );

		/**
		 * @brief Static method to try parsing a LocalId string
		 * @param localIdStr String to parse
		 * @param[out] errors Object to receive parsing errors
		 * @param[out] localId Optional to receive the parsed builder
		 * @return true if parsing succeeded
		 */
		static bool tryParse( const std::string& localIdStr, ParsingErrors& errors, std::optional<LocalIdBuilder>& localId );

		/**
		 * @brief Static method to try parsing a LocalId string with detailed errors
		 * @param localIdStr String to parse
		 * @param[out] errorBuilder Object to receive detailed parsing errors
		 * @param[out] localIdBuilder Optional to receive the parsed builder
		 * @return true if parsing succeeded
		 */
		static bool tryParseInternal( const std::string& localIdStr, LocalIdParsingErrorBuilder& errorBuilder, std::optional<LocalIdBuilder>& localIdBuilder );

		//-------------------------------------------------------------------------
		// Operators and Utilities
		//-------------------------------------------------------------------------

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
		size_t getHashCode() const;

		/**
		 * @brief Convert to string
		 * @param builder Stream to write to
		 */
		void toString( std::stringstream& builder ) const;

	private:
		//-------------------------------------------------------------------------
		// Member Variables
		//-------------------------------------------------------------------------
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
	};
}
