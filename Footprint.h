//  ==============================================================
/**
    @file   Footprint.h
    @brief  enxxx brief description

    @author kurosawa
    @date   2012/12/13
 */
//  ==============================================================
#if !defined(__Footprint_h__)
#define __Footprint_h__

// ==============================================================
//  includes
// ==============================================================
#include <cstddef>
// #include "hogehoge.h"

#include <stdio.h>

#if defined(_WINDOWS)
#include <windows.h>
#endif

#if defined(_DEBUG) || defined(DEBUG) || defined(_DEBUG_) || defined(_SEQ_DEBUG_)

// Debugのとき
#if defined(_WINDOWS)

#define _FOOTPRINT(str)     OutputDebugString(str)

#define _FOOT_PRINT(...)                            \
do {                                                \
    char string[512];                               \
    sprintf_s(string, sizeof(string), __VA_ARGS__); \
    OutputDebugString(string);                      \
} while (0)

#else   /* ! _WINDOWS */

#define _FOOTPRINT(str)     printf("%s", str)

#define _FOOT_PRINT(format, ...)                \
do {                                            \
    char __szTemp__[256];                       \
    sprintf(__szTemp__, format, __VA_ARGS__);   \
    printf("%s", __szTemp__);                   \
} while (0)

#endif  // _WINDOWS

#else   // debug

// Releaseのとき
#define _FOOTPRINT(str)
#define _FOOT_PRINT(format, ...)

#endif  // debug

#endif  // __Footprint_h__
/* vim: set ts=8 sw=4 sts=4 expandtab: */
