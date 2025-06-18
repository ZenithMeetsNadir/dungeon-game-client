cmake_minimum_required(VERSION 3.14)

if(NOT DEFINED SDL_DEPS_INCLUDED)
  set(SDL_DEPS_INCLUDED TRUE)
  
  include(FetchContent)

  if(WIN32)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  endif()

  set(SDL_VIDEO_DRIVER_X11 OFF CACHE BOOL "" FORCE)
  set(SDL_X11_XINPUT2 OFF CACHE BOOL "" FORCE)

  FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.0
  )
  FetchContent_MakeAvailable(SDL3)

  FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-3.2.0
  )
  set(SDL3IMAGE_BACKEND_STB OFF CACHE BOOL "" FORCE)
  set(SDL3IMAGE_PNG ON CACHE BOOL "" FORCE)
  set(SDL3IMAGE_INSTALL_PLUGINS OFF CACHE BOOL "" FORCE)
  set(SDL3IMAGE_SAMPLES OFF CACHE BOOL "" FORCE)
  set(SDL3IMAGE_TESTS OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(SDL3_image)

  if(WIN32)
    find_package(SDL3_ttf CONFIG REQUIRED)
  elseif(LINUX)
    FetchContent_Declare(
      SDL3_ttf
      GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
      GIT_TAG release-3.2.0
    )
    FetchContent_MakeAvailable(SDL3_ttf)
  endif()
endif()