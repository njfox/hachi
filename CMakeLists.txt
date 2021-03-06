cmake_minimum_required(VERSION 3.5)

project(hachi LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Multimedia REQUIRED)
SET(QT_USE_QTMULTIMEDIA TRUE)


add_subdirectory(extern/fmt)
find_package(fmt)

add_subdirectory(extern/Catch2)
add_executable(tests tests/test.cpp)

add_subdirectory(extern/cmrc)
find_package(CMakeRC)
cmrc_add_resource_library(resources ALIAS hachi::rc  NAMESPACE hachi resources/beep.mp3)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(hachi
        ""
    )
else()
    add_executable(hachi "")
endif()

add_subdirectory(src)
add_subdirectory(include)
target_link_libraries(hachi PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
     fmt::fmt-header-only
     PRIVATE hachi::rc
     PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia
)
target_link_libraries(tests PRIVATE Catch2::Catch2)
