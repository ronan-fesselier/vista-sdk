#include "pch.h"

#include "dnv/vista/sdk/LocalId.h"
#include "dnv/vista/sdk/ParsingErrors.h"
#include "dnv/vista/sdk/IUniversalIdBuilder.h"
#include "dnv/vista/sdk/UniversalId.h"
#include "dnv/vista/sdk/UniversalIdBuilder.h"

namespace dnv::vista::sdk
{
	UniversalId::UniversalId( const std::shared_ptr<IUniversalIdBuilder>& builder )
		: m_builder( builder ),
		  m_localId( builder->GetLocalId().value().Build() ),
		  m_imoNumber( builder->GetImoNumber().value() )
	{
		if ( !builder->IsValid() )
		{
			SPDLOG_ERROR( "Invalid UniversalId state" );
			throw std::invalid_argument( "Invalid UniversalId state" );
		}
	}

	const ImoNumber& UniversalId::GetImoNumber() const
	{
		if ( !m_builder->GetImoNumber().has_value() )
		{
			SPDLOG_ERROR( "Invalid ImoNumber" );
			throw std::runtime_error( "Invalid ImoNumber" );
		}
		return m_imoNumber;
	}

	const LocalId& UniversalId::GetLocalId() const
	{
		return m_localId;
	}

	bool UniversalId::Equals( const UniversalId& other ) const
	{
		return m_builder->ToString() == other.m_builder->ToString();
	}

	UniversalId UniversalId::Parse( const std::string& universalIdStr )
	{
		auto builder = UniversalIdBuilder::Parse( universalIdStr );
		return builder.Build();
	}

	bool UniversalId::TryParse( const std::string& universalIdStr, ParsingErrors& errors, std::unique_ptr<UniversalId>& universalId )
	{
		std::shared_ptr<UniversalIdBuilder> builder;
		if ( !UniversalIdBuilder::TryParse( universalIdStr, errors, builder ) )
		{
			universalId = nullptr;
			return false;
		}

		universalId = std::make_unique<UniversalId>( builder );
		return true;
	}

	std::string UniversalId::ToString() const
	{
		return m_builder->ToString();
	}

	size_t UniversalId::GetHashCode() const
	{
		return std::hash<std::string>{}( m_builder->ToString() );
	}
}
