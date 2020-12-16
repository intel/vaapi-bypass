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
//! \file    target_va_shim.h
//! \brief   Main program execution for KMB Target
//! \details Include basic functions required to retrieve/send payload & call corresponding
//!          VA function.
//!

#ifndef __TARGET_VA_SHIM_H__
#define __TARGET_VA_SHIM_H__

#include "hddl_va_shim_common.h"
#include "payload.h"
#include "gen_comm.h"
#include "va_display.h"

#ifdef HDDL_UNITE
#include <DeviceClient.h>
#endif

//!
//! \brief   Main program to start VAAPI Shim on accelerator
//! \return  int
//!          Return 0 if success, else fail
//!
int main (int argc, char *argv[]);

//!
//! \brief   Start VAAPI Shim with specific mode on accelerator
//! \return  HDDLShimStatus
//!          Return HDDL_SHIM_STATUS_SUCCESS if success, else fail
//!
HDDLShimStatus HDDLShim_StartVAAPIShimWithMode (CommMode commMode);

//!
//! \brief   Receiver listener for receiving payload
//! \return  void
//!          Return nothing
//!
void MainReceiverListener (HDDLShimCommContext *ctx);

#ifdef HDDL_UNITE
//!
//! \brief   Callback function to register to HDDLUnite
//! \return  void
//!          Return nothing
//!
void HDDLShim_NewWorkloadAvailable (uint64_t workloadId, ChannelID* channelId, uint32_t channelNum);
#endif

//!
//! \brief   Starting a execution new thread
//! \return  void
//!          Return nothing
//!
void *HDDLShim_StartNewThread (void *);
#endif
