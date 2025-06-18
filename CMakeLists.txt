cmake_minimum_required(VERSION 3.14)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()

project(dungeonGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_SOURCE_DIR}/cmake/SDLDeps.cmake)

if(LINUX)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
endif()

add_executable(${PROJECT_NAME} src/main.cpp)

#target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_STATIC SDL3TTF_STATIC)

add_subdirectory(src)

install(TARGETS ${PROJECT_NAME} DESTINATION .)

if(LINUX)
    set(BUILD_DEPS build/_deps)

    # ugly but I'm tired
    install(FILES
        ${BUILD_DEPS}/sdl3-build/libSDL3.so
        ${BUILD_DEPS}/sdl3-build/libSDL3.so
        ${BUILD_DEPS}/sdl3-build/libSDL3.so

        ${BUILD_DEPS}/sdl3_image-build/libSDL3_image.so
        ${BUILD_DEPS}/sdl3_image-build/libSDL3_image.so.0
        ${BUILD_DEPS}/sdl3_image-build/libSDL3_image.so.0.2.0

        ${BUILD_DEPS}/sdl3_ttf-build/libSDL3_ttf.so
        ${BUILD_DEPS}/sdl3_ttf-build/libSDL3_ttf.so.0
        ${BUILD_DEPS}/sdl3_ttf-build/libSDL3_ttf.so.0.2.0

        DESTINATION lib
    )
endif()