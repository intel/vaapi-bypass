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
//! \file    unite.c
//! \brief   Communcation interface for HDDLUnite
//! \details Provide HDDLUnite basic communication operation
//!

#include "unite.h"

static bool gXlinkInit = false;

#ifdef KMB
bool unregisterUnite = false;
device_state uniteDeviceState[MAX_DEVICE];

uint32_t uniteEventList[] = {
    NOTIFY_DEVICE_DISCONNECTED, //device down
    NOTIFY_DEVICE_CONNECTED, //device up
    NOTIFY_DEVICE_UNKNOWN_1, //to be confirm with component owner
    NOTIFY_DEVICE_UNKNOWN_2 // to be confirm with component owner
};
#endif

#ifdef KMB
int Unite_DeviceEventCallback (uint32_t swDeviceId, uint32_t eventType)
{
    if (eventType == NOTIFY_DEVICE_DISCONNECTED)
    {
        SHIM_ERROR_MESSAGE ("Callback: DeviceID %u Down, event = [0x%x].", swDeviceId, eventType);

        for (int i = 0; i < MAX_DEVICE; i++)
        {
            if (uniteDeviceState[i].swDeviceId == swDeviceId)
            {
                uniteDeviceState[i].state = DEVICE_DOWN;
            }
        }
    }
    return 0;
}

XLinkStatus Unite_CheckDeviceEvent (HDDLShimUniteContext *uniteCtx)
{
    XLinkStatus xLinkStatus = X_LINK_SUCCESS;

    for (int i = 0; i < MAX_DEVICE; i++)
    {
        if (uniteDeviceState[i].swDeviceId == uniteCtx->xLinkCtx->xLinkHandler.sw_device_id)
        {
            if (uniteDeviceState[i].state == DEVICE_DOWN)
            {
                SHIM_ERROR_MESSAGE ("Error: DeviceID %u is down",
                    uniteCtx->xLinkCtx->xLinkHandler.sw_device_id);

                if (unregisterUnite != true)
                {
	            //TODO: W/A Temperary remove unregister where the first unregister call will unregister all the
                    //event on other thread as well.
                    //HSD filed: https://hsdes.intel.com/resource/1508442210
                    //xLinkStatus = xlink_unregister_device_event (&uniteCtx->xLinkCtx->xLinkHandler,
		    //    uniteEventList, MAX_XLINK_EVENT);

                    unregisterUnite = true;
                }

                return X_LINK_ERROR;
            }
        }
    }
    return xLinkStatus;
}
#endif

HDDLShimUniteContext *Unite_ContextInit (xLinkChannelId_t channelTX, xLinkChannelId_t channelRX,
    uint64_t workloadId, uint32_t swDeviceId)
{
    HDDLShimUniteContext *uniteCtx = HDDLMemoryMgr_AllocAndZeroMemory (
        sizeof (HDDLShimUniteContext));
    SHIM_CHK_NULL (uniteCtx, "Fail to create Unite context", NULL);

    HDDLShimXLinkContext *xLinkCtx = HDDLMemoryMgr_AllocAndZeroMemory (
        sizeof (HDDLShimXLinkContext));

    if (xLinkCtx == NULL)
    {
        SHIM_ERROR_MESSAGE ("Failed to create XLink context");
	HDDLMemoryMgr_FreeMemory (uniteCtx);
	return NULL;
    }

    uniteCtx->internalWorkloadId = WORKLOAD_ID_NONE;
    uniteCtx->workloadId = WORKLOAD_ID_NONE;

    if ( (channelTX != QUERY_FROM_UNITE) && (channelRX != QUERY_FROM_UNITE) &&
        (workloadId != QUERY_FROM_UNITE))
    {
        xLinkCtx->xLinkChannelTX = channelTX;
        xLinkCtx->xLinkChannelRX = channelRX;
        uniteCtx->workloadId = workloadId;
        uniteCtx->xLinkCtx = xLinkCtx;
        return uniteCtx;
    }

#if defined (HDDL_UNITE) && defined (IA)
    HddlStatusCode hddlStatus;
    uint64_t workload;
    uint32_t deviceId = -1;
    ChannelID channelIdList[MAX_XLINK_CHANNELS];

    hddlStatus = getWorkloadContextId (getpid (), syscall (SYS_gettid), &workload);
    if (hddlStatus != HDDL_OK)
    {
        // Internally create HDDLUnite workload ID to support application which
        // does not explicitly modify for HDDLUnite
        ContextHint contextHint;
        contextHint.internalWorkload = 1;

        hddlStatus = createWorkloadContext (&workload, &contextHint);
        if (hddlStatus != HDDL_OK)
        {
            HDDLMemoryMgr_FreeMemory (xLinkCtx);
            HDDLMemoryMgr_FreeMemory (uniteCtx);
            return NULL;
        }

        uniteCtx->internalWorkloadId = workload;

        SHIM_NORMAL_MESSAGE ("Internal workload %ld created", uniteCtx->internalWorkloadId);
    }
    else
    {
        uniteCtx->workloadId = workload;
    }

    getSwDeviceId (workload, &deviceId);
    uniteCtx->swDeviceId = deviceId;

    hddlStatus = allocateVAChannelId (workload, channelIdList, MAX_XLINK_CHANNELS);
    if (hddlStatus != HDDL_OK)
    {
        HDDLMemoryMgr_FreeMemory (xLinkCtx);
        HDDLMemoryMgr_FreeMemory (uniteCtx);
        return NULL;
    }

    xLinkCtx->xLinkChannelTX = channelIdList[0];
    xLinkCtx->xLinkChannelRX = channelIdList[1];
#endif

    uniteCtx->xLinkCtx = xLinkCtx;

    return uniteCtx;
}

CommStatus Unite_Initialize (HDDLShimUniteContext *uniteCtx, int flag)
{
    HDDLShimXLinkContext *xLinkCtx;
    XLinkStatus xLinkStatus;
    uint32_t swDeviceIdList[MAX_DEVICE_LIST_SIZE];
    uint32_t numDevices;
    uint32_t swDeviceId = -1;
    uint32_t deviceStatus = 0;

    SHIM_CHK_NULL (uniteCtx, "NULL Unite context", COMM_STATUS_FAILED);

    xLinkCtx = uniteCtx->xLinkCtx;
    SHIM_CHK_NULL (xLinkCtx, "NULL XLink context", COMM_STATUS_FAILED);

    if (!gXlinkInit)
    {
	xLinkStatus = xlink_initialize ();
        if (xLinkStatus != X_LINK_SUCCESS)
        {
            SHIM_ERROR_MESSAGE ("Error initialize XLink PCIe");
            HDDLMemoryMgr_FreeMemory (xLinkCtx);
            HDDLMemoryMgr_FreeMemory (uniteCtx);
            return COMM_STATUS_FAILED;
        }
    }

    xLinkStatus = xlink_get_device_list (swDeviceIdList, &numDevices);
    SHIM_NORMAL_MESSAGE ("%u devices found", numDevices);

    if (xLinkStatus != X_LINK_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Error get device list");
	return COMM_STATUS_FAILED;
    }

    if (flag == HOST)
    {
        swDeviceId = uniteCtx->swDeviceId;
    }
    else if (flag == TARGET)
    {
	for (uint32_t i = 0; i < numDevices; i++)
        {
            uint32_t swInterface = GET_INTERFACE_FROM_SW_DEVICE_ID (swDeviceIdList[i]);
            if (swInterface == SW_DEVICE_ID_PCIE_INTERFACE)
            {
                swDeviceId = swDeviceIdList[i];
                break;
            }
        }
    }

    xLinkHandler_t handler = {
       .dev_type = HOST_DEVICE,
       .sw_device_id = swDeviceId
    };

    xLinkCtx->xLinkHandler = handler;
    HDDLThreadMgr_InitMutex (&xLinkCtx->xLinkMutex);

    if (flag == HOST)
    {
#ifdef KMB
        for (int i = 0; i < MAX_DEVICE; i++)
        {
           uniteDeviceState[i].state = DEVICE_UP;
           uniteDeviceState[i].swDeviceId = swDeviceIdList[i];
        }

        //notification registration function that notify client when the target event[s] occur
        xLinkStatus = xlink_register_device_event (&xLinkCtx->xLinkHandler, uniteEventList,
            MAX_XLINK_EVENT, Unite_DeviceEventCallback);

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
                    SHIM_ERROR_MESSAGE ("Failed to boot FIP");
                    exit (1);
                }

                SHIM_NORMAL_MESSAGE ("Booting device file: %s......\n", BOOT_LOCAL_HOST_IMAGE_FILE);
                xLinkStatus = xlink_boot_device (&xLinkCtx->xLinkHandler, BOOT_LOCAL_HOST_IMAGE_FILE);
                if (xLinkStatus != X_LINK_SUCCESS)
                {
                    SHIM_ERROR_MESSAGE ("Failed to boot Image file");
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
            SHIM_ERROR_MESSAGE ("xlink_get_device_status failed");
	}
    }

    gXlinkInit = true;

    return COMM_STATUS_SUCCESS;
}

CommStatus Unite_Connect (HDDLShimUniteContext *uniteCtx)
{
    SHIM_CHK_NULL (uniteCtx, "NULL Unite context", COMM_STATUS_FAILED);

    XLinkStatus xLinkStatus = XLink_Connect (uniteCtx->xLinkCtx);
    SHIM_CHK_ERROR (xLinkStatus, "Failed to connect device", COMM_STATUS_FAILED);

    return COMM_STATUS_SUCCESS;
}

CommStatus Unite_Write (HDDLShimUniteContext *uniteCtx, int size, void *payload)
{
    SHIM_CHK_NULL (uniteCtx, "NULL Unite context", COMM_STATUS_FAILED);

    XLinkStatus xLinkStatus = X_LINK_ERROR;

#if defined (HDDL_UNITE) && defined (IA) && defined (KMB)
    int timeoutCount = 0;
    ChannelID channelIdList[MAX_XLINK_CHANNELS];
    channelIdList[0] = uniteCtx->xLinkCtx->xLinkChannelTX;
    channelIdList[1] = uniteCtx->xLinkCtx->xLinkChannelRX;

    do
    {
	HddlStatusCode hddlStatus;

        xLinkStatus = Unite_CheckDeviceEvent (uniteCtx);
        SHIM_CHK_ERROR (xLinkStatus, "XLink Device Down", COMM_STATUS_FAILED);

        xLinkStatus = XLink_Write (uniteCtx->xLinkCtx, size, payload);
	SHIM_CHK_EQUAL (xLinkStatus, X_LINK_ERROR, "Failed to write data", COMM_STATUS_FAILED);

	if (xLinkStatus == X_LINK_TIMEOUT)
	{
	    timeoutCount += 1;

	    if (timeoutCount == 2)
	    {
	        SHIM_ERROR_MESSAGE ("xLink timeout two times. Performing Reset");

		uint64_t workloadId = uniteCtx->workloadId == WORKLOAD_ID_NONE ?
		    uniteCtx->internalWorkloadId : uniteCtx->workloadId;

		for (int i = 0; i < MAX_XLINK_CHANNELS; i++)
                {
                    hddlStatus = releaseVAChannelId (workloadId, channelIdList[i]);
                    if (hddlStatus != HDDL_OK)
                    {
                        SHIM_NORMAL_MESSAGE ("Failed to release channel %d for workload %ld",
                            channelIdList[i], uniteCtx->workloadId);
                        return COMM_STATUS_FAILED;
                    }
                }

		hddlStatus = resetDevice (workloadId, RESET_SOURCE, RESET_REASON);
		if (hddlStatus != HDDL_OK)
                {
                    SHIM_ERROR_MESSAGE ("HDDLUnite reset device failed");
                    return COMM_STATUS_FAILED;
                }

		SHIM_ERROR_MESSAGE ("DeviceID %u: HDDLUnite reset device done",
		    uniteCtx->xLinkCtx->xLinkHandler.sw_device_id);
		return COMM_STATUS_FAILED;
	    }
	}
    }
    while (xLinkStatus == X_LINK_TIMEOUT);
#else
    xLinkStatus = XLink_Write (uniteCtx->xLinkCtx, size, payload);
    SHIM_CHK_ERROR (xLinkStatus, "Failed to write data", COMM_STATUS_FAILED);
#endif

    return COMM_STATUS_SUCCESS;
}

CommStatus Unite_Read (HDDLShimUniteContext *uniteCtx, int size, void *payload)
{
    SHIM_CHK_NULL (uniteCtx, "NULL Unite context", COMM_STATUS_FAILED);

    XLinkStatus xLinkStatus = X_LINK_ERROR;

#ifdef KMB
    xLinkStatus = Unite_CheckDeviceEvent (uniteCtx);
    SHIM_CHK_ERROR (xLinkStatus, "XLink Device Down", COMM_STATUS_FAILED);
#endif

    xLinkStatus = XLink_Read (uniteCtx->xLinkCtx, size, payload);
    SHIM_CHK_ERROR (xLinkStatus, "Failed to read data", COMM_STATUS_FAILED);

    return COMM_STATUS_SUCCESS;
}

CommStatus Unite_Peek (HDDLShimUniteContext *uniteCtx, uint32_t *size, void *payload)
{
    SHIM_CHK_NULL (uniteCtx, "NULL Unite context", COMM_STATUS_FAILED);

    XLinkStatus xLinkStatus = X_LINK_ERROR;

#ifdef KMB
    xLinkStatus = Unite_CheckDeviceEvent (uniteCtx);
    SHIM_CHK_ERROR (xLinkStatus, "XLink Device Down", COMM_STATUS_FAILED);
#endif

    xLinkStatus = XLink_Peek (uniteCtx->xLinkCtx, size, payload);
    SHIM_CHK_ERROR (xLinkStatus, "Failed to peek data", COMM_STATUS_FAILED);

    return COMM_STATUS_SUCCESS;
}

CommStatus Unite_Disconnect (HDDLShimUniteContext *uniteCtx, int flag)
{
    SHIM_CHK_NULL (uniteCtx, "NULL Unite context", COMM_STATUS_FAILED);

    XLinkStatus xLinkStatus = X_LINK_ERROR;

#if defined (HDDL_UNITE) && defined (IA)
    HddlStatusCode hddlStatus;
    ChannelID channelIdList[MAX_XLINK_CHANNELS];
    uint64_t workloadId;

    channelIdList[0] = uniteCtx->xLinkCtx->xLinkChannelTX;
    channelIdList[1] = uniteCtx->xLinkCtx->xLinkChannelRX;

    workloadId = uniteCtx->workloadId != WORKLOAD_ID_NONE ? uniteCtx->workloadId
        : uniteCtx->internalWorkloadId;

    for (int i = 0; i < MAX_XLINK_CHANNELS; i++)
    {
        hddlStatus = releaseVAChannelId (workloadId, channelIdList[i]);
        if (hddlStatus != HDDL_OK)
        {
            SHIM_NORMAL_MESSAGE ("Failed to release channel %d for workload %ld",
                channelIdList[i], uniteCtx->workloadId);
            return COMM_STATUS_FAILED;
        }
    }

    if (uniteCtx->internalWorkloadId != WORKLOAD_ID_NONE)
    {
        destroyWorkloadContext (uniteCtx->internalWorkloadId);
        SHIM_NORMAL_MESSAGE ("Internal workload %ld destroyed", uniteCtx->internalWorkloadId);
        uniteCtx->internalWorkloadId = WORKLOAD_ID_NONE;
    }
#endif

#ifdef KMB
    //Unregister the client from receiving event
    //TODO: Temperary remove unregister where the first unregister call will unregister all the
    //event on other thread as well.
    //HSD filed: https://hsdes.intel.com/resource/1508442210
    //
    //if (registerUniteEventCallback == true)
    //{
    //    xLinkStatus = xlink_unregister_device_event (&uniteCtx->xLinkCtx->xLinkHandler,
    //    uniteEventList, MAX_XLINK_EVENT);
    //    SHIM_CHK_ERROR (xLinkStatus, "Unite: Failed to unregister device", COMM_STATUS_FAILED);
    //}

#endif
    xLinkStatus = XLink_Disconnect (uniteCtx->xLinkCtx, flag);
    SHIM_CHK_ERROR (xLinkStatus, "Unite: Failed to disconnect", COMM_STATUS_FAILED);

    if (flag == TARGET)
    {
	    HDDLMemoryMgr_FreeMemory (uniteCtx->xLinkCtx);
    }

    HDDLMemoryMgr_FreeMemory (uniteCtx);

    return COMM_STATUS_SUCCESS;
}
