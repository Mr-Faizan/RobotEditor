IF(NOT EXISTS "C:/Users/fahmed/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/build/Desktop_Qt_6_8_1_MinGW_64_bit-Debug/install_manifest.txt")
  MESSAGE(FATAL_ERROR "Cannot find install manifest: \"C:/Users/fahmed/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/build/Desktop_Qt_6_8_1_MinGW_64_bit-Debug/install_manifest.txt\"")
ENDIF(NOT EXISTS "C:/Users/fahmed/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/build/Desktop_Qt_6_8_1_MinGW_64_bit-Debug/install_manifest.txt")

FILE(READ "C:/Users/fahmed/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/build/Desktop_Qt_6_8_1_MinGW_64_bit-Debug/install_manifest.txt" files)
STRING(REGEX REPLACE "\n" ";" files "${files}")
FOREACH(file ${files})
  MESSAGE(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  EXEC_PROGRAM(
    "C:/InstalledSoftwares/QT/Tools/CMake_64/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
    OUTPUT_VARIABLE rm_out
    RETURN_VALUE rm_retval
    )
  IF(NOT "${rm_retval}" STREQUAL 0)
    MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
  ENDIF(NOT "${rm_retval}" STREQUAL 0)
ENDFOREACH(file)
