#include "pch.h"

#include "dnv/vista/sdk/LocalIdItems.h"

#include "dnv/vista/sdk/VIS.h"

namespace dnv::vista::sdk
{
	//-------------------------------------------------------------------------
	// Constructors and Assignment
	//-------------------------------------------------------------------------

	LocalIdItems::LocalIdItems(
		const std::optional<GmodPath>& primaryItem,
		const std::optional<GmodPath>& secondaryItem )
	{
		if ( primaryItem.has_value() )
		{
			SPDLOG_INFO( "LocalIdItems: primaryItem: {}", primaryItem->ToString() );
			m_primaryItem = primaryItem;
		}
		else
		{
			SPDLOG_WARN( "LocalIdItems: primaryItem is empty" );
		}

		if ( secondaryItem.has_value() )
		{
			SPDLOG_INFO( "LocalIdItems: secondaryItem: {}", secondaryItem->ToString() );
			m_secondaryItem = secondaryItem;
		}
		else
		{
			SPDLOG_WARN( "LocalIdItems: secondaryItem is empty" );
		}

		SPDLOG_INFO( "LocalIdItems: Created successfully" );
	}

	LocalIdItems::LocalIdItems( const LocalIdItems& other )
	{
		m_primaryItem = other.m_primaryItem;
		m_secondaryItem = other.m_secondaryItem;
	}

	LocalIdItems& LocalIdItems::operator=( const LocalIdItems& other )
	{
		if ( this != &other )
		{
			m_primaryItem = other.m_primaryItem;
			m_secondaryItem = other.m_secondaryItem;
		}
		return *this;
	}

	//-------------------------------------------------------------------------
	// Core Properties
	//-------------------------------------------------------------------------

	const std::optional<GmodPath>& LocalIdItems::getPrimaryItem() const
	{
		return m_primaryItem;
	}

	const std::optional<GmodPath>& LocalIdItems::getSecondaryItem() const
	{
		return m_secondaryItem;
	}

	//-------------------------------------------------------------------------
	// String Generation
	//-------------------------------------------------------------------------

	void LocalIdItems::append( std::stringstream& builder, bool verboseMode ) const
	{
		if ( !m_primaryItem.has_value() && !m_secondaryItem.has_value() )
		{
			return;
		}

		if ( m_primaryItem.has_value() )
		{
			m_primaryItem->ToString( builder );
			builder << '/';
		}

		if ( m_secondaryItem.has_value() )
		{
			builder << "sec/";
			m_secondaryItem->ToString( builder );
			builder << '/';
		}

		if ( verboseMode )
		{
			SPDLOG_INFO( "Appending verbose information for LocalIdItems" );

			if ( m_primaryItem.has_value() )
			{
				for ( const auto& [depth, name] : m_primaryItem->GetCommonNames() )
				{
					builder << '~';
					std::optional<std::string> location;

					if ( ( *m_primaryItem )[depth].GetLocation().has_value() )
						location = ( *m_primaryItem )[depth].GetLocation()->toString();

					appendCommonName( builder, name, location );
					builder << '/';
				}
			}

			if ( m_secondaryItem.has_value() )
			{
				std::string prefix = "~for.";
				for ( const auto& [depth, name] : m_secondaryItem->GetCommonNames() )
				{
					builder << prefix;
					if ( prefix != "~" )
						prefix = "~";

					std::optional<std::string> location;

					if ( ( *m_secondaryItem )[depth].GetLocation().has_value() )
						location = ( *m_secondaryItem )[depth].GetLocation()->toString();

					appendCommonName( builder, name, location );
					builder << '/';
				}
			}
		}
	}

	void LocalIdItems::appendCommonName(
		std::stringstream& builder,
		const std::string& commonName,
		const std::optional<std::string>& location )
	{
		char prev = '\0';

		for ( const char ch : commonName )
		{
			if ( ch == '/' )
				continue;
			if ( prev == ' ' && ch == ' ' )
				continue;

			char current = ch;
			switch ( ch )
			{
				case ' ':
					current = '.';
					break;
				default:
					bool match = VIS::isISOString( ch );
					if ( !match )
					{
						current = '.';
						break;
					}
					current = static_cast<char>( std::tolower( ch ) );
					break;
			}

			if ( current == '.' && prev == '.' )
				continue;

			builder << current;
			prev = current;
		}

		if ( location.has_value() && !location->empty() )
		{
			builder << '.';
			builder << *location;
		}
	}

	//-------------------------------------------------------------------------
	// Comparison Operators
	//-------------------------------------------------------------------------

	bool LocalIdItems::operator==( const LocalIdItems& other ) const
	{
		return m_primaryItem == other.m_primaryItem &&
			   m_secondaryItem == other.m_secondaryItem;
	}

	bool LocalIdItems::operator!=( const LocalIdItems& other ) const
	{
		return !( *this == other );
	}
}
