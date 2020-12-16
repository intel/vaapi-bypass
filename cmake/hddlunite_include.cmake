function (FindHDDLUnite)

    if (DEFINED ENV{HDDLUNITE})
	add_definitions (-DHDDL_UNITE)
	find_library (HDDLUNITE_LIB 
	NAMES HddlUnite
	HINTS "/opt/intel/hddlunite/lib")

	if (HDDLUNITE_LIB)
		message ("HDDLUnite Library located at ${HDDLUNITE_LIB}")
        else ()
                message ("Cannot locate HDDLUnite Library")
        endif ()

        find_library (DEVICE_CLIENT_LIB
        NAMES DeviceClient
        HINTS "/opt/intel/hddlunite/lib")
        
        if (DEVICE_CLIENT_LIB)
            message ("HDDL DeviceClient Library located at ${DEVICE_CLIENT_LIB}")
        else ()
            message ("Cannot locate HDDL DeviceClient Library")
        endif ()

	set (DEFAULT_INCLUDE_DIR "/opt/intel/hddlunite/include")

	if (DEFINED ENV{BYPASS_INCLUDE_DIR})
	    set (HDDLUNITE_INCLUDE_DIR $ENV{BYPASS_INCLUDE_DIR})
        else ()
	    set (HDDLUNITE_INCLUDE_DIR ${DEFAULT_INCLUDE_DIR})
	endif ()

	include_directories (${HDDLUNITE_INCLUDE_DIR})
	message ("HddlUnite header directory: ${HDDLUNITE_INCLUDE_DIR}")

    else ()
	message ("HDDLUNITE not DEFINED and not included")
    endif ()

endfunction (FindHDDLUnite)
