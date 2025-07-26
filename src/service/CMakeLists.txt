add_library(service STATIC ServiceManager.cpp DotenvRepository.cpp)
target_include_directories(service PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(service PRIVATE 
    client 
    util
)
