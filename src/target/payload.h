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
//! \file    payload.h
//! \brief   Payload handler for VSI LibVA
//! \details Extract Libva info from payload and call corresponding VA function in VSI.
//!

#ifndef __PAYLOAD_H__
#define __PAYLOAD_H__

#include "hddl_va_shim_common.h"
#include "thread_manager.h"
#include "memory_manager.h"

#ifdef USE_HANTRO
// For accelerator, use system va_hantro.h instead of local header
#include <va_hantro/va_hantro.h>
#endif

#ifdef HDDL_UNITE
#include <DeviceClient.h>
#endif

//!
//! \brief   Main interface for payload extraction
//! \return  void
//!          Return nothing
//!
void * HDDLShim_MainPayloadExtraction (HDDLVAFunctionID vaFunctionId, HDDLShimCommContext *ctx,
    void *inPayload, int inSize);

//!
//! \brief   Extract individual payload based on functionId
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractPayload (HDDLVAFunctionID functionId, HDDLShimCommContext *ctx,
    void *inPayload, void **outPayload);

//!
//! \brief   Extract & call vaInitialize for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAInit (VADisplay vaDpy, void *inPayload, void **outPayload);

//!
//! \brief   Extract & call vaTerminate for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVATerminate (VADisplay vaDpy, void *inPayload, void **outPayload);

//!
//! \brief   Extract & call vaCreateConfig for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVACreateConfig (HDDLShimCommContext *ctx, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaDestroyConfig for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVADestroyConfig (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaCreateContext for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVACreateContext (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaDestroyContext for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVADestroyContext (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaCreateSurfaces for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVACreateSurfaces (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaDestroySurfaces for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVADestroySurfaces (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaCreateBuffer for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVACreateBuffer (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaDestroyBuffer for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVADestroyBuffer (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaMapBuffer for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAMapBuffer (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaUnmapBuffer for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAUnmapBuffer (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaCreateImage for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVACreateImage (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaDeriveImage for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVADeriveImage (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaDestroyImage for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVADestroyImage (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaBeginPicture for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVABeginPicture (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaRenderPicture for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVARenderPicture (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaEndPicture for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAEndPicture (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaSyncSurface for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVASyncSurface (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaQueyConfigProfiles for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAQueryConfigProfiles (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaQueryConfigEntrypoints for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAQueryConfigEntrypoints (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaGetConfigAttributes for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAGetConfigAttributes (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaQueryConfigAttributes for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAQueryConfigAttributes (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaQuerySurfaceStatus for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAQuerySurfaceStatus (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaQueryImageFormats for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAQueryImageFormats (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaGetImage for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAGetImage (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaPutImage for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAPutImage (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaQueryDisplayAttributes for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAQueryDisplayAttributes (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaGetDisplayAttributes for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAGetDisplayAttributes (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaSetDisplayAttributes for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVASetDisplayAttributes (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaQueySurfaceAttributes for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAQuerySurfaceAttributes (VADisplay vaDpy, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaCreateSurfaces2 for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVACreateSurfaces2 (VADisplay vaDpy, void *inPayload,
    void **outPayload, bool importFd, uint32_t swDeviceId);

//!
//! \brief   Extract & call vaExportSurfaceHandle for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAExportSurfaceHandle (VADisplay vaDpy, void *inPayload,
    void **outPayload, bool registerFd, uint64_t workloadId);

//!
//! \brief   Extract & call vaCreateBuffer2 for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVACreateBuffer2 (HDDLShimCommContext *ctx, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract & call vaAcquireBufferHandle for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//
VAStatus HDDLShim_ExtractandCallVAAcquireBufferHandle (VADisplay vaDpy, void *inPayload,
    void **outPayload, bool registerFd, uint64_t workloadId, HDDLVABufferNode *vaBufferNodeList);

//!
//! \brief   Extract & call vaReleaseBufferHandle for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVAReleaseBufferHandle (VADisplay vaDpy, void *inPayload,
    void **outPayload, bool unregisterFd, uint64_t workloadId, HDDLVABufferNode *vaBufferNodeList);

//!
//! \brief   Extract & call vaBufferSetNumElements for KMB Target
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallVASetNumElements (HDDLShimCommContext *ctx, void *inPayload,
    void **outPayload);

//!
//! \brief   Extract new dynamic channel ID to be used
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLShim_ExtractandCallDynamicChannelID (HDDLShimCommContext *ctx, void *inPayload,
    void **outPayload);

#endif
