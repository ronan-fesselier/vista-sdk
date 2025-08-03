/**
 * @file LocationBuilder.cpp
 * @brief Implementation of the LocationBuilder class
 */

#include "pch.h"

#include "dnv/vista/sdk/LocationBuilder.h"

#include "dnv/vista/sdk/Locations.h"
#include "dnv/vista/sdk/VISVersion.h"
#include "dnv/vista/sdk/utils/StringBuilderPool.h"
#include "dnv/vista/sdk/internal/HashMap.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// LocationBuilder class
	//=====================================================================

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	LocationBuilder::LocationBuilder( VisVersion visVersion, std::shared_ptr<const internal::HashMap<char, LocationGroup>> reversedGroups )
		: m_visVersion{ visVersion },
		  m_reversedGroups{ std::move( reversedGroups ) }
	{
	}

	//----------------------------------------------
	// Conversion and comparison
	//----------------------------------------------

	std::string LocationBuilder::toString() const
	{
		auto lease = utils::StringBuilderPool::instance();
		auto builder = lease.Builder();

		if ( m_number.has_value() )
		{
			builder.append( std::to_string( m_number.value() ) );
		}

		if ( m_side.has_value() )
		{
			builder.push_back( m_side.value() );
		}
		if ( m_vertical.has_value() )
		{
			builder.push_back( m_vertical.value() );
		}
		if ( m_transverse.has_value() )
		{
			builder.push_back( m_transverse.value() );
		}
		if ( m_longitudinal.has_value() )
		{
			builder.push_back( m_longitudinal.value() );
		}

		std::string result = lease.toString();
		std::sort( result.begin(), result.end() );

		return result;
	}

	//----------------------------------------------
	// Static factory methods
	//----------------------------------------------

	LocationBuilder LocationBuilder::create( const Locations& locations )
	{
		auto hashMap = std::make_shared<internal::HashMap<char, LocationGroup>>();

		const auto& originalMap = locations.reversedGroups();
		for ( const auto& [key, value] : originalMap )
		{
			hashMap->insertOrAssign( key, value );
		}

		return LocationBuilder( locations.visVersion(), hashMap );
	}

	//----------------------------------------------
	// Build methods (Immutable fluent interface)
	//----------------------------------------------

	//----------------------------
	// Build
	//----------------------------

	Location LocationBuilder::build() const
	{
		return Location( toString() );
	}

	//----------------------------
	// Location
	//----------------------------

	LocationBuilder LocationBuilder::withLocation( const Location& location ) const
	{
		LocationBuilder builder = *this;

		std::string locationStr = location.toString();
		std::string_view span = locationStr;
		std::optional<int> number;

		for ( size_t i = 0; i < span.length(); ++i )
		{
			char ch = span[i];

			if ( std::isdigit( ch ) )
			{
				if ( !number.has_value() )
				{
					number = ch - '0';
				}
				else
				{
					int parsedNumber;
					if ( !Locations::tryParseInt( span, 0, static_cast<int>( i + 1 ), parsedNumber ) )
					{
						throw std::invalid_argument( "Should include a valid number" );
					}
					number = parsedNumber;
				}
				continue;
			}

			builder = builder.withValue( ch );
		}

		if ( number.has_value() )
		{
			builder = builder.withNumber( number.value() );
		}

		return builder;
	}

	//----------------------------
	// Number
	//----------------------------

	LocationBuilder LocationBuilder::withNumber( int number ) const
	{
		if ( number < 1 )
		{
			throw std::invalid_argument( "Value should be greater than 0" );
		}

		LocationBuilder result = *this;
		result.m_number = number;

		return result;
	}

	LocationBuilder LocationBuilder::withoutNumber() const
	{
		LocationBuilder result = *this;
		result.m_number = std::nullopt;

		return result;
	}

	//----------------------------
	// Side
	//----------------------------

	LocationBuilder LocationBuilder::withSide( char side ) const
	{
		const auto* group = m_reversedGroups->tryGetValue( side );
		if ( !group || *group != LocationGroup::Side )
		{
			throw std::runtime_error(
				"The value '" + std::string{ 1, side } + "' is an invalid Side value" );
		}

		LocationBuilder result = *this;
		result.m_side = side;

		return result;
	}

	LocationBuilder LocationBuilder::withoutSide() const
	{
		LocationBuilder result = *this;
		result.m_side = std::nullopt;

		return result;
	}

	//----------------------------
	// Vertical
	//----------------------------

	LocationBuilder LocationBuilder::withVertical( char vertical ) const
	{
		const auto* group = m_reversedGroups->tryGetValue( vertical );
		if ( !group || *group != LocationGroup::Vertical )
		{
			throw std::runtime_error(
				"The value '" + std::string{ 1, vertical } + "' is an invalid Vertical value" );
		}

		LocationBuilder result = *this;
		result.m_vertical = vertical;

		return result;
	}

	LocationBuilder LocationBuilder::withoutVertical() const
	{
		LocationBuilder result = *this;
		result.m_vertical = std::nullopt;

		return result;
	}

	//----------------------------
	// Transverse
	//----------------------------

	LocationBuilder LocationBuilder::withTransverse( char transverse ) const
	{
		const auto* group = m_reversedGroups->tryGetValue( transverse );
		if ( !group || *group != LocationGroup::Transverse )
		{
			throw std::runtime_error(
				"The value '" + std::string{ 1, transverse } + "' is an invalid Transverse value" );
		}

		LocationBuilder result = *this;
		result.m_transverse = transverse;

		return result;
	}

	LocationBuilder LocationBuilder::withoutTransverse() const
	{
		LocationBuilder result = *this;
		result.m_transverse = std::nullopt;

		return result;
	}

	//----------------------------
	// Longitudinal
	//----------------------------

	LocationBuilder LocationBuilder::withLongitudinal( char longitudinal ) const
	{
		const auto* group = m_reversedGroups->tryGetValue( longitudinal );
		if ( !group || *group != LocationGroup::Longitudinal )
		{
			throw std::runtime_error(
				"The value '" + std::string{ 1, longitudinal } + "' is an invalid Longitudinal value" );
		}

		LocationBuilder result = *this;
		result.m_longitudinal = longitudinal;

		return result;
	}

	LocationBuilder LocationBuilder::withoutLongitudinal() const
	{
		LocationBuilder result = *this;
		result.m_longitudinal = std::nullopt;

		return result;
	}

	//----------------------------
	// Value
	//----------------------------

	LocationBuilder LocationBuilder::withValue( int value ) const
	{
		return withNumber( value );
	}

	LocationBuilder LocationBuilder::withValue( char value ) const
	{
		const auto* group = m_reversedGroups->tryGetValue( value );
		if ( !group )
		{
			throw std::runtime_error( "The value '" + std::string{ 1, value } + "' is an invalid Locations value" );
		}

		LocationBuilder result = *this;
		switch ( *group )
		{
			case LocationGroup::Side:
			{
				result.m_side = value;
				return result;
			}
			case LocationGroup::Vertical:
			{
				result.m_vertical = value;
				return result;
			}
			case LocationGroup::Transverse:
			{
				result.m_transverse = value;
				return result;
			}
			case LocationGroup::Longitudinal:
			{
				result.m_longitudinal = value;
				return result;
			}
			case LocationGroup::Number:
			{
				throw std::invalid_argument( "Number group should not contain character values" );
			}
			default:
			{
				throw std::invalid_argument( "Unsupported LocationGroup" );
			}
		}
	}

	LocationBuilder LocationBuilder::withoutValue( LocationGroup group ) const
	{
		switch ( group )
		{
			case LocationGroup::Number:
			{
				return withoutNumber();
			}
			case LocationGroup::Side:
			{
				return withoutSide();
			}
			case LocationGroup::Vertical:
			{
				return withoutVertical();
			}
			case LocationGroup::Transverse:
			{
				return withoutTransverse();
			}
			case LocationGroup::Longitudinal:
			{
				return withoutLongitudinal();
			}
			default:
			{
				throw std::invalid_argument( "Unsupported LocationGroup" );
			}
		}
	}
}
