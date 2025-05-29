add_subdirectory(udp)
add_subdirectory(client)
add_subdirectory(window)
add_subdirectory(component)

include_directories(${PROJECT_SOURCE_DIR}/include)

#if(WIN32)
    #set(SDL3_ttf_DIR "C:\\Users\\marti\\vcpkg\\packages\\sdl3-ttf_x64-windows\\share\\sdl3_ttf")
#else()
    #list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.local")
#endif()

#find_package(SDL3_ttf REQUIRED CONFIG)

target_link_libraries(${PROJECT_NAME} PRIVATE
    client
    context
    window
    #SDL3_image::SDL3_image
    #SDL3_ttf::SDL3_ttf
    #SDL3::SDL3
)