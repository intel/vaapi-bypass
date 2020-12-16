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
//! \file    target_shim_entry.h
//! \brief   External entry point for VAAPI Shim execution on accelerator
//! \details Start hddl_bypass_shim execution with given mode
//!

#ifndef __TARGET_SHIM_ENTRY_H__
#define __TARGET_SHIM_ENTRY_H__

typedef enum
{
    HDDL_VA_SHIM_SUCCESS,
    HDDL_VA_SHIM_FAILED
}HDDLVAShimStatus;

typedef enum
{
    HDDL_COMM_MODE_TCP,
    HDDL_COMM_MODE_XLINK,
    HDDL_COMM_MODE_UNITE
}HDDLVAShimCommMode;

//!
//! \brief   External entry point to start VAAPI Shim on accelerator
//! \return  HDDLVAShimStatus
//!          Return HDDL_VA_SHIM_SUCCESS if success, else fail
//!
HDDLVAShimStatus HDDLShim_StartVAAPIShim (HDDLVAShimCommMode vaCommMode);

#endif
