#include "pch.h"

#include "dnv/vista/sdk/VisVersion.h"

namespace dnv::vista::sdk
{
	bool VisVersionExtensions::IsValid( VisVersion version )
	{
		switch ( version )
		{
			case VisVersion::v3_4a:
			case VisVersion::v3_5a:
			case VisVersion::v3_6a:
			case VisVersion::v3_7a:
			case VisVersion::v3_8a:
				return true;
			default:
				return false;
		}
	}

	std::string VisVersionExtensions::toVersionString( VisVersion version )
	{
		switch ( version )
		{
			case VisVersion::v3_4a:
				return "3-4a";
			case VisVersion::v3_5a:
				return "3-5a";
			case VisVersion::v3_6a:
				return "3-6a";
			case VisVersion::v3_7a:
				return "3-7a";
			case VisVersion::v3_8a:
				return "3-8a";
			default:
			{
				SPDLOG_ERROR( "Invalid VIS version: {}", static_cast<int>( version ) );
				throw std::invalid_argument( "Invalid VIS version: " +
											 std::to_string( static_cast<int>( version ) ) );
			}
		}
	}

	bool VisVersionExtensions::TryParse( const std::string& versionString, VisVersion& version )
	{
		if ( versionString == "3.4a" )
			version = VisVersion::v3_4a;
		if ( versionString == "3.5a" )
			version = VisVersion::v3_5a;
		else if ( versionString == "3.6a" )
			version = VisVersion::v3_6a;
		else if ( versionString == "3.7a" )
			version = VisVersion::v3_7a;
		else if ( versionString == "3.8a" )
			version = VisVersion::v3_8a;
		else if ( versionString == "3-4a" )
			version = VisVersion::v3_4a;
		else if ( versionString == "3-5a" )
			version = VisVersion::v3_5a;
		else if ( versionString == "3-6a" )
			version = VisVersion::v3_6a;
		else if ( versionString == "3-7a" )
			version = VisVersion::v3_7a;
		else if ( versionString == "3-8a" )
			version = VisVersion::v3_8a;
		else
		{
			std::string normalizedVersion = versionString;
			std::replace( normalizedVersion.begin(), normalizedVersion.end(), '-', '.' );

			if ( normalizedVersion != versionString )
			{
				return TryParse( normalizedVersion, version );
			}
			return false;
		}

		return true;
	}

	VisVersion VisVersionExtensions::Parse( const std::string& versionString )
	{
		VisVersion result;
		if ( !TryParse( versionString, result ) )
		{
			SPDLOG_ERROR( "Invalid VIS version string: {}", versionString );
			throw std::invalid_argument( "Invalid VIS version string: " + versionString );
		}
		return result;
	}

	std::vector<VisVersion> VisVersionExtensions::GetAllVersions()
	{
		static const std::array<VisVersion, 5> allVersions = {
			VisVersion::v3_4a,
			VisVersion::v3_5a,
			VisVersion::v3_6a,
			VisVersion::v3_7a,
			VisVersion::v3_8a };

		return std::vector<VisVersion>( allVersions.begin(), allVersions.end() );
	}

	VisVersion VisVersionExtensions::GetLatestVersion()
	{
		auto versions = GetAllVersions();
		return versions.empty() ? VisVersion::Unknown : versions.back();
	}
}
