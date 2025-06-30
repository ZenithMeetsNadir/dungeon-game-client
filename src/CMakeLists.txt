add_library(sdl_deps INTERFACE)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(net)
add_subdirectory(client)
add_subdirectory(window)
add_subdirectory(component)

target_link_libraries(sdl_deps INTERFACE
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3::SDL3
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    client
    context
    window
    sdl_deps
)

if(WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/external/libpng/include
        ${PROJECT_SOURCE_DIR}/external/zlib/include
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/external/libpng/lib/libpng.a
        ${PROJECT_SOURCE_DIR}/external/zlib/lib/libzs.a
    )
endif()