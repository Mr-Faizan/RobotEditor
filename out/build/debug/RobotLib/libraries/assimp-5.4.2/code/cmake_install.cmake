# Install script for directory: H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/RobotEditor")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.4.1-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "H:/WorkFolder/Projects/RobotEditor/out/build/debug/RobotLib/libraries/assimp-5.4.2/lib/assimp-vc143-mtd.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/anim.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/aabb.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/ai_assert.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/camera.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/color4.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/color4.inl"
    "H:/WorkFolder/Projects/RobotEditor/out/build/debug/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/config.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/ColladaMetaData.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/commonMetaData.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/defs.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/cfileio.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/light.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/material.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/material.inl"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/matrix3x3.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/matrix3x3.inl"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/matrix4x4.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/matrix4x4.inl"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/mesh.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/ObjMaterial.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/pbrmaterial.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/GltfMaterial.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/postprocess.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/quaternion.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/quaternion.inl"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/scene.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/metadata.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/texture.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/types.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/vector2.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/vector2.inl"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/vector3.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/vector3.inl"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/version.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/cimport.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/AssertHandler.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/importerdesc.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/Importer.hpp"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/DefaultLogger.hpp"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/ProgressHandler.hpp"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/IOStream.hpp"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/IOSystem.hpp"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/Logger.hpp"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/LogStream.hpp"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/NullLogger.hpp"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/cexport.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/Exporter.hpp"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/DefaultIOStream.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/DefaultIOSystem.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/ZipArchiveIOSystem.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/SceneCombiner.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/fast_atof.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/qnan.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/BaseImporter.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/Hash.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/MemoryIOWrapper.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/ParsingUtils.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/StreamReader.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/StreamWriter.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/StringComparison.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/StringUtils.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/SGSpatialSort.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/GenericProperty.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/SpatialSort.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/SkeletonMeshBuilder.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/SmallVector.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/SmoothingGroups.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/SmoothingGroups.inl"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/StandardShapes.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/RemoveComments.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/Subdivision.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/Vertex.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/LineSplitter.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/TinyFormatter.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/Profiler.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/LogAux.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/Bitmap.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/XMLTools.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/IOStreamBuffer.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/CreateAnimMesh.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/XmlParser.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/BlobIOSystem.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/MathFunctions.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/Exceptional.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/ByteSwapper.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/Compiler/pushpack1.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/Compiler/poppack1.h"
    "H:/WorkFolder/Projects/RobotEditor/RobotLib/libraries/assimp-5.4.2/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "H:/WorkFolder/Projects/RobotEditor/out/build/debug/RobotLib/libraries/assimp-5.4.2/code/assimp-vc143-mtd.pdb")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "H:/WorkFolder/Projects/RobotEditor/out/build/debug/RobotLib/libraries/assimp-5.4.2/code/assimp-vc143-mt.pdb")
  endif()
endif()

