add_library(client STATIC LanLobbyClient.cpp GameClient.cpp)
target_include_directories(client PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(client PRIVATE 
    sdl_deps
    udp 
    tcp 
    util
) 
