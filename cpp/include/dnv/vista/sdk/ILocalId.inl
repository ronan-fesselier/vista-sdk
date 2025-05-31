/**
 * @file ILocalId.inl
 * @brief Inline definitions for the ILocalId template class.
 * @details This file contains the implementation of template methods declared in ILocalId.h.
 *          It should only be included at the end of ILocalId.h.
 */

namespace dnv::vista::sdk
{
	//=========================================================================
	// Inline Operator Definitions
	//=========================================================================

	template <typename T>
	inline bool ILocalId<T>::operator==( const T& other ) const
	{
		return equals( other );
	}

	template <typename T>
	inline bool ILocalId<T>::operator!=( const T& other ) const
	{
		return !equals( other );
	}

	//=========================================================================
	// Inline Static Parsing Method Definitions
	//=========================================================================

	template <typename T>
	inline T ILocalId<T>::parse( std::string_view localIdStr )
	{
		return T::parse( localIdStr );
	}

	template <typename T>
	inline bool ILocalId<T>::tryParse( std::string_view localIdStr, ParsingErrors& errors, std::optional<T>& localId )
	{
		return T::tryParse( localIdStr, errors, localId );
	}
}
