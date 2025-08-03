/**
 * @file ImoNumber.inl
 * @brief Inline implementations for performance-critical ImoNumber operations
 */

#pragma once

#include "config/Platform.h"
#include "utils/Hashing.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// ImoNumber class
	//=====================================================================

	//----------------------------------------------
	// Operators
	//----------------------------------------------

	inline bool ImoNumber::operator==( const ImoNumber& other ) const noexcept
	{
		return m_value == other.m_value;
	}

	inline bool ImoNumber::operator!=( const ImoNumber& other ) const noexcept
	{
		return !( *this == other );
	}

	inline ImoNumber::operator int() const noexcept
	{
		return m_value;
	}

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	VISTA_SDK_CPP_FORCE_INLINE int ImoNumber::hashCode() const noexcept
	{
		return utils::Hash::hash( m_value );
	}
}
