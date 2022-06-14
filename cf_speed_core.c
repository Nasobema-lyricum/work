/*==============================================================
 * cf_speed_core.c      CF speed check tool core
 *
 *                      $Author: kuga $
 *                      $Revision: 1.4 $
 *                      $Date: 2008/ 7/14 14:00:58 $
 *==============================================================
 */
/*****  Include  ************************************************/
#include "types.h"

/*----- DEV -----*/
// #include "timer_drv.h"
// #include "timer_id.h"
// #include "led_config.h"
// #include "led_drv.h"

/*----- LIB -----*/
// #include "wxlib_call.h"
// #include "waitlib_call.h"
// #include "mem_mgr_call.h"
// #include "text_util.h"

/*----- MEDIA CONTROL -----*/
// #include "../FILE/ata_lib.h"
// #include "../FILE/ata_work.h"
// #include "../FILE/cfa_call.h"
// #include "../FILE/ramdisk_drv.h"
// #include "../FILE/dos_types.h"
// #include "../FILE/file_sys_call.h"
// #include "../FILE/file_exp_call.h"
// #include "../FILE/disk_lamp.h"
// #include "../MEDIA/disk_mgr.h"

/*----- TESTMODE -----*/
#include "cf_speed.h"

#ifdef  __v850__
#pragma section const begin
#endif  /* __v850__ */

#ifdef  __v850__
#pragma section const end
#endif  /* __v850__ */

/*****  Define  *************************************************/
#define TIMERID_CF_SPD_CHK      TIMERID_9       /* CMD1         */
#define PRIORITY_OF_CF_SPD_CHK          5

/*----- RAM DIKS -----*/
/*#define       SECT_PER_DISK_RAM       1888*/
#define SECT_PER_DISK_RAM       3168
#define CLUST_PER_DISK_RAM      (SECT_PER_DISK_RAM  / SECT_PER_CLUST_RAM)
#define SECT_PER_FAT_RAM        ((CLUST_PER_DISK_RAM  / (BYTES_PER_SECTOR_RAM  / SIZE_OF_CLUST)) + 1)

/*----- limit -----*/
#define REC_TIME_PER_CACHE      248000          /* 248ms */

/*----- log -----*/
#define ENTER_CODE              "\x0D\x0A"
#define TAB_CODE                "\x09"
#define CHAR_PER_CELL1          8
#define CHAR_PER_CELL2          16
#define CHAR_PER_TITLE          (CHAR_PER_CELL1 * 3)

/*****  Global Variables  ***************************************/
#ifdef  __v850__
#pragma section data begin
#endif  /* __v850__ */

/*----- timer -----*/
unsigned long           gCurTimerCount;

/*----- CF -----*/
unsigned long           gCurClust;
long                    gAllClust;

#ifdef  __v850e__
#pragma section data end
#endif  /* __v850e__ */

/*****  Static Const Data  **************************************/
#ifdef  __v850__
#pragma section const begin
#endif  /* __v850__ */

static const char       sLine[]                 = "================================================================================";
static const char       sTitleModel[]           = "CF Model NO.  = ";
static const char       sTitleSerial[]          = "CF Serial NO. = ";
static const char       sTitleFirmRev[]         = "CF Firm Rev.  = ";
static const char       sTitlePowLvl[]          = "Power Level   = ";

static const char       sTitleMin[]             = "       Min (ms) ";
static const char       sTitleMax[]             = "       Max (ms) ";
static const char       sTitleAvr[]             = "   Average (ms) ";
static const char       sCellLine[]             = "--------------- ";

static const char       sWildCard[]             = "*.*";

#ifdef  __v850__
#pragma section const end
#endif  /* __v850__ */

/*****  Static Variables  ***************************************/
#ifdef  __v850__
#pragma section data begin
#endif  /* __v850__ */

/*----- memory -----*/
static MEM_INFO         sCfCacheMemInfo;
static MEM_INFO         sMemInfoRamDiskSect;
static MEM_INFO         sMemInfoRamDiskFat;

/*----- RAM DISK -----*/
static SRamDiskInfo     sRamDiskInfo;
static SDosFatCacheInfo sFatCacheInfoRam;
static SDosFatSectInfo  sFatSectInfoRam[SECT_PER_FAT_RAM];

/*----- log -----*/
static char*            sLogBufWp;
static short            sLogFp;

#ifdef  __v850e__
#pragma section data end
#endif  /* __v850e__ */

static CFSPD_ERR sCFSPD_Format(int drive);

/****************************************************************/
/*--------------------------------------------------------------
 * CFSPD_Prepear
 * CFSPD_Dispose
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : CF Speed Check Core
 *      date  : 2007/11/13 kuga (for BG120)
 *      update: 2008/ 7/14 kuga (for BG119)
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
void CFSPD_InitCore(void)
{
        /*----- init file system -----*/
        DISKMgr_Init();
        DISKMgr_DiskEnable(DISK_DRV_CF);
        ATA_SetDrqWait(0x6E00 * 4);     /* 80ms */

        /*----- regist disk cache -----*/
        sCfCacheMemInfo = MEM_Alloc(SECT_PER_CACHE_CF * BYTES_PER_SECTOR_CF);
        DISKMgr_RegistDataCache(DISK_DRV_CF, sCfCacheMemInfo.pointer);

        /*----- init access lamp -----*/
        DiskLamp_SetTurnOnProc(LEDDRV_TurnOn);
        DiskLamp_SetTurnOffProc(LEDDRV_TurnOff);

        DiskLamp_Regist(DISK_DEV_CF, DISKLAMP_STATE_CTRL, LEDNO_CF_LUMP);
        DiskLamp_Regist(DISK_DEV_CF, DISKLAMP_STATE_RD,   LEDNO_CF_LUMP);
        DiskLamp_Regist(DISK_DEV_CF, DISKLAMP_STATE_WR,   LEDNO_CF_LUMP);

        DiskLamp_Active();

        /*===== mount RAM DISK =====*/
        RamDrv_Init();          /* RAM disk enable */
        sMemInfoRamDiskSect = MEM_Alloc(SECT_PER_DISK_RAM * BYTES_PER_SECTOR_RAM);
        sMemInfoRamDiskFat = MEM_Alloc(CLUST_PER_DISK_RAM * sizeof(CLUST));

        /*----- disk device mount -----*/
        DiskInitDrv(DISK_DRV_RAM, DISK_DEV_RAM);        /* RAM disk */

        /*----- regist memory -----*/
        sRamDiskInfo.dataPtr      = sMemInfoRamDiskSect.pointer;
        sRamDiskInfo.sectPerDev   = SECT_PER_DISK_RAM;
        sRamDiskInfo.bytesPerSect = BYTES_PER_SECTOR_RAM;
        RamDrv_RegistMemory(&sRamDiskInfo);

        /*----- regist FAT cache for RAM disk -----*/
        sFatCacheInfoRam.fatCacheTbl   = (CLUST*)sMemInfoRamDiskFat.pointer;
        sFatCacheInfoRam.fatSectInfo   = sFatSectInfoRam;
        sFatCacheInfoRam.clustPerCache = CLUST_PER_DISK_RAM;
        sFatCacheInfoRam.sectPerCache  = SECT_PER_FAT_RAM;
        DiskFatCacheRegist(DISK_DRV_RAM, &sFatCacheInfoRam);

        /*----- inserted RAM DISK -----*/
        DISKMgr_InsertProc(DISK_DRV_RAM);
        DISKMgr_LoadDriveParam(DISK_DRV_RAM);
}

/*--------------------------------------------------------------*/
void CFSPD_ExitCore(void)
{
        /*----- remove RAM DISK -----*/

        /*----- free disk cache -----*/
        DISKMgr_FreeDataCache(DISK_DRV_CF);
        MEM_Free(sCfCacheMemInfo.id);
}

/*--------------------------------------------------------------*/
CFSPD_ERR CFSPD_Prepear(void)
{
        sCFSPD_Format(DISK_DRV_RAM);
        return CFSPD_InitCf();
}

/*--------------------------------------------------------------
 * CFSPD_ChkInsert
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : Inserted confirmation
 *      date  : 2007/11/13
 *      update: ---
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
int CFSPD_ChkInsert(void)
{
        return (int)CFA_ChkInsert();
}

/****** Device **************************************************/
/*--------------------------------------------------------------
 * CFSPD_InitCf
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : init test
 *      date  : 2007/11/13 kuag (for BG120)
 *      update: 2008/ 7/14 kuag (for BG119)
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
CFSPD_ERR CFSPD_InitCf(void)
{
/*      CFA_SetWaitTime(102, 2, 4);     *//* original */
        CFA_SetWaitTime(386, 7, 15);
        if (DISK_OK != DifInsertProc(DISK_DEV_CF)) {
#if 0
                CFA_SetWaitTime(386, 7, 15);
                if (DISK_OK != DifInsertProc(DISK_DEV_CF)) {
#endif
                        return CFSPD_ERR_NOCARD;
#if 0
                }
#endif
        }
        /*----- get free cluster count -----*/
        gAllClust = DifGetTotalSector(DISK_DEV_CF) / SECT_PER_CLUST_CF;
        if (gAllClust <= 0) {
                return CFSPD_ERR_NOCARD;
        }
        return CFSPD_OK;
}

/*--------------------------------------------------------------
 * CFSPD_InitTimer
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : init timer
 *      date  : 2007/11/13
 *      update: ---
 *      entry : ---
 *      retrn : ---
 *      remks : 10us interval
 *--------------------------------------------------------------
 */
static void sCFSPD_TimerHandle(int timerID)
{
        gCurTimerCount++;
}

/*--------------------------------------------------------------*/
void CFSPD_InitTimer(void)
{
        gCurTimerCount = 0;
        TIMERDRV_Setup(TIMERID_CF_SPD_CHK, 10, sCFSPD_TimerHandle);
        TIMERDRV_SetPriority(TIMERID_CF_SPD_CHK, PRIORITY_OF_CF_SPD_CHK);
}

/*--------------------------------------------------------------*/
void CFSPD_StartTimer(void)
{
        TIMERDRV_Start(TIMERID_CF_SPD_CHK);
}

/*--------------------------------------------------------------*/
void CFSPD_StopTimer(void)
{
        TIMERDRV_Stop(TIMERID_CF_SPD_CHK);
}

/*--------------------------------------------------------------*/
void CFSPD_WaitTimer(unsigned short time /* ms */)
{
        TIMERDRV_WaitMiliSec(TIMERID_2, time);
}

/****** Limit ***************************************************/
/*--------------------------------------------------------------
 * CFSPD_InitLimitSum
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : init limit
 *      date  : 2008/ 7/18 kuga (for BG119)
 *      update: 
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
void CFSPD_InitLimitSum(SCfTimeHisInfo* hisInfo, int numofCache)
{
        int     i;

        hisInfo->mNumofCache = numofCache;
        hisInfo->mCount = 0;
        hisInfo->mWp = 0;

        for (i = 0; i < MAX_PAGE_PER_CACHE; i++) {
                hisInfo->mTimeHis[i] = 0;
        }
}

/*--------------------------------------------------------------
 * CFSPD_CalcLimitSum
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : calc limit
 *      date  : 2008/ 7/23 kuga (for BG119)
 *      update: 
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
long CFSPD_CalcLimitSum(SCfTimeHisInfo* hisInfo, long newVal)
{
        int     i;
        long    result;

        /*----- add new value -----*/
        hisInfo->mTimeHis[hisInfo->mWp] = newVal;

        hisInfo->mWp++;
        hisInfo->mCount++;
        if (hisInfo->mWp >= hisInfo->mNumofCache) {
                hisInfo->mWp = 0;
        }
        if (hisInfo->mCount < hisInfo->mNumofCache) {
                return -1;
        }
        /*----- sum -----*/
        result = hisInfo->mTimeHis[0];
        for (i = 1; i < hisInfo->mNumofCache; i++) {
                result += hisInfo->mTimeHis[i];
        }
        return result;
}

/****** Result **************************************************/
/*--------------------------------------------------------------
 * CFSPD_InitResult
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : init result
 *      date  : 2007/11/13
 *      update: 2008/ 7/30 kuga (for BG119)
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
void CFSPD_InitResult(SCfSpdInfo* info, int numofLimit)
{
        int             limit;
        SCfChkInfo*     chkInfo;

        info->mNumofLimit = numofLimit;
        info->mAll = 0UL;
        info->mAvr = 0UL;
        info->mJdg = CFSPD_JDG_UNKNOWN;

        chkInfo = info->mChkInfo;
        for (limit = 0; limit < numofLimit; limit++) {
                chkInfo->mMin = 0xFFFFFFFF;
                chkInfo->mMax = 0UL;
                chkInfo->mJdg = CFSPD_JDG_UNKNOWN;
                chkInfo++;
        }
}

/*--------------------------------------------------------------
 * CFSPD_CalcAverage
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : 
 *      date  : 2008/ 7/23 kuga (for BG119)
 *      update: ---
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
void CFSPD_CalcAverage(SCfSpdInfo* spdInfo, int numofTestClust)
{
        spdInfo->mAvr = spdInfo->mAll / numofTestClust;
}

/*--------------------------------------------------------------
 * CFSPD_Judge
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : 
 *      date  : 2008/ 7/18 kuga (for BG119)
 *      update: ---
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
void CFSPD_Judge(const SCfLimitInfo* limitInfo, SCfSpdInfo* spdInfo)
{
        int             limit;
        SCfChkInfo*     chkInfo;

        /*----- judge each limit -----*/
        chkInfo = spdInfo->mChkInfo;
        for (limit = 0; limit < spdInfo->mNumofLimit; limit++) {
                /*----- over limit -----*/
                if (chkInfo->mMax >= limitInfo->mLimitTime) {
                        if (CFSPD_JDG_MODE_VALID == limitInfo->mJudgeMode) {
                                chkInfo->mJdg = CFSPD_JDG_TOO_SLOW;
                        } else {
                                chkInfo->mJdg = CFSPD_JDG_SLOW_BUT_OK;
                        }
                /*----- over 80% -----*/
                } else if ((chkInfo->mMax >= (limitInfo->mLimitTime * 80 / 100)) && (CFSPD_JDG_MODE_VALID == limitInfo->mJudgeMode)) {
                        chkInfo->mJdg = CFSPD_JDG_NEAR_LIMIT;
                } else {
                        chkInfo->mJdg = CFSPD_JDG_FAST;
                }
                /*----- under limit (80%) -----*/
                limitInfo++;
                chkInfo++;
        }
        /*----- judge speed test -----*/
        spdInfo->mJdg = CFSPD_JDG_FAST;
        chkInfo = spdInfo->mChkInfo;
        for (limit = 0; limit < spdInfo->mNumofLimit; limit++) {
                switch(chkInfo->mJdg) {
                case CFSPD_JDG_FAST:
                default:
                        break;
                case CFSPD_JDG_SLOW_BUT_OK:
                        if (CFSPD_JDG_FAST == spdInfo->mJdg) {
                                spdInfo->mJdg = CFSPD_JDG_SLOW_BUT_OK;
                        }
                        break;
                case CFSPD_JDG_NEAR_LIMIT:
                        if (CFSPD_JDG_TOO_SLOW != spdInfo->mJdg) {
                                spdInfo->mJdg = CFSPD_JDG_NEAR_LIMIT;
                        }
                        break;
                case CFSPD_JDG_TOO_SLOW:
                        spdInfo->mJdg = CFSPD_JDG_TOO_SLOW;
                        break;
                }
                chkInfo++;
        }
}

/****** Log *****************************************************/
/*--------------------------------------------------------------
 * CFSPD_StartLog
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : rec log
 *      date  : 2007/11/13
 *      update: ---
 *      entry : ---
 *      retrn : ---
 *      remks : after CFSPD_InitCore
 *--------------------------------------------------------------
 */
CFSPD_ERR CFSPD_StartLogWithDrv(int drv, const char* fileName)
{
        /*----- change drive -----*/
        if (DiskChangeDrive(FILE_PATH_TEMP, drv) < 0) {
                return CFSPD_ERR_NOCARD;
        }
        /*----- create log file -----*/
        sLogFp = DiskCreate(FILE_PATH_TEMP, fileName);
        if (sLogFp < 0) {
                return CFSPD_ERR_NOCARD;
        }
        return CFSPD_OK;
}

/*--------------------------------------------------------------*/
CFSPD_ERR CFSPD_StartLog(const char* fileName)
{
        return CFSPD_StartLogWithDrv(DISK_DRV_RAM, fileName);
}

/*--------------------------------------------------------------*/
CFSPD_ERR CFSPD_FinishLog(void)
{
        if (DISK_OK != DiskClose(sLogFp)) {
                return CFSPD_ERR_NOCARD;
        }
        return CFSPD_OK;
}

/*--------------------------------------------------------------
 * CFSPD_PutLog
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : put charactor
 *      date  : 2007/11/13
 *      update: ---
 *      entry : ---
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
void CFSPD_PutLog(const char *pSrc)
{
        int     len;

        len = Strlen(pSrc);
        if (len != DiskWrite(sLogFp, (unsigned char*)pSrc, len)) {
                /* ASSERT */
        }
}

/****** File ****************************************************/
/*--------------------------------------------------------------
 * CFSPD_WriteTitle2File
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : write TITLE & CF info
 *      date  : 2007/11/13
 *      update: 2008/ 7/15 kuga (for BG119)
 *      entry : 
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
CFSPD_ERR CFSPD_WriteTitle2File(const char* title)
{
        unsigned long   powLvl;
        char            str[16];

        CFSPD_PutLog(ENTER_CODE);
        CFSPD_PutLog(sLine);
        CFSPD_PutLog(ENTER_CODE);
        CFSPD_PutLog("= ");
        CFSPD_PutLog(title);
        CFSPD_PutLog(ENTER_CODE);
        CFSPD_PutLog(sLine);
        CFSPD_PutLog(ENTER_CODE);
        CFSPD_PutLog(ENTER_CODE);

        /*----- test mode title -----*/
        CFSPD_PutLog("----- CF Information -----");
        CFSPD_PutLog(ENTER_CODE);

        /*----- CF model NO -----*/
        CFSPD_PutLog(sTitleModel);
        CFSPD_PutLog(ataCfDevInfo[ATADEV_CF].modelNum);
        CFSPD_PutLog(ENTER_CODE);

        /*----- CF serial NO -----*/
        CFSPD_PutLog(sTitleSerial);
        CFSPD_PutLog(ataCfDevInfo[ATADEV_CF].serialNum);
        CFSPD_PutLog(ENTER_CODE);

        /*----- CF Firmware Revision -----*/
        CFSPD_PutLog(sTitleFirmRev);
        CFSPD_PutLog(ataCfDevInfo[ATADEV_CF].firmwareRev);
        CFSPD_PutLog(ENTER_CODE);

        /*----- Power requirement description -----*/
        CFSPD_PutLog(sTitlePowLvl);
        powLvl  = (ataCfIdentify[0][160] << 8) & 0xFF00;
        powLvl += (ataCfIdentify[0][160] >> 8) & 0x00FF;
        TEXTUTIL_BinToHexStr(str, powLvl, 4, '0');
        CFSPD_PutLog(str);
        CFSPD_PutLog(ENTER_CODE);
        return CFSPD_OK;
}

/*--------------------------------------------------------------
 * CFSPD_WriteCellFrame
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : write cell frame
 *      date  : 2008/ 7/24 kuga (for BG119)
 *      update: ---
 *      entry : 
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
static void sCFSPD_PadStr(char* str, int padSize, char padChar)
{
        while(padSize--) {
                *str++ = padChar;
        }
}

/*--------------------------------------------------------------*/
static void sCFSDP_PutSubTitle(const SCfLimitInfo* limitInfo)
{
        char    str[64];
        int     len;

        CFSPD_PutLog(limitInfo->mTitleMsg);
        len = Strlen(limitInfo->mTitleMsg);

        CFSPD_PutLog(" (");
        TEXTUTIL_BinToDecStr(str, limitInfo->mLimitTime / 1000, 4, ' ');
        CFSPD_PutLog(str);
        CFSPD_PutLog("ms");
        CFSPD_PutLog(")");
        len += 9;               /* Strlen(" ( 100ms)") */

        if (CFSPD_JDG_MODE_INVALID == limitInfo->mJudgeMode) {
                CFSPD_PutLog(" (don't judge)");
                len += 14;      /* Strlen(" (don't judge)") */
        }
        if (CFSPD_AVR_MODE_VALID == limitInfo->mAvrMode) {
                len = (CHAR_PER_CELL2 * 3) - len;
        } else {
                len = (CHAR_PER_CELL2 * 2) - len;
        }
        sCFSPD_PadStr(str, len, ' ');
        str[len] = 0;
        CFSPD_PutLog(str);
}

/*--------------------------------------------------------------*/
CFSPD_ERR CFSPD_WriteCellFrame(const char* title, const SCfLimitInfo* limitInfo, int numOfLimit)
{
        char                    str[64];
        int                     len, limit;
        const SCfLimitInfo*     curLimit;

        /*----- put title -----*/
        CFSPD_PutLog(ENTER_CODE);
        CFSPD_PutLog(title);
        len = CHAR_PER_TITLE - Strlen(title);
        sCFSPD_PadStr(str, len, ' ');
        str[len] = 0;
        CFSPD_PutLog(str);

        /*----- put sub title -----*/
        curLimit = limitInfo;
        for (limit = 0; limit < numOfLimit; limit++) {
                sCFSDP_PutSubTitle(curLimit);
                curLimit++;
        }
        CFSPD_PutLog(ENTER_CODE);

        /*----- put top line -----*/
        sCFSPD_PadStr(str, CHAR_PER_TITLE - 1, '-');
        str[CHAR_PER_TITLE - 1] = ' ';
        str[CHAR_PER_TITLE] = 0;
        CFSPD_PutLog(str);
        curLimit = limitInfo;
        for (limit = 0; limit < numOfLimit; limit++) {
                if (CFSPD_AVR_MODE_VALID == curLimit->mAvrMode) {
                        len = CHAR_PER_CELL2 * 3;
                } else {
                        len = CHAR_PER_CELL2 * 2;
                }
                sCFSPD_PadStr(str, len - 1, '-');
                str[len - 1] = ' ';
                str[len] = 0;
                CFSPD_PutLog(str);
                curLimit++;
        }
        CFSPD_PutLog(ENTER_CODE);

        /*----- put cell name -----*/
        CFSPD_PutLog("Result      Div   Total ");
        curLimit = limitInfo;
        for (limit = 0; limit < numOfLimit; limit++) {
                CFSPD_PutLog(sTitleMin);
                CFSPD_PutLog(sTitleMax);
                if (CFSPD_AVR_MODE_VALID == curLimit->mAvrMode) {
                        CFSPD_PutLog(sTitleAvr);
                }
                curLimit++;
        }
        CFSPD_PutLog(ENTER_CODE);

        /*----- put bottom line -----*/
        CFSPD_PutLog("------- ------- ------- ");
        curLimit = limitInfo;
        for (limit = 0; limit < numOfLimit; limit++) {
                CFSPD_PutLog(sCellLine);
                CFSPD_PutLog(sCellLine);
                if (CFSPD_AVR_MODE_VALID == curLimit->mAvrMode) {
                        CFSPD_PutLog(sCellLine);
                }
                curLimit++;
        }
        CFSPD_PutLog(ENTER_CODE);
        return CFSPD_OK;
}

/*--------------------------------------------------------------
 * CFSPD_WriteResult2File
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : make result
 *      date  : 2007/11/13
 *      update: ---
 *      entry : 
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
static void sCFSDP_PutVal(unsigned long val, int len)
{
        char    str[32];

        TEXTUTIL_BinToDecStr(str, val, len, ' ');
        CFSPD_PutLog(str);
}

/*--------------------------------------------------------------*/
static void sCFSDP_PutFixedPos(unsigned long val, int len, int fixedPos)
{
        char            str[32];
        int             fixedVal, i;
        unsigned long   curVal;

        fixedVal = 1;
        for (i = 0; i < fixedPos; i++) {
                fixedVal *= 10;
        }
        curVal = val / fixedVal;
        TEXTUTIL_BinToDecStr(str, curVal, len - fixedPos - 1, ' ');
        str[len - fixedPos - 1] = '.';
        str[len - fixedPos] = 0;
        CFSPD_PutLog(str);

        curVal = val % fixedVal;
        TEXTUTIL_BinToDecStr(str, curVal, fixedPos, '0');
        CFSPD_PutLog(str);
}

/*--------------------------------------------------------------*/
CFSPD_ERR CFSPD_WriteResult2File(int div, const SCfLimitInfo* limitInfo, const SCfSpdInfo* spdInfo)
{
        int                     limit;
        const SCfLimitInfo*     curLimit;
        const SCfChkInfo*       chkInfo;

        /*----- put result -----*/
        switch(spdInfo->mJdg) {
        case CFSPD_JDG_FAST:
                CFSPD_PutLog("  OK ○");
                break;
        case CFSPD_JDG_NEAR_LIMIT:
                CFSPD_PutLog("  OK ●");
                break;
        case CFSPD_JDG_SLOW_BUT_OK:
                CFSPD_PutLog("  OK ▲");
                break;
        case CFSPD_JDG_TOO_SLOW:
                CFSPD_PutLog("  NG ×");
                break;
        default:
                CFSPD_PutLog("     －");
                break;
        }
        CFSPD_PutLog(TAB_CODE);

        /*----- put div -----*/
        if (div == 0) {
                CFSPD_PutLog("    ---");
        } else {
                sCFSDP_PutVal(div, 7);
        }
        CFSPD_PutLog(TAB_CODE);

        /*----- put total -----*/
        sCFSDP_PutVal(spdInfo->mAll / 1000 / 1000, 6);
        CFSPD_PutLog("s");
        CFSPD_PutLog(TAB_CODE);

        /*----- put value -----*/
        curLimit = limitInfo;
        chkInfo = spdInfo->mChkInfo;
        for (limit = 0; limit < spdInfo->mNumofLimit; limit++) {
                /*----- min -----*/
                sCFSDP_PutFixedPos(chkInfo->mMin, 15, 3);
                CFSPD_PutLog(TAB_CODE);

                /*----- max -----*/
                sCFSDP_PutFixedPos(chkInfo->mMax, 15, 3);
                CFSPD_PutLog(TAB_CODE);

                if (CFSPD_AVR_MODE_VALID == curLimit->mAvrMode) {
                        /*----- avarage -----*/
                        sCFSDP_PutFixedPos(spdInfo->mAvr, 15, 3);
                        CFSPD_PutLog(TAB_CODE);
                }
                curLimit++;
                chkInfo++;
        }
        CFSPD_PutLog(ENTER_CODE);
        return CFSPD_OK;
}

/*--------------------------------------------------------------
 * CFSPD_CopyFiles
 *--------------------------------------------------------------
 *      prog  : kuga
 *      func  : copy log file to CF from RAM DISK
 *      date  : 2008/ 7/16 kuga (for BG119)
 *      update: ---
 *      entry : 
 *      retrn : ---
 *      remks : ---
 *--------------------------------------------------------------
 */
CFSPD_ERR CFSPD_CopyFiles(unsigned char* pBuf, long bufSize)
{
        CFSPD_ERR       result;
        long            restSize, curSize;
        short           sFp, dFp;
        SDosDirEntry    entry;

        /*----- format CF -----*/
        result = DiskInsertProc_BugFix(DISK_DRV_CF);
        if (CFSPD_OK != result) {
                return result;
        }
        result = sCFSPD_Format(DISK_DRV_CF);
        if (CFSPD_OK != result) {
                return result;
        }
        /*----- change drive -----*/
        if (DiskChangeDrive(FILE_PATH_TEMP, DISK_DRV_RAM) < 0) {
                return CFSPD_ERR_NOCARD;
        }
        if (DiskChangeDrive(FILE_PATH_UTIL, DISK_DRV_CF) < 0) {
                return CFSPD_ERR_NOCARD;
        }
        /*----- copy all files -----*/
        if (! DiskFindFirst(FILE_PATH_TEMP, sWildCard, &entry)) {
                return CFSPD_ERR_NOCARD;
        }
        for (;;) {
                /*----- open source file -----*/
                sFp = DiskOpen(FILE_PATH_TEMP, entry.name);
                if (sFp < 0) {
                        return CFSPD_ERR_NOCARD;
                }
                /*----- create target file -----*/
                dFp = DiskCreate(FILE_PATH_UTIL, entry.name);
                if (dFp < 0) {
                        DiskClose(sFp);
                        return CFSPD_ERR_NOCARD;
                }
                restSize = DiskGetFileSize(sFp);

                /*----- copy -----*/
                while(restSize > 0) {
                        if (restSize > bufSize) {
                                curSize = bufSize;
                        } else {
                                curSize = restSize;
                        }
                        /*----- load file -----*/
                        if (curSize != DiskRead(sFp, pBuf, curSize)) {
                                DiskClose(sFp);
                                DiskClose(dFp);
                                return CFSPD_ERR_NOCARD;
                        }
                        /*----- write file -----*/
                        if (curSize != DiskWrite(dFp, pBuf, curSize)) {
                                DiskClose(sFp);
                                DiskClose(dFp);
                                return CFSPD_ERR_NOCARD;
                        }
                        restSize -= curSize;
                }
                DiskClose(sFp);
                DiskClose(dFp);

                /*----- find next file -----*/
                if (! DiskFindNext(FILE_PATH_TEMP, &entry)) {
                        break;  /* file not found */
                }
        }
        return CFSPD_OK;
}

/*--------------------------------------------------------------
 * sCFSPD_Format
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
static CFSPD_ERR sCFSPD_Format(int drive)
{
        CFSPD_ERR       result;

        /*----- format -----*/
        CFA_SetWriteProtect(DISK_PROTECT_OFF);
        if (DISK_OK != DiskExecFormat(drive, DISK_FMT_DOS, DISK_FMT_MODE_LOGICAL)) {
                result = CFSPD_ERR_NOCARD;      /* no CF */
        } else {
                result = CFSPD_OK;
        }
        CFA_SetWriteProtect(DISK_PROTECT_ON);
        return result;
}

/****** Static Functions ****************************************/
