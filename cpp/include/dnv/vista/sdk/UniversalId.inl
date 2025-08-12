/**
 * @file UniversalId.inl
 * @brief Inline implementations for performance-critical UniversalId operations
 */

#pragma once

#include "Config/AlgorithmConstants.h"
#include "Config/Platform.h"

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

	VISTA_SDK_CPP_FORCE_INLINE int UniversalId::hashCode() const noexcept
	{
		return utils::Hash::combine( m_imoNumber, m_localId );
	}
}
