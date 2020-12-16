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
//! \file    target_shim_entry.c
//! \brief   External entry point for VAAPI Shim execution on accelerator
//! \details Start hddl_bypass_shim execution with given mode
//!

#include "target_shim_entry.h"
#include "target_va_shim.h"

HDDLVAShimStatus HDDLShim_StartVAAPIShim (HDDLVAShimCommMode vaCommMode)
{
    CommMode commMode = COMM_MODE_UNKNOWN;
    HDDLVAShimStatus vaShimStatus = HDDL_VA_SHIM_SUCCESS;
    HDDLShimStatus shimStatus = HDDL_SHIM_STATUS_SUCCESS;

    if (vaCommMode == HDDL_COMM_MODE_XLINK)
    {
        commMode = COMM_MODE_XLINK;
    }
    else if (vaCommMode == HDDL_COMM_MODE_UNITE)
    {
        commMode = COMM_MODE_UNITE;
    }
    else if (vaCommMode == HDDL_COMM_MODE_TCP)
    {
        commMode = COMM_MODE_TCP;
    }
    else
    {
        printf ("VAAPI Shim: Invalid communciation mode\n");
        return HDDL_VA_SHIM_FAILED;
    }

    shimStatus = HDDLShim_StartVAAPIShimWithMode (commMode);

    if (shimStatus != HDDL_SHIM_STATUS_SUCCESS)
    {
        vaShimStatus = HDDL_VA_SHIM_FAILED;
    }

    return vaShimStatus;
}

