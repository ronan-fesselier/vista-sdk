/**
 * @file MetadataTag.cpp
 * @brief Implementation of the MetadataTag class
 */

#include "pch.h"

#include "dnv/vista/sdk/MetadataTag.h"

#include "dnv/vista/sdk/CodebookName.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// MetadataTag class
	//=====================================================================

	//-------------------------------------------------------------------------
	// Construction / destruction
	//-------------------------------------------------------------------------

	MetadataTag::MetadataTag( CodebookName name, const std::string& value, bool isCustom )
		: m_name{ name },
		  m_custom{ isCustom },
		  m_value{ value }
	{
	}

	//-------------------------------------------------------------------------
	// String conversion methods
	//-------------------------------------------------------------------------

	std::string MetadataTag::toString() const noexcept
	{
		return m_value;
	}

	void MetadataTag::toString( std::string& builder, char separator ) const
	{
		const auto prefixView = CodebookNames::toPrefix( m_name );

		const auto currentSize = builder.size();
		const auto requiredSize = prefixView.size() + 1 + m_value.size() + 1;
		builder.reserve( currentSize + requiredSize );

		builder.append( prefixView );
		builder.push_back( m_custom ? '~' : '-' );
		builder.append( m_value );
		builder.push_back( separator );
	}
}
