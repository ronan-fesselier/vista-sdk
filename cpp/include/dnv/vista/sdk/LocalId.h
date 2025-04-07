#pragma once

#include "ILocalId.h"
#include "LocalIdBuilder.h"

namespace dnv::vista::sdk
{
	enum class VisVersion;
	class GmodPath;
	class MetadataTag;
	class ParsingErrors;

	/**
	 * @brief Local ID class for identifying items in VIS
	 */
	class LocalId : public ILocalId<LocalId>
	{
	public:
		/**
		 * @brief Naming rule constant
		 */
		static const std::string NamingRule;

	private:
		LocalIdBuilder m_builder;

	public:
		/**
		 * @brief Constructor from builder
		 * @param builder The LocalIdBuilder to construct from
		 * @throws std::invalid_argument if builder is empty or invalid
		 */
		explicit LocalId( const LocalIdBuilder& builder );

		/**
		 * @brief Get the builder
		 * @return The LocalIdBuilder
		 */
		const LocalIdBuilder& GetBuilder() const;

		/**
		 * @brief Get the VIS version
		 * @return The VIS version
		 */
		VisVersion GetVisVersion() const override;

		/**
		 * @brief Check if verbose mode is enabled
		 * @return true if verbose mode is enabled
		 */
		bool GetVerboseMode() const override;

		/**
		 * @brief Get the primary item
		 * @return The primary item (GmodPath)
		 */
		const GmodPath& GetPrimaryItem() const override;

		/**
		 * @brief Get the secondary item
		 * @return The secondary item, if present
		 */
		std::optional<GmodPath> GetSecondaryItem() const override;

		/**
		 * @brief Get metadata tags
		 * @return Vector of metadata tags
		 */
		std::vector<MetadataTag> GetMetadataTags() const override;

		/**
		 * @brief Get specific metadata tags
		 */
		std::optional<MetadataTag> GetQuantity() const;
		std::optional<MetadataTag> GetContent() const;
		std::optional<MetadataTag> GetCalculation() const;
		std::optional<MetadataTag> GetState() const;
		std::optional<MetadataTag> GetCommand() const;
		std::optional<MetadataTag> GetType() const;
		std::optional<MetadataTag> GetPosition() const;
		std::optional<MetadataTag> GetDetail() const;

		/**
		 * @brief Check if the LocalId has any custom tags
		 * @return true if has custom tag
		 */
		bool HasCustomTag() const override;

		/**
		 * @brief Convert to string
		 * @return String representation
		 */
		std::string ToString() const override;

		/**
		 * @brief Parse a string to a LocalId
		 * @param localIdStr The string to parse
		 * @return The parsed LocalId
		 * @throws std::invalid_argument if parsing fails
		 */
		static LocalId Parse( const std::string& localIdStr );

		/**
		 * @brief Try to parse a string to a LocalId
		 * @param localIdStr The string to parse
		 * @param errors Output parameter for parsing errors
		 * @param localId Output parameter for the parsed LocalId
		 * @return true if parsing succeeded
		 */
		static bool TryParse( const std::string& localIdStr, ParsingErrors& errors, std::optional<LocalId>& localId );

		/**
		 * @brief Equality operators
		 */
		bool operator==( const LocalId& other ) const;
		bool operator!=( const LocalId& other ) const;

		/**
		 * @brief Get hash code
		 * @return Hash code of the LocalId
		 */
		size_t GetHashCode() const;
	};

	/**
	 * Represents the parsing state for LocalId.
	 */
	enum class LocalIdParsingState
	{
		NamingRule,
		VisVersion,
		PrimaryItem,
		SecondaryItem,
		ItemDescription,
		MetaQuantity,
		MetaContent,
		MetaCalculation,
		MetaState,
		MetaCommand,
		MetaType,
		MetaPosition,
		MetaDetail,

		// For "other" errors
		EmptyState = 100,
		Formatting = 101,
		Completeness = 102,

		// UniversalId
		NamingEntity = 200,
		IMONumber = 201
	};

	/**
	 * Builder for parsing errors related to LocalId.
	 */
	class LocalIdParsingErrorBuilder
	{
	public:
		LocalIdParsingErrorBuilder() = default;

		LocalIdParsingErrorBuilder& AddError( LocalIdParsingState state );
		LocalIdParsingErrorBuilder& AddError( LocalIdParsingState state, const std::string& message );
		bool HasError() const;
		ParsingErrors Build() const;
		static LocalIdParsingErrorBuilder Create();

	private:
		std::vector<std::pair<LocalIdParsingState, std::string>> m_errors;

		const std::unordered_map<LocalIdParsingState, std::string> m_predefinedErrorMessages = {
			{ LocalIdParsingState::NamingRule, "Missing or invalid naming rule" },
			{ LocalIdParsingState::VisVersion, "Missing or invalid vis version" },
			{ LocalIdParsingState::PrimaryItem, "Invalid or missing Primary item. Local IDs require at least a primary item and 1 metadata tag." },
			{ LocalIdParsingState::SecondaryItem, "Invalid secondary item" },
			{ LocalIdParsingState::ItemDescription, "Missing or invalid /meta prefix" },
			{ LocalIdParsingState::MetaQuantity, "Invalid metadata tag: Quantity" },
			{ LocalIdParsingState::MetaContent, "Invalid metadata tag: Content" },
			{ LocalIdParsingState::MetaCommand, "Invalid metadata tag: Command" },
			{ LocalIdParsingState::MetaPosition, "Invalid metadata tag: Position" },
			{ LocalIdParsingState::MetaCalculation, "Invalid metadata tag: Calculation" },
			{ LocalIdParsingState::MetaState, "Invalid metadata tag: State" },
			{ LocalIdParsingState::MetaType, "Invalid metadata tag: Type" },
			{ LocalIdParsingState::MetaDetail, "Invalid metadata tag: Detail" },
			{ LocalIdParsingState::EmptyState, "Missing primary path or metadata" } };
	};
}
