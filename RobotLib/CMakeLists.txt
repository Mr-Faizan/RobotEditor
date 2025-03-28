cmake_minimum_required(VERSION 3.16)

project(RobotLib LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# Add the miniz library as a subdirectory
add_subdirectory(libraries/miniz)
# Add the assimp library as a subdirectory
add_subdirectory(libraries/assimp)

add_library(RobotLib SHARED
  RobotLib_global.h
  robotlib.h
  robotlib.cpp
  robot.h
  robot.cpp
  joint.h
  joint.cpp
  kinematics.h
  kinematics.cpp
  dynamics.h
  dynamics.cpp
  robotlib_test.h robotlib_test.cpp
  main.cpp
  jsonkeys2.h
  importvcmx.h importvcmx.cpp
)



target_link_libraries(RobotLib PRIVATE Qt${QT_VERSION_MAJOR}::Core miniz assimp)
target_include_directories(RobotLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(RobotLib PRIVATE ROBOTLIB_LIBRARY)


# Add the executable
add_executable(RobotLibTest main.cpp robot.cpp joint.cpp kinematics.cpp dynamics.cpp robotlib_test.cpp)

# Link the RobotLib library to the executable
target_link_libraries(RobotLibTest PRIVATE RobotLib miniz assimp)
