/**
 * @file LocationsDto.inl
 * @brief Inline implementations for performance-critical LocationsDto operations
 */

namespace dnv::vista::sdk
{
	//=====================================================================
	// Relative Location Data Transfer Objects
	//=====================================================================

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline char RelativeLocationsDto::code() const
	{
		return m_code;
	}

	inline const std::string& RelativeLocationsDto::name() const
	{
		return m_name;
	}

	inline const std::optional<std::string>& RelativeLocationsDto::definition() const
	{
		return m_definition;
	}

	//=====================================================================
	// Location Data Transfer Objects
	//=====================================================================

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline const std::string& LocationsDto::visVersion() const
	{
		return m_visVersion;
	}

	inline const std::vector<RelativeLocationsDto>& LocationsDto::items() const
	{
		return m_items;
	}
}
