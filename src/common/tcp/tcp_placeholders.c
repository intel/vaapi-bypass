//!
//! \file    tcp_placeholders.c
//! \brief   placeholders for TCP communication
//!

#include "tcp.h"
#include "debug_manager.h"

int flagOpt;

HDDLShimTCPContext *TCP_ContextInit (uint16_t portTX, uint16_t portRX, struct hostent *server)
{
    HDDLShimTCPContext *tcpCtx = NULL;
    return tcpCtx;
}

TCPStatus TCP_Initialize (HDDLShimTCPContext *tcpCtx, int flag)
{
    return TCP_SUCCESS;
}

TCPStatus TCP_Connect (HDDLShimTCPContext *tcpCtx)
{
    return TCP_SUCCESS;
}

TCPStatus TCP_Write (HDDLShimTCPContext *tcpCtx, int dataSize, void *payload)
{
    return TCP_SUCCESS;
}

TCPStatus TCP_Read (HDDLShimTCPContext *tcpCtx, int dataSize, void *payload)
{
    return TCP_SUCCESS;
}

TCPStatus TCP_Peek (HDDLShimTCPContext *tcpCtx, uint32_t *size, void *payload)
{
    return TCP_SUCCESS;
}

TCPStatus TCP_Disconnect (HDDLShimTCPContext *tcpCtx, int flag)
{
    return TCP_SUCCESS;
}

TCPStatus TCP_Reconnect (HDDLShimTCPContext *tcpCtx)
{
    return TCP_SUCCESS; 
}

//EOF
