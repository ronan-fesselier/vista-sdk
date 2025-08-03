/**
 * @file Hashing.h
 * @brief Hashing utilities
 */

#pragma once

#include "dnv/vista/sdk/config/AlgorithmConstants.h"
#include "dnv/vista/sdk/config/Platform.h"

namespace dnv::vista::sdk::utils
{
	namespace Hash
	{
		template <typename... Args>
		VISTA_SDK_CPP_FORCE_INLINE int combine( const Args&... args ) noexcept;

		template <typename T>
		VISTA_SDK_CPP_FORCE_INLINE int hash( const T& value ) noexcept
		{
			if constexpr ( requires { value.has_value(); } )
			{
				return value.has_value() ? hash( *value ) : 0;
			}
			else if constexpr ( requires { value.hashCode(); } )
			{
				return value.hashCode();
			}
			else if constexpr ( requires { value.type; value.message; } )
			{
				return combine( value.type, value.message );
			}
			else
			{
				return static_cast<int>( std::hash<std::decay_t<T>>{}( value ) );
			}
		}

		template <typename Container>
		VISTA_SDK_CPP_FORCE_INLINE int hashContainer( const Container& container ) noexcept
		{
			std::size_t seed = 0;
			for ( const auto& item : container )
			{
				seed ^= hash( item ) + constants::FNV_OFFSET_BASIS + ( seed << 6 ) + ( seed >> 2 );
			}

			return static_cast<int>( seed );
		}

		template <typename Container, typename T>
		VISTA_SDK_CPP_FORCE_INLINE int hashSequence( const Container& container, const T& finalItem ) noexcept
		{
			std::size_t seed = 0;
			for ( const auto& item : container )
			{
				seed ^= hash( item ) + constants::FNV_OFFSET_BASIS + ( seed << 6 ) + ( seed >> 2 );
			}
			seed ^= hash( finalItem ) + constants::FNV_OFFSET_BASIS + ( seed << 6 ) + ( seed >> 2 );

			return static_cast<int>( seed );
		}

		template <typename... Args>
		VISTA_SDK_CPP_FORCE_INLINE int combine( const Args&... args ) noexcept
		{
			std::size_t seed = 0;
			( ( seed ^= hash( args ) + constants::FNV_OFFSET_BASIS + ( seed << 6 ) + ( seed >> 2 ) ), ... );

			return static_cast<int>( seed );
		}
	}
}
