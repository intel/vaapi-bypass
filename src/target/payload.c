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
//! \file    payload.c
//! \brief   Payload handler for VSI LibVA
//! \details Extract Libva info from payload and call corresponding VA function in VSI.
//!

#include "payload.h"
#include "va_display.h"
#define STR_VENDOR_MAX_STRLEN 200

#pragma pack(push, 1)

bool registerVABufferNodeList (VABufferID bufferId, int32_t remoteFd, HDDLVABufferNode *list)
{
    for (int i = 0; i < HDDLVABUFFER_NODE_LIST_SIZE; i++)
    {
        if (!list[i].bufferId)
        {
            list[i].bufferId = bufferId;
            list[i].remoteFd = remoteFd;
            return true;
        }
    }
    return false;
}

bool unregisterVABufferNodeList (VABufferID bufferId, HDDLVABufferNode *list)
{
    for (int i = 0; i< HDDLVABUFFER_NODE_LIST_SIZE; i++)
    {
        if (list[i].bufferId == bufferId)
        {
            list[i].bufferId = 0;
            list[i].remoteFd = 0;
            return true;
        }
    }
    return false;
}

bool queryVABufferNodeList (VABufferID bufferId, int32_t *remoteFd, HDDLVABufferNode *list)
{
    for (int i = 0; i < HDDLVABUFFER_NODE_LIST_SIZE; i++)
    {
        if (list[i].bufferId == bufferId)
        {
            *remoteFd = list[i].remoteFd;
            return true;
        }
    }
    return false;
}

bool HDDLShim_GetGPUNodesAttributes (VADisplay *vaDpy, VAProfile profile, VAEntrypoint entrypoint,
    VAConfigAttribType type, uint32_t *value)
{
    VAConfigAttrib attribs;
    VAStatus vaStatus = VA_STATUS_ERROR_UNKNOWN;

    attribs.type = type;

    vaStatus = vaGetConfigAttributes (vaDpy, profile, entrypoint, &attribs, 1);
    if ( (attribs.value == VA_ATTRIB_NOT_SUPPORTED) || (vaStatus != VA_STATUS_SUCCESS))
    {
        return false;
    }

    *value = attribs.value;

    return true;
}

VAStatus HDDLShim_ExtractPayload (HDDLVAFunctionID functionId, HDDLShimCommContext *ctx,
    void *inPayload, void **outPayload)
{
    VAStatus vaStatus = VA_STATUS_SUCCESS;

    switch (functionId)
    {
        case HDDLVAMedia_DriverInit:
        {
            vaStatus = HDDLShim_ExtractandCallVAInit (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVATerminate:
        {
            vaStatus = HDDLShim_ExtractandCallVATerminate (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVACreateConfig:
        {
            vaStatus = HDDLShim_ExtractandCallVACreateConfig (ctx, inPayload, outPayload);
            break;
        }
        case HDDLVADestroyConfig:
        {
            vaStatus = HDDLShim_ExtractandCallVADestroyConfig (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVACreateContext:
        {
            vaStatus = HDDLShim_ExtractandCallVACreateContext (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVADestroyContext:
        {
            vaStatus = HDDLShim_ExtractandCallVADestroyContext (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVACreateSurfaces:
        {
            vaStatus = HDDLShim_ExtractandCallVACreateSurfaces (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVADestroySurfaces:
        {
            vaStatus = HDDLShim_ExtractandCallVADestroySurfaces (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVACreateBuffer:
        {
            vaStatus = HDDLShim_ExtractandCallVACreateBuffer (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVADestroyBuffer:
        {
            vaStatus = HDDLShim_ExtractandCallVADestroyBuffer (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVAMapBuffer:
        {
            vaStatus = HDDLShim_ExtractandCallVAMapBuffer (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVAUnmapBuffer:
        {
            vaStatus = HDDLShim_ExtractandCallVAUnmapBuffer (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVACreateImage:
        {
            vaStatus = HDDLShim_ExtractandCallVACreateImage (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVADeriveImage:
        {
            vaStatus = HDDLShim_ExtractandCallVADeriveImage (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVADestroyImage:
        {
            vaStatus = HDDLShim_ExtractandCallVADestroyImage (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVABeginPicture:
        {
            vaStatus = HDDLShim_ExtractandCallVABeginPicture (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVARenderPicture:
        {
            vaStatus = HDDLShim_ExtractandCallVARenderPicture (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVAEndPicture:
        {
            vaStatus = HDDLShim_ExtractandCallVAEndPicture (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVASyncSurface:
        {
            vaStatus = HDDLShim_ExtractandCallVASyncSurface (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVAQueryConfigProfiles:
        {
            vaStatus = HDDLShim_ExtractandCallVAQueryConfigProfiles (ctx->vaDpy, inPayload,
                outPayload);
            break;
        }
        case HDDLVAQueryConfigEntrypoints:
        {
            vaStatus = HDDLShim_ExtractandCallVAQueryConfigEntrypoints (ctx->vaDpy, inPayload,
                outPayload);
            break;
        }
        case HDDLVAGetConfigAttributes:
        {
            vaStatus = HDDLShim_ExtractandCallVAGetConfigAttributes (ctx->vaDpy, inPayload,
                outPayload);
            break;
        }
        case HDDLVAQueryConfigAttributes:
        {
            vaStatus = HDDLShim_ExtractandCallVAQueryConfigAttributes (ctx->vaDpy, inPayload,
                outPayload);
            break;
        }
        case HDDLVAQuerySurfaceStatus:
        {
            vaStatus = HDDLShim_ExtractandCallVAQuerySurfaceStatus (ctx->vaDpy, inPayload,
                outPayload);
            break;
        }
        case HDDLVAQueryImageFormats:
        {
            vaStatus = HDDLShim_ExtractandCallVAQueryImageFormats (ctx->vaDpy, inPayload,
                outPayload);
            break;
        }
        case HDDLVAGetImage:
        {
            vaStatus = HDDLShim_ExtractandCallVAGetImage (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVAPutImage:
        {
            vaStatus = HDDLShim_ExtractandCallVAPutImage (ctx->vaDpy, inPayload, outPayload);
            break;
        }
        case HDDLVAQueryDisplayAttributes:
        {
            vaStatus = HDDLShim_ExtractandCallVAQueryDisplayAttributes (ctx->vaDpy, inPayload,
                outPayload);
            break;
        }
        case HDDLVAGetDisplayAttributes:
        {
            vaStatus = HDDLShim_ExtractandCallVAGetDisplayAttributes (ctx->vaDpy, inPayload,
                outPayload);
            break;
        }
        case HDDLVASetDisplayAttributes:
        {
            vaStatus = HDDLShim_ExtractandCallVASetDisplayAttributes (ctx->vaDpy, inPayload,
                outPayload);
            break;
        }
        case HDDLVAQuerySurfaceAttributes:
        {
            vaStatus = HDDLShim_ExtractandCallVAQuerySurfaceAttributes (ctx->vaDpy, inPayload,
                outPayload);
            break;
        }
        case HDDLVACreateSurfaces2:
        {
            bool importFd = IS_UNITE_MODE (ctx);
	    uint32_t swDeviceId = -1;

	    if (IS_UNITE_MODE (ctx))
	    {
	        swDeviceId = ctx->uniteCtx->xLinkCtx->xLinkHandler.sw_device_id;
	    }

            vaStatus = HDDLShim_ExtractandCallVACreateSurfaces2 (ctx->vaDpy, inPayload, outPayload,
                importFd, swDeviceId);
            break;
        }
        case HDDLVAExportSurfaceHandle:
        {
            bool registerFd = IS_UNITE_MODE (ctx);
            uint64_t workloadId = IS_UNITE_MODE (ctx) ? ctx->uniteCtx->workloadId : -1;

            vaStatus = HDDLShim_ExtractandCallVAExportSurfaceHandle (ctx->vaDpy, inPayload,
                outPayload, registerFd, workloadId);
            break;
        }
        case HDDLVACreateBuffer2:
        {
            vaStatus = HDDLShim_ExtractandCallVACreateBuffer2 (ctx, inPayload, outPayload);
            break;
        }
	case HDDLVAAcquireBufferHandle:
        {
            bool registerFd = IS_UNITE_MODE (ctx);
            uint64_t workloadId = IS_UNITE_MODE (ctx) ? ctx->uniteCtx->workloadId : -1;
            HDDLVABufferNode *vaBufferNodeList = IS_UNITE_MODE (ctx) ?
                ctx->uniteCtx->HDDLVABufferNodeList : NULL;

            vaStatus = HDDLShim_ExtractandCallVAAcquireBufferHandle (ctx->vaDpy, inPayload,
                outPayload, registerFd, workloadId, vaBufferNodeList);
            break;
        }
	case HDDLVAReleaseBufferHandle:
        {
            bool unregisterFd = IS_UNITE_MODE (ctx);
            uint64_t workloadId = IS_UNITE_MODE (ctx) ? ctx->uniteCtx->workloadId : -1;
            HDDLVABufferNode* vaBufferNodeList = IS_UNITE_MODE (ctx) ?
                ctx->uniteCtx->HDDLVABufferNodeList : NULL;

            vaStatus = HDDLShim_ExtractandCallVAReleaseBufferHandle (ctx->vaDpy, inPayload,
                outPayload, unregisterFd, workloadId, vaBufferNodeList);
            break;
        }
        case HDDLVABufferSetNumElements:
        {
            vaStatus = HDDLShim_ExtractandCallVASetNumElements (ctx, inPayload, outPayload);
            break;
        }
        case HDDLDynamicChannelID:
        {
            vaStatus = HDDLShim_ExtractandCallDynamicChannelID (ctx, inPayload, outPayload);
            break;
        }
        default:
            break;
    }

    return vaStatus;
}

void *HDDLShim_MainPayloadExtraction (HDDLVAFunctionID vaFunctionId, HDDLShimCommContext *ctx,
    void *inPayload, int inSize)
{
    void *outPayload = NULL;
    HDDLVAFunctionID functionId = vaFunctionId;
    bool extract = true;
    void *payload = inPayload;
    VAStatus vaStatus = VA_STATUS_SUCCESS;
    uint32_t offset = 0;

    while (extract)
    {
        SHIM_CHK_LESS (functionId, HDDLVAMaxFunctionID, "out of boundary", outPayload);
        SHIM_CHK_NULL (payload, "nullptr input payload", outPayload);

        if (functionId == HDDLTransferBatch)
        {
            offset += sizeof (HDDLVAData);
            payload = (void *) (inPayload + offset);
            functionId = ( (HDDLVAData *)payload)->vaFunctionID;
        }

        vaStatus = HDDLShim_ExtractPayload (functionId, ctx, payload, &outPayload);

        if (vaStatus != VA_STATUS_SUCCESS)
        {
            SHIM_NORMAL_MESSAGE ("Function %d return status %d", functionId, vaStatus);

            return outPayload;
        }

        offset += ( (HDDLVAData *)payload)->size;

        // Done processing all the batched content. Stop extracting.
        if (offset == inSize)
        {
            extract = false;
        }
        else
        {
            payload = (void *) (inPayload + offset);
            functionId = ( (HDDLVAData *)payload)->vaFunctionID;
	    HDDLMemoryMgr_FreeMemory (outPayload);
        }
    }

    return outPayload;
}

VAStatus HDDLShim_ExtractandCallVAInit (VADisplay vaDpy, void *inPayload, void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAInitializeRX *vaDataRX;
    VAStatus vaStatus;
    int major_version, minor_version;
    uint32_t rxSize = sizeof (HDDLVAInitializeRX);

    // Call VA function
    vaStatus = vaInitialize (vaDpy, &major_version, &minor_version);

    VADriverContextP dpyCtx = ( (VADisplayContextP)vaDpy)->pDriverContext;

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVAMedia_DriverInit;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;
    vaDataRX->version_major = dpyCtx->version_major;
    vaDataRX->version_minor = dpyCtx->version_minor;
    vaDataRX->max_profiles = dpyCtx->max_profiles;
    vaDataRX->max_entrypoints = dpyCtx->max_entrypoints;
    vaDataRX->max_attributes = dpyCtx->max_attributes;
    vaDataRX->max_image_formats = dpyCtx->max_image_formats;
    vaDataRX->max_subpic_formats = dpyCtx->max_subpic_formats;
    vaDataRX->max_display_attributes = dpyCtx->max_display_attributes;

    HDDLMemoryMgr_Memcpy (vaDataRX->str_vendor, dpyCtx->str_vendor, sizeof (vaDataRX->str_vendor),
	strnlen (dpyCtx->str_vendor, STR_VENDOR_MAX_STRLEN));

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVATerminate (VADisplay vaDpy, void *inPayload, void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVATerminateRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVATerminateRX);

    // Call VA function
    vaStatus = vaTerminate (vaDpy);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVATerminate;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVACreateConfig (HDDLShimCommContext *ctx, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVACreateConfigTX *vaDataTX = (HDDLVACreateConfigTX *)inPayload;
    int numAttrib = vaDataTX->numAttrib;

    typedef struct {
        HDDLVACreateConfigTX vaDataTX;
        VAConfigAttrib attribList[numAttrib];
    }VADataFullTX;

    VADataFullTX *vaDataFullTX;
    HDDLVACreateConfigRX *vaDataRX;
    VAConfigID configId;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVACreateConfigRX);

    // Extract payload
    vaDataFullTX = (VADataFullTX *)inPayload;

    // Call VA function
    vaStatus = vaCreateConfig (ctx->vaDpy, vaDataFullTX->vaDataTX.profile,
        vaDataFullTX->vaDataTX.entrypoint, vaDataFullTX->attribList,
        numAttrib, &configId);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVACreateConfig;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;
    vaDataRX->configId = configId;

    ctx->profile = vaDataFullTX->vaDataTX.profile;
    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVADestroyConfig (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVADestroyConfigTX *vaDataTX = (HDDLVADestroyConfigTX *)inPayload;
    HDDLVADestroyConfigRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVADestroyConfigRX);

    // Call VA function
    vaStatus = vaDestroyConfig (vaDpy, vaDataTX->configId);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVADestroyConfig;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVACreateContext (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVACreateContextTX *vaDataTX = (HDDLVACreateContextTX *)inPayload;
    int numRenderTarget = vaDataTX->numRenderTarget;

    typedef struct {
        HDDLVACreateContextTX vaDataTX;
        VASurfaceID renderTargets[numRenderTarget];
    }VADataFullTX;

    VADataFullTX *vaDataFullTX = (VADataFullTX *)inPayload;
    HDDLVACreateContextRX *vaDataRX;
    VAContextID contextId;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVACreateContextRX);

    // Call VA function
    vaStatus = vaCreateContext (vaDpy, vaDataFullTX->vaDataTX.configId,
        vaDataFullTX->vaDataTX.pictureWidth, vaDataFullTX->vaDataTX.pictureHeight,
        vaDataFullTX->vaDataTX.flag, &vaDataFullTX->renderTargets[0],
        vaDataFullTX->vaDataTX.numRenderTarget, &contextId);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVACreateContext;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;
    vaDataRX->context = contextId;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVADestroyContext (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVADestroyContextTX *vaDataTX = (HDDLVADestroyContextTX *)inPayload;
    HDDLVADestroyContextRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVADestroyContextRX);

    // Call VA function
    vaStatus = vaDestroyContext (vaDpy, vaDataTX->context);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVADestroyContext;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVACreateSurfaces (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVACreateSurfacesTX *vaDataTX = (HDDLVACreateSurfacesTX *)inPayload;
    int numSurfaces = vaDataTX->numSurfaces;
    VAStatus vaStatus;
    uint32_t rxSize = 0;
    VASurfaceID *surfaceId = HDDLMemoryMgr_AllocMemory (sizeof (VASurfaceID) * numSurfaces);
    SHIM_CHK_NULL (surfaceId, "nullptr surfaceId", VA_STATUS_ERROR_INVALID_PARAMETER);

    // Call VA function
    vaStatus = vaCreateSurfaces (vaDpy, vaDataTX->format, vaDataTX->width, vaDataTX->height,
        surfaceId, numSurfaces, NULL, 0);

    // Return message back to host
    typedef struct {
        HDDLVACreateSurfacesRX vaDataRX;
        VASurfaceID surfaces[numSurfaces];
    }HDDLVADataFullRX;

    HDDLVADataFullRX *vaDataFullRX;
    rxSize = sizeof (HDDLVADataFullRX);

    vaDataFullRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataFullRX->vaDataRX.vaData.vaFunctionID = HDDLVACreateSurfaces;
    vaDataFullRX->vaDataRX.vaData.size = rxSize;
    vaDataFullRX->vaDataRX.ret = vaStatus;

    HDDLMemoryMgr_Memcpy (vaDataFullRX->surfaces, surfaceId, sizeof (vaDataFullRX->surfaces),
        sizeof (VASurfaceID) * numSurfaces);
    HDDLMemoryMgr_FreeMemory (surfaceId);

    *outPayload = vaDataFullRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVADestroySurfaces (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVADestroySurfacesTX *vaDataTX = (HDDLVADestroySurfacesTX *)inPayload;;
    int numSurfaces = vaDataTX->numSurfaces;
    HDDLVADestroySurfacesRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVADestroySurfacesRX);

    typedef struct {
        HDDLVADestroySurfacesTX vaDataTX;
        VASurfaceID             surfaces[numSurfaces];
    }HDDLVADataFullTX;

    HDDLVADataFullTX *vaDataFullTX = (HDDLVADataFullTX *)inPayload;

    // Call VA function
    vaStatus = vaDestroySurfaces (vaDpy, vaDataFullTX->surfaces,
        vaDataFullTX->vaDataTX.numSurfaces);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVADestroySurfaces;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVACreateBuffer (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVACreateBufferTX *vaDataTX = (HDDLVACreateBufferTX *)inPayload;
    HDDLVACreateBufferRX *vaDataRX;
    int numElement = vaDataTX->numElement;
    int size = vaDataTX->size;
    int bufferSize = size * numElement;
    VABufferID bufId;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVACreateBufferRX);

    // Buffer is empty
    if (vaDataTX->vaData.size == sizeof (HDDLVACreateBufferTX))
    {
        // Call VA function
        vaStatus = vaCreateBuffer (vaDpy, vaDataTX->context, vaDataTX->type, vaDataTX->size,
            vaDataTX->numElement, NULL, &bufId);
    }
    else
    {
        typedef struct {
            HDDLVACreateBufferTX vaDataTX;
            unsigned char data[bufferSize];
        }HDDLVADataFullTX;

        HDDLVADataFullTX *vaDataFullTX = (HDDLVADataFullTX *)inPayload;

        // Call VA function
        vaStatus = vaCreateBuffer (vaDpy, vaDataFullTX->vaDataTX.context,
            vaDataFullTX->vaDataTX.type, vaDataFullTX->vaDataTX.size,
            vaDataFullTX->vaDataTX.numElement, vaDataFullTX->data, &bufId);
    }

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVACreateBuffer;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;
    vaDataRX->bufId = bufId;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVADestroyBuffer (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVADestroyBufferTX *vaDataTX;
    HDDLVADestroyBufferRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVADestroyBufferRX);

    // Extract payload
    vaDataTX = (HDDLVADestroyBufferTX *)inPayload;

    // Call VA function
    vaStatus = vaDestroyBuffer (vaDpy, vaDataTX->bufId);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVADestroyBuffer;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAMapBuffer (VADisplay vaDpy, void *inPayload, void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    // The current vaMapBuffer call is only performed on target side if
    // it's VAEncCodedBufferType
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAMapBufferTX *vaDataTX = (HDDLVAMapBufferTX *)inPayload;
    VABufferID bufId = vaDataTX->bufId;
    VABufferType bufType = vaDataTX->bufType;
    unsigned int dataSize = vaDataTX->dataSize;
    VACodedBufferSegment *segment = NULL;
    VACodedBufferSegment *loop = NULL;
    unsigned int segmentCount = 0;
    unsigned int offset = 0;
    VAStatus vaStatus;
    uint32_t rxSize = 0;

    vaStatus = vaMapBuffer (vaDpy, bufId, (void *)&segment);

    if (bufType == VAImageBufferType)
    {
        typedef struct {
            HDDLVAMapBufferRX vaDataRX;
            unsigned char data[dataSize];
        }HDDLVADataFullRX;

        HDDLVADataFullRX *vaDataFullRX;
        rxSize = sizeof (HDDLVADataFullRX);

        vaDataFullRX = HDDLMemoryMgr_AllocMemory (rxSize);
	SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_INVALID_PARAMETER);
        vaDataFullRX->vaDataRX.vaData.vaFunctionID = HDDLVAMapBuffer;
        vaDataFullRX->vaDataRX.vaData.size = rxSize;
        vaDataFullRX->vaDataRX.dataSize = dataSize;
        vaDataFullRX->vaDataRX.ret = vaStatus;
        HDDLMemoryMgr_Memcpy ( (void *)vaDataFullRX->data, segment,
	    sizeof (vaDataFullRX->data), dataSize);

        vaStatus = vaUnmapBuffer (vaDpy, bufId);
        vaDataFullRX->vaDataRX.ret = vaStatus;

        *outPayload = vaDataFullRX;
    }
    else if (bufType == VAEncCodedBufferType)
    {
        dataSize = 0;

        // Loop through mapped segment list to obtain the total segment
        // within the linked list and the total data size within the segment
        // list.
        loop = segment;
        while (loop)
        {
            segmentCount++;
            dataSize += loop->size;
            loop = loop->next;
        }

        typedef struct {
            HDDLVAMapBufferRX vaDataRX;
            VACodedBufferSegment segment[segmentCount];
            unsigned char data[dataSize];
        }HDDLVADataFullRX;

        HDDLVADataFullRX *vaDataFullRX;
        rxSize = sizeof (HDDLVADataFullRX);

        vaDataFullRX = HDDLMemoryMgr_AllocMemory (rxSize);
	SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_INVALID_PARAMETER);
        vaDataFullRX->vaDataRX.vaData.vaFunctionID = HDDLVAMapBuffer;
        vaDataFullRX->vaDataRX.vaData.size = rxSize;
        vaDataFullRX->vaDataRX.segmentCount = segmentCount;
        vaDataFullRX->vaDataRX.dataSize = dataSize;

        // Fill up the segment list and respective buffers.
        loop = segment;
        for (int i = 0; i < segmentCount; i++)
        {
            if (!loop)
            {
                vaDataFullRX->vaDataRX.ret = VA_STATUS_ERROR_INVALID_BUFFER;
                break;
            }

            HDDLMemoryMgr_Memcpy ( (void *) (vaDataFullRX->segment + i), loop,
	        sizeof (vaDataFullRX->segment) - i, sizeof (VACodedBufferSegment));
            HDDLMemoryMgr_Memcpy ( (void *) (vaDataFullRX->data + offset), loop->buf,
		sizeof (vaDataFullRX->data) - offset, loop->size);
            offset += loop->size;
            loop = loop->next;
        }

        vaStatus = vaUnmapBuffer (vaDpy, bufId);
        vaDataFullRX->vaDataRX.ret = vaStatus;

        *outPayload = vaDataFullRX;
    }


    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAUnmapBuffer (VADisplay vaDpy, void *inPayload, void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAUnmapBufferTX *vaDataTX = (HDDLVAUnmapBufferTX *)inPayload;
    unsigned int dataSize = vaDataTX->vaData.size - sizeof (HDDLVAUnmapBufferTX);
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVAUnmapBufferRX);
    HDDLVAUnmapBufferRX *vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);

    typedef struct {
        HDDLVAUnmapBufferTX vaDataTX;
        unsigned char data[dataSize];
    }VADataFullTX;

    VADataFullTX *vaDataFullTX = (VADataFullTX *)inPayload;
    VABufferID bufId = vaDataFullTX->vaDataTX.bufId;
    void *pBuf = NULL;

    // We need to call vaMapBuffer in order to obtain the memory address for updating
    // the data transferred from IA. The only exception is VAEncCodedBufferType since
    // it is not a driver input buffer but a driver output buffer instead.
    if (vaDataTX->bufType != VAEncCodedBufferType)
    {
        vaStatus = vaMapBuffer (vaDpy, bufId, &pBuf);

        if (pBuf != NULL)
        {
            if (vaDataTX->bufType == VAEncMiscParameterBufferType)
            {
                VAEncMiscParameterBuffer *misc_param = (VAEncMiscParameterBuffer *)pBuf;
                unsigned int offset = sizeof (VAEncMiscParameterBuffer);

                if ( ( (VAEncMiscParameterBuffer *) (vaDataFullTX->data))->type
                    == VAEncMiscParameterTypeROI)
                {
                    VAEncMiscParameterBufferROI *misc_roi_param =
                                (VAEncMiscParameterBufferROI *)misc_param->data;
                    HDDLMemoryMgr_Memcpy (misc_roi_param, (void *) (vaDataFullTX->data + offset),
		        sizeof (VAEncMiscParameterBufferROI),
			sizeof (vaDataFullTX->data) - sizeof (VAEncROI) - offset);

                    offset += sizeof (VAEncMiscParameterBufferROI);
		    VAEncROI *region_roi = pBuf + offset;

                    HDDLMemoryMgr_Memcpy (region_roi, (void *) (vaDataFullTX->data + offset),
		        sizeof (VAEncROI), sizeof (vaDataFullTX->data) - offset);
                }
                else if ( (int) ( (VAEncMiscParameterBuffer *) (vaDataFullTX->data))->type
                    == HANTROEncMiscParameterTypeIPCM)
                {
                    misc_param->type = HANTROEncMiscParameterTypeIPCM;
                    HANTROEncMiscParameterBufferIPCM *misc_ipcm_param =
		        (HANTROEncMiscParameterBufferIPCM *)misc_param->data;
                    HDDLMemoryMgr_Memcpy (misc_ipcm_param, (void *) (vaDataFullTX->data + offset),
		        sizeof (HANTROEncMiscParameterBufferIPCM), sizeof (vaDataFullTX->data) -
			(sizeof (HANTRORectangle) * misc_ipcm_param->num_ipcm) - offset);

                    offset += sizeof (HANTROEncMiscParameterBufferIPCM);
                    HANTRORectangle *region_ipcm = pBuf + offset;

                    HDDLMemoryMgr_Memcpy (region_ipcm, (void *) (vaDataFullTX->data + offset),
                        sizeof (HANTRORectangle) * misc_ipcm_param->num_ipcm,
			sizeof (vaDataFullTX->data) - offset);

                    misc_ipcm_param->ipcm = region_ipcm;
                }
                else if ( (int) ( (VAEncMiscParameterBuffer *) (vaDataFullTX->data))->type
                    == HANTROEncMiscParameterTypeROI)
                {
                    misc_param->type = HANTROEncMiscParameterTypeROI;
                    HANTROEncMiscParameterBufferROI *misc_roi_param =
		        (HANTROEncMiscParameterBufferROI *)misc_param->data;
                    HDDLMemoryMgr_Memcpy (misc_roi_param, (void *) (vaDataFullTX->data + offset),
		        sizeof (HANTROEncMiscParameterBufferROI), sizeof (vaDataFullTX->data) -
			(sizeof (HANTROEncROI) * misc_roi_param->num_roi) - offset);

                    offset += sizeof (HANTROEncMiscParameterBufferROI);
                    HANTROEncROI *region_roi = pBuf + offset;

                    HDDLMemoryMgr_Memcpy (region_roi, (void *) (vaDataFullTX->data + offset),
                        sizeof (HANTROEncROI) * misc_roi_param->num_roi,
			sizeof (vaDataFullTX->data) - offset);

                    misc_roi_param->roi = region_roi;
                }
                else
                {
                    HDDLMemoryMgr_Memcpy (pBuf, vaDataFullTX->data, dataSize,
		        sizeof (vaDataFullTX->data));
                }
            }
            else if (vaDataTX->bufType == VAProcPipelineParameterBufferType)
	    {
                VAProcPipelineParameterBuffer *pipelineParam = (VAProcPipelineParameterBuffer *)pBuf;
                unsigned int offset = sizeof (VAProcPipelineParameterBuffer);

                HDDLMemoryMgr_Memcpy (pipelineParam, (void *) (vaDataFullTX->data),
		    sizeof (VAProcPipelineParameterBuffer), offset);

                uint32_t numAdditionalOutputs = pipelineParam->num_additional_outputs;

                VARectangle *surfaceRegion =
		    HDDLMemoryMgr_AllocMemory (sizeof (VARectangle) * numAdditionalOutputs);
                HDDLMemoryMgr_Memcpy (surfaceRegion, (void *) (vaDataFullTX->data + offset),
		    sizeof (VARectangle) * numAdditionalOutputs, sizeof (vaDataFullTX->data) -
		    (sizeof (VASurfaceID) * numAdditionalOutputs) - offset);
                pipelineParam->surface_region = surfaceRegion;

                offset += (sizeof (VARectangle) * numAdditionalOutputs);
                VASurfaceID *additionalOutputs =
	            HDDLMemoryMgr_AllocMemory (sizeof (VASurfaceID) * numAdditionalOutputs);
                HDDLMemoryMgr_Memcpy (additionalOutputs, (void *) (vaDataFullTX->data + offset),
		    sizeof (VASurfaceID) * numAdditionalOutputs,
		    sizeof (vaDataFullTX->data) - offset);
                pipelineParam->additional_outputs = additionalOutputs;
            }
            else
            {
                HDDLMemoryMgr_Memcpy (pBuf, vaDataFullTX->data, dataSize, sizeof (vaDataFullTX->data));
            }
        }
        else
        {
            SHIM_ERROR_MESSAGE ("nullptr buffer");
            HDDLMemoryMgr_FreeMemory (vaDataRX);
            return VA_STATUS_ERROR_INVALID_BUFFER;
        }
    }

    // Call VA function
    vaStatus = vaUnmapBuffer (vaDpy, bufId);

    vaDataRX->vaData.vaFunctionID = HDDLVAUnmapBuffer;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;
    SHIM_FUNCTION_EXIT ();

    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVACreateImage (VADisplay vaDpy, void *inPayload, void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVACreateImageTX *vaDataTX = (HDDLVACreateImageTX *)inPayload;
    HDDLVACreateImageRX *vaDataRX;
    VAImage image;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVACreateImageRX);

    // Call VA function
    vaStatus = vaCreateImage (vaDpy, &vaDataTX->format, vaDataTX->width, vaDataTX->height, &image);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVACreateImage;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;
    vaDataRX->image = image;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVADeriveImage (VADisplay vaDpy, void *inPayload, void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVADeriveImageTX *vaDataTX = (HDDLVADeriveImageTX *)inPayload;
    HDDLVADeriveImageRX *vaDataRX;
    VAImage image;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVADeriveImageRX);

    // Call VA function
    vaStatus = vaDeriveImage (vaDpy, vaDataTX->surface, &image);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVADeriveImage;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;
    vaDataRX->image = image;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVADestroyImage (VADisplay vaDpy, void *inPayload, void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVADestroyImageTX *vaDataTX = (HDDLVADestroyImageTX *)inPayload;
    HDDLVADestroyImageRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVADestroyImageRX);

    // Call VA function
    vaStatus = vaDestroyImage (vaDpy, vaDataTX->image);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVADestroyImage;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVABeginPicture (VADisplay vaDpy, void *inPayload, void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVABeginPictureTX *vaDataTX = (HDDLVABeginPictureTX *)inPayload;
    HDDLVABeginPictureRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVABeginPictureRX);

    // Call VA function
    vaStatus = vaBeginPicture (vaDpy, vaDataTX->context, vaDataTX->renderTarget);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVABeginPicture;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVARenderPicture (VADisplay vaDpy, void *inPayload, void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVARenderPictureTX *vaDataTX = (HDDLVARenderPictureTX *)inPayload;
    int numBuffer = vaDataTX->numBuffer;

    typedef struct {
        HDDLVARenderPictureTX vaDataTX;
        VABufferID buffer[numBuffer];
    }HDDLVADataFullTX;

    HDDLVADataFullTX *vaDataFullTX = (HDDLVADataFullTX *)inPayload;
    HDDLVARenderPictureRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVARenderPictureRX);

    // Call VA function
    vaStatus = vaRenderPicture (vaDpy, vaDataFullTX->vaDataTX.context, vaDataFullTX->buffer,
        vaDataFullTX->vaDataTX.numBuffer);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVARenderPicture;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAEndPicture (VADisplay vaDpy, void *inPayload, void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAEndPictureTX *vaDataTX = (HDDLVAEndPictureTX *)inPayload;
    HDDLVAEndPictureRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVAEndPictureRX);

    // Call VA function
    vaStatus = vaEndPicture (vaDpy, vaDataTX->context);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVAEndPicture;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVASyncSurface (VADisplay vaDpy, void *inPayload, void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVASyncSurfaceTX *vaDataTX = (HDDLVASyncSurfaceTX *)inPayload;
    HDDLVASyncSurfaceRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVASyncSurfaceRX);

    // Call VA function
    vaStatus = vaSyncSurface (vaDpy, vaDataTX->renderTarget);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVASyncSurface;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAQueryConfigProfiles (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAQueryConfigProfilesTX *vaDataTX = (HDDLVAQueryConfigProfilesTX *)inPayload;
    int numProfiles = vaDataTX->numProfiles;
    VAStatus vaStatus;
    uint32_t rxSize = 0;
    VAProfile *profileList = HDDLMemoryMgr_AllocMemory (sizeof (VAProfile) * numProfiles);
    SHIM_CHK_NULL (profileList, "nullptr profileList", VA_STATUS_ERROR_INVALID_PARAMETER);

    // Call VA function
    vaStatus = vaQueryConfigProfiles (vaDpy, profileList, &numProfiles);

    typedef struct {
        HDDLVAQueryConfigProfilesRX vaDataRX;
        VAProfile profileList[numProfiles];
    }HDDLVADataFullRX;

    HDDLVADataFullRX *vaDataFullRX;
    rxSize = sizeof (HDDLVADataFullRX);

    // Return message back to host
    vaDataFullRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataFullRX->vaDataRX.vaData.vaFunctionID = HDDLVAQueryConfigProfiles;
    vaDataFullRX->vaDataRX.vaData.size = rxSize;
    vaDataFullRX->vaDataRX.ret = vaStatus;
    vaDataFullRX->vaDataRX.numProfile = numProfiles;

    HDDLMemoryMgr_Memcpy (vaDataFullRX->profileList, profileList,
	sizeof (vaDataFullRX->profileList), sizeof (VAProfile) * numProfiles);

    HDDLMemoryMgr_FreeMemory (profileList);

    *outPayload = vaDataFullRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAQueryConfigEntrypoints (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAQueryConfigEntrypointsTX *vaDataTX = (HDDLVAQueryConfigEntrypointsTX *)inPayload;
    int numEntrypoint = vaDataTX->numEntrypoint;
    VAStatus vaStatus;
    uint32_t rxSize = 0;
    VAEntrypoint *entrypointList = HDDLMemoryMgr_AllocMemory(sizeof (VAEntrypoint) * numEntrypoint);
    SHIM_CHK_NULL (entrypointList, "nullptr entrypointList", VA_STATUS_ERROR_INVALID_PARAMETER);

    // Call VA function
    vaStatus = vaQueryConfigEntrypoints (vaDpy, vaDataTX->profile, entrypointList,
        &numEntrypoint);

    // Return message back to host
    typedef struct {
        HDDLVAQueryConfigEntrypointsRX vaDataRX;
        VAEntrypoint entrypointList[numEntrypoint];
    }HDDLVADataFullRX;

    HDDLVADataFullRX *vaDataFullRX;
    rxSize = sizeof (HDDLVADataFullRX);

    vaDataFullRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataFullRX->vaDataRX.vaData.vaFunctionID = HDDLVAQueryConfigEntrypoints;
    vaDataFullRX->vaDataRX.vaData.size = rxSize;
    vaDataFullRX->vaDataRX.ret = vaStatus;
    vaDataFullRX->vaDataRX.numEntrypoint = numEntrypoint;

    HDDLMemoryMgr_Memcpy (vaDataFullRX->entrypointList, entrypointList,
	sizeof (vaDataFullRX->entrypointList), sizeof (VAEntrypoint) * numEntrypoint);

    HDDLMemoryMgr_FreeMemory (entrypointList);

    *outPayload = vaDataFullRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAGetConfigAttributes (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAGetConfigAttributesTX *vaData = (HDDLVAGetConfigAttributesTX *)inPayload;
    int numAttrib = vaData->numAttrib;

    typedef struct {
        HDDLVAGetConfigAttributesTX vaDataTX;
        VAConfigAttrib attribList[numAttrib];
    }HDDLVADataFullTX;

    HDDLVADataFullTX *vaDataFullTX = (HDDLVADataFullTX *)inPayload;
    VAStatus vaStatus;
    uint32_t rxSize;

    // Call VA function
    vaStatus = vaGetConfigAttributes (vaDpy, vaDataFullTX->vaDataTX.profile,
        vaDataFullTX->vaDataTX.entrypoint, vaDataFullTX->attribList, numAttrib);

    // Return message back to host
    typedef struct {
        HDDLVAGetConfigAttributesRX vaDataRX;
        VAConfigAttrib attribList[numAttrib];
    }HDDLVADataFullRX;

    HDDLVADataFullRX *vaDataFullRX;
    rxSize = sizeof (HDDLVADataFullRX);

    vaDataFullRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataFullRX->vaDataRX.vaData.vaFunctionID = HDDLVAGetConfigAttributes;
    vaDataFullRX->vaDataRX.vaData.size = rxSize;
    vaDataFullRX->vaDataRX.ret = vaStatus;
    vaDataFullRX->vaDataRX.numAttrib = numAttrib;

    HDDLMemoryMgr_Memcpy (vaDataFullRX->attribList, vaDataFullTX->attribList,
        sizeof (vaDataFullRX->attribList), sizeof (VAConfigAttrib) * (numAttrib));

    *outPayload = vaDataFullRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAQueryConfigAttributes (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAQueryConfigAttributesTX *vaDataTX = (HDDLVAQueryConfigAttributesTX *)inPayload;
    int numAttrib = vaDataTX->numAttributes;
    VAProfile profile;
    VAEntrypoint entrypoint;
    VAStatus vaStatus;
    uint32_t rxSize = 0;
    VAConfigAttrib *attribList = HDDLMemoryMgr_AllocMemory (sizeof (VAConfigAttrib) * numAttrib);
    SHIM_CHK_NULL (attribList, "nullptr attribList", VA_STATUS_ERROR_INVALID_PARAMETER);


    // Call VA function
    vaStatus = vaQueryConfigAttributes (vaDpy, vaDataTX->configId, &profile, &entrypoint,
        attribList, &numAttrib);

    // Return message back to host
    typedef struct {
        HDDLVAQueryConfigAttributesRX vaDataRX;
        VAConfigAttrib attribList[numAttrib];
    }HDDLVADataFullRX;

    HDDLVADataFullRX *vaDataFullRX;
    rxSize = sizeof (HDDLVADataFullRX);

    vaDataFullRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataFullRX->vaDataRX.vaData.vaFunctionID = HDDLVAQueryConfigAttributes;
    vaDataFullRX->vaDataRX.vaData.size = rxSize;
    vaDataFullRX->vaDataRX.ret = vaStatus;
    vaDataFullRX->vaDataRX.profile = profile;
    vaDataFullRX->vaDataRX.entrypoint = entrypoint;
    vaDataFullRX->vaDataRX.numAttrib = numAttrib;

    HDDLMemoryMgr_Memcpy (vaDataFullRX->attribList, attribList,
        sizeof (vaDataFullRX->attribList), sizeof (VAConfigAttrib) * (numAttrib));

    HDDLMemoryMgr_FreeMemory (attribList);

    *outPayload = vaDataFullRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAQuerySurfaceStatus (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAQuerySurfaceStatusTX *vaDataTX = (HDDLVAQuerySurfaceStatusTX *)inPayload;
    HDDLVAQuerySurfaceStatusRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVAQuerySurfaceStatusRX);
    VASurfaceStatus *status = HDDLMemoryMgr_AllocMemory (sizeof (VASurfaceStatus));
    SHIM_CHK_NULL (status, "nullptr status", VA_STATUS_ERROR_INVALID_PARAMETER);

    // Call VA function
    vaStatus = vaQuerySurfaceStatus (vaDpy, vaDataTX->renderTarget, status);

    // Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVAQuerySurfaceStatus;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;
    vaDataRX->status = *status;
    HDDLMemoryMgr_FreeMemory (status);

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAQueryImageFormats (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAQueryImageFormatsTX *vaDataTX = (HDDLVAQueryImageFormatsTX *)inPayload;
    int numFormat = vaDataTX->numFormat;
    VAStatus vaStatus;
    uint32_t rxSize = 0;
    VAImageFormat *formatList = HDDLMemoryMgr_AllocMemory (sizeof (VAImageFormat) * numFormat);
    SHIM_CHK_NULL (formatList, "nullptr formatList", VA_STATUS_ERROR_INVALID_PARAMETER);

    // Call VA function
    vaStatus = vaQueryImageFormats (vaDpy, formatList, &numFormat);

    // Return message back to host
    typedef struct {
        HDDLVAQueryImageFormatsRX vaDataRX;
        VAImageFormat formatList[numFormat];
    }HDDLVADataFullRX;

    HDDLVADataFullRX *vaDataFullRX;
    rxSize = sizeof (HDDLVADataFullRX);

    vaDataFullRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataFullRX->vaDataRX.vaData.vaFunctionID = HDDLVAQueryImageFormats;
    vaDataFullRX->vaDataRX.vaData.size = rxSize;
    vaDataFullRX->vaDataRX.ret = vaStatus;
    vaDataFullRX->vaDataRX.numFormat = numFormat;

    HDDLMemoryMgr_Memcpy (vaDataFullRX->formatList, formatList,
	sizeof (vaDataFullRX->formatList), sizeof (VAImageFormat) * (numFormat));
    HDDLMemoryMgr_FreeMemory (formatList);

    *outPayload = vaDataFullRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAGetImage (VADisplay vaDpy, void *inPayload, void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAGetImageTX *vaDataTX = (HDDLVAGetImageTX *)inPayload;
    HDDLVAGetImageRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVAGetImageRX);

    //Call VSI function
    vaStatus = vaGetImage (vaDpy, vaDataTX->surface, vaDataTX->x, vaDataTX->y, vaDataTX->width,
        vaDataTX->height, vaDataTX->image);

    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVAGetImage;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAPutImage (VADisplay vaDpy, void *inPayload, void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAPutImageTX *vaDataTX = (HDDLVAPutImageTX *)inPayload;
    unsigned int bufSize = vaDataTX->bufSize;
    VABufferID bufId   = vaDataTX->bufId;
    void *pBuf = NULL;
    VAStatus vaStatus;
    uint32_t rxSize = 0;

    //Call VSI function
    vaStatus = vaPutImage (vaDpy, vaDataTX->surface, vaDataTX->image, vaDataTX->srcX,
        vaDataTX->srcY, vaDataTX->srcWidth, vaDataTX->srcHeight, vaDataTX->destX, vaDataTX->destY,
        vaDataTX->destWidth, vaDataTX->destHeight);

    vaStatus = vaMapBuffer (vaDpy, bufId, &pBuf);

    typedef struct{
        HDDLVAPutImageRX vaDataRX;
        unsigned char bufData[bufSize];
    }HDDLVADataFullRX;

    rxSize = sizeof (HDDLVADataFullRX);

    //Return info back to host
    HDDLVADataFullRX *vaDataFullRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataFullRX->vaDataRX.vaData.vaFunctionID = HDDLVAPutImage;
    vaDataFullRX->vaDataRX.vaData.size = rxSize;
    vaDataFullRX->vaDataRX.ret = vaStatus;
    HDDLMemoryMgr_Memcpy (vaDataFullRX->bufData, pBuf, sizeof (vaDataFullRX->bufData), bufSize);

    vaStatus = vaUnmapBuffer (vaDpy, bufId);

    *outPayload = vaDataFullRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAQueryDisplayAttributes (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAQueryDisplayAttributesTX *vaDataTX = (HDDLVAQueryDisplayAttributesTX *)inPayload;
    int numAttributes = vaDataTX->numAttributes;
    VAStatus vaStatus;
    uint32_t rxSize = 0;
    VADisplayAttribute *attrList = HDDLMemoryMgr_AllocMemory (sizeof (VADisplayAttribute) * numAttributes);
    SHIM_CHK_NULL (attrList, "nullptr attrList", VA_STATUS_ERROR_INVALID_PARAMETER);

    //Call VSI function
    vaStatus = vaQueryDisplayAttributes (vaDpy, attrList, &numAttributes);

    // Return info back to host
    typedef struct {
        HDDLVAQueryDisplayAttributesRX vaDataRX;
        VADisplayAttribute attrList[numAttributes];
    }HDDLVADataFullRX;

    rxSize = sizeof (HDDLVADataFullRX);
    HDDLVADataFullRX *vaDataFullRX;

    vaDataFullRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataFullRX->vaDataRX.vaData.vaFunctionID = HDDLVAQueryDisplayAttributes;
    vaDataFullRX->vaDataRX.vaData.size = rxSize;
    vaDataFullRX->vaDataRX.ret = vaStatus;
    vaDataFullRX->vaDataRX.numAttributes = numAttributes;

    HDDLMemoryMgr_Memcpy (vaDataFullRX->attrList, attrList,
	sizeof (vaDataFullRX->attrList), sizeof (VADisplayAttribute) * numAttributes);

    *outPayload = vaDataFullRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAGetDisplayAttributes (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAGetDisplayAttributesTX *vaDataTX = (HDDLVAGetDisplayAttributesTX *)inPayload;
    int numAttributes = vaDataTX->numAttributes;

    typedef struct {
        HDDLVAGetDisplayAttributesTX vaDataTX;
        VADisplayAttribute           attribList[numAttributes];
    }HDDLVADataFullTX;

    HDDLVADataFullTX *vaDataFullTX = (HDDLVADataFullTX *)inPayload;
    VAStatus vaStatus;
    uint32_t rxSize = 0;

    //Call VSI function
    vaStatus = vaGetDisplayAttributes (vaDpy, vaDataFullTX->attribList, numAttributes);

    //Return message back to host
    typedef struct {
        HDDLVAGetDisplayAttributesRX vaDataRX;
        VADisplayAttribute           attribs[numAttributes];
    }HDDLVADataFullRX;

    rxSize = sizeof (HDDLVADataFullRX);
    HDDLVADataFullRX *vaDataFullRX;

    vaDataFullRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataFullRX->vaDataRX.vaData.vaFunctionID = HDDLVAGetDisplayAttributes;
    vaDataFullRX->vaDataRX.vaData.size = rxSize;
    vaDataFullRX->vaDataRX.ret = vaStatus;

    HDDLMemoryMgr_Memcpy (vaDataFullRX->attribs, vaDataFullTX->attribList,
        sizeof (vaDataFullRX->attribs), sizeof (VADisplayAttribute) * numAttributes);

    *outPayload = vaDataFullRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVASetDisplayAttributes (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVASetDisplayAttributesTX *vaDataTX = (HDDLVASetDisplayAttributesTX *)inPayload;
    int numAttributes = vaDataTX->numAttributes;

    typedef struct {
        HDDLVASetDisplayAttributesTX vaDataTX;
        VADisplayAttribute attribs[numAttributes];
    }HDDLVADataFullTX;

    HDDLVADataFullTX *vaDataFullTX = (HDDLVADataFullTX *)inPayload;
    HDDLVASetDisplayAttributesRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVASetDisplayAttributesRX);

    //Call VSI function
    vaStatus = vaSetDisplayAttributes (vaDpy, vaDataFullTX->attribs,
        vaDataFullTX->vaDataTX.numAttributes);

    //Return info back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVASetDisplayAttributes;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAQuerySurfaceAttributes (VADisplay vaDpy, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAQuerySurfaceAttributesTX *vaDataTX = (HDDLVAQuerySurfaceAttributesTX *)inPayload;
    VASurfaceAttrib *attribList;
    unsigned int numAttribs = vaDataTX->numAttribs;
    unsigned int allocAttribSize;
    bool querySize;
    VAStatus vaStatus;
    uint32_t rxSize = 0;

    if (numAttribs > 0)
    {
        attribList = HDDLMemoryMgr_AllocMemory (sizeof (VASurfaceAttrib) * numAttribs);
        SHIM_CHK_NULL (attribList, "nullptr attribList", VA_STATUS_ERROR_INVALID_PARAMETER);
        // numAttribs is provide. Expecting return of attribList
        querySize = false;
    }
    else
    {
        attribList = NULL;
        // Only query numAttribs. Not expecting return of attribList
        querySize = true;
    }

    //call VSI function
    vaStatus = vaQuerySurfaceAttributes (vaDpy, vaDataTX->config, attribList, &numAttribs);

    if (querySize)
    {
        allocAttribSize = 0;
    }
    else
    {
        allocAttribSize = numAttribs;
    }

    //Return info back to host
    typedef struct {
        HDDLVAQuerySurfaceAttributesRX vaDataRX;
        VASurfaceAttrib attribList[allocAttribSize];
    }HDDLVADataFullRX;

    HDDLVADataFullRX *vaDataFullRX;
    rxSize = sizeof (HDDLVADataFullRX);

    vaDataFullRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataFullRX->vaDataRX.vaData.vaFunctionID = HDDLVAQuerySurfaceAttributes;
    vaDataFullRX->vaDataRX.vaData.size = rxSize;
    vaDataFullRX->vaDataRX.numAttribs = numAttribs;
    vaDataFullRX->vaDataRX.ret = vaStatus;

    if (!querySize)
    {
        HDDLMemoryMgr_Memcpy (vaDataFullRX->attribList, attribList,
	    sizeof (vaDataFullRX->attribList), sizeof (VASurfaceAttrib) * numAttribs);
        HDDLMemoryMgr_FreeMemory (attribList);
    }

    *outPayload = vaDataFullRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVACreateSurfaces2 (VADisplay vaDpy, void *inPayload,
    void **outPayload, bool importFd, uint32_t swDeviceId)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVACreateSurfaces2TX *vaDataTX = (HDDLVACreateSurfaces2TX *)inPayload;
    int numAttribs = vaDataTX->numAttribs;
    int numSurfaces = vaDataTX->numSurfaces;

    typedef struct {
        HDDLVACreateSurfaces2TX vaDataTX;
        VASurfaceAttrib attribList[numAttribs];
    }HDDLVADataFullTX;

    HDDLVADataFullTX *vaDataFullTX = (HDDLVADataFullTX *)inPayload;
    VAStatus vaStatus;
    uint32_t rxSize;
    VASurfaceID *surfaces = HDDLMemoryMgr_AllocMemory (sizeof (VASurfaceID) * numSurfaces);
    SHIM_CHK_NULL (surfaces, "nullptr surfaces", VA_STATUS_ERROR_INVALID_PARAMETER);

    //redirect attribList
    for (int i =0; i<numAttribs; i++)
    {
        if (vaDataFullTX->attribList[i].type == VASurfaceAttribExternalBufferDescriptor)
        {
            if (vaDataFullTX->attribList[i].value.type == VAGenericValueTypePointer)
            {
                vaDataFullTX->vaDataTX.externalBuffers[i].extBuf.buffers =
                    (uintptr_t *) &(vaDataFullTX->vaDataTX.externalBuffers[i].extbuf_handle[0]);
                vaDataFullTX->attribList[i].value.value.p =
                    (VASurfaceAttribExternalBuffers *) &(vaDataFullTX->vaDataTX.externalBuffers[i].extBuf);
            }
        }
    }

#ifdef HDDL_UNITE
    if (importFd)
    {
        HddlStatusCode hddlStatus;

        for (int i = 0; i < vaDataFullTX->vaDataTX.numAttribs; i++)
        {
            if (vaDataFullTX->attribList[i].type == VASurfaceAttribExternalBufferDescriptor)
            {
                VASurfaceAttribExternalBuffers *memAttribute =
                    (VASurfaceAttribExternalBuffers *)vaDataFullTX->attribList[i].value.value.p;
                for (int g = 0; g < memAttribute->num_buffers; ++g)
                {
                    int32_t originalFd;
                    int32_t remoteFd = memAttribute->buffers[g];
                    hddlStatus = importDMABuf (remoteFd, &originalFd, swDeviceId);

                    if (hddlStatus == HDDL_OK)
                    {
                        SHIM_NORMAL_MESSAGE (
                            "%s: Obtained original fd %d from HDDLUnite", __func__, originalFd);
                        memAttribute->buffers[g] = originalFd;
                    }
                    else
                    {
                        SHIM_ERROR_MESSAGE(
                            "%s: Failed to obtain original fd from HDDLUnite", __func__);
                    }
                }
            }
        }
    }
#endif

    //Call VSI function
    vaStatus = vaCreateSurfaces (vaDpy, vaDataFullTX->vaDataTX.format, vaDataFullTX->vaDataTX.width,
        vaDataFullTX->vaDataTX.height, surfaces, vaDataFullTX->vaDataTX.numSurfaces,
        vaDataFullTX->attribList, vaDataFullTX->vaDataTX.numAttribs);

// In unite mode, originalFd is created by importDMABuf() for hddlunite operation
// So the fd should be closed after use.
//TODO: FD is handled by middleware, the architecture needs to be discussed later.
#ifdef HDDL_UNITE
    if (importFd)
    {
        HddlStatusCode hddlStatus;

        for (int i = 0; i < vaDataFullTX->vaDataTX.numAttribs; i++)
        {
            if (vaDataFullTX->attribList[i].type == VASurfaceAttribExternalBufferDescriptor)
            {
                VASurfaceAttribExternalBuffers *memAttribute =
                    (VASurfaceAttribExternalBuffers *)vaDataFullTX->attribList[i].value.value.p;
                for (int g = 0; g < memAttribute->num_buffers; ++g)
                {
                    int32_t originalFd = memAttribute->buffers[g];
                    hddlStatus = closeDMABuf (originalFd);

                    if (hddlStatus == HDDL_OK)
                    {
                        SHIM_NORMAL_MESSAGE (
                            "%s: close original fd %d from HDDLUnite", __func__, originalFd);
                    }
                    else
                    {
                        SHIM_ERROR_MESSAGE(
                            "%s: Failed to close original fd %d from HDDLUnite", __func__, originalFd);
                    }
                }
            }
        }
    }
#endif

    //Return info back to host
    typedef struct {
        HDDLVACreateSurfaces2RX vaDataRX;
        VASurfaceID surfaces[numSurfaces];
    }HDDLVADataFullRX;

    HDDLVADataFullRX *vaDataFullRX;
    rxSize = sizeof (HDDLVADataFullRX);

    vaDataFullRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataFullRX, "nullptr vaDataFullRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataFullRX->vaDataRX.vaData.vaFunctionID = HDDLVACreateSurfaces2;
    vaDataFullRX->vaDataRX.vaData.size = rxSize;
    vaDataFullRX->vaDataRX.ret = vaStatus;

    HDDLMemoryMgr_Memcpy (vaDataFullRX->surfaces, surfaces, sizeof (vaDataFullRX->surfaces),
        sizeof(VASurfaceID) * (numSurfaces));
    HDDLMemoryMgr_FreeMemory (surfaces);

    *outPayload = vaDataFullRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAExportSurfaceHandle (VADisplay vaDpy, void *inPayload,
    void **outPayload, bool registerFd, uint64_t workloadId)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAExportSurfaceHandleTX *vaDataTX = (HDDLVAExportSurfaceHandleTX *)inPayload;
    HDDLVAExportSurfaceHandleRX *vaDataRX;
    VADRMPRIMESurfaceDescriptor descriptor;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVAExportSurfaceHandleRX);

    HDDLMemoryMgr_ZeroMemory (&descriptor, sizeof (VADRMPRIMESurfaceDescriptor));

    //Call VSI function
    vaStatus = vaExportSurfaceHandle (vaDpy, vaDataTX->surfaceId, vaDataTX->memType,
        vaDataTX->flags, &descriptor);

    //Return info back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVAExportSurfaceHandle;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;
    HDDLMemoryMgr_Memcpy (&vaDataRX->descriptor, &descriptor, sizeof (vaDataRX->descriptor),
	sizeof (VADRMPRIMESurfaceDescriptor));

    SHIM_NORMAL_MESSAGE ("%s: Obtained fd %d with size %d", __func__,
        vaDataRX->descriptor.objects[0].fd, vaDataRX->descriptor.objects[0].size);

#ifdef HDDL_UNITE
    if (registerFd)
    {
        int32_t remoteFd;
        HddlStatusCode hddlStatus;
        hddlStatus = registerVABuffer (workloadId, vaDataRX->descriptor.objects[0].fd,
            vaDataRX->descriptor.objects[0].size, &remoteFd);

        if (hddlStatus == HDDL_OK)
        {
            vaDataRX->descriptor.objects[0].fd = remoteFd;

            SHIM_NORMAL_MESSAGE ("%s: Obtained remote fd %d from HDDLUnite", __func__,
                vaDataRX->descriptor.objects[0].fd);
        }
        else
        {
            SHIM_ERROR_MESSAGE ("%s: Failed to register fd to HDDLUnite", __func__);
        }
    }
#endif

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVACreateBuffer2 (HDDLShimCommContext *ctx, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVACreateBuffer2TX *vaDataTX = (HDDLVACreateBuffer2TX *)inPayload;
    HDDLVACreateBuffer2RX *vaDataRX;
    unsigned int unitSize;
    unsigned int pitch;
    unsigned int bufId;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVACreateBuffer2RX);

    //Call VSI function
    vaStatus = vaCreateBuffer2 (ctx->vaDpy, vaDataTX->context, vaDataTX->type, vaDataTX->width,
        vaDataTX->height, &unitSize, &pitch, &bufId);

    //Return info back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVACreateBuffer2;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;
    vaDataRX->unitSize = unitSize;
    vaDataRX->pitch = pitch;
    vaDataRX->bufId = bufId;

    switch ( (int)ctx->profile)
    {
        case VAProfileH264ConstrainedBaseline:
        case VAProfileH264Baseline:
        case VAProfileH264Main:
        case VAProfileH264High:
#ifdef USE_HANTRO
        case HANTROProfileH264High10:
#endif
            vaDataRX->unitHeight = (vaDataTX->height + 15) / 16;
            break;

        case VAProfileHEVCMain:
        case VAProfileHEVCMain10:
#ifdef USE_HANTRO
        case HANTROProfileHEVCMainStill:
#endif
            vaDataRX->unitHeight = (vaDataTX->height + 63) / 64;
            break;

        default:
            vaDataRX->unitHeight = 0;
            break;
    }

#ifdef USE_HANTRO
    if ( (int)vaDataTX->type == HANTROEncPSNROutputInfoBufferType)
    {
        vaDataRX->unitHeight = 1;
    }
#endif

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAAcquireBufferHandle(VADisplay vaDpy, void *inPayload,
    void **outPayload, bool registerFd, uint64_t workloadId, HDDLVABufferNode *vaBufferNodeList)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAAcquireBufferHandleTX *vaDataTX = (HDDLVAAcquireBufferHandleTX *)inPayload;
    HDDLVAAcquireBufferHandleRX *vaDataRX;
    VAStatus vaStatus;
    VABufferInfo bufferInfo;
    uint32_t rxSize = sizeof (HDDLVAAcquireBufferHandleRX);

    HDDLMemoryMgr_ZeroMemory (&bufferInfo, sizeof (bufferInfo));

    //Call VA function
    vaStatus = vaAcquireBufferHandle (vaDpy, vaDataTX->bufId, &bufferInfo);

    //Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVAAcquireBufferHandle;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;
    HDDLMemoryMgr_Memcpy (&vaDataRX->bufInfo, &bufferInfo, sizeof (vaDataRX->bufInfo),
	sizeof (VABufferInfo));

    *outPayload = vaDataRX;

#ifdef HDDL_UNITE
    int32_t remoteFd;
    if (registerFd)
    {
        HddlStatusCode hddlStatus;
        hddlStatus = registerVABuffer (workloadId, vaDataRX->bufInfo.handle,
            vaDataRX->bufInfo.mem_size, &remoteFd);

        if (hddlStatus == HDDL_OK)
        {
            vaDataRX->bufInfo.handle = remoteFd;

            SHIM_NORMAL_MESSAGE ("%s: Obtained remote fd %ld from HDDLUnite", __func__,
                vaDataRX->bufInfo.handle);

            //register remoteFd&bufferId in ctx
            bool ret = registerVABufferNodeList (vaDataTX->bufId, remoteFd, vaBufferNodeList);
            if (!ret)
            {
                SHIM_ERROR_MESSAGE ("%s: Failed to register VABuffer from ctx", __func__);
            }
        }
        else
        {
            SHIM_ERROR_MESSAGE ("%s: Failed to register fd to HDDLUnite", __func__);
        }
    }
#endif

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVAReleaseBufferHandle (VADisplay vaDpy, void *inPayload,
    void **outPayload, bool unregisterFd, uint64_t workloadId, HDDLVABufferNode *vaBufferNodeList)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVAReleaseBufferHandleTX *vaDataTX = (HDDLVAReleaseBufferHandleTX *)inPayload;
    HDDLVAReleaseBufferHandleRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVAReleaseBufferHandleRX);

#ifdef HDDL_UNITE
    VABufferID bufferId = vaDataTX->bufId;
    int32_t remoteFd = -1;

    if (unregisterFd)
    {
        bool ret = queryVABufferNodeList (bufferId, &remoteFd, vaBufferNodeList);
        if (!ret)
        {
            SHIM_ERROR_MESSAGE ("%s: Failed to query remoteFd from ctx", __func__);
        }

        HddlStatusCode hddlStatus;
        hddlStatus = unregisterVABuffer (workloadId, remoteFd);

        if (hddlStatus == HDDL_OK)
        {
            SHIM_NORMAL_MESSAGE ("%s: unregister VABuffer remote fd %d from HDDLUnite", __func__,
                remoteFd);

            //unregister remoteFd&bufferId from ctx
            ret = unregisterVABufferNodeList (bufferId, vaBufferNodeList);
            if (!ret)
            {
                SHIM_ERROR_MESSAGE ("%s: Failed to unregister VABuffer from ctx", __func__);
            }
        }
        else
        {
            SHIM_ERROR_MESSAGE ("%s: Failed to unregister fd to HDDLUnite", __func__);
        }
    }
#endif

    //Call VA function
    vaStatus = vaReleaseBufferHandle (vaDpy, vaDataTX->bufId);

    //Return message back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVAReleaseBufferHandle;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallVASetNumElements (HDDLShimCommContext *ctx, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    HDDLVABufferSetNumElementsTX *vaDataTX = (HDDLVABufferSetNumElementsTX *)inPayload;
    HDDLVABufferSetNumElementsRX *vaDataRX;
    VAStatus vaStatus;
    uint32_t rxSize = sizeof (HDDLVABufferSetNumElementsRX);

    //Extract payload

    //Call VSI function
    vaStatus = vaBufferSetNumElements (ctx->vaDpy, vaDataTX->bufId, vaDataTX->numElement);

    //REturn info back to host
    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLVABufferSetNumElements;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

VAStatus HDDLShim_ExtractandCallDynamicChannelID (HDDLShimCommContext *ctx, void *inPayload,
    void **outPayload)
{
    SHIM_FUNCTION_ENTER ();
    SHIM_CHK_NULL (inPayload, "nullptr input payload", VA_STATUS_ERROR_INVALID_PARAMETER);

    VAStatus vaStatus = VA_STATUS_SUCCESS;
    HDDLDynamicChannelRX *vaDataRX;
    uint32_t rxSize = sizeof (HDDLDynamicChannelRX);

    vaDataRX = HDDLMemoryMgr_AllocMemory (rxSize);
    SHIM_CHK_NULL (vaDataRX, "nullptr vaDataRX", VA_STATUS_ERROR_INVALID_PARAMETER);
    vaDataRX->vaData.vaFunctionID = HDDLDynamicChannelID;
    vaDataRX->vaData.size = rxSize;
    vaDataRX->ret = vaStatus;

    *outPayload = vaDataRX;

    SHIM_FUNCTION_EXIT ();
    return vaStatus;
}

#pragma pack(pop)

//EOF
