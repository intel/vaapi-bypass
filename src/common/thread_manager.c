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
//! \file    thread_manager.c
//! \brief   Manager which handle thread & mutex control
//! \details Allow mutex & thread event
//!

#include "thread_manager.h"
#include "debug_manager.h"

pthread_mutex_t GlobalMutex = PTHREAD_MUTEX_INITIALIZER;

void HDDLThreadMgr_InitMutex (pthread_mutex_t *mutex)
{
    pthread_mutex_init (mutex, NULL);
}

void HDDLThreadMgr_DestroyMutex (pthread_mutex_t *mutex)
{
    int32_t ret = pthread_mutex_destroy (mutex);
    if (ret != 0)
    {
        SHIM_NORMAL_MESSAGE ("can't destroy the mutex!");
    }
}

void HDDLThreadMgr_LockMutex (pthread_mutex_t *mutex)
{
    int32_t ret = pthread_mutex_lock (mutex);
    if (ret != 0)
    {
        SHIM_NORMAL_MESSAGE ("can't lock the mutex!");
    }
}

void HDDLThreadMgr_UnlockMutex (pthread_mutex_t *mutex)
{
    int32_t ret = pthread_mutex_unlock (mutex);
    if (ret != 0)
    {
        SHIM_NORMAL_MESSAGE ("can't unlock the mutex!");
    }
}

int32_t HDDLThreadMgr_CreateThread (pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine) (void *), void *arg)
{
    return pthread_create (thread, attr, start_routine, arg);
}

void HDDLThreadMgr_JoinThread (pthread_t thread, void **retval)
{
    int32_t ret = pthread_join (thread, retval);
    if (ret != 0)
    {
        SHIM_NORMAL_MESSAGE ("can't join the mutex!");
    }
}

void HDDLThreadMgr_CondWaitThread (pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    int32_t ret = pthread_cond_wait (cond, mutex);
    if (ret != 0)
    {
        SHIM_NORMAL_MESSAGE ("can't wait the mutex!");
    }
}

void HDDLThreadMgr_CondBroadcastThread (pthread_cond_t *cond)
{
    int32_t ret = pthread_cond_broadcast (cond);
    if (ret != 0)
    {
        SHIM_NORMAL_MESSAGE ("can't boardcast the mutex!");
    }
}
