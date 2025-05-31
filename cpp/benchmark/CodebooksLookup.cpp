/**
 * @file CodebooksLookup.cpp
 * @brief Codebooks lookup performance comparison between hash tables and array access
 */

#include "pch.h"

#include "dnv/vista/sdk/VIS.h"

using namespace dnv::vista::sdk;

namespace dnv::vista::sdk::benchmarks
{
	static std::unordered_map<CodebookName, const Codebook*> g_dict;
	static std::unordered_map<CodebookName, const Codebook*> g_frozenDict;
	static const dnv::vista::sdk::Codebooks* g_codebooks = nullptr;
	static bool g_initialized = false;

	static void InitializeData()
	{
		if ( !g_initialized )
		{
			auto& vis = VIS::instance();
			g_codebooks = &vis.codebooks( VisVersion::v3_7a );

			g_dict.clear();
			g_dict.reserve( NUM_CODEBOOKS );
			g_frozenDict.clear();
			g_frozenDict.reserve( NUM_CODEBOOKS );

			for ( size_t i = 1; i <= NUM_CODEBOOKS; ++i )
			{
				CodebookName name = static_cast<CodebookName>( i );
				const Codebook* ptr = &( *g_codebooks )[name];
				g_dict[name] = ptr;
				g_frozenDict[name] = ptr;
			}

			g_initialized = true;
		}
	}

	template <typename T>
	bool TryGetValue( const std::unordered_map<CodebookName, T>& map, CodebookName key )
	{
		return map.find( key ) != map.end();
	}

	static void Dict( benchmark::State& state )
	{
		InitializeData();

		for ( auto _ : state )
		{
			bool result =
				TryGetValue( g_dict, CodebookName::Quantity ) &&
				TryGetValue( g_dict, CodebookName::Type ) &&
				TryGetValue( g_dict, CodebookName::Detail );

			benchmark::DoNotOptimize( result );
		}
	}

	static void FrozenDict( benchmark::State& state )
	{
		InitializeData();

		for ( auto _ : state )
		{
			bool result =
				TryGetValue( g_frozenDict, CodebookName::Quantity ) &&
				TryGetValue( g_frozenDict, CodebookName::Type ) &&
				TryGetValue( g_frozenDict, CodebookName::Detail );

			benchmark::DoNotOptimize( result );
		}
	}

	static void Codebooks( benchmark::State& state )
	{
		InitializeData();

		for ( auto _ : state )
		{
			const Codebook* dummy1 = &( *g_codebooks )[CodebookName::Quantity];
			const Codebook* dummy2 = &( *g_codebooks )[CodebookName::Type];
			const Codebook* dummy3 = &( *g_codebooks )[CodebookName::Detail];

			bool result = ( dummy1 != nullptr ) && ( dummy2 != nullptr ) && ( dummy3 != nullptr );

			benchmark::DoNotOptimize( result );
		}
	}
}

BENCHMARK( dnv::vista::sdk::benchmarks::Dict )->MinTime( 10.0 )->Unit( benchmark::kNanosecond );
BENCHMARK( dnv::vista::sdk::benchmarks::FrozenDict )->MinTime( 10.0 )->Unit( benchmark::kNanosecond );
BENCHMARK( dnv::vista::sdk::benchmarks::Codebooks )->MinTime( 10.0 )->Unit( benchmark::kNanosecond );

BENCHMARK_MAIN();
