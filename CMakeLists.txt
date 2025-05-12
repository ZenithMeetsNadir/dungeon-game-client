cmake_minimum_required(VERSION 4.0)
project(dungeonGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(src/udp)

# Find SDL3
set(SDL3_DIR "C:\\Users\\marti\\vcpkg\\packages\\sdl3_x64-windows\\share\\sdl3")
find_package(SDL3 REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    udp
    SDL3::SDL3
    OpenGL::GL
)
