/**
 * @file GmodConstants.h
 * @brief Performance-optimized constants for GMOD node classification and operations
 */

#pragma once

namespace dnv::vista::sdk
{
	inline constexpr std::string_view NODE_CATEGORY_ASSET = "ASSET";
	inline constexpr std::string_view NODE_CATEGORY_FUNCTION = "FUNCTION";
	inline constexpr std::string_view NODE_CATEGORY_PRODUCT = "PRODUCT";

	inline constexpr std::string_view NODE_CATEGORY_ASSET_FUNCTION = "ASSET FUNCTION";
	inline constexpr std::string_view NODE_CATEGORY_PRODUCT_FUNCTION = "PRODUCT FUNCTION";

	inline constexpr std::string_view NODE_TYPE_SELECTION = "SELECTION";
	inline constexpr std::string_view NODE_TYPE_GROUP = "GROUP";
	inline constexpr std::string_view NODE_TYPE_LEAF = "LEAF";
	inline constexpr std::string_view NODE_TYPE_TYPE = "TYPE";
	inline constexpr std::string_view NODE_TYPE_COMPOSITION = "COMPOSITION";

	inline constexpr std::string_view NODE_FULLTYPE_ASSET_FUNCTION_LEAF = "ASSET FUNCTION LEAF";
	inline constexpr std::string_view NODE_FULLTYPE_PRODUCT_FUNCTION_LEAF = "PRODUCT FUNCTION LEAF";
}
