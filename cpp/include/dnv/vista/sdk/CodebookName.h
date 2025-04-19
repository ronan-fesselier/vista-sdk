#pragma once

namespace dnv::vista::sdk
{
	/**
	 * @brief Enumeration of codebook types used in the VIS system
	 *
	 * Each value represents a different category of data that can be stored
	 * in the vessel information structure according to ISO 19848.
	 */
	enum class CodebookName
	{
		Unknown = 0,

		Quantity = 1,		 ///< Represents physical quantities and units (prefix: qty)
		Content,			 ///< Represents content types (prefix: cnt)
		Calculation,		 ///< Represents calculation methods (prefix: calc)
		State,				 ///< Represents states or conditions (prefix: state)
		Command,			 ///< Represents commands or actions (prefix: cmd)
		Type,				 ///< Represents type classifications (prefix: type)
		FunctionalServices,	 ///< Represents functional services (prefix: funct.svc)
		MaintenanceCategory, ///< Represents maintenance categories (prefix: maint.cat)
		ActivityType,		 ///< Represents types of activities (prefix: act.type)
		Position,			 ///< Represents positions or locations (prefix: pos)
		Detail				 ///< Represents detailed information (prefix: detail)
	};

	/**
	 * @brief Utility class for working with CodebookName values
	 *
	 * Provides conversion methods between CodebookName enum values
	 * and their string prefix representations as defined in the ISO 19848 standard.
	 */
	class CodebookNames
	{
	public:
		/**
		 * @brief Convert a string prefix to a CodebookName
		 * @param prefix The string prefix to convert (e.g., "pos", "qty", "calc")
		 * @return The corresponding CodebookName value
		 * @throws std::invalid_argument If the prefix is empty or unknown
		 */
		static CodebookName fromPrefix( const std::string_view prefix );

		/**
		 * @brief Convert a CodebookName to its string prefix
		 * @param name The CodebookName to convert
		 * @return The corresponding string prefix (e.g., "pos" for Position)
		 * @throws std::invalid_argument If the name is unknown
		 */
		static std::string toPrefix( CodebookName name );
	};
}
