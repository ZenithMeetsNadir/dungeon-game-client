add_library(context STATIC Context.cpp)
target_include_directories(context PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(WIN32)
    #set(SDL3_ttf_DIR "C:\\Users\\marti\\vcpkg\\packages\\sdl3-ttf_x64-windows\\share\\sdl3_ttf")
else()
    list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.local")
endif()

find_package(SDL3_ttf REQUIRED CONFIG)

target_link_libraries(context PUBLIC
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3::SDL3
)

add_library(window STATIC Window.cpp LobbyWindow.cpp GameWindow.cpp)
target_include_directories(window PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(window PUBLIC
    client
    component
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3::SDL3
)