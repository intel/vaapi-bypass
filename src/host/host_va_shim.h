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
//! \file    host_va_shim.h
//! \brief   Basic vaFunction for Host IA
//! \details Receive/Return vaFunction with application & read/write va info with KMB Target
//!

#ifndef __HOST_VA_SHIM_H__
#define __HOST_VA_SHIM_H__

#include "hddl_va_shim_common.h"
#include "gen_comm.h"
#include "memory_manager.h"
#include "thread_manager.h"

#include <sys/syscall.h>

#ifdef HDDL_UNITE
#include <WorkloadContextC.h>
#endif

#ifdef USE_HANTRO
#include "va_hantro.h"
#endif

//!
//! \brief   VA driver initialization
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus __vaDriverInit (VADriverContextP ctx);

//!
//! \brief   VA shim driver initialization
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_DriverInit (VADriverContextP ctx);

//!
//! \brief   VA shim driver Terminate
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_Terminate (VADriverContextP ctx);

//!
//! \brief   VA shim driver CreateConfig
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_CreateConfig (VADriverContextP ctx, VAProfile profile, VAEntrypoint entrypoint,
    VAConfigAttrib *attrib_list, int numAttrib, VAConfigID *configId);

//!
//! \brief   VA shim driver DestroyConfig
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_DestroyConfig (VADriverContextP ctx, VAConfigID configId);

//!
//! \brief   VA shim driver CreateContext
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_CreateContext (VADriverContextP ctx, VAConfigID configId,
    int pictureWidth, int pictureHeight, int flag, VASurfaceID *render_targets,
    int numRenderTarget, VAContextID *context);

//!
//! \brief   VA shim driver DestroyContext
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_DestroyContext (VADriverContextP ctx, VAContextID context);

//!
//! \brief   VA shim driver CreateSurfaces
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_CreateSurfaces (VADriverContextP ctx, int width, int height, int format,
    int numSurfaces, VASurfaceID *surfaces);

//!
//! \brief   VA shim driver DestroySurfaces
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_DestroySurfaces (VADriverContextP ctx, VASurfaceID *surfaceList,
    int numSurfaces);

//!
//! \brief   VA shim driver CreateBuffer
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_CreateBuffer (VADriverContextP ctx, VAContextID context, VABufferType type,
    unsigned int size, unsigned int numElement, void *data, VABufferID *bufId);

//!
//! \brief   VA shim driver DestroyBuffer
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_DestroyBuffer (VADriverContextP ctx, VABufferID bufId);

//!
//! \brief   VA shim driver MapBuffer
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_MapBuffer (VADriverContextP ctx, VABufferID bufId, void **buf);

//!
//! \brief   VA shim driver UnmapBuffer
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_UnmapBuffer (VADriverContextP ctx, VABufferID bufId);

//!
//! \brief   VA shim driver CreateImage
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_CreateImage (VADriverContextP ctx, VAImageFormat *format, int width,
    int height, VAImage *image);

//!
//! \brief   VA shim driver DeriveImage
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_DeriveImage (VADriverContextP ctx, VASurfaceID surface, VAImage *image);

//!
//! \brief   VA shim driver DestroyImage
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_DestroyImage (VADriverContextP ctx, VAImageID image);

//!
//! \brief   VA shim driver BeginPicture
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_BeginPicture (VADriverContextP ctx, VAContextID context,
    VASurfaceID renderTarget);

//!
//! \brief   VA shim driver RenderPicture
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_RenderPicture (VADriverContextP ctx, VAContextID context, VABufferID *buffer,
    int numBuffer);
//! \brief   VA shim driver EndPicture
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_EndPicture (VADriverContextP ctx, VAContextID context);

//!
//! \brief   VA shim driver SyncSurface
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_SyncSurface (VADriverContextP ctx, VASurfaceID renderTarget);

//!
//! \brief   VA shim driver QueryConfigProfiles
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_QueryConfigProfiles (VADriverContextP ctx, VAProfile *profileList,
    int *numProfile);

//!
//! \brief   VA shim driver QueryConfigEntrypoints
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_QueryConfigEntrypoints (VADriverContextP ctx, VAProfile profile,
    VAEntrypoint *entrypointList, int *numEntrypoint);

//!
//! \brief   VA shim driver GetConfigAttributes
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_GetConfigAttributes (VADriverContextP ctx, VAProfile profile,
    VAEntrypoint entrypoint, VAConfigAttrib *attribList, int numAttrib);

//!
//! \brief   VA shim driver QueryConfigAttributes
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_QueryConfigAttributes (VADriverContextP ctx, VAConfigID configId,
    VAProfile *profile, VAEntrypoint *entrypoint, VAConfigAttrib *attribList, int *numAttrib);

//!
//! \brief   VA shim driver QuerySurfaceStatus
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_QuerySurfaceStatus (VADriverContextP ctx, VASurfaceID renderTarget,
    VASurfaceStatus *status);

//!
//! \brief   VA shim driver QueryImageFormats
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_QueryImageFormats (VADriverContextP ctx, VAImageFormat *formatList,
    int *numFormat);

//!
//! \brief   VA shim driver GetImage
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_GetImage (VADriverContextP ctx, VASurfaceID surface, int x, int y,
    unsigned int width, unsigned int height, VAImageID image);

//!
//! \brief   VA shim driver PutImage
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_PutImage (VADriverContextP ctx, VASurfaceID surface, VAImageID image,
    int srcX, int srcY, unsigned int srcWidth, unsigned int srcHeight, int destX, int destY,
    unsigned int destWidth, unsigned int destHeight);

//!
//! \brief   VA shim driver QueryDisplayAttributes
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_QueryDisplayAttributes (VADriverContextP ctx, VADisplayAttribute *attrList,
    int *numAttributes);

//!
//! \brief   VA shim driver GetDisplayAttributes
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_GetDisplayAttributes (VADriverContextP ctx, VADisplayAttribute *attrList,
    int numAttributes);

//!
//! \brief   VA shim driver SetDisplayAttributes
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_SetDisplayAttributes (VADriverContextP ctx, VADisplayAttribute *attrList,
    int numAttributes);

//!
//! \brief   VA shim driver QuerySurfaceAttributes
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_QuerySurfaceAttributes (VADriverContextP dpy, VAConfigID config,
    VASurfaceAttrib *attribList, unsigned int *numAttribs);

//!
//! \brief   VA shim driver CreateSurfaces2
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_CreateSurfaces2 (VADriverContextP ctx, unsigned int format, unsigned int width,
    unsigned int height, VASurfaceID *surfaces, unsigned int numSurfaces,
    VASurfaceAttrib *attribList, unsigned int numAttribs);

//!
//! \brief   VA shim driver ExportSurfaceHandle
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_ExportSurfaceHandle (VADriverContextP ctx, VASurfaceID surfaceId,
    uint32_t memType, uint32_t flags, void *descriptor);

//!
//! \brief   VA shim driver CreateBuffer2
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_CreateBuffer2 (VADriverContextP ctx, VAContextID context, VABufferType type,
    unsigned int width, unsigned int height, unsigned int *unitSize, unsigned int *pitch,
    VABufferID *bufId);

//!
//! \brief   VA shim driver BufferSetNumElements
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_BufferSetNumElements (VADriverContextP ctx, VABufferID bufId,
    unsigned int numElement);

//!
//! \brief   VA shim driver AcquireBufferHandle
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_AcquireBufferHandle (VADriverContextP ctx, VABufferID bufId,
    VABufferInfo *bufInfo);

//!
//! \brief   VA shim driver ReleaseBufferHandle
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_ReleaseBufferHandle (VADriverContextP ctx, VABufferID bufId);

//==================================================================
// Internal utilities functions
//==================================================================
//!
//! \brief   VA shim driver
//! \return  HDDLVAShimDriverContext *
//!          Return pointer if success, else NULL
//!
HDDLVAShimDriverContext *HDDLVAShim_CreateHDDLBypassDriverContext ();

//!
//! \brief   VA shim driver comm context setup
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
HDDLShimCommContext *HDDLVAShim_CommContextSetup (HDDLVAShimDriverContext *vaShimCtx,
    uint64_t pid, uint64_t tid, HDDLShimCommContextNew commContextNew);

//!
//! \brief   VA shim driver comm context
//! \return  HDDLShimCommContext *
//!          Return pointer if success, else NULL
//!
HDDLShimCommContext *HDDLVAShim_GetCommContext (HDDLVAShimDriverContext *vaShimCtx);

//!
//! \brief   VA shim driver heap initialization
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_HeapInit (HDDLVAShimDriverContext *vaShimCtx);

//!
//! \brief   VA shim driver internal buffer allocation
//! \return  VAStatus
//!          Return VA_STATUS_SUCESS if success, else fail
//!
VAStatus HDDLVAShim_CreateInternalBufferAtHeap (HDDLVAShimDriverContext *vaShimCtx,
    VAContextID context, VABufferType type, unsigned int size, unsigned int numElement,
    VABufferID bufId, void *data);

//!
//! \brief   VA shim driver buffer heap cleanup
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_DestroyBufferHeap (VADriverContextP ctx);

//!
//! \brief   VA shim driver release internal heap buffer
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_ReleaseInternalBufferFromHeap (HDDLVAShimDriverContext *vaShimCtx,
    VABufferID bufId);

//!
//! \brief   VA shim driver release internal VAEncCodedBufferType buffer data
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_DestroyInternalVAEncCodedBuffer (HDDLVABuffer *vaBuffer);

//!
//! \brief   VA shim driver mapping internal buffer
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_MapInternalBuffer (HDDLVAShimDriverContext *vaShimCtx, VABufferID bufId,
    HDDLVABuffer *vaBuffer, void **buf);

//!
//! \brief   VA shim driver unmap internal buffer
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_UnmapInternalBuffer (HDDLVAShimDriverContext *vaShimCtx, VABufferID bufId,
    HDDLVABuffer *vaBuffer);

//!
//! \brief   VA shim driver image heap cleanup
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_DestroyImageHeap (VADriverContextP ctx);

//!
//! \brief   VA shim driver context heap cleanup
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_DestroyContextHeap (VADriverContextP ctx);

//==================================================================
// Unimplemented VA functions
//==================================================================
//!
//! \brief   VA shim driver PutSurface
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_PutSurface (VADriverContextP ctx, VASurfaceID surface, void *draw, short srcX,
    short srcY, unsigned short srcW, unsigned short srcH, short destX, short destY,
    unsigned short destW, unsigned short destH, VARectangle *cliprect, unsigned int numberCliprect,
    unsigned int flag);

//!
//! \brief   VA shim driver QuerySubpictureFormats
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_QuerySubpictureFormats (VADriverContextP ctx, VAImageFormat *formatlist,
    unsigned int *flag, unsigned int *numformat);

//!
//! \brief   VA shim driver CreateSubpicture
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_CreateSubpicture (VADriverContextP ctx, VAImageID image,
    VASubpictureID *subpicture);

//!
//! \brief   VA shim driver DestroySubpicture
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_DestroySubpicture (VADriverContextP ctx, VASubpictureID subpicture);

//!
//! \brief   VA shim driver SetSubpictureImage
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_SetSubpictureImage (VADriverContextP ctx, VASubpictureID subpicture,
    VAImageID image);

//!
//! \brief   VA shim driver SetSubpictureChromakey
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
Status HDDLVAShim_SetSubpictureChromakey (VADriverContextP ctx, VASubpictureID subpicture,
    unsigned int chromakeyMin, unsigned int chromakeyMax, unsigned int chromakeyMask);

//!
//! \brief   VA shim driver SetSubpictureGlobalAlpha
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_SetSubpictureGlobalAlpha (VADriverContextP ctx, VASubpictureID subpicture,
    float globalAlpha);

//!
//! \brief   VA shim driver AssociateSubpicture
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_AssociateSubpicture (VADriverContextP ctx, VASubpictureID subpicture,
    VASurfaceID *targetSurface, int numSurface, short srcX, short srcY, unsigned short srcWidth,
    unsigned short srcHeight, short destX, short destY, unsigned short destWidth,
    unsigned short destHeight, unsigned int flag);

//!
//! \brief   VA shim driver DeassociateSubpicture
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_DeassociateSubpicture (VADriverContextP ctx, VASubpictureID subpicture,
    VASurfaceID *targetSurface, int numSurface);

//!
//! \brief   VA shim driver LockSurface
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_LockSurface (VADriverContextP ctx, VASurfaceID surface, unsigned int *fourcc,
    unsigned int *lumaStride, unsigned int *chromaUStride, unsigned int *chromaVStride,
    unsigned int *lumaOffset, unsigned int *chromaUOffset, unsigned int *chromaVOffset,
    unsigned int *bufferName, void **buffer);

//!
//! \brief   VA shim driver UnlockSurface
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_UnlockSurface (VADriverContextP ctx, VASurfaceID surface);

//!
//! \brief   VA shim driver SetImagePalette
//! \return  VAStatus
//!          Return VA_STATUS_SUCCESS if success, else fail
//!
VAStatus HDDLVAShim_SetImagePalette (VADriverContextP ctx, VAImageID image,
    unsigned char *palette);
#endif

//EOF
