cmake_minimum_required(VERSION 3.14)

project(dungeonGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_SOURCE_DIR}/cmake/SDLDeps.cmake)

add_executable(${PROJECT_NAME} src/main.cpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_STATIC SDL3TTF_STATIC)

add_subdirectory(src)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION .
)