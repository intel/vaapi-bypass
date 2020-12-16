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
//! \file    memory_manager.c
//! \brief   Manager which handle control memory allocation & release
//! \details Allocate & release memory for pointer
//!

#include "memory_manager.h"
#include "debug_manager.h"
#include <sys/syscall.h>

int32_t memAllocCounter = 0; // Counter to check memory leaks

void *HDDLMemoryMgr_AllocMemory (size_t size)
{
    void *ptr;
    ptr = malloc (size);

    if (ptr != NULL)
    {
        memAllocCounter++;
    }
    else
    {
        SHIM_ASSERT_MESSAGE ("Memory allocation fail");
    }

    return ptr;
}

void *HDDLMemoryMgr_AllocAndZeroMemory (size_t size)
{
    void *ptr;
    ptr = malloc (size);

    if (ptr != NULL)
    {
        HDDLMemoryMgr_ZeroMemory (ptr, size);
        memAllocCounter++;
    }
    else
    {
        SHIM_ASSERT_MESSAGE ("Memory allocation fail");
    }

    return ptr;
}

void *HDDLMemoryMgr_ReallocMemory (void *ptr, size_t newSize)
{
    void *oldPtr = ptr;
    void *newPtr = realloc (ptr, newSize);

    if (newPtr != oldPtr)
    {
        if (oldPtr != NULL)
        {
            memAllocCounter--;
            SHIM_NORMAL_MESSAGE ("nop");
        }

        if (newPtr != NULL)
        {
            memAllocCounter++;
            SHIM_NORMAL_MESSAGE ("nop");
        }
    }

    return newPtr;
}

void *HDDLMemoryMgr_ReallocAndZeroMemory (void *ptr, size_t newSize)
{
    void *oldPtr = ptr;
    void *newPtr = realloc (ptr, newSize);

    if (newPtr != oldPtr)
    {
        if (oldPtr != NULL)
        {
            memAllocCounter--;
            SHIM_NORMAL_MESSAGE ("nop");
        }

        if (newPtr != NULL)
        {
            memAllocCounter++;
            SHIM_NORMAL_MESSAGE ("nop");
        }

        HDDLMemoryMgr_ZeroMemory (newPtr, newSize);
    }

    return newPtr;
}

void HDDLMemoryMgr_ZeroMemory (void *destination, size_t length)
{
    if (destination != NULL)
    {
        memset_s (destination, length, 0);
    }
}

void HDDLMemoryMgr_FillMemory (void *destination, size_t length, uint8_t fill)
{
    if (destination != NULL)
    {
        memset_s (destination, length, fill);
    }
}

void HDDLMemoryMgr_FreeMemory (void *ptr)
{
    if (ptr != NULL)
    {
        memAllocCounter--;
        free(ptr);
	ptr = NULL;
    }
}

HDDLVABufferElement *HDDLMemoryMgr_AllocBufferHeap (HDDLVAHeap *bufferHeap)
{
    HDDLVABufferElement *bufferHeapBase;
    HDDLVABufferElement *bufferElement;
    void *empty = NULL;

    if (bufferHeap->pFirstFreeElement == NULL)
    {
        void *newHeapBase = HDDLMemoryMgr_ReallocMemory (bufferHeap->pHeapBase,
            (bufferHeap->uiElementCount + HEAP_INCREMENTAL_SIZE) * sizeof (HDDLVABufferElement));
        SHIM_CHK_NULL (newHeapBase, "nullptr newheapbase", empty);

        bufferHeap->pHeapBase = newHeapBase;
        bufferHeapBase = (HDDLVABufferElement *)bufferHeap->pHeapBase;
        bufferHeap->pFirstFreeElement = (void*) (&bufferHeapBase[bufferHeap->uiElementCount]);

        for (int32_t i = 0; i < (HEAP_INCREMENTAL_SIZE); i++)
        {
            // move to next element
            bufferElement = &bufferHeapBase[bufferHeap->uiElementCount + i];
            // set next-to-free element. Return null if last element.
            bufferElement->pNextFree = (i == (HEAP_INCREMENTAL_SIZE - 1)) ? NULL :
                &bufferHeapBase[bufferHeap->uiElementCount + i + 1];
        }
        bufferHeap->uiElementCount += HEAP_INCREMENTAL_SIZE; // move to next heap
    }

    // restore address of first freed element
    bufferElement = (HDDLVABufferElement *)bufferHeap->pFirstFreeElement;
    // move to next-to-free element
    bufferHeap->pFirstFreeElement = bufferElement->pNextFree;

    return bufferElement;
}

HDDLVAImageElement *HDDLMemoryMgr_AllocImageHeap (HDDLVAHeap *imageHeap)
{
    HDDLVAImageElement *imageElement;
    void *empty = NULL;

    if (imageHeap->pFirstFreeElement == NULL)
    {
        void *newHeapBase = HDDLMemoryMgr_ReallocMemory (imageHeap->pHeapBase,
            (imageHeap->uiElementCount + HEAP_INCREMENTAL_SIZE) * sizeof(HDDLVAImageElement));
        SHIM_CHK_NULL (newHeapBase, "nullptr newheapbase", empty);

        imageHeap->pHeapBase = newHeapBase;
        HDDLVAImageElement *imageHeapBase = (HDDLVAImageElement *)imageHeap->pHeapBase;
        imageHeap->pFirstFreeElement = (void*)(&imageHeapBase[imageHeap->uiElementCount]);

        for (int32_t i = 0; i < (HEAP_INCREMENTAL_SIZE); i++)
        {
            // select heap element with i counter
            imageElement = &imageHeapBase[imageHeap->uiElementCount + i];
            // set next-to-free heap element. if it is last element, then return null.
            imageElement->pNextFree = (i == (HEAP_INCREMENTAL_SIZE - 1)) ? NULL :
                &imageHeapBase[imageHeap->uiElementCount + i + 1];
        }
        imageHeap->uiElementCount += HEAP_INCREMENTAL_SIZE;
    }

    // restore address of first heap element which freed
    imageElement = (HDDLVAImageElement *)imageHeap->pFirstFreeElement;
    // clear the saved address
    imageHeap->pFirstFreeElement = imageElement->pNextFree;
    return imageElement;
}

HDDLCommContextElement *HDDLMemoryMgr_AllocCommContextHeap (HDDLVAHeap *contextHeap)
{
    HDDLCommContextElement *commCtxElement;
    void *empty = NULL;

    if (contextHeap->pFirstFreeElement == NULL)
    {
        void *newHeapBase = HDDLMemoryMgr_ReallocMemory (contextHeap->pHeapBase,
            (contextHeap->uiElementCount + HEAP_INCREMENTAL_SIZE) * sizeof (HDDLCommContextElement));
        SHIM_CHK_NULL (newHeapBase, "nullptr newheapbase", empty);

        contextHeap->pHeapBase = newHeapBase;
        HDDLCommContextElement *ctxHeapBase = (HDDLCommContextElement *)contextHeap->pHeapBase;
        contextHeap->pFirstFreeElement = (void *) (&ctxHeapBase[contextHeap->uiElementCount]);

        for (int32_t i = 0; i < HEAP_INCREMENTAL_SIZE; i++)
        {
            commCtxElement = &ctxHeapBase[contextHeap->uiElementCount + i];
            commCtxElement->pNextFree = (i == (HEAP_INCREMENTAL_SIZE - 1)) ? NULL :
                &ctxHeapBase[contextHeap->uiElementCount + i + 1];
            commCtxElement->pCommContext = NULL;
        }
        contextHeap->uiElementCount += HEAP_INCREMENTAL_SIZE;
    }

    commCtxElement = (HDDLCommContextElement *)contextHeap->pFirstFreeElement;
    contextHeap->pFirstFreeElement = commCtxElement->pNextFree;
    return commCtxElement;
}

void HDDLMemoryMgr_ReleaseBufferElement (HDDLVAHeap *bufferHeap, uint32_t bufferId)
{
    HDDLVABufferElement *bufferHeapBase;
    HDDLVABufferElement *bufferElement;
    void *firstFree;

    SHIM_CHK_LESS (bufferId, bufferHeap->uiElementCount, "invalid location", );

    bufferHeapBase = (HDDLVABufferElement *)bufferHeap->pHeapBase;
    bufferElement = &bufferHeapBase[bufferId];
    SHIM_CHK_NULL (bufferElement->pBuf, "buffer is already released", );

    firstFree = bufferHeap->pFirstFreeElement;
    bufferHeap->pFirstFreeElement = (void*)bufferElement;
    bufferElement->pNextFree = (HDDLVABufferElement *)firstFree;
    bufferElement->pBuf = NULL;
}

void HDDLMemoryMgr_ReleaseImageElement (HDDLVAHeap *imageHeap, uint32_t imageId)
{
    HDDLVAImageElement *imageHeapBase;
    HDDLVAImageElement *imageElement;
    void *firstFree;

    SHIM_CHK_LESS (imageId, imageHeap->uiElementCount, "Invalid image ID", );

    imageHeapBase = (HDDLVAImageElement *)imageHeap->pHeapBase;
    imageElement = &imageHeapBase[imageId];
    SHIM_CHK_NULL (imageElement->pImage, "Image already released", );

    firstFree = imageHeap->pFirstFreeElement;
    imageHeap->pFirstFreeElement = (void*)imageElement;
    imageElement->pNextFree = (HDDLVAImageElement *)firstFree;
    imageElement->pImage = NULL;
}

void HDDLMemoryMgr_ReleaseCommContextElement (HDDLVAHeap *contextHeap, uint32_t commCtxId)
{
    HDDLCommContextElement *commCtxHeapBase = (HDDLCommContextElement *)contextHeap->pHeapBase;
    HDDLCommContextElement *commCtxElement;
    void *firstFree;

    SHIM_CHK_LESS (commCtxId, contextHeap->uiElementCount, "Invalid context ID", );

    commCtxHeapBase = (HDDLCommContextElement *)contextHeap->pHeapBase;
    commCtxElement = &commCtxHeapBase[commCtxId];
    SHIM_CHK_NULL (commCtxElement->pCommContext, "Context already released", );

    firstFree = contextHeap->pFirstFreeElement;
    contextHeap->pFirstFreeElement = (void *)commCtxElement;
    commCtxElement->pNextFree = (HDDLCommContextElement *)firstFree;
    commCtxElement->pCommContext = NULL;
}

HDDLVABuffer *HDDLMemoryMgr_GetBufferFromVABufferID (HDDLVAShimDriverContext *ctx,
    VABufferID bufId, uint32_t *hostBufId)
{
    HDDLVABufferElement *bufferElement;
    HDDLVABuffer *buffer = NULL;
    uint32_t elementCount = ctx->bufferHeap->uiElementCount;
    void *empty = NULL;

    bufferElement = (HDDLVABufferElement *)ctx->bufferHeap->pHeapBase;
    SHIM_CHK_NULL (bufferElement, "nullptr bufferElement", empty);

    // FIXME: will implement better search mechanism
    for (int i = 0; i < elementCount; i++)
    {
        buffer = bufferElement[i].pBuf;
        if (buffer && (buffer->bufId == bufId))
        {
            *hostBufId = i; // return host's bufferId
            return buffer;
        }
    }

    return buffer;
}

VAImage *HDDLMemoryMgr_GetVAImageFromVAImageID (HDDLVAShimDriverContext *ctx,
    VAImageID imgId, uint32_t *hostImgId)
{
    HDDLVAImageElement *imageElement;
    VAImage *vaImage = NULL;
    uint32_t elementCount = ctx->imageHeap->uiElementCount;
    void *empty = NULL;

    imageElement = (HDDLVAImageElement *)ctx->imageHeap->pHeapBase;
    SHIM_CHK_NULL (imageElement, "nullptr imageElement", empty);

    // FIXME: will implement better search mechanism
    for (int i = 0; i < elementCount; i++)
    {
        vaImage = imageElement[i].pImage;
        if (vaImage && (vaImage->image_id == imgId))
        {
            *hostImgId = i; // return host's imageId
            return vaImage;
        }
    }

    return vaImage;
}

HDDLShimCommContext *HDDLMemoryMgr_GetCommContext (HDDLVAShimDriverContext *ctx, uint64_t pid,
    uint64_t tid, uint32_t *commCtxId)
{
    HDDLCommContextElement *commCtxElement;
    HDDLShimCommContext *commCtx = NULL;
    uint32_t elementCount = ctx->contextHeap->uiElementCount;
    void *empty = NULL;

    commCtxElement = (HDDLCommContextElement *)ctx->contextHeap->pHeapBase;
    SHIM_CHK_NULL (commCtxElement, "nullptr commCtxElement", empty);

    for (int i = 0; i < elementCount; i++)
    {
        commCtx = commCtxElement[i].pCommContext;

        if (commCtx)
        {
            if ( (commCtx->pid == pid) && (commCtx->tid == tid))
            {
                *commCtxId = i;
                return commCtx;
            }
        }
    }

    return commCtx;
}

void *HDDLMemoryMgr_LockBuffer (HDDLVABuffer *buf)
{
    void *empty = NULL;
    SHIM_CHK_NULL (buf, "nullptr buffer", empty);

    if (buf->bMapped == false)
    {
        buf->bMapped = true;
    }

    buf->iRefCount++;

    return buf->pData;
}

void HDDLMemoryMgr_UnlockBuffer (HDDLVABuffer *buf)
{
    SHIM_CHK_NULL(buf, "nullptr buffer", );

    if ( (buf->bMapped == true) && (buf->iRefCount == 1))
    {
        buf->bMapped = false;
    }

    buf->iRefCount--;
}

//EOF
