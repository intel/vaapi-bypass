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
//! \file    xlink_pcie.h
//! \brief   Communcation interface for XLINK communication
//! \details Provide xlink basic communication operation
//!

#ifndef __XLINK_PCIE_H__
#define __XLINK_PCIE_H__

#include "hddl_va_shim_common.h"
#include "thread_manager.h"
#include "memory_manager.h"

//!
//! \brief   XLINK communcation context initialization
//! \return  HDDLShimXLinkContext *
//!          Return pointer if success, else NULL
//!
HDDLShimXLinkContext *XLink_ContextInit (xLinkChannelId_t channelTX, xLinkChannelId_t channelRX);

//!
//! \brief   XLINK communcation initialization
//! \return  XLinkStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
XLinkStatus XLink_Initialize (HDDLShimXLinkContext *xLinkCtx, int flag);

//!
//! \brief   XLINK communcation connection
//! \return  XLinkStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
XLinkStatus XLink_Connect (HDDLShimXLinkContext *xLinkCtx);

//!
//! \brief   XLINK communcation write operation
//! \return  XLinkStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
XLinkStatus XLink_Write (HDDLShimXLinkContext *xLinkCtx, int size, void *payload);

//!
//! \brief   XLINK communcation read operation
//! \return  XLinkStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
XLinkStatus XLink_Read (HDDLShimXLinkContext *xLinkCtx, int size, void *payload);

//!
//! \brief   XLINK communcation read operation for dynamic data
//! \return  XLinkStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
XLinkStatus XLink_Peek (HDDLShimXLinkContext *xLinkCtx, uint32_t *size, void **payload);

//!
//! \brief   XLINK communcation disconnection
//! \return  XLinkStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
XLinkStatus XLink_Disconnect (HDDLShimXLinkContext *xLinkCtx, int flag);

//!
//! \brief   XLINK last used channel number
//! \return  void
//!          Return nothing
//!
void XLink_GetLastChannel (uint16_t *lastChannel);

//!
//! \brief   XLINK callback function to register xlink device
//! \return  int
//!          Return 0
//!
int XLink_DeviceEventCallback (uint32_t swDeviceId, uint32_t eventType);

//!
//! \brief   XLINK function to check xlink device event
//! \return  XLinkStatus
//!          Return COMM_STATUS_SUCCESS if success, else fail
//!
XLinkStatus XLink_CheckDeviceEvent (HDDLShimXLinkContext *xLinkCtx);
#endif

//EOF
