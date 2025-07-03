add_library(util STATIC Dotenv.cpp)
target_include_directories(util PUBLIC ${PROJECT_SOURCE_DIR}/include)
