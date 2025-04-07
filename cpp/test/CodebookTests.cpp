#include "pch.h"

#include "dnv/vista/sdk/VIS.h"
#include "dnv/vista/sdk/Codebooks.h"
#include "dnv/vista/sdk/Codebook.h"
#include "dnv/vista/sdk/VisVersion.h"

namespace dnv::vista::sdk
{
	TEST( CodebookTests, Test_Position_Validation )
	{
		VIS vis;
		try
		{
			VIS vis;
			auto codebooks = vis.GetCodebooks( VisVersion::v3_8a );
			const auto& codebookType = codebooks[CodebookName::Position];
			if ( !codebookType.HasStandardValue( "upper" ) )
			{
				FAIL() << "Codebook does not contain the expected standard value.";
			}
			auto validPosition = codebookType.ValidatePosition( "upper" );
			auto expectedOutput = PositionValidationResults::FromString( "Valid" );

			EXPECT_EQ( validPosition, expectedOutput );
		}
		catch ( const std::exception& e )
		{
			std::cerr << "Exception: " << e.what() << std::endl;
			FAIL();
		}
	}

	TEST( CodebookTests, Test_Positions )
	{
		VIS vis;
		auto codebooks = vis.GetCodebooks( VisVersion::v3_8a );

		const auto& positions = codebooks[CodebookName::Position];

		EXPECT_FALSE( positions.HasStandardValue( "invalidValue" ) );
		EXPECT_TRUE( positions.HasStandardValue( "upper" ) );
	}

	TEST( CodebookTests, Test_Standard_Values )
	{
		VIS vis;
		auto codebooks = vis.GetCodebooks( VisVersion::v3_8a );

		const auto& positions = codebooks[CodebookName::Position];

		EXPECT_TRUE( positions.HasStandardValue( "upper" ) );
		const auto& rawData = positions.GetRawData();
		EXPECT_TRUE( rawData.find( "Vertical" ) != rawData.end() );
		EXPECT_NE( std::find( rawData.at( "Vertical" ).begin(), rawData.at( "Vertical" ).end(), "upper" ), rawData.at( "Vertical" ).end() );
	}

	TEST( CodebookTests, Test_States )
	{
		VIS vis;
		auto codebooks = vis.GetCodebooks( VisVersion::v3_8a );

		const auto& states = codebooks[CodebookName::State];
		EXPECT_FALSE( states.HasGroup( "invalidGroup" ) );
		EXPECT_TRUE( states.HasStandardValue( "validValue" ) );
		EXPECT_TRUE( states.HasGroup( "validGroup" ) );
		EXPECT_TRUE( states.HasStandardValue( "secondValidValue" ) );
	}

	TEST( CodebookTests, Test_Create_Tag )
	{
		VIS vis;
		auto codebooks = vis.GetCodebooks( VisVersion::v3_8a );

		const auto& codebookType = codebooks[CodebookName::Position];

		auto metadataTag1 = codebookType.CreateTag( "firstTag" );
		EXPECT_EQ( metadataTag1.GetValue(), "firstTag" );
		EXPECT_FALSE( metadataTag1.IsCustom() );

		auto metadataTag2 = codebookType.CreateTag( "secondTag" );
		EXPECT_EQ( metadataTag2.GetValue(), "secondTag" );
		EXPECT_FALSE( metadataTag2.IsCustom() );

		EXPECT_THROW( codebookType.CreateTag( "invalidTag" ), std::invalid_argument );
		EXPECT_EQ( codebookType.TryCreateTag( "invalidTag" ), std::nullopt );
	}

	TEST( CodebookTests, Test_Get_Groups )
	{
		VIS vis;
		auto codebooks = vis.GetCodebooks( VisVersion::v3_8a );

		const auto& groups = codebooks[CodebookName::Position].GetGroups();
		EXPECT_GT( groups.Count(), 1 );
		EXPECT_TRUE( groups.Contains( "Vertical" ) );

		const auto& rawData = codebooks[CodebookName::Position].GetRawData();
		EXPECT_EQ( groups.Count(), rawData.size() - 1 );
		EXPECT_TRUE( rawData.find( "Vertical" ) != rawData.end() );
	}

	TEST( CodebookTests, Test_Iterate_Groups )
	{
		VIS vis;
		auto codebooks = vis.GetCodebooks( VisVersion::v3_8a );

		const auto& groups = codebooks[CodebookName::Position].GetGroups();
		int count = 0;
		for ( const auto& group : groups )
		{
			(void)group;
			count++;
		}

		EXPECT_EQ( count, 11 );
	}

	TEST( CodebookTests, Test_Iterate_Values )
	{
		VIS vis;
		auto codebooks = vis.GetCodebooks( VisVersion::v3_8a );

		const auto& values = codebooks[CodebookName::Position].GetStandardValues();
		int count = 0;
		for ( const auto& value : values )
		{
			(void)value;
			count++;
		}

		EXPECT_EQ( count, 28 );
	}

	TEST( CodebookTests, Test_Detail_Tag )
	{
		VIS vis;
		auto codebooks = vis.GetCodebooks( VisVersion::v3_8a );

		const auto& codebook = codebooks[CodebookName::Detail];
		EXPECT_TRUE( codebook.TryCreateTag( "validCustomTag" ).has_value() );
		EXPECT_EQ( codebook.TryCreateTag( "invalidCustomTag" ), std::nullopt );
		EXPECT_THROW( codebook.CreateTag( "invalidCustomTag" ), std::invalid_argument );
	}
}
