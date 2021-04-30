/*
 * Copyright (c) 2019 Intel Corporation. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//!
//! \file    xlink_pcie.c
//! \brief   Communcation interface for XLINK communication
//! \details Provide xlink basic communication operation
//!

#include "xlink_pcie.h"
#include "debug_manager.h"

int flagOpt;
#ifdef KMB
bool unregisterXLink = false;
bool registerEventCallback = false;
device_state deviceState[MAX_DEVICE];

uint32_t eventList[] = {
    NOTIFY_DEVICE_DISCONNECTED, //device down
    NOTIFY_DEVICE_CONNECTED, //device up
    NOTIFY_DEVICE_UNKNOWN_1, //to be confirm with component owner
    NOTIFY_DEVICE_UNKNOWN_2 // to be confirm with component owner
};
#endif
// TODO: Remove this after XLink provide fixes which allow multiple
// xlink_initialize call
static bool gXlinkInit = false;
static uint16_t gLastChannel = 0;

HDDLShimXLinkContext *XLink_ContextInit (xLinkChannelId_t channelTX, xLinkChannelId_t channelRX)
{
    HDDLShimXLinkContext *xLinkCtx = HDDLMemoryMgr_AllocAndZeroMemory (
        sizeof (HDDLShimXLinkContext));
    SHIM_CHK_NULL (xLinkCtx, "Fail to create XLink context", NULL);

    uint16_t lastChannel = 0;

    xLinkCtx->xLinkChannelTX = channelTX;
    xLinkCtx->xLinkChannelRX = channelRX;

    lastChannel = channelTX > channelRX ? channelTX : channelRX;
    gLastChannel = lastChannel > gLastChannel ? lastChannel : gLastChannel;

    return xLinkCtx;
}

void XLink_GetLastChannel (uint16_t *lastChannel)
{
    *lastChannel = gLastChannel;
}

#ifdef KMB
int XLink_DeviceEventCallback (uint32_t swDeviceId, uint32_t eventType)
{
    if (eventType == NOTIFY_DEVICE_DISCONNECTED)
    {
        SHIM_ERROR_MESSAGE ("Callback: DeviceID %u Down, event = [0x%x].", swDeviceId, eventType);

	for (int i = 0; i < MAX_DEVICE; i++)
        {
	    if (deviceState[i].swDeviceId == swDeviceId)
            {
	        deviceState[i].state = DEVICE_DOWN;
            }
	}
    }
    return 0;
}

XLinkStatus XLink_CheckDeviceEvent (HDDLShimXLinkContext *xLinkCtx)
{
    XLinkStatus xLinkStatus = X_LINK_SUCCESS;

    for (int i = 0; i < MAX_DEVICE; i++)
    {
        if (deviceState[i].swDeviceId == xLinkCtx->xLinkHandler.sw_device_id)
	{
	    if (deviceState[i].state == DEVICE_DOWN)
	    {
                SHIM_ERROR_MESSAGE ("Error: DeviceID %u is down",
		    xLinkCtx->xLinkHandler.sw_device_id);

	        if (unregisterXLink != true)
		{
                    xLinkStatus = xlink_unregister_device_event (&xLinkCtx->xLinkHandler, eventList,
                        MAX_XLINK_EVENT);

		    unregisterXLink = true;
		}

		return X_LINK_ERROR;
	    }
	}
    }
    return xLinkStatus;
}
#endif

XLinkStatus XLink_Initialize (HDDLShimXLinkContext *xLinkCtx, int flag)
{
    XLinkStatus xLinkStatus = X_LINK_SUCCESS;
    uint32_t swDeviceIdList[MAX_DEVICE_LIST_SIZE];
    uint32_t numDevices = 0;
    uint32_t swDeviceId = 0;
    uint32_t deviceStatus = 0;

    SHIM_CHK_NULL (xLinkCtx, "NULL XLink context", COMM_STATUS_FAILED);

    flagOpt = flag;

    if (!gXlinkInit)
    {
        xLinkStatus = xlink_initialize ();
        if (xLinkStatus != X_LINK_SUCCESS)
        {
            SHIM_ERROR_MESSAGE ("Error initialize xlink pcie device with XLink status %d",
		xLinkStatus);
            HDDLMemoryMgr_FreeMemory (xLinkCtx);
            return xLinkStatus;
        }
    }

    xLinkStatus = xlink_get_device_list (swDeviceIdList, &numDevices);

    SHIM_NORMAL_MESSAGE ("%u devices found", numDevices);

    if (xLinkStatus != X_LINK_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Error get device list with XLink status %d", xLinkStatus);
        exit (1);
    }

    for (uint32_t i = 0; i < numDevices; i++)
    {
        uint32_t swInterface = GET_INTERFACE_FROM_SW_DEVICE_ID (swDeviceIdList[i]);
	if (swInterface == SW_DEVICE_ID_PCIE_INTERFACE)
	{
            swDeviceId = swDeviceIdList[i];
	    break;
	}
    }

#ifdef KMB
    for (int i = 0; i < MAX_DEVICE; i++)
    {
       deviceState[i].state = DEVICE_UP;
       deviceState[i].swDeviceId = swDeviceIdList[i];
    }
#endif

    xLinkHandler_t handler = {
        .dev_type = HOST_DEVICE,
	.sw_device_id = swDeviceId
    };

    xLinkCtx->xLinkHandler = handler;

    if (flagOpt == HOST)
    {
#ifdef KMB
	//notification registration function that notify client when the target event[s] occur
        xLinkStatus = xlink_register_device_event (&xLinkCtx->xLinkHandler, eventList,
            MAX_XLINK_EVENT, XLink_DeviceEventCallback);

	registerEventCallback = true;

	if (xLinkStatus != X_LINK_SUCCESS)
	{
	     HDDLMemoryMgr_FreeMemory (xLinkCtx);
	     return xLinkStatus;
        }
#endif
        if (xlink_get_device_status (&xLinkCtx->xLinkHandler, &deviceStatus) == X_LINK_SUCCESS)
        {
            if ( (deviceStatus == XLINK_DEV_OFF) || (deviceStatus == XLINK_DEV_READY))
            {
		SHIM_NORMAL_MESSAGE ("Booting device file: %s......\n", BOOT_LOCAL_HOST_FIP_FILE);
                xLinkStatus = xlink_boot_device (&xLinkCtx->xLinkHandler, BOOT_LOCAL_HOST_FIP_FILE);
                if (xLinkStatus != X_LINK_SUCCESS)
                {
                    SHIM_ERROR_MESSAGE ("Failed to boot FIP with XLink status %d", xLinkStatus);
                    exit (1);
                }

		SHIM_NORMAL_MESSAGE ("Booting device file: %s......\n", BOOT_LOCAL_HOST_IMAGE_FILE);
		xLinkStatus = xlink_boot_device (&xLinkCtx->xLinkHandler, BOOT_LOCAL_HOST_IMAGE_FILE);
                if (xLinkStatus != X_LINK_SUCCESS)
                {
                    SHIM_ERROR_MESSAGE ("Failed to boot Image file with XLink status %d",
		        xLinkStatus);
                    exit (1);
                }

            }
            else
            {
                SHIM_ERROR_MESSAGE ("XLink Device Status Bad State %u\n", deviceStatus);
            }
        }
        else
        {
            SHIM_ERROR_MESSAGE ("xlink_get_device_status failed with XLink status %d",
	        xLinkStatus);
        }

    }

    HDDLThreadMgr_InitMutex (&xLinkCtx->xLinkMutex);
    gXlinkInit = true;

    return xLinkStatus;
}

XLinkStatus XLink_Connect (HDDLShimXLinkContext *xLinkCtx)
{
    XLinkStatus xLinkStatus;

    SHIM_CHK_NULL (xLinkCtx, "NULL XLink context", COMM_STATUS_FAILED);

    xLinkStatus = xlink_connect (&xLinkCtx->xLinkHandler);

    if (!(xLinkStatus == X_LINK_SUCCESS || xLinkStatus == X_LINK_ALREADY_OPEN))
    {
        SHIM_NORMAL_MESSAGE ("Error connect XLink PCIe device");
        return COMM_STATUS_FAILED;
    }

    // Open XLink TX Channel
    // A workaround solution to get rid of the timeout issue of xlink_open_channel where
    // open channel failed after reach the timeout.
    SHIM_NORMAL_MESSAGE ("[TX channel %u] Attempt to open", xLinkCtx->xLinkChannelTX);
    do
    {
        xLinkStatus = xlink_open_channel (&xLinkCtx->xLinkHandler, xLinkCtx->xLinkChannelTX,
            OPERATION_TYPE, DATA_FRAGMENT_SIZE, OPEN_CHANNEL_TIMEOUT);
    } while (xLinkStatus == X_LINK_TIMEOUT);
    SHIM_CHK_EQUAL (xLinkStatus, X_LINK_ERROR, "Failed to open channel", X_LINK_ERROR);
    SHIM_NORMAL_MESSAGE ("[TX channel %u] Open done", xLinkCtx->xLinkChannelTX);

    // Open XLink RX Channel
    // A workaround solution to get rid of the timeout issue of xlink_open_channel where
    // open channel failed after reach the timeout.
    SHIM_NORMAL_MESSAGE ("[RX channel %u] Attempt to open", xLinkCtx->xLinkChannelRX);
    do
    {
        xLinkStatus = xlink_open_channel (&xLinkCtx->xLinkHandler, xLinkCtx->xLinkChannelRX,
            OPERATION_TYPE, DATA_FRAGMENT_SIZE, OPEN_CHANNEL_TIMEOUT);
    } while (xLinkStatus == X_LINK_TIMEOUT);
    SHIM_CHK_EQUAL (xLinkStatus, X_LINK_ERROR, "Failed to open channel", X_LINK_ERROR);
    SHIM_NORMAL_MESSAGE ("[RX channel %u] Open done", xLinkCtx->xLinkChannelRX);

    // XLink_Connect function considered fail only if return status from XLink function call
    // is X_LINK_ERROR, which has already being handled in previous code segment. Thus return
    // X_LINK_SUCCESS directly when reaching this point.
    return X_LINK_SUCCESS;
}

XLinkStatus XLink_Write (HDDLShimXLinkContext *xLinkCtx, int size, void *payload)
{
    XLinkStatus xLinkStatus = X_LINK_SUCCESS;
    uint32_t writeSize = size;

    SHIM_CHK_NULL (xLinkCtx, "NULL XLink context", COMM_STATUS_FAILED);
    SHIM_CHK_NULL (payload, "null payload", X_LINK_ERROR);

#ifdef KMB
    if (registerEventCallback == true)
    {
        xLinkStatus = XLink_CheckDeviceEvent (xLinkCtx);
	SHIM_CHK_ERROR (xLinkStatus, "XLink Device Down", X_LINK_ERROR);
    }
#endif

    while (writeSize > DATA_MAX_SEND_SIZE)
    {
        SHIM_NORMAL_MESSAGE ("Splitting write data to smaller chunk");

        SHIM_NORMAL_MESSAGE ("[TX channel %u] Attempt to write", xLinkCtx->xLinkChannelTX);
        xLinkStatus = xlink_write_data (&xLinkCtx->xLinkHandler, xLinkCtx->xLinkChannelTX,
            (uint8_t*)payload, (uint32_t)DATA_MAX_SEND_SIZE);

	if (xLinkStatus != X_LINK_SUCCESS)
	{
	    SHIM_ERROR_MESSAGE ("DeviceID %u Channel %u: Failed to write data to the device "
                "with XLink status %d", xLinkCtx->xLinkHandler.sw_device_id,
                xLinkCtx->xLinkChannelTX, xLinkStatus);
	    return xLinkStatus;
	}

        SHIM_NORMAL_MESSAGE ("[TX channel %u] Write done", xLinkCtx->xLinkChannelTX);

        writeSize -= DATA_MAX_SEND_SIZE;
        payload += DATA_MAX_SEND_SIZE;
    }

#if defined (XLINK_SECURE)
    //Workaround as current secureXLink does not support xlink_write_control_data api
    //TODO: Will re-examine once secureXLink support xlink_write_control data api
    SHIM_NORMAL_MESSAGE ("[TX channel %u] Attempt to write", xLinkCtx->xLinkChannelTX);

    xLinkStatus = xlink_write_data (&xLinkCtx->xLinkHandler, xLinkCtx->xLinkChannelTX,
                (uint8_t*)payload, (uint32_t)writeSize);

    if (xLinkStatus != X_LINK_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("DeviceID %u Channel %u: Failed to write data to the device with "
            "XLink status %d", xLinkCtx->xLinkHandler.sw_device_id, xLinkCtx->xLinkChannelTX,
            xLinkStatus);
        return xLinkStatus;
    }

    SHIM_NORMAL_MESSAGE ("[TX channel %u] Write done", xLinkCtx->xLinkChannelTX);
#else
    if (writeSize > 0)
    {
        SHIM_NORMAL_MESSAGE ("[TX channel %u] Attempt to write", xLinkCtx->xLinkChannelTX);

	// Use xlink_write_control_data for data size less than 100 bytes since it has less DMA
	// transfer compare to xlink_write_data. The limit for xlink_write_control_data is 100 bytes.
	// This is for performance optimization.
	if (writeSize < 100)
	{
            xLinkStatus = xlink_write_control_data (&xLinkCtx->xLinkHandler,
                xLinkCtx->xLinkChannelTX, (uint8_t*)payload, (uint32_t)writeSize);

	    if (xLinkStatus != X_LINK_SUCCESS)
            {
		SHIM_ERROR_MESSAGE ("DeviceID %u Channel %u: Failed to write data to the device "
                    "with XLink status %d", xLinkCtx->xLinkHandler.sw_device_id,
                    xLinkCtx->xLinkChannelTX, xLinkStatus);
		return xLinkStatus;
            }
	}
	else
	{
            xLinkStatus = xlink_write_data (&xLinkCtx->xLinkHandler, xLinkCtx->xLinkChannelTX,
                (uint8_t*)payload, (uint32_t)writeSize);

            if (xLinkStatus != X_LINK_SUCCESS)
            {
                SHIM_ERROR_MESSAGE ("DeviceID %u Channel %u: Failed to write data to the device "
                    "with XLink status %d", xLinkCtx->xLinkHandler.sw_device_id,
                    xLinkCtx->xLinkChannelTX, xLinkStatus);
                return xLinkStatus;
            }
        }
        SHIM_NORMAL_MESSAGE ("[TX channel %u] Write done", xLinkCtx->xLinkChannelTX);

    }
#endif

    return xLinkStatus;
}

XLinkStatus XLink_Read (HDDLShimXLinkContext *xLinkCtx, int size, void *payload)
{
    XLinkStatus xLinkStatus = X_LINK_SUCCESS;
    uint32_t dataSize = 0;
    uint32_t readSize = size;

    SHIM_CHK_NULL (xLinkCtx, "NULL XLink context", COMM_STATUS_FAILED);

#ifdef KMB
    if (registerEventCallback == true)
    {
        xLinkStatus = XLink_CheckDeviceEvent (xLinkCtx);
	SHIM_CHK_ERROR (xLinkStatus, "XLink Device Down", X_LINK_ERROR);
    }
#endif

    while (readSize > DATA_MAX_SEND_SIZE)
    {
        SHIM_NORMAL_MESSAGE ("Reading huge data in smaller chunk");

        dataSize = 0;

        SHIM_NORMAL_MESSAGE ("[RX channel %u] Attempt to read", xLinkCtx->xLinkChannelRX);
        xLinkStatus = xlink_read_data (&xLinkCtx->xLinkHandler, xLinkCtx->xLinkChannelRX,
            (uint8_t **)&payload, &dataSize);

        if (xLinkStatus != X_LINK_SUCCESS)
        {
            SHIM_ERROR_MESSAGE ("DeviceID %u Channel %u: Failed to read data from device with "
                "XLink status %d", xLinkCtx->xLinkHandler.sw_device_id,
                xLinkCtx->xLinkChannelRX, xLinkStatus);
            return xLinkStatus;
        }

        SHIM_NORMAL_MESSAGE ("[RX channel %u] Read done", xLinkCtx->xLinkChannelRX);

        readSize -= dataSize;
        payload += dataSize;

        xLinkStatus = xlink_release_data (&xLinkCtx->xLinkHandler, xLinkCtx->xLinkChannelRX, NULL);
        SHIM_CHK_ERROR (xLinkStatus, "Failed to release read data", X_LINK_ERROR);
    }

    if (readSize > 0)
    {
        SHIM_NORMAL_MESSAGE ("[RX channel %u] Attempt to read", xLinkCtx->xLinkChannelRX);
        xLinkStatus = xlink_read_data (&xLinkCtx->xLinkHandler, xLinkCtx->xLinkChannelRX,
            (uint8_t **)&payload, &dataSize);

	if (xLinkStatus != X_LINK_SUCCESS)
        {
            SHIM_ERROR_MESSAGE ("DeviceID %u Channel %u: Failed to read data from device with "
                "XLink status %d", xLinkCtx->xLinkHandler.sw_device_id,
                xLinkCtx->xLinkChannelRX, xLinkStatus);
            return xLinkStatus;
        }

        SHIM_NORMAL_MESSAGE ("[RX channel %u] Read done", xLinkCtx->xLinkChannelRX);

        xLinkStatus = xlink_release_data (&xLinkCtx->xLinkHandler, xLinkCtx->xLinkChannelRX, NULL);
        SHIM_CHK_ERROR (xLinkStatus, "Failed to release read data", X_LINK_ERROR);
    }

    return xLinkStatus;
}

XLinkStatus XLink_Peek (HDDLShimXLinkContext *xLinkCtx, uint32_t *size, void **payload)
{
    XLinkStatus xLinkStatus = X_LINK_SUCCESS;

    SHIM_CHK_NULL (xLinkCtx, "NULL XLink context", COMM_STATUS_FAILED);

#ifdef KMB
    if (registerEventCallback == true)
    {
        xLinkStatus = XLink_CheckDeviceEvent (xLinkCtx);
	SHIM_CHK_ERROR (xLinkStatus, "XLink Device Down", X_LINK_ERROR);
    }
#endif

    *payload = HDDLMemoryMgr_AllocMemory (DATA_MAX_SEND_SIZE);

    SHIM_NORMAL_MESSAGE ("[RX channel %u] Attempt to read", xLinkCtx->xLinkChannelRX);

    xLinkStatus = xlink_read_data (&xLinkCtx->xLinkHandler, xLinkCtx->xLinkChannelRX,
        (uint8_t **)payload, size);

    if (xLinkStatus != X_LINK_SUCCESS)
    {
	SHIM_ERROR_MESSAGE ("DeviceID %u Channel %u: Failed to read data from device with XLink "
            "status %d", xLinkCtx->xLinkHandler.sw_device_id,
            xLinkCtx->xLinkChannelRX, xLinkStatus);
        return xLinkStatus;
    }

    SHIM_CHK_NULL (*payload, "Payload returned NULL", X_LINK_ERROR);

    SHIM_NORMAL_MESSAGE ("[RX channel %u] Read done", xLinkCtx->xLinkChannelRX);

    if (*size != DATA_MAX_SEND_SIZE)
    {
        *payload = HDDLMemoryMgr_ReallocMemory (*payload, *size);
	SHIM_CHK_NULL (*payload, "Payload returned NULL", X_LINK_ERROR);
    }

    xLinkStatus = xlink_release_data (&xLinkCtx->xLinkHandler, xLinkCtx->xLinkChannelRX, NULL);
    SHIM_CHK_ERROR (xLinkStatus, "Failed to release read data", X_LINK_ERROR);

    return xLinkStatus;
}

XLinkStatus XLink_Disconnect (HDDLShimXLinkContext *xLinkCtx, int flag)
{
    XLinkStatus xLinkStatus = X_LINK_SUCCESS;

    SHIM_CHK_NULL (xLinkCtx, "NULL XLink context", COMM_STATUS_FAILED);

    // Clean up and disconnect the pcie device
    SHIM_NORMAL_MESSAGE ("[TX channel %u] Attempt to close", xLinkCtx->xLinkChannelTX);
    xLinkStatus = xlink_close_channel (&xLinkCtx->xLinkHandler, xLinkCtx->xLinkChannelTX);
    SHIM_CHK_ERROR (xLinkStatus, "Failed to close TX channel", X_LINK_ERROR);
    SHIM_NORMAL_MESSAGE ("[TX channel %u] Close done", xLinkCtx->xLinkChannelTX);

    SHIM_NORMAL_MESSAGE ("[RX channel %u] Attempt to close", xLinkCtx->xLinkChannelRX);
    xLinkStatus = xlink_close_channel (&xLinkCtx->xLinkHandler, xLinkCtx->xLinkChannelRX);
    SHIM_CHK_ERROR (xLinkStatus, "Failed to close RX channel", X_LINK_ERROR);
    SHIM_NORMAL_MESSAGE ("[RX channel %u] Close done", xLinkCtx->xLinkChannelRX);

    if (flag == HOST)
    {
#ifdef KMB
	//Unregister the client from receiving event
	//TODO: Temperary remove unregister where the first unregister call will unregister all the
        //event on other thread as well.
        //HSD filed: https://hsdes.intel.com/resource/1508442210
        //
	//if (registerEventCallback == true)
	//{
	//    xLinkStatus = xlink_unregister_device_event (&xLinkCtx->xLinkHandler, eventList,
        //        MAX_XLINK_EVENT);
	//    SHIM_CHK_ERROR (xLinkStatus, "Failed to unregister device", X_LINK_ERROR);
	//}
#endif
        HDDLThreadMgr_DestroyMutex (&xLinkCtx->xLinkMutex);
        HDDLMemoryMgr_FreeMemory (xLinkCtx);
    }

    return xLinkStatus;
}
