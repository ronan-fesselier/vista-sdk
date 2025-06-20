/**
 * @file Config.h
 * @brief Performance-optimized constants for GMOD node classification and operations
 */

#pragma once

namespace dnv::vista::sdk
{
	//=====================================================================
	// Hash algorithm constants
	//=====================================================================

	/** @brief FNV offset basis constant for hash calculations. */
	inline constexpr uint32_t FNV_OFFSET_BASIS{ 0x811C9DC5 };

	/** @brief FNV prime constant for hash calculations. */
	inline constexpr uint32_t FNV_PRIME{ 0x01000193 };

	//=====================================================================
	// CHD dictionary configuration
	//=====================================================================

	/** @brief Maximum multiplier for seed search iterations in CHD construction. */
	inline constexpr uint32_t MAX_SEED_SEARCH_MULTIPLIER = 100;

	//=====================================================================
	// GMOD Node Classification Constants
	//=====================================================================

	//----------------------------------------------
	// Primary node categories
	//----------------------------------------------

	/** @brief Asset category identifier for GMOD nodes. */
	inline constexpr std::string_view NODE_CATEGORY_ASSET = "ASSET";

	/** @brief Function category identifier for GMOD nodes. */
	inline constexpr std::string_view NODE_CATEGORY_FUNCTION = "FUNCTION";

	/** @brief Product category identifier for GMOD nodes. */
	inline constexpr std::string_view NODE_CATEGORY_PRODUCT = "PRODUCT";

	//----------------------------------------------
	// Composite node categories
	//----------------------------------------------

	/** @brief Asset function composite category for GMOD nodes. */
	inline constexpr std::string_view NODE_CATEGORY_ASSET_FUNCTION = "ASSET FUNCTION";

	/** @brief Product function composite category for GMOD nodes. */
	inline constexpr std::string_view NODE_CATEGORY_PRODUCT_FUNCTION = "PRODUCT FUNCTION";

	//----------------------------------------------
	// Node Type Classifications
	//----------------------------------------------

	/** @brief Selection type identifier for GMOD nodes. */
	inline constexpr std::string_view NODE_TYPE_SELECTION = "SELECTION";

	/** @brief Group type identifier for GMOD nodes. */
	inline constexpr std::string_view NODE_TYPE_GROUP = "GROUP";

	/** @brief Leaf type identifier for GMOD nodes. */
	inline constexpr std::string_view NODE_TYPE_LEAF = "LEAF";

	/** @brief Type type identifier for GMOD nodes. */
	inline constexpr std::string_view NODE_TYPE_TYPE = "TYPE";

	/** @brief Composition type identifier for GMOD nodes. */
	inline constexpr std::string_view NODE_TYPE_COMPOSITION = "COMPOSITION";

	//----------------------------------------------
	// Full Type Specifications
	//----------------------------------------------

	/** @brief Complete asset function leaf type specification. */
	inline constexpr std::string_view NODE_FULLTYPE_ASSET_FUNCTION_LEAF = "ASSET FUNCTION LEAF";

	/** @brief Complete product function leaf type specification. */
	inline constexpr std::string_view NODE_FULLTYPE_PRODUCT_FUNCTION_LEAF = "PRODUCT FUNCTION LEAF";
}
