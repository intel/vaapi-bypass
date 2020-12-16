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

function (FindSafeStringLib)

    if (DEFINED ENV{USE_SYSTEM_SAFESTR})
        find_library (SAFESTRINGLIB safestring_shared)
        if (SAFESTRINGLIB)
		message ("Use system installed safestring: ${SAFESTRINGLIB}")
	else ()
		message ("USE_SYSTEM_SAFESTR is set but safestring not found in system "
			"installed directories")
	endif()
    elseif (DEFINED ENV{SAFESTR_HOME})
        message ("SafeStringlib Path found: $ENV{SAFESTR_HOME}")

        set (SAFESTRINGLIB_SRC_DIR $ENV{SAFESTR_HOME})
        set (SAFESTRINGLIB_INCLUDE_DIR "${SAFESTRINGLIB_SRC_DIR}/include")

        include_directories (${SAFESTRINGLIB_INCLUDE_DIR})
        message ("SafeStringlib include directory: ${SAFESTRINGLIB_INCLUDE_DIR}")

	set (SAFESTRINGLIB "${SAFESTRINGLIB_SRC_DIR}/build/libsafestring_static.a")
        message ("safestring library: ${SAFESTRINGLIB}")
        add_library (libsafestring STATIC IMPORTED GLOBAL)
        set_target_properties (libsafestring PROPERTIES IMPORTED_LOCATION ${SAFESTRINGLIB})

    else ()
        message( "SAFESTR_HOME not DEFINED")
    endif ()

endfunction (FindSafeStringLib)
