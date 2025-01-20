cmake_minimum_required(VERSION 3.16)

project(RobotEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Core 3DCore 3DExtras 3DInput 3DRender Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Core 3DCore 3DExtras 3DInput 3DRender Gui)

# Add the RobotLib subdirectory
add_subdirectory(RobotLib)

set(TS_FILES RobotEditor_de_DE.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RobotEditor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET RobotEditor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(RobotEditor SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(RobotEditor
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# Link the RobotLib library and Qt libraries
target_link_libraries(RobotEditor PRIVATE RobotLib Qt${QT_VERSION_MAJOR}::Widgets
Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::3DCore Qt${QT_VERSION_MAJOR}::3DExtras
Qt${QT_VERSION_MAJOR}::3DInput Qt${QT_VERSION_MAJOR}::3DRender Qt${QT_VERSION_MAJOR}::Gui)


# Include directories
target_include_directories(RobotEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/RobotLib)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.RobotEditor)
endif()
set_target_properties(RobotEditor PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS RobotEditor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RobotEditor)
endif()

