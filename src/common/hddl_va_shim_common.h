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
//! \file    hddl_va_shim_common.h
//! \brief   HDDL VA shim general context
//! \details Common context source shared by both Host IA & KMB Target
//!

#ifndef __HDDL_VA_SHIM_COMMON_H__
#define __HDDL_VA_SHIM_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdbool.h>
#include <X11/Xutil.h>
#include <linux/fb.h>

//Include libva
#include <va/va.h>
#include <va/va_backend.h>
#include <va/va_drmcommon.h>

//Local include
#include "base_definition.h"
#include "debug_manager.h"

//Include xlink header
#if defined(XLINK) || defined (XLINK_SIMULATOR)
#include <xlink.h>
#else
#include "xlink_placeholders.h"
#endif

#define HOST    0
#define TARGET  1

#define MAX_COMM_SETTINGS 3
#define MAX_XLINK_CHANNELS 2
#define MAX_XLINK_EVENT 4
//Number of device supported on KMB SRB now is 3
#define MAX_DEVICE 4
#define MAX_DEVICE_LIST_SIZE 8
#define MAX_TIMEOUT_COUNT 2

#define MIN_XLINK_CHANNEL_ID 0x400
#define MAX_XLINK_CHANNEL_ID 0xFFF

#define DATA_FRAGMENT_SIZE 4 * 1024 * 1024
#define DATA_MAX_SEND_SIZE 3 * 1024 * 1024
#define DEFAULT_XLINK_PATH "/tmp/xlink_mock"
#define OPEN_CHANNEL_TIMEOUT 50000
#define OPERATION_TYPE RXB_TXB

#define RESET_SOURCE FROM_CODEC
#define RESET_REASON XLINK_TIMEOUT

//File to boot for xlink-pcie
#define BOOT_LOCAL_HOST_FIP_FILE "intel-kmb/fip.bin"
#define BOOT_LOCAL_HOST_IMAGE_FILE "intel-kmb/image.bin"

//Used to extract the interface type from a sw device id
#define SW_DEVICE_ID_INTERFACE_SHIFT 24U
#define SW_DEVICE_ID_INTERFACE_MASK  0x7
#define GET_INTERFACE_FROM_SW_DEVICE_ID(id) \
    ((id >> SW_DEVICE_ID_INTERFACE_SHIFT) & SW_DEVICE_ID_INTERFACE_MASK)

//Used to obtain sliceid from sw device id
#define SW_DEVICE_ID_SLICEID_SHIFT 1
#define SW_DEVICE_ID_SLICEID_MASK 0x000E
#define GET_SLICEID_FROM_SW_DEVICE_ID(id) \
    ((id & SW_DEVICE_ID_SLICEID_MASK) >> SW_DEVICE_ID_SLICEID_SHIFT)

#define SW_DEVICE_ID_IPC_INTERFACE  0x0
#define SW_DEVICE_ID_PCIE_INTERFACE 0x1
#define SW_DEVICE_ID_USB_INTERFACE  0x2
#define SW_DEVICE_ID_ETH_INTERFACE  0x3

#define DEFAULT_TCP_HOST "127.0.0.1"
#define SocketAddress struct sockaddr

#define HEAP_INCREMENTAL_SIZE 8

#define WORKLOAD_ID_NONE -1
#define QUERY_FROM_UNITE -1

#define IS_BATCH(ctx) ((ctx)->doBatch)
#define COMM_MODE(ctx) ((ctx)->commMode)
#define IS_TCP_MODE(ctx) ((ctx)->commMode==COMM_MODE_TCP)
#define IS_XLINK_MODE(ctx) ((ctx)->commMode==COMM_MODE_XLINK)
#define IS_UNITE_MODE(ctx) ((ctx)->commMode==COMM_MODE_UNITE)

#define BATCH_FRAME_START_FUNC HDDLVABeginPicture
#define BATCH_FRAME_END_FUNC HDDLVAEndPicture
#define BATCH_DESTROY_START_FUNC HDDLVADestroyBuffer
#define BATCH_DESTROY_END_FUNC HDDLVACreateBuffer
#define BATCH_DESTROY_FINAL_FUNC HDDLVADestroyContext

#define HDDLVABUFFER_NODE_LIST_SIZE 128

typedef enum
{
    HDDL_SHIM_STATUS_SUCCESS,     // 0
    HDDL_SHIM_STATUS_FAILED,      // 1
    HDDL_SHIM_STATUS_READY,       // 2
    HDDL_SHIM_STATUS_NOT_READY    // 3
}HDDLShimStatus;

typedef enum
{
    COMM_STATUS_SUCCESS,           // 0
    COMM_STATUS_FAILED,            // 1
    COMM_STATUS_EOF,               // 2
    COMM_STATUS_CONNECTION_CLOSED, // 3
    COMM_STATUS_UNKNOWN            // 4
}CommStatus;

typedef enum
{
    COMM_MODE_TCP,               // 0
    COMM_MODE_XLINK,             // 1
    COMM_MODE_UNITE,             // 2
    COMM_MODE_UNKNOWN            // 3
}CommMode;
typedef CommMode CommMode;

typedef enum
{
    TCP_SUCCESS,           //0
    TCP_FAILED,            //1
    TCP_EOF,               //2
    TCP_CONNECTION_CLOSED, //3
    TCP_UNKNOWN            //4
}TCPError_t;
typedef TCPError_t TCPStatus;

typedef enum
{
    COMM_READ_FULL,    // To use Comm_Read function
    COMM_READ_PARTIAL  // To use Comm_Peak function
}CommReadOp;

typedef enum xlink_error XLinkStatus;

typedef enum
{
    BATCH_OFF,
    BATCH_PER_FRAME,
    BATCH_DESTROY_BUFFER
}HDDLShimBatchState;

typedef enum
{
    MAIN_COMM_CONTEXT,
    DYNAMIC_COMM_CONTEXT
}HDDLShimCommContextNew;

// Categorize buffer-related information in vaBuffer
typedef struct _HDDL_VA_BUFFER
{
    VABufferID bufId;
    VAContextID context;
    VABufferType type;
    uint32_t uiSize;
    uint32_t uiNumElement;
    void *pData;

    uint32_t uiWidth;
    uint32_t uiHeight;

    bool bMapped;
    int32_t iRefCount;
}HDDLVABuffer;

// Allow control entire heap
typedef struct _HDDL_VA_HEAP
{
    void *pHeapBase;
    uint32_t uiElementSize;
    uint32_t uiElementCount;
    void *pFirstFreeElement;
}HDDLVAHeap;

// Control heap element for vaBuffer
typedef struct _HDDL_VA_BUFFER_ELEMENT
{
    HDDLVABuffer *pBuf;
    struct _HDDL_VA_BUFFER_ELEMENT *pNextFree;
}HDDLVABufferElement;

// Control heap element for vaImage
typedef struct _HDDL_VA_IMAGE_ELEMENT
{
    VAImage *pImage;
    struct _HDDL_VA_IMAGE_ELEMENT *pNextFree;
}HDDLVAImageElement;

typedef struct _REMOTE_FD_NODE
{
    VABufferID bufferId;
    int32_t remoteFd;
}HDDLVABufferNode;

// Hold payload for XLINK communication
typedef struct xlink_handle xLinkHandler_t;
typedef uint16_t xLinkChannelId_t;

typedef struct _XLINK_CONTEXT
{
    xLinkChannelId_t xLinkChannelTX;
    xLinkChannelId_t xLinkChannelRX;
    xLinkHandler_t xLinkHandler;    // handler hold payload content
    pthread_mutex_t xLinkMutex;
}HDDLShimXLinkContext;

// Hold payload for TCP/IP communication
typedef struct _TCP_CONTEXT
{
    int serverTX;
    int serverRX;
    int clientTX;
    int clientRX;
    struct sockaddr_in addressTX;
    struct sockaddr_in addressRX;
    pthread_mutex_t tcpMutex;
}HDDLShimTCPContext;

typedef struct _UNITE_CONTEXT
{
    HDDLShimXLinkContext *xLinkCtx;
    uint64_t workloadId;
    uint64_t internalWorkloadId;
    uint32_t swDeviceId;
    uint32_t sliceId;
    HDDLVABufferNode HDDLVABufferNodeList[HDDLVABUFFER_NODE_LIST_SIZE];
}HDDLShimUniteContext;

typedef struct
{
    char data[DATA_MAX_SEND_SIZE];
    uint32_t offset;
    HDDLShimBatchState batchState;
}HDDLShimBatchPayload;

typedef struct _SHIM_THREAD_PARAMS
{
    CommMode commMode;
    uint16_t tx;
    uint16_t rx;
    uint64_t workloadId;
    uint32_t swDeviceId;
    VADisplay vaDpy;
    uint32_t vaDrmFd;
    VAProfile profile;
}ShimThreadParams;

typedef struct _COMM_CONTEXT
{
    union
    {
        HDDLShimXLinkContext *xLinkCtx; // Ctx for XLINK comm
        HDDLShimTCPContext *tcpCtx;     // Ctx for TCP/IP comm
        HDDLShimUniteContext *uniteCtx;
    };
    CommMode  commMode;
    uint64_t pid;
    uint64_t tid;

    // Variables for target used only
    VADisplay vaDpy;
    uint32_t vaDrmFd;
    VAProfile profile;

    // Variables for batching operations
    bool doBatch;
    HDDLShimBatchPayload *batchPayload;
    uint64_t batchThreadId;
}HDDLShimCommContext;

typedef struct _HDDL_COMM_CONTEXT_ELEMENT
{
    HDDLShimCommContext *pCommContext;
    struct _HDDL_COMM_CONTEXT_ELEMENT *pNextFree;
}HDDLCommContextElement;

typedef struct _HDDL_VA_SHIM_DRIVER_CONTEXT
{
    // Communication context for Host and Target
    HDDLShimCommContext *mainCommCtx;
    uint16_t lastChannel;

    // Used for context reference count
    uint32_t uiRefCount;

    // Heap handler
    HDDLVAHeap *bufferHeap;
    uint32_t uiNumBuffer;
    HDDLVAHeap *imageHeap;
    uint32_t uiNumImage;
    HDDLVAHeap *contextHeap;
    uint32_t uiNumContext;

    // Mutex to protect shared resource among multiple context
    pthread_mutex_t bufferMutex;
    pthread_mutex_t imageMutex;
    pthread_mutex_t contextMutex;
}HDDLVAShimDriverContext;

enum deviceStatus {
    DEVICE_DOWN = 0,
    DEVICE_UP
};

typedef struct {
    uint32_t swDeviceId;
    enum deviceStatus state;
}device_state;

//TODO:Update the event type once get response from component owner
typedef enum
{
    NOTIFY_DEVICE_DISCONNECTED, //device down
    NOTIFY_DEVICE_CONNECTED, //device up
    NOTIFY_DEVICE_UNKNOWN_1, //to be confirm with component owner
    NOTIFY_DEVICE_UNKNOWN_2 // to be confirm with component owner
}XLinkDeviceEventType;

static __inline HDDLVAShimDriverContext *HDDL_GetVAShimContext (VADriverContextP ctx)
{
    return (HDDLVAShimDriverContext *)ctx->pDriverData;
}

#endif
//EOF
