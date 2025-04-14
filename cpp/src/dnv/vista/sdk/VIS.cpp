#include "pch.h"

#include "dnv/vista/sdk/VIS.h"

#include "dnv/vista/sdk/Codebook.h"
#include "dnv/vista/sdk/EmbeddedResource.h"
#include "dnv/vista/sdk/GmodDto.h"
#include "dnv/vista/sdk/GmodNode.h"
#include "dnv/vista/sdk/LocalIdBuilder.h"
#include "dnv/vista/sdk/Locations.h"
#include "dnv/vista/sdk/LocationsDto.h"
#include "dnv/vista/sdk/VisVersion.h"
#include "dnv/vista/sdk/Gmod.h"

namespace dnv::vista::sdk
{
	const VisVersion VIS::LatestVisVersion = VisVersionExtensions::GetLatestVersion();
	const std::string VIS::m_versioning = "versioning";

	VIS& VIS::Instance()
	{
		static VIS instance;
		return instance;
	}

	template <typename K, typename V>
	V& VIS::Cache<K, V>::GetOrCreate( const K& key, std::function<V()> factory ) const
	{
		auto it = m_cache.find( key );
		if ( it == m_cache.end() )
		{
			auto result = factory();
			auto [inserted_it, success] = m_cache.emplace( key, std::move( result ) );
			return inserted_it->second;
		}
		return it->second;
	}

	VIS::VIS()
	{
	}

	GmodDto VIS::GetGmodDto( VisVersion visVersion ) const
	{
		return m_gmodDtoCache.GetOrCreate( visVersion, [visVersion]() {
			auto dto = LoadGmodDto( visVersion );
			if ( !dto )
			{
				SPDLOG_ERROR( "Failed to load GMOD DTO for version: {} ", VisVersionExtensions::ToVersionString( visVersion ) );
				throw std::runtime_error( "Failed to load GMOD DTO for version" );
			}
			return *dto;
		} );
	}

	std::optional<GmodDto> VIS::LoadGmodDto( VisVersion visVersion )
	{
		return EmbeddedResource::GetGmod( VisVersionExtensions::ToVersionString( visVersion ) );
	}

	Gmod VIS::GetGmod( VisVersion visVersion ) const
	{
		if ( !VisVersionExtensions::IsValid( visVersion ) )
		{
			SPDLOG_ERROR( "Invalid VIS version: {} ", static_cast<int>( visVersion ) );
			throw std::invalid_argument( "Invalid VIS version: " + std::to_string( static_cast<int>( visVersion ) ) );
		}

		SPDLOG_INFO( "Attempting to load GMOD for version:{} ", VisVersionExtensions::ToVersionString( visVersion ) );

		return m_gmodCache.GetOrCreate( visVersion, [this, visVersion]() {
			auto dto = GetGmodDto( visVersion );
			SPDLOG_INFO( "Successfully loaded GMOD DTO for version: {} ", VisVersionExtensions::ToVersionString( visVersion ) );

			return Gmod( visVersion, dto );
		} );
	}

	std::unordered_map<VisVersion, Gmod> VIS::GetGmodsMap( const std::vector<VisVersion>& visVersions )
	{
		for ( const auto& version : visVersions )
		{
			if ( !VisVersionExtensions::IsValid( version ) )
			{
				SPDLOG_ERROR( "Invalid VIS version: {} ", static_cast<int>( version ) );
				throw std::invalid_argument( "Invalid VIS version: " + std::to_string( static_cast<int>( version ) ) );
			}
		}

		std::set<VisVersion> versions( visVersions.begin(), visVersions.end() );

		std::unordered_map<VisVersion, Gmod> result;
		for ( const auto& version : versions )
		{
			result.insert_or_assign( version, GetGmod( version ) );
		}

		return result;
	}

	std::unordered_map<std::string, GmodVersioningDto> VIS::GetGmodVersioningDto()
	{
		return m_gmodVersioningDtoCache.GetOrCreate( m_versioning, []() {
			auto dtos = EmbeddedResource::GetGmodVersioning();
			if ( !dtos )
			{
				SPDLOG_ERROR( "Failed to load GMOD versioning DTO" );
				throw std::invalid_argument( "Failed to load GMOD versioning DTO" );
			}

			return *dtos;
		} );
	}

	GmodVersioning VIS::GetGmodVersioning()
	{
		return m_gmodVersioningCache.GetOrCreate( m_versioning, [this]() {
			auto dto = GetGmodVersioningDto();
			return GmodVersioning( dto );
		} );
	}

	CodebooksDto VIS::GetCodebooksDto( VisVersion visVersion )
	{
		return m_codebooksDtoCache.GetOrCreate( visVersion, [visVersion]() {
			auto dto = EmbeddedResource::GetCodebooks( VisVersionExtensions::ToVersionString( visVersion ) );
			if ( !dto )
			{
				SPDLOG_ERROR( "Failed to load codebooks DTO for version: {} ", VisVersionExtensions::ToVersionString( visVersion ) );
				throw std::runtime_error( "Failed to load codebooks DTO for version" );
			}
			return *dto;
		} );
	}

	Codebooks VIS::GetCodebooks( VisVersion visVersion )
	{
		return m_codebooksCache.GetOrCreate( visVersion, [this, visVersion]() {
			auto dto = GetCodebooksDto( visVersion );
			return Codebooks( visVersion, dto );
		} );
	}

	std::unordered_map<VisVersion, Codebooks> VIS::GetCodebooksMap( const std::vector<VisVersion>& visVersions )
	{
		for ( const auto& version : visVersions )
		{
			if ( !VisVersionExtensions::IsValid( version ) )
			{
				SPDLOG_ERROR( "Invalid VIS version: {} ", static_cast<int>( version ) );
				throw std::invalid_argument( "Invalid VIS version: " + std::to_string( static_cast<int>( version ) ) );
			}
		}

		std::set<VisVersion> versions( visVersions.begin(), visVersions.end() );

		std::unordered_map<VisVersion, Codebooks> result;
		for ( const auto& version : versions )
		{
			result[version] = GetCodebooks( version );
		}

		return result;
	}

	LocationsDto VIS::GetLocationsDto( VisVersion visVersion )
	{
		return m_locationsDtoCache.GetOrCreate( visVersion, [visVersion]() {
			auto dto = EmbeddedResource::GetLocations( VisVersionExtensions::ToVersionString( visVersion ) );
			if ( !dto )
			{
				SPDLOG_ERROR( "Failed to load locations DTO for version: {} ", VisVersionExtensions::ToVersionString( visVersion ) );
				throw std::runtime_error( "Failed to load locations DTO for version" );
			}
			return *dto;
		} );
	}

	Locations VIS::GetLocations( VisVersion visversion )
	{
		return m_locationsCache.GetOrCreate( visversion, [this, visversion]() {
			auto dto = GetLocationsDto( visversion );
			return Locations( visversion, dto );
		} );
	}

	std::unordered_map<VisVersion, Locations> VIS::GetLocationsMap( const std::vector<VisVersion>& visVersions )
	{
		for ( const auto& version : visVersions )
		{
			if ( !VisVersionExtensions::IsValid( version ) )
			{
				SPDLOG_ERROR( "Invalid VIS version: {} ", static_cast<int>( version ) );
				throw std::invalid_argument( "Invalid VIS version: " + std::to_string( static_cast<int>( version ) ) );
			}
		}

		std::set<VisVersion> versions( visVersions.begin(), visVersions.end() );

		std::unordered_map<VisVersion, Locations> result;
		for ( const auto& version : versions )
		{
			result[version] = GetLocations( version );
		}

		return result;
	}

	std::vector<VisVersion> VIS::GetVisVersions()
	{
		return VisVersionExtensions::GetAllVersions();
	}

	std::optional<GmodNode> VIS::ConvertNode( const GmodNode& sourceNode, VisVersion targetVersion,
		const GmodNode* sourceParent )
	{
		return ConvertNode( sourceNode.GetVisVersion(), sourceNode, targetVersion );
	}

	std::optional<GmodNode> VIS::ConvertNode( VisVersion sourceVersion, const GmodNode& sourceNode,
		VisVersion targetVersion )
	{
		return GetGmodVersioning().ConvertNode( sourceVersion, sourceNode, targetVersion );
	}

	std::optional<GmodPath> VIS::ConvertPath( const GmodPath& sourcePath, VisVersion targetVersion )
	{
		return ConvertPath( sourcePath.GetVisVersion(), sourcePath, targetVersion );
	}

	std::optional<GmodPath> VIS::ConvertPath( VisVersion sourceVersion, const GmodPath& sourcePath,
		VisVersion targetVersion )
	{
		return GetGmodVersioning().ConvertPath( sourceVersion, sourcePath, targetVersion );
	}

	std::optional<LocalIdBuilder> VIS::ConvertLocalId( const LocalIdBuilder& sourceLocalId,
		VisVersion targetVersion )
	{
		return GetGmodVersioning().ConvertLocalId( sourceLocalId, targetVersion );
	}

	std::optional<LocalId> VIS::ConvertLocalId( const LocalId& sourceLocalId, VisVersion targetVersion )
	{
		return GetGmodVersioning().ConvertLocalId( sourceLocalId, targetVersion );
	}

	bool VIS::MatchISOLocalIdString( const std::string& value )
	{
		for ( char ch : value )
		{
			if ( ch == '/' )
				continue;
			if ( !IsISOString( ch ) )
				return false;
		}
		return true;
	}

	bool VIS::IsISOString( char c )
	{
		return MatchAsciiDecimal( c );
	}

	bool VIS::IsISOString( const std::string& value )
	{
		for ( char c : value )
		{
			if ( !MatchAsciiDecimal( c ) )
				return false;
		}
		return true;
	}

	bool VIS::IsISOString( const std::string_view value )
	{
		for ( char c : value )
		{
			if ( !MatchAsciiDecimal( c ) )
				return false;
		}
		return true;
	}

	bool VIS::IsISOLocalIdString( const std::string& value )
	{
		size_t start = 0;
		size_t end = value.find( '/' );

		while ( end != std::string::npos )
		{
			std::string part = value.substr( start, end - start );

			if ( !IsISOString( part ) )
			{
				return false;
			}

			start = end + 1;
			end = value.find( '/', start );
		}

		std::string lastPart = value.substr( start );
		return IsISOString( lastPart );
	}

	bool VIS::MatchAsciiDecimal( int code )
	{
		if ( code >= 48 && code <= 57 )
			return true;
		if ( code >= 65 && code <= 90 )
			return true;
		if ( code >= 97 && code <= 122 )
			return true;
		if ( code == 45 || code == 46 || code == 95 || code == 126 )
			return true;
		return false;
	}
}
