add_subdirectory(udp)
add_subdirectory(client)
add_subdirectory(window)
add_subdirectory(component)

include_directories(${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE
    client
    context
    window
    #SDL3_image::SDL3_image
    #SDL3_ttf::SDL3_ttf
    #SDL3::SDL3
)