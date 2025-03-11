cmake_minimum_required(VERSION 3.21)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "In-source builds are not supported. Please choose a different binary directory.")
	return()
endif()

set(PACKER_VERSION_MAJOR 3)
set(PACKER_VERSION_MINOR 0) 
set(PACKER_VERSION_PATCH 0)

set(PACKER_VERSION ${PACKER_VERSION_MAJOR}.${PACKER_VERSION_MINOR}.${PACKER_VERSION_PATCH})

project(Packer VERSION ${PACKER_VERSION} LANGUAGES CXX)

message("Packer Version: ${PACKER_VERSION}")

# Packer options
option(PACKER_NAMESPACE_ENABLED "Use packer namespace" ON)
option(PACKER_LOG_ENABLED "Enable logging functionallity" ON)
option(PACKER_IGNORE_FILE_ENABLED "Enable ignore file functionallity" ON)
option(PACKER_CONSOLE_FEATURES_ENABLED "Enable console features" ON)
option(PACKER_CONFIG_FILE_ENCRYPTION_ENABLED "Enable config file encryption" OFF)

# Console app options
option(PACKER_BUILD_CONSOLE_APP "Build test executable" ON)

# Test options
option(PACKER_BUILD_TESTS "Build test executable" OFF)

# Add option for C++ standard version
set(CPP_STD 17 CACHE STRING "C++ standard version")
set_property(CACHE CPP_STD PROPERTY STRINGS "14;17;20")

# Set the output directory for packer project
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/$<CONFIG>)

# Set the install prefix to the root directory
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

# Include subdirectories
add_subdirectory(source/packer)
if(PACKER_BUILD_CONSOLE_APP)
    add_subdirectory(source/console)
endif()
if(PACKER_BUILD_TESTS)
    add_subdirectory(source/tests)
endif()

# Install targets
install(TARGETS Packer DESTINATION bin)
if(PACKER_BUILD_CONSOLE_APP)
    install(TARGETS Console DESTINATION bin)
endif()
if(PACKER_BUILD_TESTS)
    install(TARGETS Tests DESTINATION bin)
endif()

# Set the startup project to console
if(PACKER_BUILD_CONSOLE_APP)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Packer)
endif()
