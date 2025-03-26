# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\RobotLibTest_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\RobotLibTest_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\RobotLib_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\RobotLib_autogen.dir\\ParseCache.txt"
  "RobotLibTest_autogen"
  "RobotLib_autogen"
  )
endif()
