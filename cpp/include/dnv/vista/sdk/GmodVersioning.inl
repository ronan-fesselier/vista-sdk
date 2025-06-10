/**
 * @file GmodVersioning.inl
 * @brief Inline implementations for performance-critical GmodVersioning operations
 */

namespace dnv::vista::sdk
{
	//=====================================================================
	// GmodVersioning class
	//=====================================================================

	//----------------------------------------------
	// Accessors
	//----------------------------------------------

	inline VisVersion GmodVersioning::GmodVersioningNode::visVersion() const
	{
		return m_visVersion;
	}
}
