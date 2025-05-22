cmake_minimum_required(VERSION 3.14)

if(NOT DEFINED SDL_DEPS_INCLUDED)
  set(SDL_DEPS_INCLUDED TRUE)
  
  include(FetchContent)

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
  set(SDL3IMAGE_BACKEND_STB ON CACHE BOOL "" FORCE)
  set(SDL3IMAGE_PNG ON CACHE BOOL "" FORCE)
  set(SDL3IMAGE_INSTALL_PLUGINS OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(SDL3_image)

  # hope to integrate SDL3_ttf in the future... 
endif()