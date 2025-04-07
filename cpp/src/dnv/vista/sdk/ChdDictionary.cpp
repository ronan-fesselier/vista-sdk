#include "pch.h"

#include "dnv/vista/sdk/ChdDictionary.h"

namespace dnv::vista::sdk
{
	namespace internal
	{
		bool hasSSE42Support()
		{
#if defined( _MSC_VER )
			int cpuInfo[4];
			__cpuid( cpuInfo, 1 );
			return ( cpuInfo[2] & ( 1 << 20 ) ) != 0;
#elif defined( __GNUC__ )
			unsigned int eax, ebx, ecx, edx;
			if ( __get_cpuid( 1, &eax, &ebx, &ecx, &edx ) )
				return ( ecx & ( 1 << 20 ) ) != 0;
			return false;
#else
			return false;
#endif
		}

		void ThrowHelper::throwKeyNotFoundException( std::string_view key )
		{
			SPDLOG_ERROR( "Key not found: {}", key );
			throw std::out_of_range( "No value associated to key: " + std::string( key ) );
		}

		void ThrowHelper::throwInvalidOperationException()
		{
			SPDLOG_ERROR( "Invalid operation" );
			throw std::invalid_argument( "Invalid operation" );
		}

		uint32_t Hashing::Fnv1a( uint32_t hash, uint8_t ch )
		{
			return ( ch ^ hash ) * 0x01000193;
		}

		uint32_t Hashing::seed( uint32_t seed, uint32_t hash, uint64_t size )
		{
			uint32_t x = seed + hash;
			x ^= x >> 12;
			x ^= x << 25;
			x ^= x >> 27;

			return static_cast<uint32_t>( ( static_cast<uint64_t>( x ) * 0x2545F4914F6CDD1DUL ) & ( size - 1 ) );
		}
	}
}
