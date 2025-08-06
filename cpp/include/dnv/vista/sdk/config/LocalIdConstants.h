/**
 * @file LocalIdConstants.h
 * @brief LocalId parsing and string building constants
 * @details Contains prefix constants used for LocalId parsing state transitions
 *          and string serialization operations
 */

#pragma once

namespace dnv::vista::sdk::localId
{
	//----------------------------------------------
	// LocalId prefix constants
	//----------------------------------------------

	/** @brief VIS version prefix */
	inline constexpr std::string_view PREFIX_VIS = "vis-";

	/** @brief Secondary item prefix */
	inline constexpr std::string_view PREFIX_SEC = "sec";

	/** @brief Initial secondary verbose prefix */
	inline constexpr std::string_view PREFIX_FOR = "~for.";

	/** @brief Subsequent secondary verbose prefix */
	inline constexpr std::string_view PREFIX_TILDE = "~";

	/** @brief Metadata section prefix */
	inline constexpr std::string_view PREFIX_META = "meta";
}
