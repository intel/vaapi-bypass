// FIXME: This file is to be deleted after the actual xlink content
// is available
#ifndef XLINK_SIMULATOR
#include "xlink_placeholders.h"

XLinkError_t XLinkInitialize (XLinkGlobalHandler_t * globalHandler)
{
    return X_LINK_SUCCESS;
}

XLinkError_t XLinkConnect (XLinkHandler_t * deviceIdHandler)
{
    return X_LINK_SUCCESS;
}

XLinkError_t XLinkOpenChannel (XLinkHandler_t * deviceIdHandler, channelId_t channelId,
    OperationMode_t operationType, uint32_t dataSize, uint32_t timeout)
{
    return X_LINK_SUCCESS;
}

XLinkError_t XLinkWriteData (XLinkHandler_t * deviceIdHandler, channelId_t channelId,
    const uint8_t * message, uint32_t size)
{
    return X_LINK_SUCCESS;
}

XLinkError_t XLinkReadData (XLinkHandler_t * deviceIdHandler, channelId_t channelId,
    uint8_t ** message, uint32_t * size)
{
    return X_LINK_SUCCESS;
}

XLinkError_t XLinkReleaseData (XLinkHandler_t * deviceIdHandler, channelId_t channelId,
    const uint8_t * dataAddr)
{
    return X_LINK_SUCCESS;
}

XLinkError_t XLinkCloseChannel (XLinkHandler_t * deviceIdHandler, channelId_t channelId)
{
    return X_LINK_SUCCESS;
}

XLinkError_t XLinkDisconnect (XLinkHandler_t * deviceIdHandler)
{
    return X_LINK_SUCCESS;
}

XLinkError_t xLinkAssignDataAvailableCallback (XLinkHandler_t * deviceIdHandler,
    channelId_t channelId, void *callback (channelId_t channelId, uint8_t * dataAddr))
{
    return X_LINK_SUCCESS;
}


void *xlink_on_data_available (channelId_t channelId, uint8_t * dataAddr)
{
    return NULL;
}

uint32_t XlinkGetNumberOfDevices (void)
{
    return 1;
}

XLinkError_t XLinkGetDeviceName (int index, char *name, int nameSize)
{
    return X_LINK_SUCCESS;
}
#endif
