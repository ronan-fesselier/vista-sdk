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
		return m_imoNumber.has_value() && m_localId.has_value() && m_localId->isValid();
	}

	UniversalIdBuilder UniversalIdBuilder::Create( VisVersion version )
	{
		return UniversalIdBuilder().WithLocalId( LocalIdBuilder::create( version ) );
	}

	UniversalId UniversalIdBuilder::Build() const
	{
		if ( !IsValid() )
		{
			SPDLOG_ERROR( "Invalid UniversalIdBuilder state" );
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
			SPDLOG_ERROR( "Invalid UniversalIdBuilder state: Missing IMO Number" );
			throw std::invalid_argument( "Invalid UniversalIdBuilder state: Missing IMO Number" );
		}
		if ( !m_localId.has_value() )
		{
			SPDLOG_ERROR( "Invalid UniversalIdBuilder state: Missing LocalId" );
			throw std::invalid_argument( "Invalid UniversalIdBuilder state: Missing LocalId" );
		}

		std::ostringstream builder;
		builder << NamingEntity << "/";
		builder << m_imoNumber->toString();
		builder << m_localId->toString();

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
			hash ^= hasher( m_imoNumber->toString() );
		}
		if ( m_localId.has_value() )
		{
			hash ^= hasher( m_localId->toString() );
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
		builder = nullptr;

		LocalIdParsingErrorBuilder errorBuilder = LocalIdParsingErrorBuilder::create();

		if ( universalIdStr.empty() )
		{
			AddError( errorBuilder, LocalIdParsingState::NamingRule, "Failed to find localId start segment" );
			errors = errorBuilder.build();
			return false;
		}

		size_t localIdStartIndex = universalIdStr.find( "/dnv-v" );
		if ( localIdStartIndex == std::string::npos )
		{
			AddError( errorBuilder, LocalIdParsingState::NamingRule, "Failed to find localId start segment" );
			errors = errorBuilder.build();
			return false;
		}

		std::string universalIdSegment = universalIdStr.substr( 0, localIdStartIndex );
		std::string localIdSegment = universalIdStr.substr( localIdStartIndex );

		std::optional<LocalIdBuilder> localIdBuilder;
		ParsingErrors parseErrors;
		if ( !LocalIdBuilder::tryParse( localIdSegment, parseErrors, localIdBuilder ) )
		{
			errors = errorBuilder.build();
			std::vector<ParsingErrors::ErrorEntry> combinedErrors;
			for ( const auto& err : errors )
				combinedErrors.push_back( err );
			for ( const auto& err : parseErrors )
				combinedErrors.push_back( err );

			errors = ParsingErrors( combinedErrors );

			return false;
		}

		std::optional<ImoNumber> imoNumber;

		LocalIdParsingState state = LocalIdParsingState::NamingEntity;
		size_t i = 0;

		while ( state <= LocalIdParsingState::IMONumber )
		{
			if ( i >= universalIdSegment.length() )
				break;

			size_t nextSlash = universalIdSegment.find( '/', i );
			std::string segment;

			if ( nextSlash == std::string::npos )
			{
				segment = universalIdSegment.substr( i );
			}
			else
			{
				segment = universalIdSegment.substr( i, nextSlash - i );
			}

			switch ( state )
			{
				case LocalIdParsingState::NamingRule:
				case LocalIdParsingState::VisVersion:
				case LocalIdParsingState::PrimaryItem:
				case LocalIdParsingState::SecondaryItem:
				case LocalIdParsingState::ItemDescription:
				case LocalIdParsingState::MetaQuantity:
				case LocalIdParsingState::MetaContent:
				case LocalIdParsingState::MetaCalculation:
				case LocalIdParsingState::MetaState:
				case LocalIdParsingState::MetaCommand:
				case LocalIdParsingState::MetaType:
				case LocalIdParsingState::MetaPosition:
				case LocalIdParsingState::MetaDetail:
				case LocalIdParsingState::EmptyState:
				case LocalIdParsingState::Formatting:
				case LocalIdParsingState::Completeness:
					break;
				case LocalIdParsingState::NamingEntity:
					if ( segment != LocalIdBuilder::NamingRule )
					{
						AddError( errorBuilder, state, "Naming entity segment didn't match. Found: " + segment );
					}
					break;

				case LocalIdParsingState::IMONumber:
					std::optional<ImoNumber> imo = ImoNumber::tryParse( segment );
					if ( !imo.has_value() )
					{
						AddError( errorBuilder, state, "Invalid IMO number segment" );
					}
					else
					{
						imoNumber = imo;
					}
					break;
			}

			state = static_cast<LocalIdParsingState>( static_cast<int>( state ) + 1 );
			i += segment.length() + 1;
		}

		std::optional<VisVersion> visVersion = localIdBuilder->getVisVersion();
		if ( !visVersion.has_value() )
		{
			AddError( errorBuilder, LocalIdParsingState::VisVersion, nullptr );
			errors = errorBuilder.build();
			return false;
		}

		builder = std::make_shared<UniversalIdBuilder>( Create( visVersion.value() ) );
		builder->TryWithLocalId( *localIdBuilder );
		builder->TryWithImoNumber( imoNumber );

		errors = errorBuilder.build();
		return true;
	}

	void UniversalIdBuilder::AddError( LocalIdParsingErrorBuilder& errorBuilder, LocalIdParsingState state, const std::string& message )
	{
		errorBuilder.addError( state, message );
	}
}
