cmake_minimum_required(VERSION 3.21)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "In-source builds are not supported. Please choose a different binary directory.")
	return()
endif()

function(extract_version_info FILE_NAME OUTPUT_VAR)
    if(NOT EXISTS ${FILE_NAME})
        message(FATAL_ERROR "File ${FILE_NAME} not found.")
    endif()

    file(READ ${FILE_NAME} VERSION_HEADER_CONTENT)
    string(REPLACE " " "" VERSION_HEADER_CONTENT ${VERSION_HEADER_CONTENT})

    string(REGEX MATCH "major=([0-9]+)" _ ${VERSION_HEADER_CONTENT})
    set(VERSION_MAJOR ${CMAKE_MATCH_1})

    string(REGEX MATCH "minor=([0-9]+)" _ ${VERSION_HEADER_CONTENT})
    set(VERSION_MINOR ${CMAKE_MATCH_1})

    string(REGEX MATCH "patch=([0-9]+)" _ ${VERSION_HEADER_CONTENT})
    set(VERSION_PATCH ${CMAKE_MATCH_1})

    set(${OUTPUT_VAR} "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" PARENT_SCOPE)
endfunction()

extract_version_info("version.py" PACKER_VERSION)
message("Packer Version: ${PACKER_VERSION}")

project(packer VERSION ${PACKER_VERSION} LANGUAGES CXX)

file(GLOB packer_headers source/packer/*.h)
file(GLOB packer_sources source/packer/*.cpp)
file(GLOB console_headers source/console/*.h)
file(GLOB console_sources source/console/*.cpp)
file(GLOB test_headers source/test/*.h)
file(GLOB test_sources source/test/*.cpp)

# Add resource file conditionally for Windows
if(WIN32)
    list(APPEND console_resources
        source/console/resources.rc
    )
endif()

# Add the option for packer namespace
option(PACKER_NAMESPACE_ENABLED "Use packer namespace" ON)

# Add the option for logging functionallity
option(LOG_ENABLED "Enabled logging functionallity" ON)

# Add the options for ignore file functionallity
option(IGNORE_FILE_ENABLED "Enabled ignore file functionallity" ON)

# Add the option for console features
option(CONSOLE_FEATURES_ENABLED "Enabled console features" ON)

# Add the option for config file encryption
option(CONFIG_FILE_ENCRYPTION_ENABLED "Enabled config file encryption" OFF)

# Add option for C++ standard version
set(CPP_STD 17 CACHE STRING "C++ standard version")
set_property(CACHE CPP_STD PROPERTY STRINGS "14;17;20")

# Set the output directory for packer project
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/$<CONFIG>)

# Set the install prefix to the root directory
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

# Create packer target
add_executable(Packer ${packer_headers} ${packer_sources} ${console_headers} ${console_sources} ${console_resources})
target_include_directories(Packer PRIVATE source/packer)
target_compile_features(Packer PRIVATE cxx_std_${CPP_STD})
target_compile_options(Packer PRIVATE ${COMMON_COMPILE_OPTIONS})

# Conditionally add the preprocessor definition
if(PACKER_NAMESPACE_ENABLED)
    target_compile_definitions(Packer PUBLIC PACKER_NAMESPACE_ENABLED)
endif()
if(LOG_ENABLED)
    target_compile_definitions(Packer PUBLIC LOG_ENABLED)
endif()
if(IGNORE_FILE_ENABLED)
    target_compile_definitions(Packer PUBLIC IGNORE_FILE_ENABLED)
endif()
if(CONSOLE_FEATURES_ENABLED)
    target_compile_definitions(Packer PUBLIC CONSOLE_FEATURES_ENABLED)
endif()
if(CONFIG_FILE_ENCRYPTION_ENABLED)
    target_compile_definitions(Packer PUBLIC CONFIG_FILE_ENCRYPTION_ENABLED)
endif()

# Option for building test
option(BUILD_TESTS "Build test executable" OFF)
if(BUILD_TESTS)
    # Create test target
    add_executable(test ${packer_headers} ${packer_sources} ${test_headers} ${test_sources})
    target_include_directories(test PRIVATE source/packer source/test)    
    target_compile_features(test PRIVATE cxx_std_${CPP_STD})
    target_compile_options(test PRIVATE ${COMMON_COMPILE_OPTIONS})
    
    # Conditionally add the preprocessor definition for test as well
    if(PACKER_NAMESPACE_ENABLED)
        target_compile_definitions(test PUBLIC PACKER_NAMESPACE_ENABLED)
    endif()
    if(LOG_ENABLED)
        target_compile_definitions(test PUBLIC LOG_ENABLED)
    endif()
    if(IGNORE_FILE_ENABLED)
        target_compile_definitions(test PUBLIC IGNORE_FILE_ENABLED)
    endif()
    if(CONSOLE_FEATURES_ENABLED)
        target_compile_definitions(test PUBLIC CONSOLE_FEATURES_ENABLED)
    endif()
endif()

# Set the startup project to console
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Packer)

# Install targets
install(TARGETS Packer DESTINATION bin)
if(BUILD_TESTS)
    install(TARGETS test DESTINATION bin)
endif()

# Source groups
source_group("packer/headers" FILES ${packer_headers})
source_group("packer/source" FILES ${packer_sources})
source_group("console/headers" FILES ${console_headers})
source_group("console/source" FILES ${console_sources})
if(WIN32)
source_group("console/resources" FILES ${console_resources})
endif()
source_group("test/headers" FILES ${test_headers})
source_group("test/source" FILES ${test_sources})
