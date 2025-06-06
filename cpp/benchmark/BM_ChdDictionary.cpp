/**
 * @file BM_ChdDictionary.cpp
 * @brief CHD Dictionary comprehensive benchmarks including hash function profiling
 */

#include "pch.h"

#include "dnv/vista/sdk/ChdDictionary.h"
#include "dnv/vista/sdk/Codebook.h"
#include "dnv/vista/sdk/Codebooks.h"
#include "dnv/vista/sdk/CodebookName.h"
#include "dnv/vista/sdk/VIS.h"

using namespace dnv::vista::sdk;

namespace dnv::vista::sdk::benchmarks
{
	//=====================================================================
	// Hash function benchmarks - exact codebook strings
	//=====================================================================

	static void BM_hash_Type( benchmark::State& state )
	{
		const std::string_view key = "Type";
		for ( auto _ : state )
		{
			uint32_t hash = ChdDictionary<std::string_view>::hash( key );
			benchmark::DoNotOptimize( hash );
		}

		/* Add custom counter for hash rate */
		state.counters["HashRate"] = benchmark::Counter( state.iterations(), benchmark::Counter::kIsRate );
	}

	static void BM_hash_Detail( benchmark::State& state )
	{
		const std::string_view key = "Detail";
		for ( auto _ : state )
		{
			uint32_t hash = ChdDictionary<std::string_view>::hash( key );
			benchmark::DoNotOptimize( hash );
		}
		state.counters["HashRate"] = benchmark::Counter( state.iterations(), benchmark::Counter::kIsRate );
	}

	static void BM_hash_Quantity( benchmark::State& state )
	{
		const std::string_view key = "Quantity";
		for ( auto _ : state )
		{
			uint32_t hash = ChdDictionary<std::string_view>::hash( key );
			benchmark::DoNotOptimize( hash );
		}
		state.counters["HashRate"] = benchmark::Counter( state.iterations(), benchmark::Counter::kIsRate );
	}

	static void BM_hash_Position( benchmark::State& state )
	{
		const std::string_view key = "Position";
		for ( auto _ : state )
		{
			uint32_t hash = ChdDictionary<std::string_view>::hash( key );
			benchmark::DoNotOptimize( hash );
		}
		state.counters["HashRate"] = benchmark::Counter( state.iterations(), benchmark::Counter::kIsRate );
	}

	static void BM_hash_State( benchmark::State& state )
	{
		const std::string_view key = "State";
		for ( auto _ : state )
		{
			uint32_t hash = ChdDictionary<std::string_view>::hash( key );
			benchmark::DoNotOptimize( hash );
		}
		state.counters["HashRate"] = benchmark::Counter( state.iterations(), benchmark::Counter::kIsRate );
	}

	static void BM_hash_Content( benchmark::State& state )
	{
		const std::string_view key = "Content";
		for ( auto _ : state )
		{
			uint32_t hash = ChdDictionary<std::string_view>::hash( key );
			benchmark::DoNotOptimize( hash );
		}
		state.counters["HashRate"] = benchmark::Counter( state.iterations(), benchmark::Counter::kIsRate );
	}

	static void BM_hash_Command( benchmark::State& state )
	{
		const std::string_view key = "Command";
		for ( auto _ : state )
		{
			uint32_t hash = ChdDictionary<std::string_view>::hash( key );
			benchmark::DoNotOptimize( hash );
		}
		state.counters["HashRate"] = benchmark::Counter( state.iterations(), benchmark::Counter::kIsRate );
	}

	static void BM_hash_Calculation( benchmark::State& state )
	{
		const std::string_view key = "Calculation";
		for ( auto _ : state )
		{
			uint32_t hash = ChdDictionary<std::string_view>::hash( key );
			benchmark::DoNotOptimize( hash );
		}
		state.counters["HashRate"] = benchmark::Counter( state.iterations(), benchmark::Counter::kIsRate );
	}

	static void BM_hash_ActivityType( benchmark::State& state )
	{
		const std::string_view key = "ActivityType";
		for ( auto _ : state )
		{
			uint32_t hash = ChdDictionary<std::string_view>::hash( key );
			benchmark::DoNotOptimize( hash );
		}
		state.counters["HashRate"] = benchmark::Counter( state.iterations(), benchmark::Counter::kIsRate );
	}

	static void BM_hash_FunctionalServices( benchmark::State& state )
	{
		const std::string_view key = "FunctionalServices";
		for ( auto _ : state )
		{
			uint32_t hash = ChdDictionary<std::string_view>::hash( key );
			;
			benchmark::DoNotOptimize( hash );
		}
		state.counters["HashRate"] = benchmark::Counter( state.iterations(), benchmark::Counter::kIsRate );
	}

	static void BM_hash_MaintenanceCategory( benchmark::State& state )
	{
		const std::string_view key = "MaintenanceCategory";
		for ( auto _ : state )
		{
			uint32_t hash = ChdDictionary<std::string_view>::hash( key );
			benchmark::DoNotOptimize( hash );
		}
		state.counters["HashRate"] = benchmark::Counter( state.iterations(), benchmark::Counter::kIsRate );
	}

	//=====================================================================
	// Hash function benchmarks - by string length
	//=====================================================================

	static void BM_hash_ByLength( benchmark::State& state )
	{
		size_t length = static_cast<size_t>( state.range( 0 ) );
		std::string key( length, 'A' ); /* Create string of 'A's */

		for ( auto _ : state )
		{
			uint32_t hash = ChdDictionary<std::string_view>::hash( key );
			benchmark::DoNotOptimize( hash );
		}

		state.counters["Length"] = length;
		state.counters["HashRate"] = benchmark::Counter( state.iterations(), benchmark::Counter::kIsRate );
		state.counters["ns_per_char"] = benchmark::Counter(
			state.iterations() * length,
			benchmark::Counter::kIsRate | benchmark::Counter::kInvert );
	}

	//=====================================================================
	// Hash function benchmarks - comparative Analysis
	//=====================================================================

	/* Compare all three benchmark strings in one test */
	static void BM_hash_BenchmarkTriple( benchmark::State& state )
	{
		const std::array<std::string_view, 3> keys = { "Type", "Detail", "Quantity" };
		size_t keyIndex = 0;

		for ( auto _ : state )
		{
			uint32_t hash = ChdDictionary<std::string_view>::hash( keys[keyIndex] );
			benchmark::DoNotOptimize( hash );
			keyIndex = ( keyIndex + 1 ) % 3; /* Cycle through keys */
		}

		state.counters["HashRate"] = benchmark::Counter( state.iterations(), benchmark::Counter::kIsRate );
	}

	/* Test hash distribution quality (for hash function validation) */
	static void BM_hash_Distribution( benchmark::State& state )
	{
		/* Use a set of similar strings to test hash distribution */
		const std::array<std::string_view, 8> keys = {
			"Type", "Type1", "Type2", "Type3",
			"Detail", "Detail1", "Detail2", "Detail3" };

		size_t keyIndex = 0;
		std::vector<uint32_t> hashes;
		hashes.reserve( keys.size() );

		for ( auto _ : state )
		{
			uint32_t hash = ChdDictionary<std::string_view>::hash( keys[keyIndex] );
			benchmark::DoNotOptimize( hash );

			if ( state.thread_index() == 0 && keyIndex < keys.size() )
			{
				hashes.push_back( hash );
			}

			keyIndex = ( keyIndex + 1 ) % keys.size();
		}

		state.counters["HashRate"] = benchmark::Counter( state.iterations(), benchmark::Counter::kIsRate );
	}

	//=====================================================================
	// Benchmark registrations
	//=====================================================================

	/* Individual codebook string benchmarks */
	BENCHMARK( BM_hash_Type )->MinTime( 5.0 )->Unit( benchmark::kNanosecond );
	BENCHMARK( BM_hash_Detail )->MinTime( 5.0 )->Unit( benchmark::kNanosecond );
	BENCHMARK( BM_hash_Quantity )->MinTime( 5.0 )->Unit( benchmark::kNanosecond );
	BENCHMARK( BM_hash_Position )->MinTime( 5.0 )->Unit( benchmark::kNanosecond );
	BENCHMARK( BM_hash_State )->MinTime( 5.0 )->Unit( benchmark::kNanosecond );
	BENCHMARK( BM_hash_Content )->MinTime( 5.0 )->Unit( benchmark::kNanosecond );
	BENCHMARK( BM_hash_Command )->MinTime( 5.0 )->Unit( benchmark::kNanosecond );
	BENCHMARK( BM_hash_Calculation )->MinTime( 5.0 )->Unit( benchmark::kNanosecond );
	BENCHMARK( BM_hash_ActivityType )->MinTime( 5.0 )->Unit( benchmark::kNanosecond );
	BENCHMARK( BM_hash_FunctionalServices )->MinTime( 5.0 )->Unit( benchmark::kNanosecond );
	BENCHMARK( BM_hash_MaintenanceCategory )->MinTime( 5.0 )->Unit( benchmark::kNanosecond );

	/* String length analysis */
	BENCHMARK( BM_hash_ByLength )
		->Arg( 1 )
		->Arg( 2 )
		->Arg( 3 )
		->Arg( 4 )
		->Arg( 5 )
		->Arg( 6 )
		->Arg( 7 )
		->Arg( 8 )
		->Arg( 10 )
		->Arg( 12 )
		->Arg( 16 )
		->Arg( 20 )
		->Arg( 24 )
		->Arg( 32 )
		->MinTime( 3.0 )
		->Unit( benchmark::kNanosecond );

	/* Comparative benchmarks */
	BENCHMARK( BM_hash_BenchmarkTriple )->MinTime( 5.0 )->Unit( benchmark::kNanosecond );
	BENCHMARK( BM_hash_Distribution )->MinTime( 3.0 )->Unit( benchmark::kNanosecond );
}

BENCHMARK_MAIN();
