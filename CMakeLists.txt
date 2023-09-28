cmake_minimum_required(VERSION 3.21)

# Extract version information from packer/version.h
file(STRINGS "source/packer/version.h" VERSION_HEADER_CONTENT REGEX "#define VERSION_(MAJOR|MINOR|PATCH) [0-9]+")

# Extract the version numbers from the content
string(REGEX MATCH "#define VERSION_MAJOR ([0-9]+)" _ ${VERSION_HEADER_CONTENT})
set(VERSION_MAJOR ${CMAKE_MATCH_1})

string(REGEX MATCH "#define VERSION_MINOR ([0-9]+)" _ ${VERSION_HEADER_CONTENT})
set(VERSION_MINOR ${CMAKE_MATCH_1})

string(REGEX MATCH "#define VERSION_PATCH ([0-9]+)" _ ${VERSION_HEADER_CONTENT})
set(VERSION_PATCH ${CMAKE_MATCH_1})

# Create a string representation of the version in the format "MAJOR.MINOR.PATCH"
set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

message("Packer Version: ${VERSION_STRING}")

# Set project name and version
project(packer VERSION ${VERSION_STRING})

# Set policy version explicitly
cmake_policy(SET CMP0076 NEW)

# Define source files for targets
file(GLOB packer_headers
    source/packer/*.h
)

file(GLOB packer_sources
    source/packer/*.cpp
)

file(GLOB console_headers
    source/console/*.h
)

file(GLOB console_sources
    source/console/*.cpp
)

file(GLOB test_headers
    source/test/*.h
)

file(GLOB test_sources
    source/test/*.cpp
)

# Add resource file conditionally for Windows
if(WIN32)
    list(APPEND console_resources
        source/console/resources.rc
    )
endif()

# Common compile options
set(COMMON_COMPILE_OPTIONS "/Zc:__cplusplus")

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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)

# Set the install prefix to the root directory
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

# Create console target
add_executable(console ${packer_headers} ${packer_sources} ${console_headers} ${console_sources} ${console_resources})
target_include_directories(console PRIVATE source/packer)
set_target_properties(console PROPERTIES
    CXX_STANDARD ${CPP_STD}
    CXX_STANDARD_REQUIRED ON
)
target_compile_options(console PRIVATE ${COMMON_COMPILE_OPTIONS})

# Conditionally add the preprocessor definition
if(PACKER_NAMESPACE_ENABLED)
    target_compile_definitions(console PUBLIC PACKER_NAMESPACE_ENABLED)
endif()
if(LOG_ENABLED)
    target_compile_definitions(console PUBLIC LOG_ENABLED)
endif()
if(IGNORE_FILE_ENABLED)
    target_compile_definitions(console PUBLIC IGNORE_FILE_ENABLED)
endif()
if(CONSOLE_FEATURES_ENABLED)
    target_compile_definitions(console PUBLIC CONSOLE_FEATURES_ENABLED)
endif()
if(CONFIG_FILE_ENCRYPTION_ENABLED)
    target_compile_definitions(console PUBLIC CONFIG_FILE_ENCRYPTION_ENABLED)
endif()

# Option for building test
option(BUILD_TESTS "Build test executable" OFF)
if(BUILD_TESTS)
    # Create test target
    add_executable(test ${packer_headers} ${packer_sources} ${test_headers} ${test_sources})
    target_include_directories(test PRIVATE source/packer source/test)
    set_target_properties(test PROPERTIES
        CXX_STANDARD ${CPP_STD}
        CXX_STANDARD_REQUIRED ON
    )
    target_compile_options(test PRIVATE ${COMMON_COMPILE_OPTIONS})
    
    # Conditionally add the preprocessor definition for test as well
    if(PACKER_NAMESPACE_ENABLED)
        target_compile_definitions(test PUBLIC PACKER_NAMESPACE_ENABLED)
    endif()
    if(LOG_ENABLED)
        target_compile_definitions(console PUBLIC LOG_ENABLED)
    endif()
    if(IGNORE_FILE_ENABLED)
        target_compile_definitions(console PUBLIC IGNORE_FILE_ENABLED)
    endif()
    if(CONSOLE_FEATURES_ENABLED)
        target_compile_definitions(console PUBLIC CONSOLE_FEATURES_ENABLED)
    endif()
endif()

# Set the startup project to console
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT console)

# Install targets
install(TARGETS console DESTINATION bin)
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
