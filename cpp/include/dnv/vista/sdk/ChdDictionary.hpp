#pragma once

#if defined( __SSE4_2__ ) || ( defined( _MSC_VER ) && defined( __AVX__ ) )
#	define HAS_SSE42 1
#else
#	define HAS_SSE42 0
#endif

#include "ChdDictionary.h"

namespace dnv::vista::sdk
{
	template <typename TValue>
	uint32_t ChdDictionary<TValue>::hash( std::string_view key )
	{
		static bool hasSse42 = internal::hasSSE42Support();

#if HAS_SSE42
		if ( key.empty() )
			return 0;

		uint32_t hashValue = 0x811C9DC5;

		const char* data = key.data();
		size_t len = key.size();

		for ( size_t i = 0; i < len; i++ )
		{
			hashValue = _mm_crc32_u8( hashValue, static_cast<uint8_t>( data[i] & 0xFF ) );
			hashValue = _mm_crc32_u8( hashValue, static_cast<uint8_t>( ( data[i] >> 8 ) & 0xFF ) );
		}

		SPDLOG_DEBUG( "Using SSE4.2 optimized hash value for key: {} / {}", key, hashValue );
		return hashValue;

#else
		uint32_t hashValue = 0x811C9DC5;

		for ( size_t i = 0; i < key.size(); i++ )
		{
			char c = key[i];
			hashValue = internal::Hashing::Fnv1a( hashValue, static_cast<uint8_t>( c & 0xFF ) );
			hashValue = internal::Hashing::Fnv1a( hashValue, static_cast<uint8_t>( ( c >> 8 ) & 0xFF ) );
		}

		SPDLOG_DEBUG( "Using standard hash value for key: {} / {}", key, hash );
		return hashValue;
#endif
	}

	template <typename TValue>
	ChdDictionary<TValue>::ChdDictionary( const std::vector<std::pair<std::string, TValue>>& items )
	{
		if ( items.empty() )
		{
			SPDLOG_INFO( "Created empty CHD Dictionary" );
			return;
		}

		uint64_t size = 1;
		while ( size < items.size() )
			size *= 2;
		size *= 2;

		SPDLOG_DEBUG( "Building CHD dictionary with {} items, table size {}", items.size(), size );

		std::vector<std::vector<std::pair<int, uint32_t>>> h( size );

		for ( size_t i = 0; i < items.size(); i++ )
		{
			const auto& key = items[i].first;
			uint32_t hashValue = hash( key );
			auto index = hashValue & ( size - 1 );
			h[index].push_back( { static_cast<int>( i + 1 ), hashValue } );
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
					uint32_t hash = internal::Hashing::seed( seed, k.second, size );

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
	ChdDictionary<TValue>::ChdDictionary( const ChdDictionary<TValue>& other )
		: m_table( other.m_table ), m_seeds( other.m_seeds )
	{
		SPDLOG_INFO( "ChdDictionary copied with {} entries ({} seeds)",
			m_table.size(), m_seeds.size() );
	}

	template <typename TValue>
	ChdDictionary<TValue>::ChdDictionary( ChdDictionary<TValue>&& other ) noexcept
		: m_table( std::move( other.m_table ) ), m_seeds( std::move( other.m_seeds ) )
	{
		SPDLOG_INFO( "ChdDictionary moved with {} entries ({} seeds)",
			m_table.size(), m_seeds.size() );
	}

	template <typename TValue>
	ChdDictionary<TValue>& ChdDictionary<TValue>::operator=( const ChdDictionary<TValue>& other )
	{
		if ( this != &other )
		{
			m_table = other.m_table;
			m_seeds = other.m_seeds;
		}

		SPDLOG_INFO( "ChdDictionary copy-assigned with {} entries ({} seeds)",
			m_table.size(), m_seeds.size() );

		return *this;
	}

	template <typename TValue>
	ChdDictionary<TValue>& ChdDictionary<TValue>::operator=( ChdDictionary<TValue>&& other ) noexcept
	{
		if ( this != &other )
		{
			m_table = std::move( other.m_table );
			m_seeds = std::move( other.m_seeds );
		}
		SPDLOG_INFO( "ChdDictionary move-assigned with {} entries ({} seeds)",
			m_table.size(), m_seeds.size() );

		return *this;
	}

	template <typename TValue>
	TValue& ChdDictionary<TValue>::operator[]( std::string_view key )
	{
		if ( isEmpty() )
		{
			SPDLOG_ERROR( "Attempted indexing empty dictionary with key: {}", key );
			internal::ThrowHelper::throwInvalidOperationException();
		}

		TValue* value = nullptr;
		if ( !tryGetValue( key, value ) )
		{
			internal::ThrowHelper::throwKeyNotFoundException( key );
		}
		return *value;
	}

	template <typename TValue>
	const TValue& ChdDictionary<TValue>::operator[]( std::string_view key ) const
	{
		if ( isEmpty() )
		{
			SPDLOG_ERROR( "Attempted indexing empty dictionary with key: {}", key );
			internal::ThrowHelper::throwInvalidOperationException();
		}

		TValue* value = nullptr;
		if ( !tryGetValue( key, value ) )
		{
			internal::ThrowHelper::throwKeyNotFoundException( key );
		}
		return *value;
	}

	template <typename TValue>
	bool ChdDictionary<TValue>::tryGetValue( std::string_view key, TValue* value ) const
	{
		if ( isEmpty() )
		{
			SPDLOG_WARN( "Attempted lookup in empty dictionary for key: {}", key );
			return false;
		}

		if ( m_table.empty() || key.empty() )
		{
			return false;
		}

		uint32_t hashValue = hash( key );
		uint64_t size = m_table.size();
		auto index = hashValue & ( size - 1 );
		int seed = m_seeds[index];

		if ( seed < 0 )
		{
			const auto& kvp = m_table[0 - seed - 1];
			if ( key != kvp.first )
			{
				return false;
			}

			if ( value )
				*value = kvp.second;
			return true;
		}
		else
		{
			index = internal::Hashing::seed( static_cast<uint32_t>( seed ), hashValue, size );
			const auto& kvp = m_table[index];

			if ( key != kvp.first )
			{
				return false;
			}

			if ( value )
				*value = kvp.second;
			return true;
		}
	}

	template <typename TValue>
	bool ChdDictionary<TValue>::isEmpty() const
	{
		bool tableEmpty = m_table.empty();
		bool seedsEmpty = m_seeds.empty();
		bool hasValidEntries = !tableEmpty && !seedsEmpty;

		SPDLOG_INFO( "isEmpty check: table_size={}, seeds_size={}, has_valid_entries={}",
			m_table.size(), m_seeds.size(), hasValidEntries );

		return !hasValidEntries;
	}

	template <typename TValue>
	ChdDictionary<TValue>::Iterator::Iterator(
		const std::vector<std::pair<std::string, TValue>>* table, int index )
		: m_table( table ), m_index( index )
	{
		if ( m_table != nullptr && index == 0 )
		{
			while ( m_index < m_table->size() && ( *m_table )[m_index].first.empty() )
				++m_index;
		}
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Iterator::reference
	ChdDictionary<TValue>::Iterator::operator*() const
	{
		if ( static_cast<size_t>( m_index ) >= m_table->size() )
			internal::ThrowHelper::throwInvalidOperationException();

		return ( *m_table )[m_index];
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Iterator::pointer
	ChdDictionary<TValue>::Iterator::operator->() const
	{
		if ( static_cast<size_t>( m_index ) >= m_table->size() )
			internal::ThrowHelper::throwInvalidOperationException();

		return &( ( *m_table )[m_index] );
	}

	template <typename TValue>
	typename ChdDictionary<TValue>::Iterator&
	ChdDictionary<TValue>::Iterator::operator++()
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
	typename ChdDictionary<TValue>::Iterator
	ChdDictionary<TValue>::Iterator::operator++( int )
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
	void ChdDictionary<TValue>::Iterator::reset()
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
	typename ChdDictionary<TValue>::Enumerator ChdDictionary<TValue>::enumerator() const
	{
		return begin();
	}
}
