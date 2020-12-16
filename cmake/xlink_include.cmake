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

function (FindXLink)

       if (DEFINED ENV{XLINK_HOME})
                add_definitions (-DXLINK_SIMULATOR)
                message ("XLink Simulator Path found: $ENV{XLINK_HOME}")

                set (XLINK_ROOT_DIR $ENV{XLINK_HOME})

                if (TARGETS STREQUAL "IA")
                       set (XLINK_LIB "${XLINK_ROOT_DIR}/libXLinkPC.so")
                       message("XLink simulator library: ${XLINK_LIB}")
                       add_library (libXLinkPC SHARED IMPORTED GLOBAL)
                       set_target_properties(libXLinkPC PROPERTIES IMPORTED_LOCATION ${XLINK_LIB})

                elseif (TARGETS STREQUAL "KMB")
                       set (XLINK_LIB "${XLINK_ROOT_DIR}/libXLinkARM.so")
                       message("XLink simulator library: ${XLINK_LIB}")
                       add_library (libXLinkARM SHARED IMPORTED GLOBAL)
                       set_target_properties(libXLinkARM PROPERTIES IMPORTED_LOCATION ${XLINK_LIB})

                else ()
                       set (XLINK_LIBPC "${XLINK_ROOT_DIR}/libXLinkPC.so")
                       message("XLink simulator library: ${XLINK_LIBPC}")
                       set (XLINK_LIBARM "${XLINK_ROOT_DIR}/libXLinkARM.so")
                       message("XLink simulator library: ${XLINK_LIBARM}")
                       add_library (libXLinkPC SHARED IMPORTED GLOBAL)
                       set_target_properties(libXLinkPC PROPERTIES IMPORTED_LOCATION ${XLINK_LIBPC})
                       add_library (libXLinkARM SHARED IMPORTED GLOBAL)
                       set_target_properties(libXLinkARM PROPERTIES IMPORTED_LOCATION ${XLINK_LIBARM})
               endif ()
       else ()

               find_library (XLINK_LIB XLink)

               if (XLINK_LIB)
                       add_definitions (-DXLINK)
                       message ("XLink Library located at ${XLINK_LIB}")
	       else ()
                       message ("Cannot locate XLink Library")
	       endif ()
       endif ()

endfunction (FindXLink)
