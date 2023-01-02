cmake_minimum_required(VERSION 3.14...3.24)

project(sockets_test)

add_subdirectory(libs/asio-grpc)
#add_subdirectory(libs/asio/asio/include/asio)

include_directories(libs/asio/asio/include/asio)

add_executable(sockets_test test.cpp)

set_property(TARGET sockets_test PROPERTY CXX_STANDARD 17)

target_link_libraries(sockets_test PUBLIC asio-grpc::asio-grpc-standalone-asio)
