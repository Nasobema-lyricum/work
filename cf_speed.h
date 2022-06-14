/*==============================================================
 * cf_speed.h           CF speed check tool
 *
 *                      $Author: kuga $
 *                      $Revision: 1.8 $
 *                      $Date: 2008/ 7/17 11:43:20 $
 *==============================================================
 */
#ifndef _cf_speed_h_
#define _cf_speed_h_

/*****  Include  ************************************************/

/*****  Define  *************************************************/
#define MAX_LIMIT_INFO          2

/*----- limit -----*/
#define MAX_PAGE_PER_CACHE      48              /* 3page * 8track * 2 */

/*****  Enum  ***************************************************/
typedef enum {
        CFSPD_ERR_NOCARD = (-100),
        CFSPD_ERR_SLOW,
        CFSPD_OK = 0
} CFSPD_ERR;

typedef enum {
        CFSPD_JDG_UNKNOWN = 0,
        CFSPD_JDG_TOO_SLOW,
        CFSPD_JDG_SLOW_BUT_OK,
        CFSPD_JDG_NEAR_LIMIT,
        CFSPD_JDG_FAST
} CFSPD_JDG;

typedef enum {
        CFSPD_JDG_MODE_INVALID = 0,
        CFSPD_JDG_MODE_VALID,
        NUMOF_CFSPD_JDG_MODE_
} CFSPD_JDG_MODE;

typedef enum {
        CFSPD_AVR_MODE_INVALID = 0,
        CFSPD_AVR_MODE_VALID,
        NUMOF_CFSPD_AVR_MODE_
} CFSPD_AVR_MODE;

/*----- random write test -----*/
typedef enum {
        CFSPD_RNDWR_DIV8 = 0,
        CFSPD_RNDWR_DIV16,
        CFSPD_RNDWR_DIV32,
        CFSPD_RNDWR_DIV64,
        CFSPD_RNDWR_DIV128,
        NUMOF_CFSPD_RNDWR_DIV
} CFSPD_RNDWR_DIV;

/*----- random read and sequencial write -----*/
typedef enum {
        CFSPD_RDWR_DIV2,
        CFSPD_RDWR_DIV4,
        CFSPD_RDWR_DIV8,
/*      CFSPD_RDWR_DIV8 = 0,*/
        CFSPD_RDWR_DIV16,
        CFSPD_RDWR_DIV32,
        CFSPD_RDWR_DIV64,
/*      CFSPD_RDWR_DIV128,*/
        NUMOF_CFSPD_RDWR_DIV
} CFSPD_RDWR_DIV;

typedef enum {
        CFSPD_RDWR_TEST_RNDRD = 0,
        CFSPD_RDWR_TEST_SEQWR,
        CFSPD_RDWR_TEST_ALL,
        NUMOF_CFSPD_RDWR_TEST
} CFSPD_RDWR_TEST;

/*****  Variable's Prototype  ***********************************/
/*----- timer -----*/
extern unsigned long            gCurTimerCount;

/*----- CF -----*/
extern unsigned long            gCurClust;
extern long                     gAllClust;


/*****  Typedef  ************************************************/
typedef void    (*CFSPD_NotifyProgress)(int progress);

/*----- result -----*/
typedef struct {
        unsigned long   mMin;
        unsigned long   mMax;
        unsigned long   mCur;
        CFSPD_JDG       mJdg;
} SCfChkInfo;

typedef struct {
        unsigned long   mAll;
        unsigned long   mAvr;
        int             mNumofLimit;
        CFSPD_JDG       mJdg;
        SCfChkInfo      mChkInfo[MAX_LIMIT_INFO];
} SCfSpdInfo;

/*----- limit -----*/
typedef long    (*CFSPD_ChkLimit)(SCfChkInfo* info, unsigned long time);

typedef struct {
        CFSPD_JDG_MODE  mJudgeMode;
        CFSPD_AVR_MODE  mAvrMode;
        unsigned long   mLimitTime;     /* us */
        char*           mTitleMsg;
        CFSPD_ChkLimit  mChkProc;
} SCfLimitInfo;

typedef struct {
        int             mNumofCache;
        int             mWp;
        long            mCount;
        long            mTimeHis[MAX_PAGE_PER_CACHE];
} SCfTimeHisInfo;

/*****  Function's Prototype  ***********************************/
/*----- cf_speed.c -----*/
extern void                     CFSPD_InitCore(void);
extern void                     CFSPD_ExitCore(void);
extern CFSPD_ERR                CFSPD_Prepear(void);
extern int                      CFSPD_ChkInsert(void);

extern CFSPD_ERR                CFSPD_InitCf(void);
extern void                     CFSPD_InitTimer(void);
extern void                     CFSPD_StartTimer(void);
extern void                     CFSPD_StopTimer(void);
extern void                     CFSPD_WaitTimer(unsigned short time /* ms */);

extern void                     CFSPD_InitLimitSum(SCfTimeHisInfo* hisInfo, int numofCache);
extern long                     CFSPD_CalcLimitSum(SCfTimeHisInfo* hisInfo, long newVal);

extern void                     CFSPD_InitResult(SCfSpdInfo* info, int numofLimit);
extern void                     CFSPD_CalcAverage(SCfSpdInfo* spdInfo, int numofTestClust);
extern void                     CFSPD_Judge(const SCfLimitInfo* limitInfo, SCfSpdInfo* spdInfo);

extern CFSPD_ERR                CFSPD_StartLogWithDrv(int drv, const char* fileName);
extern CFSPD_ERR                CFSPD_StartLog(const char* fileName);
extern CFSPD_ERR                CFSPD_FinishLog(void);
extern void                     CFSPD_PutLog(const char *pSrc);

extern CFSPD_ERR                CFSPD_WriteTitle2File(const char* title);
extern CFSPD_ERR                CFSPD_WriteCellFrame(const char* title, const SCfLimitInfo* limitInfo, int numOfLimit);
extern CFSPD_ERR                CFSPD_WriteResult2File(int div, const SCfLimitInfo* limitInfo, const SCfSpdInfo* spdInfo);
extern CFSPD_ERR                CFSPD_CopyFiles(unsigned char* pBuf, long bufSize);
extern void                     CFSPD_WriteLcdMessage(const char* str, int size);

/*----- cf_speed_test.c -----*/
extern CFSPD_ERR                CFSPD_SeqWrTest(CFSPD_NotifyProgress proc);
extern void                     CFSPD_SeqWrInitResult(void);
extern const SCfSpdInfo*        CFSPD_SeqWrGetResult(void);

extern CFSPD_ERR                CFSPD_RndWrTest(CFSPD_NotifyProgress proc);
extern void                     CFSPD_RndWrInitResult(void);
extern const SCfSpdInfo*        CFSPD_RndWrGetResult(int divMode);

extern CFSPD_ERR                CFSPD_RdWrTest(CFSPD_NotifyProgress proc);
extern void                     CFSPD_RdWrInitResult(void);
extern const SCfSpdInfo*        CFSPD_RdWrGetResult(int testNum, int divMode);

extern CFSPD_ERR                CFSPD_SaveFiles(void);

#endif  /* _cf_speed_h_ */
