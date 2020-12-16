# Copyright (c) 2019 Intel Corporation. All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required (VERSION 3.5)
project (sample)
include (FindPkgConfig)
include (${CMAKE_SOURCE_DIR}/../../cmake/safestringlib_include.cmake)

option (DEBUG "Turn on debug build." OFF)

# Find GLib
pkg_check_modules (GLIB REQUIRED glib-2.0)
if (GLIB_FOUND)
    include_directories (${GLIB_INCLUDE_DIRS})
    link_directories (${GLIB_LIBRARY_DIRS})
else ()
    message ("Failed to find GLib")
endif ()

# Find GStreamer
pkg_check_modules (GSTREAMER REQUIRED gstreamer-1.0>=1.16)
if (GSTREAMER_FOUND)
    include_directories (${GSTREAMER_INCLUDE_DIRS})
    link_directories (${GSTREAMER_LIBRARY_DIRS})
else ()
    message ("Failed to find GStreamer")
endif ()

# Find HDDLUnite
find_library (HDDLUNITE_LIB NAMES HddlUnite HINTS "/opt/intel/hddlunite/lib")
if (HDDLUNITE_LIB)
    set (HDDLUNITE_INCLUDE_DIR "/opt/intel/hddlunite/include")
    include_directories (${HDDLUNITE_INCLUDE_DIR})
    message ("HDDLUnite Library located at ${HDDLUNITE_LIB}")
else ()
    message ("Failed to find HDDLUnite")
endif ()

FindSafeStringLib ()

# Set debug option
if (DEBUG)
    set (CMAKE_BUILD_TYPE debug)
else ()
    set (CMAKE_BUILD_TYPE release)
endif()

add_subdirectory (plugins)

# Link necessary libraries
set (LINK_LIBS ${GSTREAMER_LIBRARIES} ${GLIB_LIBRARIES} ${HDDLUNITE_LIB} pthread)

set (PLUGIN "gstbypass")
add_library (${PLUGIN} SHARED ${PLUGIN_SOURCE})
target_link_libraries (gstbypass ${LINK_LIBS})

set (DEC_APP "gstDecodeApp")
add_executable (${DEC_APP} ${DEC_APP}.c)
target_link_libraries (${DEC_APP} ${LINK_LIBS})

set (ENC_APP "gstEncodeApp")
add_executable (${ENC_APP} ${ENC_APP}.c)
target_link_libraries (${ENC_APP} ${LINK_LIBS})

if (DEFINED ENV{USE_SYSTEM_SAFESTR})
    target_link_libraries (${ENC_APP} ${SAFESTRINGLIB})
    target_link_libraries (${DEC_APP} ${SAFESTRINGLIB})
    target_link_libraries (gstbypass ${SAFESTRINGLIB})
else ()
    target_link_libraries (${ENC_APP} libsafestring)
    target_link_libraries (${DEC_APP} libsafestring)
    target_link_libraries (gstbypass libsafestring)
endif ()
