cmake_minimum_required(VERSION 2.6)

find_package(FBXSDK REQUIRED)


set(UTIL Hades-Workshop)

if (MSVC)
     add_compile_options(/bigobj)
	 add_definitions(-DWXUSINGDLL -DWXUSINGDLL -DwxMSVC_VERSION_AUTO)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_BINARY_DIR ON)

include_directories(${CMAKE_SOURCE_DIR}/thirdParty/wxWidgets/include)
include_directories(${CMAKE_BINARY_DIR}/lib/vc_x64_dll/mswu)
include_directories(${FBXSDK_INCLUDE_DIR})

file(GLOB C_FILES *.cpp)
file(GLOB H_FILES *.h)

file(GLOB C_SQUISH_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Squish/*.cpp)
file(GLOB H_SQUISH_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Squish/*.h)

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${C_FILES} ${H_FILES})

source_group(FILES resource.rc  PREFIX resources TREE ${CMAKE_CURRENT_SOURCE_DIR})

source_group(FILES makeppf3.cpp makeppf3.h ${C_SQUISH_FILES} ${H_SQUISH_FILES} PREFIX extsrc TREE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${UTIL} WIN32 ${C_FILES} ${H_FILES} ${C_SQUISH_FILES} ${H_SQUISH_FILES} resource.rc )

target_link_libraries(${UTIL} ${FBXSDK_LIBRARY} base net xml adv aui core gl html net media propgrid qa ribbon richtext stc webview xrc wxexpat wxjpeg wxpng wxregex wxscintilla wxtiff wxzlib)

set_target_properties(${UTIL} PROPERTIES PREFIX "")






