add_library(udp udpServer.cpp)
target_link_libraries(udp ws2_32)