# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\RobotEditor_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\RobotEditor_autogen.dir\\ParseCache.txt"
  "RobotEditor_autogen"
  "RobotLib\\CMakeFiles\\RobotLibTest_autogen.dir\\AutogenUsed.txt"
  "RobotLib\\CMakeFiles\\RobotLibTest_autogen.dir\\ParseCache.txt"
  "RobotLib\\CMakeFiles\\RobotLib_autogen.dir\\AutogenUsed.txt"
  "RobotLib\\CMakeFiles\\RobotLib_autogen.dir\\ParseCache.txt"
  "RobotLib\\RobotLibTest_autogen"
  "RobotLib\\RobotLib_autogen"
  "RobotLib\\libraries\\miniz\\CMakeFiles\\miniz_autogen.dir\\AutogenUsed.txt"
  "RobotLib\\libraries\\miniz\\CMakeFiles\\miniz_autogen.dir\\ParseCache.txt"
  "RobotLib\\libraries\\miniz\\miniz_autogen"
  )
endif()
