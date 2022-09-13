cmake_minimum_required(VERSION 3.8.0)

include(intern/intern.cmake)

set(prj_name TextGL)

project(${prj_name})


#tadd_dir()

set(src
	TCFont.h
	TCFontTexture.h
	)

find_package(Freetype)
find_package(Fontconfig)
find_package(Boost COMPONENTS filesystem REQUIRED)

add_executable(${prj_name} ${src} ${src_intern})

set_target_properties(${prj_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY   "$<TARGET_FILE_DIR:${prj_name}>"
                                               VS_DEBUGGER_COMMAND           "$<TARGET_FILE:${prj_name}>"
                                               VS_DEBUGGER_ENVIRONMENT       "PATH=%PATH%;${CMAKE_PREFIX_PATH}/bin")

target_include_directories(${prj_name} PRIVATE ${OSG_INCLUDE_DIR})

foreach(lib ${libs})
    target_link_libraries(${prj_name} debug ${${lib}_DEBUG} optimized ${${lib}_RELEASE})
endforeach()

target_include_directories(${prj_name} PRIVATE ${FREETYPE_INCLUDE_DIR_freetype2})
target_link_libraries(${prj_name} ${FREETYPE_LIBRARY})

target_include_directories(${prj_name} PRIVATE ${Fontconfig_INCLUDE_DIR})
target_link_libraries(${prj_name} ${Fontconfig_LIBRARY})

target_link_libraries(${prj_name} ${Boost_FILESYSTEM_LIBRARY})


target_include_directories(${prj_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
