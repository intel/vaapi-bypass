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

option (DEBUG "Turn on debug build." OFF)

find_library (SHIM_ENTRY_LIB hddl_bypass_shim_entry)
if (SHIM_ENTRY_LIB)
    message ("ShimEntryLib found: ${SHIM_ENTRY_LIB}")
else ()
    message ("ShimEntryLib not found")
endif( )

# Set debug option
if (DEBUG)
    set (CMAKE_BUILD_TYPE debug)
else ()
    set (CMAKE_BUILD_TYPE release)
endif()

# Link necessary libraries
set (LINK_LIBS ${SHIM_ENTRY_LIB})

set (INVOKE_SHIM_APP "invokeShim")
add_executable (${INVOKE_SHIM_APP} ${INVOKE_SHIM_APP}.c)
target_link_libraries (${INVOKE_SHIM_APP} ${LINK_LIBS})
