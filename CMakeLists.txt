cmake_minimum_required(VERSION 2.8.3)
project(dmotion)
add_definitions(-std=c++11 -Wall -O2)

include_directories(/usr/include/modbus)

add_executable(AC_servo serial_try.cpp)
target_link_libraries(AC_servo /usr/lib/x86_64-linux-gnu/libmodbus.so.5.0.5)
