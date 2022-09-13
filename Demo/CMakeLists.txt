cmake_minimum_required(VERSION 3.8.0)

set(prj_name TextGLDemo)

project(${prj_name})

set(prj_src
    main.cpp
)

#set(libs
#    OSG_LIBRARY
#    OSGUTIL_LIBRARY
#    OSGVIEWER_LIBRARY
#)

add_executable(${prj_name} ${prj_src})

set_target_properties(${prj_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY   "$<TARGET_FILE_DIR:${prj_name}>"
                                               VS_DEBUGGER_COMMAND           "$<TARGET_FILE:${prj_name}>"
                                               VS_DEBUGGER_ENVIRONMENT       "PATH=%PATH%;${CMAKE_PREFIX_PATH}/bin")

target_include_directories(${prj_name} PRIVATE ${OSG_INCLUDE_DIR})

target_include_directories(${prj_name} PRIVATE ../TextGL)

foreach(lib ${libs})
    target_link_libraries(${prj_name} debug ${${lib}_DEBUG} optimized ${${lib}_RELEASE})
endforeach()

target_link_libraries(${prj_name} PRIVATE ${TextGL})
