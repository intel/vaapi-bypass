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

function (FindLTTNG)
	find_library (LTTNG_DL_LIB dl)
	if (LTTNG_DL_LIB)
		message ("LTTNG DL Library located at ${LTTNG_DL_LIB}")
	else ()
		message ("Cannot locate LTTNG DL Library")
        endif ()

	find_library (LTTNG_UST_LIB lttng-ust)
	if (LTTNG_UST_LIB)
		message ("LTTNG UST Library located at ${LTTNG_UST_LIB}")
	else ()
		message ("Cannot locate LTTNG UST Library")
	endif ()
endfunction (FindLTTNG)

function (AddARMITT)
	set (ITT_LIB ${CMAKE_SOURCE_DIR}/thirdparty/itt/arm/lib/libittnotify.a)
	message ("ITT Library located at ${ITT_LIB}")
endfunction (AddARMITT)

function (AddX86ITT)
	set (ITT_LIB ${CMAKE_SOURCE_DIR}/thirdparty/itt/x86/lib/libittnotify.a)
	message ("ITT Library located at ${ITT_LIB}")
endfunction (AddX86ITT)
