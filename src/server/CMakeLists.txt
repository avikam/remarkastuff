include_directories(/Volumes/LinuxSource/linux-4.0.1/usr/include/)

add_executable(
        server
        main.cpp
        FbFile.cpp
)

target_link_libraries(
        server
        ${cppzmq_LIBRARY}
        ${ZeroMQ_LIBRARY}
)