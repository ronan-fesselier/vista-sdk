#include "pch.h"

#include "dnv/vista/sdk/ChdDictionary.h"
#include "dnv/vista/sdk/GmodNode.h"

namespace dnv::vista::sdk
{
	namespace internal
	{
		void ThrowHelper::ThrowKeyNotFoundException( std::string_view key )
		{
			throw std::out_of_range( "No value associated to key: " + std::string( key ) );
		}

		void ThrowHelper::ThrowInvalidOperationException()
		{
			throw std::invalid_argument( "Invalid operation" );
		}

		uint32_t Hashing::LarssonHash( uint32_t hash, uint8_t ch )
		{
			return 37 * hash + ch;
		}

		uint32_t Hashing::Fnv( uint32_t hash, uint8_t ch )
		{
			return ( ch ^ hash ) * 0x01000193;
		}

		uint32_t Hashing::Seed( uint32_t seed, uint32_t hash, uint64_t size )
		{
			auto x = seed + hash;
			x ^= x >> 12;
			x ^= x << 25;
			x ^= x >> 27;

			return static_cast<uint32_t>( ( x * 0x2545F4914F6CDD1DUL ) & ( size - 1 ) );
		}
	}

	template <typename TValue>
	ChdDictionary<TValue>::ChdDictionary( const std::vector<std::pair<std::string, TValue>>& items )
	{
		uint64_t size = 1;
		while ( size < static_cast<uint64_t>( items.size() ) )
			size *= 2;

		size *= 2;

		std::vector<std::vector<std::pair<int, uint32_t>>> h( size );

		for ( size_t i = 0; i < items.size(); i++ )
		{
			std::string_view k = items[i].first;
			uint32_t hash = Hash( k );
			h[hash & ( size - 1 )].push_back( { static_cast<int>( i + 1 ), hash } );
		}

		std::sort( h.begin(), h.end(), []( const auto& i, const auto& j ) {
			return j.size() - i.size();
		} );

		std::vector<int> indices( size, 0 );
		std::vector<int> seeds( size, 0 );

		size_t index;
		for ( index = 0; index < h.size() && h[index].size() > 1; ++index )
		{
			const auto& subKeys = h[index];

			uint32_t seed = 0;
			std::unordered_map<uint32_t, int> entries;

			while ( true )
			{
				++seed;
				bool success = true;

				for ( const auto& [idx, hash] : subKeys )
				{
					uint32_t seededHash = internal::Hashing::Seed( seed, hash, size );

					if ( entries.find( seededHash ) == entries.end() && indices[seededHash] == 0 )
					{
						entries[seededHash] = idx;
					}
					else
					{
						entries.clear();
						success = false;
						break;
					}
				}

				if ( success )
					break;
			}

			for ( const auto& [seededHash, idx] : entries )
				indices[seededHash] = idx;

			seeds[subKeys[0].second & ( size - 1 )] = static_cast<int>( seed );
		}

		m_table.resize( size );

		std::vector<int> free;
		for ( size_t i = 0; i < indices.size(); i++ )
		{
			if ( indices[i] == 0 )
			{
				free.push_back( static_cast<int>( i ) );
			}
			else
			{
				--indices[i];
				m_table[i] = items[indices[i]];
			}
		}

		for ( size_t i = 0; index < h.size() && h[index].size() > 0; i++ )
		{
			const auto& k = h[index++][0];
			int dst = free[i];
			indices[dst] = k.first - 1;
			m_table[dst] = items[k.first - 1];

			seeds[k.second & ( size - 1 )] = 0 - ( dst + 1 );
		}

		m_seeds = std::move( seeds );
	}

	template <typename TValue>
	TValue& ChdDictionary<TValue>::operator[]( std::string_view key )
	{
		for ( auto& kvp : m_table )
		{
			if ( kvp.first == key )
			{
				return kvp.second;
			}
		}
		throw std::out_of_range( "Key not found in ChdDictionary" );
	}

	template <typename TValue>
	const TValue& ChdDictionary<TValue>::operator[]( std::string_view key ) const
	{
		for ( const auto& kvp : m_table )
		{
			if ( kvp.first == key )
			{
				return kvp.second;
			}
		}
		throw std::out_of_range( "Key not found in ChdDictionary" );
	}

	template <typename TValue>
	bool ChdDictionary<TValue>::TryGetValue( std::string_view key, TValue* value ) const
	{
		if ( key.empty() )
		{
			if ( value )
				*value = TValue{};
			return false;
		}

		uint32_t hash = Hash( key );
		uint64_t size = m_table.size();
		auto index = hash & ( size - 1 );
		int seed = m_seeds[index];

		if ( seed < 0 )
		{
			const auto& kvp = m_table[0 - seed - 1];
			if ( key != kvp.first )
			{
				if ( value )
					*value = TValue{};
				return false;
			}

			if ( value )
				*value = kvp.second;
			return true;
		}
		else
		{
			index = internal::Hashing::Seed( static_cast<uint32_t>( seed ), hash, size );
			const auto& kvp = m_table[index];
			if ( key != kvp.first )
			{
				if ( value )
					*value = TValue{};
				return false;
			}
			if ( value )
				*value = kvp.second;
			return true;
		}
	}

	template <typename TValue>
	uint32_t ChdDictionary<TValue>::Hash( std::string_view key )
	{
		static_assert( sizeof( char ) == 1 || sizeof( char ) == 2, "Char size must be either 1 or 2 bytes" );

		uint32_t hash = 0x811C9DC5;
		const char* data = key.data();
		size_t size = key.size() * sizeof( char );

		for ( size_t i = 0; i < size; i += 2 )
		{
			hash = internal::Hashing::Fnv( hash, static_cast<uint8_t>( data[i] ) );
			if ( i + 1 < size )
				hash = internal::Hashing::Fnv( hash, static_cast<uint8_t>( data[i + 1] ) );
		}

		return hash;
	}

	template <typename TValue>
	ChdDictionary<TValue>::Iterator::Iterator( const std::vector<std::pair<std::string, TValue>>* table, int index )
		: m_table( table ), m_index( index )
	{
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Iterator::reference ChdDictionary<TValue>::Iterator::operator*() const
	{
		if ( static_cast<size_t>( m_index ) >= m_table->size() )
			internal::ThrowHelper::ThrowInvalidOperationException();

		m_current = ( *m_table )[m_index];
		return m_current;
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Iterator::pointer ChdDictionary<TValue>::Iterator::operator->() const
	{
		if ( static_cast<size_t>( m_index ) >= m_table->size() )
			internal::ThrowHelper::ThrowInvalidOperationException();

		m_current = ( *m_table )[m_index];
		return &m_current;
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Iterator& ChdDictionary<TValue>::Iterator::operator++()
	{
		do
		{
			++m_index;
		} while ( static_cast<size_t>( m_index ) < m_table->size() && ( *m_table )[m_index].first.empty() );

		if ( static_cast<size_t>( m_index ) >= m_table->size() )
		{
			m_index = static_cast<int>( m_table->size() );
		}

		return *this;
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Iterator ChdDictionary<TValue>::Iterator::operator++( int )
	{
		Iterator tmp = *this;
		++( *this );
		return tmp;
	}

	template <typename TValue>
	bool ChdDictionary<TValue>::Iterator::operator==( const Iterator& other ) const
	{
		return m_table == other.m_table && m_index == other.m_index;
	}

	template <typename TValue>
	bool ChdDictionary<TValue>::Iterator::operator!=( const Iterator& other ) const
	{
		return !( *this == other );
	}

	template <typename TValue>
	void ChdDictionary<TValue>::Iterator::Reset()
	{
		m_index = -1;
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Iterator ChdDictionary<TValue>::begin() const
	{
		Iterator it( &m_table, -1 );
		++it;
		return it;
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Iterator ChdDictionary<TValue>::end() const
	{
		if ( m_table.size() > static_cast<size_t>( std::numeric_limits<int>::max() ) )
		{
			throw std::overflow_error( "Size exceeds the maximum value of int" );
		}
		return Iterator( &m_table, static_cast<int>( m_table.size() ) );
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Enumerator ChdDictionary<TValue>::GetEnumerator() const
	{
		return Iterator( &m_table, -1 );
	}

	template class ChdDictionary<int>;
	template class ChdDictionary<std::string>;
	template class ChdDictionary<class GmodNode>;
}
