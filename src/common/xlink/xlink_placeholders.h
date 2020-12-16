// FIXME: This file is to be deleted after the actual xlink content
// is available

#ifndef __XLINK_PLACEHOLDERS_H__
#define __XLINK_PLACEHOLDERS_H__

#include <stdint.h>
#include <stddef.h>

typedef struct _XLinkProf XLinkProf_t;
typedef struct _XLinkGlobalHandler XLinkGlobalHandler_t;
typedef struct _XLinkHandler XLinkHandler_t;
typedef uint8_t channelId_t;

typedef enum
{
    RXB_TXB,
    RXN_TXN,
    RXB_TXN,
    RXN_TXB
} OperationMode_t;

typedef enum
{
    HOST_DEVICE,
    VPUIP_DEVICE
} XLinkDeviceTypes_t;

typedef enum
{
    X_LINK_SUCCESS,
    X_LINK_ALREADY_INIT,
    X_LINK_ALREADY_OPEN,
    X_LINK_COMMUNICATION_NOT_OPEN,
    X_LINK_COMMUNICATION_FAIL,
    X_LINK_COMMUNICATION_UNKNOWN_ERROR,
    X_LINK_DEVICE_NOT_FOUND,
    X_LINK_TIMEOUT,
    X_LINK_ERROR,
    X_LINK_CHAN_FULL
} XLinkError_t;

typedef enum
{
    USB_VSC,
    USB_CDC,
    PCIE,
    IPC,
    NMB_OF_PROTOCOLS
} XLinkProtocol_t;

struct _XLinkProf
{
    float totalReadTime;
    float totalWriteTime;
    unsigned long totalReadBytes;
    unsigned long totalWriteBytes;
    unsigned long totalBootCount;
    float totalBootTime;
};

struct _XLinkGlobalHandler
{
    int loglevel;
    int profEnable;
    int protocol;
    XLinkProf_t profilingData;
    char *serverAddress;
};

struct _XLinkHandler
{
    char *devicePath;
    char *devicePath2;
    int linkId;
    XLinkDeviceTypes_t deviceType;
};

XLinkError_t XLinkInitialize (XLinkGlobalHandler_t * globalHandler);

XLinkError_t XLinkConnect (XLinkHandler_t * deviceIdHandler);

XLinkError_t XLinkOpenChannel (XLinkHandler_t * deviceIdHandler, channelId_t channelId,
    OperationMode_t operationType, uint32_t dataSize, uint32_t timeout);

XLinkError_t XLinkWriteData (XLinkHandler_t * deviceIdHandler, channelId_t channelId,
    const uint8_t * message, uint32_t size);

XLinkError_t XLinkReadData (XLinkHandler_t * deviceIdHandler, channelId_t channelId,
    uint8_t ** message, uint32_t * size);

XLinkError_t XLinkReleaseData (XLinkHandler_t * deviceIdHandler, channelId_t channelId,
    const uint8_t * dataAddr);

XLinkError_t XLinkCloseChannel (XLinkHandler_t * deviceIdHandler, channelId_t channelId);

XLinkError_t XLinkDisconnect (XLinkHandler_t * deviceIdHandler);

XLinkError_t xLinkAssignDataAvailableCallback (XLinkHandler_t * deviceIdHandler,
    channelId_t channelId, void *callback (channelId_t channelId, uint8_t * dataAddr));

void *xlink_on_data_available (channelId_t channelId, uint8_t * dataAddr);

uint32_t XlinkGetNumberOfDevices (void);

XLinkError_t XLinkGetDeviceName (int index, char *name, int nameSize);

#endif
