set(PUBLIC_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

set(PUBLIC_FILES
    config_file.h
    console.h
    crypto.h
    error.h
    log.h
    log_file.h
    packer.h
    variant.h
    version.h
)

set(PRIVATE_FILES
    config_file.cpp
    console.cpp
    crypto.cpp
    error.cpp
    log.cpp
    log_file.cpp
    packer.cpp
    variant.cpp
)

set(ALL_FILES ${PUBLIC_FILES} ${PRIVATE_FILES})

add_library(Packer STATIC ${ALL_FILES})

set_target_properties(Packer PROPERTIES FOLDER "Packer")

source_group("headers" FILES ${PUBLIC_FILES})
source_group("source" FILES ${PRIVATE_FILES})

target_include_directories(Packer PUBLIC ${PUBLIC_DIRS})
target_compile_features(Packer PRIVATE cxx_std_${CPP_STD})

target_compile_definitions(Packer PUBLIC VERSION_MAJOR=${PACKER_VERSION_MAJOR})
target_compile_definitions(Packer PUBLIC VERSION_MINOR=${PACKER_VERSION_MINOR})
target_compile_definitions(Packer PUBLIC VERSION_PATCH=${PACKER_VERSION_PATCH})

if(PACKER_NAMESPACE_ENABLED)
    target_compile_definitions(Packer PUBLIC PACKER_NAMESPACE_ENABLED)
endif()
if(PACKER_LOG_ENABLED)
    target_compile_definitions(Packer PUBLIC LOG_ENABLED)
endif()
if(PACKER_IGNORE_FILE_ENABLED)
    target_compile_definitions(Packer PUBLIC IGNORE_FILE_ENABLED)
endif()
if(PACKER_CONSOLE_FEATURES_ENABLED)
    target_compile_definitions(Packer PUBLIC CONSOLE_FEATURES_ENABLED)
endif()
if(PACKER_CONFIG_FILE_ENCRYPTION_ENABLED)
    target_compile_definitions(Packer PUBLIC CONFIG_FILE_ENCRYPTION_ENABLED)
endif()

