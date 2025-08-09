/**
 * @file TESTS_Int128.cpp
 * @brief Comprehensive tests for Int128 128-bit signed integer class
 * @details Tests covering construction, arithmetic, comparison, and edge cases
 */

#include "dnv/vista/sdk/DataTypes/Int128.h"

using namespace dnv::vista::sdk::datatypes;

namespace
{
	/**
	 * @brief Test fixture for Int128 operations
	 */
	class Int128Test : public ::testing::Test
	{
	protected:
		void SetUp() override {}
		void TearDown() override {}
	};

	//----------------------------------------------
	// Construction Tests
	//----------------------------------------------

	TEST_F( Int128Test, DefaultConstruction )
	{
		Int128 zero;
		EXPECT_TRUE( zero.isZero() );
		EXPECT_FALSE( zero.isNegative() );
		EXPECT_EQ( 0ULL, zero.toLow() );
		EXPECT_EQ( 0ULL, zero.toHigh() );
	}

	TEST_F( Int128Test, ConstructionFromUint64 )
	{
		const std::uint64_t value = 0x123456789ABCDEFULL;
		Int128 num( value );

		EXPECT_FALSE( num.isZero() );
		EXPECT_FALSE( num.isNegative() );
		EXPECT_EQ( value, num.toLow() );
		EXPECT_EQ( 0ULL, num.toHigh() );
	}

	TEST_F( Int128Test, ConstructionFromInt64Positive )
	{
		const std::int64_t value = 0x123456789ABCDEFLL;
		Int128 num( value );

		EXPECT_FALSE( num.isZero() );
		EXPECT_FALSE( num.isNegative() );
		EXPECT_EQ( static_cast<std::uint64_t>( value ), num.toLow() );
		EXPECT_EQ( 0ULL, num.toHigh() );
	}

	TEST_F( Int128Test, ConstructionFromInt64Negative )
	{
		const std::int64_t value = -0x123456789ABCDEFLL;
		Int128 num( value );

		EXPECT_FALSE( num.isZero() );
		EXPECT_TRUE( num.isNegative() );
		EXPECT_EQ( static_cast<std::uint64_t>( value ), num.toLow() );
		EXPECT_EQ( 0xFFFFFFFFFFFFFFFFULL, num.toHigh() ); /* Sign extension */
	}

	TEST_F( Int128Test, ConstructionFromUint32 )
	{
		const std::uint32_t value = 0x12345678U;
		Int128 num( value );

		EXPECT_FALSE( num.isZero() );
		EXPECT_FALSE( num.isNegative() );
		EXPECT_EQ( static_cast<std::uint64_t>( value ), num.toLow() );
		EXPECT_EQ( 0ULL, num.toHigh() );
	}

	TEST_F( Int128Test, ConstructionFromIntPositive )
	{
		const int value = 0x12345678;
		Int128 num( value );

		EXPECT_FALSE( num.isZero() );
		EXPECT_FALSE( num.isNegative() );
		EXPECT_EQ( static_cast<std::uint64_t>( value ), num.toLow() );
		EXPECT_EQ( 0ULL, num.toHigh() );
	}

	TEST_F( Int128Test, ConstructionFromIntNegative )
	{
		const int value = -0x12345678;
		Int128 num( value );

		EXPECT_FALSE( num.isZero() );
		EXPECT_TRUE( num.isNegative() );
		EXPECT_EQ( static_cast<std::uint64_t>( value ), num.toLow() );
		/* Sign extension */
		EXPECT_EQ( 0xFFFFFFFFFFFFFFFFULL, num.toHigh() );
	}

	TEST_F( Int128Test, ConstructionFromLowHigh )
	{
		const std::uint64_t low = 0x123456789ABCDEFULL;
		const std::uint64_t high = 0xFEDCBA9876543210ULL;
		Int128 num( low, high );

		EXPECT_FALSE( num.isZero() );
		/* High bit set */
		EXPECT_TRUE( num.isNegative() );
		EXPECT_EQ( low, num.toLow() );
		EXPECT_EQ( high, num.toHigh() );
	}

	//----------------------------------------------
	// Arithmetic Tests
	//----------------------------------------------

	TEST_F( Int128Test, Addition )
	{
		Int128 a( 100 );
		Int128 b( 200 );
		Int128 result = a + b;

		EXPECT_EQ( 300ULL, result.toLow() );
		EXPECT_EQ( 0ULL, result.toHigh() );
	}

	TEST_F( Int128Test, AdditionWithCarry )
	{
		/* Test overflow from low to high word */
		Int128 a( 0xFFFFFFFFFFFFFFFFULL, 0 );
		Int128 b( 1 );
		Int128 result = a + b;

		EXPECT_EQ( 0ULL, result.toLow() );
		EXPECT_EQ( 1ULL, result.toHigh() );
	}

	TEST_F( Int128Test, Subtraction )
	{
		Int128 a( 300 );
		Int128 b( 100 );
		Int128 result = a - b;

		EXPECT_EQ( 200ULL, result.toLow() );
		EXPECT_EQ( 0ULL, result.toHigh() );
	}

	TEST_F( Int128Test, SubtractionWithBorrow )
	{
		/* Test borrow from high to low word */
		Int128 a( 0, 1 );
		Int128 b( 1 );
		Int128 result = a - b;

		EXPECT_EQ( 0xFFFFFFFFFFFFFFFFULL, result.toLow() );
		EXPECT_EQ( 0ULL, result.toHigh() );
	}

	TEST_F( Int128Test, Multiplication )
	{
		Int128 a( 123 );
		Int128 b( 456 );
		Int128 result = a * b;

		EXPECT_EQ( 123ULL * 456ULL, result.toLow() );
		EXPECT_EQ( 0ULL, result.toHigh() );
	}

	TEST_F( Int128Test, MultiplicationLarge )
	{
		/* Test multiplication that requires high word */
		const std::uint64_t a_val = 0x123456789ABCDEFULL;
		/* 2^32 */
		const std::uint64_t b_val = 0x100000000ULL;

		Int128 a( a_val );
		Int128 b( b_val );
		Int128 result = a * b;

		/* Result should be a_val shifted left by 32 bits */
		EXPECT_EQ( ( a_val << 32 ) & 0xFFFFFFFFFFFFFFFFULL, result.toLow() );
		EXPECT_EQ( a_val >> 32, result.toHigh() );
	}

	TEST_F( Int128Test, Division )
	{
		Int128 a( 456 );
		Int128 b( 123 );
		Int128 result = a / b;

		EXPECT_EQ( 3ULL, result.toLow() );
		EXPECT_EQ( 0ULL, result.toHigh() );
	}

	TEST_F( Int128Test, DivisionByZero )
	{
		Int128 a( 123 );
		Int128 zero;

		EXPECT_THROW( a / zero, std::overflow_error );
	}

	TEST_F( Int128Test, Modulo )
	{
		Int128 a( 456 );
		Int128 b( 123 );
		Int128 result = a % b;

		/* 456 % 123 = 87 */
		EXPECT_EQ( 87ULL, result.toLow() );
		EXPECT_EQ( 0ULL, result.toHigh() );
	}

	TEST_F( Int128Test, ModuloByZero )
	{
		Int128 a( 123 );
		Int128 zero;

		EXPECT_THROW( a % zero, std::overflow_error );
	}

	TEST_F( Int128Test, UnaryMinus )
	{
		Int128 positive( 123 );
		Int128 negative = -positive;

		EXPECT_TRUE( negative.isNegative() );
		EXPECT_FALSE( negative.isZero() );

		/* Two's complement: ~123 + 1 */
		EXPECT_EQ( static_cast<std::uint64_t>( -123 ), negative.toLow() );
		EXPECT_EQ( 0xFFFFFFFFFFFFFFFFULL, negative.toHigh() );
	}

	TEST_F( Int128Test, UnaryMinusZero )
	{
		Int128 zero;
		Int128 negated = -zero;

		EXPECT_TRUE( negated.isZero() );
		EXPECT_FALSE( negated.isNegative() );
	}

	//----------------------------------------------
	// Comparison Tests
	//----------------------------------------------

	TEST_F( Int128Test, Equality )
	{
		Int128 a( 123 );
		Int128 b( 123 );
		Int128 c( 456 );

		EXPECT_TRUE( a == b );
		EXPECT_FALSE( a == c );
	}

	TEST_F( Int128Test, Inequality )
	{
		Int128 a( 123 );
		Int128 b( 123 );
		Int128 c( 456 );

		EXPECT_FALSE( a != b );
		EXPECT_TRUE( a != c );
	}

	TEST_F( Int128Test, LessThan )
	{
		Int128 a( 123 );
		Int128 b( 456 );

		EXPECT_TRUE( a < b );
		EXPECT_FALSE( b < a );
		EXPECT_FALSE( a < a );
	}

	TEST_F( Int128Test, LessThanSignedComparison )
	{
		Int128 positive( 123 );
		Int128 negative( -456 );

		EXPECT_TRUE( negative < positive );
		EXPECT_FALSE( positive < negative );
	}

	TEST_F( Int128Test, LessEqual )
	{
		Int128 a( 123 );
		Int128 b( 456 );
		Int128 c( 123 );

		EXPECT_TRUE( a <= b );
		EXPECT_TRUE( a <= c );
		EXPECT_FALSE( b <= a );
	}

	TEST_F( Int128Test, GreaterThan )
	{
		Int128 a( 123 );
		Int128 b( 456 );

		EXPECT_FALSE( a > b );
		EXPECT_TRUE( b > a );
		EXPECT_FALSE( a > a );
	}

	TEST_F( Int128Test, GreaterEqual )
	{
		Int128 a( 123 );
		Int128 b( 456 );
		Int128 c( 123 );

		EXPECT_FALSE( a >= b );
		EXPECT_TRUE( a >= c );
		EXPECT_TRUE( b >= a );
	}

	TEST_F( Int128Test, ComparisonHighWord )
	{
		/* Test comparison when high words differ */
		Int128 a( 0xFFFFFFFFFFFFFFFFULL, 0 );
		Int128 b( 0, 1 );

		EXPECT_TRUE( a < b );
		EXPECT_FALSE( b < a );
	}

	//----------------------------------------------
	// State Checking Tests
	//----------------------------------------------

	TEST_F( Int128Test, IsZero )
	{
		Int128 zero;
		Int128 nonZero( 1 );

		EXPECT_TRUE( zero.isZero() );
		EXPECT_FALSE( nonZero.isZero() );
	}

	TEST_F( Int128Test, IsNegative )
	{
		Int128 positive( 123 );
		Int128 negative( -123 );
		Int128 zero;

		EXPECT_FALSE( positive.isNegative() );
		EXPECT_TRUE( negative.isNegative() );
		EXPECT_FALSE( zero.isNegative() );
	}

	TEST_F( Int128Test, IsNegativeHighBit )
	{
		/* Test negativity based on high bit */
		Int128 negative( 0, 0x8000000000000000ULL );
		Int128 positive( 0, 0x7FFFFFFFFFFFFFFFULL );

		EXPECT_TRUE( negative.isNegative() );
		EXPECT_FALSE( positive.isNegative() );
	}

	//----------------------------------------------
	// Mathematical Operation Tests
	//----------------------------------------------

	TEST_F( Int128Test, AbsoluteValue )
	{
		Int128 positive( 123 );
		Int128 negative( -123 );
		Int128 zero;

		EXPECT_EQ( positive, positive.abs() );
		EXPECT_EQ( positive, negative.abs() );
		EXPECT_EQ( zero, zero.abs() );
	}

	//----------------------------------------------
	// Edge Case Tests
	//----------------------------------------------

	TEST_F( Int128Test, MaxValues )
	{
		/* Test with maximum possible values */
		Int128 maxVal( 0xFFFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL );

		EXPECT_FALSE( maxVal.isZero() );
		EXPECT_FALSE( maxVal.isNegative() );
		EXPECT_EQ( 0xFFFFFFFFFFFFFFFFULL, maxVal.toLow() );
		EXPECT_EQ( 0x7FFFFFFFFFFFFFFFULL, maxVal.toHigh() );
	}

	TEST_F( Int128Test, MinValue )
	{
		/* Test with minimum possible value (most negative) */
		Int128 minVal( 0, 0x8000000000000000ULL );

		EXPECT_FALSE( minVal.isZero() );
		EXPECT_TRUE( minVal.isNegative() );
		EXPECT_EQ( 0ULL, minVal.toLow() );
		EXPECT_EQ( 0x8000000000000000ULL, minVal.toHigh() );
	}

	TEST_F( Int128Test, OverflowAddition )
	{
		/* Test addition overflow */
		Int128 maxPositive( 0xFFFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL );
		Int128 one( 1 );
		Int128 result = maxPositive + one;

		/* Should wrap to minimum negative value */
		EXPECT_TRUE( result.isNegative() );
		EXPECT_EQ( 0ULL, result.toLow() );
		EXPECT_EQ( 0x8000000000000000ULL, result.toHigh() );
	}
}
