/**
 * @file ParsingErrors.cpp
 * @brief Implementation of ParsingErrors class
 */

#include "pch.h"

#include "dnv/vista/sdk/ParsingErrors.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// ParsingErrors class
	//=====================================================================

	//----------------------------------------------
	// Static members
	//----------------------------------------------

	const ParsingErrors ParsingErrors::Empty = ParsingErrors{};

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	ParsingErrors::ParsingErrors( const std::vector<ErrorEntry>& errors )
		: m_errors{ errors }
	{
	}

	ParsingErrors::ParsingErrors()
		: m_errors{}
	{
	}

	//----------------------------------------------
	// Operators
	//----------------------------------------------

	bool ParsingErrors::operator==( const ParsingErrors& other ) const
	{
		return m_errors == other.m_errors;
	}

	bool ParsingErrors::operator!=( const ParsingErrors& other ) const
	{
		return !( *this == other );
	}

	//----------------------------------------------
	// Public methods
	//----------------------------------------------

	bool ParsingErrors::hasErrors() const
	{
		return !m_errors.empty();
	}

	bool ParsingErrors::hasErrorType( const std::string& type ) const
	{
		bool found = std::any_of( m_errors.begin(), m_errors.end(),
			[&type]( const ErrorEntry& error ) { return error.first == type; } );

		return found;
	}

	bool ParsingErrors::equals( const ParsingErrors& other ) const
	{
		return *this == other;
	}

	bool ParsingErrors::equals( const void* obj ) const
	{
		if ( obj == nullptr )
		{
			return false;
		}

		const ParsingErrors* other = static_cast<const ParsingErrors*>( obj );

		return equals( *other );
	}

	size_t ParsingErrors::count() const
	{
		return m_errors.size();
	}

	size_t ParsingErrors::hashCode() const noexcept
	{
		std::size_t hash = 0;
		for ( const auto& error : m_errors )
		{
			hash ^= std::hash<std::string>{}( error.first ) + 0x9e3779b9 + ( hash << 6 ) + ( hash >> 2 );
			hash ^= std::hash<std::string>{}( error.second ) + 0x9e3779b9 + ( hash << 6 ) + ( hash >> 2 );
		}

		return hash;
	}

	std::string ParsingErrors::toString() const
	{
		if ( m_errors.empty() )
		{
			return "Success";
		}

		std::ostringstream builder;
		builder << "Parsing errors:\n";

		for ( const auto& [type, message] : m_errors )
		{
			builder << '\t' << type << " - " << message << '\n';
		}

		std::string result = builder.str();

		return result;
	}

	//----------------------------
	// Enumeration
	//----------------------------

	ParsingErrors::Enumerator ParsingErrors::enumerator() const
	{
		return Enumerator( &m_errors );
	}

	//----------------------------------------------
	// ParsingErrors enumerator
	//----------------------------------------------

	//----------------------------
	// Construction / destruction
	//----------------------------

	ParsingErrors::Enumerator::Enumerator( const std::vector<ErrorEntry>* data )
		: m_data{ data },
		  m_index{ 0 },
		  m_current{}
	{
	}

	//----------------------------
	// Enumeration methods
	//----------------------------

	bool ParsingErrors::Enumerator::next()
	{
		if ( m_index < m_data->size() )
		{
			m_current = m_data->at( m_index );
			++m_index;
			return true;
		}

		m_index = m_data->size() + 1;
		m_current = {};
		return false;
	}

	const ParsingErrors::ErrorEntry& ParsingErrors::Enumerator::current() const
	{
		return m_current;
	}

	void ParsingErrors::Enumerator::reset()
	{
		m_index = 0;
		m_current = {};
	}
}
