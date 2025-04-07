#include "pch.h"

#include "dnv/vista/sdk/ISO19848Dtos.h"

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

	void to_json( nlohmann::json& j, const DataChannelTypeNameDto& dto )
	{
		j = { { "type", dto.type }, { "description", dto.description } };
	}

	void from_json( const nlohmann::json& j, DataChannelTypeNameDto& dto )
	{
		j.at( "type" ).get_to( dto.type );
		j.at( "description" ).get_to( dto.description );
	}
}
