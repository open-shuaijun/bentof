# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake -DCMAKE_BUILD_TYPE=Release ..
# 选择静态或者动态编译
option(BUILD_SHARED "Build shared library instead of static library" ON)

cmake_minimum_required(VERSION 3.10)

function(get_bento4_version)
    file(STRINGS "Source/C++/Core/Ap4Version.h" _temp REGEX "AP4_VERSION_STRING")
    string(REGEX MATCH "\"([^\"]+)\"" _temp "${_temp}")
    set(BENTO4_VERSION "${CMAKE_MATCH_1}" PARENT_SCOPE)
endfunction()

get_bento4_version()
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
project(bento4 VERSION "${BENTO4_VERSION}")

# Variables
set(SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Source/C++)

set(SOURCE_CODECS ${SOURCE_ROOT}/Codecs)
set(SOURCE_CORE ${SOURCE_ROOT}/Core)
set(SOURCE_CRYPTO ${SOURCE_ROOT}/Crypto)
set(SOURCE_METADATA ${SOURCE_ROOT}/MetaData)
set(SOURCE_SYSTEM ${SOURCE_ROOT}/System)
set(SOURCE_APPS ${SOURCE_ROOT}/Apps)
set(SOURCE_BENTO4 ${SOURCE_ROOT}/bentof)
message("-------------------------------------")
message(${SOURCE_BENTO4})

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
        ${SOURCE_APPS}/*.h
        )
find_library(log-lib log)
add_library(ap4 SHARED ${AP4_SOURCES})
add_library(bentof SHARED ${BENTO4_SOURCES})

target_include_directories(ap4 PUBLIC ${AP4_INCLUDE_DIRS})
target_include_directories(bentof PUBLIC ${AP4_INCLUDE_DIRS})


# Apps
#option(BUILD_APPS "Build example applications" ON)
#if (BUILD_APPS)
#    file(GLOB BENTO4_APPS RELATIVE ${SOURCE_ROOT}/Apps ${SOURCE_ROOT}/Apps/*)
#    foreach (app ${BENTO4_APPS})
#        #   set(app Mp4Mux)
#        string(TOLOWER ${app} binary_name)
#        list(APPEND BENTO4_APPS_LOWERCASE ${binary_name})
#        #        add_executable(${binary_name} ${SOURCE_ROOT}/Apps/${app}/${app}.cpp)
##        message("App path: ${SOURCE_APPS}/${app}")
#        add_library(${binary_name} SHARED ${SOURCE_APPS}/${app}/${app}.cpp)
##        set(${app}_include ${SOURCE_APPS}/${app}/${app}.h)
#
##        target_include_directories(${binary_name} PRIVATE ${app}_include)
#        target_link_libraries(${binary_name} ap4)
##        include_directories(${app}_include)
#        target_link_libraries(bentof ${binary_name})
#
##        message(${app}_include)
#
#
#
#        if (MSVC)
#            set_property(TARGET ${binary_name} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#            target_compile_definitions(${binary_name} PRIVATE -D_CONSOLE)
#        endif ()
#    endforeach ()
#endif (BUILD_APPS)


target_link_libraries(
        bentof
        PUBLIC ap4
        ${log-lib})

target_link_libraries(
        ap4
        PRIVATE ${log-lib})

# Install
#include(GNUInstallDirs)
#set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
#set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
#set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
#set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
#set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
#set(namespace "${PROJECT_NAME}::")
#
#include(CMakePackageConfigHelpers)
#write_basic_package_version_file(
#        "${version_config}" COMPATIBILITY SameMajorVersion
#)
#
#configure_package_config_file(
#        "Build/cmake/Config.cmake.in"
#        "${project_config}"
#        INSTALL_DESTINATION "${config_install_dir}"
#)
#
#install(
#        TARGETS ap4 ${BENTO4_APPS_LOWERCASE}
#        EXPORT "${TARGETS_EXPORT_NAME}"
#        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
#        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
#        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
#        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
#)
#
#install(
#        FILES ${AP4_HEADERS}
#        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/bento4"
#)
#
#install(
#        FILES "${project_config}" "${version_config}"
#        DESTINATION "${config_install_dir}"
#)
#
#install(
#        EXPORT "${TARGETS_EXPORT_NAME}"
#        NAMESPACE "${namespace}"
#        DESTINATION "${config_install_dir}"
#)
