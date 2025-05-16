cmake_minimum_required(VERSION 3.16)

project(jetson_control VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

include_directories("G:/opencv-mingw/OpenCV-MinGW-Build-OpenCV-3.4.8-x64/include")
link_directories("G:/opencv-mingw/OpenCV-MinGW-Build-OpenCV-3.4.8-x64/x64/mingw/lib")
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(jetson_control
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        mjpegviewer.h
        mjpegviewer.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET jetson_control APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(jetson_control SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(jetson_control
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# target_link_libraries(jetson_control PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(jetson_control
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    ${OpenCV_LIBS}
)

target_link_libraries(jetson_control
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    G:/opencv-mingw/OpenCV-MinGW-Build-OpenCV-3.4.8-x64/x64/mingw/lib/libopencv_core348.dll.a
    G:/opencv-mingw/OpenCV-MinGW-Build-OpenCV-3.4.8-x64/x64/mingw/lib/libopencv_imgproc348.dll.a
    G:/opencv-mingw/OpenCV-MinGW-Build-OpenCV-3.4.8-x64/x64/mingw/lib/libopencv_highgui348.dll.a
    G:/opencv-mingw/OpenCV-MinGW-Build-OpenCV-3.4.8-x64/x64/mingw/lib/libopencv_imgcodecs348.dll.a
    G:/opencv-mingw/OpenCV-MinGW-Build-OpenCV-3.4.8-x64/x64/mingw/lib/libopencv_videoio348.dll.a
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.jetson_control)
endif()
set_target_properties(jetson_control PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS jetson_control
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(jetson_control)
endif()
