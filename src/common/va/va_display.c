/*
 * Copyright (c) 2012 Intel Corporation. All Rights Reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <va/va.h>
#include "va_display.h"

extern const VADisplayHooks va_display_hooks_drm;
static const VADisplayHooks *g_display_hooks;
static const VADisplayHooks *g_display_hooks_available[] = {
    &va_display_hooks_drm,
    NULL
};

void va_open_display (VADisplay *vaDpy, int *drmFd)
{
    unsigned int i;

    for (i = 0; !(*vaDpy) && g_display_hooks_available[i]; i++)
    {
        g_display_hooks = g_display_hooks_available[i];
        g_display_hooks->open_display (vaDpy, drmFd);
    }

    if (!(*vaDpy))
    {
        fprintf (stderr, "error: failed to initialize display\n");
        exit (1);
    }
}

void va_close_display (VADisplay vaDpy, int drmFd)
{
    if (!vaDpy)
        return;

    if (g_display_hooks && g_display_hooks->close_display)
        g_display_hooks->close_display (drmFd);
}
