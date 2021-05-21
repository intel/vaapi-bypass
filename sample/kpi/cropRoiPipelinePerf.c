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
//! \file    cropRoiPipelinePerf.c
//! \brief   Sample app for GStreamer transcode pipeline with crop-roi support
//! \details Sample Gstreamer transcode pipeline for performance measurements
//!

#include <gst/gst.h>
#include <glib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#define OUTPUT_NAME_MAX_STRLEN 300

typedef struct _PARAMS
{
    gchar *pipeline;
    gchar *source;
    gchar *sink;
    gint buffers;
    gchar *decoder;
    gchar *encoder;
    gchar *roi;
    gchar *caps;
    gint thread;
    gboolean plugin;
} Params;

gdouble *runTime;

static gboolean busCallback (GstBus *bus, GstMessage *msg, gpointer data)
{
    GMainLoop *loop = (GMainLoop *)data;

    switch (GST_MESSAGE_TYPE (msg))
    {
        case GST_MESSAGE_EOS:
            g_print ("Reach end of stream\n");
            g_main_loop_quit (loop);
            break;

        case GST_MESSAGE_ERROR:
        {
            gchar *debug;
            GError *error;

            gst_message_parse_error (msg, &error, &debug);
            g_printerr ("Error: %s\n", error->message);

            g_free (debug);
            g_error_free (error);

            g_main_loop_quit (loop);
            break;
        }

        default:
            break;
    }

    return TRUE;
}

/* Thread mechanism */
void * thread_entry (void *value)
{
    GMainLoop *loop;
    GstElement *pipeline, *source, *parser, *plugin, *decoder, *encoder, *sink, *caps;
    GstCaps *caps_params;

    GstBus *bus;
    guint busWatchId;
    struct timeval startTime, endTime;

    Params inputElement = *(Params *)value;
    int thread = inputElement.thread;
    bool usePlugin = inputElement.plugin;

    loop = g_main_loop_new (NULL, FALSE);

    if (usePlugin)
    {
        plugin = gst_element_factory_make ("bypass", "bypass");
	if (!plugin)
	{
	    g_printerr ("[Thread %d] Failed to create bypass plugin element\n", thread);
	    pthread_exit (NULL);
	}
    }

    // Create GStreamer elements
    pipeline = gst_pipeline_new ("transcode-pipeline");
    source = gst_element_factory_make ("filesrc", "source");

    if (g_strcmp0 (inputElement.decoder, "h264") == 0)
    {
        parser = gst_element_factory_make ("h264parse", "parser");
        decoder = gst_element_factory_make ("vaapih264dec", "decoder");
    }
    else if (g_strcmp0 (inputElement.decoder, "h265") == 0)
    {
        parser = gst_element_factory_make ("h265parse", "parser");
        decoder = gst_element_factory_make ("vaapih265dec", "decoder");
    }

    if (g_strcmp0 (inputElement.encoder, "h264") == 0)
    {
        encoder = gst_element_factory_make ("vaapih264enc", "encoder");
    }
    else if (g_strcmp0 (inputElement.encoder, "h265") == 0)
    {
        encoder = gst_element_factory_make ("vaapih265enc", "encoder");
    }
    else if (g_strcmp0 (inputElement.encoder, "jpeg") == 0)
    {
        encoder = gst_element_factory_make ("vaapijpegenc", "encoder");
    }

    caps = gst_element_factory_make ("capsfilter", "caps");

    if (g_strcmp0 (inputElement.caps, "dma") == 0)
    {
        caps_params = gst_caps_from_string ("video/x-raw(memory:DMABuf)");
    }
    else if (g_strcmp0 (inputElement.caps, "sys") == 0)
    {
        caps_params = gst_caps_from_string ("video/x-raw");
    }

    g_object_set (G_OBJECT (caps), "caps", caps_params, NULL);
    gst_caps_unref (caps_params);

    if (inputElement.sink == NULL)
    {
        sink = gst_element_factory_make ("fakesink", "sink");
    }
    else if ((inputElement.sink != NULL) && (g_strcmp0 (inputElement.encoder, "jpeg") == 0))
    {
        sink = gst_element_factory_make ("multifilesink", "sink");
    }
    else
    {
        sink = gst_element_factory_make ("filesink", "sink");
    }

    if (!pipeline || !source || !parser || !decoder || !encoder || !caps || !sink)
    {
        g_printerr ("[Thread %d] Failed to create element\n", thread);
        gst_object_unref (pipeline);
        pthread_exit (NULL);
    }

    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    busWatchId = gst_bus_add_watch (bus, busCallback, loop);
    gst_object_unref (bus);

    // Build and link the GStreamer pipeline
    if (usePlugin)
    {
        gst_bin_add_many (GST_BIN (pipeline), source, parser, plugin, decoder, caps, encoder,
	    sink, NULL);

        if (!gst_element_link_many (source, parser, plugin, decoder, caps, encoder, sink, NULL))
        {
            g_printerr ("[Thread %d] Failed to link element\n", thread);
            gst_object_unref (pipeline);
            pthread_exit (NULL);
        }
    }
    else
    {
        gst_bin_add_many (GST_BIN (pipeline), source, parser, decoder, caps, encoder, sink, NULL);

	if (!gst_element_link_many (source, parser, decoder, caps, encoder, sink, NULL))
	{
            g_printerr ("[Thread %d] no plugin Failed to link element\n", thread);
	    gst_object_unref (pipeline);
	    pthread_exit (NULL);
	}
    }

    // Set filesrc and filesink location property with input parameter
    g_object_set (G_OBJECT (source), "location", inputElement.source, NULL);

    g_object_set (encoder, "crop-roi", inputElement.roi, NULL);
    if (inputElement.buffers > 0)
    {
        g_object_set (G_OBJECT (source), "num-buffers", inputElement.buffers, NULL);
    }

    if (inputElement.sink != NULL)
    {
        g_object_set (G_OBJECT (sink), "location", inputElement.sink, NULL);
	free (inputElement.sink);
    }

    // Start playing
    gettimeofday (&startTime, NULL);

    if (gst_element_set_state (pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr ("[Thread %d] Failed to set pipeline to playing state\n", thread);
        gst_object_unref (pipeline);
        pthread_exit (NULL);
    }

    g_print ("[Thread %d] gst loop run\n", thread);
    g_main_loop_run (loop);

    gettimeofday (&endTime, NULL);

    runTime[thread] = ((endTime.tv_sec - startTime.tv_sec) * 1000000) +
        (endTime.tv_usec - startTime.tv_usec);

    gst_element_set_state (pipeline, GST_STATE_NULL);

    gst_object_unref (pipeline);
    g_source_remove (busWatchId);
    g_main_loop_unref (loop);

    g_print ("[Thread %d] Return\n", thread);
}


int main (int argc, char *argv[])
{
    // Check input arguments
    if (argc != 10)
    {
        g_printerr ("Incorrect input.\n"
            "***********************************************************************************\n"
            "Usage:\n"
            "%s <decoder> <encoder> <input> <num_buffers> <roi> <num_threads> <memory>"
	    " <output> <plugin>\n"
            "-----------------------------------------------------------------------------------\n"
            "decoder\t\t: Decoder type - h264 / h265\n"
            "encoder\t\t: Encoder type - h264 / h265 / jpeg\n"
            "input\t\t: Video file to be decoded - *.h264 / *.h265\n"
            "num_buffers\t: Number of buffer allocated\n"
	    "\t\t  Set to 0 to decode the whole video files\n"
	    "roi\t\t: roi region to crop - x, y, width, height\n"
            "num_threads\t: Number of concurrent threads\n"
	    "memory\t\t: memory type\n"
            "\t\t  Set dma for DMABuf\n"
            "\t\t  Set sys for system memory\n"
	    "output\t\t: Output file name\n"
	    "\t\t  Set to NULL for fakesink\n"
	    "plugin\t\t: Use bypass plugin for workload scheduling - yes / no\n"
            "***********************************************************************************\n",
	    argv[0]);
        return -1;
    }

    int numBuffers = atoi (argv[4]);
    int numThreads = atoi (argv[6]);
    bool plugin = false;
    pthread_t newThread[numThreads];
    Params *inputElement = (Params *)malloc (numThreads *
        sizeof (Params));
    gdouble totalTime = 0.0;

    runTime = malloc (numThreads * sizeof (gdouble));

    if ( (runTime != NULL) && (sizeof (numThreads * sizeof (gdouble)) > 0))
    {
        memset (runTime, 0, sizeof (numThreads * sizeof (gdouble)));
    }
    else
    {
        g_printerr ("%s: Memset operation failed with overflow", __func__);
        free (inputElement);
        return -1;
    }


    if (!(g_strcmp0 (argv[1], "h264") == 0 || g_strcmp0 (argv[1], "h265") == 0))
    {
        g_printerr ("Unsupported decoder: %s\n", argv[1]);
        free (inputElement);
        return -1;
    }

    if (!(g_strcmp0 (argv[2], "h264") == 0 || g_strcmp0 (argv[2], "h265") == 0||
        g_strcmp0 (argv[2], "jpeg") == 0))
    {
        g_printerr ("Unsupported encoder: %s\n", argv[2]);
        free (inputElement);
        return -1;
    }

    if (!(g_strcmp0 (argv[7], "dma") == 0 || g_strcmp0 (argv[7], "sys") == 0))
    {
        g_printerr ("Unsupported memory type: %s\n", argv[7]);
        free (inputElement);
        return -1;
    }

    if (!(g_strcmp0 (argv[9], "yes") == 0 || g_strcmp0 (argv[9], "no") == 0))
    {
        g_printerr ("Unknown selection for plugin support: %s\n", argv[9]);
	free (inputElement);
	return -1;
    }
    else
    {
        if (g_strcmp0 (argv[9], "yes") == 0)
	{
	    plugin = true;
	}
    }

    g_print ("======== pipeline ========\n"
        "num threads\t: %d\n"
	"decoder\t\t: %s\n"
	"encoder\t\t: %s\n"
	"memory type\t: %s\n"
	"plugin\t\t: %s\n"
	"==============================\n",
	numThreads, argv[1], argv[2], argv[7], argv[9]);

    // Initialize GStreamer
    gst_init (&argc, &argv);

    for (int i=0; i < numThreads; i++)
    {
	gchar *outputName =  malloc (sizeof (gchar) *
	    (strnlen (argv[8], OUTPUT_NAME_MAX_STRLEN) + 6));

	if (g_strcmp0 (argv[8], "NULL") == 0)
	{
            outputName = NULL;
	}
	else
	{
            sprintf (outputName, "%s-%d", argv[8], i);
	}

	inputElement[i].sink = outputName;
        inputElement[i].source = argv[3];
        inputElement[i].buffers = numBuffers;
        inputElement[i].decoder = argv[1];
        inputElement[i].encoder = argv[2];
	inputElement[i].roi = argv[5];
        inputElement[i].thread = i;
	inputElement[i].caps = argv[7];
	inputElement[i].plugin = plugin;

        pthread_create (&newThread[i], NULL, thread_entry, (void *)&inputElement[i]);
    }

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join (newThread[i], NULL);
        g_print ("[Thread %d] Join\n", i);
    }

    for (int i = 0; i < numThreads; i++)
    {
        totalTime += runTime[i];
	g_print ("[Thread %d] Execution time: %2f seconds\n", i, runTime[i] / 1000000);
    }

    g_print ("-- Average execution time: %2f seconds --\n", totalTime / numThreads / 1000000);

    free (inputElement);

    return 0;
}
