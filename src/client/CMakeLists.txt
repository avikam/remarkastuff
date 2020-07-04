add_executable(client main.cpp)
target_link_libraries(
        client
        ${cppzmq_LIBRARY}
        ${ZeroMQ_LIBRARY}
)