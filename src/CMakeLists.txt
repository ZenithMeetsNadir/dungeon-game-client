add_subdirectory(udp)
add_subdirectory(client)
add_subdirectory(window)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    client
    context
    window
)