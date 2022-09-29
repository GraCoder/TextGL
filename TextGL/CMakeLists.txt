cmake_minimum_required(VERSION 3.8.0)

set(prj_name TextGL)

project(${prj_name})

find_package(OpenSceneGraph COMPONENTS osg osgDB osgText)

set(src
	TC_Font.h
	TC_FontTexture.h
	TC_GlyText.h
	TC_GlyChar.h
	)

include(intern/intern.cmake)

find_package(Freetype)
find_package(Fontconfig)
find_package(Boost COMPONENTS filesystem REQUIRED)

if(OpenSceneGraph_FOUND)
	list(APPEND src TC_TextNode.h)
endif()

add_library(${prj_name} SHARED ${src} ${src_intern})

set_target_properties(${prj_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY   "$<TARGET_FILE_DIR:${prj_name}>"
                                               VS_DEBUGGER_COMMAND           "$<TARGET_FILE:${prj_name}>"
                                               VS_DEBUGGER_ENVIRONMENT       "PATH=%PATH%;${CMAKE_PREFIX_PATH}/bin")


target_include_directories(${prj_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(OpenSceneGraph_FOUND)
	target_include_directories(${prj_name} PUBLIC ${OSG_INCLUDE_DIR})
	target_link_libraries(${prj_name} PUBLIC ${OSG_LIBRARY} ${OSGTEXT_LIBRARY} ${OSGTEXT_LIBRARY} ${OSGDB_LIBRARY})
endif()


target_include_directories(${prj_name} PRIVATE ${FREETYPE_INCLUDE_DIR_freetype2})
target_link_libraries(${prj_name} PRIVATE ${FREETYPE_LIBRARY})


target_include_directories(${prj_name} PRIVATE ${Fontconfig_INCLUDE_DIR})
target_link_libraries(${prj_name} PRIVATE ${Fontconfig_LIBRARY})


target_link_libraries(${prj_name} PRIVATE ${Boost_FILESYSTEM_LIBRARY})


set_target_properties(${prj_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
