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
//! \file    gstbypass.h
//! \brief   GStreamer plugins for HDDL Bypass Mode
//! \details Gstreamer plugins for initiate WorkloadContext
//!

#ifndef GST_BYPASS_H
#define GST_BYPASS_H

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_BYPASS \
(gst_bypass_get_type())
#define GST_BYPASS(obj) \
(G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_BYPASS, GstBypass))
#define GST_BYPASS_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_BYPASS, \
GstBypassClass))
#define GST_IS_BYPASS(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_BYPASS))
#define GST_IS_BYPASS_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_BYPAS
typedef struct _GstBypass GstBypass;
typedef struct _GstBypassClass GstBypassClass;

struct _GstBypass
{
  GstElement element;
  GstPad *sinkpad;
  GstPad *srcpad;
};

struct _GstBypassClass
{
  GstElementClass parent_class;
};

GType gst_bypass_get_type (void);

G_END_DECLS

#endif /* GST_BYPASS_H */
