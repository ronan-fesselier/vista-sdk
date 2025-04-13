#include "pch.h"

#include "dnv/vista/sdk/CodebookName.h"
#include "dnv/vista/sdk/GmodPath.h"
#include "dnv/vista/sdk/LocalId.h"
#include "dnv/vista/sdk/LocalIdBuilder.h"
#include "dnv/vista/sdk/VisVersion.h"
#include "dnv/vista/sdk/VIS.h"
#include "dnv/vista/sdk/ParsingErrors.h"

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
			SPDLOG_ERROR( "Invalid VisVersion string: {}", visVersion );
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
			SPDLOG_ERROR( "Invalid primary item: {}", item.ToString() );
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
			SPDLOG_ERROR( "Invalid secondary item: {}", item.ToString() );
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
			SPDLOG_ERROR( "Invalid metadata tag: {}", metadataTag.ToString() );
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
		{
			SPDLOG_ERROR( "Cannot build to LocalId from empty LocalIdBuilder" );
			throw std::invalid_argument( "Cannot build to LocalId from empty LocalIdBuilder" );
		}
		if ( !IsValid() )
		{
			SPDLOG_ERROR( "Cannot build to LocalId from invalid LocalIdBuilder" );
			throw std::invalid_argument( "Cannot build to LocalId from invalid LocalIdBuilder" );
		}

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
		{
			SPDLOG_ERROR( "No VisVersion configured on LocalId" );
			throw std::invalid_argument( "No VisVersion configured on LocalId" );
		}

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
			SPDLOG_ERROR( "Failed to parse LocalId: {}", errors.ToString() );
			throw std::invalid_argument( "Failed to parse LocalId: " + errors.ToString() );
		}
		return *builder;
	}

	bool LocalIdBuilder::TryParse( const std::string& localIdStr, ParsingErrors& errors, std::optional<LocalIdBuilder>& localId )
	{
		LocalIdParsingErrorBuilder errorBuilder;

		bool result = TryParseInternal( localIdStr, errorBuilder, localId );
		errors = errorBuilder.Build();
		return result;
	}

	void AddError( LocalIdParsingErrorBuilder& errorBuilder, LocalIdParsingState state, const std::string& message )
	{
		errorBuilder.AddError( state, message );
	}

	void AdvanceParser( size_t& i, const std::string& segment )
	{
		i += segment.length() + 1;

		void AdvanceParser( size_t& i, const std::string& segment, LocalIdParsingState& state )
		{
			i += segment.length() + 1;
			state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
		}

		void AdvanceParser( size_t& i, const std::string& segment, LocalIdParsingState& state, LocalIdParsingState nextState )
		{
			i += segment.length() + 1;
			state = nextState;
		}

		void AdvanceState( LocalIdParsingState & state, LocalIdParsingState nextState )
		{
			state = nextState;
		}

		std::pair<int, int> GetNextStateIndexes( const std::string& span, LocalIdParsingState state )
		{
			size_t customIndex = span.find( "~" );
			size_t endOfCustomIndex = ( customIndex + 1 + 1 );

			size_t metaIndex = span.find( "/meta" );
			size_t endOfMetaIndex = ( metaIndex + 5 + 1 );
			bool isVerbose = customIndex < metaIndex;

			switch ( state )
			{
				case LocalIdParsingState::PrimaryItem:
				{
					size_t secIndex = span.find( "/sec" );
					size_t endOfSecIndex = ( secIndex + 4 + 1 );

					if ( secIndex != std::string::npos )
						return { secIndex, endOfSecIndex };

					if ( isVerbose && customIndex != std::string::npos )
						return { customIndex, endOfCustomIndex };

					return { metaIndex, endOfMetaIndex };
				}

				case LocalIdParsingState::SecondaryItem:
					if ( isVerbose && customIndex != std::string::npos )
						return { customIndex, endOfCustomIndex };
					return { metaIndex, endOfMetaIndex };

				default:
					return { metaIndex, endOfMetaIndex };
			}
		}

		static std::optional<LocalIdParsingState> MetaPrefixToState( const std::string& prefix )
		{
			if ( prefix == "qty" )
				return LocalIdParsingState::MetaQuantity;
			if ( prefix == "cnt" )
				return LocalIdParsingState::MetaContent;
			if ( prefix == "calc" )
				return LocalIdParsingState::MetaCalculation;
			if ( prefix == "state" )
				return LocalIdParsingState::MetaState;
			if ( prefix == "cmd" )
				return LocalIdParsingState::MetaCommand;
			if ( prefix == "type" )
				return LocalIdParsingState::MetaType;
			if ( prefix == "pos" )
				return LocalIdParsingState::MetaPosition;
			if ( prefix == "detail" )
				return LocalIdParsingState::MetaDetail;

			return std::nullopt;
		}

		static std::optional<LocalIdParsingState> NextParsingState( LocalIdParsingState prev )
		{
			switch ( prev )
			{
				case LocalIdParsingState::MetaQuantity:
					return LocalIdParsingState::MetaContent;
				case LocalIdParsingState::MetaContent:
					return LocalIdParsingState::MetaCalculation;
				case LocalIdParsingState::MetaCalculation:
					return LocalIdParsingState::MetaState;
				case LocalIdParsingState::MetaState:
					return LocalIdParsingState::MetaCommand;
				case LocalIdParsingState::MetaCommand:
					return LocalIdParsingState::MetaType;
				case LocalIdParsingState::MetaType:
					return LocalIdParsingState::MetaPosition;
				case LocalIdParsingState::MetaPosition:
					return LocalIdParsingState::MetaDetail;
				default:
					return std::nullopt;
			}
		}

		static std::string CodebookNameToString( CodebookName name )
		{
			switch ( name )
			{
				case CodebookName::Quantity:
					return "Quantity";
				case CodebookName::Content:
					return "Content";
				case CodebookName::Calculation:
					return "Calculation";
				case CodebookName::State:
					return "State";
				case CodebookName::Command:
					return "Command";
				case CodebookName::Type:
					return "Type";
				case CodebookName::FunctionalServices:
					return "FunctionalServices";
				case CodebookName::MaintenanceCategory:
					return "MaintenanceCategory";
				case CodebookName::ActivityType:
					return "ActivityType";
				case CodebookName::Position:
					return "Position";
				case CodebookName::Detail:
					return "Detail";
				default:
					return "Unknown";
			}
		}

		static bool ParseMetatag(
			CodebookName codebookName,
			LocalIdParsingState & state,
			size_t& i,
			const std::string& segment,
			std::optional<MetadataTag>& tag,
			const Codebooks* codebooks,
			LocalIdParsingErrorBuilder& errorBuilder )
		{
			if ( !codebooks )
				return false;

			size_t dashIndex = segment.find( '-' );
			size_t tildeIndex = segment.find( '~' );
			size_t prefixIndex = dashIndex == std::string::npos ? tildeIndex : dashIndex;
			if ( prefixIndex == std::string::npos )
			{
				AddError(
					errorBuilder,
					state,
					"Invalid metadata tag: missing prefix '-' or '~' in " + segment );
				AdvanceParser( i, segment, state );
				return true;
			}

			std::string prefix = segment.substr( 0, prefixIndex );

			std::optional<LocalIdParsingState> actualState = MetaPrefixToState( prefix );
			if ( !actualState.has_value() || actualState < state )
			{
				AddError( errorBuilder, state, "Invalid metadata tag: unknown prefix " + prefix );
				return false;
			}

			if ( actualState > state )
			{
				AdvanceState( state, actualState.value() );
				return true;
			}

			std::optional<LocalIdParsingState> nextState = NextParsingState( actualState.value() );

			std::string value = segment.substr( prefixIndex + 1 );
			if ( value.empty() )
			{
				AddError( errorBuilder, state, "Invalid " + CodebookNameToString( codebookName ) + " metadata tag: missing value" );
				return false;
			}

			tag = codebooks->TryCreateTag( codebookName, value );
			if ( !tag.has_value() )
			{
				if ( prefixIndex == tildeIndex )
					AddError(
						errorBuilder,
						state,
						"Invalid custom " + CodebookNameToString( codebookName ) + " metadata tag: failed to create " + value );
				else
					AddError(
						errorBuilder,
						state,
						"Invalid " + CodebookNameToString( codebookName ) + " metadata tag: failed to create " + value );

				AdvanceParser( i, segment, state );
				return true;
			}

			if ( prefixIndex == dashIndex && tag->IsCustom() )
				AddError(
					errorBuilder,
					state,
					"Invalid " + CodebookNameToString( codebookName ) + " metadata tag: '" + value + "'. Use prefix '~' for custom values" );

			if ( !nextState.has_value() )
				AdvanceParser( i, segment, state );
			else
				AdvanceParser( i, segment, state, nextState.value() );
			return true;
		}

		bool LocalIdBuilder::TryParseInternal( const std::string& localIdStr, LocalIdParsingErrorBuilder& errorBuilder, std::optional<LocalIdBuilder>& localIdBuilder )
		{
			localIdBuilder = std::nullopt;

			if ( localIdStr.empty() )
			{
				errorBuilder.AddError(
					LocalIdParsingState::Formatting,
					"LocalId string is empty" );
				return false;
			}

			if ( localIdStr[0] != '/' )
			{
				AddError(
					errorBuilder,
					LocalIdParsingState::Formatting,
					"Invalid string format" );
				return false;
			}

			VisVersion visVersion = static_cast<VisVersion>( std::numeric_limits<int>::max() );
			const Gmod* gmod = nullptr;
			const Codebooks* codebooks = nullptr;
			std::optional<GmodPath> primaryItem;
			std::optional<GmodPath> secondaryItem;
			std::optional<MetadataTag> qty;
			std::optional<MetadataTag> cnt;
			std::optional<MetadataTag> calc;
			std::optional<MetadataTag> stateTag;
			std::optional<MetadataTag> cmd;
			std::optional<MetadataTag> type;
			std::optional<MetadataTag> pos;
			std::optional<MetadataTag> detail;
			bool verbose = false;
			std::string predefinedMessage;
			bool invalidSecondaryItem = false;

			int primaryItemStart = -1;
			int secondaryItemStart = -1;

			LocalIdParsingState state = LocalIdParsingState::NamingRule;
			size_t i = 1;

			while ( state <= LocalIdParsingState::MetaDetail )
			{
				size_t nextStart = std::min( localIdStr.length(), i );
				size_t nextSlash = localIdStr.find( '/', nextStart );
				std::string segment;

				if ( nextSlash == std::string::npos )
				{
					segment = localIdStr.substr( nextStart );
				}
				else
				{
					segment = localIdStr.substr( nextStart, nextSlash - nextStart );
				}

				switch ( state )
				{
					case LocalIdParsingState::NamingRule:
						if ( segment.empty() )
						{
							AddError( errorBuilder, LocalIdParsingState::NamingRule,
								"Invalid naming rule prefix" );
							state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
							break;
						}

						if ( segment != NamingRule )
						{
							AddError( errorBuilder, LocalIdParsingState::NamingRule,
								"Invalid naming rule prefix: " + segment );
							return false;
						}

						AdvanceParser( i, segment, state );
						break;

					case LocalIdParsingState::VisVersion:
						if ( segment.empty() )
						{
							AddError( errorBuilder, LocalIdParsingState::VisVersion,
								"Missing VIS version" );
							state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
							break;
						}

						if ( segment.substr( 0, 4 ) != "vis-" )
						{
							AddError( errorBuilder, LocalIdParsingState::VisVersion,
								"Invalid VIS version format: " + segment );
							return false;
						}

						if ( !VisVersionExtensions::TryParse( segment.substr( 4 ), visVersion ) )
						{
							AddError( errorBuilder, LocalIdParsingState::VisVersion,
								"Invalid VIS version: " + segment.substr( 4 ) );
							return false;
						}

						try
						{
							VIS& vis = VIS::Instance();

							const Gmod& gmodRef = vis.GetGmod( visVersion );
							const Codebooks& codebooksRef = vis.GetCodebooks( visVersion );

							gmod = &gmodRef;
							codebooks = &codebooksRef;
						}
						catch ( ... )
						{
							SPDLOG_ERROR( "Failed to get Gmod or Codebooks for version: {}", static_cast<int>( visVersion ) );
							return false;
						}

						if ( !gmod || !codebooks )
							return false;

						AdvanceParser( i, segment, state );
						break;

					case LocalIdParsingState::PrimaryItem:
					{
						if ( segment.empty() )
						{
							if ( primaryItemStart != -1 )
							{
								if ( !gmod )
									return false;

								std::string path = localIdStr.substr( primaryItemStart, i - 1 - primaryItemStart );
								if ( !gmod->TryParsePath( path, primaryItem ) )
								{
									AddError(
										errorBuilder,
										LocalIdParsingState::PrimaryItem,
										"Invalid GmodPath in Primary item: " + path );
								}
							}
							else
							{
								AddError( errorBuilder, LocalIdParsingState::PrimaryItem, predefinedMessage );
							}

							AddError(
								errorBuilder,
								LocalIdParsingState::PrimaryItem,
								"Invalid or missing '/meta' prefix after Primary item" );

							state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
							break;
						}

						size_t dashIndex = segment.find( '-' );
						std::string code;
						if ( dashIndex == std::string::npos )
						{
							code = segment;
						}
						else
						{
							code = segment.substr( 0, dashIndex );
						}

						if ( !gmod )
							return false;

						if ( primaryItemStart == -1 )
						{
							GmodNode node;
							if ( !gmod->TryGetNode( code, node ) )
							{
								AddError(
									errorBuilder,
									LocalIdParsingState::PrimaryItem,
									"Invalid start GmodNode in Primary item: " + code );
							}

							primaryItemStart = i;
							AdvanceParser( i, segment );
						}
						else
						{
							bool isSecSegment = segment == "sec";
							bool isMetaSegment = segment == "meta";
							bool isTildeSegment = !segment.empty() && segment[0] == '~';

							LocalIdParsingState nextState = state;
							if ( isSecSegment )
								nextState = LocalIdParsingState::SecondaryItem;
							else if ( isMetaSegment )
								nextState = LocalIdParsingState::MetaQuantity;
							else if ( isTildeSegment )
								nextState = LocalIdParsingState::ItemDescription;

							if ( nextState != state )
							{
								std::string path = localIdStr.substr( primaryItemStart, i - 1 - primaryItemStart );
								if ( !gmod->TryParsePath( path, primaryItem ) )
								{
									AddError(
										errorBuilder,
										LocalIdParsingState::PrimaryItem,
										"Invalid GmodPath in Primary item: " + path );

									auto [nextStateIndex, endOfNextStateIndex] = GetNextStateIndexes( localIdStr, state );
									i = endOfNextStateIndex;
									AdvanceState( state, nextState );
									break;
								}

								if ( isTildeSegment )
									AdvanceState( state, nextState );
								else
									AdvanceParser( i, segment, state, nextState );
								break;
							}

							GmodNode node;
							if ( !gmod->TryGetNode( code, node ) )
							{
								AddError(
									errorBuilder,
									LocalIdParsingState::PrimaryItem,
									"Invalid GmodNode in Primary item: " + code );

								auto [nextStateIndex, endOfNextStateIndex] = GetNextStateIndexes( localIdStr, state );

								if ( nextStateIndex == -1 )
								{
									AddError(
										errorBuilder,
										LocalIdParsingState::PrimaryItem,
										"Invalid or missing '/meta' prefix after Primary item" );
									return false;
								}

								std::string nextSegment;
								if ( nextStateIndex + 1 < localIdStr.length() )
								{
									size_t nextSegSlash = localIdStr.find( '/', nextStateIndex + 1 );
									if ( nextSegSlash == std::string::npos )
										nextSegment = localIdStr.substr( nextStateIndex + 1 );
									else
										nextSegment = localIdStr.substr( nextStateIndex + 1, nextSegSlash - ( nextStateIndex + 1 ) );
								}

								bool isNextSecSegment = nextSegment == "sec";
								bool isNextMetaSegment = nextSegment == "meta";
								bool isNextTildeSegment = !nextSegment.empty() && nextSegment[0] == '~';

								if ( isNextSecSegment )
									nextState = LocalIdParsingState::SecondaryItem;
								else if ( isNextMetaSegment )
									nextState = LocalIdParsingState::MetaQuantity;
								else if ( isNextTildeSegment )
									nextState = LocalIdParsingState::ItemDescription;
								else
									throw std::runtime_error( "Inconsistent parsing state" );

								std::string invalidPrimaryItemPath = localIdStr.substr( i, nextStateIndex - i );

								AddError(
									errorBuilder,
									LocalIdParsingState::PrimaryItem,
									"Invalid GmodPath: Last part in Primary item: " + invalidPrimaryItemPath );

								i = endOfNextStateIndex;
								AdvanceState( state, nextState );
								break;
							}

							AdvanceParser( i, segment );
						}
						break;
					}

					case LocalIdParsingState::SecondaryItem:
					{
						if ( segment.empty() )
						{
							state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
							break;
						}

						size_t dashIndex = segment.find( '-' );
						std::string code;
						if ( dashIndex == std::string::npos )
						{
							code = segment;
						}
						else
						{
							code = segment.substr( 0, dashIndex );
						}

						if ( !gmod )
							return false;

						if ( secondaryItemStart == -1 )
						{
							GmodNode node;
							if ( !gmod->TryGetNode( code, node ) )
							{
								AddError(
									errorBuilder,
									LocalIdParsingState::SecondaryItem,
									"Invalid start GmodNode in Secondary item: " + code );
							}

							secondaryItemStart = i;
							AdvanceParser( i, segment );
						}
						else
						{
							bool isMetaSegment = segment == "meta";
							bool isTildeSegment = !segment.empty() && segment[0] == '~';

							LocalIdParsingState nextState = state;
							if ( isMetaSegment )
								nextState = LocalIdParsingState::MetaQuantity;
							else if ( isTildeSegment )
								nextState = LocalIdParsingState::ItemDescription;

							if ( nextState != state )
							{
								std::string path = localIdStr.substr( secondaryItemStart, i - 1 - secondaryItemStart );
								if ( !gmod->TryParsePath( path, secondaryItem ) )
								{
									invalidSecondaryItem = true;
									AddError(
										errorBuilder,
										LocalIdParsingState::SecondaryItem,
										"Invalid GmodPath in Secondary item: " + path );

									auto [nextStateIndex, endOfNextStateIndex] = GetNextStateIndexes( localIdStr, state );
									i = endOfNextStateIndex;
									AdvanceState( state, nextState );
									break;
								}

								if ( isTildeSegment )
									AdvanceState( state, nextState );
								else
									AdvanceParser( i, segment, state, nextState );
								break;
							}

							GmodNode node;
							if ( !gmod->TryGetNode( code, node ) )
							{
								invalidSecondaryItem = true;
								AddError(
									errorBuilder,
									LocalIdParsingState::SecondaryItem,
									"Invalid GmodNode in Secondary item: " + code );

								auto [nextStateIndex, endOfNextStateIndex] = GetNextStateIndexes( localIdStr, state );
								if ( nextStateIndex == -1 )
								{
									AddError(
										errorBuilder,
										LocalIdParsingState::SecondaryItem,
										"Invalid or missing '/meta' prefix after Secondary item" );
									return false;
								}

								std::string nextSegment;
								if ( nextStateIndex + 1 < localIdStr.length() )
								{
									size_t nextSegSlash = localIdStr.find( '/', nextStateIndex + 1 );
									if ( nextSegSlash == std::string::npos )
										nextSegment = localIdStr.substr( nextStateIndex + 1 );
									else
										nextSegment = localIdStr.substr( nextStateIndex + 1, nextSegSlash - ( nextStateIndex + 1 ) );
								}

								bool isNextMetaSegment = nextSegment == "meta";
								bool isNextTildeSegment = !nextSegment.empty() && nextSegment[0] == '~';

								if ( isNextMetaSegment )
									nextState = LocalIdParsingState::MetaQuantity;
								else if ( isNextTildeSegment )
									nextState = LocalIdParsingState::ItemDescription;
								else
									throw std::runtime_error( "Inconsistent parsing state" );

								std::string invalidSecondaryItemPath = localIdStr.substr( i, nextStateIndex - i );

								AddError(
									errorBuilder,
									LocalIdParsingState::SecondaryItem,
									"Invalid GmodPath: Last part in Secondary item: " + invalidSecondaryItemPath );

								i = endOfNextStateIndex;
								AdvanceState( state, nextState );
								break;
							}

							AdvanceParser( i, segment );
						}
						break;
					}
					case LocalIdParsingState::ItemDescription:
					{
						if ( segment.empty() )
						{
							state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
							break;
						}

						verbose = true;

						size_t metaIndex = localIdStr.find( "/meta" );
						if ( metaIndex == std::string::npos )
						{
							AddError( errorBuilder, LocalIdParsingState::ItemDescription, predefinedMessage );
							return false;
						}

						segment = localIdStr.substr( i, ( metaIndex + 5 ) - i );

						AdvanceParser( i, segment, state );
						break;
					}
					case LocalIdParsingState::MetaQuantity:
					{
						if ( segment.empty() )
						{
							state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
							break;
						}

						bool result = ParseMetatag(
							CodebookName::Quantity,
							state,
							i,
							segment,
							qty,
							codebooks,
							errorBuilder );
						if ( !result )
							return false;
						break;
					}
					case LocalIdParsingState::MetaContent:
					{
						break;
					}
					case LocalIdParsingState::MetaCalculation:
					{
						break;
					}
					case LocalIdParsingState::MetaState:
					{
						break;
					}
					case LocalIdParsingState::MetaCommand:
					{
						break;
					}
					case LocalIdParsingState::MetaType:
					{
						break;
					}
					case LocalIdParsingState::MetaPosition:
					{
						break;
					}
					case LocalIdParsingState::MetaDetail:
					{
						break;
					}
					default:
					{
						AdvanceParser( i, segment, state );
						break;
					}
				}

				if ( nextSlash == std::string::npos )
					break;
			}

			localIdBuilder = Create( visVersion )
								 .TryWithPrimaryItem( primaryItem )
								 .TryWithSecondaryItem( secondaryItem )
								 .WithVerboseMode( verbose )
								 .TryWithMetadataTag( qty )
								 .TryWithMetadataTag( cnt )
								 .TryWithMetadataTag( calc )
								 .TryWithMetadataTag( stateTag )
								 .TryWithMetadataTag( cmd )
								 .TryWithMetadataTag( type )
								 .TryWithMetadataTag( pos )
								 .TryWithMetadataTag( detail );

			if ( localIdBuilder->IsEmptyMetadata() )
			{
				AddError(
					errorBuilder,
					LocalIdParsingState::Completeness,
					"No metadata tags specified. Local IDs require at least 1 metadata tag." );
			}

			return ( !errorBuilder.HasError() && !invalidSecondaryItem );
		}
	}
