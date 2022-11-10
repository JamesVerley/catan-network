cmake_minimum_required(VERSION 3.22)
project(catan)

set(CMAKE_CXX_STANDARD 20)

add_executable(catan main.cpp json.h net.common.h gamecore.h core_unittest.h)

