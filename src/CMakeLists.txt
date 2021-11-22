#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#add_definitions("-DQT_MESSAGELOGCONTEXT")

find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)
#pkg_search_module(GLIB REQUIRED glib-2.0 gio-2.0)
pkg_check_modules(GLIB REQUIRED glib-2.0 gio-2.0)

#查找当前目录下的所有源文件将名称保存到 BASE_SRC 变量
aux_source_directory(basecmd BASE_SRC)
aux_source_directory(speccmd SPEC_SRC)
aux_source_directory(utils UTILS_SRC)
include_directories(utils basecmd speccmd SRC_INCLUDE)
set(LLCMD_SOURCES
        ../llmain.cpp
        ${BASE_SRC}
        ${SPEC_SRC}
        ${UTILS_SRC}
        ${SRC_INCLUDE}
        )

set(LINK_LIBS
    CURL::libcurl
    stdc++
    ${GLIB_LINK_LIBRARIES}
    )

add_executable(llcmd
        ${LLCMD_SOURCES})

target_include_directories(llcmd PRIVATE
        ${GLIB_INCLUDE_DIRS}
        ${CURL_INCLUDE_DIRS}
)

MESSAGE(STATUS "curl dir " ${CURL_INCLUDE_DIRS})
MESSAGE(STATUS "glib dir " ${GLIB_INCLUDE_DIRS})
#设置生成目标二进制的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(llcmd PRIVATE ${LINK_LIBS})
install(TARGETS llcmd RUNTIME DESTINATION bin)