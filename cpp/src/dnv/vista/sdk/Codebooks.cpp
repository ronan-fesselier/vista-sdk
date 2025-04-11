#include "pch.h"

#include "dnv/vista/sdk/Codebooks.h"
#include "dnv/vista/sdk/CodebookName.h"

#include "dnv/vista/sdk/CodebooksDto.h"
#include "dnv/vista/sdk/MetadataTag.h"

namespace dnv::vista::sdk
{
	Codebooks::Codebooks( VisVersion version, const CodebooksDto& dto )
		: m_visVersion( version )
	{
		constexpr int numCodebooks = static_cast<int>( CodebookName::Detail );
		m_codebooks.resize( numCodebooks );

		for ( const auto& typeDto : dto.items )
		{
			Codebook codebook( typeDto );
			int index = static_cast<int>( codebook.GetName() ) - 1;

			if ( index >= 0 && index < numCodebooks )
				m_codebooks[index] = codebook;
			else
				SPDLOG_WARN( "Invalid codebook name index: {}", index );
		}

		std::unordered_map<std::string, std::vector<std::string>> emptyValues;
		CodebookDto detailsDto( "detail", emptyValues );
		Codebook detailsCodebook( detailsDto );
		int detailIndex = static_cast<int>( CodebookName::Detail ) - 1;
		if ( detailIndex >= 0 && detailIndex < numCodebooks )
			m_codebooks[detailIndex] = detailsCodebook;
	}

	VisVersion Codebooks::GetVisVersion() const
	{
		return m_visVersion;
	}

	const Codebook& Codebooks::operator[]( CodebookName name ) const
	{
		int index = static_cast<int>( name ) - 1;
		if ( index >= static_cast<int>( m_codebooks.size() ) || index < 0 )
		{
			std::stringstream ss;
			ss << "Invalid codebook name: " << static_cast<int>( name );
			throw std::invalid_argument( ss.str() );
		}

		return m_codebooks[index];
	}

	std::optional<MetadataTag> Codebooks::TryCreateTag( CodebookName name, const std::string_view value ) const
	{
		return ( *this )[name].TryCreateTag( value );
	}

	MetadataTag Codebooks::CreateTag( CodebookName name, const std::string& value ) const
	{
		return ( *this )[name].CreateTag( value );
	}

	const Codebook& Codebooks::GetCodebook( CodebookName name ) const
	{
		return ( *this )[name];
	}

	Codebooks::Iterator::Iterator( const std::vector<Codebook>* codebooks, int index )
		: m_codebooks( codebooks ), m_index( index )
	{
	}

	Codebooks::Iterator::reference Codebooks::Iterator::operator*() const
	{
		if ( static_cast<size_t>( m_index ) >= m_codebooks->size() )
			throw std::invalid_argument( "Iterator out of range" );

		const Codebook& codebook = ( *m_codebooks )[m_index];
		if ( !m_current.has_value() )
		{
			m_current = std::make_tuple( codebook.GetName(), std::cref( codebook ) );
		}
		else
		{
			m_current = std::make_tuple( codebook.GetName(), std::cref( codebook ) );
		}
		return *m_current;
	}

	Codebooks::Iterator::pointer Codebooks::Iterator::operator->() const
	{
		if ( static_cast<size_t>( m_index ) >= m_codebooks->size() )
			throw std::invalid_argument( "Iterator out of range" );

		const Codebook& codebook = ( *m_codebooks )[m_index];
		if ( !m_current.has_value() )
		{
			m_current = std::make_tuple( codebook.GetName(), std::cref( codebook ) );
		}
		else
		{
			m_current = std::make_tuple( codebook.GetName(), std::cref( codebook ) );
		}
		return &( *m_current );
	}

	Codebooks::Iterator& Codebooks::Iterator::operator++()
	{
		++m_index;
		return *this;
	}

	Codebooks::Iterator Codebooks::Iterator::operator++( int )
	{
		Iterator tmp = *this;
		++( *this );
		return tmp;
	}

	bool Codebooks::Iterator::operator==( const Iterator& other ) const
	{
		return m_codebooks == other.m_codebooks && m_index == other.m_index;
	}

	bool Codebooks::Iterator::operator!=( const Iterator& other ) const
	{
		return !( *this == other );
	}

	void Codebooks::Iterator::Reset()
	{
		m_index = -1;
	}

	Codebooks::Iterator Codebooks::begin() const
	{
		return Iterator( &m_codebooks, 0 );
	}

	Codebooks::Iterator Codebooks::end() const
	{
		return Iterator( &m_codebooks, static_cast<int>( m_codebooks.size() ) );
	}

	Codebooks::Enumerator Codebooks::GetEnumerator() const
	{
		return Iterator( &m_codebooks, 0 );
	}
}
