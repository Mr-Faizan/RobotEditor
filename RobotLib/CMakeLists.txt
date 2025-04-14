cmake_minimum_required(VERSION 3.16)

project(RobotLib LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/InstalledSoftwares/QT/6.8.1/mingw_64")

# This is for debugging
set(CMAKE_BUILD_TYPE Debug)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# Add the miniz library as a subdirectory
# add_subdirectory(libraries/miniz)

# Add the assimp library as a subdirectory
#add_subdirectory(libraries/assimp-5.4.2)


# --- Assimp setup ---
set(ASSIMP_DIR "C:/Users/fahmed/WorkFolder/Projects/RobotEditor/RobotLib/assimp-install")
include_directories(${ASSIMP_DIR}/include)
link_directories(${ASSIMP_DIR}/lib)

# --- Miniz setup ---
set(MINIZ_DIR "C:/Users/fahmed/WorkFolder/Projects/RobotEditor/RobotLib/miniz-install")
include_directories(${MINIZ_DIR}/include)
link_directories(${MINIZ_DIR}/lib)


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
  dhcalculator.h dhcalculator.cpp
)




#target_link_libraries(RobotLib PRIVATE Qt${QT_VERSION_MAJOR}::Core assimp ${ASSIMP_LIBRARIES})
#target_include_directories(RobotLib PUBLIC 
#${CMAKE_CURRENT_SOURCE_DIR}/libraries/assimp-5.4.2/include
#${CMAKE_CURRENT_BINARY_DIR}/libraries/assimp-5.4.2/include
#)

target_include_directories(RobotLib PUBLIC 
  ${ASSIMP_DIR}/include
  ${MINIZ_DIR}/include
)

target_link_libraries(RobotLib PRIVATE 
  Qt${QT_VERSION_MAJOR}::Core 
  assimp 
  miniz
)


target_compile_definitions(RobotLib PRIVATE ROBOTLIB_LIBRARY)


# Add the executable
add_executable(RobotLibTest main.cpp robot.cpp joint.cpp kinematics.cpp dynamics.cpp robotlib_test.cpp importvcmx.cpp dhcalculator.cpp)

# Link the RobotLib library to the executable
target_link_libraries(RobotLibTest PRIVATE RobotLib assimp miniz)
