cmake_minimum_required(VERSION 3.16)

project(RobotLib LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(RobotLib SHARED
  RobotLib_global.h
  robotlib.cpp
  robotlib.h
  robot.cpp
  robot.h
  joint.cpp
  joint.h
  tool.cpp
  tool.h
  kinematics.cpp
  kinematics.h
  dynamics.cpp
  dynamics.h
  visualization.cpp
  visualization.h
  robotlib_test.h robotlib_test.cpp
  main.cpp
)

target_link_libraries(RobotLib PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_include_directories(RobotLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(RobotLib PRIVATE ROBOTLIB_LIBRARY)
