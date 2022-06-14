/*==============================================================
 *
 * types.h  Global type definition
 *
 *      $Author: kurosawa $
 *      $Revision: 1.1 $
 *      $Date: 2008/04/04 05:42:04 $
 *
 *==============================================================
 */
#ifndef _types_h_
#define _types_h_

#ifndef __ASM

#ifdef __v850e__
typedef unsigned char   BYTE;
typedef signed char S8Bit;
typedef signed char SByte;
typedef unsigned char   U8Bit;
typedef unsigned char   UByte;
typedef unsigned short  WORD;
typedef signed short    S16Bit;
typedef signed short    S2Byte;
typedef unsigned short  U16Bit;
typedef unsigned short  U2Byte;
typedef unsigned long   DWORD;
typedef signed long S32Bit;
typedef signed long S4Byte;
typedef unsigned long   U32Bit;
typedef unsigned long   U4Byte;
typedef unsigned int    UINT;
#else /* !__v850e__ */
typedef unsigned char   BYTE;
typedef signed char S8Bit;
typedef signed char SByte;
typedef unsigned char   U8Bit;
typedef unsigned char   UByte;
typedef unsigned short  WORD;
typedef signed short    S16Bit;
typedef signed short    S2Byte;
typedef unsigned short  U16Bit;
typedef unsigned short  U2Byte;
typedef unsigned long   DWORD;
typedef signed long S32Bit;
typedef signed long S4Byte;
typedef unsigned long   U32Bit;
typedef unsigned long   U4Byte;
typedef unsigned int    UINT;
#endif /* __v850e__ */

typedef S32Bit      TimeUnit;                       /* time unit */
typedef S32Bit      Tick;                           /* sequencer tick */
typedef U32Bit      Sample;                         /* wave sample */
#define TICK_NULL       (-1L)
#define SAMPLING_FREQ       44100UL
#define SAMPLE_PER_TIME_UNIT    16                          /* 16 sample = 1 time unit */
#define TIME_UNIT_NULL      (-1L)
#define TIME_UNIT_5MSEC     14L /* same with VS-840 */
#define TIME_UNIT_10MSEC    28L /* same with VS-840 */
#define TIME_UNIT_15MSEC    42L
#define TIME_UNIT_20MSEC    55L /* same with VS-840 */
#define TIME_UNIT_45MSEC    124L
#define TIME_UNIT_50MSEC    138L    /* same with VS-840 */
#define TIME_UNIT_100MSEC   276L    /* same with VS-840 */
#define TIME_UNIT_1SEC      2760L   /* same with VS-840 */
#define TIME_UNIT_PER_DAY   (86400UL * SAMPLING_FREQ / SAMPLE_PER_TIME_UNIT)    /* 238140000 (0x0E31BA60) */
#define TIME_UNIT_MAX       (TIME_UNIT_PER_DAY - 1)
#define TIME_UNIT_FAR_END   0x7FFFFFFFL
#define TIME_UNIT_PHRASE_MIN    TIME_UNIT_1SEC      /* playable phrase length minimum */

/* Hour/minute/second/frame/sub-frame */
typedef struct _meas_beat_ {
    int meas;       /* 0..998 */
    int beat;       /* 0..8 */
    int tick;       /* 0..95 */
} MeasBeat;

#ifndef NULL
#define NULL    ((void *)0)
#endif /* !NULL */

#ifndef DEFINED_BOOL
#define DEFINED_BOOL
typedef enum {
    BOOL_FALSE = 0,
    BOOL_TRUE = !BOOL_FALSE
} BOOL;
typedef BOOL    Bool;
typedef BOOL    Boolean;
#endif /* !DEFINED_BOOL */

#ifndef DEFINED_W_B
#define DEFINED_W_B
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
#endif  /* DEFINED_W_B */

#define FALSE   0
#define TRUE    !FALSE

#define OFF 0
#define ON  !FALSE

#else /* __ASM */

#ifndef NULL
#define NULL    0
#endif /* !NULL */

#define FALSE   0
#define TRUE    !FALSE

#define OFF 0
#define ON  !FALSE

#endif /* __ASM */

#endif /* !_types_h_ */
