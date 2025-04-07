#include "pch.h"

#include "dnv/vista/sdk/Codebooks.h"
#include "dnv/vista/sdk/CodebookName.h"
#include "dnv/vista/sdk/VisVersion.h"
#include "dnv/vista/sdk/CodebooksDto.h"
#include "dnv/vista/sdk/MetadataTag.h"

namespace dnv::vista::sdk
{
	Codebooks::Codebooks( VisVersion version, const CodebooksDto& dto )
		: m_visVersion( version )
	{
		SPDLOG_INFO( "Initializing codebooks for VIS version: {}",
			VisVersionExtensions::toVersionString( version ) );

		constexpr int numCodebooks = static_cast<int>( CodebookName::Detail );
		SPDLOG_INFO( "Sizing codebook array to {} slots based on Detail enum value {}",
			numCodebooks, static_cast<int>( CodebookName::Detail ) );

		m_codebooks.resize( numCodebooks );

		SPDLOG_INFO( "Vector size after initialization: {}", m_codebooks.size() );
		SPDLOG_INFO( "Processing {} codebook items from DTO", dto.items.size() );

		for ( const auto& typeDto : dto.items )
		{
			try
			{
				SPDLOG_INFO( "Processing codebook: {}", typeDto.name );

				Codebook codebook( typeDto );
				CodebookName bookName = codebook.name();

				int index = static_cast<int>( bookName ) - 1;
				SPDLOG_INFO( "Mapped '{}' to index {} (enum value: {})",
					typeDto.name, index, static_cast<int>( bookName ) );

				if ( index >= 0 && index < numCodebooks )
				{
					m_codebooks[index] = codebook;
					SPDLOG_INFO( "Added codebook '{}' at index {}", typeDto.name, index );
				}
				else
				{
					SPDLOG_WARN( "Invalid codebook index: {} for name: {}",
						index, typeDto.name );
				}
			}
			catch ( const std::exception& ex )
			{
				SPDLOG_ERROR( "Error processing codebook '{}': {}", typeDto.name, ex.what() );
			}
		}

		std::unordered_map<std::string, std::vector<std::string>> emptyValues;
		CodebookDto detailDto( "detail", emptyValues );
		Codebook detailCodebook( detailDto );

		int detailIndex = static_cast<int>( detailCodebook.name() ) - 1;
		if ( detailIndex >= 0 && detailIndex < numCodebooks )
		{
			m_codebooks[detailIndex] = detailCodebook;
			SPDLOG_INFO( "Added empty Detail codebook at index {}", detailIndex );
		}

		std::size_t populatedCount = 0;
		for ( const auto& codebook : m_codebooks )
		{
			try
			{
				std::string prefix = CodebookNames::toPrefix( codebook.name() );
				if ( !prefix.empty() )
				{
					populatedCount++;
					SPDLOG_INFO( "Valid codebook found at index {}: {}",
						static_cast<int>( codebook.name() ) - 1, prefix );
				}
			}
			catch ( const std::exception& )
			{
				SPDLOG_WARN( "Empty/invalid codebook slot at index {}",
					&codebook - &m_codebooks[0] );
			}
		}

		SPDLOG_INFO( "Finished initializing {} codebooks ({} array slots, {} populated)",
			populatedCount, m_codebooks.size(), populatedCount );
	}

	VisVersion Codebooks::visVersion() const
	{
		return m_visVersion;
	}

	const Codebook& Codebooks::operator[]( CodebookName name ) const
	{
		if ( m_codebooks.empty() )
		{
			SPDLOG_ERROR( "Codebooks array is empty or uninitialized" );
		}

		int index = static_cast<int>( name ) - 1;

		SPDLOG_DEBUG( "Accessing codebook[{}], vector size: {}", index, m_codebooks.size() );

		if ( index >= static_cast<int>( m_codebooks.size() ) || index < 0 )
		{
			std::stringstream ss;
			ss << "Invalid codebook name: " << static_cast<int>( name );

			SPDLOG_ERROR( ss.str() );
			throw std::invalid_argument( ss.str() );
		}

		return m_codebooks[index];
	}

	std::optional<MetadataTag> Codebooks::tryCreateTag( CodebookName name, const std::string_view value ) const
	{
		SPDLOG_INFO( "Attempting to create tag '{}' for codebook: {} (enum value: {})",
			value, CodebookNames::toPrefix( name ), static_cast<int>( name ) );

		if ( m_codebooks.empty() )
		{
			SPDLOG_ERROR( "Codebooks array is empty or uninitialized" );
			return std::nullopt;
		}

		try
		{
			auto result = ( *this )[name].tryCreateTag( value );

			if ( result.has_value() )
			{
				SPDLOG_DEBUG( "Successfully created tag for '{}' in codebook '{}'",
					value, CodebookNames::ToPrefix( name ) );
			}
			else
			{
				SPDLOG_DEBUG( "Failed to create tag for '{}' in codebook '{}' - invalid value",
					value, CodebookNames::ToPrefix( name ) );
			}

			return result;
		}
		catch ( const std::exception& ex )
		{
			SPDLOG_ERROR( "Exception in TryCreateTag for '{}' in codebook '{}': {}",
				value, CodebookNames::toPrefix( name ), ex.what() );
			throw;
		}
	}

	MetadataTag Codebooks::createTag( CodebookName name, const std::string& value ) const
	{
		return ( *this )[name].createTag( value );
	}

	const Codebook& Codebooks::codebook( CodebookName name ) const
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
		{
			SPDLOG_ERROR( "Iterator out of range: index={}", m_index );
			throw std::invalid_argument( "Iterator out of range" );
		}

		const Codebook& codebook = ( *m_codebooks )[m_index];
		if ( !m_current.has_value() )
		{
			m_current = std::make_tuple( codebook.name(), std::cref( codebook ) );
		}
		else
		{
			m_current = std::make_tuple( codebook.name(), std::cref( codebook ) );
		}
		return *m_current;
	}

	Codebooks::Iterator::pointer Codebooks::Iterator::operator->() const
	{
		if ( static_cast<size_t>( m_index ) >= m_codebooks->size() )
		{
			SPDLOG_ERROR( "Iterator out of range: index={}", m_index );
			throw std::invalid_argument( "Iterator out of range" );
		}

		const Codebook& codebook = ( *m_codebooks )[m_index];
		if ( !m_current.has_value() )
		{
			m_current = std::make_tuple( codebook.name(), std::cref( codebook ) );
		}
		else
		{
			m_current = std::make_tuple( codebook.name(), std::cref( codebook ) );
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

	void Codebooks::Iterator::reset()
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

	Codebooks::Enumerator Codebooks::enumerator() const
	{
		return Iterator( &m_codebooks, 0 );
	}
}
