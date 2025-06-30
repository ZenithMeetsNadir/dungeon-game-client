add_library(tcp STATIC TcpClient.cpp)
target_include_directories(tcp PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(tcp net_deps)


if(WIN32)
    target_link_libraries(tcp ws2_32)
endif()