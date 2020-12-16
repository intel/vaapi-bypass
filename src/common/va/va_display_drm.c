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
#include <unistd.h>
#include <fcntl.h>
#include <va/va_drm.h>
#include "va_display.h"

static void va_open_display_drm (VADisplay *vaDpy, int *drmFd)
{
    VADisplay va_dpy;
    int drm_fd = -1;
    int i;

    static const char *drm_device_paths[] = {
        "/dev/dri/renderD129",
        "/dev/dri/renderD128",
        "/dev/dri/card0",
        NULL
    };

    for (i = 0; drm_device_paths[i]; i++)
    {
        drm_fd = open (drm_device_paths[i], O_RDWR);

        if (drm_fd < 0)
            continue;

        va_dpy = vaGetDisplayDRM (drm_fd);

        if (va_dpy)
	{
            *vaDpy = va_dpy;
            *drmFd = drm_fd;
            return;
	}

        close (drm_fd);
        drm_fd = -1;
    }
}

static void va_close_display_drm (int drmFd)
{
    if (drmFd < 0)
        return;

    close (drmFd);
}

const VADisplayHooks va_display_hooks_drm = {
    "drm",
    va_open_display_drm,
    va_close_display_drm,
};
