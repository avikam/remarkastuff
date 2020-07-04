cmake_minimum_required(VERSION 3.16)
project(remark)

set(CMAKE_CXX_STANDARD 17)

find_package(cppzmq)
find_package(ZeroMQ)

include_directories(src ${cppzmq_INCLUDE_DIR})

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^arm.*")
    add_link_options(-pthread -static)
endif()

add_subdirectory(src/server)
add_subdirectory(src/client)