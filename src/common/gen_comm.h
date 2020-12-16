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
//! \file    gen_comm.h
//! \brief   General interface for different communication
//! \details Provide connecting option for communcation depend on its API
//!          Available communcation method, such as XLINK, TCP
//!

#ifndef __GEN_COMM_H__
#define __GEN_COMM_H__

#include "hddl_va_shim_common.h"
#include "xlink/xlink_pcie.h"
#include "tcp/tcp.h"
#include "unite/unite.h"

typedef enum
{
    PAYLOAD_FREE,
    PAYLOAD_RESET
}HDDLShimBatchPayloadOp;

//!
//! \brief   Communication settings initialization from config files
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_ContextInitFromConfig (HDDLShimCommContext **ctx);

//!
//! \brief   Communication settings initialization
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_ContextInit (HDDLShimCommContext **ctx, ShimThreadParams *threadParams);

//!
//! \brief   Communication setting initializations with dynamic channels
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_DynamicContextInit (HDDLShimCommContext **ctx, HDDLShimCommContext *mainCtx,
    uint16_t *lastChannel, uint16_t *tx, uint16_t *rx);

//!
//! \brief   Communication initialization
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_Initialize (HDDLShimCommContext *ctx, int flag);

//!
//! \brief   Communication connection
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_Connect (HDDLShimCommContext *ctx, int flag);

//!
//! \brief   Communication write operation
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_Write (HDDLShimCommContext *ctx, int size, void *payload);

//!
//! \brief   Communication read operation
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_Read (HDDLShimCommContext *ctx, int size, void *payload);

//!
//! \brief   Communication safe read operation with mutex lock and unlock
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_ReadSafe (HDDLShimCommContext *ctx, int size, void *payload);

//!
//! \brief   Communication peek operation
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_Peek (HDDLShimCommContext *ctx, uint32_t *size, void *payload);

//!
//! \brief   Communication write and read operations
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_Submission (HDDLShimCommContext *ctx, HDDLVAFunctionID functionId,
    CommReadOp readOp, int inSize, void *inPayload, int outSize, void **outPayload);

//!
//! \brief   Communication write and read operations in batch
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_BatchSubmission (HDDLShimCommContext *ctx, HDDLVAFunctionID functionId,
    CommReadOp readOp, int inSize, void *inPayload, int outSize, void **outPayload);

//!
//! \brief   Single communcation write and read operation
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_SingleSubmission (HDDLShimCommContext *ctx, CommReadOp readOp, int inSize,
    void *inPayload, int outSize, void **outPayload);

//!
//! \brief   Communication disconnection
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_Disconnect (HDDLShimCommContext *ctx, int flag);

//!
//! \brief   Communication reconnect, only needed on TARGET side
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_Reconnect (HDDLShimCommContext *ctx);

//!
//! \brief   Get Communication Mode from starting parameter
//! \return  void
//!          Return nothing
//!
void Comm_ProcessCommMode (CommMode *commMode, char *mode);

//!
//! \brief   Get Communication Mode from user input
//! \return  void
//!          Return nothing
//!
void Comm_GetCommMode (CommMode *commMode);

//!
//! \brief   Get Communication channel from user input
//! \return  void
//!          Return nothing
//!
void Comm_GetNewCommChannel (CommMode commMode, uint16_t *tx, uint16_t *rx);

//!
//! \brief   Get the last open channel
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Comm_GetLastChannel (HDDLShimCommContext *ctx, uint16_t *lastChannel);

//!
//! \brief   Destroy Mutex for each CommMode
//! \return  void
//!          Return nothing
//!
void Comm_MutexDestroy (HDDLShimCommContext *ctx);

//!
//! \brief   Close Sockets for TCP_COMM_MODE
//! \return  void
//!          Return nothing
//!
void Comm_CloseSocket (HDDLShimCommContext *ctx, int flag);
#endif
//EOF
