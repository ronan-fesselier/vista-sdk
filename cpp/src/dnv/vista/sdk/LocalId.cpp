#include "pch.h"

#include "dnv/vista/sdk/LocalId.h"
#include "dnv/vista/sdk/GmodPath.h"
#include "dnv/vista/sdk/LocalIdBuilder.h"
#include "dnv/vista/sdk/MetadataTag.h"
#include "dnv/vista/sdk/ParsingErrors.h"
#include "dnv/vista/sdk/VisVersion.h"

namespace dnv::vista::sdk
{
	const std::string LocalId::NamingRule = "dnv-v2";

	LocalId::LocalId( const LocalIdBuilder& builder )
		: m_builder( builder )
	{
		if ( m_builder.isEmpty() )
		{
			SPDLOG_ERROR( "LocalId cannot be constructed from empty LocalIdBuilder" );
			throw std::invalid_argument( "LocalId cannot be constructed from empty LocalIdBuilder" );
		}
		if ( !m_builder.isValid() )
		{
			SPDLOG_ERROR( "LocalId cannot be constructed from invalid LocalIdBuilder" );
			throw std::invalid_argument( "LocalId cannot be constructed from invalid LocalIdBuilder" );
		}
	}

	const LocalIdBuilder& LocalId::getBuilder() const
	{
		return m_builder;
	}

	VisVersion LocalId::getVisVersion() const
	{
		return *m_builder.getVisVersion();
	}

	bool LocalId::getVerboseMode() const
	{
		return m_builder.getVerboseMode();
	}

	const GmodPath& LocalId::getPrimaryItem() const
	{
		return *m_builder.getPrimaryItem();
	}

	std::optional<GmodPath> LocalId::getSecondaryItem() const
	{
		return m_builder.getSecondaryItem();
	}

	const std::vector<MetadataTag> LocalId::getMetadataTags() const
	{
		return m_builder.getMetadataTags();
	}

	std::optional<MetadataTag> LocalId::getQuantity() const
	{
		return m_builder.getQuantity();
	}

	std::optional<MetadataTag> LocalId::getContent() const
	{
		return m_builder.getContent();
	}

	std::optional<MetadataTag> LocalId::getCalculation() const
	{
		return m_builder.getCalculation();
	}

	std::optional<MetadataTag> LocalId::getState() const
	{
		return m_builder.getState();
	}

	std::optional<MetadataTag> LocalId::getCommand() const
	{
		return m_builder.getCommand();
	}

	std::optional<MetadataTag> LocalId::getType() const
	{
		return m_builder.getType();
	}

	std::optional<MetadataTag> LocalId::getPosition() const
	{
		return m_builder.getPosition();
	}

	std::optional<MetadataTag> LocalId::getDetail() const
	{
		return m_builder.getDetail();
	}

	bool LocalId::hasCustomTag() const
	{
		return m_builder.hasCustomTag();
	}

	std::string LocalId::toString() const
	{
		SPDLOG_INFO( "Converting LocalId to string" );
		return m_builder.toString();
	}

	LocalId LocalId::parse( const std::string& localIdStr )
	{
		SPDLOG_INFO( "Parsing LocalId from string: {}", localIdStr );
		return LocalIdBuilder::parse( localIdStr ).build();
	}

	bool LocalId::tryParse( const std::string& localIdStr, ParsingErrors& errors, std::optional<LocalId>& localId )
	{
		SPDLOG_INFO( "Attempting to parse LocalId from: {}", localIdStr );

		std::optional<LocalIdBuilder> localIdBuilder;
		if ( !LocalIdBuilder::tryParse( localIdStr, errors, localIdBuilder ) )
		{
			SPDLOG_ERROR( "LocalId parsing failed" );
			localId = std::nullopt;
			return false;
		}

		SPDLOG_INFO( "LocalId parsing succeeded, building final LocalId" );
		try
		{
			localId.emplace( localIdBuilder->build() );
			return true;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Failed to build LocalId after parsing: {}", e.what() );
			errors.addError( "BuildError", e.what() );
			localId = std::nullopt;
			return false;
		}
	}

	bool LocalId::equals( const LocalId& other ) const
	{
		return m_builder == other.m_builder;
	}

	bool LocalId::operator==( const LocalId& other ) const
	{
		return m_builder == other.m_builder;
	}

	bool LocalId::operator!=( const LocalId& other ) const
	{
		return !( *this == other );
	}

	size_t LocalId::getHashCode() const
	{
		return m_builder.getHashCode();
	}

	LocalIdParsingErrorBuilder& LocalIdParsingErrorBuilder::addError( LocalIdParsingState state )
	{
		auto it = m_predefinedErrorMessages.find( state );
		if ( it != m_predefinedErrorMessages.end() )
		{
			m_errors.emplace_back( state, it->second );
			SPDLOG_INFO( "Added predefined parsing error: {}", it->second );
		}
		return *this;
	}

	LocalIdParsingErrorBuilder& LocalIdParsingErrorBuilder::addError( LocalIdParsingState state, const std::string& message )
	{
		m_errors.emplace_back( state, message );
		SPDLOG_INFO( "Added custom parsing error for state {}: {}", static_cast<int>( state ), message );
		return *this;
	}

	bool LocalIdParsingErrorBuilder::hasError() const
	{
		return !m_errors.empty();
	}

	ParsingErrors LocalIdParsingErrorBuilder::build() const
	{
		ParsingErrors errors;
		for ( const auto& [state, message] : m_errors )
		{
			std::string stateStr;
			switch ( state )
			{
				case LocalIdParsingState::NamingRule:
					stateStr = "NamingRule";
					break;
				case LocalIdParsingState::VisVersion:
					stateStr = "VisVersion";
					break;
				case LocalIdParsingState::PrimaryItem:
					stateStr = "PrimaryItem";
					break;

				default:
					stateStr = "State" + std::to_string( static_cast<int>( state ) );
					break;
			}

			errors.addError( stateStr, message );
		}
		return errors;
	}

	LocalIdParsingErrorBuilder LocalIdParsingErrorBuilder::create()
	{
		return LocalIdParsingErrorBuilder();
	}
}
