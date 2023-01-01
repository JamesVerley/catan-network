cmake_minimum_required(VERSION 3.14...3.24)

project(sockets_test)

add_subdirectory(/libs/asio-grpc)
target_link_libraries(your_app PUBLIC asio-grpc::asio-grpc-standalone-asio)
