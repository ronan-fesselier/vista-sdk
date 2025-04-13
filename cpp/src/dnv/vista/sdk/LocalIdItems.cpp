#include "pch.h"

#include "dnv/vista/sdk/LocalIdItems.h"
#include "dnv/vista/sdk/VIS.h"

namespace dnv::vista::sdk
{
	LocalIdItems::LocalIdItems(
		const std::optional<GmodPath>& primaryItem,
		const std::optional<GmodPath>& secondaryItem )
	{
		try
		{
			if ( primaryItem.has_value() )
			{
				SPDLOG_INFO( "LocalIdItems: primaryItem: {}", primaryItem->ToString() );
				m_primaryItem = primaryItem;
			}
			else
			{
				SPDLOG_INFO( "LocalIdItems: primaryItem is empty" );
			}

			if ( secondaryItem.has_value() )
			{
				SPDLOG_INFO( "LocalIdItems: secondaryItem: {}", secondaryItem->ToString() );
				m_secondaryItem = secondaryItem;
			}
			else
			{
				SPDLOG_INFO( "LocalIdItems: secondaryItem is empty" );
			}
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "LocalIdItems: Error creating LocalIdItems: {}", e.what() );
			throw std::invalid_argument( "Error creating LocalIdItems" );
		}
		catch ( ... )
		{
			SPDLOG_ERROR( "LocalIdItems: Unknown error creating LocalIdItems" );
			throw std::runtime_error( "Unknown error creating LocalIdItems" );
		}

		SPDLOG_INFO( "LocalIdItems: Created successfully" );
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

	LocalIdItems::LocalIdItems( const LocalIdItems& other )
	{
		m_primaryItem = other.m_primaryItem;
		m_secondaryItem = other.m_secondaryItem;
	}

	bool LocalIdItems::operator==( const LocalIdItems& other ) const
	{
		return m_primaryItem == other.m_primaryItem &&
			   m_secondaryItem == other.m_secondaryItem;
	}

	bool LocalIdItems::operator!=( const LocalIdItems& other ) const
	{
		return !( *this == other );
	}

	const std::optional<GmodPath>& LocalIdItems::GetPrimaryItem() const
	{
		return m_primaryItem;
	}

	const std::optional<GmodPath>& LocalIdItems::GetSecondaryItem() const
	{
		return m_secondaryItem;
	}

	void LocalIdItems::Append( std::stringstream& builder, bool verboseMode ) const
	{
		if ( !m_primaryItem.has_value() && !m_secondaryItem.has_value() )
			return;

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
			if ( m_primaryItem.has_value() )
			{
				for ( const auto& [depth, name] : m_primaryItem->GetCommonNames() )
				{
					builder << '~';
					std::optional<std::string> location;

					if ( ( *m_primaryItem )[depth].GetLocation().has_value() )
						location = ( *m_primaryItem )[depth].GetLocation()->ToString();

					AppendCommonName( builder, name, location );
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
						location = ( *m_secondaryItem )[depth].GetLocation()->ToString();

					AppendCommonName( builder, name, location );
					builder << '/';
				}
			}
		}
	}

	void LocalIdItems::AppendCommonName( std::stringstream& builder,
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
					bool match = VIS::IsISOString( ch );
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
}
