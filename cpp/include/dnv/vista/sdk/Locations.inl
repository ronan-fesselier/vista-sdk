/**
 * @file UniversalIdBuilder.inl
 * @brief Inline implementations for performance-critical UniversalIdBuilder operations
 */

#pragma once

#include "config/Platform.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// Location Class
	//=====================================================================

	//----------------------------------------------
	// Comparison Operators
	//----------------------------------------------

	inline bool Location::operator==( const Location& other ) const
	{
		return m_value == other.m_value;
	}

	inline bool Location::operator!=( const Location& other ) const
	{
		return !( m_value == other.m_value );
	}

	//----------------------------------------------
	// Conversion Operators
	//----------------------------------------------

	inline Location::operator std::string() const noexcept
	{
		return m_value;
	}

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	const inline std::string& Location::value() const noexcept
	{
		return m_value;
	}

	VISTA_SDK_CPP_FORCE_INLINE int Location::hashCode() const noexcept
	{
		return utils::Hash::combine( m_value );
	}

	//=====================================================================
	// RelativeLocation Class
	//=====================================================================

	//----------------------------------------------
	// Comparison Operators
	//----------------------------------------------

	inline bool RelativeLocation::operator==( const RelativeLocation& other ) const
	{
		return m_code == other.m_code;
	}

	inline bool RelativeLocation::operator!=( const RelativeLocation& other ) const
	{
		return !( m_code == other.m_code );
	}

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline char RelativeLocation::code() const noexcept
	{
		return m_code;
	}

	inline const std::string& RelativeLocation::name() const noexcept
	{
		return m_name;
	}

	inline const std::optional<std::string>& RelativeLocation::definition() const noexcept
	{
		return m_definition;
	}

	inline const Location& RelativeLocation::location() const noexcept
	{
		return m_location;
	}

	VISTA_SDK_CPP_FORCE_INLINE int RelativeLocation::hashCode() const noexcept
	{
		return utils::Hash::combine( m_code );
	}
}
