cmake_minimum_required(VERSION 2.8.3)
project(dmotion)
add_definitions(-std=c++11 -Wall -g -O2)

include_directories(/usr/include/modbus)

add_executable(AC_servo test/serial_try.cpp)
target_link_libraries(AC_servo /usr/lib/x86_64-linux-gnu/libmodbus.so.5.0.5)

add_executable(Torque_servo test/torque.cpp)
target_link_libraries(Torque_servo /usr/lib/x86_64-linux-gnu/libmodbus.so.5.0.5)
