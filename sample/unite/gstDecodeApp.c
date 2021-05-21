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
//! \file    decodeApp.c
//! \brief   Sample app for GStreamer H.264/H.265 decode pipeline
//! \details Sample Gstreamer decode pipeline with HDDLUnite integration
//!

#include <gst/gst.h>
#include <glib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <stdio.h>
#define CODEC_MAX_STRLEN 10
#define OUTPUTNAME_MAX_STRLEN 300


typedef struct _PARAMS
{
    gchar *source;
    gchar *sink;
    gint buffers;
    gchar *codec;
} Params;

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
    GstElement *pipeline, *source, *bypass, *parser, *decoder, *sink;
    GstBus *bus;
    guint busWatchId;

    Params inputElement = *(Params *)value;

    loop = g_main_loop_new (NULL, FALSE);

    g_print ("enter thread and create pipeline\n");
    // Create GStreamer elements
    pipeline = gst_pipeline_new ("decode-pipeline");
    source = gst_element_factory_make ("filesrc", "source");
    bypass = gst_element_factory_make ("bypass", "bypass");

    if (g_strcmp0 (inputElement.codec, "h264") == 0)
    {
        parser = gst_element_factory_make ("h264parse", "parser");
        decoder = gst_element_factory_make ("vaapih264dec", "decoder");
    }
    else if (g_strcmp0 (inputElement.codec, "h265") == 0)
    {
        parser = gst_element_factory_make ("h265parse", "parser");
        decoder = gst_element_factory_make ("vaapih265dec", "decoder");
    }

    sink = gst_element_factory_make ("filesink", "sink");

    if (!pipeline || !bypass || !source || !parser || !decoder || !sink)
    {
        g_printerr ("Failed to create element\n");
        gst_object_unref (pipeline);
        pthread_exit (NULL);
    }

    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    busWatchId = gst_bus_add_watch (bus, busCallback, loop);
    gst_object_unref (bus);

    // Build and link the GStreamer pipeline
    gst_bin_add_many (GST_BIN (pipeline), source, bypass, parser, decoder, sink, NULL);

    if (!gst_element_link_many (source, bypass, parser, decoder, sink, NULL))
    {
        g_printerr ("Failed to link element\n");
        gst_object_unref (pipeline);
        pthread_exit (NULL);
    }

    // Set filesrc and filesink location property with input parameter
    g_object_set (G_OBJECT (source), "location", inputElement.source, NULL);

    if (inputElement.buffers > 0)
    {
        g_object_set (G_OBJECT (source), "num-buffers", inputElement.buffers, NULL);
    }

    g_object_set (G_OBJECT (sink), "location", inputElement.sink, NULL);

    // Start playing
    if (gst_element_set_state (pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr ("Failed to set pipeline to playing state\n");
        gst_object_unref (pipeline);
        pthread_exit (NULL);
    }

    g_print ("gst loop run\n");
    g_main_loop_run (loop);
    gst_element_set_state (pipeline, GST_STATE_NULL);

    gst_object_unref (pipeline);
    g_source_remove (busWatchId);
    g_main_loop_unref (loop);

    g_print ("thread_entry return\n");
}


int main (int argc, char *argv[])
{
    // Check input arguments
    if (argc != 6)
    {
        g_printerr ("Incorrect input.\n"
            "***********************************************************************************\n"
            "Usage:\n"
            "%s <decoder> <input> <output> <num_buffers> <num_threads>\n"
            "-----------------------------------------------------------------------------------\n"
	    "decoder\t\t: Decoder type - h264 / h265\n"
            "input\t\t: Video file to be decoded - *.h264 / *.h265\n"
            "output\t\t: Decoded file name\n"
            "num_buffers\t: Number of buffers to be decoded\n"
            "\t\t  Set to 0 to decode the whole video files\n"
            "num_threads\t: Number of concurrent threads\n"
            "***********************************************************************************\n",
	    argv[0]);
        return -1;
    }

    int numBuffers = atoi (argv[4]);
    int numThreads = atoi (argv[5]);
    pthread_t newThread[numThreads];
    Params *inputElement = (Params *)malloc (numThreads *
        sizeof (Params));

    if (!(g_strcmp0 (argv[1], "h264") == 0 || g_strcmp0 (argv[1], "h265") == 0))
    {
        g_printerr ("Unsupported codec: %s\n", argv[1]);
        free (inputElement);
        return -1;
    }

    g_print ("num threads %d, codec %s\n", numThreads, argv[1]);

    // Initialize GStreamer
    gst_init (&argc, &argv);

    for (int i=0; i < numThreads; i++)
    {
	gchar *codec = malloc (sizeof (gchar) * (strnlen (argv[1], CODEC_MAX_STRLEN)));
        gchar *outputName = malloc (sizeof (gchar) * (strnlen (argv[3],
	    OUTPUTNAME_MAX_STRLEN) + 6));

        sprintf (codec, "%s", argv[1]);
        sprintf (outputName, "%s-%d.yuv", argv[3], i);

        inputElement[i].source = argv[2];
        inputElement[i].sink = outputName;
        inputElement[i].buffers = numBuffers;
        inputElement[i].codec = codec;

        pthread_create (&newThread[i], NULL, thread_entry, (void *)&inputElement[i]);
    }

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join (newThread[i], NULL);
        g_print ("pthread_join return\n");
    }

    for (int i = 0; i < numThreads; i++)
    {
        free (inputElement[i].sink);
        free (inputElement[i].codec);
    }
    free (inputElement);

    return 0;
}
