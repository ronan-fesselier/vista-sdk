/**
 * @file VISVersion.inl
 * @brief Inline implementations for performance-critical VISVersion operations
 */

namespace dnv::vista::sdk
{
	inline bool VisVersionExtensions::isValid( VisVersion version )
	{
		return version >= VisVersion::v3_4a && version <= VisVersion::v3_8a;
	}

	inline VisVersion VisVersionExtensions::latestVersion()
	{
		return VisVersion::LATEST;
	}

	inline std::string VisVersionExtensions::toVersionString( VisVersion version )
	{
		switch ( version )
		{
			case VisVersion::v3_4a:
				return "vis-3-4a";
			case VisVersion::v3_5a:
				return "vis-3-5a";
			case VisVersion::v3_6a:
				return "vis-3-6a";
			case VisVersion::v3_7a:
				return "vis-3-7a";
			case VisVersion::v3_8a:
				return "vis-3-8a";
			case VisVersion::Unknown:
				return "Unknown";
			case VisVersion::COUNT_VALID:
			default:
			{
				throw std::invalid_argument( "Invalid VIS version: " + std::to_string( static_cast<int>( version ) ) );
			}
		}
	}
}
