/**
 * @file TestDataLoader.h
 * @brief Helper utilities for loading test data from JSON files.
 */

#pragma once

namespace dnv::vista::sdk
{
	const nlohmann::json& loadTestData( const char* testDataPath );
}
