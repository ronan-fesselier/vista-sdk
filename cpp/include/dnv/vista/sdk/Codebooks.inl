/**
 * @file Codebooks.inl
 * @brief Inline implementation of Codebooks class methods
 * @details Contains performance-critical inline implementations for
 *          fast codebook access operations.
 */

namespace dnv::vista::sdk
{
	//=====================================================================
	// Inline method implementations
	//=====================================================================

	inline const Codebook& Codebooks::operator[]( CodebookName name ) const noexcept
	{
		return m_codebooks[static_cast<size_t>( name ) - 1];
	}

	inline const Codebook& Codebooks::codebook( CodebookName name ) const
	{
		const auto rawValue = static_cast<size_t>( name );
		if ( rawValue == 0 || rawValue > NUM_CODEBOOKS )
		{
			throw std::invalid_argument( "Invalid codebook name: " + std::to_string( rawValue ) );
		}

		const auto index = rawValue - 1;

		return m_codebooks[index];
	}
}
