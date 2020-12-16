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
//! \file    gstbypass.c
//! \brief   GStreamer plugins for HDDL Bypass Mode
//! \details Gstreamer plugins for initiate WorkloadContext
//!

#include "gstbypass.h"

#include <WorkloadContextC.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>

/* FIXME* update PACKAGE_ORIGIN after opened source */
#define PACKAGE_ORIGIN "http://www.intel.com"
#define PACKAGE_VERSION "0.0.1.0"
#define PLUGIN_LICENSE "MIT/X11"

#define PACKAGE "gstbypass"
#define GST_PLUGIN_NAME "bypass"
#define GST_PLUGIN_DESC "Bypass helper for setting up workload"

GST_DEBUG_CATEGORY_STATIC (gst_bypass_debug);
#define GST_CAT_DEFAULT gst_bypass_debug

#define gst_bypass_parent_class parent_class
G_DEFINE_TYPE (GstBypass, gst_bypass, GST_TYPE_ELEMENT);

static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY"));

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY"));

static GstFlowReturn gst_bypass_chain (GstPad *pad, GstObject *parent, GstBuffer *buf)
{
    GstBypass *bypass = GST_BYPASS (parent);

    gst_pad_push (bypass->srcpad, buf);
}

static void gst_bypass_finalize (GObject *object)
{
    GST_DEBUG ("pid %u tid %lu\n", getpid(), syscall (SYS_gettid));
    WorkloadID workloadId;
    HddlStatusCode hddlStatus = HDDL_OK;

    hddlStatus = getWorkloadContextId (getpid (), syscall (SYS_gettid), &workloadId);

    if (hddlStatus != HDDL_OK)
    {
        GST_ERROR ("Failed to obtain workload %ld\n", workloadId);
    }

    destroyWorkloadContext (workloadId);

    GST_DEBUG ("Destroyed workload %lu with pid %u tid %lu\n", workloadId, getpid(),
	syscall (SYS_gettid));

    G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void gst_bypass_dispose (GObject *object)
{
    GST_DEBUG ("pid %u tid %lu\n", getpid(), syscall (SYS_gettid));

    G_OBJECT_CLASS (parent_class)->dispose (object);
}

static void gst_bypass_workload_init ()
{
    WorkloadID workloadId;
    ContextHint contextHint;
    HddlStatusCode hddlStatus = HDDL_OK;

    // FIXME: Hardcoded context hint. Fix it later.
    contextHint.mediaDecodeType = MEDIA_DECODE_TYPE_H264;
    contextHint.mediaBitrate = 2.1;
    contextHint.ResolutionWidth = 640;
    contextHint.ResolutionHeight = 480;
    contextHint.mediaFps = 29.7;
    contextHint.internalWorkload = 0;

    hddlStatus = getWorkloadContextId (getpid (), syscall (SYS_gettid), &workloadId);

    if (hddlStatus != HDDL_OK)
    {
        hddlStatus = createWorkloadContext (&workloadId, &contextHint);

        if (hddlStatus != HDDL_OK)
        {
	    GST_ERROR ("Failed to create workload context\n");
        }

        GST_DEBUG ("Created workload %lu with pid %u tid %lu\n", workloadId, getpid(),
	    syscall (SYS_gettid));
    }
}

static void gst_bypass_class_init (GstBypassClass *klass)
{
    GST_DEBUG ("pid %u tid %lu\n", getpid(), syscall (SYS_gettid));
    gst_bypass_workload_init ();

    GObjectClass *const gobjectClass = G_OBJECT_CLASS (klass);
    GstElementClass *const elementClass = GST_ELEMENT_CLASS (klass);

    gobjectClass->finalize = gst_bypass_finalize;
    gobjectClass->dispose = gst_bypass_dispose;

    gst_element_class_set_static_metadata (elementClass, "Bypass", "Generic",
        GST_PLUGIN_DESC, "Soon, Thean Siew <thean.siew.soon@intel.com>");

    gst_element_class_add_pad_template (elementClass,
        gst_static_pad_template_get (&sink_factory));
    gst_element_class_add_pad_template (elementClass,
        gst_static_pad_template_get (&src_factory));

}

static void gst_bypass_init (GstBypass *bypass)
{
    GST_DEBUG ("pid %u tid %lu\n", getpid(), syscall (SYS_gettid));
    gst_bypass_workload_init ();

    bypass->sinkpad = gst_pad_new_from_static_template (&sink_factory, "sink");
    gst_pad_set_chain_function (bypass->sinkpad, gst_bypass_chain);
    GST_PAD_SET_PROXY_CAPS (bypass->sinkpad);
    gst_element_add_pad (GST_ELEMENT (bypass), bypass->sinkpad);

    bypass->srcpad = gst_pad_new_from_static_template (&src_factory, "src");
    GST_PAD_SET_PROXY_CAPS (bypass->srcpad);
    gst_element_add_pad (GST_ELEMENT (bypass), bypass->srcpad);
}


static gboolean bypass_plugin_init (GstPlugin *plugin)
{

    gboolean ret = FALSE;
    GST_DEBUG_CATEGORY_INIT (gst_bypass_debug, GST_PLUGIN_NAME, 0, GST_PLUGIN_DESC);

    ret = gst_element_register (plugin, "bypass", GST_RANK_NONE, GST_TYPE_BYPASS);

    GST_DEBUG ("pid %u tid %lu\n", getpid(), syscall (SYS_gettid));
    gst_bypass_workload_init ();

    return ret;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR, GST_VERSION_MINOR, bypass, GST_PLUGIN_DESC,
    bypass_plugin_init, PACKAGE_VERSION, PLUGIN_LICENSE, PACKAGE, PACKAGE_ORIGIN);
