set(PUBLIC_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

set(PUBLIC_FILES
    test_config_file.h
    test_crypto.h
    test_packer.h
    test_suite.h
    test_variant.h
)

set(PRIVATE_FILES
    main.cpp
    test_config_file.cpp
    test_crypto.cpp
    test_packer.cpp
    test_suite.cpp
    test_variant.cpp
)

set(ALL_FILES ${PUBLIC_FILES} ${PRIVATE_FILES})

add_executable(Tests ${ALL_FILES})

set_target_properties(Tests PROPERTIES FOLDER "Tests")

source_group("headers" FILES ${PUBLIC_FILES})
source_group("source" FILES ${PRIVATE_FILES})

target_link_libraries(Tests PUBLIC Packer)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "" FILES ${ALL_FILES})

target_include_directories(Tests PUBLIC ${PUBLIC_DIRS})
target_compile_features(Tests PRIVATE cxx_std_${CPP_STD})
