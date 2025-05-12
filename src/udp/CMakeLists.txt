add_library(udp udpServer.cpp)

if(WIN32)
    target_link_libraries(udp ws2_32)
endif()