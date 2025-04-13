#
# VISTA-SDK-CPP
#

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC-specific settings
if(MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
	set(BUILD_SHARED_LIBS TRUE)
endif()

# Define directories
set(VISTA_SDK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(VISTA_SDK_RESOURCE_DIR ${VISTA_SDK_SOURCE_DIR}/resources)
set(VISTA_SDK_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)

# Define build options
option(VISTA_SDK_BUILD_TESTS	"Build tests"						ON	)
option(VISTA_SDK_COPY_RESOURCES "Copy resources to build directory"	ON	)
option(VISTA_SDK_BUILD_SAMPLES	"Build vista-sdk-cpp samples"		ON	)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${VISTA_SDK_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${VISTA_SDK_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${VISTA_SDK_OUTPUT_DIR})

# Copy resources
if(EXISTS ${VISTA_SDK_RESOURCES_DIR})
	add_custom_target(CopyResources ALL
		COMMAND ${CMAKE_COMMAND} -E make_directory ${VISTA_SDK_OUTPUT_DIR}/resources
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			${VISTA_SDK_RESOURCES_DIR} ${VISTA_SDK_OUTPUT_DIR}/resources
		COMMENT "Copying VISTA SDK resources to build directory"
	)
	message(STATUS "Resources will be copied from ${VISTA_SDK_RESOURCES_DIR} to ${VISTA_SDK_OUTPUT_DIR}/resources")
else()
	message(WARNING "Resources directory not found: ${VISTA_SDK_RESOURCES_DIR}. Skipping resource copy.")
endif()



# Add subdirectories
add_subdirectory(cmake)

# Enable and add tests if enabled
if(VISTA_SDK_BUILD_TESTS)
	enable_testing()
	add_subdirectory(test_gtest)
	#add_subdirectory(test_catch2)
endif()

# Print configuration summary
message(STATUS "VISTA SDK Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build tests: ${VISTA_SDK_BUILD_TESTS}")
message(STATUS "  Build examples: ${VISTA_SDK_BUILD_EXAMPLES}")
message(STATUS "  Copy resources: ${VISTA_SDK_COPY_RESOURCES}")
message(STATUS "  Resources source: ${VISTA_SDK_RESOURCE_DIR}")
message(STATUS "  Resources destination: ${VISTA_SDK_OUTPUT_DIR}/resources")
message(STATUS "  Output directory: ${VISTA_SDK_OUTPUT_DIR}")

#Set SPDLOG LEVEL TO WARN:

