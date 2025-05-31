/**
 * @file VIS.inl
 * @brief Template implementations for VIS class.
 */

#pragma once

#include "VIS.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// Caching
	//=====================================================================

	//----------------------------------------------
	// Construction / destruction
	//----------------------------------------------

	template <typename K, typename V>
	inline VIS::Cache<K, V>::Cache()
		: m_lastCleanup{ std::chrono::steady_clock::now() }
	{
	}

	//------------------------------------------
	// Public methods
	//------------------------------------------

	template <typename K, typename V>
	inline V& VIS::Cache<K, V>::getOrCreate( const K& key, std::function<V()> factory ) const
	{
		const auto now = std::chrono::steady_clock::now();

		std::scoped_lock lock( m_mutex );

		if ( now - m_lastCleanup > std::chrono::hours( 1 ) )
		{
			cleanupCache();
			m_lastCleanup = now;
		}

		auto it = m_cache.find( key );
		if ( it == m_cache.end() )
		{
			if ( m_cache.size() >= 10 )
			{
				removeOldestEntry();
			}

			auto result = factory();
			auto [inserted_it, success] = m_cache.emplace( key, CacheItem{ std::move( result ), now } );

			return inserted_it->second.value;
		}

		it->second.lastAccess = now;

		return it->second.value;
	}

	template <typename K, typename V>
	inline void VIS::Cache<K, V>::teardown() const
	{
		std::scoped_lock lock( m_mutex );

		if constexpr ( std::is_pointer_v<V> )
		{
			for ( auto& [key, item] : m_cache )
			{
				delete item.value;
			}
		}

		m_cache.clear();
	}

	//------------------------------------------
	// Private Methods
	//------------------------------------------

	template <typename K, typename V>
	inline void VIS::Cache<K, V>::cleanupCache() const
	{
		const auto now = std::chrono::steady_clock::now();
		for ( auto it = m_cache.begin(); it != m_cache.end(); )
		{
			if ( now - it->second.lastAccess > std::chrono::hours( 1 ) )
				it = m_cache.erase( it );
			else
				++it;
		}
	}

	template <typename K, typename V>
	inline void VIS::Cache<K, V>::removeOldestEntry() const
	{
		if ( m_cache.empty() )
			return;

		auto oldest = m_cache.begin();
		for ( auto it = std::next( m_cache.begin() ); it != m_cache.end(); ++it )
		{
			if ( it->second.lastAccess < oldest->second.lastAccess )
			{
				oldest = it;
			}
		}
		m_cache.erase( oldest );
	}
}
