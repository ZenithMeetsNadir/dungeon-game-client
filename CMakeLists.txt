cmake_minimum_required(VERSION 3.14)

project(dungeonGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} src/main.cpp)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION .
)

include(${CMAKE_SOURCE_DIR}/cmake/SDLDeps.cmake)

add_subdirectory(src)
