add_library(udp STATIC UdpServer.cpp UdpClient.cpp)
target_include_directories(udp PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(datapacker STATIC DataPacker.cpp)
target_include_directories(datapacker PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(udp datapacker)

if(WIN32)
    target_link_libraries(udp ws2_32)
endif()