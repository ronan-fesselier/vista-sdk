#include "pch.h"

#include "dnv/vista/sdk/VIS.h"
#include "dnv/vista/sdk/VisVersion.h"
#include "dnv/vista/sdk/Gmod.h"
#include "dnv/vista/sdk/GmodNode.h"
#include <gtest/gtest.h>
#include <unordered_set>
#include <algorithm>
#include "dnv/vista/sdk/CodebooksDto.h"

namespace dnv::vista::sdk::tests
{
	class GmodTests : public ::testing::TestWithParam<VisVersion>
	{
	protected:
		static std::pair<VIS, Gmod> GetVisAndGmod( VisVersion visVersion )
		{
			VIS vis = VIS::Instance();
			Gmod gmod = vis.GetGmod( visVersion );
			return { vis, gmod };
		}
	};

	TEST_P( GmodTests, Test_Gmod_Loads )
	{
		auto visVersion = GetParam();
		auto [vis, gmod] = GetVisAndGmod( visVersion );

		GmodNode tempNode;
		ASSERT_TRUE( gmod.TryGetNode( std::string( "400a" ), tempNode ) ) << "Node '400a' not found in GMOD.";
	}

	TEST_P( GmodTests, Test_Gmod_Properties )
	{
		auto visVersion = GetParam();
		auto [vis, gmod] = GetVisAndGmod( visVersion );

		const auto& rootNode = gmod.GetRootNode();
		ASSERT_FALSE( rootNode.GetChildren().empty() ) << "Root node has no children.";

		auto minLengthNode = std::min_element(
			rootNode.GetChildren().begin(), rootNode.GetChildren().end(),
			[]( const GmodNode* a, const GmodNode* b ) { return a->GetCode().length() < b->GetCode().length(); } );

		auto maxLengthNode = std::max_element(
			rootNode.GetChildren().begin(), rootNode.GetChildren().end(),
			[]( const GmodNode* a, const GmodNode* b ) { return a->GetCode().length() < b->GetCode().length(); } );

		ASSERT_NE( minLengthNode, rootNode.GetChildren().end() ) << "No minimum length node found.";
		ASSERT_NE( maxLengthNode, rootNode.GetChildren().end() ) << "No maximum length node found.";

		EXPECT_EQ( ( *minLengthNode )->GetCode().length(), 2 ) << "Minimum length node code length mismatch.";
		EXPECT_EQ( ( *minLengthNode )->GetCode(), "VE" ) << "Minimum length node code mismatch.";
		EXPECT_EQ( ( *maxLengthNode )->GetCode().length(), 10 ) << "Maximum length node code length mismatch.";
	}

	TEST_P( GmodTests, Test_Gmod_Lookup )
	{
		auto visVersion = GetParam();
		auto [vis, gmod] = GetVisAndGmod( visVersion );

		GmodNode tempNode;
		ASSERT_TRUE( gmod.TryGetNode( std::string( "400a" ), tempNode ) ) << "Node '400a' not found.";
		ASSERT_FALSE( gmod.TryGetNode( std::string( "InvalidCode" ), tempNode ) ) << "Invalid node code unexpectedly found.";
	}

	TEST_P( GmodTests, Test_Gmod_Node_Equality )
	{
		auto visVersion = GetParam();
		auto [vis, gmod] = GetVisAndGmod( visVersion );

		const auto& node1 = gmod["400a"];
		const auto& node2 = gmod["400a"];

		EXPECT_EQ( node1, node2 ) << "Nodes with the same code are not equal.";

		auto node3 = node2.WithLocation( "1" );
		EXPECT_NE( node1, node3 ) << "Nodes with different locations are unexpectedly equal.";
	}

	TEST_P( GmodTests, Test_Gmod_Node_Types )
	{
		auto visVersion = GetParam();
		auto [vis, gmod] = GetVisAndGmod( visVersion );

		std::unordered_set<std::string> nodeTypes;
		for ( const auto& node : gmod )
		{
			nodeTypes.insert( node.GetMetadata().GetCategory() + " | " + node.GetMetadata().GetType() );
		}

		EXPECT_FALSE( nodeTypes.empty() ) << "No node types found in GMOD.";
	}

	TEST_P( GmodTests, Test_Gmod_RootNode_Children )
	{
		auto visVersion = GetParam();
		auto [vis, gmod] = GetVisAndGmod( visVersion );

		const auto& rootNode = gmod.GetRootNode();
		ASSERT_FALSE( rootNode.GetChildren().empty() ) << "Root node has no children.";
	}

	TEST_P( GmodTests, Test_Mappability )
	{
		auto visVersion = GetParam();
		auto [vis, gmod] = GetVisAndGmod( visVersion );

		struct TestCase
		{
			std::string Code;
			bool Mappable;
		};

		std::vector<TestCase> testCases = {
			{ "VE", false }, { "300a", false }, { "300", true }, { "411", true }, { "410", true },
			{ "651.21s", false }, { "924.2", true }, { "411.1", false }, { "C101", true },
			{ "CS1", false }, { "C101.663", true }, { "C101.4", true }, { "C101.21s", false },
			{ "F201.11", true }, { "C101.211", false } };

		for ( const auto& testCase : testCases )
		{
			GmodNode node;
			ASSERT_TRUE( gmod.TryGetNode( testCase.Code, node ) ) << "Node '" << testCase.Code << "' not found.";
			EXPECT_EQ( node.IsMappable(), testCase.Mappable ) << "Mappability mismatch for node '" << testCase.Code << "'.";
		}
	}

	INSTANTIATE_TEST_SUITE_P(
		GmodTestsParameterized,
		GmodTests,
		::testing::Values( VisVersion::v3_8a ) );
}
