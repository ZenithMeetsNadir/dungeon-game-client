add_library(udp UdpServer.cpp)
target_include_directories(udp PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(dataPacker dataPacker.cpp)
target_include_directories(dataPacker PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(udp dataPacker)

if(WIN32)
    target_link_libraries(udp ws2_32)
endif()