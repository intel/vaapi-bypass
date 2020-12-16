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
//! \file    invokeShim.c
//! \brief   Sample app to invoke VAAPI Shim on accelerator
//! \details Sample app to invode VAAPI Shim through external entry point
//!

#include <stdio.h>
#include <stdlib.h>
#include <target_shim_entry.h>

int main (int argc, char *argv[])
{
    if (argc < 2)
    {
        printf ("Error: Please speficy comm mode\n"
            "\t1 - XLINK\n"
            "\t2 - UNITE\n");
	return 1;
    }

    HDDLVAShimStatus vaShimStatus = HDDL_VA_SHIM_FAILED;
    HDDLVAShimCommMode vaShimCommMode;

    if (atoi (argv[1]) == 1)
    {
        vaShimCommMode = HDDL_COMM_MODE_XLINK;
    }
    else if (atoi (argv[1]) == 2)
    {
        vaShimCommMode = HDDL_COMM_MODE_UNITE;
    }
    else
    {
        printf ("Error: Invalid comm mode\n");
    }

    vaShimStatus = HDDLShim_StartVAAPIShim (vaShimCommMode);

    return vaShimStatus;
}
