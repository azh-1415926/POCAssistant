cmake_minimum_required(VERSION 3.5)
project(POCAssistant VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 请自行填写 qt 工具链所在路径
# set(CMAKE_PREFIX_PATH "D:/Qt/6.5.3/msvc2019_64") # Qt Kit Dir
# set(CMAKE_PREFIX_PATH "/Users/azh/Qt/6.7.3/macos") # Qt Kit Dir

# 默认使用 Qt6 版本，其次 Qt5(用Qt5的时候不要打开中文路径的题库)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network )
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core5Compat) 
endif()

set(QMARKDOWNTEXTEDIT_EXE OFF)
add_subdirectory(3rdparty/qmarkdowntextedit)

set(ROOT_DIR .)

# 搜寻源文件

file(GLOB_RECURSE srcs ${ROOT_DIR}/src/*.c* ${ROOT_DIR}/src/*.h*)
file(GLOB_RECURSE qrcs ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)

# ui 文件路径
list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "ui")

# Specify MSVC UTF-8 encoding
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

if(EMSCRIPTEN)
    message("编译网页端，使用 WebAssembly")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file cfg/config.json")
else()
    message("编译客户端")
endif()
# 

set(PROJECT_SOURCES
    ${srcs}
    ${qrcs}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core5Compat)
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 链接的头文件、库
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network qmarkdowntextedit)
target_include_directories(${PROJECT_NAME} PRIVATE ${ROOT_DIR}/src ${ROOT_DIR}/src/base ${ROOT_DIR}/src/pages ${ROOT_DIR}/src/tools ${ROOT_DIR}/src/modules 3rdparty/qmarkdowntextedit)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    # WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()