# library
cmake_minimum_required(VERSION 3.10)

# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake -DCMAKE_BUILD_TYPE=Release ..

# 选择静态或者动态编译,Android一般默认动态就行
option(BUILD_SHARED "Build shared library instead of static library" ON)

function(get_bento4_version)
    file(STRINGS "bentof/Core/Ap4Version.h" _temp REGEX "AP4_VERSION_STRING")
    string(REGEX MATCH "\"([^\"]+)\"" _temp "${_temp}")
    set(BENTO4_VERSION "${CMAKE_MATCH_1}" PARENT_SCOPE)
endfunction()

get_bento4_version()
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
project(bento4 VERSION "${BENTO4_VERSION}")

# Variables
set(SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_BENTOF ${CMAKE_CURRENT_SOURCE_DIR}/bentof)
set(SOURCE_CODECS ${SOURCE_BENTOF}/Codecs)
set(SOURCE_CORE ${SOURCE_BENTOF}/Core)
set(SOURCE_CRYPTO ${SOURCE_BENTOF}/Crypto)
set(SOURCE_METADATA ${SOURCE_BENTOF}/MetaData)
set(SOURCE_SYSTEM ${SOURCE_BENTOF}/System)
set(SOURCE_APPS ${SOURCE_BENTOF}/Apps)
set(SOURCE_BENTO4 ${SOURCE_ROOT}/jni)

# Compiler warning and optimization flags
if (CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-class-memaccess")
endif ()

if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-warn-absolute-paths")
endif ()

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12)

# AP4 Library
file(GLOB AP4_SOURCES
        ${SOURCE_CODECS}/*.cpp
        ${SOURCE_CORE}/*.cpp
        ${SOURCE_CRYPTO}/*.cpp
        ${SOURCE_METADATA}/*.cpp
        ${SOURCE_SYSTEM}/StdC/*.cpp
        ${SOURCE_APPS}/*.cpp
        )

# AP4 Library
file(GLOB BENTO4_SOURCES ${SOURCE_BENTO4}/*.cpp)

set(AP4_SOURCES ${AP4_SOURCES} ${SOURCE_SYSTEM}/Posix/Ap4PosixRandom.cpp)

# Includes
set(AP4_INCLUDE_DIRS
        ${SOURCE_CORE}
        ${SOURCE_CODECS}
        ${SOURCE_CRYPTO}
        ${SOURCE_METADATA}
        ${SOURCE_BENTO4}
        ${SOURCE_APPS}
        )

file(GLOB AP4_HEADERS
        ${SOURCE_CORE}/*.h
        ${SOURCE_CODECS}/*.h
        ${SOURCE_CRYPTO}/*.h
        ${SOURCE_METADATA}/*.h
        ${SOURCE_BENTO4}/*.h
        ${SOURCE_APPS}/*.h)

find_library(log-lib log)
add_library(ap4 SHARED ${AP4_SOURCES})
add_library(bentof SHARED ${BENTO4_SOURCES})

target_include_directories(ap4 PUBLIC ${AP4_INCLUDE_DIRS})
target_include_directories(bentof PUBLIC ${AP4_INCLUDE_DIRS})


target_link_libraries(
        bentof
        PUBLIC ap4
        ${log-lib})

target_link_libraries(
        ap4
        PRIVATE ${log-lib})