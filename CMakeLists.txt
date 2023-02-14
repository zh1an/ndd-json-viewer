cmake_minimum_required(VERSION 3.16)

project(ndd-json-viewer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets PrintSupport Network Concurrent XmlPatterns)
find_library(QSCINT_LIB qmyedit_qt5 PATH ${QSCINT_SDK_PATH}/lib)

aux_source_directory(src SRC_FILES)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES} src/jsonviewsettings.ui src/image.qrc src/docktitlewidget.ui)

target_include_directories(${PROJECT_NAME} PRIVATE
        src
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${QSCINT_SDK_PATH}/include
        ${QSCINT_SDK_PATH}/include/src)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Concurrent Qt5::Network Qt5::PrintSupport Qt5::XmlPatterns
        ${QSCINT_LIB})