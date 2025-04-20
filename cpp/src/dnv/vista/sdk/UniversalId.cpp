#include "pch.h"

#include "dnv/vista/sdk/LocalId.h"
#include "dnv/vista/sdk/ParsingErrors.h"
#include "dnv/vista/sdk/IUniversalIdBuilder.h"
#include "dnv/vista/sdk/UniversalId.h"
#include "dnv/vista/sdk/UniversalIdBuilder.h"

namespace dnv::vista::sdk
{
	//-------------------------------------------------------------------------
	// Construction / Destruction
	//-------------------------------------------------------------------------

	UniversalId::UniversalId( const std::shared_ptr<IUniversalIdBuilder>& builder )
		: m_builder( builder ),
		  m_localId( builder->localId().value().build() ),
		  m_imoNumber( builder->imoNumber().value() )
	{
		SPDLOG_INFO( "Constructing UniversalId" );
		if ( !builder->isValid() )
		{
			SPDLOG_ERROR( "Invalid UniversalId state" );
			throw std::invalid_argument( "Invalid UniversalId state" );
		}
	}

	//-------------------------------------------------------------------------
	// Interface Implementation
	//-------------------------------------------------------------------------

	const ImoNumber& UniversalId::imoNumber() const
	{
		SPDLOG_INFO( "Getting IMO number from UniversalId" );
		if ( !m_builder->imoNumber().has_value() )
		{
			SPDLOG_ERROR( "Invalid ImoNumber" );
			throw std::runtime_error( "Invalid ImoNumber" );
		}
		return m_imoNumber;
	}

	const LocalId& UniversalId::localId() const
	{
		SPDLOG_INFO( "Getting LocalId from UniversalId" );
		return m_localId;
	}

	//-------------------------------------------------------------------------
	// Static Methods - Parsing
	//-------------------------------------------------------------------------

	UniversalId UniversalId::parse( const std::string& universalIdStr )
	{
		SPDLOG_INFO( "Parsing UniversalId from string: {}", universalIdStr );
		auto builder = UniversalIdBuilder::parse( universalIdStr );
		return builder.build();
	}

	bool UniversalId::tryParse( const std::string& universalIdStr, ParsingErrors& errors, std::unique_ptr<UniversalId>& universalId )
	{
		SPDLOG_INFO( "Attempting to parse UniversalId: {}", universalIdStr );
		std::shared_ptr<UniversalIdBuilder> builder;

		if ( !UniversalIdBuilder::tryParse( universalIdStr, errors, builder ) )
		{
			SPDLOG_WARN( "Failed to parse UniversalId: {}", universalIdStr );
			universalId = nullptr;
			return false;
		}

		SPDLOG_INFO( "Successfully parsed UniversalId" );
		universalId = std::make_unique<UniversalId>( builder );
		return true;
	}

	//-------------------------------------------------------------------------
	// Instance Methods - Comparison
	//-------------------------------------------------------------------------

	bool UniversalId::equals( const UniversalId& other ) const
	{
		SPDLOG_INFO( "Comparing UniversalIds for equality" );
		return m_builder->toString() == other.m_builder->toString();
	}

	bool UniversalId::operator==( const UniversalId& other ) const
	{
		return equals( other );
	}

	bool UniversalId::operator!=( const UniversalId& other ) const
	{
		return !equals( other );
	}

	//-------------------------------------------------------------------------
	// Instance Methods - Conversion
	//-------------------------------------------------------------------------

	std::string UniversalId::toString() const
	{
		SPDLOG_INFO( "Converting UniversalId to string" );
		return m_builder->toString();
	}

	size_t UniversalId::hashCode() const
	{
		SPDLOG_INFO( "Calculating hash code for UniversalId" );
		return std::hash<std::string>{}( m_builder->toString() );
	}
}
