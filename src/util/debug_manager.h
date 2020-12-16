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

#ifndef __DEBUG_MANAGER_H__
#define __DEBUG_MANAGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>

//------------------------------------------------------------------------------
// Macros for debug messages, Assert, Null check and condition check within hddl2 vaapi files
//------------------------------------------------------------------------------
#define SHIM_ERROR_MESSAGE(fmt, ...) \
    fprintf (stderr, "[%u][%lu] error: " fmt "\n", getpid (), syscall (SYS_gettid), ##__VA_ARGS__)

#ifdef DEBUG

// Print log for compile process
#define SHIM_INFO_MESSAGE(fmt, ...) \
    printf ("[%u][%lu] info: " fmt "\n", getpid (), syscall (SYS_gettid), ##__VA_ARGS__)

// Print log for compile error
#define SHIM_ASSERT_MESSAGE(fmt, ...) \
    printf ("[%u][%lu] error: " fmt "\n", getpid (), syscall (SYS_gettid), ##__VA_ARGS__)

#define SHIM_NORMAL_MESSAGE(fmt, ...) \
    printf ("[%u][%lu] " fmt "\n", getpid (), syscall (SYS_gettid), ##__VA_ARGS__)

#if defined(FUNCTION_TIME_PROFILE) || defined(COMM_TIME_PROFILE)
#define PROF_NAME "vaapi_bypass_prof"
FILE *file;

#define SHIM_PROFILE_INIT()           \
{                                     \
    char fileName[50];                \
    struct timeval time;              \
    gettimeofday (&time, NULL);       \
    snprintf (fileName, sizeof (fileName), "%s_%ld-%ld", PROF_NAME, time.tv_sec, time.tv_usec); \
    file = fopen (fileName, "w");     \
}

#define SHIM_PROFILE_TERMINATE()   \
{                                  \
    fclose (file);                 \
}
#else
#define SHIM_PROFILE_INIT()
#define SHIM_PROFILE_TERMINATE()
#endif

#ifdef FUNCTION_TIME_PROFILE
struct timeval funcStart, funcEnd;
#define SHIM_FUNCTION_ENTER()        \
{                                    \
    gettimeofday (&funcStart, NULL);    \
    fprintf (file, "[%u][%lu] %s enter\n", getpid (), syscall (SYS_gettid), __func__);  \
}

#define SHIM_FUNCTION_EXIT()         \
{                                    \
    gettimeofday (&funcEnd, NULL);   \
    fprintf (file, "[%u][%lu] %s exit\ttime (us): %ld\n", getpid (), syscall (SYS_gettid), \
        __func__, ( (funcEnd.tv_sec - funcStart.tv_sec) * 1000000) +                       \
	(funcEnd.tv_usec - funcStart.tv_usec));                                            \
}
#else
#define SHIM_FUNCTION_ENTER()    \
    printf ("[%u][%lu] %s enter\n", getpid (), syscall (SYS_gettid), __func__)

#define SHIM_FUNCTION_EXIT()     \
    printf ("[%u][%lu] %s exit\n", getpid (), syscall (SYS_gettid), __func__)
#endif

#ifdef COMM_TIME_PROFILE
struct timeval commStart, commEnd;
#define SHIM_PROFILE_START()        \
    gettimeofday (&commStart, NULL)

#define SHIM_PROFILE_END()          \
{                                   \
    gettimeofday (&commEnd, NULL);  \
    fprintf (file, "\t\t* %s\t\ttime (us): %ld\n", __func__, \
        ( (commEnd.tv_sec - commStart.tv_sec) * 1000000) + (commEnd.tv_usec - commStart.tv_usec));\
}
#else
#define SHIM_PROFILE_START()
#define SHIM_PROFILE_END()
#endif

#else /*release*/

#define SHIM_INFO_MESSAGE(fmt, ...)
#define SHIM_ASSERT_MESSAGE(fmt, ...)
#define SHIM_NORMAL_MESSAGE(fmt, ...)
#define SHIM_FUNCTION_ENTER()
#define SHIM_FUNCTION_EXIT()
#define SHIM_PROFILE_INIT()
#define SHIM_PROFILE_TERMINATE()
#define SHIM_PROFILE_START()
#define SHIM_PROFILE_END()

#endif

// If _ptr = NULL, print _str & return _ret
#define SHIM_CHK_NULL(_ptr, _str, _ret)                    \
    SHIM_CHK_CONDITION((NULL == (_ptr)), _str, _ret)

// If integer is 0, print the error message and return the specific value.
#define SHIM_CHK_ZERO(_int, _str, _ret)                    \
    SHIM_CHK_CONDITION(('\0' == (_int)), _str, _ret)

#define SHIM_CHK_NOT_ZERO(_int, _str, _ret)                \
    SHIM_CHK_CONDITION(('\0' != (_int)), _str, _ret)

#define SHIM_CHK_ERROR(_int, _str, _ret)                   \
    SHIM_CHK_NOT_ZERO(_int, _str, _ret)

#define SHIM_CHK_EQUAL(_int, _compare, _str, _ret)         \
    SHIM_CHK_CONDITION((_compare) == (_int), _str, _ret)

#define SHIM_CHK_NOT_EQUAL(_int, _compare, _str, _ret)     \
    SHIM_CHK_CONDITION((_compare) != (_int), _str, _ret)

#define SHIM_CHK_LARGER(p, bottom, str, ret)               \
    SHIM_CHK_CONDITION((p <= bottom), str, ret)

#define SHIM_CHK_LESS(p, upper, str, ret)                  \
    SHIM_CHK_CONDITION((p >= upper), str, ret)

#define SHIM_CHK_BOUNDARY(p, bottom, upper, str, ret)      \
{                                                          \
    SHIM_CHK_LESS(p, upper, str, ret);                     \
    SHIM_CHK_LARGER(p, bottom, str, ret);                  \
}
//TODO:Need confirm
// Check the return value of function.
// If failed,  print the error message and return,
// do nothing otherwise.
#define SHIM_CHK_RET(_ret, _str)                           \
{                                                          \
    if (VA_STATUS_SUCCESS != _ret) {                       \
        SHIM_ASSERT_MESSAGE("%s [%d].", _str, _ret);       \
        return _ret;                                       \
    }                                                      \
}

// TODO:Need confirm
// Check the return status of parse function
// If failed, assign new status and break
// do nothing otherwise.
#define SHIM_CHK_STATUS(_ret, _newret)                     \
{                                                          \
    if (VA_STATUS_SUCCESS != _ret) {                       \
        vaStatus = _newret;                                \
        break;                                             \
    }                                                      \
}

// Check the condition, if true, print the error message
// and return the specified value, do nothing otherwise.
#define SHIM_CHK_CONDITION(condition, _str, _ret)          \
    if (condition) {                                       \
        SHIM_ASSERT_MESSAGE(_str);                         \
        return _ret;                                       \
    }

#endif
