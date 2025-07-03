add_library(component STATIC 
    Component.cpp 
    FocusComponent.cpp
    Button.cpp 
    SelectButton.cpp 
    textInput/TextInput.cpp
    textInput/IpInput.cpp
    PauseOverlay.cpp
)
target_include_directories(component PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(component PRIVATE
    context
    sdl_deps
)