/*==============================================================
 * cf_speed_test.c      CompactFlash speed check tool
 *
 *                      $Author: kuga $
 *                      $Revision: 1.4 $
 *                      $Date: 2008/07/23 14:00:58 $
 *==============================================================
 */
/*****  Include  ************************************************/
// #include "cpu.h"
// #include "types.h"

/*----- SYS -----*/
// #include "int_priority.h"

/*----- LIB -----*/
// #include "wxlib_call.h"
// #include "waitlib_call.h"
// #include "mem_mgr_call.h"
// #include "text_util.h"

/*----- MEDIA CONTROL -----*/
// #include "../FILE/ata_work.h"
// #include "../FILE/cfa_call.h"
// #include "../FILE/disk_if.h"
// #include "../FILE/file_sys_call.h"
// #include "../MEDIA/disk_mgr.h"
// #include "../MEDIA/file_log.h"
// 
/*----- TESTMODE -----*/
#include "cf_speed.h"

#ifdef  __v850__
#pragma section const begin
#endif  /* __v850__ */

#ifdef  __v850__
#pragma section const end
#endif  /* __v850__ */

/*****  Define  ************************************************/
/*----- limit -----*/
#define PAGE_PER_CACHE_REC      6       /* <= MAX_PAGE_PER_CACHE */
#define PAGE_PER_CACHE_PLAY     24      /* <= MAX_PAGE_PER_CACHE */
#define PAGE_PER_CACHE_RDWR     3       /* <= MAX_PAGE_PER_CACHE */

#define NUMOF_LIMIT_SEQWR       2       /* <= MAX_LIMIT_INFO */
#define NUMOF_LIMIT_RNDWR       2       /* <= MAX_LIMIT_INFO */
#define NUMOF_LIMIT_RNDRD       2       /* <= MAX_LIMIT_INFO */
#define NUMOF_LIMIT_RDWR        2       /* <= MAX_LIMIT_INFO */

/*----- log -----*/
#define ENTER_CODE              "\x0D\x0A"

/*----- result -----*/
#define LIMIT_AREA_ALL          0

/*----- random write test -----*/
#define CLUST_PER_RNDWR_TEST    256     /* 4MB * 5 */

/*----- random read and sequencial write -----*/
#define TRACK_PER_PLAY          8
#define TRACK_PER_REC           2
#define LOOP_RDWR_TEST          2048    /* read = 32MB * 8 = 256MB, write = 32MB */

/*****  Static Const Data  *************************************/
static long     sCFSPD_ChkLimitSimple(SCfChkInfo* info, unsigned long time);
static long     sCFSPD_ChkLimitWrSum(SCfChkInfo* info, unsigned long time);
static long     sCFSPD_ChkLimitRdSum(SCfChkInfo* info, unsigned long time);
static long     sCFSPD_ChkLimitRdWrSum(SCfChkInfo* info, unsigned long time);

#ifdef  __v850__
#pragma section const begin
#endif  /* __v850__ */

/*----- result -----*/
static const char       sTestNameSeqWr[]        = "Sequencial Write Test";
static const char       sTestNameRndWr[]        = "Random Write Test";
static const char       sTestNameRdWr[]         = "Random Read and Sequencial Write Test";
static const char       sTestNameRndWrDiv[]     = "Random Write (DIV___) Test";
static const char       sTestNameRdWrDiv[]      = "Rnd Rd (DIV___) and Seq Wr Test";

static const char       sLogFileNameSeqWr[]     = "log_seqw.txt";
static const char       sLogFileNameRndWr[]     = "log_rndw.txt";
static const char       sLogFileNameRdWr[]      = "log_rdwr.txt";
static const char       sResultFileName[]       = "result.txt";

static const char       sLogLimitMsgWr1[]       = "1 WR";
static const char       sLogLimitMsgWr6[]       = "6 WR";
static const char       sLogLimitMsgRd1[]       = "1 RD";
static const char       sLogLimitMsgRd24[]      = "24 RD";
static const char       sLogLimitMsgRdWr2[]     = "2 WR & 8 RD";
static const char       sLogLimitMsgRdWr6[]     = "6 WR & 24 RD";
static const char       sLogPad[]               = "        ";

/*----- limit -----*/
static const SCfLimitInfo       sCfLimitInfoSeqWr[NUMOF_LIMIT_SEQWR] = {
     /* mJudgeMode               mAvrMode                mLimitTime mTitleMsg                 mChkProc */
        {CFSPD_JDG_MODE_VALID,   CFSPD_AVR_MODE_VALID,    500000,   (char*)sLogLimitMsgWr1,   sCFSPD_ChkLimitSimple},
        {CFSPD_JDG_MODE_VALID,   CFSPD_AVR_MODE_INVALID, 1000000,   (char*)sLogLimitMsgWr6,   sCFSPD_ChkLimitWrSum}
};

static const SCfLimitInfo       sCfLimitInfoRndWr[NUMOF_LIMIT_RNDWR] = {
     /* mJudgeMode               mAvrMode                mLimitTime mTitleMsg                 mChkProc */
        {CFSPD_JDG_MODE_VALID,   CFSPD_AVR_MODE_VALID,    500000,   (char*)sLogLimitMsgWr1,   sCFSPD_ChkLimitSimple},
        {CFSPD_JDG_MODE_INVALID, CFSPD_AVR_MODE_INVALID, 1000000,   (char*)sLogLimitMsgWr6,   sCFSPD_ChkLimitWrSum}
};

static const SCfLimitInfo       sCfLimitInfoRndRd[NUMOF_LIMIT_RNDRD] = {
     /* mJudgeMode               mAvrMode                mLimitTime mTitleMsg                 mChkProc */
        {CFSPD_JDG_MODE_INVALID, CFSPD_AVR_MODE_VALID,    100000,   (char*)sLogLimitMsgRd1,   sCFSPD_ChkLimitSimple},
        {CFSPD_JDG_MODE_VALID,   CFSPD_AVR_MODE_INVALID, 1000000,   (char*)sLogLimitMsgRd24,  sCFSPD_ChkLimitRdSum}
};

static const SCfLimitInfo       sCfLimitInfoRdWr[NUMOF_LIMIT_RDWR] = {
     /* mJudgeMode               mAvrMode                mLimitTime mTitleMsg                 mChkProc */
        {CFSPD_JDG_MODE_INVALID, CFSPD_AVR_MODE_VALID,    400000,   (char*)sLogLimitMsgRdWr2, sCFSPD_ChkLimitSimple},
        {CFSPD_JDG_MODE_VALID,   CFSPD_AVR_MODE_INVALID, 1000000,   (char*)sLogLimitMsgRdWr6, sCFSPD_ChkLimitRdWrSum}
};

/*----- random write test -----*/
static const int        sRndWrClustDiv[NUMOF_CFSPD_RNDWR_DIV] = {
          8,  16,  32,  64, 128
};

/*----- read / write test -----*/
static const int        sRdWrClustDiv[NUMOF_CFSPD_RDWR_DIV] = {
/*      8, 16, 32, 64, 128*/
          2,   4,   8,  16,  32,  64
};

#ifdef  __v850__
#pragma section const end
#endif  /* __v850__ */

/*****  Static Variables  **************************************/
#ifdef  __v850__
#pragma section data begin
#endif  /* __v850__ */

/*----- limit -----*/
static SCfTimeHisInfo   SCfTimeHisInfoWr;
static SCfTimeHisInfo   SCfTimeHisInfoRd;
static SCfTimeHisInfo   SCfTimeHisInfoRdWr;

/*----- result -----*/
static SCfSpdInfo       sCfSpdInfoSeqWr;
static SCfSpdInfo       sCfSpdInfoRndWr[NUMOF_CFSPD_RNDWR_DIV];
static SCfSpdInfo       sCfSpdInfoRdWr[NUMOF_CFSPD_RDWR_TEST][NUMOF_CFSPD_RDWR_DIV];

/*----- random read and sequencial write -----*/
static int              sRndWrTestClust;
static int              sRdWrTestLoop;

#ifdef  __v850__
#pragma section data end
#endif  /* __v850__ */

/*****  Static Function's Prototype  ****************************/
static void             sCFSPD_RecLog(unsigned long time);
static void             sCFSPD_ChkLimit(const SCfLimitInfo* limitInfo, SCfSpdInfo* spfInfo, unsigned long time);

/****************************************************************/
/* Sequencisla Write Test                                       */
/****************************************************************/
/*--------------------------------------------------------------
 * CFSPD_SeqWrTest
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : Sequecial Write Test
 *      date  : 2007/11/13 kuga (for BG120)
 *      update: 2008/ 7/15 kuga (for BG119)
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
static CFSPD_ERR sCFSPD_SeqWrTestCore(char* dataBuf, CFSPD_NotifyProgress proc)
{
        unsigned long           startCnt;
        unsigned long           curTime;

        /*----- write -----*/
        for (gCurClust = 0; gCurClust < gAllClust; gCurClust++) {
                startCnt = gCurTimerCount;
                if (DISK_OK != DifWriteBlock(DISK_DEV_CF, gCurClust * SECT_PER_CLUST_CF, (unsigned char*)dataBuf, SECT_PER_CLUST_CF)) {
                        return CFSPD_ERR_NOCARD;
                }
                /*----- rec log -----*/
                curTime = (gCurTimerCount - startCnt) * 10;     /* us */
                sCFSPD_RecLog(curTime);
                CFSPD_PutLog(ENTER_CODE);

                /*----- check limit -----*/
                sCFSPD_ChkLimit(sCfLimitInfoSeqWr, &sCfSpdInfoSeqWr, curTime);

                /*----- notify progress -----*/
                if (proc) {
                        (*proc)(gCurClust * 100 / gAllClust);
                }
        }
        /*----- calc average -----*/
        CFSPD_CalcAverage(&sCfSpdInfoSeqWr, gAllClust);

        return CFSPD_OK;
}

/*--------------------------------------------------------------*/
static CFSPD_ERR sCFSPD_SeqWrSaveLog(void)
{
        /*----- save title & CF info -----*/
        if (CFSPD_OK != CFSPD_WriteTitle2File(sTestNameSeqWr)) {
                return CFSPD_ERR_NOCARD;
        }
        /*----- save result -----*/
        CFSPD_WriteCellFrame("WRITE", sCfLimitInfoSeqWr, NUMOF_LIMIT_SEQWR);
        if (CFSPD_OK != CFSPD_WriteResult2File(0, sCfLimitInfoSeqWr, (const SCfSpdInfo*)&sCfSpdInfoSeqWr)) {
                return CFSPD_ERR_NOCARD;
        }
        return CFSPD_OK;
}

/*--------------------------------------------------------------*/
CFSPD_ERR CFSPD_SeqWrTest(CFSPD_NotifyProgress proc)
{
        CFSPD_ERR       result;
        MEM_INFO        memInfoBuf;

        gCurClust = 0;
        CFSPD_InitTimer();
#if 0
        result = CFSPD_InitCf();
        if (CFSPD_OK != result) {
                return result;          /* no CF or Card Error */
        }
#endif
        CFSPD_StartLog(sLogFileNameSeqWr);
        CFSPD_PutLog("     WR1");
        CFSPD_PutLog(ENTER_CODE);
        CFSPD_PutLog("========");
        CFSPD_PutLog(ENTER_CODE);

        /*----- mem alloc -----*/
        memInfoBuf = MEM_Alloc(BYTES_PER_CLUST_CF);

        /*----- init -----*/
        CFSPD_InitLimitSum(&SCfTimeHisInfoWr, PAGE_PER_CACHE_REC);
        CFSPD_InitResult(&sCfSpdInfoSeqWr, NUMOF_LIMIT_SEQWR);

        /*----- test -----*/
        CFSPD_StartTimer();
        result = sCFSPD_SeqWrTestCore(memInfoBuf.pointer, proc);
        CFSPD_StopTimer();

        /*----- judge -----*/
        CFSPD_Judge(sCfLimitInfoSeqWr, &sCfSpdInfoSeqWr);

        /*----- save log file -----*/
        if (CFSPD_ERR_NOCARD != result) {
                result = sCFSPD_SeqWrSaveLog();
        }
        /*----- mem free -----*/
        MEM_Free(memInfoBuf.id);

        CFSPD_FinishLog();

        /*----- check result -----*/
        if (CFSPD_JDG_TOO_SLOW == sCfSpdInfoSeqWr.mJdg) {
                return CFSPD_ERR_SLOW;
        }
        return CFSPD_OK;
}

/*--------------------------------------------------------------
 * CFSPD_SeqWrInitResult
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : init result
 *      date  : 2008/ 7/30 kuga (for BG119)
 *      update: ---
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
void CFSPD_SeqWrInitResult(void)
{
        CFSPD_InitResult(&sCfSpdInfoSeqWr, NUMOF_LIMIT_SEQWR);
}

/*--------------------------------------------------------------
 * CFSPD_GetSeqWrResult
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : get result
 *      date  : 2007/11/14
 *      update: ---
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
const SCfSpdInfo* CFSPD_SeqWrGetResult(void)
{
        return &sCfSpdInfoSeqWr;
}

/****************************************************************/
/* Random Write Test                                            */
/****************************************************************/
/*--------------------------------------------------------------
 * CFSPD_RndWrTest
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : Random Write Test
 *      date  : 2007/11/13 kuga (for BG120)
 *      update: 2008/ 7/15 kuga (for BG119)
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
static void sCFSPD_RecDiv(int div)
{
        char    str[16];

        CFSPD_PutLog(ENTER_CODE);
        CFSPD_PutLog("================================= ");
        TEXTUTIL_BinToDecStr(str, div, 3, ' ');
        CFSPD_PutLog(str);
        CFSPD_PutLog(" division =================================");
        CFSPD_PutLog(ENTER_CODE);
}

/*--------------------------------------------------------------*/
static CFSPD_ERR sCFSPD_RndWrTestCore(char* dataBuf, CFSPD_NotifyProgress proc, int divMode)
{
        unsigned long           startCnt;
        unsigned long           curTime;
        long                    clustDiv, writeClust;
        int                     div;

        div = sRndWrClustDiv[divMode];
        clustDiv = gAllClust / div;

        /*----- write -----*/
        for (gCurClust = 0; gCurClust < sRndWrTestClust; gCurClust++) {
                writeClust = (gCurClust % div) * clustDiv + (gCurClust / div);

                startCnt = gCurTimerCount;
                if (DISK_OK != DifWriteBlock(DISK_DEV_CF, writeClust * SECT_PER_CLUST_CF, (unsigned char*)dataBuf, SECT_PER_CLUST_CF)) {
                        return CFSPD_ERR_NOCARD;
                }
                /*----- rec log -----*/
                curTime = (gCurTimerCount - startCnt) * 10;     /* us */
                sCFSPD_RecLog(curTime);
                CFSPD_PutLog(ENTER_CODE);

                /*----- check limit -----*/
                sCFSPD_ChkLimit(sCfLimitInfoRndWr, &sCfSpdInfoRndWr[divMode], curTime);

                /*----- notify progress -----*/
                if (proc) {
                        (*proc)(divMode * 100 / NUMOF_CFSPD_RNDWR_DIV + gCurClust * 100 / NUMOF_CFSPD_RNDWR_DIV / sRndWrTestClust);
                }
        }
        /*----- calc average -----*/
        CFSPD_CalcAverage(&sCfSpdInfoRndWr[divMode], sRndWrTestClust);

        return CFSPD_OK;
}

/*--------------------------------------------------------------*/
static CFSPD_ERR sCFSPD_RndWrSaveLog(void)
{
        int     divMode;

        /*----- save title & CF info -----*/
        if (CFSPD_OK != CFSPD_WriteTitle2File(sTestNameRndWr)) {
                return CFSPD_ERR_NOCARD;
        }
        /*----- save result -----*/
        CFSPD_WriteCellFrame("WRITE", sCfLimitInfoRndWr, NUMOF_LIMIT_RNDWR);
        for (divMode = 0; divMode < NUMOF_CFSPD_RNDWR_DIV; divMode++) {
                if (CFSPD_OK != CFSPD_WriteResult2File(sRndWrClustDiv[divMode], sCfLimitInfoRndWr, (const SCfSpdInfo*)&sCfSpdInfoRndWr[divMode])) {
                        return CFSPD_ERR_NOCARD;
                }
        }
        return CFSPD_OK;
}

/*--------------------------------------------------------------*/
CFSPD_ERR CFSPD_RndWrTest(CFSPD_NotifyProgress proc)
{
        int             divMode;
        CFSPD_ERR       result;
        MEM_INFO        memInfoBuf;
        SCfSpdInfo*     spdInfo;

        gCurClust = 0;
        CFSPD_InitTimer();
#if 0
        result = CFSPD_InitCf();
        if (CFSPD_OK != result) {
                return result;          /* no CF or Card Error */
        }
#endif
        CFSPD_StartLog(sLogFileNameRndWr);
        /*----- mem alloc -----*/
        memInfoBuf = MEM_Alloc(BYTES_PER_CLUST_CF);

        sRndWrTestClust = gAllClust;
        if (sRndWrTestClust > CLUST_PER_RNDWR_TEST) {
                sRndWrTestClust = CLUST_PER_RNDWR_TEST;
        }
        spdInfo = sCfSpdInfoRndWr;
        for (divMode = 0; divMode < NUMOF_CFSPD_RNDWR_DIV; divMode++) {
                /*----- init -----*/
                CFSPD_InitLimitSum(&SCfTimeHisInfoWr, PAGE_PER_CACHE_REC);
                CFSPD_InitResult(spdInfo, NUMOF_LIMIT_RNDWR);

                /*----- save title to file -----*/
                sCFSPD_RecDiv(sRndWrClustDiv[divMode]);
                CFSPD_PutLog("     WR1");
                CFSPD_PutLog(ENTER_CODE);
                CFSPD_PutLog("========");
                CFSPD_PutLog(ENTER_CODE);

                /*----- test -----*/
                CFSPD_StartTimer();
                result = sCFSPD_RndWrTestCore(memInfoBuf.pointer, proc, divMode);
                if (CFSPD_OK != result) {
                        break;
                }
                CFSPD_StopTimer();

                /*----- judge -----*/
                CFSPD_Judge(sCfLimitInfoRndWr, spdInfo);
                spdInfo++;
        }
        /*----- save result to file -----*/
        if (CFSPD_ERR_NOCARD != result) {
                result = sCFSPD_RndWrSaveLog();
        }
        /*----- mem free -----*/
        MEM_Free(memInfoBuf.id);

        CFSPD_FinishLog();

        /*----- check result -----*/
        for (divMode = 0; divMode < NUMOF_CFSPD_RNDWR_DIV; divMode++) {
                if (CFSPD_JDG_TOO_SLOW == sCfSpdInfoRndWr[divMode].mJdg) {
                        return CFSPD_ERR_SLOW;
                }
        }
        return CFSPD_OK;
}

/*--------------------------------------------------------------
 * CFSPD_RndWrInitResult
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : init result
 *      date  : 2008/ 7/30 kuga (for BG119(
 *      update: ---
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
void CFSPD_RndWrInitResult(void)
{
        int             divMode;
        SCfSpdInfo*     spdInfo;

        spdInfo = sCfSpdInfoRndWr;
        for (divMode = 0; divMode < NUMOF_CFSPD_RNDWR_DIV; divMode++) {
                CFSPD_InitResult(spdInfo, NUMOF_LIMIT_RNDWR);
                spdInfo++;
        }
}

/*--------------------------------------------------------------
 * CFSPD_RndWrGetResult
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : get result
 *      date  : 2007/11/14
 *      update: ---
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
const SCfSpdInfo* CFSPD_RndWrGetResult(int divMode)
{
        return &sCfSpdInfoRndWr[divMode];
}

/****************************************************************/
/* Random Read and Sequencisla Write Test                       */
/****************************************************************/
/*--------------------------------------------------------------
 * CFSPD_RdWrTest
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : Random Read and Sequencial Write Test
 *      date  : 2007/11/13 kuga (for BG120)
 *      update: 2008/ 7/30 kuga (for BG119)
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
static CFSPD_ERR sCFSPD_RdWrTestCore(char* dataBuf, CFSPD_NotifyProgress proc, int divMode)
{
        unsigned long           startCnt;
        unsigned long           curTime, rdSum, wrSum;
        long                    readClust, div;
        int                     loop, playLoop;

        div = gAllClust / sRdWrClustDiv[divMode];
        playLoop = sRdWrClustDiv[divMode];
        if (playLoop > TRACK_PER_PLAY) {
                playLoop = TRACK_PER_PLAY;
        }
        for (gCurClust = 0; gCurClust < sRdWrTestLoop; gCurClust++) {
                /*----- read -----*/
                rdSum = 0;
                for (loop = 0; loop < playLoop; loop++) {
                        readClust = div * loop + gCurClust;
                        startCnt = gCurTimerCount;
                        if (DISK_OK != DifReadBlock(DISK_DEV_CF, readClust * SECT_PER_CLUST_CF, (unsigned char*)dataBuf, SECT_PER_CLUST_CF)) {
                                return CFSPD_ERR_NOCARD;
                        }
                        /*----- rec log -----*/
                        curTime = (gCurTimerCount - startCnt) * 10;     /* us */
                        rdSum += curTime;
                        sCFSPD_RecLog(curTime);

                        /*----- check limit -----*/
                        sCFSPD_ChkLimit(sCfLimitInfoRndRd, &sCfSpdInfoRdWr[CFSPD_RDWR_TEST_RNDRD][divMode], curTime);
                }
                /*----- pad space -----*/
                for (loop; loop < TRACK_PER_PLAY; loop++) {
                        CFSPD_PutLog(sLogPad);
                }
                /*----- write -----*/
                wrSum = 0;
                for (loop = 0; loop < TRACK_PER_REC; loop++) {
                        startCnt = gCurTimerCount;
                        if (DISK_OK != DifWriteBlock(DISK_DEV_CF, gCurClust * SECT_PER_CLUST_CF, (unsigned char*)dataBuf, SECT_PER_CLUST_CF)) {
                                return CFSPD_ERR_NOCARD;
                        }
                        /*----- rec log -----*/
                        curTime = (gCurTimerCount - startCnt) * 10;     /* us */
                        wrSum += curTime;
                        sCFSPD_RecLog(curTime);

                        /*----- check limit -----*/
                        sCFSPD_ChkLimit(sCfLimitInfoSeqWr, &sCfSpdInfoRdWr[CFSPD_RDWR_TEST_SEQWR][divMode], curTime);
                }
                /*----- read & write -----*/
                curTime = rdSum + wrSum;
                sCFSPD_ChkLimit(sCfLimitInfoRdWr, &sCfSpdInfoRdWr[CFSPD_RDWR_TEST_ALL][divMode], curTime);
                CFSPD_PutLog(ENTER_CODE);

                /*----- notify progress -----*/
                if (proc) {
                        (*proc)(divMode * 100 / NUMOF_CFSPD_RDWR_DIV + gCurClust * 100 / NUMOF_CFSPD_RDWR_DIV / sRdWrTestLoop);
                }
        }
        /*----- calc average -----*/
        CFSPD_CalcAverage(&sCfSpdInfoRdWr[CFSPD_RDWR_TEST_RNDRD][divMode], sRdWrTestLoop * playLoop);
        CFSPD_CalcAverage(&sCfSpdInfoRdWr[CFSPD_RDWR_TEST_SEQWR][divMode], sRdWrTestLoop * TRACK_PER_REC);
        CFSPD_CalcAverage(&sCfSpdInfoRdWr[CFSPD_RDWR_TEST_ALL][divMode],   sRdWrTestLoop);
        return CFSPD_OK;
}

/*--------------------------------------------------------------*/
static CFSPD_ERR sCFSPD_RdWrSaveLog(void)
{
        int     divMode;

        /*----- save title & CF info -----*/
        if (CFSPD_OK != CFSPD_WriteTitle2File(sTestNameRdWr)) {
                return CFSPD_ERR_NOCARD;
        }
        /*----- save result -----*/
        CFSPD_WriteCellFrame("READ", sCfLimitInfoRndRd, NUMOF_LIMIT_RNDRD);
        for (divMode = 0; divMode < NUMOF_CFSPD_RDWR_DIV; divMode++) {
                if (CFSPD_OK != CFSPD_WriteResult2File(sRdWrClustDiv[divMode], sCfLimitInfoRndRd, (const SCfSpdInfo*)&sCfSpdInfoRdWr[CFSPD_RDWR_TEST_RNDRD][divMode])) {
                        return CFSPD_ERR_NOCARD;
                }
        }
        CFSPD_WriteCellFrame("WRITE", sCfLimitInfoSeqWr, NUMOF_LIMIT_SEQWR);
        for (divMode = 0; divMode < NUMOF_CFSPD_RDWR_DIV; divMode++) {
                if (CFSPD_OK != CFSPD_WriteResult2File(sRdWrClustDiv[divMode], sCfLimitInfoSeqWr, (const SCfSpdInfo*)&sCfSpdInfoRdWr[CFSPD_RDWR_TEST_SEQWR][divMode])) {
                        return CFSPD_ERR_NOCARD;
                }
        }
        CFSPD_WriteCellFrame("READ & WRITE", sCfLimitInfoRdWr, NUMOF_LIMIT_RDWR);
        for (divMode = 0; divMode < NUMOF_CFSPD_RDWR_DIV; divMode++) {
                if (CFSPD_OK != CFSPD_WriteResult2File(sRdWrClustDiv[divMode], sCfLimitInfoRdWr, (const SCfSpdInfo*)&sCfSpdInfoRdWr[CFSPD_RDWR_TEST_ALL][divMode])) {
                        return CFSPD_ERR_NOCARD;
                }
        }
        return CFSPD_OK;
}

/*--------------------------------------------------------------*/
CFSPD_ERR CFSPD_RdWrTest(CFSPD_NotifyProgress proc)
{
        int             divMode, playLoop;
        int             test;
        CFSPD_ERR       result;
        MEM_INFO        memInfoBuf;

        gCurClust = 0;
        CFSPD_InitTimer();
#if 0
        result = CFSPD_InitCf();
        if (CFSPD_OK != result) {
                return result;          /* no CF or Card Error */
        }
#endif
        CFSPD_StartLog(sLogFileNameRdWr);

        /*----- mem alloc -----*/
        memInfoBuf = MEM_Alloc(BYTES_PER_CLUST_CF);

        sRdWrTestLoop = gAllClust / TRACK_PER_PLAY;
        if (sRdWrTestLoop > LOOP_RDWR_TEST) {
                sRdWrTestLoop = LOOP_RDWR_TEST;
        }
        for (divMode = 0; divMode < NUMOF_CFSPD_RDWR_DIV; divMode++) {
                /*----- save title to file -----*/
                sCFSPD_RecDiv(sRdWrClustDiv[divMode]);
                CFSPD_PutLog("     RD1     RD2     RD3     RD4     RD5     RD6     RD7     RD8     WR1     WR2");
                CFSPD_PutLog(ENTER_CODE);
                CFSPD_PutLog("================================================================================");
                CFSPD_PutLog(ENTER_CODE);

                /*----- init -----*/
                CFSPD_InitLimitSum(&SCfTimeHisInfoRd,   PAGE_PER_CACHE_PLAY);
                CFSPD_InitLimitSum(&SCfTimeHisInfoWr,   PAGE_PER_CACHE_REC);
                CFSPD_InitLimitSum(&SCfTimeHisInfoRdWr, PAGE_PER_CACHE_RDWR);
                CFSPD_InitResult(&sCfSpdInfoRdWr[CFSPD_RDWR_TEST_RNDRD][divMode], NUMOF_LIMIT_RNDRD);
                CFSPD_InitResult(&sCfSpdInfoRdWr[CFSPD_RDWR_TEST_SEQWR][divMode], NUMOF_LIMIT_SEQWR);
                CFSPD_InitResult(&sCfSpdInfoRdWr[CFSPD_RDWR_TEST_ALL][divMode],   NUMOF_LIMIT_RDWR);

                /*----- test -----*/
                CFSPD_StartTimer();
                result = sCFSPD_RdWrTestCore(memInfoBuf.pointer, proc, divMode);
                if (CFSPD_OK != result) {
                        break;
                }
                CFSPD_StopTimer();

                /*----- judge -----*/
                CFSPD_Judge(sCfLimitInfoRndRd, &sCfSpdInfoRdWr[CFSPD_RDWR_TEST_RNDRD][divMode]);
                CFSPD_Judge(sCfLimitInfoSeqWr, &sCfSpdInfoRdWr[CFSPD_RDWR_TEST_SEQWR][divMode]);
                CFSPD_Judge(sCfLimitInfoRdWr,  &sCfSpdInfoRdWr[CFSPD_RDWR_TEST_ALL][divMode]);
        }
        /*----- save result to file -----*/
        if (CFSPD_ERR_NOCARD != result) {
                result = sCFSPD_RdWrSaveLog();
        }
        /*----- mem free -----*/
        MEM_Free(memInfoBuf.id);

        CFSPD_FinishLog();

        /*----- check result -----*/
        for (divMode = 0; divMode < NUMOF_CFSPD_RDWR_DIV; divMode++) {
                for (test = 0; test < NUMOF_CFSPD_RDWR_TEST; test++) {
                        if (CFSPD_JDG_TOO_SLOW == sCfSpdInfoRdWr[test][divMode].mJdg) {
                                return CFSPD_ERR_SLOW;
                        }
                }
        }
        return CFSPD_OK;
}

/*--------------------------------------------------------------
 * CFSPD_RdWrInitResult
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : init result
 *      date  : 2008/ 7/30 kuga (for BG119)
 *      update: 
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
void CFSPD_RdWrInitResult(void)
{
        int     divMode;

        for (divMode = 0; divMode < NUMOF_CFSPD_RDWR_DIV; divMode++) {
                CFSPD_InitResult(&sCfSpdInfoRdWr[CFSPD_RDWR_TEST_RNDRD][divMode], NUMOF_LIMIT_RNDRD);
                CFSPD_InitResult(&sCfSpdInfoRdWr[CFSPD_RDWR_TEST_SEQWR][divMode], NUMOF_LIMIT_SEQWR);
                CFSPD_InitResult(&sCfSpdInfoRdWr[CFSPD_RDWR_TEST_ALL][divMode],   NUMOF_LIMIT_RDWR);
        }
}

/*--------------------------------------------------------------
 * CFSPD_RdWrGetResult
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : get result
 *      date  : 2007/11/14
 *      update: 2008/ 7/17 kuga (for BG119)
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
const SCfSpdInfo* CFSPD_RdWrGetResult(int testNum, int divMode)
{
        return &sCfSpdInfoRdWr[testNum][divMode];
}

/****** Static Functions ****************************************/
/*--------------------------------------------------------------
 * sCFSPD_RecLog
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : rec log
 *      date  : 2007/11/13 kuga (for BG120)
 *      update: 2008/ 7/15 kuga (for BG119)
 *      entry : time    : 1us * time
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
static void sCFSPD_RecLog(unsigned long time)
{
        char    str[16];

        /*----- rec log -----*/
        TEXTUTIL_BinToDecStr(str, time, 8, ' ');        /* unit = us */
        CFSPD_PutLog(str);
}

/*--------------------------------------------------------------
 * sCFSPD_ChkLimit*
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : save log file
 *      date  : 2007/11/13
 *      update: ---
 *      entry : count   : 10us * count
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
static void sCFSPD_ChkLimit(const SCfLimitInfo* limitInfo, SCfSpdInfo* spfInfo, unsigned long time)
{
        int             limit;
        SCfChkInfo*     chkInfo;

        spfInfo->mAll += time;
        chkInfo = spfInfo->mChkInfo;
        for (limit = 0; limit < spfInfo->mNumofLimit; limit++) {
                chkInfo->mCur = (*limitInfo->mChkProc)(chkInfo, time);
                limitInfo++;
                chkInfo++;
        }
}

/*--------------------------------------------------------------*/
static long sCFSPD_ChkLimitSimple(SCfChkInfo* info, unsigned long time)
{
        /*----- min -----*/
        if (info->mMin > time) {
                info->mMin = time;
        }
        /*----- max -----*/
        if (info->mMax < time) {
                info->mMax = time;
        }
        return time;
}

/*--------------------------------------------------------------*/
static long sCFSPD_ChkLimitWrSum(SCfChkInfo* info, unsigned long time)
{
        long    sum;

        sum = CFSPD_CalcLimitSum(&SCfTimeHisInfoWr, time);
        if (sum < 0) {
                return -1;
        }
        /*----- min -----*/
        if (info->mMin > sum) {
                info->mMin = sum;
        }
        /*----- max -----*/
        if (info->mMax < sum) {
                info->mMax = sum;
        }
        return sum;
/*      return sCFSPD_ChkLimitSimple(info, sum);*/
}

/*--------------------------------------------------------------*/
static long sCFSPD_ChkLimitRdSum(SCfChkInfo* info, unsigned long time)
{
        long    sum;

        sum = CFSPD_CalcLimitSum(&SCfTimeHisInfoRd, time);
        if (sum < 0) {
                return -1;
        }
        return sCFSPD_ChkLimitSimple(info, sum);
}

/*--------------------------------------------------------------*/
static long sCFSPD_ChkLimitRdWrSum(SCfChkInfo* info, unsigned long time)
{
        long    sum;

        sum = CFSPD_CalcLimitSum(&SCfTimeHisInfoRdWr, time);
        if (sum < 0) {
                return -1;
        }
        return sCFSPD_ChkLimitSimple(info, sum);
}

/*--------------------------------------------------------------
 * CFSPD_SaveFiles
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : CF Format
 *      date  : 2007/12/18
 *      update: 2008/ 7/16 kuga (for BG119)
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
static void sCFSPD_RecResult(const char* title, const char* subTitle, CFSPD_JDG jdg)
{
        int     len, i;
        char    str[64];

        if (subTitle) {
                len = Strlen(title) + Strlen(subTitle) + 3;     /* +3 = _:_ */
                CFSPD_PutLog(title);
                CFSPD_PutLog(" : ");
                CFSPD_PutLog(subTitle);
        } else {
                len = Strlen(title);                            /* +3 = _:_ */
                CFSPD_PutLog(title);
        }
        for (i = 0; i < 40 - len; i++) {
                str[i] = ' ';
        }
        str[i] = 0;
        CFSPD_PutLog(str);
        CFSPD_PutLog(" : ");

        /*----- judge -----*/
        switch(jdg) {
        case CFSPD_JDG_FAST:
                CFSPD_PutLog("○ OK");
                break;
        case CFSPD_JDG_NEAR_LIMIT:
                CFSPD_PutLog("● OK but near the limit");
                break;
        case CFSPD_JDG_SLOW_BUT_OK:
                CFSPD_PutLog("▲ OK but slow");
                break;
        case CFSPD_JDG_TOO_SLOW:
                CFSPD_PutLog("× NG");
                break;
        default:
                CFSPD_PutLog("－ Unknown");
                break;
        }
        CFSPD_PutLog(ENTER_CODE);
}

/*--------------------------------------------------------------*/
static CFSPD_ERR sCFSPD_SaveResult(void)
{
        CFSPD_ERR       result;
        int             div, divMode, testNum;
        char            str[64];

        result = CFSPD_StartLogWithDrv(DISK_DRV_CF, sResultFileName);
        if (CFSPD_OK != result) {
                return result;
        }
        /*----- Title & CF Info -----*/
        CFSPD_WriteTitle2File("Result");
        CFSPD_PutLog(ENTER_CODE);

        /*----- sequentially write test -----*/
        sCFSPD_RecResult(sTestNameSeqWr, 0, sCfSpdInfoSeqWr.mJdg);
        CFSPD_PutLog(ENTER_CODE);

        /*----- random write -----*/
        Strcpy(str, (char*)sTestNameRndWrDiv);
        for (divMode = 0; divMode < NUMOF_CFSPD_RNDWR_DIV; divMode++) {
                div = sRndWrClustDiv[divMode];
                TEXTUTIL_BinToDecStr(&str[17], div, 3, ' ');
                str[20]=')';
                sCFSPD_RecResult(str, 0, sCfSpdInfoRndWr[divMode].mJdg);
        }
        CFSPD_PutLog(ENTER_CODE);

        /*----- random read / write test -----*/
        Strcpy(str, (char*)sTestNameRdWrDiv);
        for (divMode = 0; divMode < NUMOF_CFSPD_RDWR_DIV; divMode++) {
                div = sRdWrClustDiv[divMode];
                TEXTUTIL_BinToDecStr(&str[11], div, 3, ' ');
                str[14]=')';
                sCFSPD_RecResult(str, "RD   ", sCfSpdInfoRdWr[CFSPD_RDWR_TEST_RNDRD][divMode].mJdg);
                sCFSPD_RecResult(str, "WR   ", sCfSpdInfoRdWr[CFSPD_RDWR_TEST_SEQWR][divMode].mJdg);
                sCFSPD_RecResult(str, "RD&WR", sCfSpdInfoRdWr[CFSPD_RDWR_TEST_ALL][divMode].mJdg);
                div *= 2;
        }
        return CFSPD_FinishLog();
}

/*--------------------------------------------------------------*/
CFSPD_ERR CFSPD_SaveFiles(void)
{
        CFSPD_ERR       result;
        MEM_INFO        memInfoBuf;

        /*----- copy log file to CF from RAM DISK -----*/
        memInfoBuf = MEM_Alloc(BYTES_PER_CLUST_CF);
        result = CFSPD_CopyFiles(memInfoBuf.pointer, BYTES_PER_CLUST_CF);
        if (CFSPD_OK != result) {
                return result;
        }
        MEM_Free(memInfoBuf.id);

        /*----- save result to CF -----*/
        sCFSPD_SaveResult();

        /*----- save CF description to CF -----*/
        FILEMgr_SaveCfIdentify();

        return CFSPD_OK;
}

