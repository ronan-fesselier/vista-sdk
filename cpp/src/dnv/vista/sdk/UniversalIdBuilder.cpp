#include "pch.h"

#include "dnv/vista/sdk/ImoNumber.h"
#include "dnv/vista/sdk/IUniversalIdBuilder.h"
#include "dnv/vista/sdk/LocalIdBuilder.h"
#include "dnv/vista/sdk/ParsingErrors.h"
#include "dnv/vista/sdk/UniversalId.h"
#include "dnv/vista/sdk/UniversalIdBuilder.h"
#include "dnv/vista/sdk/VisVersion.h"

namespace dnv::vista::sdk
{
	std::optional<ImoNumber> UniversalIdBuilder::GetImoNumber() const
	{
		return m_imoNumber;
	}

	std::optional<LocalIdBuilder> UniversalIdBuilder::GetLocalId() const
	{
		return m_localId;
	}

	bool UniversalIdBuilder::IsValid() const
	{
		return m_imoNumber.has_value() && m_localId.has_value() && m_localId->IsValid();
	}

	UniversalIdBuilder UniversalIdBuilder::Create( VisVersion version )
	{
		return UniversalIdBuilder().WithLocalId( LocalIdBuilder::Create( version ) );
	}

	UniversalId UniversalIdBuilder::Build() const
	{
		if ( !IsValid() )
		{
			throw std::invalid_argument( "Invalid UniversalIdBuilder state" );
		}
		return UniversalId( std::make_shared<UniversalIdBuilder>( *this ) );
	}

	UniversalIdBuilder UniversalIdBuilder::WithLocalId( const LocalIdBuilder& localId )
	{
		bool succeeded;
		return TryWithLocalId( localId, succeeded );
	}

	UniversalIdBuilder UniversalIdBuilder::WithoutLocalId()
	{
		m_localId.reset();
		return *this;
	}

	UniversalIdBuilder UniversalIdBuilder::TryWithLocalId( const std::optional<LocalIdBuilder>& localId )
	{
		bool succeeded;
		return TryWithLocalId( localId, succeeded );
	}

	UniversalIdBuilder UniversalIdBuilder::TryWithLocalId( const std::optional<LocalIdBuilder>& localId, bool& succeeded )
	{
		if ( !localId.has_value() )
		{
			succeeded = false;
			return *this;
		}
		succeeded = true;
		m_localId = localId;
		return *this;
	}

	UniversalIdBuilder UniversalIdBuilder::WithImoNumber( const ImoNumber& imoNumber )
	{
		bool succeeded;
		return TryWithImoNumber( imoNumber, succeeded );
	}

	UniversalIdBuilder UniversalIdBuilder::TryWithImoNumber( const std::optional<ImoNumber>& imoNumber )
	{
		bool succeeded;
		return TryWithImoNumber( imoNumber, succeeded );
	}

	UniversalIdBuilder UniversalIdBuilder::TryWithImoNumber( const std::optional<ImoNumber>& imoNumber, bool& succeeded )
	{
		if ( !imoNumber.has_value() )
		{
			succeeded = false;
			return *this;
		}
		succeeded = true;
		m_imoNumber = imoNumber;
		return *this;
	}

	UniversalIdBuilder UniversalIdBuilder::WithoutImoNumber()
	{
		m_imoNumber.reset();
		return *this;
	}

	std::string UniversalIdBuilder::ToString() const
	{
		if ( !m_imoNumber.has_value() )
		{
			throw std::invalid_argument( "Invalid UniversalIdBuilder state: Missing IMO Number" );
		}
		if ( !m_localId.has_value() )
		{
			throw std::invalid_argument( "Invalid UniversalIdBuilder state: Missing LocalId" );
		}

		std::ostringstream builder;
		builder << NamingEntity << "/";
		builder << m_imoNumber->ToString();
		builder << m_localId->ToString();

		return builder.str();
	}

	bool UniversalIdBuilder::Equals( const UniversalIdBuilder& other ) const
	{
		return m_imoNumber == other.m_imoNumber && m_localId == other.m_localId;
	}

	size_t UniversalIdBuilder::GetHashCode() const
	{
		std::hash<std::string> hasher;
		size_t hash = 0;
		if ( m_imoNumber.has_value() )
		{
			hash ^= hasher( m_imoNumber->ToString() );
		}
		if ( m_localId.has_value() )
		{
			hash ^= hasher( m_localId->ToString() );
		}
		return hash;
	}

	UniversalIdBuilder UniversalIdBuilder::Parse( const std::string& universalIdStr )
	{
		ParsingErrors errors;
		std::shared_ptr<UniversalIdBuilder> builder;
		if ( !TryParse( universalIdStr, errors, builder ) )
		{
			SPDLOG_ERROR( "Failed to parse UniversalId: {}", universalIdStr );

			throw std::invalid_argument( "Failed to parse UniversalId: " + universalIdStr );
		}
		return *builder;
	}

	bool UniversalIdBuilder::TryParse( const std::string& universalIdStr, ParsingErrors& errors, std::shared_ptr<UniversalIdBuilder>& builder )
	{
		if ( universalIdStr.empty() )
		{
			return false;
		}

		builder = std::make_shared<UniversalIdBuilder>();

		builder->m_imoNumber = ImoNumber( 1234567 ); // TODO Replace with actual parsing logic
		builder->m_localId = LocalIdBuilder();		 // TODO Replace with actual parsing logic

		return true;
	}

	void UniversalIdBuilder::AddError( LocalIdParsingErrorBuilder& errorBuilder, LocalIdParsingState state, const std::string& message )
	{
		errorBuilder.AddError( state, message );
	}

	LocalIdParsingErrorBuilder& LocalIdParsingErrorBuilder::AddError( LocalIdParsingState state )
	{
		if ( m_predefinedErrorMessages.find( state ) == m_predefinedErrorMessages.end() )
		{
			SPDLOG_ERROR( "Couldn't find predefined message for the given state" );

			throw std::invalid_argument( "Couldn't find predefined message for the given state." );
		}

		m_errors.emplace_back( state, m_predefinedErrorMessages.at( state ) );
		return *this;
	}

	LocalIdParsingErrorBuilder& LocalIdParsingErrorBuilder::AddError( LocalIdParsingState state, const std::string& message )
	{
		if ( message.empty() )
		{
			return AddError( state );
		}

		SPDLOG_ERROR( "Error: {}", message );

		m_errors.emplace_back( state, message );
		return *this;
	}

	bool LocalIdParsingErrorBuilder::HasError() const
	{
		return !m_errors.empty();
	}

	ParsingErrors LocalIdParsingErrorBuilder::Build() const
	{
		if ( m_errors.empty() )
		{
			std::vector<ParsingErrors::ErrorEntry> convertedErrors;
			for ( const auto& [result, message] : m_errors )
			{
				convertedErrors.emplace_back( std::to_string( static_cast<int>( result ) ), message );
			}
			return ParsingErrors( convertedErrors );
		}
		return ParsingErrors( std::vector<ParsingErrors::ErrorEntry>{} );
	}

	LocalIdParsingErrorBuilder LocalIdParsingErrorBuilder::Create()
	{
		return LocalIdParsingErrorBuilder();
	}
}
