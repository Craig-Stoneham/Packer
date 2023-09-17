cmake_minimum_required(VERSION 3.21)

# Extract version information from Packer/Version.h
file(STRINGS "Source/Packer/Version.h" VERSION_HEADER_CONTENT REGEX "#define VERSION_(MAJOR|MINOR|PATCH) [0-9]+")

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
project(Packer VERSION ${VERSION_STRING})

# Set policy version explicitly
cmake_policy(SET CMP0076 NEW)

# Define source files for targets
file(GLOB Packer_Headers
    Source/Packer/*.h
)

file(GLOB Packer_Sources
    Source/Packer/*.cpp
)

file(GLOB Console_Headers
    Source/Console/*.h
)

file(GLOB Console_Sources
    Source/Console/*.cpp
)

file(GLOB Tests_Headers
    Source/Tests/*.h
)

file(GLOB Tests_Sources
    Source/Tests/*.cpp
)

# Add resource file conditionally for Windows
if(WIN32)
    list(APPEND Console_Sources
        Source/Console/Resources.rc
    )
endif()

# Set the output directory for Packer project
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin/$<CONFIG>)

# Create Console target
add_executable(Console ${Packer_Headers} ${Packer_Sources} ${Console_Headers} ${Console_Sources})
target_include_directories(Console PRIVATE Source/Packer)
target_compile_features(Console PRIVATE cxx_std_17)
target_compile_options(Console PRIVATE /bigobj /Zc:__cplusplus)

# Option for building Tests
option(BUILD_TESTS "Build test executable" OFF)
if(BUILD_TESTS)
    # Create Tests target
    add_executable(Tests ${Packer_Headers} ${Packer_Sources} ${Tests_Headers} ${Tests_Sources})
    target_include_directories(Tests PRIVATE Source/Packer Source/Tests)
    target_compile_features(Tests PRIVATE cxx_std_17)
    target_compile_options(Tests PRIVATE /bigobj /Zc:__cplusplus)
endif()

# Set the startup project to Console
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Console)

# Install targets
install(TARGETS Console DESTINATION bin)
if(BUILD_TESTS)
    install(TARGETS Tests DESTINATION bin)
endif()

# Source groups
source_group("Packer/Headers" FILES ${Packer_Headers})
source_group("Packer/Source" FILES ${Packer_Sources})
source_group("Console/Headers" FILES ${Console_Headers})
source_group("Console/Source" FILES ${Console_Sources})
source_group("Tests/Headers" FILES ${Tests_Headers})
source_group("Tests/Source" FILES ${Tests_Sources})
