add_subdirectory(udp)
add_subdirectory(window)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    udp
    context
    lobbywindow
    lanlobbyclient
)