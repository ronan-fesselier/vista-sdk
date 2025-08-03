/**
 * @file UniversalId.inl
 * @brief Inline implementations for performance-critical UniversalId operations
 */

#pragma once

#include "config/AlgorithmConstants.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// UniversalId class
	//=====================================================================

	//----------------------------------------------
	// Operators
	//----------------------------------------------

	inline bool UniversalId::operator==( const UniversalId& other ) const noexcept
	{
		return equals( other );
	}

	inline bool UniversalId::operator!=( const UniversalId& other ) const noexcept
	{
		return !equals( other );
	}

	inline bool UniversalId::equals( const UniversalId& other ) const noexcept
	{
		return m_imoNumber == other.m_imoNumber &&
			   m_localId.equals( other.m_localId );
	}

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline const ImoNumber& UniversalId::imoNumber() const noexcept
	{
		return m_imoNumber;
	}

	inline const LocalId& UniversalId::localId() const noexcept
	{
		return m_localId;
	}

	inline size_t UniversalId::hashCode() const noexcept
	{
		size_t hash = 0;

		hash ^= m_imoNumber.hashCode() + constants::HASH_MAGIC + ( hash << 6 ) + ( hash >> 2 );
		hash ^= m_localId.hashCode() + constants::HASH_MAGIC + ( hash << 6 ) + ( hash >> 2 );

		return hash;
	}
}
