cmake_minimum_required(VERSION 2.8.3)
project(sine_servo)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)


catkin_package()


include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  #/usr/include/modbus
  /usr/local/include/modbus
)



add_executable(sine_listen test/sine_listen.cpp)
#target_link_libraries(sine_listen ${catkin_LIBRARIES}  /usr/lib/x86_64-linux-gnu/libmodbus.so.5.0.5)
target_link_libraries(sine_listen ${catkin_LIBRARIES}  /usr/local/lib/libmodbus.so.5.1.0)
add_dependencies(sine_listen beginner_tutorials_generate_messages_cpp) 
