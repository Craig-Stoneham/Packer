set(PUBLIC_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

set(PUBLIC_FILES
    console_app.h
)

set(PRIVATE_FILES
    console_app.cpp
    main.cpp
)

if(WIN32)
    set(RESOURCE_FILES
        resources.rc
    )
endif()

set(ALL_FILES ${PUBLIC_FILES} ${PRIVATE_FILES} ${RESOURCE_FILES})

add_executable(Console ${ALL_FILES})

set_target_properties(Console PROPERTIES FOLDER "Console")

source_group("headers" FILES ${PUBLIC_FILES})
source_group("source" FILES ${PRIVATE_FILES})
source_group("resources" FILES ${RESOURCE_FILES})

target_link_libraries(Console PUBLIC Packer)

target_include_directories(Console PUBLIC ${PUBLIC_DIRS})
target_compile_features(Console PRIVATE cxx_std_${CPP_STD})
