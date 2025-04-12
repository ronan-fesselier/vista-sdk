cmake_minimum_required(VERSION 3.24)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(vista-sdk VERSION 0.1.0 LANGUAGES CXX)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "In-source builds are not allowed. Please use a separate build directory.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
	set(BUILD_SHARED_LIBS TRUE)
endif()

set(VISTA_SDK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(VISTA_SDK_RESOURCE_DIR ${VISTA_SDK_SOURCE_DIR}/resources)
set(VISTA_SDK_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)

option(VISTA_SDK_BUILD_TESTS "Build tests" ON)
option(VISTA_SDK_BUILD_EXAMPLES "Build examples" OFF)
option(VISTA_SDK_COPY_RESOURCES "Copy resources to build directory" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${VISTA_SDK_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${VISTA_SDK_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${VISTA_SDK_OUTPUT_DIR})

if(VISTA_SDK_COPY_RESOURCES)
	add_custom_target(CopyResources ALL
		COMMAND ${CMAKE_COMMAND} -E make_directory ${VISTA_SDK_OUTPUT_DIR}/resources
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			${VISTA_SDK_RESOURCE_DIR} ${VISTA_SDK_OUTPUT_DIR}/$<CONFIG>/resources
		COMMENT "Copying VISTA SDK resources to build directory"
	)
endif()

add_subdirectory(cpp)

message(STATUS "VISTA SDK Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build tests: ${VISTA_SDK_BUILD_TESTS}")
message(STATUS "  Build examples: ${VISTA_SDK_BUILD_EXAMPLES}")
message(STATUS "  Copy resources: ${VISTA_SDK_COPY_RESOURCES}")
message(STATUS "  Resources source: ${VISTA_SDK_RESOURCE_DIR}")
message(STATUS "  Resources destination: ${VISTA_SDK_OUTPUT_DIR}/resources")
message(STATUS "  Output directory: ${VISTA_SDK_OUTPUT_DIR}")
