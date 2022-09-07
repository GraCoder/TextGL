cmake_minimum_required(VERSION 3.8.0)

set(prj_name text_gl)

project(${prj_name})

set(src
    main.cpp

	texture-atlas.h
	texture-atlas.c

	texture-font.h
	texture-font.c

	font-manager.h
	font-manager.c

	vertex-buffer.h
	#vertex-buffer.c

	vertex-attribute.h
	#vertex-attribute.c

	distance-field.h
	distance-field.c

	ftgl-utils.h
	ftgl-utils.c

	platform.h
	platform.c

	utf8-utils.h
	utf8-utils.c

	vector.h
	vector.c

	edtaa3func.h
	edtaa3func.c

	vec234.h
)

find_package(Freetype)
find_package(Fontconfig)
find_package(Boost COMPONENTS filesystem REQUIRED)

add_executable(${prj_name} ${src})

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
