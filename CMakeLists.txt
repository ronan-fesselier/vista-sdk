#
#	VISTA-SDK-CPP
#
#

cmake_minimum_required(VERSION 3.18)

# Set the VCPKG toolchain file if VCPKG_ROOT is defined
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Define the project
project(vista-sdk-cpp VERSION 0.1.0 LANGUAGES CXX)

# Prevent in-source builds
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "In-source builds are not allowed. Please use a separate build directory.")
endif()


set(VISTA_SDK_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
set(VISTA_SDK_TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/testdata)

add_subdirectory(cpp)
