//  ==============================================================
/*!
    @file   types.h
    @brief  Global type definition, macro etc.

            $Author: kurosawa $
            $Date: 2005/03/11 11:52:05 $
            $Revision: 1.6 $
*/
//  ==============================================================
#ifndef _types_h_
#define _types_h_

#include    "cpu.h"

// ------------------ define OK/NG ON/OFF etc. -------------------
#ifndef __cplusplus

#if (! defined(ON)) || (! defined(OFF))
#define ON  1
#define OFF 0
#endif /* !ON/OFF */

#if (! defined(OK)) || (! defined(NG))
#define OK  0
#define NG  -1
#endif /* !OK/NG */

#if (! defined(YES)) || (! defined(NO))
#define YES 1
#define NO  0
#endif /* !YES/NO */

#if (! defined(SET)) || (! defined(CLR))
#define SET 1
#define CLR 0
#endif /* !SET/CLR */

#else  // __cplusplus

enum {ON  = 1, OFF =  0};
enum {OK  = 0, NG  = -1};
enum {YES = 1, NO  =  0};
enum {SET = 1, CLR =  0};

#endif // __cplusplus

// ------------------ define short name -------------------
typedef unsigned char   BYTE;
typedef signed char     S8Bit;
typedef signed char     SByte;
typedef unsigned char   U8Bit;
typedef unsigned char   UByte;
typedef unsigned short  WORD;
typedef signed short    S16Bit;
typedef signed short    S2Byte;
typedef unsigned short  U16Bit;
typedef unsigned short  U2Byte;
typedef unsigned long   DWORD;
typedef signed long     S32Bit;
typedef signed long     S4Byte;
typedef unsigned long   U32Bit;
typedef unsigned long   U4Byte;

// ------------------ define NULL -------------------
#ifndef NULL
#define NULL    ((void *)0)
#endif /* !NULL */

// ------------------ define Bool -------------------
#ifndef __cplusplus

typedef enum {
    FALSE = 0,
    TRUE = 1
} BOOL;
typedef BOOL    Bool;
typedef BOOL    Boolean;

#else  // __cplusplus

// enum {FALSE = false, TRUE = true};
typedef bool    Boolean;
typedef bool    Bool;

#endif // __cplusplus

// ------------------ define union/struct for byte swap -------------------
#if BYTE_ORDER == LITTLE_ENDIAN
typedef union _D_B {
    unsigned long   d;
    signed long sd;
    struct {
        unsigned short  l;
        unsigned short  h;
    } w;
    struct {
        signed short    l;
        signed short    h;
    } sw;
    struct {
        unsigned char   ll;
        unsigned char   l;
        unsigned char   h;
        unsigned char   hh;
    } b;
    struct {
        signed char ll;
        signed char l;
        signed char h;
        signed char hh;
    } sb;
} D_B;

typedef union _W_B {
    unsigned short  w;
    signed short    sw;
    struct {
        unsigned char   l;
        unsigned char   h;
    } b;
    struct {
        signed char l;
        signed char h;
    } sb;
} W_B;

#elif BYTE_ORDER == BIG_ENDIAN
typedef union _D_B {
    unsigned long   d;
    signed long sd;
    struct {
        unsigned short  h;
        unsigned short  l;
    } w;
    struct {
        signed short    h;
        signed short    l;
    } sw;
    struct {
        unsigned char   hh;
        unsigned char   h;
        unsigned char   l;
        unsigned char   ll;
    } b;
    struct {
        signed char hh;
        signed char h;
        signed char l;
        signed char ll;
    } sb;
} D_B;

typedef union _W_B {
    unsigned short      w;
    signed short        sw;
    struct {
        unsigned char   h;
        unsigned char   l;
    } b;
    struct {
        signed char h;
        signed char l;
    } sb;
} W_B;

#else   /* if wrong byte order definition */

#error  BYTE_ORDER not correctly defined.

#endif /* BYTE_ORDER */

// ==============================================================
//  Macros
// ==============================================================
// ------------------------  Macro  -----------------------------
// BitSizeof
// --------------------------------------------------------------
//
//  prog  : 
//  func  : 
//  date  : 
//  update: 
//  entry : 
//  return: 
//  remks : 
//
// --------------------------------------------------------------
#define BitSizeof(x)    (sizeof (x) * 8)


#endif /* !_types_h_ */
