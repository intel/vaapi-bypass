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
//! \file    unite.h
//! \brief   Communcation interface for HDDLUnite
//! \details Provide HDDLUnite basic communication operation
//!

#include <sys/syscall.h>

#include "hddl_va_shim_common.h"
#include "thread_manager.h"
#include "memory_manager.h"
#include "debug_manager.h"
#include "xlink_pcie.h"

#ifdef HDDL_UNITE
#ifdef IA
#include <WorkloadContextC.h>
#include <DeviceResetC.h>
#endif

#ifdef ACCEL
#include <DeviceClient.h>
#endif
#endif

//!
//! \brief   Unite communcation context initialization
//! \return  HDDLShimXLinkContext *
//!          Return pointer if success, else NULL
//!
HDDLShimUniteContext *Unite_ContextInit (xLinkChannelId_t channelTX, xLinkChannelId_t channelRX,
    uint64_t workloadId, uint32_t swDeviceId);

//!
//! \brief   Unite communcation initialization
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Unite_Initialize (HDDLShimUniteContext *uniteCtx, int flag);

//!
//! \brief   Unite communcation connection
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Unite_Connect (HDDLShimUniteContext *uniteCtx);

//!
//! \brief   Unite communcation write operation
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Unite_Write (HDDLShimUniteContext *uniteCtx, int size, void *payload);

//!
//! \brief   Unite communcation read operation
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Unite_Read (HDDLShimUniteContext *uniteCtx, int size, void *payload);

//!
//! \brief   Unite communcation peek operation
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Unite_Peek (HDDLShimUniteContext *uniteCtx, uint32_t *size, void *payload);

//!
//! \brief   Unite communcation disconnection
//! \return  CommStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
CommStatus Unite_Disconnect (HDDLShimUniteContext *uniteCtx, int flag);

//!
//! \brief   Callback function to register xlink device for unite mode
//! \return  int
//!          Return 0
//!
int Unite_DeviceEventCallback (uint32_t swDeviceId, uint32_t eventType);

//!
//! \brief   Function to check xlink device event returned
//! \return  XLinkStatus
//!          Return X_LINK_SUCCESS if sucess, else fail
//!
XLinkStatus Unite_CheckDeviceEvent (HDDLShimUniteContext *uniteCtx);
