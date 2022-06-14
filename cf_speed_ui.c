/*==============================================================
 * cf_speed_ui.c	UI for CompactFlash speed check tool
 *
 *			$Author: kuga $
 *			$Revision: 1.4 $
 *			$Date: 2008/ 7/15 14:00:58 $
 *==============================================================
 */
/*****	Include  ************************************************/
#include "cpu.h"
#include "types.h"

/*----- DEV -----*/
#include "ga_call.h"
#include "sw_config.h"

/*----- LIB -----*/
#include "wxlib_call.h"
#include "waitlib_call.h"

/*----- HMIOS -----*/
#include "../HMIOS/hmi_text_disp.h"

/*----- POLLINGUI -----*/
#include "../POLLINGUI/ui_disp_mgr.h"

/*----- TESTMODE -----*/
#include "cf_speed.h"
#include "cf_speed_ui.h"

/*****	Static Const Data  *************************************/
#ifdef	__v850__
#pragma	section const begin
#endif	/* __v850__ */

static const char STR_CF_SPD_TITLE[]		= "CF Speed Test   ";
static const char STR_CF_INS[]			= "Please insert CF";
static const char STR_CF_START_TEST[]		= "Start Test   [\x0E0]";
static const char STR_CF_NOW_DETECT[]		= "Now Detecting...";
static const char STR_CF_SEQRD_TEST[]		= "SeqW:           ";
static const char STR_CF_RNDRD_TEST[]		= "RndW:           ";
static const char STR_CF_RDWR_TEST[]		= "R/W :           ";
static const char STR_CF_OK[]			= "OK:            \x0E4";
static const char STR_CF_NG[]			= "NG:            \x0E4";

#ifdef	__v850__
#pragma	section const end
#endif	/* __v850__ */

/*****	Static Variables  **************************************/
#ifdef	__v850__
#pragma	section data begin
#endif	/* __v850__ */

static int	sKeepProgress;

#ifdef	__v850__
#pragma	section data end
#endif	/* __v850__ */

/*****	Static Function's Prototype  ****************************/
static void		sCFSPD_UiInit(void);
static CFSPD_ERR	sCFSPD_TestInit(void);
static void		sCFSPD_WaitInsert(void);
static int		sCFSPD_WaitEnterSw(void);
static int		sCFSPD_WaitEject(void);
static void		sCFSPD_DispTestTitle(const char* title);
static CFSPD_ERR	sCFSPD_SeqWrTest(void);
static CFSPD_ERR	sCFSPD_RndWrTest(void);
static CFSPD_ERR	sCFSPD_RdWrTest(void);
static void		sCFSPD_DispProgress(int progress);
static void		sCFSPD_MakeErrDetails(char* bufTop);

/****************************************************************/
/*--------------------------------------------------------------
 * CFSPD_Main
 *--------------------------------------------------------------
 *	prog  : kuga
 *	func  : CF Speed Check
 *	date  : 2007/11/16
 *	update: 2008/ 8/ 1 kuga (for BG119)
 *	entry : ---
 *	retrn : ---
 *	remks : ---
 *--------------------------------------------------------------
 */
void	CFSPD_Main(void)
{
	CFSPD_ERR	resultSeqW, resultRndW, resultRW;
	char		str[32];

	/*----- init -----*/
	CFSPD_InitCore();
	sCFSPD_UiInit();

	for (;;) {
		TEXTDISP_MoveTo(0, 0);
		TEXTDISP_DrawString(STR_CF_SPD_TITLE);

		/*----- wait CF insert -----*/
		TEXTDISP_MoveTo(1, 0);
		TEXTDISP_DrawString(STR_CF_INS);
		sCFSPD_WaitInsert();

		/*----- wait enter -----*/
		TEXTDISP_MoveTo(1, 0);
		TEXTDISP_DrawString(STR_CF_START_TEST);
		if (! sCFSPD_WaitEnterSw()) {
			continue;	/* CF ejected */
		}
		/*=============== test ===============*/
		/*----- prepear test -----*/
		TEXTDISP_MoveTo(1, 0);
		TEXTDISP_DrawString(STR_CF_NOW_DETECT);

		if (CFSPD_OK == sCFSPD_TestInit()) {
			/*----- sequentially write test -----*/
			sCFSPD_DispTestTitle(STR_CF_SEQRD_TEST);
			resultSeqW = sCFSPD_SeqWrTest();

			/*----- random write test -----*/
			sCFSPD_DispTestTitle(STR_CF_RNDRD_TEST);
			resultRndW = sCFSPD_RndWrTest();

			/*----- random read / write test -----*/
			sCFSPD_DispTestTitle(STR_CF_RDWR_TEST);
			resultRW = sCFSPD_RdWrTest();
		} else {
			resultSeqW = CFSPD_ERR_NOCARD;
			resultRndW = CFSPD_ERR_NOCARD;
			resultRW  = CFSPD_ERR_NOCARD;
		}
		/*=============== result ===============*/
		/*----- save result file -----*/
		CFSPD_SaveFiles();

		/*----- display error details -----*/
		TEXTDISP_MoveTo(1, 0);
		if ((CFSPD_OK == resultSeqW) && (CFSPD_OK == resultRndW) && (CFSPD_OK == resultRW)) {
			TEXTDISP_DrawString(STR_CF_OK);
		} else {
			TEXTDISP_DrawString(STR_CF_NG);
		}
		sCFSPD_MakeErrDetails(str);
		TEXTDISP_MoveTo(1, 3);
		TEXTDISP_DrawString(str);

		/*----- wait eject -----*/
		sCFSPD_WaitEject();
	}
	CFSPD_ExitCore();
}

/******	Static Functions ****************************************/
/*--------------------------------------------------------------
 * sCFSPD_UiInit
 *--------------------------------------------------------------
 *	prog  : kuga
 *	func  : CF Speed Check
 *	date  : 2007/11/12
 *	update: ---
 *	entry : ---
 *	retrn : ---
 *	remks : ---
 *--------------------------------------------------------------
 */
static void sCFSPD_UiInit(void)
{
	/*----- init HMIOS -----*/
	LCD2DRV_SetCursorDisplay(LCD2_CURSOR_DISPLAY_OFF);
	TEXTDISP_Init();

	/*----- init SW -----*/
	GA_SwInit();
	GA_StartSwScan();	// start switch scan
}

/*--------------------------------------------------------------
 * sCFSPD_TestInit
 *--------------------------------------------------------------
 *	prog  : kuga
 *	func  : Init all test
 *	date  : 2008/ 7/30 kuga (for BG119)
 *	update: ---
 *	entry : ---
 *	retrn : ---
 *	remks : ---
 *--------------------------------------------------------------
 */
CFSPD_ERR sCFSPD_TestInit(void)
{
	CFSPD_ERR	result;

	/*----- init any test -----*/
	CFSPD_SeqWrInitResult();
	CFSPD_RndWrInitResult();
	CFSPD_RdWrInitResult();

	/*----- init core -----*/
	result = CFSPD_Prepear();
	if (CFSPD_OK != result) {
		return result;
	}
	return CFSPD_OK;
}

/*--------------------------------------------------------------
 * sCFSPD_WaitInsert
 *--------------------------------------------------------------
 *	prog  : kuga
 *	func  : Wait CF insert
 *	date  : 2007/11/12
 *	update: ---
 *	entry : ---
 *	retrn : ---
 *	remks : ---
 *--------------------------------------------------------------
 */
static void sCFSPD_WaitInsert(void)
{
	for (;;) {
		if (CFSPD_ChkInsert()) {
			break;
		}
	}
}

/*--------------------------------------------------------------
 * sCFSPD_WaitEnterSw
 *--------------------------------------------------------------
 *	prog  : kuga
 *	func  : Wait [ENTER] switch
 *	date  : 2007/11/12
 *	update: ---
 *	entry : ---
 *	retrn : ---
 *	remks : ---
 *--------------------------------------------------------------
 */
static int sCFSPD_WaitEnterSw(void)
{
	SWEdge	sw;

	/*----- clear event -----*/
	while(GA_GetSW(&sw)) {
	}
	/*----- wait -----*/
	for (;;) {
		Wait_msec(10);
		while(GA_GetSW(&sw)) {
			if (UI_SW_ENTER == sw.number) {
				if (SW_ACTION_PRESS == sw.edge) {
					return !0;
				}
			}
		}
		/*----- eject check -----*/
		if (! CFSPD_ChkInsert()) {
			return 0;
		}
	}
}

/*--------------------------------------------------------------
 * sCFSPD_WaitEject
 *--------------------------------------------------------------
 *	prog  : kuga
 *	func  : Wait [ENTER] switch
 *	date  : 2007/11/16
 *	update: ---
 *	entry : ---
 *	retrn : ---
 *	remks : ---
 *--------------------------------------------------------------
 */
static int sCFSPD_WaitEject(void)
{
	/*----- wait -----*/
	for (;;) {
		/*----- eject check -----*/
		if (! CFSPD_ChkInsert()) {
			return 0;
		}
	}
}

/*--------------------------------------------------------------
 * sCFSPD_DispTestTitle
 *--------------------------------------------------------------
 *	prog  : kuga
 *	func  : Test
 *	date  : 
 *	update: 2008/ 7/30
 *	entry : ---
 *	retrn : ---
 *	remks : ---
 *--------------------------------------------------------------
 */
static void sCFSPD_DispTestTitle(const char* title)
{
	TEXTDISP_MoveTo(1, 0);
	TEXTDISP_DrawString(title);
	UIMGR_DispProgress(0);
}

/*--------------------------------------------------------------
 * sCFSPD_SeqWrTest
 *--------------------------------------------------------------
 *	prog  : kuga
 *	func  : Test
 *	date  : 2007/11/12
 *	update: ---
 *	entry : ---
 *	retrn : ---
 *	remks : ---
 *--------------------------------------------------------------
 */
static CFSPD_ERR sCFSPD_SeqWrTest(void)
{
	CFSPD_ERR	result;

	sKeepProgress = -1;

	result = CFSPD_SeqWrTest(sCFSPD_DispProgress);

	return result;
}

/*--------------------------------------------------------------*/
static CFSPD_ERR sCFSPD_RndWrTest(void)
{
	CFSPD_ERR	result;

	sKeepProgress = -1;

	result = CFSPD_RndWrTest(sCFSPD_DispProgress);

	return result;
}

/*--------------------------------------------------------------*/
static CFSPD_ERR sCFSPD_RdWrTest(void)
{
	CFSPD_ERR	result;

	sKeepProgress = -1;

	result = CFSPD_RdWrTest(sCFSPD_DispProgress);

	return result;
}

/*--------------------------------------------------------------
 * sCFSPD_DispProgress
 *--------------------------------------------------------------
 *	prog  : kuga
 *	func  : Test
 *	date  : 2007/11/12
 *	update: ---
 *	entry : ---
 *	retrn : ---
 *	remks : ---
 *--------------------------------------------------------------
 */
static void sCFSPD_DispProgress(int curProg)
{
	if (sKeepProgress != curProg) {
		sKeepProgress = curProg;
		UIMGR_DispProgress(curProg);
		Wait_msec(20);
	}
}

/*--------------------------------------------------------------
 * sCFSPD_MakeErrDetails
 *--------------------------------------------------------------
 *	prog  : kuga
 *	func  : Test
 *	date  : 2007/11/14
 *	update: 2008/ 7/25 kuga (for BG119)
 *	entry : ---
 *	retrn : ---
 *	remks : ---
 *--------------------------------------------------------------
 */
static char* sCFSPD_MakeErrDetailsOne(CFSPD_JDG jdg, char* bufTop)
{
	char*	pDst;

	pDst = bufTop;

	switch(jdg) {
	case CFSPD_JDG_FAST:
		*pDst++ = 'o';
		break;
	case CFSPD_JDG_SLOW_BUT_OK:
		*pDst++ = '\x0E5';
		break;
	case CFSPD_JDG_NEAR_LIMIT:
		*pDst++ = '\x0E6';
		break;
	case CFSPD_JDG_TOO_SLOW:
		*pDst++ = 'x';
		break;
	default:
		*pDst++ = '-';
		break;
	}
	return pDst;
}

/*--------------------------------------------------------------*/
static CFSPD_JDG sCFSPD_ChooseJudge(CFSPD_JDG curJdg, CFSPD_JDG newJdg)
{
	switch(newJdg) {
	case CFSPD_JDG_FAST:
		if (CFSPD_JDG_UNKNOWN == curJdg) {
			curJdg = CFSPD_JDG_FAST;
		}
		break;
	case CFSPD_JDG_SLOW_BUT_OK:
		if ((CFSPD_JDG_UNKNOWN == curJdg) || (CFSPD_JDG_FAST == curJdg))  {
			curJdg = CFSPD_JDG_SLOW_BUT_OK;
		}
		break;
	case CFSPD_JDG_NEAR_LIMIT:
		if (CFSPD_JDG_TOO_SLOW != curJdg) {
			curJdg = CFSPD_JDG_NEAR_LIMIT;
		}
		break;
	case CFSPD_JDG_TOO_SLOW:
		curJdg = CFSPD_JDG_TOO_SLOW;
		break;
	default:
		break;
	}
	return curJdg;
}

/*--------------------------------------------------------------*/
static void sCFSPD_MakeErrDetails(char* bufTop)
{
	const SCfSpdInfo*	info;
	char*			pDst;
	int			testNum, divMode;
	CFSPD_JDG		jdg;

	pDst = bufTop;

	/*----- sequentially write test -----*/
	info = CFSPD_SeqWrGetResult();
	pDst = sCFSPD_MakeErrDetailsOne(info->mJdg, pDst);

	*pDst++ = '/';

	/*----- random write -----*/
	jdg = CFSPD_JDG_UNKNOWN;
	for (divMode = 0; divMode < NUMOF_CFSPD_RNDWR_DIV; divMode++) {
		info = CFSPD_RndWrGetResult(divMode);
		jdg = sCFSPD_ChooseJudge(jdg, info->mJdg);
	}
	pDst = sCFSPD_MakeErrDetailsOne(jdg, pDst);

	*pDst++ = '/';

	/*----- random read / write test -----*/
	jdg = CFSPD_JDG_UNKNOWN;
	for (testNum = 0; testNum < NUMOF_CFSPD_RDWR_TEST; testNum++) {
		for (divMode = 0; divMode < NUMOF_CFSPD_RDWR_DIV; divMode++) {
			info = CFSPD_RdWrGetResult(testNum, divMode);
			jdg = sCFSPD_ChooseJudge(jdg, info->mJdg);
		}
	}
	pDst = sCFSPD_MakeErrDetailsOne(jdg, pDst);
	*pDst++ = 0;
}
