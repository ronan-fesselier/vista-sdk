/**
 * @file GmodDto.inl
 * @brief Inline implementations for performance-critical GmodDto operations
 */

namespace dnv::vista::sdk
{
	//=====================================================================
	// GMOD Node Data Transfer Object
	//=====================================================================

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline const std::string& GmodNodeDto::category() const
	{
		return m_category;
	}

	inline const std::string& GmodNodeDto::type() const
	{
		return m_type;
	}

	inline const std::string& GmodNodeDto::code() const
	{
		return m_code;
	}

	inline const std::string& GmodNodeDto::name() const
	{
		return m_name;
	}

	inline const std::optional<std::string>& GmodNodeDto::commonName() const
	{
		return m_commonName;
	}

	inline const std::optional<std::string>& GmodNodeDto::definition() const
	{
		return m_definition;
	}

	inline const std::optional<std::string>& GmodNodeDto::commonDefinition() const
	{
		return m_commonDefinition;
	}

	inline const std::optional<bool>& GmodNodeDto::installSubstructure() const
	{
		return m_installSubstructure;
	}

	inline const std::optional<GmodNodeDto::NormalAssignmentNamesMap>& GmodNodeDto::normalAssignmentNames() const
	{
		return m_normalAssignmentNames;
	}

	//=====================================================================
	// GMOD Data Transfer Object
	//=====================================================================

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline const std::string& GmodDto::visVersion() const
	{
		return m_visVersion;
	}

	inline const GmodDto::Items& GmodDto::items() const
	{
		return m_items;
	}

	inline const GmodDto::Relations& GmodDto::relations() const
	{
		return m_relations;
	}
}
