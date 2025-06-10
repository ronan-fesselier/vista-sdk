/**
 * @file GmodPath.inl
 * @brief Inline implementations for performance-critical GmodPath operations
 */

#include "GmodNode.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// GmodPath class
	//=====================================================================

	//----------------------------------------------
	// Equality operators
	//----------------------------------------------

	inline bool GmodPath::operator==( const GmodPath& other ) const noexcept
	{
		if ( this == &other )
		{
			return true;
		}

		if ( m_parents.size() != other.m_parents.size() )
		{
			return false;
		}

		if ( m_visVersion != other.m_visVersion )
		{
			return false;
		}

		/* Compare parents */
		for ( size_t i = 0; i < m_parents.size(); ++i )
		{
			{ /* Null pointers */
				if ( ( !m_parents[i] && other.m_parents[i] ) || ( m_parents[i] && !other.m_parents[i] ) )
				{
					return false;
				}

				if ( !m_parents[i] && !other.m_parents[i] )
				{
					continue;
				}
			}

			{ /* Compare codes */
				if ( m_parents[i]->code() != other.m_parents[i]->code() )
				{
					return false;
				}
			}

			{ /* Compare locations */
				bool thisHasLocation = m_parents[i]->location().has_value();
				bool otherHasLocation = other.m_parents[i]->location().has_value();

				if ( thisHasLocation != otherHasLocation )
				{
					return false;
				}

				if ( thisHasLocation && otherHasLocation &&
					 m_parents[i]->location().value() != other.m_parents[i]->location().value() )
				{
					return false;
				}
			}
		}

		{ /* Null target nodes */
			if ( ( !m_node && other.m_node ) || ( m_node && !other.m_node ) )
			{
				return false;
			}

			if ( !m_node && !other.m_node )
			{
				return true;
			}
		}

		{ /* Target node code */

			if ( m_node->code() != other.m_node->code() )
			{
				return false;
			}
		}

		{ /* Target node location */

			bool thisHasLocation = m_node->location().has_value();
			bool otherHasLocation = other.m_node->location().has_value();

			if ( thisHasLocation != otherHasLocation )
			{
				return false;
			}

			if ( thisHasLocation && otherHasLocation &&
				 m_node->location().value() != other.m_node->location().value() )
			{
				return false;
			}
		}

		return true;
	}

	inline bool GmodPath::operator!=( const GmodPath& other ) const noexcept
	{
		return !( *this == other );
	}

	//----------------------------------------------
	// Lookup operators
	//----------------------------------------------

	inline GmodNode* GmodPath::operator[]( size_t index ) const
	{
		if ( index >= ( m_parents.size() + 1 ) )
		{
			throw std::out_of_range( fmt::format( "Index {} out of range for GmodPath indexer. Path length is {}.", index, m_parents.size() + 1 ) );
		}

		if ( index < m_parents.size() )
		{
			GmodNode* parentNode = m_parents[index];
			return parentNode;
		}
		else
		{
			return m_node;
		}
	}

	inline GmodNode*& GmodPath::operator[]( size_t index )
	{
		if ( index >= ( m_parents.size() + 1 ) )
		{
			throw std::out_of_range( fmt::format( "Index {} out of range for GmodPath indexer. Path length is {}.", index, m_parents.size() + 1 ) );
		}

		if ( index < m_parents.size() )
		{
			return m_parents[index];
		}
		else
		{
			return m_node;
		}
	}

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline VisVersion GmodPath::visVersion() const noexcept
	{
		return m_visVersion;
	}

	inline const Gmod* GmodPath::gmod() const noexcept
	{
		return m_gmod;
	}

	inline GmodNode* GmodPath::node() const noexcept
	{
		return m_node;
	}

	inline const std::vector<GmodNode*>& GmodPath::parents() const noexcept
	{
		return m_parents;
	}

	inline size_t GmodPath::length() const noexcept
	{
		return m_parents.size() + 1;
	}

	inline size_t GmodPath::hashCode() const noexcept
	{
		size_t hashCode = 0;

		for ( size_t i = 0; i < m_parents.size(); ++i )
		{
			if ( m_parents[i] )
			{
				size_t nodeHash = m_parents[i]->hashCode();
				hashCode ^= nodeHash + 0x9e3779b9 + ( hashCode << 6 ) + ( hashCode >> 2 );
			}
		}

		if ( m_node )
		{
			size_t nodeHash = m_node->hashCode();
			hashCode ^= nodeHash + 0x9e3779b9 + ( hashCode << 6 ) + ( hashCode >> 2 );
		}

		return hashCode;
	}

	//----------------------------------------------
	// GmodPath enumerator
	//----------------------------------------------

	//----------------------------
	// Enumeration methods
	//----------------------------

	inline bool GmodPath::Enumerator::next()
	{
		if ( !m_pathInstance )
		{
			return false;
		}

		if ( m_currentIndex == std::numeric_limits<size_t>::max() )
		{
			m_currentIndex = 0;
		}
		else
		{
			m_currentIndex++;
		}

		return m_currentIndex < m_pathInstance->length();
	}

	inline const GmodPath::Enumerator::PathElement& GmodPath::Enumerator::current() const
	{
		if ( !m_pathInstance || m_currentIndex >= m_pathInstance->length() )
		{
			static PathElement invalidElement{ std::numeric_limits<size_t>::max(), nullptr };
			return invalidElement;
		}

		m_current.first = m_currentIndex;
		m_current.second = ( *m_pathInstance )[m_currentIndex];
		return m_current;
	}

	inline void GmodPath::Enumerator::reset()
	{
		m_currentIndex = std::numeric_limits<size_t>::max();
		m_current = { std::numeric_limits<size_t>::max(), nullptr };
	}

	//=====================================================================
	// GmodIndividualizableSet class
	//=====================================================================

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline const std::vector<int>& GmodIndividualizableSet::nodeIndices() const noexcept
	{
		return m_nodeIndices;
	}

	inline std::optional<Location> GmodIndividualizableSet::location() const
	{
		if ( m_nodeIndices.empty() )
		{
			return std::nullopt;
		}

		int firstNodeIdx = m_nodeIndices[0];
		if ( firstNodeIdx < 0 || static_cast<size_t>( firstNodeIdx ) >= m_path.length() )
		{
			return std::nullopt;
		}

		const GmodNode* firstNode = m_path[static_cast<size_t>( firstNodeIdx )];
		if ( !firstNode )
		{
			return std::nullopt;
		}

		return firstNode->location();
	}
}
