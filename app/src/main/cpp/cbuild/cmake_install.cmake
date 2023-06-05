# Install script for directory: /home/easytouch/hls/Bento4-master

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libap4.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libap4.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libap4.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/easytouch/hls/Bento4-master/cbuild/libap4.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libap4.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libap4.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libap4.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/aac2mp4" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/aac2mp4")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/aac2mp4"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/aac2mp4")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/aac2mp4" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/aac2mp4")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/aac2mp4"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/aac2mp4")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/avcinfo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/avcinfo")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/avcinfo"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/avcinfo")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/avcinfo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/avcinfo")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/avcinfo"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/avcinfo")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fixaacsampledescription" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fixaacsampledescription")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fixaacsampledescription"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/fixaacsampledescription")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fixaacsampledescription" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fixaacsampledescription")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fixaacsampledescription"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/fixaacsampledescription")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/hevcinfo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/hevcinfo")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/hevcinfo"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/hevcinfo")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/hevcinfo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/hevcinfo")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/hevcinfo"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/hevcinfo")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42aac" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42aac")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42aac"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp42aac")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42aac" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42aac")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42aac"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42aac")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42avc" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42avc")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42avc"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp42avc")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42avc" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42avc")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42avc"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42avc")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42hevc" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42hevc")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42hevc"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp42hevc")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42hevc" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42hevc")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42hevc"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42hevc")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42hls" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42hls")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42hls"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp42hls")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42hls" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42hls")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42hls"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42hls")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42ts" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42ts")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42ts"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp42ts")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42ts" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42ts")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42ts"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp42ts")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4audioclip" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4audioclip")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4audioclip"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4audioclip")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4audioclip" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4audioclip")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4audioclip"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4audioclip")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4compact" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4compact")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4compact"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4compact")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4compact" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4compact")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4compact"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4compact")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4dcfpackager" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4dcfpackager")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4dcfpackager"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4dcfpackager")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4dcfpackager" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4dcfpackager")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4dcfpackager"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4dcfpackager")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4decrypt" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4decrypt")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4decrypt"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4decrypt")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4decrypt" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4decrypt")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4decrypt"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4decrypt")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4diff" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4diff")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4diff"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4diff")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4diff" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4diff")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4diff"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4diff")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4dump" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4dump")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4dump"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4dump")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4dump" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4dump")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4dump"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4dump")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4edit" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4edit")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4edit"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4edit")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4edit" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4edit")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4edit"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4edit")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4encrypt" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4encrypt")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4encrypt"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4encrypt")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4encrypt" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4encrypt")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4encrypt"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4encrypt")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4extract" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4extract")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4extract"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4extract")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4extract" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4extract")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4extract"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4extract")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4fragment" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4fragment")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4fragment"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4fragment")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4fragment" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4fragment")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4fragment"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4fragment")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4iframeindex" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4iframeindex")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4iframeindex"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4iframeindex")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4iframeindex" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4iframeindex")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4iframeindex"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4iframeindex")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4info" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4info")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4info"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4info")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4info" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4info")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4info"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4info")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4mux" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4mux")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4mux"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4mux")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4mux" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4mux")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4mux"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4mux")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4pssh" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4pssh")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4pssh"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4pssh")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4pssh" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4pssh")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4pssh"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4pssh")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4rtphintinfo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4rtphintinfo")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4rtphintinfo"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4rtphintinfo")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4rtphintinfo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4rtphintinfo")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4rtphintinfo"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4rtphintinfo")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4split" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4split")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4split"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4split")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4split" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4split")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4split"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4split")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4tag" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4tag")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4tag"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/easytouch/hls/Bento4-master/cbuild/mp4tag")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4tag" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4tag")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4tag"
         OLD_RPATH "/home/easytouch/hls/Bento4-master/cbuild:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mp4tag")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/bento4" TYPE FILE FILES
    "/home/easytouch/hls/Bento4-master/Source/C++/Codecs/Ap4Ac3Parser.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Codecs/Ap4Ac4Parser.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Codecs/Ap4AdtsParser.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Codecs/Ap4AvcParser.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Codecs/Ap4BitStream.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Codecs/Ap4Eac3Parser.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Codecs/Ap4HevcParser.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Codecs/Ap4Mp4AudioInfo.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Codecs/Ap4NalParser.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap48bdlAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Ac4Utils.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4AinfAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Array.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Atom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4AtomFactory.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4AtomSampleTable.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Av1cAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4AvccAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4BlocAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4ByteStream.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Co64Atom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Command.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4CommandFactory.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4CommonEncryption.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Config.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Constants.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4ContainerAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4CttsAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Dac3Atom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Dac4Atom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4DataBuffer.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Debug.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Dec3Atom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4DecoderConfigDescriptor.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4DecoderSpecificInfoDescriptor.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Descriptor.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4DescriptorFactory.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4DrefAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4DvccAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4DynamicCast.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4ElstAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4EsDescriptor.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4EsdsAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Expandable.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4File.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4FileByteStream.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4FileCopier.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4FileWriter.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4FragmentSampleTable.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4FrmaAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4FtypAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4GrpiAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4HdlrAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4HintTrackReader.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4HmhdAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4HvccAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4IkmsAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Interfaces.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4IodsAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Ipmp.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4IproAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4IsfmAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4IsltAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4IsmaCryp.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4LinearReader.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4List.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Marlin.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4MdhdAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4MehdAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4MfhdAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4MfroAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4MoovAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Movie.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4MovieFragment.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Mpeg2Ts.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4MvhdAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4NmhdAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4ObjectDescriptor.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4OdafAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4OddaAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4OdheAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4OhdrAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4OmaDcf.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4PdinAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Piff.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Processor.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Protection.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4PsshAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Results.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4RtpAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4RtpHint.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SLConfigDescriptor.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SaioAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SaizAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Sample.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SampleDescription.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SampleEntry.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SampleSource.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SampleTable.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SbgpAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SchmAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SdpAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SegmentBuilder.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SencAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SgpdAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SidxAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SmhdAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4StcoAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SthdAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4String.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4StscAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4StsdAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4StssAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4StszAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SttsAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Stz2Atom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4SyntheticSampleTable.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4TencAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4TfdtAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4TfhdAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4TfraAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4TimsAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4TkhdAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Track.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4TrakAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4TrefTypeAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4TrexAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4TrunAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Types.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4UrlAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Utils.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4UuidAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4Version.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4VmhdAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Core/Ap4VpccAtom.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Crypto/Ap4AesBlockCipher.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Crypto/Ap4Hmac.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Crypto/Ap4KeyWrap.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/Crypto/Ap4StreamCipher.h"
    "/home/easytouch/hls/Bento4-master/Source/C++/MetaData/Ap4MetaData.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/bento4" TYPE FILE FILES
    "/home/easytouch/hls/Bento4-master/cbuild/generated/bento4Config.cmake"
    "/home/easytouch/hls/Bento4-master/cbuild/generated/bento4ConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/bento4/bento4Targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/bento4/bento4Targets.cmake"
         "/home/easytouch/hls/Bento4-master/cbuild/CMakeFiles/Export/e0dd49b2eb3ca1303803848168356d30/bento4Targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/bento4/bento4Targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/bento4/bento4Targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/bento4" TYPE FILE FILES "/home/easytouch/hls/Bento4-master/cbuild/CMakeFiles/Export/e0dd49b2eb3ca1303803848168356d30/bento4Targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/bento4" TYPE FILE FILES "/home/easytouch/hls/Bento4-master/cbuild/CMakeFiles/Export/e0dd49b2eb3ca1303803848168356d30/bento4Targets-noconfig.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/easytouch/hls/Bento4-master/cbuild/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
