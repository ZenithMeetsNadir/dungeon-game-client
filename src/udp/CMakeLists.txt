add_library(udp UdpServer.cpp UdpClient.cpp)
target_include_directories(udp PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(datapacker DataPacker.cpp)
target_include_directories(datapacker PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(udp datapacker)

if(WIN32)
    target_link_libraries(udp ws2_32)
endif()