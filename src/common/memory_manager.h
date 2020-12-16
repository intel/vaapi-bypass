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
//! \file    memory_manager.h
//! \brief   Manager which handle control memory allocation & release
//! \details Allocate & release memory for pointer
//!

#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

#include "hddl_va_shim_common.h"

//!
//! \brief   Allocate memory with a size
//! \return  void *
//!          Return pointer if success, else NULL
//!
void *HDDLMemoryMgr_AllocMemory (size_t size);

//!
//! \brief   Allocate memory for a pointer with a size and initialize the pointer to 0
//! \return  void *
//!          Return pointer if success, else NULL
//!
void *HDDLMemoryMgr_AllocAndZeroMemory (size_t size);

//!
//! \brief   Reallocate a memory with new size
//! \return  void *
//!          Return pointer if success, else NULL
//!
void *HDDLMemoryMgr_ReallocMemory (void *ptr, size_t newSize);

//!
//! \brief   Reallocate a memory with new size and set memory to 0
//! \return  void *
//!          Return pointer if success, else NULL
//!
void *HDDLMemoryMgr_ReallocAndZeroMemory (void *ptr, size_t newSize);

//!
//! \brief   Fill pointer memory with 0
//! \return  void
//!          Return nothing
//!
void  HDDLMemoryMgr_ZeroMemory (void *destination, size_t length);

//!
//! \brief   Fill pointer memory with value
//! \return  void
//!          Return nothing
//!
void  HDDLMemoryMgr_FillMemory (void *destination, size_t length, uint8_t fill);

//!
//! \brief   Free pointer memory space
//! \return  void
//!          Return nothing
//!
void  HDDLMemoryMgr_FreeMemory (void *ptr);

//!
//! \brief   Allocate memory & unique ID for each buffer heap element
//! \return  HDDLVABufferElement *
//!          Return pointer if success, else NULL
//!
HDDLVABufferElement *HDDLMemoryMgr_AllocBufferHeap (HDDLVAHeap *bufferHeap);

//!
//! \brief   Allocate memory & unique ID for each image heap element
//! \return  HDDLVAImageElement *
//!          Return pointer if success, else NULL
//!
HDDLVAImageElement  *HDDLMemoryMgr_AllocImageHeap (HDDLVAHeap *imageHeap);


//!
//! \brief   Allocate memory & unique ID for each comm context heap element
//! \return  HDDLCommContextElement *
//!          Return pointer if success, else NULL
//!
HDDLCommContextElement *HDDLMemoryMgr_AllocCommContextHeap (HDDLVAHeap *contextHeap);

//!
//! \brief   Release buffer element referring to Host VABufferID
//! \return  void
//!          Return nothing
//!
void HDDLMemoryMgr_ReleaseBufferElement (HDDLVAHeap *bufferHeap, uint32_t vaBufferID);

//!
//! \brief   Release image element referring to Host VAImageID
//! \return  void
//!          Return nothing
//!
void HDDLMemoryMgr_ReleaseImageElement (HDDLVAHeap *imageHeap, uint32_t vaImageID);

//!
//! \brief   Release comm context element referring to host commCtxId
//! \return  void
//!          Return nothing
//!
//
void HDDLMemoryMgr_ReleaseCommContextElement (HDDLVAHeap *contextHeap, uint32_t commCtxId);

// Get Heap from heapID

//!
//! \brief   Get buffer element from buffer heap in host
//! \return  HDDLVABuffer *
//!          Return pointer if success, else NULL
//!
HDDLVABuffer *HDDLMemoryMgr_GetBufferFromVABufferID (HDDLVAShimDriverContext *ctx,
    VABufferID bufId, uint32_t *hostBufId);

//!
//! \brief   Get image element from image heap in host
//! \return  VAImage *
//!          Return pointer if success, else NULL
//!
VAImage *HDDLMemoryMgr_GetVAImageFromVAImageID (HDDLVAShimDriverContext *ctx,
    VAImageID imgId, uint32_t *hostImgId);

//!
//! \brief   Get comm context element from comm context heap in host
//! \return  HDDLShimCommContext *
//!          Return pointer if success, else NULL
//!
//
HDDLShimCommContext *HDDLMemoryMgr_GetCommContext (HDDLVAShimDriverContext *ctx, uint64_t pid,
    uint64_t tid, uint32_t *commCtxId);

// Buffer control
//!
//! \brief   Map data into buffer
//! \return  void *
//!          Return pointer if success, else fail
//!
void *HDDLMemoryMgr_LockBuffer (HDDLVABuffer *buf);

//!
//! \brief   Unmap data inside buffer
//! \return  void
//!          Return nothing
//!
void HDDLMemoryMgr_UnlockBuffer (HDDLVABuffer *buf);

#endif

//EOF
