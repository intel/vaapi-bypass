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
//! \file    thread_manager.h
//! \brief   Manager which handle thread & mutex control
//! \details Allow mutex & thread event
//!

#ifndef __THREAD_MANAGER_H__
#define __THREAD_MANAGER_H__

#include "hddl_va_shim_common.h"

//!
//! \brief   Initialises the mutex referenced by mutex
//! \return  void
//!          Return nothing
//!
void HDDLThreadMgr_InitMutex (pthread_mutex_t *mutex);

//!
//! \brief   Destroys the mutex object referenced by mutex
//! \return  void
//!          Return nothing
//!
void HDDLThreadMgr_DestroyMutex (pthread_mutex_t *mutex);

//!
//! \brief   The mutex object referenced by mutex shall be locked
//! \return  void
//!          Return nothing
//!
void HDDLThreadMgr_LockMutex (pthread_mutex_t *mutex);

//!
//! \brief   Release the mutex object referenced by mutex
//! \return  void
//!          Return nothing
//!
void HDDLThreadMgr_UnlockMutex (pthread_mutex_t *mutex);

//!
//! \brief   Start a new thread
//! \return  int32_t
//!          Return 0 if success, else fail
//!
int32_t HDDLThreadMgr_CreateThread (pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine) (void *), void *arg);

//!
//! \brief   Join with a terminated thread
//! \return  void
//!          Return nothing
//!
void HDDLThreadMgr_JoinThread (pthread_t thread, void **retval);

//!
//! \brief   Function block on a condition variable
//! \return  void
//!          Return nothing
//!
void HDDLThreadMgr_CondWaitThread (pthread_cond_t  *cond, pthread_mutex_t *mutex);

//!
//! \brief   Unblock threads blocked on a condition variable
//! \return  void
//!          Return nothing
//!
void HDDLThreadMgr_CondBroadcastThread (pthread_cond_t *cond);

#endif

//EOF
