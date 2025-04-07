#include "pch.h"

#include "dnv/vista/sdk/ImoNumber.h"

using json = nlohmann::json;
using namespace dnv::vista::sdk;

class ImoNumberTests : public ::testing::Test
{
protected:
	struct TestDataItem
	{
		std::string value;
		bool success;
		std::optional<std::string> output;
	};

	std::vector<TestDataItem> testData;

	void SetUp() override
	{
		std::ifstream file;
		file.open( "testdata/ImoNumbers.json" );
		ASSERT_TRUE( file.is_open() ) << "Failed to open testdata/ImoNumbers.json";

		json data;
		file >> data;

		for ( const auto& item : data["ImoNumbers"] )
		{
			testData.push_back( { item["Value"].get<std::string>(),
				item["Success"].get<bool>(),
				item.contains( "Output" ) ? std::optional<std::string>( item["Output"].get<std::string>() ) : std::nullopt } );
		}
	}
};

TEST_F( ImoNumberTests, TestValidation )
{
	for ( const auto& item : testData )
	{
		std::optional<ImoNumber> parsedImo = ImoNumber::TryParse( item.value );

		if ( item.success )
		{
			ASSERT_TRUE( parsedImo.has_value() ) << "Expected parsing to succeed for value: " << item.value;
			EXPECT_NE( parsedImo->ToString(), "" ) << "Parsed IMO number should not be empty for value: " << item.value;
		}
		else
		{
			ASSERT_FALSE( parsedImo.has_value() ) << "Expected parsing to fail for value: " << item.value;
		}

		if ( item.output.has_value() )
		{
			EXPECT_EQ( parsedImo->ToString(), item.output.value() ) << "Expected output mismatch for value: " << item.value;
		}
	}
}
