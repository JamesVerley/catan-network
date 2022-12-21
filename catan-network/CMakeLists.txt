cmake_minimum_required(VERSION 3.22)
project(catan)

set(CMAKE_CXX_STANDARD 20)

add_executable(catan ../api/net.common.h ../core/gamecore.h ../test/core_unittest.h ../api/message_handler.h ../lobby/lobby.h)
