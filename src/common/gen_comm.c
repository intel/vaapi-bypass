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
//! \file    gen_comm.c
//! \brief   General interface for different communication
//! \details Provide connecting option for communcation depend on its API.
//!          Available communcation method, such as XLINK, TCP
//!

#include "gen_comm.h"
#define CONFIG_PATH_MAX_STRLEN 300
#define RX_TX_MAX_STRLEN 20

#ifdef SVE_HOOK
#define SVE_INPUT_SLEEP_INTERVAL 1000000
#endif  // ifdef SVE_HOOK

int indicator = -1;

CommStatus Comm_ContextInitFromConfig (HDDLShimCommContext **ctx)
{
    FILE *file;
    char inputMode[8];
    char line[256];
    char firstInput[128], lastInput[128];
    char *mode = NULL;
    char *configParam = NULL;
    char *env = NULL;
    char *path = NULL;
    struct stat tmp;
    CommStatus commStatus = COMM_STATUS_SUCCESS;

    // Read the connection.cfg file to setup the tcp/xlink connection to the server
    if ( (env = getenv ("CONFIG_PATH")) != NULL)
    {
        path = strndup (env, strnlen_s (env, CONFIG_PATH_MAX_STRLEN));

        // Read the connection.cfg file to setup the tcp connection to the server
        if ( (path != NULL) && (file = fopen (path, "r")) != NULL)
        {
            fstat (fileno (file), &tmp);
	    if (tmp.st_uid != getuid ())
	    {
	        SHIM_ERROR_MESSAGE ("uid not match. Cannot access the file");
		fclose (file);
		return COMM_STATUS_FAILED;
	    }

            if (fscanf (file, "%*s %5s", inputMode))
            {
                mode = strndup (inputMode, 5);
            }
        }
        else
        {
            SHIM_ERROR_MESSAGE ("Communication config file not found");
            return COMM_STATUS_FAILED;
        }
    }
    else
    {
        SHIM_ERROR_MESSAGE ("CONFIG_PATH ENV not set");
        return COMM_STATUS_FAILED;
    }

    if (mode == NULL)
    {
        SHIM_ERROR_MESSAGE ("Comm Mode unspecified");
        fclose (file);
        return COMM_STATUS_FAILED;
    }
    if ( (strcmp_s (mode, 5, "XLINK", &indicator) == 0) && (indicator == 0))
    {
        uint16_t channelTX = 0;
        uint16_t channelRX = 0;

        while (fgets (line, sizeof (line), file) != NULL)
        {
            sscanf (line, "%15s %128s", firstInput, lastInput);
            configParam = strndup (firstInput, 15);

	    if (configParam != NULL)
	    {
                if ( (strcmp_s (configParam, 9, "CHANNELTX", &indicator) == 0) && (indicator == 0))
                {
                    channelTX = (int)strtol (lastInput, (char **)NULL, 16);
                }
                else if ( (strcmp_s (configParam, 9, "CHANNELRX", &indicator) == 0) &&
		    (indicator == 0))
                {
                    channelRX = (int)strtol (lastInput, (char **)NULL, 16);
                }

                free (configParam);
	    }
        }

        if (channelTX == 0 || channelRX == 0)
        {
            SHIM_ERROR_MESSAGE ("XLINK CHANNELTX / CHANNELRX not specified");
	    fclose (file);
            return COMM_STATUS_FAILED;
        }

        (*ctx)->commMode = COMM_MODE_XLINK;
        (*ctx)->xLinkCtx = XLink_ContextInit (channelTX, channelRX);

        if ( (*ctx)->xLinkCtx == NULL)
        {
            SHIM_ERROR_MESSAGE ("%s: XLinkCtx NULL value", __func__);
            fclose (file);
            return COMM_STATUS_FAILED;
        }
    }
    else if ( (strcmp_s (mode, 3, "TCP", &indicator) == 0) && (indicator == 0))
    {
	//TODO: To uncomment out once able to release TCP support
        /*uint16_t portTX = 0;
        uint16_t portRX = 0;
        struct hostent *server = NULL;

        while (fgets (line, sizeof (line), file) != NULL)
        {
            memset_s (lastInput, sizeof (lastInput), 0);
            sscanf (line, "%8s %128s", firstInput, lastInput);
            configParam = strndup (firstInput, 15);

            if (configParam != NULL)
            {
                if ( (strcmp_s (configParam, 6, "PORTTX", &indicator) == 0) && (indicator == 0))
                {
                    portTX = (uint16_t)atoi (lastInput);
                }
                else if ( (strcmp_s (configParam, 6, "PORTRX", &indicator) == 0) &&
		    (indicator == 0))
                {
                    portRX = (uint16_t)atoi (lastInput);
                }
                else if ( (strcmp_s (configParam, 2, "IP", &indicator) == 0) && (indicator == 0))
                {
	            if (strlen (lastInput) != 0)
		    {
		        server = gethostbyname (lastInput);
		    }
		    else
		    {
		        SHIM_ERROR_MESSAGE ("IP input returned NULL");
			fclose (file);
			return COMM_STATUS_FAILED;
		    }
                }

                free (configParam);
            }
        }

        if (portTX == 0 || portRX == 0 || server == NULL)
        {
            SHIM_ERROR_MESSAGE ("TCP PORT and SERVERIP not specified");
	    fclose (file);
            return COMM_STATUS_FAILED;
        }

        (*ctx)->commMode = COMM_MODE_TCP;
        (*ctx)->tcpCtx = TCP_ContextInit (portRX, portTX, server);

	if ( (*ctx)->tcpCtx == NULL)
        {
            SHIM_ERROR_MESSAGE ("%s: tcpCtx NULL value", __func__);
            fclose (file);
            return COMM_STATUS_FAILED;
        }*/
        SHIM_ERROR_MESSAGE ("Unsupported Communication mode");
	return COMM_STATUS_FAILED;
    }
    else if ( (strcmp_s (mode, 5, "UNITE", &indicator) == 0) && (indicator == 0))
    {
        (*ctx)->commMode = COMM_MODE_UNITE;
        (*ctx)->uniteCtx = Unite_ContextInit (QUERY_FROM_UNITE, QUERY_FROM_UNITE,
            QUERY_FROM_UNITE);

        if ( (*ctx)->uniteCtx == NULL)
	{
            SHIM_ERROR_MESSAGE ("%s: XLinkCtx NULL value", __func__);
            fclose (file);
            return COMM_STATUS_FAILED;
        }
    }
    else
    {
        SHIM_ERROR_MESSAGE ("Unsupported Communication mode");
    }

    free (mode);
    free (path);
    fclose (file);

    return commStatus;
}

CommStatus Comm_ContextInit (HDDLShimCommContext **ctx, ShimThreadParams *threadParams)
{
    CommStatus commStatus = COMM_STATUS_SUCCESS;
    (*ctx)->commMode = threadParams->commMode;

    if ( (*ctx)->commMode == COMM_MODE_XLINK)
    {
        (*ctx)->xLinkCtx = XLink_ContextInit (threadParams->tx, threadParams->rx);
        SHIM_CHK_NULL ( (*ctx)->xLinkCtx, "", COMM_STATUS_FAILED);
    }
    else if ( (*ctx)->commMode == COMM_MODE_TCP)
    {
        (*ctx)->tcpCtx = TCP_ContextInit (threadParams->tx, threadParams->rx, NULL);
        SHIM_CHK_NULL ( (*ctx)->tcpCtx, "", COMM_STATUS_FAILED);
    }
    else if ( (*ctx)->commMode == COMM_MODE_UNITE)
    {
        (*ctx)->uniteCtx = Unite_ContextInit (threadParams->tx, threadParams->rx,
            threadParams->workloadId);
    }

    return commStatus;
}

CommStatus Comm_DynamicContextInit (HDDLShimCommContext **ctx, HDDLShimCommContext *mainCtx,
    uint16_t *lastChannel, uint16_t *tx, uint16_t *rx)
{
    CommStatus commStatus = COMM_STATUS_SUCCESS;
    (*ctx)->commMode = mainCtx->commMode;
    *tx = ++(*lastChannel);
    *rx = ++(*lastChannel);

    if ( (*ctx)->commMode == COMM_MODE_XLINK)
    {
        (*ctx)->xLinkCtx = XLink_ContextInit (*tx, *rx);
    }
    else if ( (*ctx)->commMode == COMM_MODE_TCP)
    {
        // TOOD: fill in later
        // (*ctx)->tcpCtx = TCP_ContextInit (*tx, *rx,
        //     (char *)&mainCtx->tcpCtx->addressTX.sin_addr.s_addr,
        //    strlen ((char *)&mainCtx->tcpCtx->addressTX.sin_addr.s_addr));
    }
    else if ( (*ctx)->commMode == COMM_MODE_UNITE)
    {
        // TODO: fill in later
    }

    return commStatus;
}

CommStatus Comm_Initialize (HDDLShimCommContext *ctx, int flag)
{
    CommStatus commStatus = COMM_STATUS_UNKNOWN;

    if (IS_XLINK_MODE (ctx))
    {
        XLinkStatus xlinkStatus = XLink_Initialize (ctx->xLinkCtx, flag);

        if (xlinkStatus == X_LINK_SUCCESS)
            commStatus = COMM_STATUS_SUCCESS;
        else
            commStatus = COMM_STATUS_FAILED;
    }
    else if (IS_TCP_MODE (ctx))
    {
        TCPStatus tcpStatus = TCP_Initialize (ctx->tcpCtx, flag);

        if (tcpStatus == TCP_SUCCESS)
            commStatus = COMM_STATUS_SUCCESS;
        else
            commStatus = COMM_STATUS_FAILED;
    }
    else if (IS_UNITE_MODE (ctx))
    {
        commStatus = Unite_Initialize (ctx->uniteCtx, flag);
    }

    return commStatus;
}

CommStatus Comm_Connect (HDDLShimCommContext *ctx, int flag)
{
    CommStatus commStatus = COMM_STATUS_UNKNOWN;

    if (IS_XLINK_MODE (ctx))
    {
        XLinkStatus xlinkStatus = XLink_Connect (ctx->xLinkCtx);

        if (xlinkStatus == X_LINK_SUCCESS)
            commStatus = COMM_STATUS_SUCCESS;
        else
            commStatus = COMM_STATUS_FAILED;
    }
    else if (IS_TCP_MODE (ctx))
    {
        if (flag == TARGET)
        {
            TCPStatus tcpStatus = TCP_Connect (ctx->tcpCtx);

            if (tcpStatus == TCP_SUCCESS)
                commStatus = COMM_STATUS_SUCCESS;
            else
                commStatus = COMM_STATUS_FAILED;
        }
        else
        {
            commStatus = COMM_STATUS_SUCCESS;
        }
    }
    else if (IS_UNITE_MODE (ctx))
    {
        commStatus = Unite_Connect (ctx->uniteCtx);
    }

    return commStatus;
}

CommStatus Comm_Write (HDDLShimCommContext *ctx, int size, void *payload)
{
    SHIM_PROFILE_START ();

    CommStatus commStatus = COMM_STATUS_UNKNOWN;

    if (IS_XLINK_MODE (ctx))
    {
        XLinkStatus xlinkStatus = XLink_Write (ctx->xLinkCtx, size, payload);

        if (xlinkStatus == X_LINK_SUCCESS)
            commStatus = COMM_STATUS_SUCCESS;
        else
            commStatus = COMM_STATUS_FAILED;
    }
    else if (IS_TCP_MODE (ctx))
    {
        TCPStatus tcpStatus = TCP_Write (ctx->tcpCtx, size, payload);

        if (tcpStatus == TCP_SUCCESS)
            commStatus = COMM_STATUS_SUCCESS;
        else
            commStatus = COMM_STATUS_FAILED;
    }
    else if (IS_UNITE_MODE (ctx))
    {
        commStatus = Unite_Write (ctx->uniteCtx, size, payload);
    }

    SHIM_NORMAL_MESSAGE ("write size: %d", size);

    SHIM_PROFILE_END ();

    return commStatus;
}

CommStatus Comm_Read (HDDLShimCommContext *ctx, int size, void *payload)
{
    SHIM_PROFILE_START ();

    CommStatus commStatus = COMM_STATUS_UNKNOWN;

    if (IS_XLINK_MODE (ctx))
    {
        XLinkStatus xlinkStatus = XLink_Read (ctx->xLinkCtx, size, payload);

        if (xlinkStatus == X_LINK_SUCCESS)
            commStatus = COMM_STATUS_SUCCESS;
        else
            commStatus = COMM_STATUS_FAILED;
    }
    else if (IS_TCP_MODE (ctx))
    {
        TCPStatus tcpStatus = TCP_Read (ctx->tcpCtx, size, payload);

        if (tcpStatus == TCP_SUCCESS)
            commStatus = COMM_STATUS_SUCCESS;
        else if (tcpStatus == TCP_EOF)
            commStatus = COMM_STATUS_EOF;
        else
            commStatus = COMM_STATUS_FAILED;
    }
    else if (IS_UNITE_MODE (ctx))
    {
        commStatus = Unite_Read (ctx->uniteCtx, size, payload);
    }

    SHIM_NORMAL_MESSAGE ("read size: %d", size);

    SHIM_PROFILE_END ();

    return commStatus;
}

CommStatus Comm_ReadSafe (HDDLShimCommContext *ctx, int size, void *payload)
{
    SHIM_PROFILE_START ();

    CommStatus commStatus = COMM_STATUS_UNKNOWN;

    if (IS_XLINK_MODE (ctx))
    {
        HDDLThreadMgr_LockMutex (&ctx->xLinkCtx->xLinkMutex);

        XLinkStatus xlinkStatus = XLink_Read (ctx->xLinkCtx, size, payload);

	HDDLThreadMgr_UnlockMutex (&ctx->xLinkCtx->xLinkMutex);

        if (xlinkStatus == X_LINK_SUCCESS)
            commStatus = COMM_STATUS_SUCCESS;
        else
            commStatus = COMM_STATUS_FAILED;
    }
    else if (IS_UNITE_MODE (ctx))
    {
	HDDLThreadMgr_LockMutex (&ctx->uniteCtx->xLinkCtx->xLinkMutex);

        commStatus = Unite_Read (ctx->uniteCtx, size, payload);

	HDDLThreadMgr_UnlockMutex (&ctx->uniteCtx->xLinkCtx->xLinkMutex);
    }

    SHIM_NORMAL_MESSAGE ("read size: %d", size);

    SHIM_PROFILE_END ();

    return commStatus;
}

CommStatus Comm_Peek (HDDLShimCommContext *ctx, uint32_t *size, void *payload)
{
    SHIM_PROFILE_START ();

    CommStatus commStatus = COMM_STATUS_UNKNOWN;

    if (IS_XLINK_MODE (ctx))
    {
        XLinkStatus xlinkStatus = XLink_Peek (ctx->xLinkCtx, size, payload);

        if (xlinkStatus == X_LINK_SUCCESS)
            commStatus = COMM_STATUS_SUCCESS;
        else
            commStatus = COMM_STATUS_FAILED;
    }
    else if (IS_TCP_MODE (ctx))
    {
        TCPStatus tcpStatus = TCP_Peek (ctx->tcpCtx, size, payload);

        if (tcpStatus == TCP_SUCCESS)
            commStatus = COMM_STATUS_SUCCESS;
        else if (tcpStatus == TCP_CONNECTION_CLOSED)
            commStatus = COMM_STATUS_CONNECTION_CLOSED;
        else
            commStatus = COMM_STATUS_FAILED;
    }
    else if (IS_UNITE_MODE (ctx))
    {
        commStatus = Unite_Peek (ctx->uniteCtx, size, payload);
    }

    SHIM_NORMAL_MESSAGE ("read size: %d", *size);

    SHIM_PROFILE_END ();

    return commStatus;
}

CommStatus Comm_Submission (HDDLShimCommContext *ctx, HDDLVAFunctionID functionId,
    CommReadOp readOp, int inSize, void *inPayload, int outSize, void **outPayload)
{
    SHIM_PROFILE_START ();

    CommStatus commStatus = COMM_STATUS_UNKNOWN;

    if (IS_BATCH (ctx))
    {
        commStatus = Comm_BatchSubmission (ctx, functionId, readOp, inSize, inPayload, outSize,
            outPayload);
    }
    else
    {
        commStatus = Comm_SingleSubmission (ctx, readOp, inSize, inPayload, outSize, outPayload);
    }

    SHIM_PROFILE_END ();

    return commStatus;
}

CommStatus Comm_BatchFlush (HDDLShimCommContext *ctx, int outSize, void **outPayload,
    HDDLShimBatchPayloadOp payloadOp)
{
    CommStatus commStatus = COMM_STATUS_UNKNOWN;
    HDDLShimBatchPayload *batchPayload = ctx->batchPayload;
    HDDLVAData batchHeader;

    // Append batch header to the begining of the payload data which reserved earlier during
    // Comm_BatchInit function
    batchHeader.vaFunctionID = HDDLTransferBatch;
    batchHeader.size = batchPayload->offset;

    memcpy_s (batchPayload->data, sizeof (HDDLVAData), &batchHeader, sizeof (HDDLVAData));

    batchPayload->batchState = BATCH_OFF;

    commStatus = Comm_SingleSubmission (ctx, COMM_READ_FULL, batchPayload->offset,
        batchPayload->data, outSize, outPayload);

    if (payloadOp == PAYLOAD_FREE)
    {
        HDDLMemoryMgr_FreeMemory (batchPayload);
        ctx->batchPayload = NULL;
    }
    else if (payloadOp == PAYLOAD_RESET)
    {
        memset_s (batchPayload, sizeof (HDDLShimBatchPayload), 0);
        // Reserve the header for batch function and size identification before flush
        batchPayload->offset += sizeof (HDDLVAData);
    }

    return commStatus;
}

CommStatus  Comm_BatchAppend (HDDLShimCommContext* ctx, HDDLShimBatchPayload *batchPayload, 
    int inSize, void *inPayload, int outSize, void** outPayload)
{
    CommStatus commStatus = COMM_STATUS_SUCCESS;
    if (batchPayload->offset + inSize >= DATA_MAX_SEND_SIZE)
    {
        HDDLVAData batchHeader;

        // Append batch header to the begining of the payload data which reserved earlier during
        // Comm_BatchInit function
        batchHeader.vaFunctionID = HDDLTransferBatch;
        batchHeader.size = batchPayload->offset;

        memcpy_s (batchPayload->data, sizeof(HDDLVAData), &batchHeader, sizeof(HDDLVAData));

        HDDLVAData save = *(HDDLVAData*)(outPayload);
        commStatus = Comm_SingleSubmission (ctx, COMM_READ_FULL, batchPayload->offset,
            batchPayload->data, outSize, outPayload);

        // restore functionId & size
        *(HDDLVAData*)(outPayload) = save;

        memset (batchPayload, 0, sizeof(HDDLShimBatchPayload));
        // Reserve the header for batch function and size identification before flush
        batchPayload->offset += sizeof(HDDLVAData);
    }

    if (batchPayload->offset + inSize >= DATA_MAX_SEND_SIZE)
    {
        return Comm_SingleSubmission (ctx, COMM_READ_FULL, inSize,
            inPayload, outSize, outPayload);
    }

    memcpy_s ( (void *) (batchPayload->data + batchPayload->offset), inSize, inPayload, inSize);
    batchPayload->offset += inSize;
    return commStatus;
}

HDDLShimBatchPayload *Comm_BatchInit ()
{
    // TODO: Currently the max payload data that can be sent is being set to DATA_MAX_SEND_SIZE.
    // We might need to be able to expand the size when the limit has been reached though very
    // unlikely that batch mode data will exceed DATA_MAX_SEND_SIZE.
    HDDLShimBatchPayload *batchPayload = HDDLMemoryMgr_AllocAndZeroMemory (
        sizeof (HDDLShimBatchPayload));

    // Reserve the header for batch function and size identification before flush
    if (batchPayload != NULL)
    {
        batchPayload->offset += sizeof (HDDLVAData);
    }

    return batchPayload;
}

// When we are batching the VAAPI call and sent over to accelerator, we should only
// expect COMM_READ_FULL operations. The only exception is when batch mode is turn on,
// but the current VAAPI call operation cannot be batched (e.g. not within per frame
// batching, or not within vaDestroyBuffer batching).
CommStatus Comm_BatchSubmission (HDDLShimCommContext *ctx, HDDLVAFunctionID functionId,
    CommReadOp readOp, int inSize, void *inPayload, int outSize, void **outPayload)
{
    CommStatus commStatus = COMM_STATUS_SUCCESS;
    HDDLShimBatchPayload *batchPayload = ctx->batchPayload;

    if (readOp == COMM_READ_FULL)
    {
        memset_s (outPayload, outSize, 0);
        ( (HDDLVAData *)outPayload)->vaFunctionID = functionId;
        ( (HDDLVAData *)outPayload)->size = outSize;
    }

    // Begining of per frame batching
    if (functionId == BATCH_FRAME_START_FUNC)
    {
        if (batchPayload)
        {
            // Flush the batched vaDestroyBuffer calls to accelerator before we start performing
            // per frame batching
            if (batchPayload->batchState == BATCH_DESTROY_BUFFER)
            {
                commStatus = Comm_BatchFlush (ctx, outSize, outPayload, PAYLOAD_RESET);
                SHIM_CHK_ERROR(commStatus, "Error to BatchFlush", commStatus);

                // restore functionId & size
                ((HDDLVAData*)outPayload)->vaFunctionID = functionId;
                ((HDDLVAData*)outPayload)->size = outSize;
            }
        }
        else
        {
            batchPayload = Comm_BatchInit ();

	    SHIM_CHK_NULL (batchPayload, "batchPayload returned NULL", COMM_STATUS_FAILED);
        }

        commStatus = Comm_BatchAppend (ctx, batchPayload, inSize, inPayload, outSize, outPayload);

        batchPayload->batchState = BATCH_PER_FRAME;
        ctx->batchThreadId = syscall (SYS_gettid);
        ctx->batchPayload = batchPayload;

    }
    else
    {
        if (batchPayload && ctx->batchThreadId == syscall (SYS_gettid))
        {
            if (batchPayload->batchState == BATCH_PER_FRAME)
            {
                commStatus = Comm_BatchAppend (ctx, batchPayload, inSize, inPayload, outSize, outPayload);
                SHIM_CHK_ERROR(commStatus, "Error to BatchAppend", commStatus);

                if (functionId == BATCH_FRAME_END_FUNC)
                {
                    commStatus = Comm_BatchFlush (ctx, outSize, outPayload, PAYLOAD_FREE);
                }
            }
            else if (batchPayload->batchState == BATCH_DESTROY_BUFFER)
            {
                if (functionId == BATCH_DESTROY_START_FUNC ||
                    functionId == BATCH_DESTROY_END_FUNC || functionId == BATCH_DESTROY_FINAL_FUNC)
                {
                    commStatus = Comm_BatchAppend (ctx, batchPayload, inSize, inPayload, outSize, outPayload);
                    SHIM_CHK_ERROR(commStatus, "Error to BatchAppend", commStatus);

                    // Flush the batched vaDestroyBuffer function calls to accelerator under two
                    // conditions:
                    //  1) before calling vaCreateBuffer to avoid the possibility of messing up the
                    //     buffer creation sequence on accelerator
                    //  2) before calling vaDestroyContext
                    if (functionId == BATCH_DESTROY_END_FUNC ||
                        functionId == BATCH_DESTROY_FINAL_FUNC)
                    {
                        commStatus = Comm_BatchFlush (ctx, outSize, outPayload, PAYLOAD_FREE);
                    }
                }
                else
                {
                    commStatus = Comm_SingleSubmission (ctx, readOp, inSize, inPayload, outSize,
                        outPayload);
                }
            }
            else if (batchPayload->batchState == BATCH_OFF)
            {
                commStatus = Comm_SingleSubmission (ctx, readOp, inSize, inPayload, outSize,
                    outPayload);
            }
        }
        else
        {
            // Begining of vaDestroyBuffer batching
            if (functionId == BATCH_DESTROY_START_FUNC)
            {
                batchPayload = Comm_BatchInit ();

                SHIM_CHK_NULL (batchPayload, "batchPayload returned NULL", COMM_STATUS_FAILED);

                commStatus = Comm_BatchAppend (ctx, batchPayload, inSize, inPayload, outSize, outPayload);

                batchPayload->batchState = BATCH_DESTROY_BUFFER;
                ctx->batchThreadId = syscall (SYS_gettid);
                ctx->batchPayload = batchPayload;
            }
            else
            {
                commStatus = Comm_SingleSubmission (ctx, readOp, inSize, inPayload, outSize,
                    outPayload);
            }
        }
    }

    return commStatus;
}

CommStatus Comm_SingleSubmission (HDDLShimCommContext *ctx, CommReadOp readOp, int inSize,
    void *inPayload, int outSize, void **outPayload)
{
    CommStatus commStatus = COMM_STATUS_SUCCESS;

    if (IS_XLINK_MODE (ctx))
    {
        HDDLThreadMgr_LockMutex (&ctx->xLinkCtx->xLinkMutex);
        XLinkStatus xlinkStatus = XLink_Write (ctx->xLinkCtx, inSize, inPayload);

        if (xlinkStatus != X_LINK_SUCCESS)
        {
            HDDLThreadMgr_UnlockMutex (&ctx->xLinkCtx->xLinkMutex);
            return COMM_STATUS_FAILED;
        }

        if (readOp == COMM_READ_FULL)
        {
            xlinkStatus = XLink_Read (ctx->xLinkCtx, outSize, outPayload);
        }
        else
        {
            xlinkStatus = XLink_Peek (ctx->xLinkCtx, (uint32_t *)&outSize, outPayload);
        }

        if (xlinkStatus != X_LINK_SUCCESS)
        {
            HDDLThreadMgr_UnlockMutex (&ctx->xLinkCtx->xLinkMutex);
            commStatus = COMM_STATUS_FAILED;
        }

        HDDLThreadMgr_UnlockMutex (&ctx->xLinkCtx->xLinkMutex);
    }
    else if (IS_TCP_MODE (ctx))
    {
        TCPStatus tcpStatus = TCP_Write (ctx->tcpCtx, inSize, inPayload);

        if (tcpStatus != TCP_SUCCESS)
            return COMM_STATUS_FAILED;

        if (readOp == COMM_READ_FULL)
        {
            tcpStatus = TCP_Read (ctx->tcpCtx, outSize, outPayload);
        }
        else
        {
            tcpStatus = TCP_Peek (ctx->tcpCtx, (uint32_t *)&outSize, (void *)*outPayload);
        }

	if (tcpStatus != TCP_SUCCESS)
            commStatus = COMM_STATUS_FAILED;
    }
    else if (IS_UNITE_MODE (ctx))
    {
        HDDLThreadMgr_LockMutex (&ctx->uniteCtx->xLinkCtx->xLinkMutex);
        commStatus = Unite_Write (ctx->uniteCtx, inSize, inPayload);

        if (commStatus != COMM_STATUS_SUCCESS)
        {
            HDDLThreadMgr_UnlockMutex (&ctx->uniteCtx->xLinkCtx->xLinkMutex);
	    return commStatus;
        }

        if (readOp == COMM_READ_FULL)
        {
            commStatus = Unite_Read (ctx->uniteCtx, outSize, outPayload);
        }
        else
        {
            commStatus = Unite_Peek (ctx->uniteCtx, (uint32_t *)&outSize, outPayload);
        }

        if (commStatus != COMM_STATUS_SUCCESS)
        {
            HDDLThreadMgr_UnlockMutex (&ctx->uniteCtx->xLinkCtx->xLinkMutex);
            return commStatus;
        }

        HDDLThreadMgr_UnlockMutex (&ctx->uniteCtx->xLinkCtx->xLinkMutex);
    }

    SHIM_NORMAL_MESSAGE ("Message submission write size: %d  read size: %d", inSize, outSize);

    return commStatus;
}

CommStatus Comm_Disconnect (HDDLShimCommContext *ctx, int flag)
{
    CommStatus commStatus = COMM_STATUS_UNKNOWN;

    if (IS_XLINK_MODE (ctx))
    {
        XLinkStatus xlinkStatus = XLink_Disconnect (ctx->xLinkCtx, flag);

        if (xlinkStatus == X_LINK_SUCCESS)
            commStatus = COMM_STATUS_SUCCESS;
        else
            commStatus = COMM_STATUS_FAILED;
    }
    else if (IS_TCP_MODE (ctx))
    {
        TCPStatus tcpStatus = TCP_Disconnect (ctx->tcpCtx, flag);

        if (tcpStatus == TCP_SUCCESS)
            commStatus = COMM_STATUS_SUCCESS;
        else
            commStatus = COMM_STATUS_FAILED;
    }
    else if (IS_UNITE_MODE (ctx))
    {
        commStatus = Unite_Disconnect (ctx->uniteCtx, flag);
    }

    return commStatus;
}

CommStatus Comm_Reconnect (HDDLShimCommContext *ctx)
{
    CommStatus commStatus = COMM_STATUS_UNKNOWN;

    if (IS_TCP_MODE (ctx))
    {
        TCPStatus tcpStatus = TCP_Reconnect (ctx->tcpCtx);

        if (tcpStatus == TCP_SUCCESS)
            commStatus = COMM_STATUS_SUCCESS;
        else
            commStatus = COMM_STATUS_FAILED;
    }
    else if (IS_XLINK_MODE (ctx))
    {
        XLinkStatus xlinkStatus = XLink_Disconnect (ctx->xLinkCtx, TARGET);

        if (xlinkStatus == X_LINK_SUCCESS)
        {
            xlinkStatus = XLink_Connect (ctx->xLinkCtx);
        }

        if (xlinkStatus == X_LINK_SUCCESS)
            commStatus = COMM_STATUS_SUCCESS;
        else
            commStatus = COMM_STATUS_FAILED;
    }
    else
    {
        commStatus = COMM_STATUS_SUCCESS;
    }

    return commStatus;
}

void Comm_ProcessCommMode (CommMode *commMode, char *mode)
{
    if ( (strcmp_s (mode, 5, "unite", &indicator) == 0) && (indicator == 0))
    {
        *commMode = COMM_MODE_UNITE;
    }
    else if ( (strcmp_s (mode, 5, "xlink", &indicator) == 0) && (indicator == 0))
    {
        *commMode = COMM_MODE_XLINK;
    }
    else if ( (strcmp_s (mode, 3, "tcp", &indicator) == 0) && (indicator == 0))
    {
	//TODO: To uncomment out once able to release TCP support
        //*commMode = COMM_MODE_TCP;
	SHIM_ERROR_MESSAGE ("* Invalid communication mode");
    }
    else
    {
        SHIM_NORMAL_MESSAGE ("* Invalid communication mode");
    }

    SHIM_NORMAL_MESSAGE ("* VAAPI Shim started with %s communication mode", mode);
}

void Comm_GetCommMode (CommMode *commMode)
{
    char *mode = NULL;
    char input[6];

    while (*commMode == COMM_MODE_UNKNOWN)
    {
        printf ("Please select communication mode (unite / xlink):\n");

        if (scanf ("%5s", input))
        {
            mode = strndup (input, 5);
        }

	if (mode != NULL)
	{
            if ( (strcmp_s (mode, 5, "xlink", &indicator) == 0) && (indicator == 0))
            {
                *commMode = COMM_MODE_XLINK;
            }
            else if ( (strcmp_s (mode, 3, "tcp", &indicator) == 0) && (indicator == 0))
            {
		//TODO: To uncomment out once able to release TCP support
                //*commMode = COMM_MODE_TCP;
		SHIM_ERROR_MESSAGE ("* Invalid communication mode");
            }
	    else if ( (strcmp_s (mode, 5, "unite", &indicator) == 0) && (indicator == 0))
	    {
	        *commMode = COMM_MODE_UNITE;
	    }
            else
            {
                SHIM_NORMAL_MESSAGE ("* Invalid communication mode");
            }

            SHIM_NORMAL_MESSAGE ("* %s communication mode is being selected", mode);
	}
    }
    free (mode);
}

CommStatus Comm_GetLastChannel (HDDLShimCommContext *ctx, uint16_t *lastChannel)
{
    CommStatus commStatus = COMM_STATUS_UNKNOWN;
    uint16_t tx = 0;
    uint16_t rx = 0;

    if (IS_XLINK_MODE (ctx))
    {
        tx = ctx->xLinkCtx->xLinkChannelTX;
        rx = ctx->xLinkCtx->xLinkChannelRX;
    }
    else if (IS_TCP_MODE (ctx))
    {
        // TODO: fill in later
        // tx = ntohs (ctx->tcpCtx->addressTX.sin_port);
        // rx = ntohs (ctx->tcpCtx->addressRX.sin_port);
    }
    else if (IS_UNITE_MODE (ctx))
    {
        // TODO: fill in later
    }

    *lastChannel = tx > rx ? tx : rx;
    return commStatus;
}

void Comm_MutexDestroy (HDDLShimCommContext *ctx)
{
    if (IS_TCP_MODE (ctx))
    {
        HDDLThreadMgr_DestroyMutex (&ctx->tcpCtx->tcpMutex);
    }
    else if (IS_XLINK_MODE (ctx))
    {
        HDDLThreadMgr_DestroyMutex (&ctx->xLinkCtx->xLinkMutex);
    }
    else if (IS_UNITE_MODE (ctx))
    {
        HDDLThreadMgr_DestroyMutex (&ctx->uniteCtx->xLinkCtx->xLinkMutex);
    }
}
void Comm_CloseSocket (HDDLShimCommContext *ctx, int flag)
{
    if (IS_TCP_MODE (ctx))
    {
	int flagOpt = flag;

	if (flagOpt == HOST)
	{
	    close (ctx->tcpCtx->clientTX);
	    close (ctx->tcpCtx->clientRX);
	}
	else if (flagOpt == TARGET)
	{
	    close (ctx->tcpCtx->serverTX);
	    close (ctx->tcpCtx->serverRX);
	}
    }
}

void Comm_GetNewCommChannel (CommMode commMode, uint16_t *tx, uint16_t *rx)
{
// Check SVE_HOOK
#ifndef SVE_HOOK
    int newChannel = -1;
    bool inputChannel = false;
    char read[10] = {};
    char *modeName = (commMode == COMM_MODE_XLINK) ? "XLink channel" : "TCP port";

    while (inputChannel != true)
    {
        // Get TX Channel/Port
        printf ("Please insert new %s(TX) to be used:\n", modeName);

        if (scanf ("%9s", read))
        {
            if (commMode == COMM_MODE_XLINK)
            {
                newChannel = (int)strtol (read, (char **)NULL, 16);
            }
            else if (commMode == COMM_MODE_TCP)
            {
		newChannel = atoi (read);
            }

            if (newChannel <= 0)
            {
                printf ("* Invalid %s (TX)\n", modeName);
                continue;
            }
            else
            {
                printf ("* %s - %u\n", modeName, newChannel);
                *tx = (uint16_t)newChannel;
                inputChannel = true;
            }
        }
    }

    newChannel = -1;
    inputChannel = false;
    memset_s (read, sizeof (read), 0);
    while (inputChannel != true)
    {
        // Get RX Channel/Port
        printf ("Please insert new %s(RX) to be used:\n", modeName);
        if (scanf ("%9s", read))
        {
            if (commMode == COMM_MODE_XLINK)
            {
                newChannel = (int)strtol (read, (char **)NULL, 16);
            }
            else if (commMode == COMM_MODE_TCP)
            {
                newChannel = atoi (read);
            }

            if (newChannel <= 0)
            {
                printf ("* Invalid %s (RX)\n", modeName);
                continue;
            }
            else
            {
                printf ("* %s - %u\n", modeName, newChannel);
                *rx = (uint16_t)newChannel;
                inputChannel = true;
            }
        }
    }
#else
    // Check for SVE_READ_FLAG
    if (getenv ("SVE_READ_FLAG") == NULL )
    {
        const char* configPath = getenv ("CONFIG_PATH");

        printf ("CONFIG_PATH: %s\n", configPath);

        FILE *fp = fopen (configPath, "r");

        // Check if readable
        if (fp == NULL)
        {
            printf ("Unable to open file: %s\n", configPath);
            exit (0);
        }

        char chunk[128];
        const char* txStr;
        const char* rxStr;

        // Get tx or rx based on mode
        if (commMode == COMM_MODE_XLINK)
        {
                printf ("XLINK mode\n");
                txStr = "CHANNELTX ";
                rxStr = "CHANNELRX ";
        } else
        {
                printf ("TCP mode\n");
                txStr = "PORTTX ";
                rxStr = "PORTRX ";
        }

        while (fgets (chunk, sizeof (chunk), fp) != NULL)
        {
            // Remove comment
            if (chunk[0] != '#')
            {
                // Get tx or rx
                if (!memcmp (txStr, chunk, strnlen_s (txStr, RX_TX_MAX_STRLEN)))
                {
                    chunk[ strnlen_s (chunk, sizeof (chunk)) - 1 ] = '\0';

                    if (commMode == COMM_MODE_XLINK)
                    {
                        *tx = (uint16_t)strtol (&chunk[strnlen_s (txStr , RX_TX_MAX_STRLEN)],
		            (char **)NULL, 16);
                    }
                    else if (commMode == COMM_MODE_TCP)
                    {
                        *tx = (uint16_t)atoi (&chunk[strnlen_s (txStr, RX_TX_MAX_STRLEN)]);
                    }

                } else if (!memcmp (rxStr, chunk, strnlen_s (rxStr, RX_TX_MAX_STRLEN)))
                {
                    chunk[ strnlen_s (chunk, sizeof (chunk)) - 1 ] = '\0';

                    if (commMode == COMM_MODE_XLINK)
                    {
                        *rx = (uint16_t)strtol (&chunk[strnlen_s (rxStr, RX_TX_MAX_STRLEN)],
		            (char **)NULL, 16);
                    }
                    else if (commMode == COMM_MODE_TCP)
                    {
                        *rx = (uint16_t)atoi (&chunk[strnlen_s (rxStr, RX_TX_MAX_STRLEN)]);
                    }
                }
            }
        }

        printf ("%-10s : %d\n", txStr, *tx);
        printf ("%-10s : %d\n", rxStr, *rx);

        setenv ("SVE_READ_FLAG", "1", 0); // set SVE_READ_FLAG
    }
    else
    {
        while (1)
        {
            usleep (SVE_INPUT_SLEEP_INTERVAL);
        }
    }
#endif  // ifndef SVE_HOOK
}
