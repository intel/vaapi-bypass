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

function (CompileIA)
	add_definitions (-DIA)

	add_library (${HOST_LIB_NAME} SHARED ${HOST_SOURCES})
	set_target_properties (${HOST_LIB_NAME} PROPERTIES PREFIX "")
	target_link_libraries (${HOST_LIB_NAME} ${HDDLUNITE_LIB})

	if (DEFINED ENV{XLINK_HOME})
		target_link_libraries (${HOST_LIB_NAME} libXLinkPC)
        elseif (XLINK_LIB)
		target_link_libraries (${HOST_LIB_NAME} ${XLINK_LIB})
        endif ()

	if (LTTNG_TRACE)
		target_link_libraries (${HOST_LIB_NAME} ${LTTNG_DL_LIB})
		target_link_libraries (${HOST_LIB_NAME} ${LTTNG_UST_LIB})
	endif()

	if (ITT_TRACE)
		target_link_libraries (${HOST_LIB_NAME} ${ITT_LIB})
	endif ()

	install (TARGETS ${HOST_LIB_NAME} DESTINATION ${INSTALL_LIB_DIR})

	if (USE_HANTRO_DRIVER STREQUAL "KMB")
		install (DIRECTORY ${CMAKE_SOURCE_DIR}/src/ext/va_hantro_kmb/
			DESTINATION ${CMAKE_INSTALL_PREFIX}/include/va_hantro/
			PATTERN "*.h")
	endif ()

endfunction (CompileIA)

function (CompileARM)
	add_definitions (-DACCEL)

	add_executable (${DEVICE_BIN_NAME} ${TARGET_BIN_SOURCES})
	add_library (${DEVICE_LIB_NAME} SHARED ${TARGET_LIB_SOURCES})

	set (LINK_LIBS ${LIBVA_LIBRARIES}
		${DEVICE_CLIENT_LIB}
		${CMAKE_CXX_FLAGS}
	)

	if (DEFINED ENV{XLINK_HOME})
		set (LINK_LIBS ${LINK_LIBS} libXLinkARM)
	elseif (XLINK_LIB)
		set (LINK_LIBS ${LINK_LIBS} ${XLINK_LIB})
        endif ()

	if (LTTNG_TRACE)
		target_link_libraries (${DEVICE_BIN_NAME} ${LTTNG_DL_LIB})
		target_link_libraries (${DEVICE_BIN_NAME} ${LTTNG_UST_LIB})
		target_link_libraries (${DEVICE_LIB_NAME} ${LTTNG_DL_LIB})
		target_link_libraries (${DEVICE_LIB_NAME} ${LTTNG_UST_LIB})
	endif()

	if (ITT_TRACE)
		target_link_libraries (${DEVICE_BIN_NAME} ${ITT_LIB})
		target_link_libraries (${DEVICE_LIB_NAME} ${ITT_LIB})
	endif ()

	target_link_libraries (${DEVICE_BIN_NAME} ${LINK_LIBS})
	target_link_libraries (${DEVICE_LIB_NAME} ${LINK_LIBS})

	install (TARGETS ${DEVICE_BIN_NAME} DESTINATION ${INSTALL_BIN_DIR})
	install (TARGETS ${DEVICE_LIB_NAME} DESTINATION ${INSTALL_LIB_DIR})
	install (FILES ${CMAKE_SOURCE_DIR}/src/target/target_shim_entry.h
		DESTINATION ${INSTALL_INCLUDE_DIR})
endfunction (CompileARM)
