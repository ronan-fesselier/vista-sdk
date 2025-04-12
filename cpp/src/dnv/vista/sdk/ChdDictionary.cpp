#include "pch.h"

#include "dnv/vista/sdk/ChdDictionary.h"

#include "dnv/vista/sdk/GmodNode.h"
#include "dnv/vista/sdk/Codebooks.h"

namespace dnv::vista::sdk
{
	namespace internal
	{
		void ThrowHelper::ThrowKeyNotFoundException( std::string_view key )
		{
			SPDLOG_ERROR( "Key not found: {}", key );
			throw std::out_of_range( "No value associated to key: " + std::string( key ) );
		}

		void ThrowHelper::ThrowInvalidOperationException()
		{
			SPDLOG_ERROR( "Invalid operation" );
			throw std::invalid_argument( "Invalid operation" );
		}
	}

	template <typename TValue>
	ChdDictionary<TValue>::ChdDictionary( const std::vector<std::pair<std::string, TValue>>& items )
	{
		uint64_t size = 1;
		while ( size < items.size() )
			size *= 2;

		size *= 2;

		std::vector<std::vector<std::pair<int, uint32_t>>> h( size );

		for ( size_t i = 0; i < items.size(); i++ )
		{
			const auto& key = items[i].first;
			uint32_t hash = Hash( key );
			auto index = hash & ( size - 1 );
			h[index].push_back( { static_cast<int>( i + 1 ), hash } );
		}

		std::sort( h.begin(), h.end(),
			[]( const auto& a, const auto& b ) { return a.size() > b.size(); } );

		std::vector<int> indices( size, 0 );
		std::vector<int> seeds( size, 0 );

		size_t index = 0;
		for ( ; index < h.size() && h[index].size() > 1; ++index )
		{
			const auto& subKeys = h[index];

			uint32_t seed = 0;
			std::unordered_map<uint32_t, int> entries;

			bool retry;
			do
			{
				retry = false;
				++seed;
				entries.clear();

				for ( const auto& k : subKeys )
				{
					uint32_t hash = internal::Hashing::Seed( seed, k.second, size );

					if ( entries.find( hash ) == entries.end() && indices[hash] == 0 )
					{
						entries[hash] = k.first;
					}
					else
					{
						retry = true;
						break;
					}
				}
			} while ( retry );

			for ( const auto& entry : entries )
			{
				indices[entry.first] = entry.second;
			}

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

		int freeIndex = 0;
		for ( size_t i = 0; index < h.size() && h[index].size() > 0; i++, index++ )
		{
			const auto& k = h[index][0];

			if ( freeIndex < free.size() )
			{
				int slotIndex = free[freeIndex++];
				m_table[slotIndex] = items[k.first - 1];

				seeds[k.second & ( size - 1 )] = -( slotIndex + 1 );
			}
		}

		m_seeds = std::move( seeds );
	}

	template <typename TValue>
	TValue& ChdDictionary<TValue>::operator[]( std::string_view key )
	{
		TValue* value = nullptr;
		if ( !TryGetValue( key, value ) )
		{
			internal::ThrowHelper::ThrowKeyNotFoundException( key );
		}
		return *value;
	}

	template <typename TValue>
	const TValue& ChdDictionary<TValue>::operator[]( std::string_view key ) const
	{
		TValue* value = nullptr;
		if ( !TryGetValue( key, value ) )
		{
			internal::ThrowHelper::ThrowKeyNotFoundException( key );
		}
		return *value;
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
		uint32_t hash = 0x811C9DC5;

		for ( size_t i = 0; i < key.size(); i++ )
		{
			uint8_t lowByte = static_cast<uint8_t>( key[i] );
			hash = ( lowByte ^ hash ) * 0x01000193;

			uint8_t highByte = 0;
			hash = ( highByte ^ hash ) * 0x01000193;
		}
		return hash;
	}
	template <typename TValue>
	ChdDictionary<TValue>::Iterator::Iterator( const std::vector<std::pair<std::string, TValue>>* table, int index )
		: m_table( table ), m_index( index )
	{
		if ( m_table != nullptr && index == 0 )
		{
			while ( m_index < m_table->size() && ( *m_table )[m_index].first.empty() )
				++m_index;
		}
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Iterator::reference ChdDictionary<TValue>::Iterator::operator*() const
	{
		if ( static_cast<size_t>( m_index ) >= m_table->size() )
			internal::ThrowHelper::ThrowInvalidOperationException();

		return ( *m_table )[m_index];
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Iterator::pointer ChdDictionary<TValue>::Iterator::operator->() const
	{
		if ( static_cast<size_t>( m_index ) >= m_table->size() )
			internal::ThrowHelper::ThrowInvalidOperationException();

		return &( ( *m_table )[m_index] );
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Iterator& ChdDictionary<TValue>::Iterator::operator++()
	{
		if ( m_table == nullptr )
			return *this;

		while ( ++m_index < m_table->size() )
		{
			const auto& entry = ( *m_table )[m_index];
			if ( !entry.first.empty() )
			{
				return *this;
			}
		}

		m_index = static_cast<int>( m_table->size() );
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
		SPDLOG_INFO( "Creating iterator - table has {} entries", m_table.size() );

		for ( size_t i = 0; i < m_table.size(); i++ )
		{
			if ( !m_table[i].first.empty() )
			{
				SPDLOG_INFO( "Found first valid entry at {}: key={}", i, m_table[i].first );
				return Iterator( &m_table, static_cast<int>( i ) );
			}
		}

		return end();
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Iterator ChdDictionary<TValue>::end() const
	{
		return Iterator( &m_table, static_cast<int>( m_table.size() ) );
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Enumerator ChdDictionary<TValue>::GetEnumerator() const
	{
		return begin();
	}

	template class ChdDictionary<GmodNode>;
	template class ChdDictionary<Codebook>;
}
