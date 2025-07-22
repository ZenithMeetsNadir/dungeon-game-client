add_library(context STATIC Context.cpp)
target_include_directories(context PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(context PRIVATE
    sdl_deps
)

add_library(window STATIC WindowManager.cpp WindowRouter.cpp ServiceManager.cpp Window.cpp LobbyWindow.cpp GameWindow.cpp ConnectingWindow.cpp)
target_include_directories(window PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(window PRIVATE
    context
    client
    component
    util
    sdl_deps
)