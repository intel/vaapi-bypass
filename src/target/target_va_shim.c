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
//! \file    target_va_shim.c
//! \brief   Main program execution for KMB Target
//! \details Include basic functions required to retrieve/send payload & call corresponding
//!          VA function.
//!

#include "target_va_shim.h"
#include <sys/syscall.h>

pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gCond = PTHREAD_COND_INITIALIZER;

#ifdef HDDL_UNITE
void HDDLShim_NewWorkloadAvailable (uint64_t workloadId, ChannelID* channelId,
    uint32_t channelNum, uint32_t swDeviceID)
{
    pthread_attr_t threadAttrib;
    pthread_t newThread;
    ShimThreadParams *shimThreadParams = HDDLMemoryMgr_AllocAndZeroMemory (
        sizeof (ShimThreadParams));
    SHIM_CHK_NULL (shimThreadParams, "shimThreadParams returned NULL", );

    shimThreadParams->rx = channelId[0];
    shimThreadParams->tx = channelId[1];
    shimThreadParams->workloadId = workloadId;
    shimThreadParams->swDeviceId = swDeviceID;
    shimThreadParams->commMode = COMM_MODE_UNITE;

    SHIM_NORMAL_MESSAGE ("Obtain HDDLUnite Workload ID: %lu XLINK channel %d and %d",
        workloadId, channelId[0], channelId[1]);

    pthread_cond_signal (&gCond);

    pthread_attr_init (&threadAttrib);
    pthread_attr_setdetachstate (&threadAttrib, PTHREAD_CREATE_DETACHED);
    pthread_create (&newThread, &threadAttrib, HDDLShim_StartNewThread,
        (void *)shimThreadParams);
    pthread_attr_destroy (&threadAttrib);
}
#endif

void *HDDLShim_StartNewThread (void *params)
{
    ShimThreadParams threadParams = *(ShimThreadParams *)params;
    CommStatus commStatus;
    HDDLShimCommContext *ctx;

    ctx = (HDDLShimCommContext *)HDDLMemoryMgr_AllocAndZeroMemory (
        sizeof (HDDLShimCommContext));
    if (ctx == NULL)
    {
        SHIM_ERROR_MESSAGE ("Failed to allocated comm context");
        pthread_exit (NULL);
    }

    commStatus = Comm_ContextInit (&ctx, &threadParams);
    if (commStatus != COMM_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Error to initialize communication context");
        HDDLMemoryMgr_FreeMemory (ctx);
        pthread_exit (NULL);
    }

    commStatus = Comm_Initialize (ctx, TARGET);
    if (commStatus != COMM_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Error to initialize communication settings");
        HDDLMemoryMgr_FreeMemory (ctx);
        pthread_exit (NULL);
    }

    commStatus = Comm_Connect (ctx, TARGET);
    if (commStatus != COMM_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Error to Connect");
        Comm_CloseSocket (ctx, TARGET);
        HDDLMemoryMgr_FreeMemory (ctx);
        pthread_exit (NULL);
    }

    ctx->vaDpy = threadParams.vaDpy;
    ctx->vaDrmFd = threadParams.vaDrmFd;
    ctx->profile = threadParams.profile;

    MainReceiverListener (ctx);

    commStatus = Comm_Disconnect (ctx, TARGET);
    if (commStatus != COMM_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Error to Disconnect");
        HDDLMemoryMgr_FreeMemory (ctx);
        pthread_exit (NULL);
    }

    HDDLMemoryMgr_FreeMemory (params);
    HDDLMemoryMgr_FreeMemory (ctx);
    pthread_exit (NULL);
}

int main (int argc, char *argv[])
{
    CommMode commMode = COMM_MODE_UNKNOWN;
    HDDLShimStatus shimStatus = HDDL_SHIM_STATUS_SUCCESS;

    if (argc > 1)
    {
        Comm_ProcessCommMode (&commMode, argv[1]);
    }
    else
    {
        Comm_GetCommMode (&commMode);
    }
    SHIM_CHK_EQUAL (commMode, COMM_MODE_UNKNOWN, "Unable to set communication mode",
        HDDL_SHIM_STATUS_FAILED);

    shimStatus = HDDLShim_StartVAAPIShimWithMode (commMode);

    return shimStatus;
}

HDDLShimStatus HDDLShim_StartVAAPIShimWithMode (CommMode commMode)
{

#ifdef HDDL_UNITE
    if (commMode == COMM_MODE_UNITE)
    {
        registerVAAPIShimCallback (HDDLShim_NewWorkloadAvailable);
    }
#endif

    while (1)
    {
        if (commMode == COMM_MODE_UNITE)
        {
            HDDLThreadMgr_LockMutex (&gMutex);
            HDDLThreadMgr_CondWaitThread (&gCond, &gMutex);
            HDDLThreadMgr_UnlockMutex (&gMutex);
        }
        else if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_TCP)
        {
            pthread_attr_t threadAttrib;
            pthread_t newThread;
            ShimThreadParams *shimThreadParams = HDDLMemoryMgr_AllocAndZeroMemory (
                sizeof (ShimThreadParams));

	    if (shimThreadParams != NULL)
            {
                Comm_GetNewCommChannel (commMode, &shimThreadParams->tx, &shimThreadParams->rx);

                shimThreadParams->commMode = commMode;

                pthread_attr_init (&threadAttrib);
                pthread_attr_setdetachstate (&threadAttrib, PTHREAD_CREATE_DETACHED);
                pthread_create (&newThread, &threadAttrib, HDDLShim_StartNewThread,
                    (void *)shimThreadParams);
	        pthread_attr_destroy (&threadAttrib);
	    }
        }
	else
	{
            break;
	}
    }

    return 0;
}

void MainReceiverListener (HDDLShimCommContext *ctx)
{
    void *payload = NULL;
    void *vaDataRX = NULL;
    HDDLVAData *vaData = NULL;
    HDDLVAFunctionID vaFunctionID = 0;
    CommStatus commStatus;
    bool terminate = false;
    uint32_t size = 0;
    uint32_t peekRetryCount = 0;
    uint32_t writeRetryCount = 0;

    SHIM_PROFILE_INIT ();

    while (!terminate)
    {
        // TODO: We still need to further generalizing Comm_* functions for TCP
        // and XLINK here after revisiting peek and read functions implementation.
        if (IS_TCP_MODE (ctx))
        {
            // Read payload received
            size = sizeof (HDDLVAData);
            vaData = HDDLMemoryMgr_AllocMemory (size);
            commStatus = Comm_Peek (ctx, &size, vaData);

            if (commStatus == COMM_STATUS_CONNECTION_CLOSED)
            {
                // Operations on host side might being killed without properly
                // shutting down the connection. Reconnecting and waiting for new
                // connection if the current connection has been shutdown.
                commStatus = Comm_Reconnect (ctx);
                SHIM_CHK_EQUAL (commStatus, COMM_STATUS_FAILED, "error connect", );
                continue;
            }
            SHIM_CHK_EQUAL (commStatus, COMM_STATUS_FAILED, "error peek socket", );
            SHIM_CHK_NULL (vaData, "vaData returned NULL", );

            size = vaData->size;
            payload = HDDLMemoryMgr_AllocMemory (size);
	    SHIM_CHK_NULL (payload, "payload returned NULL", );

            commStatus = Comm_Read (ctx, size, payload);
            SHIM_CHK_EQUAL (commStatus, COMM_STATUS_FAILED, "error read socket", );

            vaFunctionID = vaData->vaFunctionID;
            SHIM_CHK_LESS (vaFunctionID, HDDLVAMaxFunctionID, "out of boundary", );
        }
        else if (IS_XLINK_MODE (ctx) || IS_UNITE_MODE (ctx))
        {
            // Read payload receive
            commStatus = Comm_Peek (ctx, &size, &payload);

	    if (commStatus == COMM_STATUS_FAILED)
	    {
                peekRetryCount++;
                SHIM_ERROR_MESSAGE ("Error peek pcie device for %u time(s)", peekRetryCount);
                HDDLMemoryMgr_FreeMemory (payload);

                if (peekRetryCount == MAX_ERROR_RETRY)
                {
                    SHIM_ERROR_MESSAGE ("Failed to peek pcie device for %u consecutive tries. "
                        "Exiting thread", peekRetryCount);
                    terminate = true;
                }

                continue;
            }
            else
            {
                // Reset retry count upon each success operation
                peekRetryCount = 0;
            }

            SHIM_CHK_NULL (payload, "payload returned NULL", );

            vaFunctionID = ( ( (HDDLVAData *)payload)->vaFunctionID);

	    if (vaFunctionID >= HDDLVAMaxFunctionID)
            {
                SHIM_ERROR_MESSAGE ("out of boundary");
                HDDLMemoryMgr_FreeMemory (payload);
                continue;
            }

            size = ( ( (HDDLVAData *)payload)->size);

            if (size > DATA_MAX_SEND_SIZE)
            {
                payload = HDDLMemoryMgr_ReallocMemory (payload, size);
		SHIM_CHK_NULL (payload, "nullptr payload", );
                commStatus = Comm_Read (ctx, size - DATA_MAX_SEND_SIZE,
                    (void *) (payload + DATA_MAX_SEND_SIZE));
            }
        }
        else
        {
            return;
        }

        if (vaFunctionID == HDDLDynamicChannelID)
        {
            pthread_attr_t threadAttrib;
            pthread_t newThread;
            ShimThreadParams *shimThreadParams = HDDLMemoryMgr_AllocAndZeroMemory (
                sizeof (ShimThreadParams));

	    if (shimThreadParams == NULL)
	    {
	        SHIM_ERROR_MESSAGE ("shimThreadParams returned NULL");
		HDDLMemoryMgr_FreeMemory (payload);
		continue;
	    }

            shimThreadParams->commMode = COMM_MODE (ctx);
            shimThreadParams->tx = ( (HDDLDynamicChannelTX *)payload)->channelTX;
            shimThreadParams->rx = ( (HDDLDynamicChannelTX *)payload)->channelRX;
            shimThreadParams->vaDpy = ctx->vaDpy;
            shimThreadParams->vaDrmFd = ctx->vaDrmFd;
            shimThreadParams->profile = ctx->profile;

            pthread_attr_init (&threadAttrib);
            pthread_attr_setdetachstate (&threadAttrib, PTHREAD_CREATE_DETACHED);
            pthread_create (&newThread, &threadAttrib, HDDLShim_StartNewThread,
                (void *)shimThreadParams);
            pthread_attr_destroy (&threadAttrib);

	    // TODO: Need to implement method to terminate all these new threads
	    // when the parent thread is being terminated through vaTerminate call
        }

        if ( (vaFunctionID == HDDLVAMedia_DriverInit) && (ctx->vaDpy == NULL))
        {
            va_open_display (&ctx->vaDpy, (int *)&ctx->vaDrmFd);
        }

        // Call corresponding function to handle VAFunctionID
        vaDataRX = HDDLShim_MainPayloadExtraction (vaFunctionID, ctx, payload, size);

	if (vaDataRX == NULL)
	{
            SHIM_ERROR_MESSAGE ("vaDataRX returned NULL");
            HDDLMemoryMgr_FreeMemory (payload);
            continue;
	}

        // Write back processed result
        commStatus = Comm_Write (ctx, ( (HDDLVAData *)vaDataRX)->size, vaDataRX);
        if (commStatus != COMM_STATUS_SUCCESS)
        {
            writeRetryCount++;
            SHIM_ERROR_MESSAGE ("Error write pcie device for %d time(s)", writeRetryCount);
            HDDLMemoryMgr_FreeMemory (vaDataRX);
            HDDLMemoryMgr_FreeMemory (payload);

            if (writeRetryCount == MAX_ERROR_RETRY)
            {
                SHIM_ERROR_MESSAGE ("Failed to write pcie device for %u consecutive tries. "
                    "Exiting thread", writeRetryCount);
                terminate = true;
            }

            continue;
        }
        else
        {
            // Reset retry count upon each success operation
            writeRetryCount = 0;
        }

        if (IS_TCP_MODE (ctx))
        {
            if (vaData)
            {
                HDDLMemoryMgr_FreeMemory (vaData);
            }
        }

        if (payload)
        {
            HDDLMemoryMgr_FreeMemory (payload);
        }

        if (vaDataRX)
        {
            HDDLMemoryMgr_FreeMemory (vaDataRX);
        }

        if (vaFunctionID == HDDLVATerminate)
        {
            va_close_display (ctx->vaDpy, ctx->vaDrmFd);
            ctx->vaDpy = NULL;
            ctx->vaDrmFd = -1;

            SHIM_PROFILE_TERMINATE ();
            SHIM_PROFILE_INIT ();

            // TODO: Currently only UNITE mode will terminate the thread for each vaTerminate
            // call since we will receive new XLink channels pairs for each vaInitialize call.
            // For XLINK and TCP mode, there's no explicit service similar as HDDLUnite to
            // provide the new channles/ports pair. We need to figure out a better way to handle
            // this instead of reconnecting and never exit.
            commStatus = Comm_Reconnect (ctx);
            SHIM_CHK_EQUAL (commStatus, COMM_STATUS_FAILED, "error reconnecting", );

            if (IS_UNITE_MODE (ctx))
            {
                terminate = true;
            }
        }
    }
    SHIM_PROFILE_TERMINATE ();
}
