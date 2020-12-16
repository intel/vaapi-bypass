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
//! \file    tcp.h
//! \brief   Communcation interface for TCP communication
//! \details Provide tcp/ip basic communication operation
//!

#ifndef __TCP_SOCKET_H__
#define __TCP_SOCKET_H__

#include "hddl_va_shim_common.h"
#include "thread_manager.h"
#include "memory_manager.h"

//!
//! \brief   TCP communcation context initialization
//! \return  HDDLShimTCPContext *
//!          Return pointer if success, else NULL
//!
HDDLShimTCPContext *TCP_ContextInit (uint16_t portTX, uint16_t portRX, struct hostent *server);

//!
//! \brief   TCP communcation initialization
//! \return  TCPStatus
//!          Return nonnegative integer if success, else fail
//!
TCPStatus TCP_Initialize (HDDLShimTCPContext *tcpCtx, int flag);

//!
//! \brief   TCP communcation connection
//! \return  TCPStatus
//!          Return nonnegative integer if success, else fail
//!
TCPStatus TCP_Connect (HDDLShimTCPContext *tcpCtx);

//!
//! \brief   TCP communcation write operation
//! \return  TCPStatus
//!          Return nonnegative integer if success, else fail
//!
TCPStatus TCP_Write (HDDLShimTCPContext *tcpCtx, int dataSize, void *payload);

//!
//! \brief   TCP communcation read operation
//! \return  TCPStatus
//!          Return nonnegative if success, else fail
//!
TCPStatus TCP_Read (HDDLShimTCPContext *tcpCtx, int dataSize, void *payload);

//!
//! \brief   TCP communcation peek operation
//! \return  TCPStatus
//!          Return positive integer if success, 0 if connection closed, else fail
//!
TCPStatus TCP_Peek (HDDLShimTCPContext *tcpCtx, uint32_t *size, void *payload);

//!
//! \brief   TCP communcation disconnection
//! \return  TCPStatus
//!          Return nonnegative integer if success, else fail
//!
TCPStatus TCP_Disconnect (HDDLShimTCPContext *tcpCtx, int flag);

//!
//! \brief   TCP communcation reconnect
//! \return  TCPStatus
//!          Return 0 if success, else fail
//!
TCPStatus TCP_Reconnect (HDDLShimTCPContext *tcpCtx);

#endif

//EOF

