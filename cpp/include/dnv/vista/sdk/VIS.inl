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
	{
	}

	//------------------------------------------
	// Public Methods
	//------------------------------------------

	template <typename K, typename V>
	inline V& VIS::Cache<K, V>::getOrCreate( const K& key, std::function<V()> factory ) const
	{
		std::scoped_lock lock( m_mutex );

		auto it = m_cache.find( key );
		if ( it == m_cache.end() )
		{
			if ( m_cache.size() >= 10 )
			{
				removeOldestEntry();
			}

			auto result = factory();
		}

		return it->second.value;
	}

	//------------------------------------------
	// Private Methods
	//------------------------------------------

	template <typename K, typename V>
	inline void VIS::Cache<K, V>::cleanupCache() const
	{
		for ( auto it = m_cache.begin(); it != m_cache.end(); )
		{
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
		}
		m_cache.erase( oldest );
	}
}
