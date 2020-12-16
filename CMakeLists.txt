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

project(HDDL_bypass_drv)
message(hddl_vashim_project_build_path = ${CMAKE_SOURCE_DIR})

include(FindPkgConfig)

include (${CMAKE_SOURCE_DIR}/cmake/libva_include.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/compile_targets.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/xlink_include.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/safestringlib_include.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/hddlunite_include.cmake)

option (DEBUG "Turn on debug build." OFF)
option (USE_HANTRO_DRIVER "Build with Hantro driver" ${})
option (FUNCTION_TIME_PROFILE "Turn on function time profiling" OFF)
option (COMM_TIME_PROFILE "Turn on communication time profiling" OFF)
option (TARGETS "Select target" ${})

if (DEBUG)
    set (CMAKE_BUILD_TYPE debug)
    add_definitions (-DDEBUG)
else ()
    set (CMAKE_BUILD_TYPE release)
endif ()

if (SVE_HOOK)
    add_definitions (-DSVE_HOOK=1)
endif ()

if (FUNCTION_TIME_PROFILE)
    if (DEBUG)
        add_definitions (-DFUNCTION_TIME_PROFILE)
        message ("Build type: " ${CMAKE_BUILD_TYPE} " - FUNCTION_TIME_PROFILE enabled")
    else ()
        message ("Build type: " ${CMAKE_BUILD_TYPE} " - FUNCTION_TIME_PROFILE disabled")
    endif ()
endif ()

if (COMM_TIME_PROFILE)
    if (DEBUG)
        add_definitions (-DCOMM_TIME_PROFILE)
        message ("Build type: " ${CMAKE_BUILD_TYPE} " - COMM_TIME_PROFILE enabled")
    else ()
        message ("Build type: " ${CMAKE_BUILD_TYPE} " - COMM_TIME_PROFILE disabled")
    endif ()
endif ()

FindLibva (LIBVA_LIBRARIES)
FindSafeStringLib ()
FindHDDLUnite ()
FindXLink ()

if (CMAKE_C_COMPILER_VERSION VERSION_GREATER 4.9)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -std=gnu99 -Wall -fPIE -fstack-protector-strong")
else ()
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -std=gnu99 -Wall -fPIE -fstack-protector")
endif ()

set (COMM_SOURCES
        ./src/common/xlink/xlink_pcie.c
        ./src/common/xlink/xlink_placeholders.c
        ./src/common/tcp/tcp_placeholders.c
        ./src/common/unite/unite.c)

if (USE_HANTRO_DRIVER STREQUAL "KMB")
        add_definitions (-DUSE_HANTRO)
	add_definitions (-DKMB)
        include_directories (src/ext/va_hantro_kmb)
        message ("Adding KMB hantro driver header")
else ()
        message ("-- Not using hantro driver header! --")
endif ()

include_directories (
    src/host
    src/util
    src/common
    src/common/xlink
    src/common/tcp
    src/common/unite
    src/common/va
    src/target
)

set (COMMON_SOURCES
    ./src/common/memory_manager.c
    ./src/common/thread_manager.c
    ./src/common/gen_comm.c
)

set (HOST_SOURCES
    ${COMMON_SOURCES}
    ${COMM_SOURCES}
    ./src/host/host_va_shim.c
)

set (TARGET_BIN_SOURCES
    ${COMMON_SOURCES}
    ${COMM_SOURCES}
    ./src/target/target_va_shim.c
    ./src/target/payload.c
    ./src/common/va/va_display_drm.c
    ./src/common/va/va_display.c
)

set (TARGET_LIB_SOURCES
    ${TARGET_BIN_SOURCES}
    ./src/target/target_shim_entry.c
)

SET (CMAKE_CXX_FLAGS "-pthread -lva-drm")
SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS}" )


set (HOST_LIB_NAME "hddl_bypass_drv_video")
set (INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/")

set (DEVICE_BIN_NAME "hddl_bypass_shim")
set (INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")

set (DEVICE_LIB_NAME "hddl_bypass_shim_entry")
set (INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")


if (TARGETS STREQUAL "IA")
        CompileIA ()
elseif (TARGETS STREQUAL "KMB")
	add_definitions (-DUSE_HANTRO)
	CompileARM ()
else ()
	message (FATAL_ERROR "TARGETS not specified - Please set -DTARGETS=<IA/KMB>")
endif ()

message ("Build type: " ${CMAKE_BUILD_TYPE})
