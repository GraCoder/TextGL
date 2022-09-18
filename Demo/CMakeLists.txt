cmake_minimum_required(VERSION 3.8.0)

set(prj_name TextGLDemo)

project(${prj_name})

set(prj_src
    FontWidget.cpp 
    FontWidget.h
    GLWidget.h
    GLWidget.cpp
    main.cpp
)

#set(libs
#    OSG_LIBRARY
#    OSGUTIL_LIBRARY
#    OSGVIEWER_LIBRARY
#)

find_package(OpenSceneGraph COMPONENTS osg osgViewer osgDB osgText osgGA)

set(CMAKE_AUTOMOC true)

find_package(Qt5 COMPONENTS Widgets Gui)

add_executable(${prj_name} ${prj_src})

target_include_directories(${prj_name} PRIVATE ${OSG_INCLUDE_DIR})

target_include_directories(${prj_name} PRIVATE ../TextGL)

foreach(lib ${libs})
    target_link_libraries(${prj_name} debug ${${lib}_DEBUG} optimized ${${lib}_RELEASE})
endforeach()

target_link_libraries(${prj_name} PRIVATE ${OSGVIEWER_LIBRARY} ${OSGGA_LIBRARY} ${OSGDB_LIBRARY})

target_link_libraries(${prj_name} PRIVATE TextGL Opengl32)

target_link_libraries(${prj_name} PRIVATE Qt5::Widgets)

set_target_properties(${prj_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


set_target_properties(${prj_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY   "$<TARGET_FILE_DIR:${prj_name}>"
                                               VS_DEBUGGER_COMMAND           "$<TARGET_FILE:${prj_name}>"
                                               VS_DEBUGGER_ENVIRONMENT       "PATH=%PATH%;${OSG_DIR}/bin")
