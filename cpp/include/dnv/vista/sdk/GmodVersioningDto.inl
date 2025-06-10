/**
 * @file GmodVersioning.inl
 * @brief Inline implementations for performance-critical GmodVersioning operations
 */

namespace dnv::vista::sdk
{
	//=====================================================================
	// GMOD Versioning Assignement Data Transfer Objects
	//=====================================================================

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline const std::string& GmodVersioningAssignmentChangeDto::oldAssignment() const
	{
		return m_oldAssignment;
	}

	inline const std::string& GmodVersioningAssignmentChangeDto::currentAssignment() const
	{
		return m_currentAssignment;
	}

	//=====================================================================
	// GMOD Node Conversion Transfer Object
	//=====================================================================

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline const GmodNodeConversionDto::OperationSet& GmodNodeConversionDto::operations() const
	{
		return m_operations;
	}

	inline const std::string& GmodNodeConversionDto::source() const
	{
		return m_source;
	}

	inline const std::string& GmodNodeConversionDto::target() const
	{
		return m_target;
	}

	inline const std::string& GmodNodeConversionDto::oldAssignment() const
	{
		return m_oldAssignment;
	}

	inline const std::string& GmodNodeConversionDto::newAssignment() const
	{
		return m_newAssignment;
	}

	inline bool GmodNodeConversionDto::deleteAssignment() const
	{
		return m_deleteAssignment;
	}

	//=====================================================================
	// GMOD Versioning Data Transfer Object
	//=====================================================================

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline const std::string& GmodVersioningDto::visVersion() const
	{
		return m_visVersion;
	}

	inline const GmodVersioningDto::ItemsMap& GmodVersioningDto::items() const
	{
		return m_items;
	}
}
