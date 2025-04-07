#include "pch.h"

#include "dnv/vista/sdk/ParsingErrors.h"

namespace dnv::vista::sdk
{
	const ParsingErrors ParsingErrors::Empty = ParsingErrors();

	ParsingErrors::ParsingErrors( const std::vector<ErrorEntry>& errors )
		: _errors( errors )
	{
	}

	bool ParsingErrors::HasErrors() const
	{
		return !_errors.empty();
	}

	bool ParsingErrors::HasErrorType( const std::string& type ) const
	{
		return std::any_of( _errors.begin(), _errors.end(),
			[&type]( const ErrorEntry& error ) { return std::get<0>( error ) == type; } );
	}

	std::string ParsingErrors::ToString() const
	{
		if ( _errors.empty() )
			return "Success";

		std::ostringstream builder;
		builder << "Parsing errors:\n";

		for ( const auto& [type, message] : _errors )
		{
			builder << '\t' << type << " - " << message << '\n';
		}

		return builder.str();
	}

	bool ParsingErrors::operator==( const ParsingErrors& other ) const
	{
		return _errors == other._errors;
	}

	bool ParsingErrors::operator!=( const ParsingErrors& other ) const
	{
		return !( *this == other );
	}

	ParsingErrors::Iterator::Iterator( const std::vector<ErrorEntry>* data, size_t index )
		: _data( data ), _index( index )
	{
	}

	ParsingErrors::Iterator::reference ParsingErrors::Iterator::operator*() const
	{
		return ( *_data )[_index];
	}

	ParsingErrors::Iterator::pointer ParsingErrors::Iterator::operator->() const
	{
		return &( ( *_data )[_index] );
	}

	ParsingErrors::Iterator& ParsingErrors::Iterator::operator++()
	{
		++_index;
		return *this;
	}

	ParsingErrors::Iterator ParsingErrors::Iterator::operator++( int )
	{
		Iterator tmp = *this;
		++( *this );
		return tmp;
	}

	bool ParsingErrors::Iterator::operator==( const Iterator& other ) const
	{
		return _data == other._data && _index == other._index;
	}

	bool ParsingErrors::Iterator::operator!=( const Iterator& other ) const
	{
		return !( *this == other );
	}

	ParsingErrors::Iterator ParsingErrors::begin() const
	{
		return Iterator( &_errors, 0 );
	}

	ParsingErrors::Iterator ParsingErrors::end() const
	{
		return Iterator( &_errors, _errors.size() );
	}
}
