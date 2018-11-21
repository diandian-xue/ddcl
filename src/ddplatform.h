﻿#pragma once

#if defined(WIN32) || defined(_WINDOWS)
/*
#define DD_WINDOWS
#define DDSYS_WIN_PC
#define DDSYS_WIN_RT
#define DDSYS_WIN_PHONE
*/
#define DDSYS_WIN
#define DD_WINDOWS

#ifdef WIN_RT
#define DDSYS_WIN_RT
#endif

#ifdef WIN_PHONE
#define DDSYS_WIN_PHONE
#endif

#if !defined(DDSYS_WIN_RT) && !defined(DDSYS_WIN_PHONE)
#define DDSYS_WINPC
#endif

#endif

#if defined(LINUX) || defined(linux)
#define DD_LINUX
#define DD_UNIX
#endif

#if defined(ANDROID)
#define DD_ANDROID
#define DD_UNIX
#endif

#if defined(__APPLE__) || defined(APPLE)
#define DD_UNIX
#define DD_APPLE
/*
#define DDSYS_MACOS
#define DDSYS_IOS
*/
#ifdef APPLE_IOS
#define DDSYS_IOS
#endif

#ifndef DDSYS_IOS
#define DDSYS_MACOS
#endif

#endif


#if !defined(DD_WINDOWS) &&\
    !defined(DD_UNIX)
#error "unknown system"
#endif


/* *****************
 * 定义NULL指针宏
 */
#ifndef NULL
#ifdef __cplusplus
#define NULL        0
#else
#define NULL        ((void *)0)
#endif
#endif
/* ***************** */


/* ****************
 * 定义编译器宏
 * */
#define DDCOMPILER_MICROSOFT       0
#define DDCOMPILER_GNU             1
#define DDCOMPILER_BORLAND         2
#define DDCOMPILER_INTEL           3
#define DDCOMPILER_CLANG           4

#ifdef _MSC_VER
#  define DDCOMPILER DDCOMPILER_MICROSOFT
#elif defined( __INTEL_COMPILER )
#  define DDCOMPILER DDCOMPILER_INTEL
#elif defined( __BORLANDC__ )
#  define DDCOMPILER DDCOMPILER_BORLAND
#elif defined( __GNUC__ )
#  define DDCOMPILER DDCOMPILER_GNU
#elif defined( __clang__ )
#  define DDCOMPILER DDCOMPILER_CLANG

#else
# error "FATAL ERROR: Unknown compiler."
#endif
/* *************** */



/* ***************
 *  定义内存单元类型
#ifndef DDSYS_UNIX
#endif
 * */
#include <sys/types.h>
#include <stdint.h>
#if DDCOMPILER != DDCOMPILER_GNU
typedef signed __int64              ddint64;
typedef signed __int32              ddint32;
typedef signed __int16              ddint16;
typedef signed __int8               ddint8;
typedef unsigned __int64            dduint64;
typedef unsigned __int32            dduint32;
typedef unsigned __int16            dduint16;
typedef unsigned __int8             dduint8;

#else
typedef int64_t                     ddint64;
typedef int32_t                     ddint32;
typedef int16_t                     ddint16;
typedef int8_t                      ddint8;
typedef uint64_t                    dduint64;
typedef uint32_t                    dduint32;
typedef uint16_t                    dduint16;
typedef uint8_t                     dduint8;
#endif
/* *************** */

