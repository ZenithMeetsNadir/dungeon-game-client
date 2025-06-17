add_subdirectory(udp)
add_subdirectory(client)
add_subdirectory(window)
add_subdirectory(component)

target_link_libraries(${PROJECT_NAME} PRIVATE
    client
    context
    window
)

if(WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/external/libpng/include
        ${PROJECT_SOURCE_DIR}/external/zlib/include
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL3_image-static
        SDL3_ttf::SDL3_ttf
        SDL3-static
        ${PROJECT_SOURCE_DIR}/external/libpng/lib/libpng.a
        ${PROJECT_SOURCE_DIR}/external/zlib/lib/libzs.a
    )
elseif(LINUX)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        SDL3::SDL3
    )
endif()