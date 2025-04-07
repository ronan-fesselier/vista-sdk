#include "pch.h"

#include "dnv/vista/sdk/ISO19848Dtos.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace dnv::vista::sdk
{
	DataChannelTypeNameDto::DataChannelTypeNameDto(
		const std::string& type,
		const std::string& description )
		: type( type ), description( description )
	{
	}

	DataChannelTypeNamesDto::DataChannelTypeNamesDto(
		const std::vector<DataChannelTypeNameDto>& values )
		: values( values )
	{
	}

	FormatDataTypeDto::FormatDataTypeDto(
		const std::string& type,
		const std::string& description )
		: type( type ), description( description )
	{
	}

	FormatDataTypesDto::FormatDataTypesDto(
		const std::vector<FormatDataTypeDto>& values )
		: values( values )
	{
	}

	void SerializeToJson( rapidjson::Writer<rapidjson::StringBuffer>& writer, const DataChannelTypeNameDto& dto )
	{
		writer.StartObject();

		writer.Key( "type" );
		writer.String( dto.type.c_str() );

		writer.Key( "description" );
		writer.String( dto.description.c_str() );

		writer.EndObject();
	}

	void SerializeToJson( rapidjson::Writer<rapidjson::StringBuffer>& writer, const DataChannelTypeNamesDto& dto )
	{
		writer.StartObject();

		writer.Key( "values" );
		writer.StartArray();
		for ( const auto& value : dto.values )
		{
			SerializeToJson( writer, value );
		}
		writer.EndArray();

		writer.EndObject();
	}

	void SerializeToJson( rapidjson::Writer<rapidjson::StringBuffer>& writer, const FormatDataTypeDto& dto )
	{
		writer.StartObject();

		writer.Key( "type" );
		writer.String( dto.type.c_str() );

		writer.Key( "description" );
		writer.String( dto.description.c_str() );

		writer.EndObject();
	}

	void SerializeToJson( rapidjson::Writer<rapidjson::StringBuffer>& writer, const FormatDataTypesDto& dto )
	{
		writer.StartObject();

		writer.Key( "values" );
		writer.StartArray();
		for ( const auto& value : dto.values )
		{
			SerializeToJson( writer, value );
		}
		writer.EndArray();

		writer.EndObject();
	}
}
