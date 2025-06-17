add_library(context STATIC Context.cpp)
target_include_directories(context PUBLIC ${PROJECT_SOURCE_DIR}/include)

#if (WIN32)
    #target_link_libraries(context PRIVATE
        #SDL3_image-static
        #SDL3_ttf-static
        #SDL3-static
    #)
#elseif (LINUX)
    #target_link_libraries(context PRIVATE
        #SDL3_image::SDL3_image
        #SDL3_ttf::SDL3_ttf
        #SDL3::SDL3
    #)
#endif()

add_library(window STATIC Window.cpp LobbyWindow.cpp) #GameWindow.cpp)
target_include_directories(window PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(window PRIVATE
    client
    component
)