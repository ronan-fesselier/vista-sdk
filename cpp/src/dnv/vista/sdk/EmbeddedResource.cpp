#include "pch.h"

#include "dnv/vista/sdk/EmbeddedResource.h"
#include "dnv/vista/sdk/CodebooksDto.h"
#include "dnv/vista/sdk/GmodDto.h"
#include "dnv/vista/sdk/GmodVersioningDto.h"
#include "dnv/vista/sdk/LocationsDto.h"

namespace dnv::vista::sdk
{
	std::vector<std::string> EmbeddedResource::GetResourceNames()
	{
		std::vector<std::string> resourceNames;

		std::vector<std::string> possibleDirs = {
			"resources/",
			"../resources/",
			"../../resources/",
			"./" };

		for ( const auto& dir : possibleDirs )
		{
			try
			{
				if ( !std::filesystem::exists( dir ) || !std::filesystem::is_directory( dir ) )
				{
					continue;
				}

				for ( const auto& entry : std::filesystem::directory_iterator( dir ) )
				{
					if ( !entry.is_regular_file() )
					{
						continue;
					}

					std::string filename = entry.path().filename().string();
					if ( filename.find( ".json.gz" ) != std::string::npos )
					{
						resourceNames.push_back( filename );
					}
				}

				if ( !resourceNames.empty() )
				{
					break;
				}
			}
			catch ( const std::exception& e )
			{
				SPDLOG_ERROR( "Error scanning directory {}: {}", dir, e.what() );
			}
		}

		return resourceNames;
	}

	std::shared_ptr<std::istream> EmbeddedResource::GetDecompressedStream( const std::string& resourceName )
	{
		auto compressedStream = GetStream( resourceName );

		auto decompressedBuffer = std::make_shared<std::stringstream>();

		z_stream zs = { 0 };
		zs.zalloc = Z_NULL;
		zs.zfree = Z_NULL;
		zs.opaque = Z_NULL;

		if ( inflateInit2( &zs, 16 + MAX_WBITS ) != Z_OK )
		{
			SPDLOG_ERROR( "Failed to initialize zlib for decompression" );
			throw std::runtime_error( "Failed to initialize zlib for decompression" );
		}

		std::vector<char> compressedData(
			( std::istreambuf_iterator<char>( *compressedStream ) ),
			std::istreambuf_iterator<char>() );

		zs.next_in = reinterpret_cast<Bytef*>( compressedData.data() );
		zs.avail_in = static_cast<uInt>( compressedData.size() );

		const int chunkSize = 16384;
		std::vector<char> outBuffer( chunkSize );

		int ret;
		do
		{
			zs.next_out = reinterpret_cast<Bytef*>( outBuffer.data() );
			zs.avail_out = chunkSize;

			ret = inflate( &zs, Z_NO_FLUSH );

			if ( ret != Z_OK && ret != Z_STREAM_END )
			{
				inflateEnd( &zs );
				SPDLOG_ERROR( "Decompression failed with error code: {}", ret );
				throw std::runtime_error( "Decompression failed" );
			}

			decompressedBuffer->write( outBuffer.data(), chunkSize - zs.avail_out );
		} while ( ret != Z_STREAM_END );

		inflateEnd( &zs );
		decompressedBuffer->seekg( 0 );

		return decompressedBuffer;
	}

	std::shared_ptr<std::istream> EmbeddedResource::GetStream( const std::string& resourceName )
	{
		std::vector<std::string> possiblePaths = {
			"resources/" + resourceName,
			"../resources/" + resourceName,
			"../../resources/" + resourceName,
			resourceName };

		std::shared_ptr<std::ifstream> fileStream;
		std::string attemptedPaths;

		for ( const auto& path : possiblePaths )
		{
			fileStream = std::make_shared<std::ifstream>( path, std::ios::binary );
			if ( fileStream->is_open() )
			{
				SPDLOG_INFO( "Found resource at path: {}", path );
				return fileStream;
			}
			attemptedPaths += path + ", ";
		}

		SPDLOG_ERROR( "Failed to open resource file: {}. Attempted paths: {}", resourceName, attemptedPaths );
		throw std::runtime_error( "Failed to open resource file: " + resourceName +
								  ". Attempted paths: " + attemptedPaths );
	}

	std::vector<std::string> EmbeddedResource::GetVisVersions()
	{
		auto resourceNames = GetResourceNames();
		std::vector<std::string> visVersions;

		for ( const auto& resourceName : resourceNames )
		{
			if ( resourceName.find( "gmod" ) != std::string::npos &&
				 resourceName.find( "versioning" ) == std::string::npos &&
				 resourceName.find( ".gz" ) != std::string::npos )
			{
				try
				{
					auto stream = GetDecompressedStream( resourceName );
					std::string jsonStr( ( std::istreambuf_iterator<char>( *stream ) ),
						std::istreambuf_iterator<char>() );

					rapidjson::Document gmodJson;
					gmodJson.Parse( jsonStr.c_str() );

					if ( !gmodJson.HasParseError() && gmodJson.HasMember( "visRelease" ) &&
						 gmodJson["visRelease"].IsString() )
					{
						visVersions.push_back( gmodJson["visRelease"].GetString() );
					}
				}
				catch ( const std::exception& e )
				{
					SPDLOG_ERROR( "Error parsing GMOD for version extraction: {}", e.what() );
				}
			}
		}

		return visVersions;
	}

	std::optional<GmodDto> EmbeddedResource::GetGmod( const std::string& visVersion )
	{
		SPDLOG_INFO( "Fetching GMOD resource for version: {}", visVersion );

		auto resourceNames = GetResourceNames();

		auto it = std::find_if( resourceNames.begin(), resourceNames.end(),
			[&visVersion]( const std::string& name ) {
				return name.find( "gmod-vis-" + visVersion ) != std::string::npos &&
					   name.find( ".gz" ) != std::string::npos;
			} );

		if ( it == resourceNames.end() )
		{
			SPDLOG_ERROR( "GMOD resource not found for version: {}.", visVersion );
			return std::nullopt;
		}

		try
		{
			auto stream = GetDecompressedStream( *it );
			SPDLOG_INFO( "Found resource at path: {}", *it );

			std::string jsonStr( ( std::istreambuf_iterator<char>( *stream ) ),
				std::istreambuf_iterator<char>() );

			rapidjson::Document gmodJson;
			gmodJson.Parse( jsonStr.c_str() );

			if ( gmodJson.HasParseError() )
			{
				SPDLOG_ERROR( "Failed to parse GMOD JSON." );
				return std::nullopt;
			}

			SPDLOG_INFO( "Successfully loaded GMOD DTO for version: {} ", visVersion );
			return GmodDto::FromJson( gmodJson );
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error parsing GMOD resource: {} ", e.what() );
			return std::nullopt;
		}
	}

	std::optional<CodebooksDto> EmbeddedResource::GetCodebooks( const std::string& visVersion )
	{
		auto resourceNames = GetResourceNames();

		auto it = std::find_if( resourceNames.begin(), resourceNames.end(),
			[&visVersion]( const std::string& name ) {
				return name.find( "codebooks" ) != std::string::npos &&
					   name.find( visVersion ) != std::string::npos &&
					   name.find( ".gz" ) != std::string::npos;
			} );

		if ( it == resourceNames.end() )
		{
			return std::nullopt;
		}

		try
		{
			auto stream = GetDecompressedStream( *it );
			std::string jsonStr( ( std::istreambuf_iterator<char>( *stream ) ),
				std::istreambuf_iterator<char>() );

			rapidjson::Document codebooksJson;
			codebooksJson.Parse( jsonStr.c_str() );

			if ( codebooksJson.HasParseError() )
			{
				SPDLOG_ERROR( "Failed to parse Codebooks JSON." );
				return std::nullopt;
			}

			return CodebooksDto::FromJson( codebooksJson );
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error parsing Codebooks resource: {} ", e.what() );
			return std::nullopt;
		}
	}

	std::optional<std::unordered_map<std::string, GmodVersioningDto>> EmbeddedResource::GetGmodVersioning()
	{
		auto resourceNames = GetResourceNames();

		auto it = std::find_if( resourceNames.begin(), resourceNames.end(),
			[]( const std::string& name ) {
				return name.find( "gmod-vis-versioning" ) != std::string::npos &&
					   name.find( ".gz" ) != std::string::npos;
			} );

		if ( it == resourceNames.end() )
		{
			return std::nullopt;
		}

		try
		{
			auto stream = GetDecompressedStream( *it );
			std::string jsonStr( ( std::istreambuf_iterator<char>( *stream ) ),
				std::istreambuf_iterator<char>() );

			rapidjson::Document versioningJson;
			versioningJson.Parse( jsonStr.c_str() );

			if ( versioningJson.HasParseError() )
			{
				SPDLOG_ERROR( "Failed to parse GMOD versioning JSON." );
				return std::nullopt;
			}

			std::unordered_map<std::string, GmodVersioningDto> versioningMap;
			for ( auto it = versioningJson.MemberBegin(); it != versioningJson.MemberEnd(); ++it )
			{
				GmodVersioningDto dto = GmodVersioningDto::FromJson( it->value );
				versioningMap[it->name.GetString()] = std::move( dto );
			}

			return versioningMap;
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error parsing GMOD versioning resource: {}", e.what() );
			return std::nullopt;
		}
	}

	std::optional<LocationsDto> EmbeddedResource::GetLocations( const std::string& visVersion )
	{
		auto resourceNames = GetResourceNames();

		auto it = std::find_if( resourceNames.begin(), resourceNames.end(),
			[&visVersion]( const std::string& name ) {
				return name.find( "locations" ) != std::string::npos &&
					   name.find( visVersion ) != std::string::npos &&
					   name.find( ".gz" ) != std::string::npos;
			} );

		if ( it == resourceNames.end() )
		{
			return std::nullopt;
		}

		try
		{
			auto stream = GetDecompressedStream( *it );
			std::string jsonStr( ( std::istreambuf_iterator<char>( *stream ) ),
				std::istreambuf_iterator<char>() );

			rapidjson::Document locationsJson;
			locationsJson.Parse( jsonStr.c_str() );

			if ( locationsJson.HasParseError() )
			{
				SPDLOG_ERROR( "Failed to parse Locations JSON." );
				return std::nullopt;
			}

			return LocationsDto::FromJson( locationsJson );
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error parsing Locations resource: {}", e.what() );
			return std::nullopt;
		}
	}

	std::optional<DataChannelTypeNamesDto> EmbeddedResource::GetDataChannelTypeNames( const std::string& version )
	{
		auto resourceNames = GetResourceNames();

		auto it = std::find_if( resourceNames.begin(), resourceNames.end(),
			[&version]( const std::string& name ) {
				return name.find( "data-channel-type-names" ) != std::string::npos &&
					   name.find( "iso19848" ) != std::string::npos &&
					   name.find( version ) != std::string::npos &&
					   name.find( ".gz" ) != std::string::npos;
			} );

		if ( it == resourceNames.end() )
		{
			return std::nullopt;
		}

		try
		{
			auto stream = GetDecompressedStream( *it );
			std::string jsonStr( ( std::istreambuf_iterator<char>( *stream ) ),
				std::istreambuf_iterator<char>() );

			rapidjson::Document dtNamesJson;
			dtNamesJson.Parse( jsonStr.c_str() );

			if ( dtNamesJson.HasParseError() )
			{
				SPDLOG_ERROR( "Failed to parse DataChannelTypeNames JSON." );
				return std::nullopt;
			}

			return DataChannelTypeNamesDto::FromJson( dtNamesJson );
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error parsing DataChannelTypeNames resource: {}", e.what() );
			return std::nullopt;
		}
	}

	std::optional<FormatDataTypesDto> EmbeddedResource::GetFormatDataTypes( const std::string& version )
	{
		auto resourceNames = GetResourceNames();

		auto it = std::find_if( resourceNames.begin(), resourceNames.end(),
			[&version]( const std::string& name ) {
				return name.find( "format-data-types" ) != std::string::npos &&
					   name.find( "iso19848" ) != std::string::npos &&
					   name.find( version ) != std::string::npos &&
					   name.find( ".gz" ) != std::string::npos;
			} );

		if ( it == resourceNames.end() )
		{
			return std::nullopt;
		}

		try
		{
			auto stream = GetDecompressedStream( *it );
			std::string jsonStr( ( std::istreambuf_iterator<char>( *stream ) ),
				std::istreambuf_iterator<char>() );

			rapidjson::Document fdTypesJson;
			fdTypesJson.Parse( jsonStr.c_str() );

			if ( fdTypesJson.HasParseError() )
			{
				SPDLOG_ERROR( "Failed to parse FormatDataTypes JSON." );
				return std::nullopt;
			}

			return FormatDataTypesDto::FromJson( fdTypesJson );
		}
		catch ( const std::exception& e )
		{
			SPDLOG_ERROR( "Error parsing FormatDataTypes resource: {}", e.what() );
			return std::nullopt;
		}
	}
}
