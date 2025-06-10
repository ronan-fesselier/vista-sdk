/**
 * @file ISO19848Dtos.inl
 * @brief Inline implementations for performance-critical ISO19848Dtos operations
 */

namespace dnv::vista::sdk
{
	//=====================================================================
	// Single Data Channel Type Data Transfer Objects
	//=====================================================================

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline const std::string& DataChannelTypeNameDto::type() const
	{
		return m_type;
	}

	inline const std::string& DataChannelTypeNameDto::description() const
	{
		return m_description;
	}

	//=====================================================================
	// Collection of Data Channel Type Data Transfer Objects
	//=====================================================================

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline const std::vector<DataChannelTypeNameDto>& DataChannelTypeNamesDto::values() const
	{
		return m_values;
	}

	//=====================================================================
	// Single Format Data Type Data Transfer Objects
	//=====================================================================

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline const std::string& FormatDataTypeDto::type() const
	{
		return m_type;
	}

	inline const std::string& FormatDataTypeDto::description() const
	{
		return m_description;
	}

	//=====================================================================
	// Collection of  Format Data Type Data Transfer Objects
	//=====================================================================

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline const std::vector<FormatDataTypeDto>& FormatDataTypesDto::values() const
	{
		return m_values;
	}
}
