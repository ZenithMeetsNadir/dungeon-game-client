add_library(net_deps INTERFACE)

add_subdirectory(udp)
add_subdirectory(tcp)

add_library(netutil STATIC IPv4Addr.cpp netutil.cpp)
target_include_directories(netutil PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(datapacker STATIC DataPacker.cpp)
target_include_directories(datapacker PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(net_deps INTERFACE
    netutil
    datapacker
)