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

# Set global compiler flags - this is needed
add_compile_options(/bigobj)

# Set c plus plus flag needed for filesystem
add_compile_options(/Zc:__cplusplus)

# Define source files for targets
set(Packer_Headers
    Source/Packer/ConfigFile.h
    Source/Packer/Console.h
    Source/Packer/Crypto.h
    Source/Packer/Defaults.h
    Source/Packer/Error.h
    Source/Packer/Log.h
    Source/Packer/LogFile.h
    Source/Packer/Packer.h
    Source/Packer/Typedefs.h
    Source/Packer/Variant.h
    Source/Packer/Version.h
)

set(Packer_Sources
    Source/Packer/ConfigFile.cpp
    Source/Packer/Console.cpp
    Source/Packer/Crypto.cpp
    Source/Packer/Error.cpp
    Source/Packer/Log.cpp
    Source/Packer/LogFile.cpp
    Source/Packer/Packer.cpp
    Source/Packer/Typedefs.cpp
    Source/Packer/Variant.cpp
)

set(Console_Headers
    Source/Console/Application.h
    Source/Console/Defaults.h
    Source/Console/Typedefs.h
)

set(Console_Sources
    Source/Console/Application.cpp
    Source/Console/Main.cpp
)

set(Tests_Headers
    Source/Tests/TestConfigFile.h
    Source/Tests/TestCrypto.h
    Source/Tests/TestPacker.h
    Source/Tests/TestSuite.h
    Source/Tests/TestVariant.h
    Source/Tests/Typedefs.h
)

set(Tests_Sources
    Source/Tests/Main.cpp
    Source/Tests/TestConfigFile.cpp
    Source/Tests/TestCrypto.cpp
    Source/Tests/TestPacker.cpp
    Source/Tests/TestSuite.cpp
    Source/Tests/TestVariant.cpp
)

# Add resource file conditionally for Windows
if(WIN32)
    list(APPEND Console_Sources
        Source/Console/Resources.rc
    )
endif()
        
# Set the output directory for Packer project
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin/$<CONFIG>/Console)

source_group("Packer/Headers" FILES ${Packer_Headers})
source_group("Packer/Source" FILES ${Packer_Sources})

source_group("Console/Headers" FILES ${Console_Headers})
source_group("Console/Source" FILES ${Console_Sources})

# Create Packer target
add_executable(Packer ${Packer_Headers} ${Packer_Sources} ${Console_Headers} ${Console_Sources})

# Set the output directory for Tests project
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin/$<CONFIG>/Tests)

source_group("Packer/Headers" FILES ${Packer_Headers})
source_group("Packer/Source" FILES ${Packer_Sources})

source_group("Tests/Headers" FILES ${Tests_Headers})
source_group("Tests/Source" FILES ${Tests_Sources})

# Create Tests target
add_executable(Tests ${Packer_Headers} ${Packer_Sources} ${Tests_Headers} ${Tests_Sources})
