add_library(udp udpServer.cpp)
add_library(dataPacker dataPacker.cpp)
target_link_libraries(udp dataPacker)

if(WIN32)
    target_link_libraries(udp ws2_32)
endif()