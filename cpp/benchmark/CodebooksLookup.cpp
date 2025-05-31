/**
 * @file CodebooksLookup.cpp
 * @brief Vista SDK codebook access performance comparison
 *
 * Compares three approaches for accessing Vista codebooks:
 * - Cached hash table for flexible lookups
 * - Pre-cached array for known, fixed lookups
 * - Direct SDK API for dynamic access
 */

#include "pch.h"

#include "dnv/vista/sdk/VIS.h"

using namespace dnv::vista::sdk;

namespace dnv::vista::sdk::benchmarks
{
	//=====================================================================
	// Codebooks lookup benchmark
	//=====================================================================

	class CodebooksLookup
	{
	private:
		//----------------------------------------------
		// Private member variables
		//----------------------------------------------

		std::unordered_map<CodebookName, Codebook> m_hashTableCodebooks;
		std::optional<std::reference_wrapper<const dnv::vista::sdk::Codebooks>> m_codebooks;
		std::array<const Codebook*, 3> m_arrayCodebooks;

		//----------------------------------------------
		// Helper methods
		//----------------------------------------------

		template <typename T>
		bool TryGetValue( const std::unordered_map<CodebookName, T>& map, CodebookName key ) const
		{
			return map.find( key ) != map.end();
		}

	public:
		//----------------------------------------------
		// Setup
		//----------------------------------------------

		void Setup()
		{
			auto& vis = VIS::instance();

			m_codebooks = std::cref( vis.codebooks( VisVersion::v3_7a ) );
			m_hashTableCodebooks.clear();

			for ( size_t i = 1; i <= NUM_CODEBOOKS; ++i )
			{
				CodebookName name = static_cast<CodebookName>( i );
				const Codebook& codebook = m_codebooks->get()[name];
				m_hashTableCodebooks[name] = codebook;
			}

			const auto& codebooks_ref = m_codebooks->get();
			m_arrayCodebooks[0] = &codebooks_ref[CodebookName::Quantity];
			m_arrayCodebooks[1] = &codebooks_ref[CodebookName::Type];
			m_arrayCodebooks[2] = &codebooks_ref[CodebookName::Detail];
		}

		//----------------------------------------------
		// Benchmarks
		//----------------------------------------------
		//-----------------------------
		// HashTableLookup
		//-----------------------------

		bool HashTableLookup()
		{
			return TryGetValue( m_hashTableCodebooks, CodebookName::Quantity ) &&
				   TryGetValue( m_hashTableCodebooks, CodebookName::Type ) &&
				   TryGetValue( m_hashTableCodebooks, CodebookName::Detail );
		}

		//-----------------------------
		// SdkApiCodebooks
		//-----------------------------

		bool SdkApiCodebooks()
		{
			const auto& codebooks_ref = m_codebooks->get();
			const Codebook* a = nullptr;
			const Codebook* b = nullptr;
			const Codebook* c = nullptr;

			try
			{
				a = &codebooks_ref[CodebookName::Quantity];
				b = &codebooks_ref[CodebookName::Type];
				c = &codebooks_ref[CodebookName::Detail];
			}
			catch ( ... )
			{
			}

			return ( a != nullptr ) && ( b != nullptr ) && ( c != nullptr );
		}

		//-----------------------------
		// SdkApiArrayLookup
		//-----------------------------

		bool SdkApiArrayLookup()
		{
			return ( m_arrayCodebooks[0] != nullptr ) &&
				   ( m_arrayCodebooks[1] != nullptr ) &&
				   ( m_arrayCodebooks[2] != nullptr );
		}
	};

	//=====================================================================
	// Benchmark wrappers
	//=====================================================================

	static CodebooksLookup g_benchmarkInstance;

	static void BM_Setup( benchmark::State& state )
	{
		if ( state.thread_index() == 0 )
		{
			g_benchmarkInstance.Setup();
		}
	}

	//----------------------------------------------
	// HashTableLookup
	//----------------------------------------------

	static void BM_HashTableLookup( benchmark::State& state )
	{
		BM_Setup( state );

		for ( auto _ : state )
		{
			bool result = g_benchmarkInstance.HashTableLookup();
			benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// SdkApiCodebooks
	//----------------------------------------------

	static void BM_SdkApiCodebooks( benchmark::State& state )
	{
		BM_Setup( state );

		for ( auto _ : state )
		{
			bool result = g_benchmarkInstance.SdkApiCodebooks();
			benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// SdkApiArrayLookup
	//----------------------------------------------

	static void BM_SdkApiArrayLookup( benchmark::State& state )
	{
		BM_Setup( state );

		for ( auto _ : state )
		{
			bool result = g_benchmarkInstance.SdkApiArrayLookup();
			benchmark::DoNotOptimize( result );
		}
	}

	//=====================================================================
	// Benchmark registrations
	//=====================================================================

	BENCHMARK( BM_HashTableLookup )->MinTime( 10.0 );
	BENCHMARK( BM_SdkApiCodebooks )->MinTime( 10.0 );
	BENCHMARK( BM_SdkApiArrayLookup )->MinTime( 10.0 );
}

BENCHMARK_MAIN();
