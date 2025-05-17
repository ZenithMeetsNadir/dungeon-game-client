add_library(context Context.cpp)
target_include_directories(context PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Find SDL3 (platform-specific)
if(WIN32)
    set(SDL3_DIR "C:\\Users\\marti\\vcpkg\\packages\\sdl3_x64-windows\\share\\sdl3")
    find_package(SDL3 REQUIRED CONFIG)

    set(SDL3_ttf_DIR "C:\\Users\\marti\\vcpkg\\packages\\sdl3-ttf_x64-windows\\share\\sdl3_ttf")
    find_package(SDL3_ttf REQUIRED CONFIG)
else()
    # no idea what's going on here
    find_path(SDL3_INCLUDE_DIR SDL.h PATH_SUFFIXES SDL3 PATHS /usr/local/include)
    find_library(SDL3_LIBRARY SDL3 PATHS /usr/local/lib)
    
    if(NOT SDL3_INCLUDE_DIR OR NOT SDL3_LIBRARY)
        message(FATAL_ERROR "SDL3 not found")
    endif()

    add_library(SDL3::SDL3 UNKNOWN IMPORTED)
    set_target_properties(SDL3::SDL3 PROPERTIES
        IMPORTED_LOCATION "${SDL3_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL3_INCLUDE_DIR}"
    )
endif()

# OpenGL
find_package(OpenGL REQUIRED)

target_link_libraries(context
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
)

add_library(lanlobbyclient LanLobbyClient.cpp)
target_include_directories(lanlobbyclient PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(lanlobbyclient PRIVATE udp)

add_library(window Window.cpp)
target_include_directories(window PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(window 
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
)

add_library(lobbywindow LobbyWindow.cpp)
target_include_directories(lobbywindow PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(lobbywindow window)