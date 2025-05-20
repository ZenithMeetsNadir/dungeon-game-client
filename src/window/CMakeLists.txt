add_library(context Context.cpp)
target_include_directories(context PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(WIN32)
    #set(SDL3_DIR "C:\\Users\\marti\\vcpkg\\packages\\sdl3_x64-windows\\share\\sdl3")
    #set(SDL3_ttf_DIR "C:\\Users\\marti\\vcpkg\\packages\\sdl3-ttf_x64-windows\\share\\sdl3_ttf")
else()
    list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.local")
endif()

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)

target_link_libraries(context
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
)

add_library(window Window.cpp LobbyWindow.cpp)
target_include_directories(window PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(window PRIVATE
    client
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
)