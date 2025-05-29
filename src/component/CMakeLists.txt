add_library(component STATIC 
    Component.cpp 
    Button.cpp 
    SelectButton.cpp 
    PauseOverlay.cpp
)
#target_include_directories(component PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(WIN32)
    #set(SDL3_ttf_DIR "C:\\Users\\marti\\vcpkg\\packages\\sdl3-ttf_x64-windows\\share\\sdl3_ttf")
else()
    list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.local")
endif()

find_package(SDL3_ttf REQUIRED CONFIG)

target_link_libraries(component PUBLIC
    context
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3::SDL3
)