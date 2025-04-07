#include "pch.h"

#include "dnv/vista/sdk/VIS.h"
#include "dnv/vista/sdk/VisVersion.h"
#include "dnv/vista/sdk/Gmod.h"
#include "dnv/vista/sdk/Codebooks.h"
#include "dnv/vista/sdk/GmodPath.h"
#include "dnv/vista/sdk/LocalId.h"
#include "dnv/vista/sdk/LocalIdBuilder.h"
#include "dnv/vista/sdk/MetadataTag.h"
#include "dnv/vista/sdk/ParsingErrors.h"
#include <gtest/gtest.h>

namespace dnv::vista::sdk::tests
{
	struct Input
	{
		std::string PrimaryItem;
		std::optional<std::string> SecondaryItem;
		std::optional<std::string> Quantity;
		std::optional<std::string> Content;
		std::optional<std::string> Position;
		VisVersion visVersion = VisVersion::v3_4a;
		bool Verbose = false;

		Input( const std::string& primaryItem,
			const std::optional<std::string>& secondaryItem = std::nullopt,
			const std::optional<std::string>& quantity = std::nullopt,
			const std::optional<std::string>& content = std::nullopt,
			const std::optional<std::string>& position = std::nullopt,
			VisVersion visVersion = VisVersion::v3_4a,
			bool verbose = false )
			: PrimaryItem( primaryItem ),
			  SecondaryItem( secondaryItem ),
			  Quantity( quantity ),
			  Content( content ),
			  Position( position ),
			  visVersion( visVersion ),
			  Verbose( verbose )
		{
		}
	};

	class LocalIdTests : public ::testing::TestWithParam<std::pair<Input, std::string>>
	{
	public:
		static std::vector<std::pair<Input, std::string>> GetValidTestData()
		{
			return {
				{ Input( "411.1/C101.31-2" ), "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta" },
				{ Input( "411.1/C101.31-2", std::nullopt, "temperature", "exhaust.gas", "inlet" ),
					"/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet" },
				{ Input( "411.1/C101.63/S206", std::nullopt, "temperature", "exhaust.gas", "inlet", VisVersion::v3_4a, true ),
					"/dnv-v2/vis-3-4a/411.1/C101.63/S206/~propulsion.engine/~cooling.system/meta/qty-temperature/cnt-exhaust.gas/pos-inlet" },
				{ Input( "411.1/C101.63/S206", "411.1/C101.31-5", "temperature", "exhaust.gas", "inlet", VisVersion::v3_4a, true ),
					"/dnv-v2/vis-3-4a/411.1/C101.63/S206/sec/411.1/C101.31-5/~propulsion.engine/~cooling.system/~for.propulsion.engine/~cylinder.5/meta/qty-temperature/cnt-exhaust.gas/pos-inlet" },
				{ Input( "511.11/C101.67/S208", std::nullopt, "pressure", "starting.air", "inlet", VisVersion::v3_6a, true ),
					"/dnv-v2/vis-3-6a/511.11/C101.67/S208/~main.generator.engine/~starting.system.pneumatic/meta/qty-pressure/cnt-starting.air/pos-inlet" } };
		}

	protected:
		std::pair<VIS, Gmod> GetVisAndGmod( VisVersion visVersion )
		{
			VIS vis = VIS::Instance();
			Gmod gmod = vis.GetGmod( visVersion );

			return { vis, gmod };
		}
	};

	GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST( LocalIdTests );

	TEST_F( LocalIdTests, Test_Parsing_Single_Valid )
	{
		try
		{
			auto vis = VIS::Instance();

			SPDLOG_INFO( "VIS instance created successfully." );

			std::string localIdStr = "/dnv-v2/vis-3-4a/valid_item/meta/cnt-refrigerant/state-leaking";
			SPDLOG_INFO( "Testing parse of: {}", localIdStr );

			std::optional<LocalIdBuilder> parsedLocalId;
			bool success = LocalIdBuilder::TryParse( localIdStr, parsedLocalId );

			SPDLOG_INFO( "Parse result: {}", success ? "success" : "failure" );

			EXPECT_FALSE( success ) << "Parsing should fail due to empty GMOD";
			EXPECT_FALSE( parsedLocalId.has_value() ) << "Parsed LocalId should be null";
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Exception caught: {}", e.what() );
			FAIL() << "Exception thrown: " << e.what();
		}
		catch ( ... )
		{
			SPDLOG_ERROR( "Unknown exception caught" );
			FAIL() << "Unknown exception thrown";
		}
	}

	TEST_F( LocalIdTests, Test_Parsing_InvalidNamingRule )
	{
		try
		{
			std::string localIdStr = "/invalid-naming/vis-3-4a/400a/meta/cnt-refrigerant/state-leaking";
			SPDLOG_INFO( "Testing parse of: {}", localIdStr );

			std::optional<LocalIdBuilder> parsedLocalId;
			bool success = LocalIdBuilder::TryParse( localIdStr, parsedLocalId );

			SPDLOG_INFO( "Parse result: {}", success ? "success" : "failure" );

			EXPECT_FALSE( success ) << "Parsing should fail due to invalid naming rule";
			EXPECT_FALSE( parsedLocalId.has_value() ) << "Parsed LocalId should be null";
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Exception caught: {}", e.what() );
			FAIL() << "Exception thrown: " << e.what();
		}
		catch ( ... )
		{
			SPDLOG_ERROR( "Unknown exception caught" );
			FAIL() << "Unknown exception thrown";
		}
	}

	TEST_F( LocalIdTests, Test_Parsing_InvalidVisVersion )
	{
		try
		{
			std::string localIdStr = "/dnv-v2/vis-invalid/400a/meta/cnt-refrigerant/state-leaking";
			SPDLOG_INFO( "Testing parse of: {}", localIdStr );

			std::optional<LocalIdBuilder> parsedLocalId;
			bool success = LocalIdBuilder::TryParse( localIdStr, parsedLocalId );

			SPDLOG_INFO( "Parse result: {}", success ? "success" : "failure" );

			EXPECT_FALSE( success ) << "Parsing should fail due to invalid VIS version";
			EXPECT_FALSE( parsedLocalId.has_value() ) << "Parsed LocalId should be null";
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Exception caught: {}", e.what() );
			FAIL() << "Exception thrown: " << e.what();
		}
		catch ( ... )
		{
			SPDLOG_ERROR( "Unknown exception caught" );
			FAIL() << "Unknown exception thrown";
		}
	}

	TEST_F( LocalIdTests, Test_Parsing_EmptyInput )
	{
		try
		{
			std::string localIdStr = "";
			SPDLOG_INFO( "Testing parse of: {}", localIdStr );

			std::optional<LocalIdBuilder> parsedLocalId;
			bool success = LocalIdBuilder::TryParse( localIdStr, parsedLocalId );

			SPDLOG_INFO( "Parse result: {}", success ? "success" : "failure" );

			EXPECT_FALSE( success ) << "Parsing should fail due to empty input";
			EXPECT_FALSE( parsedLocalId.has_value() ) << "Parsed LocalId should be null";
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Exception caught: {}", e.what() );
			FAIL() << "Exception thrown: " << e.what();
		}
		catch ( ... )
		{
			SPDLOG_ERROR( "Unknown exception caught" );
			FAIL() << "Unknown exception thrown";
		}
	}
	TEST_F( LocalIdTests, Test_Parsing )
	{
		std::vector<std::string> testCases = {
			"/dnv-v2/vis-3-4a/1031/meta/cnt-refrigerant/state-leaking",
			"/dnv-v2/vis-3-4a/1021.1i-6P/H123/meta/qty-volume/cnt-cargo/pos~percentage",
			"/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
			"/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
			"/dnv-v2/vis-3-4a/411.1/C101.63/S206/~propulsion.engine/~cooling.system/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
			"/dnv-v2/vis-3-4a/411.1/C101.63/S206/sec/411.1/C101.31-5/~propulsion.engine/~cooling.system/~for.propulsion.engine/~cylinder.5/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
			"/dnv-v2/vis-3-4a/511.11-21O/C101.67/S208/meta/qty-pressure/cnt-air/state-low" };

		for ( const auto& localIdStr : testCases )
		{
			try
			{
				std::optional<LocalIdBuilder> parsedLocalId;
				bool success = LocalIdBuilder::TryParse( localIdStr, parsedLocalId );

				EXPECT_TRUE( success ) << "Failed to parse: " << localIdStr;
				EXPECT_TRUE( parsedLocalId.has_value() ) << "Parsed LocalId is null for: " << localIdStr;

				if ( parsedLocalId.has_value() )
				{
					std::string roundTrip = parsedLocalId->ToString();
					EXPECT_EQ( localIdStr, roundTrip );
				}
			}
			catch ( const std::exception& e )
			{
				FAIL() << "Exception thrown while parsing '" << localIdStr << "': " << e.what();
			}
			catch ( ... )
			{
				FAIL() << "Unknown exception thrown while parsing '" << localIdStr << "'";
			}
		}
	}

	TEST_F( LocalIdTests, Test_LocalId_Build_AllWithout )
	{
		try
		{
			auto [vis, gmod] = GetVisAndGmod( VisVersion::v3_4a );

			ASSERT_FALSE( gmod.GetRootNode().GetChildren().empty() ) << "GMOD data not loaded properly.";

			std::optional<GmodPath> optionalPrimaryItem;
			std::optional<GmodPath> optionalSecondaryItem;

			ASSERT_TRUE( gmod.TryParsePath( "411.1", optionalPrimaryItem ) )
				<< "Failed to parse primary item path.";
			ASSERT_TRUE( gmod.TryParsePath( "411", optionalSecondaryItem ) )
				<< "Failed to parse secondary item path.";

			ASSERT_TRUE( optionalPrimaryItem.has_value() ) << "Primary item is null";
			ASSERT_TRUE( optionalSecondaryItem.has_value() ) << "Secondary item is null";

			LocalIdBuilder localId = LocalIdBuilder::Create( VisVersion::v3_4a )
										 .WithPrimaryItem( *optionalPrimaryItem )
										 .TryWithSecondaryItem( optionalSecondaryItem )
										 .WithVerboseMode( true );

			EXPECT_TRUE( localId.IsValid() ) << "LocalId is not valid after building with all components.";

			LocalIdBuilder allWithout = localId
											.WithoutPrimaryItem()
											.WithoutSecondaryItem();

			EXPECT_TRUE( allWithout.IsEmpty() ) << "LocalId is not empty after removing all components.";
			EXPECT_FALSE( allWithout.IsValid() ) << "LocalId should not be valid after removing all components.";
			EXPECT_EQ( allWithout.ToString(), "" ) << "LocalId string representation should be empty.";
		}
		catch ( const std::exception& e )
		{
			std::cerr << "Exception caught: " << e.what() << std::endl;
			FAIL() << "Exception thrown: " << e.what();
		}
		catch ( ... )
		{
			std::cerr << "Unknown exception caught" << std::endl;
			FAIL() << "Unknown exception thrown";
		}
	}

	INSTANTIATE_TEST_SUITE_P(
		ValidLocalIds,
		LocalIdTests,
		::testing::ValuesIn( LocalIdTests::GetValidTestData() ) );
}
