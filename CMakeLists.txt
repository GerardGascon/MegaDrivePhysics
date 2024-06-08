cmake_minimum_required(VERSION 3.25)

project(MegaDrivePhysics)

set(GDK C:/Frameworks/SGDK/170)
set(GDK_WIN C:\\Frameworks\\SGDK\\170)

set(HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/res
        ${GDK}/inc)

set(SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
        inc/balls.h
        src/balls.c)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    set(type debug)
ELSE()
    set(type clean release)
ENDIF()

set(SGDK_COMPILE_COMMAND
        ${GDK}/bin/make.exe GDK=${GDK} GDK_WIN=${GDK_WIN}
        -f ${GDK}/makelib.gen ${type})

set(PROJECT_COMPILE_COMMAND
        ${GDK}/bin/make.exe GDK=${GDK} GDK_WIN=${GDK_WIN}
        -f ${GDK}/makefile.gen ${type})

add_executable(megadrive ${SRC})
target_include_directories(megadrive PRIVATE ${HEADER_FILES})
add_custom_target(megadrive_make ALL ${SGDK_COMPILE_COMMAND}
        COMMAND ${PROJECT_COMPILE_COMMAND}
        DEPENDS ${HEADER_FILES}
        SOURCES ${SRC}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})