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

FindSafeStringLib()

# Find GStreamer
pkg_check_modules (GSTREAMER REQUIRED gstreamer-1.0>=1.16)
if (GSTREAMER_FOUND)
    include_directories (${GSTREAMER_INCLUDE_DIRS})
    link_directories (${GSTREAMER_LIBRARY_DIRS})
else ()
    message ("Failed to find GStreamer")
endif ()

# Set debug option
if (DEBUG)
    set (CMAKE_BUILD_TYPE debug)
else ()
    set (CMAKE_BUILD_TYPE release)
endif()

# Link necessary libraries
set (LINK_LIBS ${GSTREAMER_LIBRARIES} ${GLIB_LIBRARIES} pthread)

set (PIPELINE_APP "gstPipelinePerf")
add_executable (${PIPELINE_APP} ${PIPELINE_APP}.c)
target_link_libraries (${PIPELINE_APP} ${LINK_LIBS})

if (DEFINED ENV{USE_SYSTEM_SAFESTR})
    target_link_libraries(${PIPELINE_APP} ${SAFESTRINGLIB})
else ()
    target_link_libraries(${PIPELINE_APP} libsafestring)
endif ()

set (CROPROI_APP "cropRoiPipelinePerf")
add_executable (${CROPROI_APP} ${CROPROI_APP}.c)
target_link_libraries (${CROPROI_APP} ${LINK_LIBS})

if (DEFINED ENV{USE_SYSTEM_SAFESTR})
    target_link_libraries(${CROPROI_APP} ${SAFESTRINGLIB})
else ()
    target_link_libraries(${CROPROI_APP} libsafestring)
endif ()
