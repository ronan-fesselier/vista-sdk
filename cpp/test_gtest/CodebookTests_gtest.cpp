#include "pch.h"

#include "dnv/vista/sdk/VIS.h"
#include "dnv/vista/sdk/Codebooks.h"
#include "dnv/vista/sdk/Codebook.h"
#include "dnv/vista/sdk/VisVersion.h"

namespace dnv::vista::sdk
{
	class CodebookTest : public ::testing::Test
	{
	protected:
		VIS& vis = VIS::Instance();
		Codebooks codebooks;

		void SetUp() override
		{
			codebooks = vis.GetCodebooks( VisVersion::v3_4a );
		}
	};

	TEST_F( CodebookTest, Test_Position_Validation )
	{
		std::vector<std::pair<std::string, std::string>> testCases = {
			{ "upper", "Valid" },
			{ "lower", "Valid" } };

		auto codebookType = codebooks[CodebookName::Position];

		for ( const auto& [input, expectedOutput] : testCases )
		{
			auto validPosition = codebookType.ValidatePosition( input );
			auto parsedExpectedOutput = PositionValidationResults::FromString( expectedOutput );

			EXPECT_EQ( parsedExpectedOutput, validPosition ) << "Failed for position: " << input;
		}
	}

	TEST_F( CodebookTest, Test_Positions )
	{
		std::vector<std::pair<std::string, std::string>> testCases = {
			{ "invalidValue", "upper" } };

		for ( const auto& [invalidStandardValue, validStandardValue] : testCases )
		{
			auto positions = codebooks[CodebookName::Position];

			EXPECT_FALSE( positions.HasStandardValue( invalidStandardValue ) );
			EXPECT_TRUE( positions.HasStandardValue( validStandardValue ) );
		}
	}

	TEST_F( CodebookTest, Test_Standard_Values )
	{
		auto positions = codebooks[CodebookName::Position];

		EXPECT_TRUE( positions.HasStandardValue( "upper" ) );
		const auto& rawData = positions.GetRawData();
		EXPECT_TRUE( rawData.find( "Vertical" ) != rawData.end() );
		EXPECT_NE( std::find( rawData.at( "Vertical" ).begin(), rawData.at( "Vertical" ).end(), "upper" ),
			rawData.at( "Vertical" ).end() );
	}

	TEST_F( CodebookTest, Test_States )
	{
		struct StatesTestData
		{
			std::string invalidGroup;
			std::string validValue;
			std::string validGroup;
			std::string secondValidValue;
		};

		std::vector<StatesTestData> testCases = {
			{ "NonExistentGroup", "on", "On / off", "off" } };

		for ( const auto& data : testCases )
		{
			const auto& states = codebooks[CodebookName::State];

			EXPECT_FALSE( states.HasGroup( data.invalidGroup ) );
			EXPECT_TRUE( states.HasStandardValue( data.validValue ) );
			EXPECT_TRUE( states.HasGroup( data.validGroup ) );
			EXPECT_TRUE( states.HasStandardValue( data.secondValidValue ) );
		}
	}

	TEST_F( CodebookTest, Test_Create_Tag )
	{
		struct TagTestData
		{
			std::string firstTag;
			std::string secondTag;
			std::string thirdTag;
			char thirdTagPrefix;
			std::string customTag;
			char customTagPrefix;
			std::string firstInvalidTag;
			std::string secondInvalidTag;
		};

		std::vector<TagTestData> testCases = {
			{ "upper", "lower", "port", '-', "~customTag", '~', "##invalid1", "##invalid2" } };

		for ( const auto& data : testCases )
		{
			const auto& codebookType = codebooks[CodebookName::Position];

			auto metadataTag1 = codebookType.CreateTag( data.firstTag );
			EXPECT_EQ( metadataTag1.GetValue(), data.firstTag );
			EXPECT_FALSE( metadataTag1.IsCustom() );

			auto metadataTag2 = codebookType.CreateTag( data.secondTag );
			EXPECT_EQ( metadataTag2.GetValue(), data.secondTag );
			EXPECT_FALSE( metadataTag2.IsCustom() );

			auto metadataTag3 = codebookType.CreateTag( data.thirdTag );
			EXPECT_EQ( metadataTag3.GetValue(), data.thirdTag );
			EXPECT_FALSE( metadataTag3.IsCustom() );
			EXPECT_EQ( metadataTag3.GetPrefix(), data.thirdTagPrefix );

			auto metadataTag4 = codebookType.CreateTag( data.customTag );
			EXPECT_EQ( metadataTag4.GetValue(), data.customTag );
			EXPECT_TRUE( metadataTag4.IsCustom() );
			EXPECT_EQ( metadataTag4.GetPrefix(), data.customTagPrefix );

			EXPECT_THROW( codebookType.CreateTag( data.firstInvalidTag ), std::invalid_argument );
			EXPECT_EQ( codebookType.TryCreateTag( data.firstInvalidTag ), std::nullopt );

			EXPECT_THROW( codebookType.CreateTag( data.secondInvalidTag ), std::invalid_argument );
			EXPECT_EQ( codebookType.TryCreateTag( data.secondInvalidTag ), std::nullopt );
		}
	}

	TEST_F( CodebookTest, Test_Get_Groups )
	{
		const auto& groups = codebooks[CodebookName::Position].GetGroups();
		EXPECT_GT( groups.Count(), 1 );

		EXPECT_TRUE( groups.Contains( "Vertical" ) );
		const auto& rawData = codebooks[CodebookName::Position].GetRawData();

		EXPECT_EQ( groups.Count(), rawData.size() - 1 );
		EXPECT_TRUE( rawData.find( "Vertical" ) != rawData.end() );
	}

	TEST_F( CodebookTest, Test_Iterate_Groups )
	{
		const auto& groups = codebooks[CodebookName::Position].GetGroups();
		int count = 0;
		for ( const auto& group : groups )
		{
			(void)group;
			count++;
		}

		EXPECT_EQ( count, 11 );
	}

	TEST_F( CodebookTest, Test_Iterate_Values )
	{
		const auto& values = codebooks[CodebookName::Position].GetStandardValues();
		int count = 0;
		for ( const auto& value : values )
		{
			(void)value;
			count++;
		}

		EXPECT_EQ( count, 28 );
	}

	TEST_F( CodebookTest, Test_Detail_Tag )
	{
		struct DetailTagTestData
		{
			std::string validCustomTag;
			std::string firstInvalidCustomTag;
			std::string secondInvalidCustomTag;
		};

		std::vector<DetailTagTestData> testCases = {
			{ "validCustomTag", "#invalidTag1", "@invalidTag2" } };

		for ( const auto& data : testCases )
		{
			const auto& codebook = codebooks[CodebookName::Detail];

			EXPECT_TRUE( codebook.TryCreateTag( data.validCustomTag ).has_value() );
			EXPECT_EQ( codebook.TryCreateTag( data.firstInvalidCustomTag ), std::nullopt );
			EXPECT_EQ( codebook.TryCreateTag( data.secondInvalidCustomTag ), std::nullopt );

			EXPECT_THROW( codebook.CreateTag( data.firstInvalidCustomTag ), std::invalid_argument );
			EXPECT_THROW( codebook.CreateTag( data.secondInvalidCustomTag ), std::invalid_argument );
		}
	}
}
