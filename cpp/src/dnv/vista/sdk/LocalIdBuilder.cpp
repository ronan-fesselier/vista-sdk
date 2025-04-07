#include "pch.h"

#include "dnv/vista/sdk/CodebookName.h"
#include "dnv/vista/sdk/GmodPath.h"
#include "dnv/vista/sdk/LocalId.h"
#include "dnv/vista/sdk/LocalIdBuilder.h"
#include "dnv/vista/sdk/VisVersion.h"
#include "dnv/vista/sdk/VIS.h"
#include "dnv/vista/sdk/ParsingErrors.h"
#include <sstream>
#include <algorithm>

namespace dnv::vista::sdk
{
	const std::string LocalIdBuilder::NamingRule = "dnv-v2";

	const std::vector<CodebookName> LocalIdBuilder::UsedCodebooks = {
		CodebookName::Quantity,
		CodebookName::Content,
		CodebookName::State,
		CodebookName::Command,
		CodebookName::FunctionalServices,
		CodebookName::MaintenanceCategory,
		CodebookName::ActivityType,
		CodebookName::Position,
		CodebookName::Detail };

	LocalIdBuilder::LocalIdBuilder()
		: m_visVersion( std::nullopt ), m_verboseMode( false ), m_items(), m_quantity( std::nullopt ), m_content( std::nullopt ), m_calculation( std::nullopt ), m_state( std::nullopt ), m_command( std::nullopt ), m_type( std::nullopt ), m_position( std::nullopt ), m_detail( std::nullopt )
	{
	}

	LocalIdBuilder::LocalIdBuilder( const LocalIdBuilder& other )
		: m_visVersion( other.m_visVersion ), m_verboseMode( other.m_verboseMode ), m_items( other.m_items ), m_quantity( other.m_quantity ), m_content( other.m_content ), m_calculation( other.m_calculation ), m_state( other.m_state ), m_command( other.m_command ), m_type( other.m_type ), m_position( other.m_position ), m_detail( other.m_detail )
	{
	}

	std::optional<VisVersion> LocalIdBuilder::GetVisVersion() const
	{
		return m_visVersion;
	}

	bool LocalIdBuilder::GetVerboseMode() const
	{
		return m_verboseMode;
	}

	const LocalIdItems& LocalIdBuilder::GetItems() const
	{
		return m_items;
	}

	const std::optional<GmodPath>& LocalIdBuilder::GetPrimaryItem() const
	{
		return m_items.GetPrimaryItem();
	}

	const std::optional<GmodPath>& LocalIdBuilder::GetSecondaryItem() const
	{
		return m_items.GetSecondaryItem();
	}

	const std::optional<MetadataTag>& LocalIdBuilder::GetQuantity() const
	{
		return m_quantity;
	}

	const std::optional<MetadataTag>& LocalIdBuilder::GetContent() const
	{
		return m_content;
	}

	const std::optional<MetadataTag>& LocalIdBuilder::GetCalculation() const
	{
		return m_calculation;
	}

	const std::optional<MetadataTag>& LocalIdBuilder::GetState() const
	{
		return m_state;
	}

	const std::optional<MetadataTag>& LocalIdBuilder::GetCommand() const
	{
		return m_command;
	}

	const std::optional<MetadataTag>& LocalIdBuilder::GetType() const
	{
		return m_type;
	}

	const std::optional<MetadataTag>& LocalIdBuilder::GetPosition() const
	{
		return m_position;
	}

	const std::optional<MetadataTag>& LocalIdBuilder::GetDetail() const
	{
		return m_detail;
	}

	bool LocalIdBuilder::HasCustomTag() const
	{
		return ( m_quantity.has_value() && m_quantity->IsCustom() ) ||
			   ( m_calculation.has_value() && m_calculation->IsCustom() ) ||
			   ( m_content.has_value() && m_content->IsCustom() ) ||
			   ( m_position.has_value() && m_position->IsCustom() ) ||
			   ( m_state.has_value() && m_state->IsCustom() ) ||
			   ( m_command.has_value() && m_command->IsCustom() ) ||
			   ( m_type.has_value() && m_type->IsCustom() ) ||
			   ( m_detail.has_value() && m_detail->IsCustom() );
	}

	std::vector<MetadataTag> LocalIdBuilder::GetMetadataTags() const
	{
		std::vector<MetadataTag> tags;

		if ( m_quantity.has_value() )
			tags.push_back( *m_quantity );

		if ( m_calculation.has_value() )
			tags.push_back( *m_calculation );

		if ( m_content.has_value() )
			tags.push_back( *m_content );

		if ( m_position.has_value() )
			tags.push_back( *m_position );

		if ( m_state.has_value() )
			tags.push_back( *m_state );

		if ( m_command.has_value() )
			tags.push_back( *m_command );

		if ( m_type.has_value() )
			tags.push_back( *m_type );

		if ( m_detail.has_value() )
			tags.push_back( *m_detail );

		return tags;
	}

	bool LocalIdBuilder::IsValid() const
	{
		return m_visVersion.has_value() &&
			   m_items.GetPrimaryItem().has_value() &&
			   ( m_quantity.has_value() ||
				   m_calculation.has_value() ||
				   m_content.has_value() ||
				   m_position.has_value() ||
				   m_state.has_value() ||
				   m_command.has_value() ||
				   m_type.has_value() ||
				   m_detail.has_value() );
	}

	bool LocalIdBuilder::IsEmpty() const
	{
		return !m_items.GetPrimaryItem().has_value() &&
			   !m_items.GetSecondaryItem().has_value() &&
			   !m_quantity.has_value() &&
			   !m_calculation.has_value() &&
			   !m_content.has_value() &&
			   !m_position.has_value() &&
			   !m_state.has_value() &&
			   !m_command.has_value() &&
			   !m_type.has_value() &&
			   !m_detail.has_value();
	}

	bool LocalIdBuilder::IsEmptyMetadata() const
	{
		return !m_quantity.has_value() &&
			   !m_calculation.has_value() &&
			   !m_content.has_value() &&
			   !m_position.has_value() &&
			   !m_state.has_value() &&
			   !m_command.has_value() &&
			   !m_type.has_value() &&
			   !m_detail.has_value();
	}

	LocalIdBuilder LocalIdBuilder::WithVisVersion( const std::string& visVersion )
	{
		bool succeeded;
		auto result = TryWithVisVersion( visVersion, succeeded );
		if ( !succeeded )
		{
			throw std::invalid_argument( "Invalid VisVersion string: " + visVersion );
		}
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithVisVersion( VisVersion version )
	{
		LocalIdBuilder result = *this;
		result.m_visVersion = version;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::TryWithVisVersion( const std::optional<VisVersion>& version )
	{
		if ( !version.has_value() )
		{
			return *this;
		}

		LocalIdBuilder result = *this;
		result.m_visVersion = version;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::TryWithVisVersion( const std::optional<std::string>& visVersionStr, bool& succeeded )
	{
		if ( !visVersionStr.has_value() || visVersionStr->empty() )
		{
			succeeded = false;
			return *this;
		}

		VisVersion version;
		if ( VisVersionExtensions::TryParse( *visVersionStr, version ) )
		{
			succeeded = true;
			LocalIdBuilder result = *this;
			result.m_visVersion = version;
			return result;
		}

		succeeded = false;
		return *this;
	}

	LocalIdBuilder LocalIdBuilder::WithoutVisVersion()
	{
		LocalIdBuilder result = *this;
		result.m_visVersion = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithVerboseMode( bool verboseMode )
	{
		LocalIdBuilder result = *this;
		result.m_verboseMode = verboseMode;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithPrimaryItem( const GmodPath& item )
	{
		bool succeeded;
		LocalIdBuilder result = TryWithPrimaryItem( item, succeeded );
		if ( !succeeded )
		{
			throw std::invalid_argument( "Invalid primary item" );
		}
		return result;
	}

	LocalIdBuilder LocalIdBuilder::TryWithPrimaryItem( const std::optional<GmodPath>& item )
	{
		bool dummy;
		return TryWithPrimaryItem( item, dummy );
	}

	LocalIdBuilder LocalIdBuilder::TryWithPrimaryItem( const std::optional<GmodPath>& item, bool& succeeded )
	{
		if ( !item.has_value() )
		{
			succeeded = false;
			return *this;
		}

		succeeded = true;
		LocalIdBuilder result = *this;
		result.m_items = LocalIdItems( item, m_items.GetSecondaryItem() );
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithoutPrimaryItem()
	{
		LocalIdBuilder result = *this;
		result.m_items = LocalIdItems( std::nullopt, m_items.GetSecondaryItem() );
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithSecondaryItem( const GmodPath& item )
	{
		bool succeeded;
		LocalIdBuilder result = TryWithSecondaryItem( item, succeeded );
		if ( !succeeded )
		{
			throw std::invalid_argument( "Invalid secondary item" );
		}
		return result;
	}

	LocalIdBuilder LocalIdBuilder::TryWithSecondaryItem( const std::optional<GmodPath>& item )
	{
		bool dummy;
		return TryWithSecondaryItem( item, dummy );
	}

	LocalIdBuilder LocalIdBuilder::TryWithSecondaryItem( const std::optional<GmodPath>& item, bool& succeeded )
	{
		if ( !item.has_value() )
		{
			succeeded = false;
			return *this;
		}

		succeeded = true;
		LocalIdBuilder result = *this;
		result.m_items = LocalIdItems( m_items.GetPrimaryItem(), item );
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithoutSecondaryItem()
	{
		LocalIdBuilder result = *this;
		result.m_items = LocalIdItems( m_items.GetPrimaryItem(), std::nullopt );
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithMetadataTag( const MetadataTag& metadataTag )
	{
		bool succeeded;
		LocalIdBuilder result = TryWithMetadataTag( metadataTag, succeeded );
		if ( !succeeded )
		{
			throw std::invalid_argument( "Invalid metadata tag: " + metadataTag.ToString() );
		}
		return result;
	}

	LocalIdBuilder LocalIdBuilder::TryWithMetadataTag( const std::optional<MetadataTag>& metadataTag )
	{
		bool succeeded;
		return TryWithMetadataTag( metadataTag, succeeded );
	}

	LocalIdBuilder LocalIdBuilder::TryWithMetadataTag( const std::optional<MetadataTag>& metadataTag, bool& succeeded )
	{
		if ( !metadataTag.has_value() )
		{
			succeeded = false;
			return *this;
		}

		succeeded = true;
		switch ( metadataTag->GetName() )
		{
			case CodebookName::Quantity:
				return WithQuantity( *metadataTag );
			case CodebookName::Content:
				return WithContent( *metadataTag );
			case CodebookName::Calculation:
				return WithCalculation( *metadataTag );
			case CodebookName::State:
				return WithState( *metadataTag );
			case CodebookName::Command:
				return WithCommand( *metadataTag );
			case CodebookName::Type:
				return WithType( *metadataTag );
			case CodebookName::Position:
				return WithPosition( *metadataTag );
			case CodebookName::Detail:
				return WithDetail( *metadataTag );
			default:
				succeeded = false;
				return *this;
		}
	}

	LocalIdBuilder LocalIdBuilder::WithoutMetadataTag( CodebookName name )
	{
		switch ( name )
		{
			case CodebookName::Quantity:
				return WithoutQuantity();
			case CodebookName::Content:
				return WithoutContent();
			case CodebookName::Calculation:
				return WithoutCalculation();
			case CodebookName::State:
				return WithoutState();
			case CodebookName::Command:
				return WithoutCommand();
			case CodebookName::Type:
				return WithoutType();
			case CodebookName::Position:
				return WithoutPosition();
			case CodebookName::Detail:
				return WithoutDetail();
			default:
				return *this;
		}
	}

	LocalIdBuilder LocalIdBuilder::WithoutQuantity()
	{
		LocalIdBuilder result = *this;
		result.m_quantity = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithoutContent()
	{
		LocalIdBuilder result = *this;
		result.m_content = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithoutCalculation()
	{
		LocalIdBuilder result = *this;
		result.m_calculation = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithoutState()
	{
		LocalIdBuilder result = *this;
		result.m_state = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithoutCommand()
	{
		LocalIdBuilder result = *this;
		result.m_command = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithoutType()
	{
		LocalIdBuilder result = *this;
		result.m_type = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithoutPosition()
	{
		LocalIdBuilder result = *this;
		result.m_position = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithoutDetail()
	{
		LocalIdBuilder result = *this;
		result.m_detail = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithQuantity( const MetadataTag& quantity )
	{
		LocalIdBuilder result = *this;
		result.m_quantity = quantity;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithContent( const MetadataTag& content )
	{
		LocalIdBuilder result = *this;
		result.m_content = content;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithCalculation( const MetadataTag& calculation )
	{
		LocalIdBuilder result = *this;
		result.m_calculation = calculation;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithState( const MetadataTag& state )
	{
		LocalIdBuilder result = *this;
		result.m_state = state;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithCommand( const MetadataTag& command )
	{
		LocalIdBuilder result = *this;
		result.m_command = command;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithType( const MetadataTag& type )
	{
		LocalIdBuilder result = *this;
		result.m_type = type;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithPosition( const MetadataTag& position )
	{
		LocalIdBuilder result = *this;
		result.m_position = position;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::WithDetail( const MetadataTag& detail )
	{
		LocalIdBuilder result = *this;
		result.m_detail = detail;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::Create( VisVersion version )
	{
		LocalIdBuilder builder;
		builder.m_visVersion = version;
		return builder;
	}

	LocalId LocalIdBuilder::Build() const
	{
		if ( IsEmpty() )
			throw std::invalid_argument( "Cannot build to LocalId from empty LocalIdBuilder" );
		if ( !IsValid() )
			throw std::invalid_argument( "Cannot build to LocalId from invalid LocalIdBuilder" );

		return LocalId( *this );
	}

	bool LocalIdBuilder::operator==( const LocalIdBuilder& other ) const
	{
		return m_visVersion == other.m_visVersion &&
			   m_verboseMode == other.m_verboseMode &&
			   m_items == other.m_items &&
			   m_quantity == other.m_quantity &&
			   m_content == other.m_content &&
			   m_calculation == other.m_calculation &&
			   m_state == other.m_state &&
			   m_command == other.m_command &&
			   m_type == other.m_type &&
			   m_position == other.m_position &&
			   m_detail == other.m_detail;
	}

	bool LocalIdBuilder::operator!=( const LocalIdBuilder& other ) const
	{
		return !( *this == other );
	}

	size_t LocalIdBuilder::GetHashCode() const
	{
		size_t hash = 0;

		auto hashCombine = [&hash]( const auto& value ) {
			std::hash<std::remove_cv_t<std::remove_reference_t<decltype( value )>>> hasher;
			hash ^= hasher( value ) + 0x9e3779b9 + ( hash << 6 ) + ( hash >> 2 );
		};

		if ( m_visVersion.has_value() )
			hashCombine( static_cast<int>( m_visVersion.value() ) );

		hashCombine( m_verboseMode );

		if ( m_items.GetPrimaryItem().has_value() )
			hashCombine( m_items.GetPrimaryItem()->ToString() );

		if ( m_items.GetSecondaryItem().has_value() )
			hashCombine( m_items.GetSecondaryItem()->ToString() );

		if ( m_quantity.has_value() )
			hashCombine( m_quantity->ToString() );

		if ( m_calculation.has_value() )
			hashCombine( m_calculation->ToString() );

		if ( m_content.has_value() )
			hashCombine( m_content->ToString() );

		if ( m_position.has_value() )
			hashCombine( m_position->ToString() );

		if ( m_state.has_value() )
			hashCombine( m_state->ToString() );

		if ( m_command.has_value() )
			hashCombine( m_command->ToString() );

		if ( m_type.has_value() )
			hashCombine( m_type->ToString() );

		if ( m_detail.has_value() )
			hashCombine( m_detail->ToString() );

		return hash;
	}

	void LocalIdBuilder::ToString( std::stringstream& builder ) const
	{
		if ( !m_visVersion.has_value() )
			throw std::invalid_argument( "No VisVersion configured on LocalId" );

		std::string namingRule = "/" + NamingRule + "/";
		builder << namingRule;

		builder << "vis-";
		builder << VisVersionExtensions::ToVersionString( *m_visVersion );
		builder << '/';

		m_items.Append( builder, m_verboseMode );

		builder << "meta/";

		auto appendMeta = [&builder]( const std::optional<MetadataTag>& metadataTag ) {
			if ( metadataTag.has_value() )
			{
				std::ostringstream tagBuilder;
				metadataTag->ToString( tagBuilder );
				builder << tagBuilder.str() << "/";
			}
		};

		// NOTE: order of metadatatags matter,
		// should not be changed unless changed in the naming rule/standard
		appendMeta( m_quantity );
		appendMeta( m_content );
		appendMeta( m_calculation );
		appendMeta( m_state );
		appendMeta( m_command );
		appendMeta( m_type );
		appendMeta( m_position );
		appendMeta( m_detail );

		std::string str = builder.str();
		if ( !str.empty() && str.back() == '/' )
		{
			builder.str( "" );
			builder << str.substr( 0, str.length() - 1 );
		}
	}

	std::string LocalIdBuilder::ToString() const
	{
		std::stringstream builder;
		ToString( builder );
		return builder.str();
	}

	LocalIdBuilder LocalIdBuilder::Parse( const std::string& localIdStr )
	{
		ParsingErrors errors;
		std::optional<LocalIdBuilder> builder;
		if ( !TryParse( localIdStr, errors, builder ) )
		{
			throw std::invalid_argument( "Failed to parse LocalId: " + errors.ToString() );
		}
		return *builder;
	}

	bool LocalIdBuilder::TryParse( const std::string& localIdStr, ParsingErrors& errors, std::optional<LocalIdBuilder>& localId )
	{
		localId = std::nullopt;
		LocalIdParsingErrorBuilder errorBuilder;

		try
		{
			if ( localIdStr.empty() )
			{
				SPDLOG_ERROR( "TryParse: Empty string detected" );
				errorBuilder.AddError( static_cast<LocalIdParsingState>( 0 ), "LocalId string is empty" );
				errors = errorBuilder.Build();
				return false;
			}
			SPDLOG_INFO( "TryParse: String not empty" );

			const std::string namingRulePrefix = "/" + NamingRule + "/";
			SPDLOG_INFO( "TryParse: Checking for naming rule: {}", namingRulePrefix );
			if ( localIdStr.find( namingRulePrefix ) != 0 )
			{
				SPDLOG_ERROR( "TryParse: Invalid naming rule prefix" );
				errorBuilder.AddError( static_cast<LocalIdParsingState>( 0 ), "Invalid naming rule prefix" );
				errors = errorBuilder.Build();
				return false;
			}
			SPDLOG_INFO( "TryParse: Valid naming rule prefix found" );

			size_t visVersionPos = localIdStr.find( "vis-" );
			if ( visVersionPos == std::string::npos )
			{
				SPDLOG_ERROR( "TryParse: Missing VIS version" );
				errorBuilder.AddError( static_cast<LocalIdParsingState>( 1 ), "Missing VIS version" );
				errors = errorBuilder.Build();
				return false;
			}
			SPDLOG_INFO( "TryParse: Found VIS version position at: {}", visVersionPos );

			size_t visVersionEndPos = localIdStr.find( "/", visVersionPos );
			if ( visVersionEndPos == std::string::npos )
			{
				SPDLOG_ERROR( "TryParse: Invalid VIS version format (no end delimiter)" );
				errorBuilder.AddError( static_cast<LocalIdParsingState>( 1 ), "Invalid VIS version format" );
				errors = errorBuilder.Build();
				return false;
			}
			SPDLOG_INFO( "TryParse: Found VIS version end position at: {}", visVersionEndPos );

			std::string visVersionStr = localIdStr.substr( visVersionPos + 4, visVersionEndPos - ( visVersionPos + 4 ) );
			SPDLOG_INFO( "TryParse: Extracted VIS version string: {}", visVersionStr );

			VisVersion visVersion;
			if ( !VisVersionExtensions::TryParse( visVersionStr, visVersion ) )
			{
				SPDLOG_ERROR( "TryParse: Invalid VIS version: {}", visVersionStr );
				errorBuilder.AddError( static_cast<LocalIdParsingState>( 1 ), "Invalid VIS version: " + visVersionStr );
				errors = errorBuilder.Build();
				return false;
			}
			SPDLOG_INFO( "TryParse: Successfully parsed VIS version: {}", visVersionStr );

			LocalIdBuilder builder = Create( visVersion );
			SPDLOG_INFO( "TryParse: Created LocalIdBuilder with version: {}", visVersionStr );

			std::string pathRemainder = localIdStr.substr( visVersionEndPos + 1 );
			SPDLOG_INFO( "TryParse: Path remainder: {}", pathRemainder );

			size_t metaPos = pathRemainder.find( "/meta" );
			if ( metaPos == std::string::npos )
			{
				SPDLOG_ERROR( "TryParse: Missing metadata section" );
				errorBuilder.AddError( static_cast<LocalIdParsingState>( 4 ), "Missing metadata section" );
				errors = errorBuilder.Build();
				return false;
			}
			SPDLOG_INFO( "TryParse: Found meta position at: {}", metaPos );

			std::string itemPath = pathRemainder.substr( 0, metaPos );
			SPDLOG_INFO( "TryParse: Item path: {}", itemPath );

			size_t secPos = itemPath.find( "/sec/" );
			bool hasSecondaryItem = secPos != std::string::npos;
			SPDLOG_INFO( "TryParse: Has secondary item: {}", hasSecondaryItem ? "yes" : "no" );

			std::string primaryItemStr;
			std::string secondaryItemStr;

			if ( hasSecondaryItem )
			{
				primaryItemStr = itemPath.substr( 0, secPos );
				secondaryItemStr = itemPath.substr( secPos + 5 );
				SPDLOG_INFO( "TryParse: Primary item: {}, Secondary item: {}", primaryItemStr, secondaryItemStr );
			}
			else
			{
				primaryItemStr = itemPath;
				SPDLOG_INFO( "TryParse: Primary item only: {}", primaryItemStr );
			}

			SPDLOG_INFO( "TryParse: Getting VIS instance" );
			auto vis = VIS::Instance();
			SPDLOG_INFO( "TryParse: Getting GMOD for version" );
			auto gmod = vis.GetGmod( visVersion );
			SPDLOG_INFO( "TryParse: Successfully retrieved GMOD" );

			if ( !primaryItemStr.empty() )
			{
				SPDLOG_INFO( "TryParse: Parsing primary item: {}", primaryItemStr );
				std::optional<GmodPath> primaryItem;
				if ( !gmod.TryParsePath( primaryItemStr, primaryItem ) || !primaryItem.has_value() )
				{
					SPDLOG_ERROR( "TryParse: Invalid primary item path: {}", primaryItemStr );
					errorBuilder.AddError( static_cast<LocalIdParsingState>( 2 ), "Invalid primary item path: " + primaryItemStr );
					errors = errorBuilder.Build();
					return false;
				}
				SPDLOG_INFO( "TryParse: Successfully parsed primary item" );

				builder = builder.WithPrimaryItem( *primaryItem );
			}

			if ( hasSecondaryItem && !secondaryItemStr.empty() )
			{
				SPDLOG_INFO( "TryParse: Parsing secondary item: {}", secondaryItemStr );
				std::optional<GmodPath> secondaryItem;
				if ( !gmod.TryParsePath( secondaryItemStr, secondaryItem ) || !secondaryItem.has_value() )
				{
					SPDLOG_ERROR( "TryParse: Invalid secondary item path: {}", secondaryItemStr );
					errorBuilder.AddError( static_cast<LocalIdParsingState>( 3 ), "Invalid secondary item path: " + secondaryItemStr );
					errors = errorBuilder.Build();
					return false;
				}
				SPDLOG_INFO( "TryParse: Successfully parsed secondary item" );

				builder = builder.WithSecondaryItem( *secondaryItem );
			}

			std::string metadataStr = pathRemainder.substr( metaPos + 5 );
			SPDLOG_INFO( "TryParse: Metadata string: {}", metadataStr );

			if ( !metadataStr.empty() && metadataStr[0] == '/' )
			{
				metadataStr = metadataStr.substr( 1 );
				SPDLOG_INFO( "TryParse: Metadata string after removing leading slash: {}", metadataStr );
			}

			if ( !metadataStr.empty() )
			{
				SPDLOG_INFO( "TryParse: Parsing metadata tags" );

				std::vector<std::string> metadataTags;
				size_t pos = 0;
				std::string token;
				while ( ( pos = metadataStr.find( '/' ) ) != std::string::npos )
				{
					token = metadataStr.substr( 0, pos );
					if ( !token.empty() )
					{
						metadataTags.push_back( token );
					}
					metadataStr.erase( 0, pos + 1 );
				}
				if ( !metadataStr.empty() )
				{
					metadataTags.push_back( metadataStr );
				}

				SPDLOG_INFO( "TryParse: Found {} metadata tags", metadataTags.size() );

				for ( const auto& tagStr : metadataTags )
				{
					SPDLOG_INFO( "TryParse: Processing tag: {}", tagStr );

					size_t dashPos = tagStr.find( '-' );
					size_t tildePos = tagStr.find( '~' );

					bool isCustom = tildePos != std::string::npos;
					size_t separatorPos = isCustom ? tildePos : dashPos;

					SPDLOG_INFO( "TryParse: Tag is {} separator at position {}",
						isCustom ? "custom" : "standard", separatorPos );

					if ( separatorPos == std::string::npos || separatorPos == 0 || separatorPos == tagStr.length() - 1 )
					{
						SPDLOG_ERROR( "TryParse: Invalid metadata tag format: {}", tagStr );
						errorBuilder.AddError( static_cast<LocalIdParsingState>( 101 ), "Invalid metadata tag format: " + tagStr );
						continue;
					}

					std::string prefix = tagStr.substr( 0, separatorPos );
					std::string value = tagStr.substr( separatorPos + 1 );
					SPDLOG_INFO( "TryParse: Tag prefix: {}, value: {}", prefix, value );

					CodebookName codebookName;
					bool validPrefix = true;

					if ( prefix == "qty" )
						codebookName = CodebookName::Quantity;
					else if ( prefix == "cnt" )
						codebookName = CodebookName::Content;
					else if ( prefix == "calc" )
						codebookName = CodebookName::Calculation;
					else if ( prefix == "state" )
						codebookName = CodebookName::State;
					else if ( prefix == "cmd" )
						codebookName = CodebookName::Command;
					else if ( prefix == "type" )
						codebookName = CodebookName::Type;
					else if ( prefix == "pos" )
						codebookName = CodebookName::Position;
					else if ( prefix == "detail" )
						codebookName = CodebookName::Detail;
					else
					{
						validPrefix = false;
						SPDLOG_ERROR( "TryParse: Unknown metadata tag prefix: {}", prefix );
						errorBuilder.AddError( static_cast<LocalIdParsingState>( 101 ), "Unknown metadata tag prefix: " + prefix );
					}

					if ( validPrefix )
					{
						SPDLOG_INFO( "TryParse: Creating metadata tag with name: {}, value: {}, isCustom: {}",
							static_cast<int>( codebookName ), value, isCustom );

						MetadataTag tag( codebookName, value, isCustom );
						builder = builder.WithMetadataTag( tag );
					}
				}
			}
			else
			{
				SPDLOG_INFO( "TryParse: No metadata tags to process" );
			}

			SPDLOG_INFO( "TryParse: Checking if builder is valid" );
			if ( !builder.IsValid() )
			{
				SPDLOG_ERROR( "TryParse: Builder is not valid" );

				if ( !builder.GetPrimaryItem().has_value() )
				{
					SPDLOG_ERROR( "TryParse: Missing primary item" );
					errorBuilder.AddError( static_cast<LocalIdParsingState>( 2 ), "Missing primary item" );
				}

				if ( builder.IsEmptyMetadata() )
				{
					SPDLOG_ERROR( "TryParse: Missing metadata tags" );
					errorBuilder.AddError( static_cast<LocalIdParsingState>( 100 ), "Missing metadata tags" );
				}

				errors = errorBuilder.Build();
				return false;
			}
			SPDLOG_INFO( "TryParse: Builder is valid" );

			bool hasVerboseMode = localIdStr.find( '~' ) != std::string::npos;
			SPDLOG_INFO( "TryParse: Setting verbose mode to: {}", hasVerboseMode ? "true" : "false" );
			builder = builder.WithVerboseMode( hasVerboseMode );

			localId = builder;
			errors = errorBuilder.Build();
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error: Exception during parsing: {}", e.what() );
			errorBuilder.AddError( static_cast<LocalIdParsingState>( 101 ), std::string( "Exception: " ) + e.what() );
			errors = errorBuilder.Build();
			return false;
		}
		catch ( ... )
		{
			SPDLOG_ERROR( "Error: Unknown exception during parsing" );
			errorBuilder.AddError( static_cast<LocalIdParsingState>( 101 ), "Unknown exception" );
			errors = errorBuilder.Build();
			return false;
		}
	}

	bool LocalIdBuilder::TryParse( const std::string& localIdStr, std::optional<LocalIdBuilder>& localId )
	{
		SPDLOG_INFO( "TryParse (simple): Starting with input: {}", localIdStr );
		ParsingErrors errors;
		bool result = TryParse( localIdStr, errors, localId );
		SPDLOG_INFO( "TryParse (simple): Completed with result: {}", result ? "success" : "failure" );
		return result;
	}
}
