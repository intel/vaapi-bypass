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
//! \file    base_definition.h
//! \brief   Base header that provide declaration, definition for this project
//! \details Declaration & definitions can be accessed by all files within this project
//!

#ifndef __BASE_DEFINITION_H__
#define __BASE_DEFINITION_H__

//!#ifndef BASIC_TYPES_DEFINED
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h> // Unimplemented
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dlfcn.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

// Special function ID for Driver Init
// VTable ID used to identify VA backend function. This should reflect the order that VADriverVTable
// is defined in va_backend.h
typedef enum
{
    HDDLVAMedia_DriverInit,
    HDDLVATerminate,
    HDDLVAQueryConfigProfiles,
    HDDLVAQueryConfigEntrypoints,
    HDDLVAGetConfigAttributes,
    HDDLVACreateConfig,
    HDDLVADestroyConfig,
    HDDLVAQueryConfigAttributes,
    HDDLVACreateSurfaces,
    HDDLVADestroySurfaces,
    HDDLVACreateContext,
    HDDLVADestroyContext,
    HDDLVACreateBuffer,
    HDDLVABufferSetNumElements,
    HDDLVAMapBuffer,
    HDDLVAUnmapBuffer,
    HDDLVADestroyBuffer,
    HDDLVABeginPicture,
    HDDLVARenderPicture,
    HDDLVAEndPicture,
    HDDLVASyncSurface,
    HDDLVAQuerySurfaceStatus,
    HDDLVAQuerySurfaceError,         /* Unimplemented */
    HDDLVAPutSurface,                /* Unimplemented */
    HDDLVAQueryImageFormats,
    HDDLVACreateImage,
    HDDLVADeriveImage,
    HDDLVADestroyImage,
    HDDLVASetImagePalette,           /* Unimplemented */
    HDDLVAGetImage,
    HDDLVAPutImage,
    HDDLVAQuerySubpictureFormats,    /* Unimplemented */
    HDDLVACreateSubpicture,          /* Unimplemented */
    HDDLVADestroySubpicture,         /* Unimplemented */
    HDDLVASetSubpictureImage,        /* Unimplemented */
    HDDLVASetSubpictureChromakey,    /* Unimplemented */
    HDDLVASetSubpictureGlobalAlpha,  /* Unimplemented */
    HDDLVAAssociateSubpicture,       /* Unimplemented */
    HDDLVADeassociateSubpicture,     /* Unimplemented */
    HDDLVAQueryDisplayAttributes,
    HDDLVAGetDisplayAttributes,
    HDDLVASetDisplayAttributes,
    HDDLVALockSurface,               /* Unimplemented */
    HDDLVAUnlockSurface,             /* Unimplemented */
    HDDLVAGetSurfaceAttributes,      /* Deprecated */
    HDDLVAQuerySurfaceAttributes,
    HDDLVACreateSurfaces2,
    /* 0.36.0 */
    HDDLVAAcquireBufferHandle,
    HDDLVAReleaseBufferHandle,
    /* 1.0.0 */
    HDDLVAExportSurfaceHandle,
    HDDLVACreateBuffer2,
    /**************** SIDEBAND APIs **************/
    /* xlink channel*/
    HDDLDynamicChannelID,
    /* Batch Mode */
    HDDLTransferBatch,
    HDDLVAMaxFunctionID
}HDDLVAFunctionID;

typedef struct
{
    HDDLVAFunctionID vaFunctionID;
    uint32_t size;
}HDDLVAData;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
}HDDLVAMedia_DriverInitTX, HDDLVAInitializeTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    int version_major;
    int version_minor;
    int max_profiles;
    int max_entrypoints;
    int max_attributes;
    int max_image_formats;
    int max_subpic_formats;
    int max_display_attributes;
    char str_vendor[128];
    /* TODO: VADriverContextP is a pointer to VADriverContext
     * structure which consist of several pointers. We might want
     * to think of another way to store/transfer these information
     *
     * VADriverContextP ctx;
     */
}HDDLVAMedia_DriverInitRX, HDDLVAInitializeRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
}HDDLVATerminateTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVATerminateRX;

typedef struct
{
    HDDLVAData vaData;
    int numProfiles;
    VADriverContextP targetCtx;
}HDDLVAQueryConfigProfilesTX;


typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    int numProfile;
}HDDLVAQueryConfigProfilesRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    int numEntrypoint;
    VAProfile profile;
}HDDLVAQueryConfigEntrypointsTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    int numEntrypoint;
}HDDLVAQueryConfigEntrypointsRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAProfile profile;
    VAEntrypoint entrypoint;
    int numAttrib;
}HDDLVAGetConfigAttributesTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    int numAttrib;
}HDDLVAGetConfigAttributesRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAProfile profile;
    VAEntrypoint entrypoint;
    int numAttrib;
}HDDLVACreateConfigTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    VAConfigID configId;
}HDDLVACreateConfigRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAConfigID configId;
}HDDLVADestroyConfigTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVADestroyConfigRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAConfigID configId;
    int numAttributes;
}HDDLVAQueryConfigAttributesTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    VAProfile profile;
    VAEntrypoint entrypoint;
    int numAttrib;
}HDDLVAQueryConfigAttributesRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    int width;
    int height;
    int format;
    int numSurfaces;
}HDDLVACreateSurfacesTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVACreateSurfacesRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    int numSurfaces;
}HDDLVADestroySurfacesTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVADestroySurfacesRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAConfigID configId;
    int pictureWidth;
    int pictureHeight;
    int flag;
    int numRenderTarget;
}HDDLVACreateContextTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    VAContextID context;
}HDDLVACreateContextRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAContextID context;
}HDDLVADestroyContextTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVADestroyContextRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAContextID context;
    VABufferType type;
    unsigned int size;
    unsigned int numElement;
}HDDLVACreateBufferTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    VABufferID bufId;
}HDDLVACreateBufferRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VABufferID bufId;
    unsigned int numElement;
}HDDLVABufferSetNumElementsTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVABufferSetNumElementsRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VABufferID bufId;
    unsigned int dataSize;
    VABufferType bufType;
}HDDLVAMapBufferTX;

typedef struct
{
    HDDLVAData vaData;
    unsigned int segmentCount;
    unsigned int dataSize;
    VAStatus ret;
}HDDLVAMapBufferRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VABufferID bufId;
    VABufferType bufType;
}HDDLVAUnmapBufferTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVAUnmapBufferRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VABufferID bufId;
}HDDLVADestroyBufferTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVADestroyBufferRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAContextID context;
    VASurfaceID renderTarget;
}HDDLVABeginPictureTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVABeginPictureRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAContextID context;
    int numBuffer;
}HDDLVARenderPictureTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVARenderPictureRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAContextID context;
}HDDLVAEndPictureTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVAEndPictureRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VASurfaceID renderTarget;
}HDDLVASyncSurfaceTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVASyncSurfaceRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VASurfaceID renderTarget;
}HDDLVAQuerySurfaceStatusTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    VASurfaceStatus status;
}HDDLVAQuerySurfaceStatusRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    int numFormat;
}HDDLVAQueryImageFormatsTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    int numFormat;
}HDDLVAQueryImageFormatsRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAImageFormat format;
    int width;
    int height;
}HDDLVACreateImageTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    VAImage image;
}HDDLVACreateImageRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VASurfaceID surface;
}HDDLVADeriveImageTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    VAImage image;
}HDDLVADeriveImageRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAImageID image;
}HDDLVADestroyImageTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVADestroyImageRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAImageID image;
}HDDLVASetImagePaletteTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVASetImagePaletteRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VASurfaceID surface;
    int x;
    int y;
    unsigned int width;
    unsigned int height;
    VAImageID image;
}HDDLVAGetImageTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVAGetImageRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VASurfaceID surface;
    VAImageID image;
    int srcX;
    int srcY;
    unsigned int srcWidth;
    unsigned int srcHeight;
    int destX;
    int destY;
    unsigned int destWidth;
    unsigned int destHeight;
    // We need to include buffer information here in order to
    // obtain buffer content located in VAImage, which is being
    // internally map and unmap by the driver during vaGetImage call.
    VABufferID bufId;
    unsigned int bufSize;
}HDDLVAPutImageTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVAPutImageRX;

typedef struct
{
    HDDLVAData vaData;
    int numAttributes;
    VADriverContextP targetCtx;
}HDDLVAQueryDisplayAttributesTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    int numAttributes;
}HDDLVAQueryDisplayAttributesRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    int numAttributes;
}HDDLVAGetDisplayAttributesTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVAGetDisplayAttributesRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    int numAttributes;
}HDDLVASetDisplayAttributesTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVASetDisplayAttributesRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAConfigID config;
    unsigned int numAttribs;
}HDDLVAQuerySurfaceAttributesTX;

typedef struct
{
    HDDLVAData vaData;
    unsigned int numAttribs;
    VAStatus ret;
}HDDLVAQuerySurfaceAttributesRX;

typedef struct
{
    VASurfaceAttribExternalBuffers extBuf;
    uint64_t extbuf_handle[16];
}VASurfaceAttribExternalBuffersAndFdBuffer;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    unsigned int format;
    unsigned int width;
    unsigned int height;
    unsigned int numSurfaces;
    unsigned int numAttribs;
    VASurfaceAttribExternalBuffersAndFdBuffer externalBuffers[16];
}HDDLVACreateSurfaces2TX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLVACreateSurfaces2RX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VASurfaceID surfaceId;
    uint32_t memType;
    uint32_t flags;
}HDDLVAExportSurfaceHandleTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    VADRMPRIMESurfaceDescriptor descriptor;
}HDDLVAExportSurfaceHandleRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VAContextID context;
    VABufferType type;
    unsigned int width;
    unsigned int height;
}HDDLVACreateBuffer2TX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    unsigned int unitSize;
    unsigned int pitch;
    VABufferID bufId;
    unsigned int unitHeight;
}HDDLVACreateBuffer2RX;

typedef struct
{
   HDDLVAData vaData;
   VADriverContextP targetCtx;
   VABufferID bufId;
}HDDLVAAcquireBufferHandleTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    VABufferInfo bufInfo;
}HDDLVAAcquireBufferHandleRX;

typedef struct
{
    HDDLVAData vaData;
    VADriverContextP targetCtx;
    VABufferID bufId;
}HDDLVAReleaseBufferHandleTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
    VABufferInfo bufInfo;
}HDDLVAReleaseBufferHandleRX;

typedef struct
{
    HDDLVAData vaData;
    uint16_t channelTX;
    uint16_t channelRX;
}HDDLDynamicChannelTX;

typedef struct
{
    HDDLVAData vaData;
    VAStatus ret;
}HDDLDynamicChannelRX;

#endif

//EOF
