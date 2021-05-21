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
//! \file    host_va_shim.c
//! \brief   Basic vaFunction for Host IA
//! \details Receive/Return vaFunction with application & read/write va info with KMB Target
//!

#include "host_va_shim.h"
#define STR_VENDOR_MAX_STRLEN 200

#pragma pack(push, 1)

pthread_mutex_t gMutex;

VAStatus __vaDriverInit (VADriverContextP ctx)
{
    SHIM_PROFILE_INIT ();
    VAStatus vaStatus = VA_STATUS_SUCCESS;

    SHIM_CHK_NULL (ctx, "vaDriverInit ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    struct VADriverVTable *pVTable = ctx->vtable;

    /* Calls remote target KMB Driver Initialize function
    *  ctx is allocated in libVA. We cannot deference ctx in HDDL target. So ctx needed to be
    *  recreated in HDDL target. max_profiles, max_entrypoints, max_attributes, max_image_formats,
    *  max_subpic_formats, max_subpic_formats, max_display_attributes, str_vendor should be
    *  populated by HDDL target
    */

    vaStatus = HDDLVAShim_DriverInit (ctx);
    SHIM_CHK_NULL (ctx, "ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    pVTable->vaTerminate = HDDLVAShim_Terminate;
    pVTable->vaCreateConfig = HDDLVAShim_CreateConfig;
    pVTable->vaDestroyConfig = HDDLVAShim_DestroyConfig;
    pVTable->vaCreateSurfaces = HDDLVAShim_CreateSurfaces;
    pVTable->vaDestroySurfaces = HDDLVAShim_DestroySurfaces;
    pVTable->vaCreateContext = HDDLVAShim_CreateContext;
    pVTable->vaDestroyContext = HDDLVAShim_DestroyContext;

    pVTable->vaCreateBuffer = HDDLVAShim_CreateBuffer;
    pVTable->vaCreateBuffer2 = HDDLVAShim_CreateBuffer2;
    pVTable->vaMapBuffer = HDDLVAShim_MapBuffer;
    pVTable->vaUnmapBuffer = HDDLVAShim_UnmapBuffer;
    pVTable->vaDestroyBuffer = HDDLVAShim_DestroyBuffer;

    pVTable->vaCreateImage = HDDLVAShim_CreateImage;
    pVTable->vaDeriveImage = HDDLVAShim_DeriveImage;
    pVTable->vaDestroyImage = HDDLVAShim_DestroyImage;

    pVTable->vaBeginPicture = HDDLVAShim_BeginPicture;
    pVTable->vaRenderPicture = HDDLVAShim_RenderPicture;
    pVTable->vaEndPicture = HDDLVAShim_EndPicture;

    pVTable->vaSyncSurface = HDDLVAShim_SyncSurface;

    pVTable->vaQueryConfigProfiles = HDDLVAShim_QueryConfigProfiles;
    pVTable->vaQueryConfigEntrypoints = HDDLVAShim_QueryConfigEntrypoints;
    pVTable->vaQueryConfigAttributes = HDDLVAShim_QueryConfigAttributes;
    pVTable->vaGetConfigAttributes = HDDLVAShim_GetConfigAttributes;
    pVTable->vaQuerySurfaceStatus = HDDLVAShim_QuerySurfaceStatus;
    pVTable->vaQueryImageFormats = HDDLVAShim_QueryImageFormats;

    pVTable->vaGetImage = HDDLVAShim_GetImage;
    pVTable->vaPutImage = HDDLVAShim_PutImage;
    pVTable->vaQueryDisplayAttributes = HDDLVAShim_QueryDisplayAttributes;
    pVTable->vaGetDisplayAttributes = HDDLVAShim_GetDisplayAttributes;
    pVTable->vaSetDisplayAttributes = HDDLVAShim_SetDisplayAttributes;

    pVTable->vaQuerySurfaceAttributes = HDDLVAShim_QuerySurfaceAttributes;
    pVTable->vaCreateSurfaces2 = HDDLVAShim_CreateSurfaces2;
    pVTable->vaExportSurfaceHandle = HDDLVAShim_ExportSurfaceHandle;

    pVTable->vaAcquireBufferHandle = HDDLVAShim_AcquireBufferHandle;
    pVTable->vaReleaseBufferHandle = HDDLVAShim_ReleaseBufferHandle;

    pVTable->vaBufferSetNumElements = HDDLVAShim_BufferSetNumElements;

    // Unimplemented VA Function
    pVTable->vaQuerySubpictureFormats = HDDLVAShim_QuerySubpictureFormats;
    pVTable->vaPutSurface = HDDLVAShim_PutSurface;
    pVTable->vaQuerySubpictureFormats = HDDLVAShim_QuerySubpictureFormats;
    pVTable->vaCreateSubpicture = HDDLVAShim_CreateSubpicture;
    pVTable->vaDestroySubpicture = HDDLVAShim_DestroySubpicture;
    pVTable->vaSetSubpictureImage = HDDLVAShim_SetSubpictureImage;
    pVTable->vaSetSubpictureChromakey = HDDLVAShim_SetSubpictureChromakey;
    pVTable->vaSetSubpictureGlobalAlpha = HDDLVAShim_SetSubpictureGlobalAlpha;
    pVTable->vaAssociateSubpicture = HDDLVAShim_AssociateSubpicture;
    pVTable->vaDeassociateSubpicture = HDDLVAShim_DeassociateSubpicture;
    pVTable->vaLockSurface = HDDLVAShim_LockSurface;
    pVTable->vaUnlockSurface = HDDLVAShim_UnlockSurface;
    pVTable->vaSetImagePalette = HDDLVAShim_SetImagePalette;

    if ( (ctx->version_major != VA_MAJOR_VERSION) && (ctx->version_minor != VA_MINOR_VERSION))
    {
         SHIM_ERROR_MESSAGE ("VA version installed on Host not compatible");
         return VA_STATUS_ERROR_UNKNOWN;
    }

    return vaStatus;
}

// Get VA_MAJOR_VERSION and VA_MINOR_VERSION from libva
// To form the function name in the format of _vaDriverInit_[VA_MAJOR_VERSION]_[VA_MINOR_VERSION]
#define VA_DRV_INIT_DEF(_major,_minor)                       \
    __vaDriverInit_##_major##_##_minor

#define VA_DRV_INIT_FUNC(va_major_version, va_minor_version) \
    VA_DRV_INIT_DEF(va_major_version,va_minor_version)

#define VA_DRV_INIT_FUC_NAME                                 \
    VA_DRV_INIT_FUNC(VA_MAJOR_VERSION,VA_MINOR_VERSION)

#define HDDL_EXPORT                                          \
    __attribute__((visibility("default")))

HDDL_EXPORT VAStatus VA_DRV_INIT_FUC_NAME (VADriverContextP ctx)
{
    return __vaDriverInit(ctx);
}

VAStatus HDDLVAShim_DriverInit (VADriverContextP ctx)
{
    HDDLVAMedia_DriverInitTX vaDataTX;
    HDDLVAMedia_DriverInitRX vaDataRX;
    HDDLVAShimDriverContext *vaShimCtx;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLThreadMgr_LockMutex (&gMutex);

    vaShimCtx = HDDL_GetVAShimContext (ctx);

    // Multiple entry
    if (vaShimCtx)
    {
        vaShimCtx->uiRefCount++;
        HDDLThreadMgr_UnlockMutex (&gMutex);
        return VA_STATUS_SUCCESS;
    }

    vaShimCtx = HDDLMemoryMgr_AllocAndZeroMemory (sizeof (HDDLVAShimDriverContext));
    SHIM_CHK_NULL (vaShimCtx, "Failed to create SHIM context",
        VA_STATUS_ERROR_ALLOCATION_FAILED);

    commCtx = HDDLVAShim_CommContextSetup (vaShimCtx, getpid (), syscall (SYS_gettid),
        MAIN_COMM_CONTEXT);
    if (commCtx == NULL)
    {
        HDDLMemoryMgr_FreeMemory (vaShimCtx);
        HDDLThreadMgr_UnlockMutex (&gMutex);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaShimCtx->uiRefCount++;

    // Construct the HDDLVAData structure to send
    vaDataTX.vaData.vaFunctionID = HDDLVAMedia_DriverInit;
    vaDataTX.vaData.size = sizeof (HDDLVAMedia_DriverInitTX);

    commStatus = Comm_Submission (commCtx, HDDLVAMedia_DriverInit, COMM_READ_FULL,
        sizeof (HDDLVAMedia_DriverInitTX), (void *)&vaDataTX,
        sizeof (HDDLVAMedia_DriverInitRX), (void **)&vaDataRX);

    if (commStatus != COMM_STATUS_SUCCESS)
    {
	Comm_MutexDestroy (commCtx);
        Comm_CloseSocket (commCtx, HOST);
        HDDLMemoryMgr_FreeMemory (commCtx);
        HDDLMemoryMgr_FreeMemory (vaShimCtx);
        HDDLThreadMgr_UnlockMutex (&gMutex);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    // Returned function ID and size needed to match
    if ( (vaDataRX.vaData.vaFunctionID != HDDLVAMedia_DriverInit) ||
        (vaDataRX.vaData.size != sizeof (HDDLVAMedia_DriverInitRX)))
    {
	Comm_MutexDestroy (commCtx);
        Comm_CloseSocket (commCtx, HOST);
        HDDLMemoryMgr_FreeMemory (commCtx);
        HDDLMemoryMgr_FreeMemory (vaShimCtx);
        HDDLThreadMgr_UnlockMutex (&gMutex);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    // Copy returned data to ctx
    ctx->version_major = vaDataRX.version_major;
    ctx->version_minor = vaDataRX.version_minor;
    ctx->max_profiles = vaDataRX.max_profiles;
    ctx->max_entrypoints = vaDataRX.max_entrypoints;
    ctx->max_attributes = vaDataRX.max_attributes;
    ctx->max_image_formats = vaDataRX.max_image_formats;
    ctx->max_subpic_formats = vaDataRX.max_subpic_formats;
    ctx->max_display_attributes = vaDataRX.max_display_attributes;
    ctx->str_vendor = HDDLMemoryMgr_AllocAndZeroMemory (strnlen (vaDataRX.str_vendor,
        STR_VENDOR_MAX_STRLEN) + 1);
    HDDLMemoryMgr_Memcpy ( (void *)ctx->str_vendor, &vaDataRX.str_vendor,
        strnlen (vaDataRX.str_vendor, STR_VENDOR_MAX_STRLEN),
	strnlen (vaDataRX.str_vendor, STR_VENDOR_MAX_STRLEN));

    // Init heap
    HDDLVAShim_HeapInit (vaShimCtx);

    // Init mutexs
    HDDLThreadMgr_InitMutex (&vaShimCtx->bufferMutex);
    HDDLThreadMgr_InitMutex (&vaShimCtx->imageMutex);
    HDDLThreadMgr_InitMutex (&vaShimCtx->contextMutex);

    ctx->pDriverData = vaShimCtx;

    HDDLThreadMgr_UnlockMutex (&gMutex);

    SHIM_FUNCTION_EXIT ();
    return VA_STATUS_SUCCESS;
}

VAStatus HDDLVAShim_Terminate (VADriverContextP ctx)
{
    HDDLVATerminateTX vaDataTX;
    HDDLVATerminateRX vaDataRX;
    HDDLVAShimDriverContext *vaShimCtx;
    HDDLShimCommContext *commCtx;
    VAStatus vaStatus;
    CommStatus commStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLThreadMgr_LockMutex (&gMutex);

    vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "VAShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    if (vaShimCtx->uiRefCount > 1)
    {
        vaShimCtx->uiRefCount--;
        HDDLThreadMgr_UnlockMutex (&gMutex);
        return VA_STATUS_SUCCESS;
    }

    // Construct the HDDLVAData structure to send
    vaDataTX.vaData.vaFunctionID = HDDLVATerminate;
    vaDataTX.vaData.size = sizeof (HDDLVATerminateTX);

    commStatus = Comm_Submission (commCtx, HDDLVATerminate, COMM_READ_FULL,
        sizeof (HDDLVATerminateTX), (void *)&vaDataTX,
        sizeof (HDDLVATerminateRX), (void **)&vaDataRX);

    if (commStatus != COMM_STATUS_SUCCESS)
    {
        HDDLThreadMgr_UnlockMutex (&gMutex);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    // Returned function ID and size needed to match
    if ( (vaDataRX.vaData.vaFunctionID != HDDLVATerminate) ||
        (vaDataRX.vaData.size != sizeof (HDDLVATerminateRX)))
    {
        HDDLThreadMgr_UnlockMutex (&gMutex);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;
    if (vaStatus != VA_STATUS_SUCCESS)
    {
        HDDLThreadMgr_UnlockMutex (&gMutex);
        return vaStatus;
    }

    // Destroy resources
    HDDLVAShim_DestroyBufferHeap (ctx);
    HDDLVAShim_DestroyImageHeap (ctx);
    HDDLVAShim_DestroyContextHeap (ctx);

    commStatus = Comm_Disconnect(commCtx, HOST);
    SHIM_CHK_ERROR(commStatus, "Error to Disconnect", VA_STATUS_ERROR_UNKNOWN);

    // Destroy pointer to heap
    if (vaShimCtx->bufferHeap->pHeapBase)
    {
        HDDLMemoryMgr_FreeMemory (vaShimCtx->bufferHeap->pHeapBase);
    }

    if (vaShimCtx->imageHeap->pHeapBase)
    {
        HDDLMemoryMgr_FreeMemory (vaShimCtx->imageHeap->pHeapBase);
    }

    if (vaShimCtx->contextHeap->pHeapBase)
    {
        HDDLMemoryMgr_FreeMemory (vaShimCtx->contextHeap->pHeapBase);
    }

    // Destroy/Free the heap
    HDDLMemoryMgr_FreeMemory (vaShimCtx->bufferHeap);
    HDDLMemoryMgr_FreeMemory (vaShimCtx->imageHeap);
    HDDLMemoryMgr_FreeMemory (vaShimCtx->contextHeap);

    // Destroy the mutexs
    HDDLThreadMgr_DestroyMutex (&vaShimCtx->bufferMutex);
    HDDLThreadMgr_DestroyMutex (&vaShimCtx->imageMutex);
    HDDLThreadMgr_DestroyMutex (&vaShimCtx->contextMutex);

    // Resource check
    if (vaShimCtx->uiNumBuffer != 0)
    {
        SHIM_ASSERT_MESSAGE ("app does not destroy all buffers");
    }

    if (vaShimCtx->uiNumImage != 0)
    {
        SHIM_ASSERT_MESSAGE ("app does not destroy all images");
    }

    if (vaShimCtx->uiNumContext != 0)
    {
        SHIM_ASSERT_MESSAGE ("app does not destroy all context");
    }

    vaShimCtx->uiRefCount--;

    if (IS_BATCH (commCtx))
    {
        HDDLMemoryMgr_FreeMemory (commCtx->batchPayload);
    }

    HDDLMemoryMgr_FreeMemory (commCtx);
    HDDLMemoryMgr_FreeMemory (vaShimCtx);
    HDDLMemoryMgr_FreeMemory ( (void*)ctx->str_vendor);

    HDDLThreadMgr_UnlockMutex (&gMutex);

    SHIM_FUNCTION_EXIT ();
    SHIM_PROFILE_TERMINATE ();

    return VA_STATUS_SUCCESS;
}

VAStatus HDDLVAShim_CreateConfig (VADriverContextP ctx, VAProfile profile, VAEntrypoint entrypoint,
    VAConfigAttrib *attrib_list, int numAttrib, VAConfigID *configId)
{
    HDDLVACreateConfigRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "VAShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    /* Locally create a vaDataTXFull structure to concatenate HDDLVACreateConfigTX
    *  and the actual attribute list to be sent to target.
    *  Target has to locally define the full local struct based on the size received to
    *  extract proper HDDLVACreateConfigTX and VAConfigAttrib data
    */
    typedef struct {
        HDDLVACreateConfigTX vaDataTX;
        VAConfigAttrib attribList[numAttrib];
    }HDDLVADataFullTX;

    HDDLVADataFullTX vaDataFullTX;

    // Construct the HDDLVAData structure to send
    vaDataFullTX.vaDataTX.vaData.vaFunctionID = HDDLVACreateConfig;
    vaDataFullTX.vaDataTX.vaData.size = sizeof (HDDLVADataFullTX);
    vaDataFullTX.vaDataTX.profile = profile;
    vaDataFullTX.vaDataTX.entrypoint = entrypoint;
    vaDataFullTX.vaDataTX.numAttrib = numAttrib;

    // Copy the actual attribute list
    HDDLMemoryMgr_Memcpy (vaDataFullTX.attribList, attrib_list, sizeof (vaDataFullTX.attribList),
        sizeof (VAConfigAttrib) * numAttrib);

    commStatus = Comm_Submission (commCtx, HDDLVACreateConfig, COMM_READ_FULL,
        sizeof (HDDLVADataFullTX), (void *)&vaDataFullTX,
        sizeof (HDDLVACreateConfigRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN)

    // Returned function ID and size needed to match
    if ( (vaDataRX.vaData.vaFunctionID != HDDLVACreateConfig) ||
        (vaDataRX.vaData.size != sizeof (HDDLVACreateConfigRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;
    *configId = vaDataRX.configId;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_DestroyConfig (VADriverContextP ctx, VAConfigID configId)
{
    HDDLVADestroyConfigTX vaDataTX;
    HDDLVADestroyConfigRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "VAShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    // Construct the HDDLVAData structure to send
    vaDataTX.vaData.vaFunctionID = HDDLVADestroyConfig;
    vaDataTX.vaData.size = sizeof (HDDLVADestroyConfigTX);
    vaDataTX.configId = configId;

    commStatus = Comm_Submission (commCtx, HDDLVADestroyConfig, COMM_READ_FULL,
        sizeof (HDDLVADestroyConfigTX), (void *)&vaDataTX,
        sizeof (HDDLVADestroyConfigRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    // Returned function ID and size needed to match
    if ( (vaDataRX.vaData.vaFunctionID != HDDLVADestroyConfig) ||
        (vaDataRX.vaData.size != sizeof (HDDLVADestroyConfigRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_CreateContext (VADriverContextP ctx, VAConfigID configId,
    int pictureWidth, int pictureHeight, int flag, VASurfaceID *render_targets,
    int numRenderTarget, VAContextID *context)
{
    HDDLVACreateContextRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "VAShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    typedef struct {
        HDDLVACreateContextTX vaDataTX;
        VASurfaceID renderTargets[numRenderTarget];
    }HDDLVADataFullTX;

    HDDLVADataFullTX vaDataFullTX;

    // Construct a the vaData structure to send
    vaDataFullTX.vaDataTX.vaData.vaFunctionID = HDDLVACreateContext;
    vaDataFullTX.vaDataTX.vaData.size = sizeof (HDDLVADataFullTX);
    vaDataFullTX.vaDataTX.configId = configId;
    vaDataFullTX.vaDataTX.pictureWidth = pictureWidth;
    vaDataFullTX.vaDataTX.pictureHeight = pictureHeight;
    vaDataFullTX.vaDataTX.flag = flag;
    vaDataFullTX.vaDataTX.numRenderTarget = numRenderTarget;

    // Copy the render_target
    HDDLMemoryMgr_Memcpy (vaDataFullTX.renderTargets, render_targets,
        sizeof (vaDataFullTX.renderTargets), sizeof (VASurfaceID) * numRenderTarget);

    commStatus = Comm_Submission (commCtx, HDDLVACreateContext, COMM_READ_FULL,
        sizeof (HDDLVADataFullTX), (void *)&vaDataFullTX,
        sizeof (HDDLVACreateContextRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    // Returned function ID and size needed to match
    if ( (vaDataRX.vaData.vaFunctionID != HDDLVACreateContext) ||
        (vaDataRX.vaData.size != sizeof (HDDLVACreateContextRX))
    )
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;
    *context = vaDataRX.context; // return VAContextID

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_DestroyContext (VADriverContextP ctx, VAContextID context)
{
    HDDLVADestroyContextTX vaDataTX;
    HDDLVADestroyContextRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "VAShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    // Construct the HDDLVAData structure to send
    vaDataTX.vaData.vaFunctionID = HDDLVADestroyContext;
    vaDataTX.vaData.size = sizeof (HDDLVADestroyContextTX);
    vaDataTX.context = context;

    commStatus = Comm_Submission (commCtx, HDDLVADestroyContext, COMM_READ_FULL,
        sizeof (HDDLVADestroyContextTX), (void *)&vaDataTX,
        sizeof (HDDLVADestroyContextRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    // Returned function ID and size needed to match
    if ( (vaDataRX.vaData.vaFunctionID != HDDLVADestroyContext) ||
        (vaDataRX.vaData.size != sizeof (HDDLVADestroyContextRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_CreateSurfaces (VADriverContextP ctx, int width, int height, int format,
    int	numSurfaces, VASurfaceID *surfaces)
{
    HDDLVACreateSurfacesTX vaDataTX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (ctx, "vaShimCtx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    // Construct HDDLVAData structure to send
    vaDataTX.vaData.vaFunctionID = HDDLVACreateSurfaces;
    vaDataTX.vaData.size = sizeof (HDDLVACreateSurfacesTX);
    vaDataTX.width = width;
    vaDataTX.height = height;
    vaDataTX.format = format;
    vaDataTX.numSurfaces = numSurfaces;

    typedef struct {
        HDDLVACreateSurfacesRX vaDataRX;
        VASurfaceID surfaces[numSurfaces];
    }HDDLVADataFullRX;

    HDDLVADataFullRX vaDataFullRX;

    commStatus = Comm_Submission (commCtx, HDDLVACreateSurfaces, COMM_READ_FULL,
        sizeof (HDDLVACreateSurfacesTX), (void *)&vaDataTX,
        sizeof (HDDLVADataFullRX), (void **)&vaDataFullRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    // Returned function ID and size needed to match
    if ( (vaDataFullRX.vaDataRX.vaData.vaFunctionID != HDDLVACreateSurfaces) ||
        (vaDataFullRX.vaDataRX.vaData.size != sizeof (HDDLVADataFullRX))
    )
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataFullRX.vaDataRX.ret;
    HDDLMemoryMgr_Memcpy (surfaces, vaDataFullRX.surfaces, sizeof (VASurfaceID) * numSurfaces,
        sizeof (vaDataFullRX.surfaces));

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_DestroySurfaces (VADriverContextP ctx, VASurfaceID *surfaceList,
    int numSurfaces)
{
    HDDLVADestroySurfacesRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "VAShimCtx return NULL ptr", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    // Create a full vaDataTXFull struct to get the surface list to be sent to target
    typedef struct {
        HDDLVADestroySurfacesTX vaDataTX;
        VASurfaceID surfaces[numSurfaces];
    }HDDLVADataFullTX;
    HDDLVADataFullTX vaDataFullTX;

    // Construct the HDDLVAData structure to send
    vaDataFullTX.vaDataTX.vaData.vaFunctionID = HDDLVADestroySurfaces;
    vaDataFullTX.vaDataTX.vaData.size = sizeof (HDDLVADataFullTX);
    vaDataFullTX.vaDataTX.numSurfaces = numSurfaces;

    // Copy the surface list
    HDDLMemoryMgr_Memcpy (vaDataFullTX.surfaces, surfaceList, sizeof (vaDataFullTX.surfaces),
        sizeof (VASurfaceID) * numSurfaces);

    commStatus = Comm_Submission (commCtx, HDDLVADestroySurfaces, COMM_READ_FULL,
        sizeof (HDDLVADataFullTX), (void *)&vaDataFullTX,
        sizeof (HDDLVADestroySurfacesRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    // Returned function ID and size needed to match
    if ( (vaDataRX.vaData.vaFunctionID != HDDLVADestroySurfaces) ||
        (vaDataRX.vaData.size != sizeof (HDDLVADestroySurfacesRX))
    )
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_CreateBuffer (VADriverContextP ctx, VAContextID context, VABufferType type,
    unsigned int size, unsigned int numElement, void *data, VABufferID *bufId)
{
    HDDLVACreateBufferRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;
    unsigned int bufferSize = size * numElement;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "nullptr ctx", VA_STATUS_ERROR_INVALID_CONTEXT);

    SHIM_CHK_ZERO (size, "0 size", VA_STATUS_ERROR_INVALID_BUFFER);
    SHIM_CHK_ZERO (numElement, "0 num elements", VA_STATUS_ERROR_INVALID_BUFFER);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    if (data == NULL)
    {
        bufferSize = 0;
    }

    typedef struct {
        HDDLVACreateBufferTX vaDataTX;
        unsigned char data[bufferSize];
    }HDDLVADataFullTX;


    HDDLVADataFullTX vaDataFullTX;
    vaDataFullTX.vaDataTX.vaData.vaFunctionID = HDDLVACreateBuffer;
    vaDataFullTX.vaDataTX.vaData.size = sizeof (HDDLVADataFullTX);
    vaDataFullTX.vaDataTX.context = context;
    vaDataFullTX.vaDataTX.type = type;
    vaDataFullTX.vaDataTX.size = size;
    vaDataFullTX.vaDataTX.numElement = numElement;

    // Data might be NULL. Copy data only when it's not null to avoid segfault
    if (data)
    {
        HDDLMemoryMgr_Memcpy (vaDataFullTX.data, data, sizeof (vaDataFullTX.data),
            sizeof (unsigned char) * numElement * size);
    }

    commStatus = Comm_Submission (commCtx, HDDLVACreateBuffer, COMM_READ_FULL,
        sizeof (HDDLVADataFullTX), (void *)&vaDataFullTX,
        sizeof (HDDLVACreateBufferRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVACreateBuffer) ||
        (vaDataRX.vaData.size != sizeof (HDDLVACreateBufferRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;
    SHIM_CHK_ERROR (vaStatus, "VA status failed", VA_STATUS_ERROR_UNKNOWN);

    // Get VABufferID from target
    *bufId = vaDataRX.bufId;

    vaStatus = HDDLVAShim_CreateInternalBufferAtHeap (vaShimCtx, context, type, size, numElement,
        *bufId, (void*)data);

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_DestroyBuffer (VADriverContextP ctx, VABufferID bufId)
{
    HDDLVADestroyBufferTX vaDataTX;
    HDDLVADestroyBufferRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "nullptr ctx", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);
    SHIM_CHK_NULL (vaShimCtx->bufferHeap, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    vaDataTX.vaData.vaFunctionID = HDDLVADestroyBuffer;
    vaDataTX.vaData.size = sizeof (HDDLVADestroyBufferTX);
    vaDataTX.bufId = bufId;

    vaStatus = HDDLVAShim_ReleaseInternalBufferFromHeap (vaShimCtx, bufId);
    SHIM_CHK_ERROR (vaStatus, "VA status failed", VA_STATUS_ERROR_UNKNOWN);

    commStatus = Comm_Submission (commCtx, HDDLVADestroyBuffer, COMM_READ_FULL,
        sizeof (HDDLVADestroyBufferTX), (void *)&vaDataTX,
        sizeof (HDDLVADestroyBufferRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVADestroyBuffer) ||
        (vaDataRX.vaData.size != sizeof (HDDLVADestroyBufferRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_MapBuffer (VADriverContextP ctx, VABufferID bufId, void **buf)
{
    HDDLVAMapBufferTX vaDataTX;
    HDDLVAMapBufferRX vaDataRX;
    HDDLShimCommContext *commCtx;
    VAStatus vaStatus;
    HDDLVABuffer *vaBuffer;
    uint32_t hostBufId;
    unsigned int dataSize;
    void *peekData = NULL;
    uint32_t peekSize = sizeof (HDDLVAMapBufferRX);
    uint32_t fullRXSize = 0;
    CommMode commMode = COMM_MODE_UNKNOWN;
    CommStatus commStatus = COMM_STATUS_FAILED;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "nullptr ctx", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext * vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);
    SHIM_CHK_NULL (vaShimCtx->bufferHeap, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commMode = COMM_MODE (commCtx);
    SHIM_CHK_EQUAL (commMode, COMM_MODE_UNKNOWN, "Invalid commumication mode",
        VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLThreadMgr_LockMutex (&vaShimCtx->bufferMutex);

    vaBuffer = HDDLMemoryMgr_GetBufferFromVABufferID (vaShimCtx, bufId, &hostBufId);
    if (vaBuffer == NULL)
    {
        SHIM_ERROR_MESSAGE ("vaBuffer returned NULL");
        HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
        return VA_STATUS_ERROR_INVALID_CONTEXT;
    }

    // We need to perform the 'real' vaMapBuffer call if it's
    // 1. VAImageBufferType which contains the actual decoded data return from target
    // 2. VAEncCodedBufferType which contains the actual encoded data return from target
    // For the rest of the buffer types which the data normally being altered by the user, we
    // could defer the data updates to target until user calls vaUnmapBuffer.
    if (vaBuffer->type == VAImageBufferType || vaBuffer->type == VAEncCodedBufferType)
    {
        dataSize = vaBuffer->uiSize * vaBuffer->uiNumElement;

        vaDataTX.vaData.vaFunctionID = HDDLVAMapBuffer;
        vaDataTX.vaData.size = sizeof (HDDLVAMapBufferTX);
        vaDataTX.bufId = bufId;
        vaDataTX.dataSize = dataSize;
        vaDataTX.bufType = vaBuffer->type;

	if (vaBuffer->type == VAImageBufferType)
	{
            typedef struct {
                HDDLVAMapBufferRX vaDataRX;
                unsigned char data[dataSize];
            }HDDLVADataFullRX;

            // VAImageBufferType data might be huge, depending on decoded source.
            // Allocate the data on heap instead of stack to avoid too large
            // array allocated on the stack that might eventually causing some
            // issue
            HDDLVADataFullRX *vaDataFullRX = HDDLMemoryMgr_AllocMemory (sizeof (HDDLVADataFullRX));

	    if (vaDataFullRX == NULL)
	    {
                SHIM_ERROR_MESSAGE ("vaDataFullRX returned NULL");
		HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
		return VA_STATUS_ERROR_UNKNOWN;
	    }

            commStatus = Comm_Submission (commCtx, HDDLVAMapBuffer, COMM_READ_FULL,
                sizeof (HDDLVAMapBufferTX), (void *)&vaDataTX,
                sizeof (HDDLVADataFullRX), (void **)vaDataFullRX);

            if (commStatus != COMM_STATUS_SUCCESS)
            {
                HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
                return VA_STATUS_ERROR_UNKNOWN;
            }

            if ( (vaDataFullRX->vaDataRX.vaData.vaFunctionID != HDDLVAMapBuffer) ||
                (vaDataFullRX->vaDataRX.vaData.size != sizeof (HDDLVADataFullRX)))
            {
                HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
                return VA_STATUS_ERROR_UNKNOWN;
            }

            vaStatus = vaDataFullRX->vaDataRX.ret;
            if (vaStatus != VA_STATUS_SUCCESS)
            {
                HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
                return vaStatus;
            }

            HDDLMemoryMgr_Memcpy (vaBuffer->pData, vaDataFullRX->data,
	        vaBuffer->uiSize * vaBuffer->uiNumElement, dataSize);

            HDDLMemoryMgr_FreeMemory (vaDataFullRX);
	}
	else if (vaBuffer->type == VAEncCodedBufferType)
	{
            unsigned int offset = 0;

            if (commMode == COMM_MODE_TCP)
            {
                peekData = &vaDataRX;
            }

            commStatus = Comm_Submission (commCtx, HDDLVAMapBuffer,
                COMM_READ_PARTIAL, sizeof (HDDLVAMapBufferTX), (void *)&vaDataTX,
                peekSize, (void **)&peekData);

            if (commStatus != COMM_STATUS_SUCCESS)
            {
                HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
		HDDLMemoryMgr_FreeMemory (peekData);
                return VA_STATUS_ERROR_UNKNOWN;
            }

            if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
            {
	        vaDataRX = *(HDDLVAMapBufferRX *)peekData;
            }

            typedef struct {
                HDDLVAMapBufferRX vaDataRX;
                VACodedBufferSegment segment[vaDataRX.segmentCount];
                unsigned char data[vaDataRX.dataSize];
            }HDDLVADataFullRX;
            HDDLVADataFullRX *vaDataFullRX;

            fullRXSize = sizeof (HDDLVADataFullRX);
            commStatus = COMM_STATUS_FAILED;

            if (commMode == COMM_MODE_TCP)
            {
                vaDataFullRX = HDDLMemoryMgr_AllocMemory (fullRXSize);

		if (vaDataFullRX == NULL)
		{
                    SHIM_ERROR_MESSAGE ("vaDataFullRX returned NULL");
		    HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
		    return VA_STATUS_ERROR_UNKNOWN;
		}

                commStatus = Comm_Read (commCtx, fullRXSize, (void *)vaDataFullRX);
            }
            else if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
            {
                vaDataFullRX = (HDDLVADataFullRX *)peekData;

                if (fullRXSize > DATA_MAX_SEND_SIZE)
                {
                    vaDataFullRX = HDDLMemoryMgr_ReallocMemory (vaDataFullRX, fullRXSize);

                    commStatus = Comm_Read (commCtx, fullRXSize - DATA_MAX_SEND_SIZE,
                        (char *) (vaDataFullRX) + DATA_MAX_SEND_SIZE);
                }
                else
                {
                    commStatus = COMM_STATUS_SUCCESS;
                }
            }

            if (commStatus != COMM_STATUS_SUCCESS)
            {
                HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
                return VA_STATUS_ERROR_UNKNOWN;
            }

            if ( (vaDataFullRX->vaDataRX.vaData.vaFunctionID != HDDLVAMapBuffer) ||
                (vaDataFullRX->vaDataRX.vaData.size != sizeof (HDDLVADataFullRX)))
            {
                HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
		HDDLMemoryMgr_FreeMemory (peekData);
                return VA_STATUS_ERROR_UNKNOWN;
            }

            vaStatus = vaDataFullRX->vaDataRX.ret;
            if (vaStatus != VA_STATUS_SUCCESS)
            {
                HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
		HDDLMemoryMgr_FreeMemory (peekData);
                return vaStatus;
            }

            if (vaBuffer->pData)
            {
                HDDLVAShim_DestroyInternalVAEncCodedBuffer (vaBuffer);
            }

            // Reconstruct the linked list of VACodedBufferSegment and related buffer
            // content before returning as buffer content to the caller of vaMapBuffer.
            VACodedBufferSegment * prev = NULL;
            for (int i = 0; i < vaDataRX.segmentCount; i++)
            {
                VACodedBufferSegment *segment = HDDLMemoryMgr_AllocMemory (
                    sizeof (VACodedBufferSegment));

		if (segment == NULL)
		{
	            SHIM_ERROR_MESSAGE ("segment returned NULL");
		    HDDLMemoryMgr_FreeMemory (peekData);
		    HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
		    return VA_STATUS_ERROR_UNKNOWN;
		}

                HDDLMemoryMgr_Memcpy (segment, (void *) (vaDataFullRX->segment + i),
                    sizeof (VACodedBufferSegment), sizeof (vaDataFullRX->segment));

                unsigned char *data = HDDLMemoryMgr_AllocMemory (
                    sizeof (unsigned char) * segment->size);
                HDDLMemoryMgr_Memcpy (data, (void *) (vaDataFullRX->data + offset),
		    sizeof (unsigned char) * segment->size, sizeof (vaDataFullRX->data));

                offset += segment->size;
                segment->next = NULL;
                segment->buf = data;

                // VACodedBufferSegment received from EVM contains a 'next' ptr  which points to
                // the virt addr on ARM. We have to redirect it to host virt addr
                if(i == 0)
                {
                    vaBuffer->pData = segment;
                }
                else
                {
                    prev->next = segment;
                }
                prev = segment;
            }

	    HDDLMemoryMgr_FreeMemory (vaDataFullRX);
        }
    }

    vaStatus = HDDLVAShim_MapInternalBuffer (vaShimCtx, bufId, vaBuffer, buf);

    HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_UnmapBuffer (VADriverContextP ctx, VABufferID bufId)
{
    HDDLVAUnmapBufferRX vaDataRX;
    HDDLShimCommContext *commCtx;
    HDDLVABuffer *vaBuffer;
    uint32_t hostBufId;
    unsigned int dataSize;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "nullptr ctx", VA_STATUS_ERROR_INVALID_CONTEXT);

    if (bufId <= 0)
    {
        SHIM_ERROR_MESSAGE ("Invalid buffer id %d", bufId);
        return VA_STATUS_ERROR_INVALID_BUFFER;
    }

    HDDLVAShimDriverContext * vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);
    SHIM_CHK_NULL (vaShimCtx->bufferHeap, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLThreadMgr_LockMutex (&vaShimCtx->bufferMutex);

    vaBuffer = HDDLMemoryMgr_GetBufferFromVABufferID (vaShimCtx, bufId, &hostBufId);
    if (NULL == vaBuffer)
    {
        HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
        return VA_STATUS_ERROR_ALLOCATION_FAILED;
    }

    // We need to copy the modified buffer content and update on target side on vaMapBuffer
    // call. VAEncCodedBufferType is an exception as it is the driver encoded buffer output
    // instead of input buffer that might be updated or modified by the user. Thus unmap
    // internal buffer in local heap and return.
    if (vaBuffer->type == VAEncCodedBufferType)
    {
        vaStatus = HDDLVAShim_UnmapInternalBuffer (vaShimCtx, bufId, vaBuffer);
        if (vaStatus != VA_STATUS_SUCCESS)
        {
            SHIM_ERROR_MESSAGE ("Failed to unmap buffer");
            HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
            return VA_STATUS_ERROR_INVALID_BUFFER;
        }

        HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
        return vaStatus;
    }
    else if (vaBuffer->type == VAProcPipelineParameterBufferType)
    {
        VAProcPipelineParameterBuffer *pipelineParam = (VAProcPipelineParameterBuffer *)vaBuffer->pData;
        uint32_t numAdditionalOutputs = pipelineParam->num_additional_outputs;

        dataSize = vaBuffer->uiSize * vaBuffer->uiNumElement +
            (sizeof (VARectangle) * numAdditionalOutputs) +
	    (sizeof (VASurfaceID) * numAdditionalOutputs);
    }
    else
    {
        dataSize = vaBuffer->uiSize * vaBuffer->uiNumElement;
    }

    typedef struct {
        HDDLVAUnmapBufferTX vaDataTX;
        unsigned char data[dataSize];
    }HDDLVADataFullTX;

    HDDLVADataFullTX *vaDataFullTX = HDDLMemoryMgr_AllocMemory (sizeof (HDDLVADataFullTX));

    if (vaDataFullTX == NULL)
    {
        SHIM_ERROR_MESSAGE ("vaDataFullTX returned NULL");
	HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
	return VA_STATUS_ERROR_UNKNOWN;
    }

    HDDLMemoryMgr_ZeroMemory (vaDataFullTX->data, sizeof (unsigned char) * dataSize);

    vaDataFullTX->vaDataTX.vaData.vaFunctionID = HDDLVAUnmapBuffer;
    vaDataFullTX->vaDataTX.vaData.size = sizeof (HDDLVADataFullTX);
    vaDataFullTX->vaDataTX.bufId = bufId;
    vaDataFullTX->vaDataTX.bufType = vaBuffer->type;

    if (vaBuffer->type == VAEncMiscParameterBufferType)
    {
        VAEncMiscParameterBuffer *misc_param = (VAEncMiscParameterBuffer *)vaBuffer->pData;
        unsigned int offset = sizeof (VAEncMiscParameterBuffer);
        ( (VAEncMiscParameterBuffer *)vaDataFullTX->data)->type = misc_param->type;

        switch ( (int)misc_param->type)
        {
            case VAEncMiscParameterTypeROI:
            {
                VAEncMiscParameterBufferROI *misc_roi_param = (VAEncMiscParameterBufferROI *)misc_param->data;
                VAEncROI *region_roi = (VAEncROI *)misc_roi_param->roi;

                HDDLMemoryMgr_Memcpy ( (void *) (vaDataFullTX->data + offset), misc_roi_param,
		    sizeof (vaDataFullTX->data) - offset, sizeof (VAEncMiscParameterBufferROI));

                offset += sizeof (VAEncMiscParameterBufferROI);

                HDDLMemoryMgr_Memcpy ( (void *) (vaDataFullTX->data + offset), region_roi,
		    sizeof (vaDataFullTX->data) - offset, sizeof (VAEncROI));
                break;
            }

#ifdef USE_HANTRO
            case HANTROEncMiscParameterTypeEmbeddedPreprocess:
            {
                HANTROEncMiscParameterBufferEmbeddedPreprocess *misc_prp_param =
                    (HANTROEncMiscParameterBufferEmbeddedPreprocess *)misc_param->data;

                HDDLMemoryMgr_Memcpy ( (void *) (vaDataFullTX->data + offset), misc_prp_param,
                    sizeof (vaDataFullTX->data) - offset,
		    sizeof (HANTROEncMiscParameterBufferEmbeddedPreprocess));
                break;
            }
            case HANTROEncMiscParameterTypeROI:
            {
                HANTROEncMiscParameterBufferROI *misc_roi_param = (HANTROEncMiscParameterBufferROI *)misc_param->data;
                HANTROEncROI *region_roi = (HANTROEncROI *)misc_roi_param->roi;

                HDDLMemoryMgr_Memcpy ( (void *) (vaDataFullTX->data + offset), misc_roi_param,
		    sizeof (vaDataFullTX->data) - offset, sizeof (HANTROEncMiscParameterBufferROI));

                offset += sizeof (HANTROEncMiscParameterBufferROI);

                HDDLMemoryMgr_Memcpy ( (void *) (vaDataFullTX->data + offset), region_roi,
		    sizeof (vaDataFullTX->data) - offset,
		    sizeof (HANTROEncROI) * misc_roi_param->num_roi);
                break;
            }
            case HANTROEncMiscParameterTypeIPCM:
            {
                HANTROEncMiscParameterBufferIPCM *misc_ipcm_param = (HANTROEncMiscParameterBufferIPCM *)misc_param->data;
                HANTRORectangle *region_ipcm = (HANTRORectangle *)misc_ipcm_param->ipcm;

                HDDLMemoryMgr_Memcpy ( (void *) (vaDataFullTX->data + offset), misc_ipcm_param,
		    sizeof (vaDataFullTX->data) - offset,
		    sizeof (HANTROEncMiscParameterBufferIPCM));

                offset += sizeof (HANTROEncMiscParameterBufferIPCM);
                HDDLMemoryMgr_Memcpy ( (void *) (vaDataFullTX->data + offset), region_ipcm,
		    sizeof (vaDataFullTX->data) - offset,
		    sizeof (HANTRORectangle) * misc_ipcm_param->num_ipcm);
                break;
            }

#endif
            default:
            {
                HDDLMemoryMgr_Memcpy (vaDataFullTX->data, vaBuffer->pData,
		    sizeof (vaDataFullTX->data), dataSize);
                break;
            }
        }
    }
    else if (vaBuffer->type == VAProcPipelineParameterBufferType)
    {
        VAProcPipelineParameterBuffer *pipelineParam =
	    (VAProcPipelineParameterBuffer *)vaBuffer->pData;
        VARectangle *surfaceRegion = (VARectangle *)pipelineParam->surface_region;
        VASurfaceID *additionalOutputs = pipelineParam->additional_outputs;
        uint32_t numAdditionalOutputs = pipelineParam->num_additional_outputs;
        unsigned int offset = sizeof (VAProcPipelineParameterBuffer);

        HDDLMemoryMgr_Memcpy ( (void *) (vaDataFullTX->data), pipelineParam,
	    sizeof (vaDataFullTX->data), sizeof (VAProcPipelineParameterBuffer));

        HDDLMemoryMgr_Memcpy ( (void *) (vaDataFullTX->data + offset), surfaceRegion,
	    sizeof (vaDataFullTX->data) - offset, sizeof (VARectangle) * numAdditionalOutputs);

        offset += (sizeof (VARectangle) * numAdditionalOutputs);
        HDDLMemoryMgr_Memcpy ( (void *) (vaDataFullTX->data + offset), additionalOutputs,
	    sizeof (vaDataFullTX->data) - offset, sizeof (VASurfaceID) * numAdditionalOutputs);
    }
    // Do not perform HDDLMemoryMgr_Memcpy when it's VAEncCodedBufferType
    else if (vaBuffer->type != VAEncCodedBufferType)
    {
        HDDLMemoryMgr_Memcpy (vaDataFullTX->data, vaBuffer->pData,
	    sizeof (vaDataFullTX->data), dataSize);
    }

    commStatus = Comm_Submission (commCtx, HDDLVAUnmapBuffer, COMM_READ_FULL,
        sizeof (HDDLVADataFullTX), (void *)vaDataFullTX,
        sizeof (HDDLVAUnmapBufferRX), (void **)&vaDataRX);
        
    HDDLMemoryMgr_FreeMemory (vaDataFullTX);

    if (commStatus != COMM_STATUS_SUCCESS)
    {
        HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVAUnmapBuffer) ||
        (vaDataRX.vaData.size != sizeof (HDDLVAUnmapBufferRX)))
    {
        HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = HDDLVAShim_UnmapInternalBuffer (vaShimCtx, bufId, vaBuffer);
    if (vaStatus != VA_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Failed to unmap buffer");
        HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
        return VA_STATUS_ERROR_INVALID_BUFFER;
    }

    HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);

    vaStatus = vaDataRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_CreateImage (VADriverContextP ctx, VAImageFormat *format, int width,
    int height, VAImage *image)
{
    HDDLVACreateImageTX vaDataTX;
    HDDLVACreateImageRX vaDataRX;
    HDDLShimCommContext *commCtx;
    VAImage *vaImg;
    HDDLVAImageElement *vaImageElement;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();

    SHIM_CHK_NULL (ctx, "ctx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);
    SHIM_CHK_NULL (format, "Invalid format", VA_STATUS_ERROR_INVALID_PARAMETER);
    SHIM_CHK_NULL (image, "Invalid image", VA_STATUS_ERROR_INVALID_PARAMETER);
    SHIM_CHK_LARGER (width, 0, "Invalid width", VA_STATUS_ERROR_INVALID_PARAMETER);
    SHIM_CHK_LARGER (height, 0, "Invalid height", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "VAShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    // Create the HDDLVAData structure to be send out
    vaDataTX.vaData.vaFunctionID = HDDLVACreateImage;
    vaDataTX.vaData.size = sizeof (HDDLVACreateImageTX);
    vaDataTX.format = *format;
    vaDataTX.width = width;
    vaDataTX.height = height;

    // Initiate HDDL target CreateImage
    commStatus = Comm_Submission (commCtx, HDDLVACreateImage, COMM_READ_FULL,
        sizeof (HDDLVACreateImageTX), (void *)&vaDataTX,
        sizeof (HDDLVACreateImageRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVACreateImage) ||
        (vaDataRX.vaData.size != sizeof (HDDLVACreateImageRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;
    SHIM_CHK_ERROR (vaStatus, "VA status failed", VA_STATUS_ERROR_UNKNOWN);

   *image = vaDataRX.image;

    // Create a HDDLVAIMAGEMAP structure to add to local image heap
    vaImg = (VAImage *)HDDLMemoryMgr_AllocAndZeroMemory (sizeof (VAImage));
    SHIM_CHK_NULL (vaImg, "vaImg returned NULL", VA_STATUS_ERROR_ALLOCATION_FAILED);

    HDDLMemoryMgr_Memcpy (vaImg, image, sizeof (VAImage), sizeof (vaDataRX.image));

    HDDLThreadMgr_LockMutex (&vaShimCtx->imageMutex);

    vaImageElement = (HDDLVAImageElement *)HDDLMemoryMgr_AllocImageHeap (vaShimCtx->imageHeap);
    if (NULL == vaImageElement)
    {
        HDDLMemoryMgr_FreeMemory (vaImg);
        HDDLThreadMgr_UnlockMutex (&vaShimCtx->imageMutex);
        return VA_STATUS_ERROR_ALLOCATION_FAILED;
    }

    vaStatus = HDDLVAShim_CreateInternalBufferAtHeap (vaShimCtx, 0, VAImageBufferType,
        vaImg->data_size, 1, vaImg->buf, NULL);

    // Store the info into heap
    vaImageElement->pImage = vaImg;
    vaShimCtx->uiNumImage++;
    SHIM_CHK_NULL (vaImageElement, "vaImageElement returned NULL", VA_STATUS_ERROR_INVALID_IMAGE);

    HDDLThreadMgr_UnlockMutex (&vaShimCtx->imageMutex);

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_DeriveImage (VADriverContextP ctx, VASurfaceID surface, VAImage *image)
{
    HDDLVADeriveImageTX vaDataTX;
    HDDLVADeriveImageRX vaDataRX;
    HDDLShimCommContext *commCtx;
    VAImage *vaImg;
    HDDLVAImageElement *vaImageElement;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx,"ctx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "vaShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);
    SHIM_CHK_NULL (vaShimCtx->imageHeap, "imageHeap returned NULL", VA_STATUS_ERROR_INVALID_IMAGE);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    // Construct the structure to send out
    vaDataTX.vaData.vaFunctionID = HDDLVADeriveImage;
    vaDataTX.vaData.size = sizeof (HDDLVADeriveImageTX);
    vaDataTX.surface = surface;

    commStatus = Comm_Submission (commCtx, HDDLVADeriveImage, COMM_READ_FULL,
        sizeof (HDDLVADeriveImageTX), (void *)&vaDataTX,
        sizeof (HDDLVADeriveImageRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVADeriveImage) ||
        (vaDataRX.vaData.size != sizeof (HDDLVADeriveImageRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;
    if (vaStatus != VA_STATUS_SUCCESS)
    {
        SHIM_NORMAL_MESSAGE ("KMB returned error: 0x%X\r\n", vaStatus);
        return vaStatus;
    }

    *image = vaDataRX.image;

    vaImg = (VAImage *)HDDLMemoryMgr_AllocAndZeroMemory (sizeof (VAImage));
    SHIM_CHK_NULL (vaImg, "vaImg returned NULL", VA_STATUS_ERROR_ALLOCATION_FAILED);

    HDDLMemoryMgr_Memcpy (vaImg, image, sizeof (VAImage), sizeof (vaDataRX.image));

    HDDLThreadMgr_LockMutex (&vaShimCtx->imageMutex);

    vaImageElement = (HDDLVAImageElement *)HDDLMemoryMgr_AllocImageHeap (vaShimCtx->imageHeap);
    if (NULL == vaImageElement)
    {
        HDDLMemoryMgr_FreeMemory (vaImg);
        HDDLThreadMgr_UnlockMutex (&vaShimCtx->imageMutex);
        return VA_STATUS_ERROR_ALLOCATION_FAILED;
    }

    // Store the info into heap
    vaImageElement->pImage = vaImg;
    vaShimCtx->uiNumImage++;
    SHIM_CHK_NULL (vaImageElement, "vaImageElement returned NULL", VA_STATUS_ERROR_INVALID_IMAGE);

    // Create buffer for Image into buffer heap
    vaStatus = HDDLVAShim_CreateInternalBufferAtHeap (vaShimCtx, 0, VAImageBufferType,
        vaImg->data_size, 1, vaImg->buf, NULL);
    if (vaStatus != VA_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Failed to create buffer at heap");
        HDDLThreadMgr_UnlockMutex (&vaShimCtx->imageMutex);
        return VA_STATUS_ERROR_INVALID_BUFFER;
    }

    HDDLThreadMgr_UnlockMutex (&vaShimCtx->imageMutex);

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_DestroyImage (VADriverContextP ctx, VAImageID image)
{
    HDDLVADestroyImageTX vaDataTX;
    HDDLVADestroyImageRX vaDataRX;
    HDDLShimCommContext *commCtx;
    VAImage *vaImg;
    uint32_t hostImgId;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "vaShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);
    SHIM_CHK_NULL (vaShimCtx->imageHeap, "vaShimCtx returned NULL", VA_STATUS_ERROR_INVALID_IMAGE);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    // Get VAImage from VAImageID that stores inside local imageHeap
    HDDLThreadMgr_LockMutex (&vaShimCtx->imageMutex);

    vaImg = HDDLMemoryMgr_GetVAImageFromVAImageID (vaShimCtx, image, &hostImgId);
    if (NULL == vaImg)
    {
        HDDLThreadMgr_UnlockMutex (&vaShimCtx->imageMutex);
        return VA_STATUS_ERROR_INVALID_PARAMETER;
    }

    vaStatus = HDDLVAShim_ReleaseInternalBufferFromHeap (vaShimCtx, vaImg->buf);
    if (vaStatus != VA_STATUS_SUCCESS)
    {
        HDDLThreadMgr_UnlockMutex (&vaShimCtx->imageMutex);
        return VA_STATUS_ERROR_INVALID_BUFFER;
    }

    HDDLMemoryMgr_ReleaseImageElement (vaShimCtx->imageHeap, hostImgId);

    vaShimCtx->uiNumImage--;

    HDDLThreadMgr_UnlockMutex (&vaShimCtx->imageMutex);

    HDDLMemoryMgr_FreeMemory (vaImg);

    vaDataTX.vaData.vaFunctionID = HDDLVADestroyImage;
    vaDataTX.vaData.size = sizeof (HDDLVADestroyImageTX);
    vaDataTX.image = image;

    commStatus = Comm_Submission (commCtx, HDDLVADestroyImage, COMM_READ_FULL,
        sizeof (HDDLVADestroyImageTX), (void *)&vaDataTX,
        sizeof (HDDLVADestroyImageRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVADestroyImage) ||
        (vaDataRX.vaData.size != sizeof (HDDLVADestroyImageRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;
    SHIM_CHK_ERROR (vaStatus, "VA status failed", VA_STATUS_ERROR_UNKNOWN);

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_BeginPicture (VADriverContextP ctx, VAContextID context,
    VASurfaceID renderTarget)
{
    HDDLVABeginPictureTX vaDataTX;
    HDDLVABeginPictureRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "VAShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    vaDataTX.vaData.vaFunctionID = HDDLVABeginPicture;
    vaDataTX.vaData.size = sizeof(vaDataTX);
    vaDataTX.context = context;
    vaDataTX.renderTarget = renderTarget;

    commStatus = Comm_Submission (commCtx, HDDLVABeginPicture, COMM_READ_FULL,
        sizeof (HDDLVABeginPictureTX), (void *)&vaDataTX,
        sizeof (HDDLVABeginPictureRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVABeginPicture) ||
        (vaDataRX.vaData.size != sizeof (HDDLVABeginPictureRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_RenderPicture (VADriverContextP ctx, VAContextID context, VABufferID *buffer,
    int numBuffer)
{
    HDDLVARenderPictureRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "VAShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    typedef struct {
        HDDLVARenderPictureTX vaDataTX;
        VABufferID buffer[numBuffer];
    }HDDLVADataFullTX;

    HDDLVADataFullTX vaDataFullTX;

    vaDataFullTX.vaDataTX.vaData.vaFunctionID = HDDLVARenderPicture;
    vaDataFullTX.vaDataTX.vaData.size = sizeof (HDDLVADataFullTX);
    vaDataFullTX.vaDataTX.context = context;
    vaDataFullTX.vaDataTX.numBuffer = numBuffer;
    HDDLMemoryMgr_Memcpy (vaDataFullTX.buffer, buffer, sizeof (vaDataFullTX.buffer),
	sizeof (VABufferID) * numBuffer);

    commStatus = Comm_Submission (commCtx, HDDLVARenderPicture, COMM_READ_FULL,
        sizeof (HDDLVADataFullTX), (void *)&vaDataFullTX,
        sizeof (HDDLVARenderPictureRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVARenderPicture) ||
        (vaDataRX.vaData.size != sizeof (HDDLVARenderPictureRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_EndPicture (VADriverContextP ctx, VAContextID context)
{
    HDDLVAEndPictureTX vaDataTX;
    HDDLVAEndPictureRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "VAShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    vaDataTX.vaData.vaFunctionID = HDDLVAEndPicture;
    vaDataTX.vaData.size = sizeof (HDDLVAEndPictureTX);
    vaDataTX.context = context;

    commStatus = Comm_Submission (commCtx, HDDLVAEndPicture, COMM_READ_FULL,
        sizeof (HDDLVAEndPictureTX), (void *)&vaDataTX,
        sizeof (HDDLVAEndPictureRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVAEndPicture) ||
        (vaDataRX.vaData.size != sizeof (HDDLVAEndPictureRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_SyncSurface (VADriverContextP ctx, VASurfaceID renderTarget)
{
    HDDLVASyncSurfaceTX vaDataTX;
    HDDLVASyncSurfaceRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "vaShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    vaDataTX.vaData.vaFunctionID = HDDLVASyncSurface;
    vaDataTX.vaData.size = sizeof (HDDLVASyncSurfaceTX);
    vaDataTX.renderTarget = renderTarget;

    commStatus = Comm_Submission (commCtx, HDDLVASyncSurface, COMM_READ_FULL,
        sizeof (HDDLVASyncSurfaceTX), (void *)&vaDataTX,
        sizeof (HDDLVASyncSurfaceRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVASyncSurface) ||
        (vaDataRX.vaData.size != sizeof (HDDLVASyncSurfaceRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_QueryConfigProfiles (VADriverContextP ctx, VAProfile *profileList,
    int *numProfile)
{
    HDDLVAQueryConfigProfilesTX vaDataTX;
    HDDLShimCommContext *commCtx;
    HDDLVAData vaData;
    VAStatus vaStatus;
    int numProfiles;
    void *peekData = NULL;
    uint32_t peekSize = sizeof (HDDLVAData);
    uint32_t fullRXSize = 0;
    CommMode commMode = COMM_MODE_UNKNOWN;
    CommStatus commStatus = COMM_STATUS_FAILED;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "nullptr ctx", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commMode = COMM_MODE (commCtx);
    SHIM_CHK_EQUAL (commMode, COMM_MODE_UNKNOWN, "Invalid commumication mode",
        VA_STATUS_ERROR_INVALID_CONTEXT);

    vaDataTX.vaData.vaFunctionID = HDDLVAQueryConfigProfiles;
    vaDataTX.vaData.size = sizeof (HDDLVAQueryConfigProfilesTX);
    vaDataTX.numProfiles = ctx->max_profiles;

    if (commMode == COMM_MODE_TCP)
    {
        peekData = &vaData;
    }

    commStatus = Comm_Submission (commCtx, HDDLVAQueryConfigProfiles,
        COMM_READ_PARTIAL, sizeof (HDDLVAQueryConfigProfilesTX), (void *)&vaDataTX,
        peekSize, (void **)&peekData);

    if (commStatus != COMM_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Comm operation failed");
	HDDLMemoryMgr_FreeMemory (peekData);
	return VA_STATUS_ERROR_UNKNOWN;
    }

    if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
    {
        vaData = *(HDDLVAData *)peekData;
    }

    numProfiles = (vaData.size - sizeof (HDDLVAQueryConfigProfilesRX)) / sizeof (VAProfile);

    typedef struct {
        HDDLVAQueryConfigProfilesRX vaDataRX;
        VAProfile profileList[numProfiles];
    }HDDLVADataFullRX;
    HDDLVADataFullRX *vaDataFullRX;

    fullRXSize = sizeof (HDDLVADataFullRX);
    commStatus = COMM_STATUS_FAILED;

    if (commMode == COMM_MODE_TCP)
    {
        vaDataFullRX = HDDLMemoryMgr_AllocMemory (fullRXSize);
	SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_UNKNOWN);
        commStatus = Comm_Read (commCtx, fullRXSize, vaDataFullRX);
    }
    else if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
    {
        vaDataFullRX = (HDDLVADataFullRX *)peekData;

        if (fullRXSize > DATA_MAX_SEND_SIZE)
        {
            vaDataFullRX = HDDLMemoryMgr_ReallocMemory (vaDataFullRX, fullRXSize);

            commStatus = Comm_Read (commCtx, fullRXSize - DATA_MAX_SEND_SIZE,
                (char *) (vaDataFullRX) + DATA_MAX_SEND_SIZE);
        }
        else
        {
            commStatus = COMM_STATUS_SUCCESS;
        }
    }
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataFullRX->vaDataRX.vaData.vaFunctionID != HDDLVAQueryConfigProfiles) ||
        (vaDataFullRX->vaDataRX.vaData.size != sizeof (HDDLVADataFullRX)))
    {
        HDDLMemoryMgr_FreeMemory (vaShimCtx);
	HDDLMemoryMgr_FreeMemory (peekData);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataFullRX->vaDataRX.ret;
    *numProfile = vaDataFullRX->vaDataRX.numProfile;
    HDDLMemoryMgr_Memcpy (profileList, &vaDataFullRX->profileList, sizeof (VAProfile) * (*numProfile),
        sizeof (vaDataFullRX->profileList));

    HDDLMemoryMgr_FreeMemory (vaDataFullRX);

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_QueryConfigEntrypoints (VADriverContextP ctx, VAProfile profile,
    VAEntrypoint *entrypointList, int *numEntrypoint)
{
    HDDLVAQueryConfigEntrypointsTX vaDataTX;
    HDDLShimCommContext *commCtx;
    HDDLVAData vaData;
    VAStatus vaStatus;
    int numEntrypoints;
    void *peekData = NULL;
    uint32_t peekSize = sizeof (HDDLVAData);
    uint32_t fullRXSize = 0;
    CommMode commMode = COMM_MODE_UNKNOWN;
    CommStatus commStatus = COMM_STATUS_FAILED;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "nullptr ctx", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commMode = COMM_MODE (commCtx);
    SHIM_CHK_EQUAL (commMode, COMM_MODE_UNKNOWN, "Invalid commumication mode",
        VA_STATUS_ERROR_INVALID_CONTEXT);

    vaDataTX.vaData.vaFunctionID = HDDLVAQueryConfigEntrypoints;
    vaDataTX.vaData.size = sizeof (HDDLVAQueryConfigEntrypointsTX);
    vaDataTX.numEntrypoint = ctx->max_entrypoints;
    vaDataTX.profile = profile;

    if (commMode == COMM_MODE_TCP)
    {
        peekData = &vaData;
    }

    commStatus = Comm_Submission (commCtx, HDDLVAQueryConfigEntrypoints,
        COMM_READ_PARTIAL, sizeof (HDDLVAQueryConfigEntrypointsTX), (void *)&vaDataTX,
        peekSize, (void **)&peekData);

    if (commStatus != COMM_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Comm operation failed");
        HDDLMemoryMgr_FreeMemory (peekData);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
    {
	vaData = *(HDDLVAData *)peekData;
    }

    numEntrypoints = (vaData.size - sizeof (HDDLVAQueryConfigEntrypointsRX)) /
        sizeof (VAEntrypoint);

    typedef struct {
        HDDLVAQueryConfigEntrypointsRX vaDataRX;
        VAEntrypoint entrypointList[numEntrypoints];
    }HDDLVADataFullRX;
    HDDLVADataFullRX *vaDataFullRX;

    fullRXSize = sizeof (HDDLVADataFullRX);
    commStatus = COMM_STATUS_FAILED;

    if (commMode == COMM_MODE_TCP)
    {
        vaDataFullRX = HDDLMemoryMgr_AllocMemory (fullRXSize);
        SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_UNKNOWN);
        commStatus = Comm_Read (commCtx, fullRXSize, (void *)vaDataFullRX);
    }
    else if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
    {
        vaDataFullRX = (HDDLVADataFullRX *)peekData;

        if (fullRXSize > DATA_MAX_SEND_SIZE)
        {
            vaDataFullRX = HDDLMemoryMgr_ReallocMemory (vaDataFullRX, fullRXSize);

            commStatus = Comm_Read (commCtx, fullRXSize - DATA_MAX_SEND_SIZE,
                (char *) (vaDataFullRX) + DATA_MAX_SEND_SIZE);
        }
        else
        {
            commStatus = COMM_STATUS_SUCCESS;
        }

    }

    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataFullRX->vaDataRX.vaData.vaFunctionID != HDDLVAQueryConfigEntrypoints) ||
        (vaDataFullRX->vaDataRX.vaData.size != sizeof (HDDLVADataFullRX)))
    {
	HDDLMemoryMgr_FreeMemory (peekData);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataFullRX->vaDataRX.ret;
    *numEntrypoint  = vaDataFullRX->vaDataRX.numEntrypoint;
    HDDLMemoryMgr_Memcpy (entrypointList, &vaDataFullRX->entrypointList,
        sizeof (VAEntrypoint) * (*numEntrypoint), sizeof (vaDataFullRX->entrypointList));

    HDDLMemoryMgr_FreeMemory (vaDataFullRX);

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_GetConfigAttributes (VADriverContextP ctx, VAProfile profile,
    VAEntrypoint entrypoint, VAConfigAttrib *attribList, int numAttrib)
{
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "nullptr ctx", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    typedef struct {
        HDDLVAGetConfigAttributesTX vaDataTX;
        VAConfigAttrib attribList[numAttrib];
    }HDDLVADataFullTX;

    HDDLVADataFullTX vaDataFullTX;

    vaDataFullTX.vaDataTX.vaData.vaFunctionID = HDDLVAGetConfigAttributes;
    vaDataFullTX.vaDataTX.vaData.size = sizeof (HDDLVADataFullTX);
    vaDataFullTX.vaDataTX.profile = profile;
    vaDataFullTX.vaDataTX.entrypoint = entrypoint;
    vaDataFullTX.vaDataTX.numAttrib = numAttrib;
    HDDLMemoryMgr_Memcpy (vaDataFullTX.attribList, attribList, sizeof (vaDataFullTX.attribList),
        sizeof (VAConfigAttrib) * numAttrib);

    typedef struct {
        HDDLVAGetConfigAttributesRX vaDataRX;
        VAConfigAttrib attribList[numAttrib];
    }HDDLVADataFullRX;
    HDDLVADataFullRX vaDataFullRX;

    commStatus = Comm_Submission (commCtx, HDDLVAGetConfigAttributes,
        COMM_READ_FULL, sizeof (HDDLVADataFullTX), (void *)&vaDataFullTX,
	sizeof (HDDLVADataFullRX), (void **)&vaDataFullRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataFullRX.vaDataRX.vaData.vaFunctionID != HDDLVAGetConfigAttributes) ||
        (vaDataFullRX.vaDataRX.vaData.size != sizeof (HDDLVADataFullRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataFullRX.vaDataRX.ret;
    HDDLMemoryMgr_Memcpy (attribList, vaDataFullRX.attribList, sizeof (VAConfigAttrib) * numAttrib,
        sizeof (vaDataFullRX.attribList));

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_QueryConfigAttributes (VADriverContextP ctx, VAConfigID configId,
    VAProfile *profile, VAEntrypoint *entrypoint, VAConfigAttrib *attribList, int *numAttrib)
{
    HDDLVAQueryConfigAttributesTX vaDataTX;
    HDDLShimCommContext *commCtx;
    HDDLVAData vaData;
    VAStatus vaStatus;
    int numAttribs;
    void *peekData = NULL;
    uint32_t peekSize = sizeof (HDDLVAData);
    uint32_t fullRXSize = 0;
    CommMode commMode = COMM_MODE_UNKNOWN;
    CommStatus commStatus = COMM_STATUS_FAILED;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "nullptr ctx", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commMode = COMM_MODE (commCtx);
    SHIM_CHK_EQUAL (commMode, COMM_MODE_UNKNOWN, "Invalid commumication mode",
        VA_STATUS_ERROR_INVALID_CONTEXT);

    vaDataTX.vaData.vaFunctionID = HDDLVAQueryConfigAttributes;
    vaDataTX.vaData.size = sizeof (HDDLVAQueryConfigAttributesTX);
    vaDataTX.numAttributes = ctx->max_attributes;
    vaDataTX.configId = configId;

    if (commMode == COMM_MODE_TCP)
    {
        peekData = &vaData;
    }

    commStatus = Comm_Submission (commCtx, HDDLVAQueryConfigAttributes,
        COMM_READ_PARTIAL, sizeof (HDDLVAQueryConfigAttributesTX), (void *)&vaDataTX,
        peekSize, (void **)&peekData);

    if (commStatus != COMM_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Comm operation failed");
        HDDLMemoryMgr_FreeMemory (peekData);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
    {
        vaData = *(HDDLVAData *)peekData;
    }

    numAttribs = (vaData.size - sizeof (HDDLVAQueryConfigAttributesRX)) /
        sizeof (VAConfigAttrib);

    typedef struct {
        HDDLVAQueryConfigAttributesRX vaDataRX;
        VAConfigAttrib attribList[numAttribs];
    }HDDLVADataFullRX;
    HDDLVADataFullRX *vaDataFullRX;

    fullRXSize = sizeof (HDDLVADataFullRX);
    commStatus = COMM_STATUS_FAILED;

    if (commMode == COMM_MODE_TCP)
    {
        vaDataFullRX = HDDLMemoryMgr_AllocMemory (fullRXSize);
	SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_UNKNOWN);
        commStatus = Comm_Read (commCtx, fullRXSize,
            (void *)vaDataFullRX);
    }
    else if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
    {
        vaDataFullRX = (HDDLVADataFullRX *)peekData;

	if (fullRXSize > DATA_MAX_SEND_SIZE)
        {
            vaDataFullRX = HDDLMemoryMgr_ReallocMemory (vaDataFullRX, fullRXSize);

            commStatus = Comm_Read (commCtx, fullRXSize - DATA_MAX_SEND_SIZE,
                (char *) (vaDataFullRX) + DATA_MAX_SEND_SIZE);
        }
        else
        {
            commStatus = COMM_STATUS_SUCCESS;
        }
    }

    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataFullRX->vaDataRX.vaData.vaFunctionID != HDDLVAQueryConfigAttributes) ||
        (vaDataFullRX->vaDataRX.vaData.size != sizeof (HDDLVADataFullRX)))
    {
	HDDLMemoryMgr_FreeMemory (peekData);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    *profile = vaDataFullRX->vaDataRX.profile;
    *entrypoint = vaDataFullRX->vaDataRX.entrypoint;
    *numAttrib= vaDataFullRX->vaDataRX.numAttrib;
    HDDLMemoryMgr_Memcpy (attribList, vaDataFullRX->attribList, sizeof (VAConfigAttrib) * (*numAttrib),
        sizeof (vaDataFullRX->attribList));
    vaStatus = vaDataFullRX->vaDataRX.ret;

    HDDLMemoryMgr_FreeMemory (vaDataFullRX);

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_QuerySurfaceStatus (VADriverContextP ctx, VASurfaceID renderTarget,
    VASurfaceStatus *status)
{
    HDDLVAQuerySurfaceStatusTX vaDataTX;
    HDDLVAQuerySurfaceStatusRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "nullptr ctx", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    vaDataTX.vaData.vaFunctionID = HDDLVAQuerySurfaceStatus;
    vaDataTX.vaData.size = sizeof (HDDLVAQuerySurfaceStatusTX);
    vaDataTX.renderTarget = renderTarget;

    commStatus = Comm_Submission (commCtx, HDDLVAQuerySurfaceStatus, COMM_READ_FULL,
        sizeof (HDDLVAQuerySurfaceStatusTX), (void *)&vaDataTX,
        sizeof (HDDLVAQuerySurfaceStatusRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVAQuerySurfaceStatus) ||
        (vaDataRX.vaData.size != sizeof (HDDLVAQuerySurfaceStatusRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    HDDLMemoryMgr_Memcpy (status, &vaDataRX.status, sizeof (VASurfaceStatus),
        sizeof (vaDataRX.status));
    vaStatus = vaDataRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_QueryImageFormats (VADriverContextP ctx, VAImageFormat *formatList,
    int *numFormat)
{
    HDDLVAQueryImageFormatsTX vaDataTX;
    HDDLShimCommContext *commCtx;
    HDDLVAData vaData;
    VAStatus vaStatus;
    int numFormats;
    void *peekData = NULL;
    uint32_t peekSize = sizeof (HDDLVAData);
    uint32_t fullRXSize = 0;
    CommMode commMode = COMM_MODE_UNKNOWN;
    CommStatus commStatus = COMM_STATUS_FAILED;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "nullptr ctx", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commMode = COMM_MODE (commCtx);
    SHIM_CHK_EQUAL (commMode, COMM_MODE_UNKNOWN, "Invalid commumication mode",
        VA_STATUS_ERROR_INVALID_CONTEXT);

    vaDataTX.vaData.vaFunctionID = HDDLVAQueryImageFormats;
    vaDataTX.vaData.size = sizeof (HDDLVAQueryImageFormatsTX);
    vaDataTX.numFormat = ctx->max_image_formats;

    if (commMode == COMM_MODE_TCP)
    {
	peekData = &vaData;
    }

    commStatus = Comm_Submission (commCtx, HDDLVAQueryImageFormats,
        COMM_READ_PARTIAL, sizeof (HDDLVAQueryImageFormatsTX), (void *)&vaDataTX,
        peekSize, (void **)&peekData);

    if (commStatus != COMM_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Comm operation failed");
        HDDLMemoryMgr_FreeMemory (peekData);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
    {
	vaData = *(HDDLVAData *)peekData;
    }

    numFormats = (vaData.size - sizeof (HDDLVAQueryImageFormatsRX)) / sizeof (VAImageFormat);

    typedef struct {
        HDDLVAQueryImageFormatsRX vaDataRX;
        VAImageFormat formatList[numFormats];
    }HDDLVADataFullRX;
    HDDLVADataFullRX *vaDataFullRX;

    fullRXSize = sizeof (HDDLVADataFullRX);
    commStatus = COMM_STATUS_FAILED;

    if (commMode == COMM_MODE_TCP)
    {
        vaDataFullRX = HDDLMemoryMgr_AllocMemory (fullRXSize);
	SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_UNKNOWN);
        commStatus = Comm_Read (commCtx, fullRXSize,
            (void *)vaDataFullRX);
    }
    else if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
    {
        vaDataFullRX = (HDDLVADataFullRX *)peekData;

	if (fullRXSize > DATA_MAX_SEND_SIZE)
        {
            vaDataFullRX = HDDLMemoryMgr_ReallocMemory (vaDataFullRX, fullRXSize);

            commStatus = Comm_Read (commCtx, fullRXSize - DATA_MAX_SEND_SIZE,
                (char *) (vaDataFullRX) + DATA_MAX_SEND_SIZE);
        }
        else
        {
            commStatus = COMM_STATUS_SUCCESS;
        }
    }

    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataFullRX->vaDataRX.vaData.vaFunctionID != HDDLVAQueryImageFormats) ||
        (vaDataFullRX->vaDataRX.vaData.size != sizeof (HDDLVADataFullRX)))
    {
	HDDLMemoryMgr_FreeMemory (peekData);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataFullRX->vaDataRX.ret;
    *numFormat = vaDataFullRX->vaDataRX.numFormat;
    HDDLMemoryMgr_Memcpy (formatList, vaDataFullRX->formatList,
        sizeof (VAImageFormat) * (*numFormat), sizeof (vaDataFullRX->formatList));

    HDDLMemoryMgr_FreeMemory (vaDataFullRX);

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_GetImage (VADriverContextP ctx, VASurfaceID surface, int x, int y,
    unsigned int width, unsigned int height, VAImageID image)
{
    HDDLVAGetImageTX vaDataTX;
    HDDLVAGetImageRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "VAShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    vaDataTX.vaData.vaFunctionID = HDDLVAGetImage;
    vaDataTX.vaData.size = sizeof (HDDLVAGetImageTX);
    vaDataTX.surface = surface;
    vaDataTX.x = x;
    vaDataTX.y = y;
    vaDataTX.width = width;
    vaDataTX.height = height;
    vaDataTX.image = image;

    commStatus = Comm_Submission (commCtx, HDDLVAGetImage, COMM_READ_FULL,
        sizeof (HDDLVAGetImageTX), (void *)&vaDataTX,
        sizeof (HDDLVAGetImageRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVAGetImage) ||
        (vaDataRX.vaData.size != sizeof (HDDLVAGetImageRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

// TODO: Verify if internap map buffer is needed for this function
VAStatus HDDLVAShim_PutImage (VADriverContextP ctx, VASurfaceID surface, VAImageID image,
    int srcX, int srcY, unsigned int srcWidth, unsigned int srcHeight, int destX, int destY,
    unsigned int destWidth, unsigned int destHeight)
{
    HDDLVAPutImageTX vaDataTX;
    HDDLShimCommContext *commCtx;
    HDDLVAData vaData;
    VAStatus vaStatus;
    VAImage *vaImage;
    uint32_t hostImgId;
    unsigned int bufSize;
    void *peekData = NULL;
    uint32_t peekSize = sizeof (HDDLVAData);
    uint32_t fullRXSize = 0;
    CommMode commMode = COMM_MODE_UNKNOWN;
    CommStatus commStatus = COMM_STATUS_FAILED;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx ptr returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "VAShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commMode = COMM_MODE (commCtx);
    SHIM_CHK_EQUAL (commMode, COMM_MODE_UNKNOWN, "Invalid commumication mode",
        VA_STATUS_ERROR_INVALID_CONTEXT);

    vaImage = HDDLMemoryMgr_GetVAImageFromVAImageID (vaShimCtx, image, &hostImgId);
    SHIM_CHK_NULL (vaImage, "VAImage return NULL", VA_STATUS_ERROR_ALLOCATION_FAILED);

    vaDataTX.vaData.vaFunctionID = HDDLVAPutImage;
    vaDataTX.vaData.size = sizeof (HDDLVAPutImageTX);
    vaDataTX.surface = surface;
    vaDataTX.image = image;
    vaDataTX.srcX = srcX;
    vaDataTX.srcY = srcY;
    vaDataTX.srcWidth = srcWidth;
    vaDataTX.srcHeight = srcHeight;
    vaDataTX.destX = destX;
    vaDataTX.destY = destY;
    vaDataTX.destWidth = destWidth;
    vaDataTX.destHeight = destHeight;
    vaDataTX.bufId = vaImage->buf;
    vaDataTX.bufSize = vaImage->data_size;

    if (commMode == COMM_MODE_TCP)
    {
        peekData = &vaData;
    }

    commStatus = Comm_Submission (commCtx, HDDLVAPutImage, COMM_READ_PARTIAL,
        sizeof (HDDLVAPutImageTX), (void *)&vaDataTX,
        peekSize, (void **)&peekData);

    if (commStatus != COMM_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Comm operation failed");
        HDDLMemoryMgr_FreeMemory (peekData);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
    {
        vaData = *(HDDLVAData *)peekData;
    }

    bufSize = (vaData.size - sizeof (HDDLVAPutImageRX)) / sizeof (unsigned char);

    typedef struct {
        HDDLVAPutImageRX vaDataRX;
        unsigned char bufData[bufSize];
    }HDDLVADataFullRX;
    HDDLVADataFullRX *vaDataFullRX;

    fullRXSize = sizeof (HDDLVADataFullRX);
    commStatus = COMM_STATUS_FAILED;

    if (commMode == COMM_MODE_TCP)
    {
        vaDataFullRX = HDDLMemoryMgr_AllocMemory (fullRXSize);
	SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_UNKNOWN);
        commStatus = Comm_Read (commCtx, fullRXSize,
            (void *)vaDataFullRX);
    }
    else if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
    {
        vaDataFullRX = (HDDLVADataFullRX *)peekData;

	if (fullRXSize > DATA_MAX_SEND_SIZE)
        {
            vaDataFullRX = HDDLMemoryMgr_ReallocMemory (vaDataFullRX, fullRXSize);

            commStatus = Comm_ReadSafe (commCtx, fullRXSize - DATA_MAX_SEND_SIZE,
                (void *) (vaDataFullRX + DATA_MAX_SEND_SIZE));
        }
        else
        {
            commStatus = COMM_STATUS_SUCCESS;
        }
    }

    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataFullRX->vaDataRX.vaData.vaFunctionID != HDDLVAPutImage) ||
        (vaDataFullRX->vaDataRX.vaData.size != sizeof (HDDLVADataFullRX)))
    {
	HDDLMemoryMgr_FreeMemory (peekData);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    void *pBuf = NULL;

    HDDLThreadMgr_LockMutex (&vaShimCtx->bufferMutex);
    vaStatus = HDDLVAShim_MapInternalBuffer (vaShimCtx, vaImage->buf, NULL, &pBuf);
    if (vaStatus != VA_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Fail to map buffer");
	HDDLMemoryMgr_FreeMemory (peekData);
        HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
        return VA_STATUS_ERROR_INVALID_BUFFER;
    }

    HDDLMemoryMgr_Memcpy (pBuf, vaDataFullRX->bufData, bufSize, sizeof (vaDataFullRX->bufData));

    vaStatus = HDDLVAShim_UnmapInternalBuffer (vaShimCtx, vaImage->buf, NULL);
    HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);

    if (vaStatus != VA_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Failed to unmap buffer");
	HDDLMemoryMgr_FreeMemory (peekData);
	return VA_STATUS_ERROR_INVALID_BUFFER;
    }

    vaStatus = vaDataFullRX->vaDataRX.ret;

    HDDLMemoryMgr_FreeMemory (vaDataFullRX);

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_QueryDisplayAttributes (VADriverContextP ctx, VADisplayAttribute *attrList,
    int *numAttributes)
{
    HDDLVAQueryDisplayAttributesTX vaDataTX;
    HDDLShimCommContext *commCtx;
    HDDLVAData vaData;
    VAStatus vaStatus;
    int numAttribs;
    void *peekData = NULL;
    uint32_t peekSize = sizeof (HDDLVAData);
    uint32_t fullRXSize = 0;
    CommMode commMode = COMM_MODE_UNKNOWN;
    CommStatus commStatus = COMM_STATUS_FAILED;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx,"ctx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "vaShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commMode = COMM_MODE (commCtx);
    SHIM_CHK_EQUAL (commMode, COMM_MODE_UNKNOWN, "Invalid commumication mode",
        VA_STATUS_ERROR_INVALID_CONTEXT);

    vaDataTX.vaData.vaFunctionID = HDDLVAQueryDisplayAttributes;
    vaDataTX.vaData.size = sizeof (HDDLVAQueryDisplayAttributesTX);
    vaDataTX.numAttributes = ctx->max_display_attributes;

    if (commMode == COMM_MODE_TCP)
    {
        peekData = &vaData;
    }

    commStatus = Comm_Submission (commCtx, HDDLVAQueryDisplayAttributes,
        COMM_READ_PARTIAL, sizeof (HDDLVAQueryDisplayAttributesTX), (void *)&vaDataTX,
        peekSize, (void **)&peekData);

     if (commStatus != COMM_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Comm operation failed");
        HDDLMemoryMgr_FreeMemory (peekData);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
    {
        vaData = *(HDDLVAData *)peekData;
    }

    numAttribs = (vaData.size - sizeof (HDDLVAQueryDisplayAttributesRX)) /
        sizeof (VADisplayAttribute);

    typedef struct {
        HDDLVAQueryDisplayAttributesRX vaDataRX;
        VADisplayAttribute attrList[numAttribs];
    }HDDLVADataFullRX;
    HDDLVADataFullRX *vaDataFullRX;

    fullRXSize = sizeof (HDDLVADataFullRX);
    commStatus = COMM_STATUS_FAILED;

    if (commMode == COMM_MODE_TCP)
    {
        vaDataFullRX = HDDLMemoryMgr_AllocMemory (fullRXSize);
	SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_UNKNOWN);
        commStatus = Comm_Read (commCtx, fullRXSize,
            (void *)vaDataFullRX);
    }
    else if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
    {
        vaDataFullRX = (HDDLVADataFullRX *)peekData;

	if (fullRXSize > DATA_MAX_SEND_SIZE)
        {
            vaDataFullRX = HDDLMemoryMgr_ReallocMemory (vaDataFullRX, fullRXSize);

            commStatus = Comm_Read (commCtx, fullRXSize - DATA_MAX_SEND_SIZE,
                (char *) (vaDataFullRX) + DATA_MAX_SEND_SIZE);
        }
        else
        {
            commStatus = COMM_STATUS_SUCCESS;
        }
    }
    SHIM_CHK_NOT_EQUAL (commStatus, COMM_STATUS_SUCCESS, "Comm operation failed",
        VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataFullRX->vaDataRX.vaData.vaFunctionID != HDDLVAQueryDisplayAttributes) ||
        (vaDataFullRX->vaDataRX.vaData.size != sizeof (HDDLVADataFullRX)))
    {
	HDDLMemoryMgr_FreeMemory (peekData);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    *numAttributes = vaDataFullRX->vaDataRX.numAttributes;
    HDDLMemoryMgr_Memcpy (attrList, &vaDataFullRX->attrList,
        sizeof (VADisplayAttribute) * (*numAttributes), sizeof (vaDataFullRX->attrList));
    vaStatus = vaDataFullRX->vaDataRX.ret;

    HDDLMemoryMgr_FreeMemory (vaDataFullRX);

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_GetDisplayAttributes (VADriverContextP ctx, VADisplayAttribute *attrList,
    int numAttributes)
{
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "vaShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    typedef struct {
        HDDLVAGetDisplayAttributesTX vaDataTX;
        VADisplayAttribute attribList[numAttributes];
    }HDDLVADataFullTX;

    HDDLVADataFullTX vaDataFullTX;

    vaDataFullTX.vaDataTX.vaData.vaFunctionID = HDDLVAGetDisplayAttributes;
    vaDataFullTX.vaDataTX.vaData.size = sizeof (HDDLVADataFullTX);
    vaDataFullTX.vaDataTX.numAttributes = numAttributes;
    HDDLMemoryMgr_Memcpy (vaDataFullTX.attribList, attrList,
        sizeof (vaDataFullTX.attribList), sizeof (VADisplayAttribute) * numAttributes);

    typedef struct {
        HDDLVAGetDisplayAttributesRX vaDataRX;
        VADisplayAttribute  attribs[numAttributes];
    }HDDLVADataFullRX;

    HDDLVADataFullRX vaDataFullRX;

    commStatus = Comm_Submission (commCtx, HDDLVAGetDisplayAttributes,
        COMM_READ_FULL, sizeof (HDDLVADataFullTX), (void *)&vaDataFullTX,
        sizeof (HDDLVADataFullRX), (void **)&vaDataFullRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataFullRX.vaDataRX.vaData.vaFunctionID != HDDLVAGetDisplayAttributes) ||
        (vaDataFullRX.vaDataRX.vaData.size != sizeof (HDDLVADataFullRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    HDDLMemoryMgr_Memcpy (attrList, vaDataFullRX.attribs,
	sizeof (VADisplayAttribute) * numAttributes, sizeof (vaDataFullRX.attribs));
    vaStatus = vaDataFullRX.vaDataRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_SetDisplayAttributes (VADriverContextP ctx, VADisplayAttribute *attrList,
    int numAttributes)
{
    HDDLVASetDisplayAttributesRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "vaShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    typedef struct {
        HDDLVASetDisplayAttributesTX vaDataTX;
        VADisplayAttribute attribs[numAttributes];
    }HDDLVADataFullTX;

    HDDLVADataFullTX vaDataFullTX;

    vaDataFullTX.vaDataTX.vaData.vaFunctionID = HDDLVASetDisplayAttributes;
    vaDataFullTX.vaDataTX.vaData.size = sizeof (HDDLVADataFullTX);
    vaDataFullTX.vaDataTX.numAttributes = numAttributes;

    HDDLMemoryMgr_Memcpy (vaDataFullTX.attribs, attrList,
	sizeof (vaDataFullTX.attribs), sizeof (VADisplayAttribute) * numAttributes);

    commStatus = Comm_Submission (commCtx, HDDLVASetDisplayAttributes, COMM_READ_FULL,
        sizeof (HDDLVADataFullTX), (void *)&vaDataFullTX,
        sizeof (HDDLVASetDisplayAttributesRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVASetDisplayAttributes) ||
        (vaDataRX.vaData.size != sizeof (HDDLVASetDisplayAttributesRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_QuerySurfaceAttributes (VADriverContextP dpy, VAConfigID config,
    VASurfaceAttrib *attribList, unsigned int *numAttribs)
{
    HDDLVAQuerySurfaceAttributesTX vaDataTX;
    HDDLShimCommContext *commCtx;
    HDDLVAData vaData;
    VAStatus vaStatus;
    int numAttrib;
    void *peekData = NULL;
    uint32_t peekSize = sizeof (HDDLVAData);
    uint32_t fullRXSize = 0;
    CommMode commMode = COMM_MODE_UNKNOWN;
    CommStatus commStatus = COMM_STATUS_FAILED;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (dpy, "dpy returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (dpy);
    SHIM_CHK_NULL (vaShimCtx, "vaShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commMode = COMM_MODE (commCtx);
    SHIM_CHK_EQUAL (commMode, COMM_MODE_UNKNOWN, "Invalid commumication mode",
        VA_STATUS_ERROR_INVALID_CONTEXT);	

    vaDataTX.vaData.vaFunctionID = HDDLVAQuerySurfaceAttributes;
    vaDataTX.vaData.size = sizeof (HDDLVAQuerySurfaceAttributesTX);
    vaDataTX.config = config;
    vaDataTX.numAttribs = *numAttribs;

    if (commMode == COMM_MODE_TCP)
    {
        peekData = &vaData;
    }

    commStatus = Comm_Submission (commCtx, HDDLVAQuerySurfaceAttributes,
        COMM_READ_PARTIAL, sizeof (HDDLVAQuerySurfaceAttributesTX), (void *)&vaDataTX,
        peekSize, (void **)&peekData);

    if (commStatus != COMM_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Comm operation failed");
        HDDLMemoryMgr_FreeMemory (peekData);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
    {
        vaData = *(HDDLVAData *)peekData;
    }

    numAttrib = (vaData.size - sizeof (HDDLVAQuerySurfaceAttributesRX)) /
        sizeof (VASurfaceAttrib);

    typedef struct {
        HDDLVAQuerySurfaceAttributesRX vaDataRX;
        VASurfaceAttrib attribList[numAttrib];
    }HDDLVADataFullRX;
    HDDLVADataFullRX *vaDataFullRX;

    fullRXSize = sizeof (HDDLVADataFullRX);
    commStatus = COMM_STATUS_FAILED;

    if (commMode == COMM_MODE_TCP)
    {
        vaDataFullRX = HDDLMemoryMgr_AllocMemory (fullRXSize);
	SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_UNKNOWN);
        commStatus = Comm_Read (commCtx, fullRXSize,
            (void *)vaDataFullRX);
    }
    else if (commMode == COMM_MODE_XLINK || commMode == COMM_MODE_UNITE)
    {
        vaDataFullRX = (HDDLVADataFullRX *)peekData;

	if (fullRXSize > DATA_MAX_SEND_SIZE)
        {
            vaDataFullRX = HDDLMemoryMgr_ReallocMemory (vaDataFullRX, fullRXSize);

            commStatus = Comm_Read (commCtx, fullRXSize - DATA_MAX_SEND_SIZE,
                (char *) (vaDataFullRX) + DATA_MAX_SEND_SIZE);
        }
        else
        {
            commStatus = COMM_STATUS_SUCCESS;
        }
    }

    SHIM_CHK_NOT_EQUAL (commStatus, COMM_STATUS_SUCCESS, "Comm operation failed",
        VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataFullRX->vaDataRX.vaData.vaFunctionID != HDDLVAQuerySurfaceAttributes) ||
        (vaDataFullRX->vaDataRX.vaData.size != sizeof (HDDLVADataFullRX)))
    {
	HDDLMemoryMgr_FreeMemory (peekData);
        return VA_STATUS_ERROR_UNKNOWN;
    }

    *numAttribs = vaDataFullRX->vaDataRX.numAttribs;

    if (attribList)
    {
        HDDLMemoryMgr_Memcpy (attribList, vaDataFullRX->attribList,
            sizeof (VASurfaceAttrib) * (*numAttribs), sizeof (vaDataFullRX->attribList));
    }

    vaStatus = vaDataFullRX->vaDataRX.ret;

    HDDLMemoryMgr_FreeMemory (vaDataFullRX);

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_CreateSurfaces2 (VADriverContextP ctx, unsigned int format, unsigned int width,
    unsigned int height, VASurfaceID *surfaces, unsigned int numSurfaces,
    VASurfaceAttrib *attribList, unsigned int numAttribs)
{
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    SHIM_CHK_LARGER (width, 0, "Invalid width", VA_STATUS_ERROR_INVALID_PARAMETER);
    SHIM_CHK_LARGER (height, 0, "Invalid height", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "vaShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    typedef struct {
        HDDLVACreateSurfaces2TX vaDataTX;
        VASurfaceAttrib attribList[numAttribs];
    }HDDLVADataFullTX;

    HDDLVADataFullTX vaDataFullTX;

    vaDataFullTX.vaDataTX.vaData.vaFunctionID = HDDLVACreateSurfaces2;
    vaDataFullTX.vaDataTX.vaData.size = sizeof (HDDLVADataFullTX);
    vaDataFullTX.vaDataTX.format = format;
    vaDataFullTX.vaDataTX.width = width;
    vaDataFullTX.vaDataTX.height = height;
    vaDataFullTX.vaDataTX.numSurfaces = numSurfaces;
    vaDataFullTX.vaDataTX.numAttribs = numAttribs;

    HDDLMemoryMgr_Memcpy (vaDataFullTX.attribList, attribList,
	sizeof (vaDataFullTX.attribList), sizeof (VASurfaceAttrib) * numAttribs);

    //VASurfaceAttribExternalBuffersAndFdBuffer
    for(int i = 0; i < numAttribs; i++)
    {
        if (vaDataFullTX.attribList[i].type == VASurfaceAttribExternalBufferDescriptor)
        {
            if (vaDataFullTX.attribList[i].value.type == VAGenericValueTypePointer)
            {
                VASurfaceAttribExternalBuffers *memAttribute =
                (VASurfaceAttribExternalBuffers *)vaDataFullTX.attribList[i].value.value.p;

                vaDataFullTX.vaDataTX.externalBuffers[i].extBuf.pixel_format = memAttribute->pixel_format;
                vaDataFullTX.vaDataTX.externalBuffers[i].extBuf.width = memAttribute->width;
                vaDataFullTX.vaDataTX.externalBuffers[i].extBuf.height = memAttribute->height;
                vaDataFullTX.vaDataTX.externalBuffers[i].extBuf.data_size = memAttribute->data_size;
                vaDataFullTX.vaDataTX.externalBuffers[i].extBuf.num_planes = memAttribute->num_planes;
                for (int j = 0; j < vaDataFullTX.vaDataTX.externalBuffers[i].extBuf.num_planes; j++)
                {
                    vaDataFullTX.vaDataTX.externalBuffers[i].extBuf.pitches[j] = memAttribute->pitches[j];
                    vaDataFullTX.vaDataTX.externalBuffers[i].extBuf.offsets[j] = memAttribute->offsets[j];
                }

                vaDataFullTX.vaDataTX.externalBuffers[i].extBuf.flags = memAttribute->flags;
                vaDataFullTX.vaDataTX.externalBuffers[i].extBuf.num_buffers = memAttribute->num_buffers;
                vaDataFullTX.vaDataTX.externalBuffers[i].extBuf.private_data = memAttribute->private_data;

                for(int k = 0 ; k < vaDataFullTX.vaDataTX.externalBuffers[i].extBuf.num_buffers; k++)
                {
                    vaDataFullTX.vaDataTX.externalBuffers[i].extbuf_handle[k] = memAttribute->buffers[k];
                }
            }
        }
    }

    typedef struct {
        HDDLVACreateSurfaces2RX vaDataRX;
        VASurfaceID surfaces[numSurfaces];
    }HDDLVADataFullRX;

    HDDLVADataFullRX vaDataFullRX;

    commStatus = Comm_Submission (commCtx, HDDLVACreateSurfaces2, COMM_READ_FULL,
        sizeof (HDDLVADataFullTX), (void *)&vaDataFullTX,
        sizeof (HDDLVADataFullRX), (void **)&vaDataFullRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataFullRX.vaDataRX.vaData.vaFunctionID != HDDLVACreateSurfaces2) ||
        (vaDataFullRX.vaDataRX.vaData.size != sizeof (HDDLVADataFullRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataFullRX.vaDataRX.ret;
    HDDLMemoryMgr_Memcpy (surfaces, vaDataFullRX.surfaces, sizeof (VASurfaceID) * numSurfaces,
        sizeof (vaDataFullRX.surfaces));

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_ExportSurfaceHandle (VADriverContextP ctx, VASurfaceID surfaceId,
    uint32_t memType, uint32_t flags, void *descriptor)
{
    HDDLVAExportSurfaceHandleTX vaDataTX;
    HDDLVAExportSurfaceHandleRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "ctx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "vaShimCtx returned NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    // HANTRO driver only support VA_SURFACE_ATTRIB_MEM_TYPE_DRM_PRIME_2 memory type.
    if (memType != VA_SURFACE_ATTRIB_MEM_TYPE_DRM_PRIME_2)
    {
        return VA_STATUS_ERROR_UNSUPPORTED_MEMORY_TYPE;
    }

    vaDataTX.vaData.vaFunctionID = HDDLVAExportSurfaceHandle;
    vaDataTX.vaData.size = sizeof (HDDLVAExportSurfaceHandleTX);
    vaDataTX.surfaceId = surfaceId;
    vaDataTX.memType = memType;
    vaDataTX.flags = flags;

    commStatus = Comm_Submission (commCtx, HDDLVAExportSurfaceHandle, COMM_READ_FULL,
        sizeof (HDDLVAExportSurfaceHandleTX), (void *)&vaDataTX,
        sizeof (HDDLVAExportSurfaceHandleRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVAExportSurfaceHandle) ||
        (vaDataRX.vaData.size != sizeof (HDDLVAExportSurfaceHandleRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;
    HDDLMemoryMgr_Memcpy (descriptor, &vaDataRX.descriptor, sizeof (VADRMPRIMESurfaceDescriptor),
        sizeof (vaDataRX.descriptor));

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_CreateBuffer2 (VADriverContextP ctx, VAContextID context, VABufferType type,
    unsigned int width, unsigned int height, unsigned int *unitSize, unsigned int *pitch,
    VABufferID *bufId)
{
    HDDLVACreateBuffer2TX vaDataTX;
    HDDLVACreateBuffer2RX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "nullptr ctx", VA_STATUS_ERROR_INVALID_CONTEXT);

    SHIM_CHK_ZERO (width, "0 width", VA_STATUS_ERROR_INVALID_BUFFER);
    SHIM_CHK_ZERO (height, "0 height", VA_STATUS_ERROR_INVALID_BUFFER);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    vaDataTX.vaData.vaFunctionID = HDDLVACreateBuffer2;
    vaDataTX.vaData.size = sizeof (HDDLVACreateBuffer2TX);
    vaDataTX.context = context;
    vaDataTX.type = type;
    vaDataTX.width = width;
    vaDataTX.height = height;

    commStatus = Comm_Submission (commCtx, HDDLVACreateBuffer2, COMM_READ_FULL,
        sizeof (HDDLVACreateBuffer2TX), (void *)&vaDataTX,
        sizeof (HDDLVACreateBuffer2RX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVACreateBuffer2) ||
        (vaDataRX.vaData.size != sizeof (HDDLVACreateBuffer2RX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;
    SHIM_CHK_ERROR (vaStatus, "VA status failed", VA_STATUS_ERROR_UNKNOWN);

    *unitSize = vaDataRX.unitSize;
    *pitch = vaDataRX.pitch;
    *bufId = vaDataRX.bufId;

    vaStatus = HDDLVAShim_CreateInternalBufferAtHeap (vaShimCtx, context, type,
        (*pitch) * vaDataRX.unitHeight, 1, *bufId, NULL);

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_BufferSetNumElements (VADriverContextP ctx, VABufferID bufId,
    unsigned int numElement)
{
    HDDLVABufferSetNumElementsTX vaBufferSetNumElementsTX;
    HDDLVABufferSetNumElementsRX vaBufferSetNumElementsRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "nullptr ctx", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    vaBufferSetNumElementsTX.vaData.vaFunctionID = HDDLVABufferSetNumElements;
    vaBufferSetNumElementsTX.vaData.size = sizeof (HDDLVABufferSetNumElementsTX);
    vaBufferSetNumElementsTX.bufId = bufId;
    vaBufferSetNumElementsTX.numElement = numElement;

    commStatus = Comm_Submission (commCtx, HDDLVABufferSetNumElements, COMM_READ_FULL,
        sizeof (HDDLVABufferSetNumElementsTX), (void *)&vaBufferSetNumElementsTX,
        sizeof (HDDLVABufferSetNumElementsRX), (void **)&vaBufferSetNumElementsRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    // Returned function ID and size needed to match
    if ( (vaBufferSetNumElementsRX.vaData.vaFunctionID != HDDLVABufferSetNumElements) ||
        (vaBufferSetNumElementsRX.vaData.size != sizeof (HDDLVABufferSetNumElementsRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaBufferSetNumElementsRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_AcquireBufferHandle (VADriverContextP ctx, VABufferID bufId,
    VABufferInfo *bufInfo)
{
    HDDLVAAcquireBufferHandleTX vaDataTX;
    HDDLVAAcquireBufferHandleRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "nullptr ctx", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    vaDataTX.vaData.vaFunctionID = HDDLVAAcquireBufferHandle;
    vaDataTX.vaData.size = sizeof (HDDLVAAcquireBufferHandleTX);
    vaDataTX.bufId = bufId;

    commStatus = Comm_Submission (commCtx, HDDLVAAcquireBufferHandle, COMM_READ_FULL,
        sizeof (HDDLVAAcquireBufferHandleTX), (void *)&vaDataTX,
        sizeof (HDDLVAAcquireBufferHandleRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVAAcquireBufferHandle) ||
        (vaDataRX.vaData.size != sizeof (HDDLVAAcquireBufferHandleRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;
    SHIM_CHK_ERROR (vaStatus, "VA status failed", VA_STATUS_ERROR_UNKNOWN);

    HDDLMemoryMgr_Memcpy (bufInfo, &vaDataRX.bufInfo, sizeof (VABufferInfo),
        sizeof (vaDataRX.bufInfo));

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLVAShim_ReleaseBufferHandle (VADriverContextP ctx, VABufferID bufId)
{
    HDDLVAReleaseBufferHandleTX vaDataTX;
    HDDLVAReleaseBufferHandleRX vaDataRX;
    HDDLShimCommContext *commCtx;
    CommStatus commStatus;
    VAStatus vaStatus;

    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (ctx, "nullptr ctx", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    commCtx = HDDLVAShim_GetCommContext (vaShimCtx);
    SHIM_CHK_NULL (commCtx, "commCtx return NULL", VA_STATUS_ERROR_INVALID_CONTEXT);

    vaDataTX.vaData.vaFunctionID = HDDLVAReleaseBufferHandle;
    vaDataTX.vaData.size = sizeof (HDDLVAReleaseBufferHandleTX);
    vaDataTX.bufId = bufId;

    commStatus = Comm_Submission (commCtx, HDDLVAReleaseBufferHandle, COMM_READ_FULL,
        sizeof (HDDLVAReleaseBufferHandleTX), (void *)&vaDataTX,
        sizeof (HDDLVAReleaseBufferHandleRX), (void **)&vaDataRX);
    SHIM_CHK_ERROR (commStatus, "Com operation failed", VA_STATUS_ERROR_UNKNOWN);

    if ( (vaDataRX.vaData.vaFunctionID != HDDLVAReleaseBufferHandle) ||
        (vaDataRX.vaData.size != sizeof (HDDLVAReleaseBufferHandleRX)))
    {
        return VA_STATUS_ERROR_UNKNOWN;
    }

    vaStatus = vaDataRX.ret;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

// ************************************************************************************************
// Internal utilities functions
// ************************************************************************************************
HDDLVAShimDriverContext *HDDLVAShim_CreateHDDLBypassDriverContext ()
{
    return (HDDLVAShimDriverContext *)HDDLMemoryMgr_AllocAndZeroMemory (sizeof
        (HDDLVAShimDriverContext));
}

HDDLShimCommContext *HDDLVAShim_CommContextSetup (HDDLVAShimDriverContext *vaShimCtx,
    uint64_t pid, uint64_t tid, HDDLShimCommContextNew commContextNew)
{
    HDDLShimCommContext *commCtx = NULL;
    CommStatus commStatus;
    char *batchEnv = getenv ("BYPASS_BATCH_MODE");

    commCtx = (HDDLShimCommContext *)HDDLMemoryMgr_AllocAndZeroMemory (
        sizeof (HDDLShimCommContext));
    SHIM_CHK_NULL (commCtx, "Failed to allocated comm context", NULL);

    commCtx->pid = pid;
    commCtx->tid = tid;
    commCtx->batchPayload = NULL;
    IS_BATCH (commCtx) = true;

    if (batchEnv)
    {
        if (atoi (batchEnv) == 0)
        {
            IS_BATCH (commCtx) = false;
        }
    }

    if (commContextNew == MAIN_COMM_CONTEXT)
    {
        commStatus = Comm_ContextInitFromConfig (&commCtx);
        if (commStatus != COMM_STATUS_SUCCESS)
        {
            SHIM_ERROR_MESSAGE ("Invalid comm config file");
            HDDLMemoryMgr_FreeMemory (commCtx);
            return NULL;
        }

        vaShimCtx->mainCommCtx = commCtx;

	// Store the last channel being used in vaShimCtx for subsequence
	// dynamic context creation
        commStatus = Comm_GetLastChannel (commCtx, &vaShimCtx->lastChannel);
    }
    else if (commContextNew == DYNAMIC_COMM_CONTEXT)
    {
        HDDLShimCommContext *mainCommCtx = vaShimCtx->mainCommCtx;
        HDDLDynamicChannelTX vaDataTX;
        HDDLDynamicChannelRX vaDataRX;
        uint16_t tx = 0;
        uint16_t rx = 0;

        commStatus = Comm_DynamicContextInit (&commCtx, mainCommCtx, &vaShimCtx->lastChannel,
            &tx, &rx);
        if (commStatus != COMM_STATUS_SUCCESS)
        {
            SHIM_ERROR_MESSAGE ("Fail to init dynamic context");
            HDDLMemoryMgr_FreeMemory (commCtx);
            return NULL;
        }

        vaDataTX.vaData.vaFunctionID = HDDLDynamicChannelID;
        vaDataTX.vaData.size = sizeof (HDDLDynamicChannelTX);
        vaDataTX.channelTX = rx;
        vaDataTX.channelRX = tx;

        commStatus = Comm_Submission (mainCommCtx, HDDLDynamicChannelID, COMM_READ_FULL,
            sizeof (HDDLDynamicChannelTX), (void *)&vaDataTX,
            sizeof (HDDLDynamicChannelRX), (void **)&vaDataRX);

        if (commStatus != COMM_STATUS_SUCCESS)
        {
            HDDLMemoryMgr_FreeMemory (commCtx);
            return NULL;
        }

        if ( (vaDataRX.vaData.vaFunctionID != HDDLDynamicChannelID) ||
            (vaDataRX.vaData.size != sizeof (HDDLDynamicChannelRX)))
        {
            HDDLMemoryMgr_FreeMemory (commCtx);
            return NULL;
        }

        COMM_MODE (commCtx) = mainCommCtx->commMode;
    }

    // Batching Mode is not supported for TCP communication. Turning off Batching
    // Mode if it has been set.
    if (IS_TCP_MODE (commCtx))
    {
        SHIM_NORMAL_MESSAGE ("Turning off Batching Mode in TCP communication");
        IS_BATCH (commCtx) = false;
    }
    SHIM_NORMAL_MESSAGE ("Batching Mode: %d", IS_BATCH (commCtx));

    commStatus = Comm_Initialize (commCtx, HOST);
    if (commStatus != COMM_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Error initializing communication settings");
        HDDLMemoryMgr_FreeMemory (commCtx);
        return NULL;
    }

    commStatus = Comm_Connect (commCtx, HOST);
    if (commStatus != COMM_STATUS_SUCCESS)
    {
        SHIM_ERROR_MESSAGE ("Failed to connect to communication settings");
        Comm_MutexDestroy (commCtx);
	Comm_CloseSocket (commCtx, HOST);
        HDDLMemoryMgr_FreeMemory (commCtx);
        return NULL;
    }

    return commCtx;
}

HDDLShimCommContext *HDDLVAShim_GetCommContext (HDDLVAShimDriverContext *vaShimCtx)
{
    HDDLShimCommContext *commCtx = NULL;
    HDDLShimCommContext *mainCommCtx = vaShimCtx->mainCommCtx;

    // TODO: Temporary disable this piece of code until UNITE and TCP code is
    // ready
#if 0
    HDDLCommContextElement *commCtxElement = NULL;
    uint64_t pid;
    uint64_t tid;
    uint32_t commCtxId;
#endif

    SHIM_CHK_NULL (mainCommCtx, "Null main comm context", NULL);

    commCtx = mainCommCtx;

    // TODO: Temporary disable this piece of code until UNITE and TCP code is
    // ready
#if 0
    pid = getpid ();
    tid = syscall (SYS_gettid);

    // Case 1: Caller is in the sharing the same thread as the main context
    if ( (mainCommCtx->pid == pid) && (mainCommCtx->tid == tid))
    {
        return mainCommCtx;
    }

    // Case 2: Caller is from different thread but we already have a dynamic
    // comm context created for this pid + tid combination
    commCtx = HDDLMemoryMgr_GetCommContext (vaShimCtx, pid, tid, &commCtxId);

    // Case 3: Caller is from different thread and there's no dynamic comm
    // context being created yet. Create a new comm context for this.
    if (!commCtx)
    {
        HDDLThreadMgr_LockMutex (&vaShimCtx->contextMutex);

        commCtx = HDDLVAShim_CommContextSetup (vaShimCtx, pid, tid, DYNAMIC_COMM_CONTEXT);
        if (commCtx == NULL)
        {
            SHIM_ERROR_MESSAGE ("Failed to setup comm context");
            HDDLThreadMgr_UnlockMutex (&vaShimCtx->contextMutex);
            return NULL;
        }

        commCtxElement = HDDLMemoryMgr_AllocCommContextHeap (vaShimCtx->contextHeap);
        if (!commCtxElement)
        {
            HDDLThreadMgr_UnlockMutex (&vaShimCtx->contextMutex);
            return NULL;
        }

        commCtxElement->pCommContext = commCtx;
        vaShimCtx->uiNumContext++;

        HDDLThreadMgr_UnlockMutex (&vaShimCtx->contextMutex);

        SHIM_NORMAL_MESSAGE ("Create new comm context for pid %lu tid %lu", pid, tid);
    }
#endif

    return commCtx;
}

VAStatus HDDLVAShim_HeapInit (HDDLVAShimDriverContext *vaShimCtx)
{
    vaShimCtx->bufferHeap = (HDDLVAHeap *)HDDLMemoryMgr_AllocAndZeroMemory (sizeof (HDDLVAHeap));
    if (vaShimCtx->bufferHeap == NULL)
    {
        HDDLMemoryMgr_FreeMemory (vaShimCtx);
        return VA_STATUS_ERROR_ALLOCATION_FAILED;
    }
    vaShimCtx->bufferHeap->uiElementSize = sizeof (HDDLVABufferElement);

    vaShimCtx->imageHeap = (HDDLVAHeap *)HDDLMemoryMgr_AllocAndZeroMemory (sizeof (HDDLVAHeap));
    if (vaShimCtx->imageHeap == NULL)
    {
        HDDLMemoryMgr_FreeMemory (vaShimCtx);
        return VA_STATUS_ERROR_ALLOCATION_FAILED;
    }
    vaShimCtx->imageHeap->uiElementSize = sizeof (HDDLVAImageElement);

    vaShimCtx->contextHeap = (HDDLVAHeap *)HDDLMemoryMgr_AllocAndZeroMemory (sizeof (HDDLVAHeap));
    if (vaShimCtx->contextHeap == NULL)
    {
        HDDLMemoryMgr_FreeMemory (vaShimCtx);
        return VA_STATUS_ERROR_ALLOCATION_FAILED;
    }
    vaShimCtx->contextHeap->uiElementSize = sizeof (HDDLCommContextElement);

    return VA_STATUS_SUCCESS;
}

VAStatus HDDLVAShim_CreateInternalBufferAtHeap (HDDLVAShimDriverContext *vaShimCtx,
    VAContextID context, VABufferType type, unsigned int size, unsigned int numElement,
    VABufferID bufId, void *data)
{
    HDDLVABuffer *vaBuffer;
    HDDLVABufferElement *vaBufferElement;
    uint32_t bufSize = 0;

    HDDLThreadMgr_LockMutex (&vaShimCtx->bufferMutex);

    vaBuffer = (HDDLVABuffer *)HDDLMemoryMgr_AllocAndZeroMemory (sizeof (HDDLVABuffer));

    if (vaBuffer == NULL)
    {
        SHIM_ERROR_MESSAGE ("vaBuffer returned NULL");
        HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
        return VA_STATUS_ERROR_ALLOCATION_FAILED;
    }

    vaBuffer->bufId = bufId;
    vaBuffer->context = context;
    vaBuffer->type = type;
    vaBuffer->uiSize = size;
    vaBuffer->uiNumElement = numElement;

    switch ( (int32_t)type)
    {
        case VAPictureParameterBufferType:
        case VAIQMatrixBufferType:
        case VAQMatrixBufferType:
        case VABitPlaneBufferType:
        case VASliceGroupMapBufferType:
        case VASliceParameterBufferType:
        case VASliceDataBufferType:
        case VAMacroblockParameterBufferType:
        case VAResidualDataBufferType:
        case VADeblockingParameterBufferType:
        case VAImageBufferType:
        case VAEncSequenceParameterBufferType:
        case VAEncPictureParameterBufferType:
        case VAEncSliceParameterBufferType:
        case VAEncPackedHeaderParameterBufferType:
        case VAEncPackedHeaderDataBufferType:
        case VAEncMiscParameterBufferType:
        case VAProcPipelineParameterBufferType:
        case VAProcFilterParameterBufferType:
        case VAHuffmanTableBufferType:
        case VAProbabilityBufferType:
        case VAEncMacroblockMapBufferType:
        case VAEncQPBufferType:
        case VAEncFEIMVBufferType:
        case VAEncFEIMBCodeBufferType:
        case VAEncFEIDistortionBufferType:
        case VAEncFEIMBControlBufferType:
        case VAEncFEIMVPredictorBufferType:
        case VAStatsStatisticsParameterBufferType:
        case VAStatsStatisticsBufferType:
        case VAStatsStatisticsBottomFieldBufferType:
        case VAStatsMVBufferType:
        case VAStatsMVPredictorBufferType:
#ifdef USE_HANTRO
        case HANTROEncROIMapBufferType:
        case HANTROEncMBCUOutputInfoBufferType:
        case HANTRODecEmbeddedPostprocessParameterBufferType:
        case HANTRODecMiscParameterBufferType:
        case HANTROEncPSNROutputInfoBufferType:
#ifndef KMB
        case HANTROEncCuCtrlBufferType:
#endif
#endif
            bufSize = sizeof (unsigned char) * size * numElement;
            vaBuffer->pData = HDDLMemoryMgr_AllocAndZeroMemory (bufSize);

            // Data might be NULL. Copy data only when it's not null to avoid segfault
            if (data)
            {
                HDDLMemoryMgr_Memcpy ( (void *)vaBuffer->pData, data, bufSize,
		    sizeof (unsigned char) * size * numElement);
            }
            else
            {
		HDDLMemoryMgr_ZeroMemory ((void *)vaBuffer->pData, bufSize);
            }
            break;

        // We need to explictily handling VAEncCodedBufferType. Since the data should
        // be a linked list of VACodedBufferSegment, we should only allocate and construct
        // vaBuffer->pData during vaMapBuffer call.
        case VAEncCodedBufferType:
            vaBuffer->pData = NULL;
            break;

        default:
            HDDLMemoryMgr_FreeMemory (vaBuffer);
            HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
            return VA_STATUS_ERROR_UNSUPPORTED_BUFFERTYPE;
    }

    // Count total buffer created
    vaShimCtx->uiNumBuffer++;

    vaBufferElement = HDDLMemoryMgr_AllocBufferHeap (vaShimCtx->bufferHeap);
    if (vaBufferElement == NULL)
    {
        HDDLMemoryMgr_FreeMemory (vaBuffer);
        HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
        return VA_STATUS_ERROR_ALLOCATION_FAILED;
    }

    // Store user info in created element
    vaBufferElement->pBuf = vaBuffer;

    HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);

    return VA_STATUS_SUCCESS;
}

VAStatus HDDLVAShim_MapInternalBuffer (HDDLVAShimDriverContext *vaShimCtx, VABufferID bufId,
    HDDLVABuffer *vaBuffer, void **buf)
{
    uint32_t hostBufId;

    if (!vaBuffer)
    {
        vaBuffer = HDDLMemoryMgr_GetBufferFromVABufferID (vaShimCtx, bufId, &hostBufId);
    }
    SHIM_CHK_NULL (vaBuffer, "Unable to get VA Buffer from heap for internal mapping",
        VA_STATUS_ERROR_ALLOCATION_FAILED;);

    switch ( (int32_t)vaBuffer->type)
    {
        case VAPictureParameterBufferType:
        case VAIQMatrixBufferType:
        case VAQMatrixBufferType:
        case VABitPlaneBufferType:
        case VASliceGroupMapBufferType:
        case VASliceParameterBufferType:
        case VASliceDataBufferType:
        case VAMacroblockParameterBufferType:
        case VAResidualDataBufferType:
        case VADeblockingParameterBufferType:
        case VAImageBufferType:
        case VAEncCodedBufferType:
        case VAEncSequenceParameterBufferType:
        case VAEncPictureParameterBufferType:
        case VAEncSliceParameterBufferType:
        case VAEncPackedHeaderParameterBufferType:
        case VAEncPackedHeaderDataBufferType:
        case VAEncMiscParameterBufferType:
        case VAProcPipelineParameterBufferType:
        case VAProcFilterParameterBufferType:
        case VAHuffmanTableBufferType:
        case VAProbabilityBufferType:
        case VAEncMacroblockMapBufferType:
        case VAEncQPBufferType:
        case VAEncFEIMVBufferType:
        case VAEncFEIMBCodeBufferType:
        case VAEncFEIDistortionBufferType:
        case VAEncFEIMBControlBufferType:
        case VAEncFEIMVPredictorBufferType:
        case VAStatsStatisticsParameterBufferType:
        case VAStatsStatisticsBufferType:
        case VAStatsStatisticsBottomFieldBufferType:
        case VAStatsMVBufferType:
        case VAStatsMVPredictorBufferType:
#ifdef USE_HANTRO
        case HANTROEncROIMapBufferType:
        case HANTROEncMBCUOutputInfoBufferType:
        case HANTRODecEmbeddedPostprocessParameterBufferType:
        case HANTRODecMiscParameterBufferType:
        case HANTROEncPSNROutputInfoBufferType:
#ifndef KMB
        case HANTROEncCuCtrlBufferType:
#endif
#endif
            *buf = HDDLMemoryMgr_LockBuffer (vaBuffer);
            break;

        default:
            *buf = (void *)vaBuffer->pData;
            return VA_STATUS_ERROR_UNSUPPORTED_BUFFERTYPE;
    }

    return VA_STATUS_SUCCESS;
}

VAStatus HDDLVAShim_UnmapInternalBuffer (HDDLVAShimDriverContext *vaShimCtx, VABufferID bufId,
    HDDLVABuffer *vaBuffer)
{
    uint32_t hostBufId;

    if (!vaBuffer)
    {
        vaBuffer = HDDLMemoryMgr_GetBufferFromVABufferID (vaShimCtx, bufId, &hostBufId);
    }
    SHIM_CHK_NULL (vaBuffer, "Unable to get VA Buffer from heap for internal unmapping",
        VA_STATUS_ERROR_ALLOCATION_FAILED;);

    switch ( (int32_t)vaBuffer->type)
    {
        case VAPictureParameterBufferType:
        case VAIQMatrixBufferType:
        case VAQMatrixBufferType:
        case VABitPlaneBufferType:
        case VASliceGroupMapBufferType:
        case VASliceParameterBufferType:
        case VASliceDataBufferType:
        case VAMacroblockParameterBufferType:
        case VAResidualDataBufferType:
        case VADeblockingParameterBufferType:
        case VAImageBufferType:
        case VAEncCodedBufferType:
        case VAEncSequenceParameterBufferType:
        case VAEncPictureParameterBufferType:
        case VAEncSliceParameterBufferType:
        case VAEncPackedHeaderParameterBufferType:
        case VAEncPackedHeaderDataBufferType:
        case VAEncMiscParameterBufferType:
        case VAProcPipelineParameterBufferType:
        case VAProcFilterParameterBufferType:
        case VAHuffmanTableBufferType:
        case VAProbabilityBufferType:
        case VAEncMacroblockMapBufferType:
        case VAEncQPBufferType:
        case VAEncFEIMVBufferType:
        case VAEncFEIMBCodeBufferType:
        case VAEncFEIDistortionBufferType:
        case VAEncFEIMBControlBufferType:
        case VAEncFEIMVPredictorBufferType:
        case VAStatsStatisticsParameterBufferType:
        case VAStatsStatisticsBufferType:
        case VAStatsStatisticsBottomFieldBufferType:
        case VAStatsMVBufferType:
        case VAStatsMVPredictorBufferType:
#ifdef USE_HANTRO
        case HANTROEncROIMapBufferType:
        case HANTROEncMBCUOutputInfoBufferType:
        case HANTRODecEmbeddedPostprocessParameterBufferType:
        case HANTRODecMiscParameterBufferType:
        case HANTROEncPSNROutputInfoBufferType:
#ifndef KMB
        case HANTROEncCuCtrlBufferType:
#endif
#endif
            HDDLMemoryMgr_UnlockBuffer (vaBuffer);
            break;

        default:
            HDDLMemoryMgr_FreeMemory (vaBuffer->pData);
            return VA_STATUS_ERROR_UNSUPPORTED_BUFFERTYPE;
    }

    return VA_STATUS_SUCCESS;
}

VAStatus HDDLVAShim_ReleaseInternalBufferFromHeap (HDDLVAShimDriverContext *vaShimCtx,
    VABufferID bufId)
{
    HDDLVABuffer *vaBuffer;
    uint32_t hostBufId;

    HDDLThreadMgr_LockMutex (&vaShimCtx->bufferMutex);

    vaBuffer = HDDLMemoryMgr_GetBufferFromVABufferID (vaShimCtx, bufId, &hostBufId);

    if (NULL == vaBuffer)
    {
        HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);
        SHIM_ERROR_MESSAGE ("va buffer not exist");
        return VA_STATUS_ERROR_ALLOCATION_FAILED;
    }

    if (!vaBuffer->bMapped)
    {
        HDDLMemoryMgr_ReleaseBufferElement (vaShimCtx->bufferHeap, hostBufId);

        if (vaBuffer->type == VAEncCodedBufferType)
        {
            if (vaBuffer->pData)
            {
                HDDLVAShim_DestroyInternalVAEncCodedBuffer (vaBuffer);
            }
        }
        else
        {
            HDDLMemoryMgr_FreeMemory (vaBuffer->pData);
            vaBuffer->pData = NULL;
        }

        HDDLMemoryMgr_FreeMemory (vaBuffer);
        vaShimCtx->uiNumBuffer--;
    }
    else
    {
        SHIM_ERROR_MESSAGE ("Try to free a mapped buffer: type %d", vaBuffer->type);
    }

    HDDLThreadMgr_UnlockMutex (&vaShimCtx->bufferMutex);

    return VA_STATUS_SUCCESS;
}

VAStatus HDDLVAShim_DestroyInternalVAEncCodedBuffer (HDDLVABuffer *vaBuffer)
{
    VACodedBufferSegment *segment = (VACodedBufferSegment *)vaBuffer->pData;
    VACodedBufferSegment *next;

    while (segment)
    {
        next = segment->next;
        HDDLMemoryMgr_FreeMemory (segment->buf);
        HDDLMemoryMgr_FreeMemory (segment);
        segment = next;
    }

    vaBuffer->pData = NULL;

    return VA_STATUS_SUCCESS;
}

VAStatus HDDLVAShim_DestroyBufferHeap (VADriverContextP ctx)
{
    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAHeap *bufferHeap = vaShimCtx->bufferHeap;
    SHIM_CHK_NULL (bufferHeap, "nullptr bufferHeap", VA_STATUS_ERROR_INVALID_CONTEXT);

    int32_t bufNum = vaShimCtx->uiNumBuffer;

    if (bufNum > 0)
    {
        HDDLVABufferElement *bufferHeapBase = (HDDLVABufferElement *)bufferHeap->pHeapBase;
        SHIM_CHK_NULL (bufferHeapBase, "nullptr bufferHeapBase", VA_STATUS_ERROR_INVALID_CONTEXT);

        for (int32_t elementId = 0; elementId < bufNum; ++elementId)
        {
            HDDLVABufferElement *bufferElement = &bufferHeapBase[elementId];
            SHIM_CHK_NULL (bufferElement->pBuf, "nullptr buffer", VA_STATUS_ERROR_INVALID_CONTEXT);
            HDDLVAShim_DestroyBuffer (ctx, bufferElement->pBuf->bufId);
        }
    }

    return VA_STATUS_SUCCESS;
}

VAStatus HDDLVAShim_DestroyImageHeap (VADriverContextP ctx)
{
    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAHeap *imageHeap = vaShimCtx->imageHeap;
    SHIM_CHK_NULL (imageHeap, "nullptr imageHeap", VA_STATUS_ERROR_INVALID_CONTEXT);

    int32_t imageNum = vaShimCtx->uiNumImage;

    if (imageNum > 0)
    {
        HDDLVAImageElement *imageHeapBase = (HDDLVAImageElement *)imageHeap->pHeapBase;

        for (int32_t elementId = 0; elementId < imageNum; ++elementId)
        {
            HDDLVAImageElement *imageElement = &imageHeapBase[elementId];
            SHIM_CHK_NULL (&imageElement->pImage, "nullptr buffer", VA_STATUS_ERROR_INVALID_CONTEXT);
            HDDLVAShim_DestroyImage (ctx, imageElement->pImage->image_id);
        }
    }

    return VA_STATUS_SUCCESS;
}

VAStatus HDDLVAShim_DestroyContextHeap (VADriverContextP ctx)
{
    HDDLVAShimDriverContext *vaShimCtx = HDDL_GetVAShimContext (ctx);
    SHIM_CHK_NULL (vaShimCtx, "nullptr VAShimCtx", VA_STATUS_ERROR_INVALID_CONTEXT);

    HDDLVAHeap *ctxHeap = vaShimCtx->contextHeap;
    SHIM_CHK_NULL (ctxHeap, "nullptr ctxHeap", VA_STATUS_ERROR_INVALID_CONTEXT);

    int32_t ctxNum = vaShimCtx->uiNumContext;

    HDDLShimCommContext *commCtx;

    if (ctxNum > 0)
    {
        HDDLCommContextElement *ctxHeapBase = (HDDLCommContextElement *)ctxHeap->pHeapBase;
        SHIM_CHK_NULL (ctxHeapBase, "nullptr ctxHeapBase", VA_STATUS_ERROR_INVALID_CONTEXT);

        for (int32_t elementId = 0; elementId < ctxNum; ++elementId)
        {
            HDDLCommContextElement *ctxElement = &ctxHeapBase[elementId];
            SHIM_CHK_NULL (&ctxElement->pCommContext, "nullptr context", VA_STATUS_ERROR_INVALID_CONTEXT);

            commCtx = ctxElement->pCommContext;

            HDDLThreadMgr_LockMutex (&vaShimCtx->contextMutex);

            HDDLMemoryMgr_ReleaseCommContextElement (vaShimCtx->contextHeap, elementId);
            vaShimCtx->uiNumContext--;

            HDDLThreadMgr_UnlockMutex (&vaShimCtx->contextMutex);

            HDDLMemoryMgr_FreeMemory (commCtx);
        }
    }

    return VA_STATUS_SUCCESS;
}

// ************************************************************************************************
// Unimplemented VA Function
// ************************************************************************************************
VAStatus HDDLVAShim_PutSurface (VADriverContextP ctx, VASurfaceID surface, void *draw, short srcX,
    short srcY, unsigned short srcW, unsigned short srcH, short destX, short destY,
    unsigned short destW, unsigned short destH, VARectangle *cliprect, unsigned int numberCliprect,
    unsigned int flag)
{
    SHIM_FUNCTION_ENTER ();
    return VA_STATUS_ERROR_UNIMPLEMENTED;
}

VAStatus HDDLVAShim_QuerySubpictureFormats (VADriverContextP ctx, VAImageFormat *formatlist,
    unsigned int *flag, unsigned int *numformat)
{
    SHIM_FUNCTION_ENTER ();
    return VA_STATUS_ERROR_UNIMPLEMENTED;
}

VAStatus HDDLVAShim_CreateSubpicture (VADriverContextP ctx, VAImageID image,
    VASubpictureID *subpicture)
{
    SHIM_FUNCTION_ENTER ();
    return VA_STATUS_ERROR_UNIMPLEMENTED;
}

VAStatus HDDLVAShim_DestroySubpicture (VADriverContextP ctx, VASubpictureID subpicture)
{
    SHIM_FUNCTION_ENTER ();
    return VA_STATUS_ERROR_UNIMPLEMENTED;
}

VAStatus HDDLVAShim_SetSubpictureImage (VADriverContextP ctx, VASubpictureID subpicture,
    VAImageID image)
{
    SHIM_FUNCTION_ENTER ();
    return VA_STATUS_ERROR_UNIMPLEMENTED;
}

VAStatus HDDLVAShim_SetSubpictureChromakey (VADriverContextP ctx, VASubpictureID subpicture,
    unsigned int chromakeyMin, unsigned int chromakeyMax, unsigned int chromakeyMask)
{
    SHIM_FUNCTION_ENTER ();
    return VA_STATUS_ERROR_UNIMPLEMENTED;
}

VAStatus HDDLVAShim_SetSubpictureGlobalAlpha (VADriverContextP ctx, VASubpictureID subpicture,
    float globalAlpha)
{
    SHIM_FUNCTION_ENTER ();
    return VA_STATUS_ERROR_UNIMPLEMENTED;
}

VAStatus HDDLVAShim_AssociateSubpicture (VADriverContextP ctx, VASubpictureID subpicture,
    VASurfaceID *targetSurface, int numSurface, short srcX, short srcY, unsigned short srcWidth,
    unsigned short srcHeight, short destX, short destY, unsigned short destWidth,
    unsigned short destHeight, unsigned int flag)
{
    SHIM_FUNCTION_ENTER ();
    return VA_STATUS_ERROR_UNIMPLEMENTED;
}

VAStatus HDDLVAShim_DeassociateSubpicture (VADriverContextP ctx, VASubpictureID subpicture,
    VASurfaceID *targetSurface, int numSurface)
{
    SHIM_FUNCTION_ENTER ();
    return VA_STATUS_ERROR_UNIMPLEMENTED;
}

VAStatus HDDLVAShim_LockSurface (VADriverContextP ctx, VASurfaceID surface, unsigned int *fourcc,
    unsigned int *lumaStride, unsigned int *chromaUStride, unsigned int *chromaVStride,
    unsigned int *lumaOffset, unsigned int *chromaUOffset, unsigned int *chromaVOffset,
    unsigned int *bufferName, void **buffer)
{
    SHIM_FUNCTION_ENTER ();
    return VA_STATUS_ERROR_UNIMPLEMENTED;
}

VAStatus HDDLVAShim_UnlockSurface (VADriverContextP ctx, VASurfaceID surface)
{
    SHIM_FUNCTION_ENTER ();
    return VA_STATUS_ERROR_UNIMPLEMENTED;
}

VAStatus HDDLVAShim_SetImagePalette (VADriverContextP ctx, VAImageID image,
    unsigned char *palette)
{
    SHIM_FUNCTION_ENTER ();
    return VA_STATUS_ERROR_UNIMPLEMENTED;
}

#pragma pack(pop)
//EOF

