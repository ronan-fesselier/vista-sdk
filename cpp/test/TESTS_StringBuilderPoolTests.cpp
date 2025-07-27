/**
 * @file TESTS_StringBuilderPoolTests.cpp
 * @brief StringBuilderPool unit tests
 * @details Validates pooled string building functionality and resource management
 */

#include "pch.h"

#include "dnv/vista/sdk/utils/StringBuilderPool.h"

namespace dnv::vista::sdk::utils
{
	TEST( StringBuilderPoolTests, Test_Roundtrip )
	{
		auto lease = StringBuilderPool::instance();

		lease.Builder().append( "1" );

		ASSERT_EQ( "1", lease.toString() );
	}

	TEST( StringBuilderPoolTests, Test_Use_After_Move_Throws_Exception )
	{
		auto lease = StringBuilderPool::instance();
		auto movedLease = std::move( lease );

		ASSERT_THROW( [[maybe_unused]] auto result = lease.toString(), std::runtime_error );
		ASSERT_THROW( [[maybe_unused]] auto builder = lease.Builder(), std::runtime_error );

		ASSERT_NO_THROW( [[maybe_unused]] auto validBuilder = movedLease.Builder() );
	}

	TEST( StringBuilderPoolTests, Test_Builder_Is_Cleaned )
	{
		{
			auto lease = StringBuilderPool::instance();
			auto builder = lease.Builder();
			builder.append( "a" );
			ASSERT_EQ( 1U, builder.length() );
		}

		{
			auto newLease = StringBuilderPool::instance();
			auto newBuilder = newLease.Builder();

			ASSERT_EQ( 0U, newBuilder.length() );
		}
	}
}
