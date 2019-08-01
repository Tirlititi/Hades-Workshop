cmake_minimum_required(VERSION 2.6)

find_package(FBXSDK REQUIRED)


set(UTIL Hades-Workshop)

if (MSVC)
     add_compile_options(/bigobj)
	 add_definitions(-DWXUSINGDLL -DwxMSVC_VERSION_AUTO)
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

target_link_libraries(${UTIL} base net xml adv aui core gl html net media propgrid qa ribbon richtext stc webview xrc wxexpat wxjpeg wxpng wxregex wxscintilla wxtiff wxzlib)
target_link_libraries(${UTIL} optimized ${FBXSDK_LIBRARY} )
target_link_libraries(${UTIL} debug ${FBXSDK_LIBRARY_DEBUG} )

set_target_properties(${UTIL} PROPERTIES PREFIX "")

if(MSVC)
	# Include generator expression to suppress default Debug/Release pair
	set(wxPLATFORM_LIB_DIR "$<1:/>${wxCOMPILER_PREFIX}${wxARCH_SUFFIX}_${lib_suffix}")
else()
	set(wxPLATFORM_LIB_DIR "/${wxCOMPILER_PREFIX}${wxARCH_SUFFIX}_${lib_suffix}")
endif()

message(STATUS )

add_custom_target(copyWxBinaries
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:base>         	${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:net>         	${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:xml>         	${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:adv>         	${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:aui>         	${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:core>         	${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gl>         		${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:html>         	${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:net>         	${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:media>       	${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:propgrid>    ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:qa>       		${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ribbon>       ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:richtext>     ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:webview>    ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:xrc>            ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
 
)