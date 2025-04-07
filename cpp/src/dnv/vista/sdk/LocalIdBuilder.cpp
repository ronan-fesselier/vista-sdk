#include "pch.h"

#include "dnv/vista/sdk/LocalIdBuilder.h"
#include "dnv/vista/sdk/VisVersion.h"
#include "dnv/vista/sdk/LocalId.h"
#include "dnv/vista/sdk/CodebookName.h"
#include "dnv/vista/sdk/ParsingErrors.h"
#include "dnv/vista/sdk/MetadataTag.h"
#include "dnv/vista/sdk/GmodPath.h"
#include "dnv/vista/sdk/CodeBooks.h"
#include "dnv/vista/sdk/VIS.h"

namespace dnv::vista::sdk
{
	//-------------------------------------------------------------------------
	// Constants
	//-------------------------------------------------------------------------

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

	//-------------------------------------------------------------------------
	// Constructors and Static Factories
	//-------------------------------------------------------------------------

	LocalIdBuilder::LocalIdBuilder()
	{
		SPDLOG_INFO( "Created empty LocalIdBuilder" );
	}

	LocalIdBuilder::LocalIdBuilder( const LocalIdBuilder& other )
		: m_visVersion( other.m_visVersion ), m_verboseMode( other.m_verboseMode ), m_items( other.m_items ), m_quantity( other.m_quantity ), m_content( other.m_content ), m_calculation( other.m_calculation ), m_state( other.m_state ), m_command( other.m_command ), m_type( other.m_type ), m_position( other.m_position ), m_detail( other.m_detail )
	{
		SPDLOG_INFO( "Created LocalIdBuilder via copy constructor" );
	}

	LocalIdBuilder LocalIdBuilder::create( VisVersion version )
	{
		SPDLOG_INFO( "Creating LocalIdBuilder with VisVersion: {}", static_cast<int>( version ) );
		LocalIdBuilder builder;
		builder.m_visVersion = version;
		return builder;
	}

	//-------------------------------------------------------------------------
	// Core Interface Implementation (ILocalIdBuilder)
	//-------------------------------------------------------------------------

	LocalId LocalIdBuilder::build() const
	{
		try
		{
			SPDLOG_INFO( "Building LocalId from builder" );
			return LocalId( *this );
		}
		catch ( const std::exception& ex )
		{
			SPDLOG_ERROR( "Failed to build LocalId: {}", ex.what() );
			throw;
		}
	}

	std::optional<VisVersion> LocalIdBuilder::getVisVersion() const
	{
		return m_visVersion;
	}

	bool LocalIdBuilder::getVerboseMode() const
	{
		return m_verboseMode;
	}

	bool LocalIdBuilder::hasCustomTag() const
	{
		return ( m_quantity.has_value() && m_quantity->isCustom() ) ||
			   ( m_calculation.has_value() && m_calculation->isCustom() ) ||
			   ( m_content.has_value() && m_content->isCustom() ) ||
			   ( m_position.has_value() && m_position->isCustom() ) ||
			   ( m_state.has_value() && m_state->isCustom() ) ||
			   ( m_command.has_value() && m_command->isCustom() ) ||
			   ( m_type.has_value() && m_type->isCustom() ) ||
			   ( m_detail.has_value() && m_detail->isCustom() );
	}

	bool LocalIdBuilder::isValid() const
	{
		if ( !m_visVersion.has_value() )
			return false;

		if ( !m_items.getPrimaryItem().has_value() )
			return false;

		return !isEmptyMetadata();
	}

	bool LocalIdBuilder::isEmpty() const
	{
		return !m_visVersion.has_value() &&
			   !m_items.getPrimaryItem().has_value() &&
			   !m_items.getSecondaryItem().has_value() &&
			   isEmptyMetadata();
	}

	std::string LocalIdBuilder::toString() const
	{
		std::stringstream stream;
		toString( stream );
		return stream.str();
	}

	const LocalIdItems& LocalIdBuilder::getItems() const
	{
		return m_items;
	}

	const std::optional<GmodPath>& LocalIdBuilder::getPrimaryItem() const
	{
		return m_items.getPrimaryItem();
	}

	const std::optional<GmodPath>& LocalIdBuilder::getSecondaryItem() const
	{
		return m_items.getSecondaryItem();
	}

	const std::optional<MetadataTag>& LocalIdBuilder::getQuantity() const
	{
		return m_quantity;
	}

	const std::optional<MetadataTag>& LocalIdBuilder::getContent() const
	{
		return m_content;
	}

	const std::optional<MetadataTag>& LocalIdBuilder::getCalculation() const
	{
		return m_calculation;
	}

	const std::optional<MetadataTag>& LocalIdBuilder::getState() const
	{
		return m_state;
	}

	const std::optional<MetadataTag>& LocalIdBuilder::getCommand() const
	{
		return m_command;
	}

	const std::optional<MetadataTag>& LocalIdBuilder::getType() const
	{
		return m_type;
	}

	const std::optional<MetadataTag>& LocalIdBuilder::getPosition() const
	{
		return m_position;
	}

	const std::optional<MetadataTag>& LocalIdBuilder::getDetail() const
	{
		return m_detail;
	}

	bool LocalIdBuilder::isEmptyMetadata() const
	{
		return !m_quantity.has_value() &&
			   !m_content.has_value() &&
			   !m_calculation.has_value() &&
			   !m_state.has_value() &&
			   !m_command.has_value() &&
			   !m_type.has_value() &&
			   !m_position.has_value() &&
			   !m_detail.has_value();
	}

	const std::vector<MetadataTag> LocalIdBuilder::getMetadataTags() const
	{
		static thread_local std::vector<MetadataTag> tags;
		tags.clear();

		if ( m_quantity.has_value() )
			tags.push_back( *m_quantity );

		if ( m_content.has_value() )
			tags.push_back( *m_content );

		if ( m_calculation.has_value() )
			tags.push_back( *m_calculation );

		if ( m_state.has_value() )
			tags.push_back( *m_state );

		if ( m_command.has_value() )
			tags.push_back( *m_command );

		if ( m_type.has_value() )
			tags.push_back( *m_type );

		if ( m_position.has_value() )
			tags.push_back( *m_position );

		if ( m_detail.has_value() )
			tags.push_back( *m_detail );

		return tags;
	}

	void LocalIdBuilder::toString( std::stringstream& builder ) const
	{
		SPDLOG_INFO( "Converting LocalIdBuilder to string" );

		if ( !m_visVersion.has_value() )
		{
			SPDLOG_ERROR( "Cannot convert LocalIdBuilder to string: missing VisVersion" );
			return;
		}

		builder << NamingRule << "/v" << static_cast<int>( *m_visVersion ) << '/';
		m_items.append( builder, m_verboseMode );

		if ( !isEmptyMetadata() )
		{
			builder << "meta/";

			if ( m_quantity.has_value() )
			{
				builder << "q/";
				builder << m_quantity->value() << '/';
			}

			if ( m_content.has_value() )
			{
				builder << "c/";
				builder << m_content->value() << '/';
			}

			if ( m_calculation.has_value() )
			{
				builder << "cal/";
				builder << m_calculation->value() << '/';
			}

			if ( m_state.has_value() )
			{
				builder << "s/";
				builder << m_state->value() << '/';
			}

			if ( m_command.has_value() )
			{
				builder << "cmd/";
				builder << m_command->value() << '/';
			}

			if ( m_type.has_value() )
			{
				builder << "t/";
				builder << m_type->value() << '/';
			}

			if ( m_position.has_value() )
			{
				builder << "pos/";
				builder << m_position->value() << '/';
			}

			if ( m_detail.has_value() )
			{
				builder << "d/";
				builder << m_detail->value() << '/';
			}
		}
	}

	//-------------------------------------------------------------------------
	// Builder Methods - Core Properties
	//-------------------------------------------------------------------------

	LocalIdBuilder LocalIdBuilder::withVisVersion( const std::string& visVersion )
	{
		try
		{
			SPDLOG_INFO( "Setting VisVersion from string: {}", visVersion );

			VisVersion version = VisVersionExtensions::Parse( visVersion );

			return withVisVersion( version );
		}
		catch ( const std::exception& ex )
		{
			SPDLOG_ERROR( "Invalid VisVersion string '{}': {}", visVersion, ex.what() );
			throw std::invalid_argument( "Invalid VisVersion: " + visVersion );
		}
	}

	LocalIdBuilder LocalIdBuilder::withVisVersion( VisVersion version )
	{
		SPDLOG_INFO( "Setting VisVersion: {}", static_cast<int>( version ) );
		LocalIdBuilder result( *this );
		result.m_visVersion = version;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::tryWithVisVersion( const std::optional<VisVersion>& version )
	{
		if ( !version.has_value() )
		{
			SPDLOG_INFO( "Skipping VisVersion setting: value is empty" );
			return *this;
		}

		return withVisVersion( *version );
	}

	LocalIdBuilder LocalIdBuilder::tryWithVisVersion( const std::optional<std::string>& visVersionStr, bool& succeeded )
	{
		succeeded = false;

		if ( !visVersionStr.has_value() || visVersionStr->empty() )
		{
			SPDLOG_INFO( "Skipping VisVersion setting: string is empty" );
			return *this;
		}

		try
		{
			auto result = withVisVersion( *visVersionStr );
			succeeded = true;
			return result;
		}
		catch ( const std::exception& ex )
		{
			SPDLOG_ERROR( "Failed to set VisVersion: {}", ex.what() );
			return *this;
		}
	}

	LocalIdBuilder LocalIdBuilder::withoutVisVersion()
	{
		SPDLOG_INFO( "Removing VisVersion" );
		LocalIdBuilder result( *this );
		result.m_visVersion = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withVerboseMode( bool verboseMode )
	{
		SPDLOG_INFO( "Setting verbose mode: {}", verboseMode );
		LocalIdBuilder result( *this );
		result.m_verboseMode = verboseMode;
		return result;
	}

	//-------------------------------------------------------------------------
	// Builder Methods - Items
	//-------------------------------------------------------------------------

	LocalIdBuilder LocalIdBuilder::withPrimaryItem( const GmodPath& item )
	{
		SPDLOG_INFO( "Setting primary item: {}", item.ToString() );
		LocalIdBuilder result( *this );
		result.m_items = LocalIdItems( item, m_items.getSecondaryItem() );
		return result;
	}

	LocalIdBuilder LocalIdBuilder::tryWithPrimaryItem( const std::optional<GmodPath>& item )
	{
		if ( !item.has_value() )
		{
			SPDLOG_INFO( "Skipping primary item: value is empty" );
			return *this;
		}

		return withPrimaryItem( *item );
	}

	LocalIdBuilder LocalIdBuilder::tryWithPrimaryItem( const std::optional<GmodPath>& item, bool& succeeded )
	{
		succeeded = false;

		if ( !item.has_value() )
		{
			SPDLOG_INFO( "Skipping primary item: value is empty" );
			return *this;
		}

		try
		{
			auto result = withPrimaryItem( *item );
			succeeded = true;
			return result;
		}
		catch ( const std::exception& ex )
		{
			SPDLOG_ERROR( "Failed to set primary item: {}", ex.what() );
			return *this;
		}
	}

	LocalIdBuilder LocalIdBuilder::withoutPrimaryItem()
	{
		SPDLOG_INFO( "Removing primary item" );
		LocalIdBuilder result( *this );
		result.m_items = LocalIdItems( std::nullopt, m_items.getSecondaryItem() );
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withSecondaryItem( const GmodPath& item )
	{
		SPDLOG_INFO( "Setting secondary item: {}", item.ToString() );
		LocalIdBuilder result( *this );
		result.m_items = LocalIdItems( m_items.getPrimaryItem(), item );
		return result;
	}

	LocalIdBuilder LocalIdBuilder::tryWithSecondaryItem( const std::optional<GmodPath>& item )
	{
		if ( !item.has_value() )
		{
			SPDLOG_INFO( "Skipping secondary item: value is empty" );
			return *this;
		}

		return withSecondaryItem( *item );
	}

	LocalIdBuilder LocalIdBuilder::tryWithSecondaryItem( const std::optional<GmodPath>& item, bool& succeeded )
	{
		succeeded = false;

		if ( !item.has_value() )
		{
			SPDLOG_INFO( "Skipping secondary item: value is empty" );
			return *this;
		}

		try
		{
			auto result = withSecondaryItem( *item );
			succeeded = true;
			return result;
		}
		catch ( const std::exception& ex )
		{
			SPDLOG_ERROR( "Failed to set secondary item: {}", ex.what() );
			return *this;
		}
	}

	LocalIdBuilder LocalIdBuilder::withoutSecondaryItem()
	{
		SPDLOG_INFO( "Removing secondary item" );
		LocalIdBuilder result( *this );
		result.m_items = LocalIdItems( m_items.getPrimaryItem(), std::nullopt );
		return result;
	}

	//-------------------------------------------------------------------------
	// Builder Methods - General Metadata Tags
	//-------------------------------------------------------------------------

	LocalIdBuilder LocalIdBuilder::withMetadataTag( const MetadataTag& metadataTag )
	{
		SPDLOG_INFO( "Adding metadata tag: {} ({})",
			static_cast<int>( metadataTag.name() ),
			metadataTag.value() );

		switch ( metadataTag.name() )
		{
			case CodebookName::Quantity:
				return withQuantity( metadataTag );
			case CodebookName::Content:
				return withContent( metadataTag );
			case CodebookName::Calculation:
				return withCalculation( metadataTag );
			case CodebookName::State:
				return withState( metadataTag );
			case CodebookName::Command:
				return withCommand( metadataTag );
			case CodebookName::Type:
				return withType( metadataTag );
			case CodebookName::Position:
				return withPosition( metadataTag );
			case CodebookName::Detail:
				return withDetail( metadataTag );
			default:
				SPDLOG_ERROR( "Unsupported codebook for metadata tag: {}",
					static_cast<int>( metadataTag.name() ) );
				throw std::invalid_argument( "Unsupported codebook for metadata tag" );
		}
	}

	LocalIdBuilder LocalIdBuilder::tryWithMetadataTag( const std::optional<MetadataTag>& metadataTag )
	{
		if ( !metadataTag.has_value() )
		{
			SPDLOG_INFO( "Skipping metadata tag: value is empty" );
			return *this;
		}

		return withMetadataTag( *metadataTag );
	}

	LocalIdBuilder LocalIdBuilder::tryWithMetadataTag( const std::optional<MetadataTag>& metadataTag, bool& succeeded )
	{
		succeeded = false;

		if ( !metadataTag.has_value() )
		{
			SPDLOG_INFO( "Skipping metadata tag: value is empty" );
			return *this;
		}

		try
		{
			auto result = withMetadataTag( *metadataTag );
			succeeded = true;
			return result;
		}
		catch ( const std::exception& ex )
		{
			SPDLOG_ERROR( "Failed to set metadata tag: {}", ex.what() );
			return *this;
		}
	}

	LocalIdBuilder LocalIdBuilder::withoutMetadataTag( CodebookName name )
	{
		SPDLOG_INFO( "Removing metadata tag: {}", static_cast<int>( name ) );

		switch ( name )
		{
			case CodebookName::Quantity:
				return withoutQuantity();
			case CodebookName::Content:
				return withoutContent();
			case CodebookName::Calculation:
				return withoutCalculation();
			case CodebookName::State:
				return withoutState();
			case CodebookName::Command:
				return withoutCommand();
			case CodebookName::Type:
				return withoutType();
			case CodebookName::Position:
				return withoutPosition();
			case CodebookName::Detail:
				return withoutDetail();
			default:
				SPDLOG_ERROR( "Unsupported codebook for metadata tag removal: {}",
					static_cast<int>( name ) );
				throw std::invalid_argument( "Unsupported codebook for metadata tag removal" );
		}
	}

	//-------------------------------------------------------------------------
	// Builder Methods - Specific Metadata Tags
	//-------------------------------------------------------------------------

	LocalIdBuilder LocalIdBuilder::withQuantity( const MetadataTag& quantity )
	{
		SPDLOG_INFO( "Setting quantity tag: {}", quantity.value() );
		LocalIdBuilder result( *this );
		result.m_quantity = quantity;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withContent( const MetadataTag& content )
	{
		SPDLOG_INFO( "Setting content tag: {}", content.value() );
		LocalIdBuilder result( *this );
		result.m_content = content;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withCalculation( const MetadataTag& calculation )
	{
		SPDLOG_INFO( "Setting calculation tag: {}", calculation.value() );
		LocalIdBuilder result( *this );
		result.m_calculation = calculation;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withState( const MetadataTag& state )
	{
		SPDLOG_INFO( "Setting state tag: {}", state.value() );
		LocalIdBuilder result( *this );
		result.m_state = state;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withCommand( const MetadataTag& command )
	{
		SPDLOG_INFO( "Setting command tag: {}", command.value() );
		LocalIdBuilder result( *this );
		result.m_command = command;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withType( const MetadataTag& type )
	{
		SPDLOG_INFO( "Setting type tag: {}", type.value() );
		LocalIdBuilder result( *this );
		result.m_type = type;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withPosition( const MetadataTag& position )
	{
		SPDLOG_INFO( "Setting position tag: {}", position.value() );
		LocalIdBuilder result( *this );
		result.m_position = position;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withDetail( const MetadataTag& detail )
	{
		SPDLOG_INFO( "Setting detail tag: {}", detail.value() );
		LocalIdBuilder result( *this );
		result.m_detail = detail;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::parse( const std::string& localIdStr )
	{
		SPDLOG_INFO( "Parsing LocalId from string: {}", localIdStr );

		ParsingErrors errors;
		std::optional<LocalIdBuilder> result;
		if ( !tryParse( localIdStr, errors, result ) || !result.has_value() )
		{
			SPDLOG_ERROR( "Failed to parse LocalId: {}", localIdStr );
			std::string errorMsg = "Failed to parse LocalId: " + localIdStr;
			if ( !errors.isEmpty() )
			{
				errorMsg += "\nErrors: ";
				for ( const auto& err : errors )
				{
					errorMsg += "\n  - " + std::get<0>( err ) + ": " + std::get<1>( err );
				}
			}
			throw std::invalid_argument( errorMsg );
		}

		return *result;
	}

	bool LocalIdBuilder::tryParse( const std::string& localIdStr, ParsingErrors& errors, std::optional<LocalIdBuilder>& localId )
	{
		SPDLOG_INFO( "Attempting to parse LocalId: {}", localIdStr );

		LocalIdParsingErrorBuilder errorBuilder = LocalIdParsingErrorBuilder::create();

		bool result = tryParseInternal( localIdStr, errorBuilder, localId );

		if ( errorBuilder.hasError() )
		{
			SPDLOG_ERROR( "Parsing encountered errors" );
			errors = errorBuilder.build();
		}

		return result && localId.has_value();
	}

	/* HELPERS   TODO  TODO TODO */
	void advanceParser( size_t& i, const std::string& segment, LocalIdParsingState& state )
	{
		state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
		i += segment.length() + 1;
	}

	void advanceParser( size_t& i, const std::string& segment )
	{
		i += segment.length() + 1;
	}

	void advanceParser( LocalIdParsingState& state, LocalIdParsingState to )
	{
		state = to;
	}

	void advanceParser( size_t& i, const std::string& segment,
		LocalIdParsingState& state, LocalIdParsingState to )
	{
		i += segment.length() + 1;
		state = to;
	}

	void addError( LocalIdParsingErrorBuilder& errorBuilder,
		LocalIdParsingState state, const std::string& message = "" )
	{
		errorBuilder.addError( state, message );
	}

	std::pair<size_t, size_t> getNextStateIndexes(
		const std::string& str, LocalIdParsingState state )
	{
		size_t customIndex = str.find( "~" );
		size_t endOfCustomIndex = ( customIndex + 1 + 1 );

		size_t metaIndex = str.find( "/meta" );
		size_t endOfMetaIndex = ( metaIndex + 5 + 1 );
		bool isVerbose = customIndex < metaIndex;

		switch ( state )
		{
			case LocalIdParsingState::PrimaryItem:
			{
				size_t secIndex = str.find( "/sec" );
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
	/* END HELPERS   TODO  TODO TODO */

	bool LocalIdBuilder::tryParseInternal( const std::string& localIdStr, LocalIdParsingErrorBuilder& errorBuilder, std::optional<LocalIdBuilder>& localIdBuilder )
	{
		SPDLOG_INFO( "Parsing LocalId internal: {}", localIdStr );
		localIdBuilder = std::nullopt;

		if ( localIdStr.empty() )
		{
			SPDLOG_INFO( "LocalId string is empty" );
			errorBuilder.addError( LocalIdParsingState::EmptyState );
			return false;
		}

		if ( localIdStr[0] != '/' )
		{
			SPDLOG_ERROR( "Invalid format: missing '/' as first character" );
			errorBuilder.addError( LocalIdParsingState::Formatting, "Invalid format: missing '/' as first character" );
			return false;
		}

		std::optional<VisVersion> visVersion;
		std::shared_ptr<Gmod> gmod = nullptr;
		std::shared_ptr<Codebooks> codebooks = nullptr;
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
		bool invalidSecondaryItem = false;
		std::string predefinedMessage;

		size_t primaryItemStart = -1;
		size_t secondaryItemStart = -1;

		LocalIdParsingState state = LocalIdParsingState::NamingRule;
		size_t i = 1;

		while ( state <= LocalIdParsingState::MetaDetail )
		{
			size_t nextStart = std::min( localIdStr.size(), i );
			size_t nextSlash = localIdStr.find( '/', nextStart );
			std::string segment;

			if ( nextSlash == std::string::npos )
				segment = localIdStr.substr( nextStart );
			else
				segment = localIdStr.substr( nextStart, nextSlash - nextStart );

			SPDLOG_INFO( "Processing segment: '{}', state: {}", segment, static_cast<int>( state ) );

			switch ( state )
			{
				case LocalIdParsingState::NamingRule:
					if ( segment.empty() )
					{
						addError( errorBuilder, state, predefinedMessage );
						state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						break;
					}

					if ( segment != NamingRule )
					{
						SPDLOG_ERROR( "Invalid naming rule: {}", segment );
						addError( errorBuilder, state, predefinedMessage );
						return false;
					}

					advanceParser( i, segment, state );
					break;

				case LocalIdParsingState::VisVersion:
					if ( segment.empty() )
					{
						addError( errorBuilder, state, predefinedMessage );
						state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						break;
					}

					if ( segment.substr( 0, 4 ) != "vis-" )
					{
						SPDLOG_ERROR( "Invalid VisVersion format: {}", segment );
						addError( errorBuilder, state, predefinedMessage );
						return false;
					}

					try
					{
						visVersion = VisVersionExtensions::Parse( segment.substr( 4 ) );
					}
					catch ( const std::exception& e )
					{
						SPDLOG_ERROR( "Failed to parse VisVersion: {}", e.what() );
						addError( errorBuilder, state, predefinedMessage );
						return false;
					}

					gmod = std::make_shared<Gmod>( VIS::instance().gmod( *visVersion ) );
					codebooks = std::make_shared<Codebooks>( VIS::instance().codebooks( *visVersion ) );

					if ( !gmod || !codebooks )
					{
						SPDLOG_ERROR( "Failed to get Gmod or Codebooks for version {}", static_cast<int>( *visVersion ) );
						return false;
					}

					advanceParser( i, segment, state );
					break;

				case LocalIdParsingState::PrimaryItem:
				{
					if ( segment.empty() )
					{
						if ( primaryItemStart != static_cast<size_t>( -1 ) )
						{
							if ( !gmod )
								return false;

							std::string path = localIdStr.substr( primaryItemStart, i - 1 - primaryItemStart );
							if ( !gmod->TryParsePath( path, primaryItem ) )
							{
								SPDLOG_ERROR( "Invalid GmodPath in Primary item: {}", path );
								addError( errorBuilder, state, "Invalid GmodPath in Primary item: " + path );
							}
						}
						else
						{
							addError( errorBuilder, state, predefinedMessage );
						}

						addError( errorBuilder, state, "Invalid or missing '/meta' prefix after Primary item" );
						state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						break;
					}

					size_t dashIndex = segment.find( '-' );
					std::string code = ( dashIndex == std::string::npos ) ? segment : segment.substr( 0, dashIndex );

					if ( !gmod )
					{
						SPDLOG_ERROR( "Gmod is not initialized" );
						return false;
					}

					if ( primaryItemStart == static_cast<size_t>( -1 ) )
					{
						GmodNode tempNode;
						if ( !gmod->tryGetNode( code, tempNode ) )
						{
							SPDLOG_ERROR( "Invalid start GmodNode in Primary item: {}", code );
							addError( errorBuilder, state, "Invalid start GmodNode in Primary item: " + code );
						}

						primaryItemStart = i;
						advanceParser( i, segment );
					}
					else
					{
						bool isSecPrefix = segment == "sec";
						bool isMetaPrefix = segment == "meta";
						bool isTildePrefix = !segment.empty() && segment[0] == '~';

						LocalIdParsingState nextState = state;

						if ( isSecPrefix )
							nextState = LocalIdParsingState::SecondaryItem;
						else if ( isMetaPrefix )
							nextState = LocalIdParsingState::MetaQuantity;
						else if ( isTildePrefix )
							nextState = LocalIdParsingState::ItemDescription;

						if ( nextState != state )
						{
							std::string path = localIdStr.substr( primaryItemStart, i - 1 - primaryItemStart );
							if ( !gmod->TryParsePath( path, primaryItem ) )
							{
								SPDLOG_ERROR( "Invalid GmodPath in Primary item: {}", path );
								addError( errorBuilder, state, "Invalid GmodPath in Primary item: " + path );

								auto [_, endOfNextStateIndex] = getNextStateIndexes( localIdStr, state );
								i = endOfNextStateIndex;
								advanceParser( state, nextState );
								break;
							}

							if ( isTildePrefix )
								advanceParser( state, nextState );
							else
								advanceParser( i, segment, state, nextState );

							break;
						}

						GmodNode tempNode2;
						if ( !gmod->tryGetNode( code, tempNode2 ) )
						{
							SPDLOG_ERROR( "Invalid GmodNode in Primary item: {}", code );
							addError( errorBuilder, state, "Invalid GmodNode in Primary item: " + code );

							auto [nextStateIndex, endOfNextStateIndex] = getNextStateIndexes( localIdStr, state );

							if ( nextStateIndex == static_cast<size_t>( -1 ) )
							{
								addError( errorBuilder, state, "Invalid or missing '/meta' prefix after Primary item" );
								return false;
							}

							std::string nextSegment = localIdStr.substr( nextStateIndex + 1 );

							bool nextIsSecPrefix = nextSegment.substr( 0, 3 ) == "sec";
							bool nextIsMetaPrefix = nextSegment.substr( 0, 4 ) == "meta";
							bool nextIsTildePrefix = !nextSegment.empty() && nextSegment[0] == '~';

							if ( nextIsSecPrefix )
								nextState = LocalIdParsingState::SecondaryItem;
							else if ( nextIsMetaPrefix )
								nextState = LocalIdParsingState::MetaQuantity;
							else if ( nextIsTildePrefix )
								nextState = LocalIdParsingState::ItemDescription;

							std::string invalidPrimaryItemPath = localIdStr.substr( i, nextStateIndex - i );

							addError( errorBuilder, state, "Invalid GmodPath: Last part in Primary item: " + invalidPrimaryItemPath );

							i = endOfNextStateIndex;
							advanceParser( state, nextState );
							break;
						}

						advanceParser( i, segment );
					}
				}
				break;

				case LocalIdParsingState::SecondaryItem:
				{
					if ( segment.empty() )
					{
						if ( secondaryItemStart != static_cast<size_t>( -1 ) )
						{
							if ( !gmod )
								return false;

							std::string path = localIdStr.substr( secondaryItemStart, i - 1 - secondaryItemStart );
							if ( !gmod->TryParsePath( path, secondaryItem ) )
							{
								SPDLOG_ERROR( "Invalid GmodPath in Secondary item: {}", path );
								addError( errorBuilder, state, "Invalid GmodPath in Secondary item: " + path );
								invalidSecondaryItem = true;
							}
						}
						else
						{
							addError( errorBuilder, state, predefinedMessage );
							invalidSecondaryItem = true;
						}

						addError( errorBuilder, state, "Invalid or missing '/meta' prefix after Secondary item" );
						state = LocalIdParsingState::MetaQuantity;
						break;
					}

					size_t dashIndex = segment.find( '-' );
					std::string code = ( dashIndex == std::string::npos ) ? segment : segment.substr( 0, dashIndex );

					if ( !gmod )
					{
						SPDLOG_ERROR( "Gmod is not initialized" );
						return false;
					}

					if ( secondaryItemStart == static_cast<size_t>( -1 ) )
					{
						GmodNode tempNode;
						if ( !gmod->tryGetNode( code, tempNode ) )
						{
							SPDLOG_ERROR( "Invalid start GmodNode in Secondary item: {}", code );
							addError( errorBuilder, state, "Invalid start GmodNode in Secondary item: " + code );
							invalidSecondaryItem = true;
						}

						secondaryItemStart = i;
						advanceParser( i, segment );
					}
					else
					{
						bool isMetaPrefix = segment == "meta";
						bool isTildePrefix = !segment.empty() && segment[0] == '~';

						LocalIdParsingState nextState = state;

						if ( isMetaPrefix )
							nextState = LocalIdParsingState::MetaQuantity;
						else if ( isTildePrefix )
							nextState = LocalIdParsingState::ItemDescription;

						if ( nextState != state )
						{
							std::string path = localIdStr.substr( secondaryItemStart, i - 1 - secondaryItemStart );
							if ( !gmod->TryParsePath( path, secondaryItem ) )
							{
								SPDLOG_ERROR( "Invalid GmodPath in Secondary item: {}", path );
								addError( errorBuilder, state, "Invalid GmodPath in Secondary item: " + path );
								invalidSecondaryItem = true;

								auto [_, endOfNextStateIndex] = getNextStateIndexes( localIdStr, state );
								i = endOfNextStateIndex;
								advanceParser( state, nextState );
								break;
							}

							if ( isTildePrefix )
								advanceParser( state, nextState );
							else
								advanceParser( i, segment, state, nextState );

							break;
						}

						GmodNode tempNode;
						if ( !gmod->tryGetNode( code, tempNode ) )
						{
							SPDLOG_ERROR( "Invalid GmodNode in Secondary item: {}", code );
							addError( errorBuilder, state, "Invalid GmodNode in Secondary item: " + code );
							invalidSecondaryItem = true;

							auto [nextStateIndex, endOfNextStateIndex] = getNextStateIndexes( localIdStr, state );

							if ( nextStateIndex == static_cast<size_t>( -1 ) )
							{
								addError( errorBuilder, state, "Invalid or missing '/meta' prefix after Secondary item" );
								return false;
							}

							i = endOfNextStateIndex;
							advanceParser( state, LocalIdParsingState::MetaQuantity );
							break;
						}

						advanceParser( i, segment );
					}
				}
				break;

				case LocalIdParsingState::ItemDescription:
					verbose = true;
					if ( segment.empty() )
					{
						addError( errorBuilder, state, predefinedMessage );
						state = LocalIdParsingState::MetaQuantity;
						break;
					}

					if ( segment[0] != '~' )
					{
						addError( errorBuilder, state, "Item description must start with ~" );
						state = LocalIdParsingState::MetaQuantity;
						break;
					}

					if ( segment == "~meta" )
					{
						advanceParser( i, segment, state, LocalIdParsingState::MetaQuantity );
					}
					else
					{
						advanceParser( i, segment );
					}
					break;

				case LocalIdParsingState::MetaQuantity:
					if ( segment.empty() )
					{
						addError( errorBuilder, state, predefinedMessage );
						state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						break;
					}

					if ( segment != "meta" && segment != "q" )
					{
						if ( segment == "c" )
						{
							state = LocalIdParsingState::MetaContent;
							break;
						}
						else if ( segment == "cal" )
						{
							state = LocalIdParsingState::MetaCalculation;
							break;
						}
						else if ( segment == "s" )
						{
							state = LocalIdParsingState::MetaState;
							break;
						}
						else if ( segment == "cmd" )
						{
							state = LocalIdParsingState::MetaCommand;
							break;
						}
						else if ( segment == "t" )
						{
							state = LocalIdParsingState::MetaType;
							break;
						}
						else if ( segment == "pos" )
						{
							state = LocalIdParsingState::MetaPosition;
							break;
						}
						else if ( segment == "d" )
						{
							state = LocalIdParsingState::MetaDetail;
							break;
						}
						else
						{
							SPDLOG_ERROR( "Invalid meta prefix: {}", segment );
							addError( errorBuilder, state, "Invalid meta prefix: " + segment );
							state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						}
						break;
					}

					if ( segment == "meta" )
					{
						advanceParser( i, segment );
						break;
					}

					advanceParser( i, segment );
					if ( i < localIdStr.size() )
					{
						size_t valueStart = i;
						size_t valueEnd = localIdStr.find( '/', valueStart );
						if ( valueEnd == std::string::npos )
							valueEnd = localIdStr.size();

						std::string value = localIdStr.substr( valueStart, valueEnd - valueStart );
						if ( !value.empty() )
						{
							if ( codebooks )
							{
								try
								{
									std::optional<MetadataTag> tagOpt = codebooks->tryCreateTag( CodebookName::Quantity, value );
									if ( tagOpt.has_value() )
									{
										qty = *tagOpt;
									}
									else
									{
										qty = MetadataTag( CodebookName::Quantity, value );
									}
									SPDLOG_INFO( "Added quantity tag: {}", value );
								}
								catch ( const std::exception& e )
								{
									SPDLOG_ERROR( "Invalid quantity tag: {} - {}", value, e.what() );
									addError( errorBuilder, state, "Invalid quantity tag: " + value );
								}
							}
							i = valueEnd + 1;
						}
						else
						{
							addError( errorBuilder, state, "Empty quantity value" );
							i = valueEnd + 1;
						}
					}
					state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
					break;

				case LocalIdParsingState::MetaContent:
					if ( segment.empty() )
					{
						addError( errorBuilder, state, predefinedMessage );
						state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						break;
					}

					if ( segment != "c" )
					{
						if ( segment == "cal" )
						{
							state = LocalIdParsingState::MetaCalculation;
							break;
						}
						else if ( segment == "s" )
						{
							state = LocalIdParsingState::MetaState;
							break;
						}
						else if ( segment == "cmd" )
						{
							state = LocalIdParsingState::MetaCommand;
							break;
						}
						else if ( segment == "t" )
						{
							state = LocalIdParsingState::MetaType;
							break;
						}
						else if ( segment == "pos" )
						{
							state = LocalIdParsingState::MetaPosition;
							break;
						}
						else if ( segment == "d" )
						{
							state = LocalIdParsingState::MetaDetail;
							break;
						}
						else
						{
							SPDLOG_ERROR( "Invalid meta content prefix: {}", segment );
							addError( errorBuilder, state, "Invalid meta content prefix: " + segment );
							state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						}
						break;
					}

					advanceParser( i, segment );
					if ( i < localIdStr.size() )
					{
						size_t valueStart = i;
						size_t valueEnd = localIdStr.find( '/', valueStart );
						if ( valueEnd == std::string::npos )
							valueEnd = localIdStr.size();

						std::string value = localIdStr.substr( valueStart, valueEnd - valueStart );
						if ( !value.empty() )
						{
							if ( codebooks )
							{
								try
								{
									std::optional<MetadataTag> tagOpt = codebooks->tryCreateTag( CodebookName::Content, value );
									if ( tagOpt.has_value() )
									{
										cnt = *tagOpt;
									}
									else
									{
										cnt = MetadataTag( CodebookName::Content, value );
									}
									SPDLOG_INFO( "Added content tag: {}", value );
								}
								catch ( const std::exception& e )
								{
									SPDLOG_ERROR( "Invalid content tag: {} - {}", value, e.what() );
									addError( errorBuilder, state, "Invalid content tag: " + value );
								}
							}
							i = valueEnd + 1;
						}
						else
						{
							addError( errorBuilder, state, "Empty content value" );
							i = valueEnd + 1;
						}
					}
					state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
					break;

				case LocalIdParsingState::MetaCalculation:
					if ( segment.empty() )
					{
						addError( errorBuilder, state, predefinedMessage );
						state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						break;
					}

					if ( segment != "cal" )
					{
						if ( segment == "s" )
						{
							state = LocalIdParsingState::MetaState;
							break;
						}
						else if ( segment == "cmd" )
						{
							state = LocalIdParsingState::MetaCommand;
							break;
						}
						else if ( segment == "t" )
						{
							state = LocalIdParsingState::MetaType;
							break;
						}
						else if ( segment == "pos" )
						{
							state = LocalIdParsingState::MetaPosition;
							break;
						}
						else if ( segment == "d" )
						{
							state = LocalIdParsingState::MetaDetail;
							break;
						}
						else
						{
							SPDLOG_ERROR( "Invalid meta calculation prefix: {}", segment );
							addError( errorBuilder, state, "Invalid meta calculation prefix: " + segment );
							state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						}
						break;
					}

					advanceParser( i, segment );
					if ( i < localIdStr.size() )
					{
						size_t valueStart = i;
						size_t valueEnd = localIdStr.find( '/', valueStart );
						if ( valueEnd == std::string::npos )
							valueEnd = localIdStr.size();

						std::string value = localIdStr.substr( valueStart, valueEnd - valueStart );
						if ( !value.empty() )
						{
							if ( codebooks )
							{
								try
								{
									std::optional<MetadataTag> tagOpt = codebooks->tryCreateTag( CodebookName::Calculation, value );
									if ( tagOpt.has_value() )
									{
										calc = *tagOpt;
									}
									else
									{
										calc = MetadataTag( CodebookName::Calculation, value );
									}
									SPDLOG_INFO( "Added calculation tag: {}", value );
								}
								catch ( const std::exception& e )
								{
									SPDLOG_ERROR( "Invalid calculation tag: {} - {}", value, e.what() );
									addError( errorBuilder, state, "Invalid calculation tag: " + value );
								}
							}
							i = valueEnd + 1;
						}
						else
						{
							addError( errorBuilder, state, "Empty calculation value" );
							i = valueEnd + 1;
						}
					}
					state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
					break;

				case LocalIdParsingState::MetaState:
					if ( segment.empty() )
					{
						addError( errorBuilder, state, predefinedMessage );
						state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						break;
					}

					if ( segment != "s" )
					{
						if ( segment == "cmd" )
						{
							state = LocalIdParsingState::MetaCommand;
							break;
						}
						else if ( segment == "t" )
						{
							state = LocalIdParsingState::MetaType;
							break;
						}
						else if ( segment == "pos" )
						{
							state = LocalIdParsingState::MetaPosition;
							break;
						}
						else if ( segment == "d" )
						{
							state = LocalIdParsingState::MetaDetail;
							break;
						}
						else
						{
							SPDLOG_ERROR( "Invalid meta state prefix: {}", segment );
							addError( errorBuilder, state, "Invalid meta state prefix: " + segment );
							state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						}
						break;
					}

					advanceParser( i, segment );
					if ( i < localIdStr.size() )
					{
						size_t valueStart = i;
						size_t valueEnd = localIdStr.find( '/', valueStart );
						if ( valueEnd == std::string::npos )
							valueEnd = localIdStr.size();

						std::string value = localIdStr.substr( valueStart, valueEnd - valueStart );
						if ( !value.empty() )
						{
							if ( codebooks )
							{
								try
								{
									std::optional<MetadataTag> tagOpt = codebooks->tryCreateTag( CodebookName::State, value );
									if ( tagOpt.has_value() )
									{
										stateTag = *tagOpt;
									}
									else
									{
										stateTag = MetadataTag( CodebookName::State, value );
									}
									SPDLOG_INFO( "Added state tag: {}", value );
								}
								catch ( const std::exception& e )
								{
									SPDLOG_ERROR( "Invalid state tag: {} - {}", value, e.what() );
									addError( errorBuilder, state, "Invalid state tag: " + value );
								}
							}
							i = valueEnd + 1;
						}
						else
						{
							addError( errorBuilder, state, "Empty state value" );
							i = valueEnd + 1;
						}
					}
					state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
					break;

				case LocalIdParsingState::MetaCommand:
					if ( segment.empty() )
					{
						addError( errorBuilder, state, predefinedMessage );
						state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						break;
					}

					if ( segment != "cmd" )
					{
						if ( segment == "t" )
						{
							state = LocalIdParsingState::MetaType;
							break;
						}
						else if ( segment == "pos" )
						{
							state = LocalIdParsingState::MetaPosition;
							break;
						}
						else if ( segment == "d" )
						{
							state = LocalIdParsingState::MetaDetail;
							break;
						}
						else
						{
							SPDLOG_ERROR( "Invalid meta command prefix: {}", segment );
							addError( errorBuilder, state, "Invalid meta command prefix: " + segment );
							state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						}
						break;
					}

					advanceParser( i, segment );
					if ( i < localIdStr.size() )
					{
						size_t valueStart = i;
						size_t valueEnd = localIdStr.find( '/', valueStart );
						if ( valueEnd == std::string::npos )
							valueEnd = localIdStr.size();

						std::string value = localIdStr.substr( valueStart, valueEnd - valueStart );
						if ( !value.empty() )
						{
							if ( codebooks )
							{
								try
								{
									std::optional<MetadataTag> tagOpt = codebooks->tryCreateTag( CodebookName::Command, value );
									if ( tagOpt.has_value() )
									{
										cmd = *tagOpt;
									}
									else
									{
										cmd = MetadataTag( CodebookName::Command, value );
									}
									SPDLOG_INFO( "Added command tag: {}", value );
								}
								catch ( const std::exception& e )
								{
									SPDLOG_ERROR( "Invalid command tag: {} - {}", value, e.what() );
									addError( errorBuilder, state, "Invalid command tag: " + value );
								}
							}
							i = valueEnd + 1;
						}
						else
						{
							addError( errorBuilder, state, "Empty command value" );
							i = valueEnd + 1;
						}
					}
					state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
					break;

				case LocalIdParsingState::MetaType:
					if ( segment.empty() )
					{
						addError( errorBuilder, state, predefinedMessage );
						state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						break;
					}

					if ( segment != "t" )
					{
						if ( segment == "pos" )
						{
							state = LocalIdParsingState::MetaPosition;
							break;
						}
						else if ( segment == "d" )
						{
							state = LocalIdParsingState::MetaDetail;
							break;
						}
						else
						{
							SPDLOG_ERROR( "Invalid meta type prefix: {}", segment );
							addError( errorBuilder, state, "Invalid meta type prefix: " + segment );
							state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						}
						break;
					}

					advanceParser( i, segment );
					if ( i < localIdStr.size() )
					{
						size_t valueStart = i;
						size_t valueEnd = localIdStr.find( '/', valueStart );
						if ( valueEnd == std::string::npos )
							valueEnd = localIdStr.size();

						std::string value = localIdStr.substr( valueStart, valueEnd - valueStart );
						if ( !value.empty() )
						{
							if ( codebooks )
							{
								try
								{
									std::optional<MetadataTag> tagOpt = codebooks->tryCreateTag( CodebookName::Type, value );
									if ( tagOpt.has_value() )
									{
										type = *tagOpt;
									}
									else
									{
										type = MetadataTag( CodebookName::Type, value );
									}
									SPDLOG_INFO( "Added type tag: {}", value );
								}
								catch ( const std::exception& e )
								{
									SPDLOG_ERROR( "Invalid type tag: {} - {}", value, e.what() );
									addError( errorBuilder, state, "Invalid type tag: " + value );
								}
							}
							i = valueEnd + 1;
						}
						else
						{
							addError( errorBuilder, state, "Empty type value" );
							i = valueEnd + 1;
						}
					}
					state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
					break;

				case LocalIdParsingState::MetaPosition:
					if ( segment.empty() )
					{
						addError( errorBuilder, state, predefinedMessage );
						state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						break;
					}

					if ( segment != "pos" )
					{
						if ( segment == "d" )
						{
							state = LocalIdParsingState::MetaDetail;
							break;
						}
						else
						{
							SPDLOG_ERROR( "Invalid meta position prefix: {}", segment );
							addError( errorBuilder, state, "Invalid meta position prefix: " + segment );
							state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						}
						break;
					}

					advanceParser( i, segment );
					if ( i < localIdStr.size() )
					{
						size_t valueStart = i;
						size_t valueEnd = localIdStr.find( '/', valueStart );
						if ( valueEnd == std::string::npos )
							valueEnd = localIdStr.size();

						std::string value = localIdStr.substr( valueStart, valueEnd - valueStart );
						if ( !value.empty() )
						{
							if ( codebooks )
							{
								try
								{
									std::optional<MetadataTag> tagOpt = codebooks->tryCreateTag( CodebookName::Position, value );
									if ( tagOpt.has_value() )
									{
										pos = *tagOpt;
									}
									else
									{
										pos = MetadataTag( CodebookName::Position, value );
									}
									SPDLOG_INFO( "Added position tag: {}", value );
								}
								catch ( const std::exception& e )
								{
									SPDLOG_ERROR( "Invalid position tag: {} - {}", value, e.what() );
									addError( errorBuilder, state, "Invalid position tag: " + value );
								}
							}
							i = valueEnd + 1;
						}
						else
						{
							addError( errorBuilder, state, "Empty position value" );
							i = valueEnd + 1;
						}
					}
					state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
					break;

				case LocalIdParsingState::MetaDetail:
					if ( segment.empty() )
					{
						addError( errorBuilder, state, predefinedMessage );
						state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						break;
					}

					if ( segment != "d" )
					{
						SPDLOG_ERROR( "Invalid meta detail prefix: {}", segment );
						addError( errorBuilder, state, "Invalid meta detail prefix: " + segment );
						state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
						break;
					}

					advanceParser( i, segment );
					if ( i < localIdStr.size() )
					{
						size_t valueStart = i;
						size_t valueEnd = localIdStr.find( '/', valueStart );
						if ( valueEnd == std::string::npos )
							valueEnd = localIdStr.size();

						std::string value = localIdStr.substr( valueStart, valueEnd - valueStart );
						if ( !value.empty() )
						{
							if ( codebooks )
							{
								try
								{
									std::optional<MetadataTag> tagOpt = codebooks->tryCreateTag( CodebookName::Detail, value );
									if ( tagOpt.has_value() )
									{
										detail = *tagOpt;
									}
									else
									{
										detail = MetadataTag( CodebookName::Detail, value );
									}
									SPDLOG_INFO( "Added detail tag: {}", value );
								}
								catch ( const std::exception& e )
								{
									SPDLOG_ERROR( "Invalid detail tag: {} - {}", value, e.what() );
									addError( errorBuilder, state, "Invalid detail tag: " + value );
								}
							}
							i = valueEnd + 1;
						}
						else
						{
							addError( errorBuilder, state, "Empty detail value" );
							i = valueEnd + 1;
						}
					}
					state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
					break;

				default:
					SPDLOG_ERROR( "Unknown parsing state: {}", static_cast<int>( state ) );
					addError( errorBuilder, LocalIdParsingState::Formatting );
					return false;
			}
		}

		localIdBuilder = create( visVersion.value_or( VisVersion::Unknown ) )
							 .tryWithPrimaryItem( primaryItem )
							 .tryWithSecondaryItem( secondaryItem )
							 .withVerboseMode( verbose )
							 .tryWithMetadataTag( qty )
							 .tryWithMetadataTag( cnt )
							 .tryWithMetadataTag( calc )
							 .tryWithMetadataTag( stateTag )
							 .tryWithMetadataTag( cmd )
							 .tryWithMetadataTag( type )
							 .tryWithMetadataTag( pos )
							 .tryWithMetadataTag( detail );

		if ( localIdBuilder->isEmptyMetadata() )
		{
			addError( errorBuilder, LocalIdParsingState::Completeness,
				"No metadata tags specified. Local IDs require at least 1 metadata tag." );
		}

		return ( !errorBuilder.hasError() && !invalidSecondaryItem );
	}

	bool LocalIdBuilder::operator==( const LocalIdBuilder& other ) const
	{
		SPDLOG_INFO( "Comparing LocalIdBuilders for equality" );

		if ( !m_visVersion.has_value() && !other.m_visVersion.has_value() )
			return true;

		if ( m_visVersion.has_value() != other.m_visVersion.has_value() )
			return false;

		if ( *m_visVersion != *other.m_visVersion )
			throw std::invalid_argument( "Can't compare local IDs from different VIS versions" );

		return m_items == other.m_items &&
			   //   m_verboseMode == other.m_verboseMode &&
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
		SPDLOG_INFO( "Comparing LocalIdBuilders for inequality" );
		return !( *this == other );
	}

	size_t LocalIdBuilder::getHashCode() const
	{
		SPDLOG_DEBUG( "Calculating hash code for LocalIdBuilder" );

		size_t hash = 17;

		if ( m_items.getPrimaryItem().has_value() )
			hash = hash * 31 + m_items.getPrimaryItem()->getHashCode();

		if ( m_items.getSecondaryItem().has_value() )
			hash = hash * 31 + m_items.getSecondaryItem()->getHashCode();

		if ( m_quantity.has_value() )
			hash = hash * 31 + m_quantity->hashCode();

		if ( m_calculation.has_value() )
			hash = hash * 31 + m_calculation->hashCode();

		if ( m_content.has_value() )
			hash = hash * 31 + m_content->hashCode();

		if ( m_position.has_value() )
			hash = hash * 31 + m_position->hashCode();

		if ( m_state.has_value() )
			hash = hash * 31 + m_state->hashCode();

		if ( m_command.has_value() )
			hash = hash * 31 + m_command->hashCode();

		if ( m_type.has_value() )
			hash = hash * 31 + m_type->hashCode();

		if ( m_detail.has_value() )
			hash = hash * 31 + m_detail->hashCode();

		return hash;
	}

	//-------------------------------------------------------------------------
	// Builder Methods - Metadata Tag Removal
	//-------------------------------------------------------------------------

	LocalIdBuilder LocalIdBuilder::withoutQuantity()
	{
		SPDLOG_INFO( "Removing quantity tag" );
		LocalIdBuilder result( *this );
		result.m_quantity = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withoutContent()
	{
		SPDLOG_INFO( "Removing content tag" );
		LocalIdBuilder result( *this );
		result.m_content = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withoutCalculation()
	{
		SPDLOG_INFO( "Removing calculation tag" );
		LocalIdBuilder result( *this );
		result.m_calculation = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withoutState()
	{
		SPDLOG_INFO( "Removing state tag" );
		LocalIdBuilder result( *this );
		result.m_state = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withoutCommand()
	{
		SPDLOG_INFO( "Removing command tag" );
		LocalIdBuilder result( *this );
		result.m_command = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withoutType()
	{
		SPDLOG_INFO( "Removing type tag" );
		LocalIdBuilder result( *this );
		result.m_type = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withoutPosition()
	{
		SPDLOG_INFO( "Removing position tag" );
		LocalIdBuilder result( *this );
		result.m_position = std::nullopt;
		return result;
	}

	LocalIdBuilder LocalIdBuilder::withoutDetail()
	{
		SPDLOG_INFO( "Removing detail tag" );
		LocalIdBuilder result( *this );
		result.m_detail = std::nullopt;
		return result;
	}
}
