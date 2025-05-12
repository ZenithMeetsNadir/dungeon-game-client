cmake_minimum_required(VERSION 3.14)
project(dungeonGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define source files
file(GLOB_RECURSE SOURCES src/main.cpp)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(src/udp)

# Find SDL3 (platform-specific)
if(WIN32)
    set(SDL3_DIR "C:\\Users\\marti\\vcpkg\\packages\\sdl3_x64-windows\\share\\sdl3")
    find_package(SDL3 REQUIRED)
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

# Link libraries
target_link_libraries(${PROJECT_NAME}
    udp
    SDL3::SDL3
    OpenGL::GL
)
