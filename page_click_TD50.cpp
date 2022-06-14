//==============================================================
// 
// Click Type page
// 
//==============================================================
#include "xpage_base_td.h"
#include "hmi_prm_song.h"
//==============================================================
// Definitions
//==============================================================
#define TAB_TEMPO	CUR_TAB1
#define TAB_SOUND	CUR_TAB2
#define TAB_OTHER	CUR_TAB3

class CPageClick : public CXPageBaseTD , CMsgListener {
public:
	virtual void OnEventActivate();
	virtual void OnEventDeactivate();
	virtual Boolean OnEventKeyDown(int keyNum);
	virtual Boolean OnEventKeyUp(int keyNum);
	virtual void OnEventUpdate(void);
	virtual Boolean OnEventEncoder(int encNum, int encVal);
	static _EncPrmDefStep EncPrmDef[4][3];
	void DrawEncPrmLine2(short EncNum);
	
private:
	void Startup();
	virtual void RestartObjects() { Startup(); }
	virtual void ListenToMsg(CMsg *theMsg);
	virtual void OnEventAlarm(long theInterval);
	
	static short CurTab;
	static const short NumOfTab = 3;
	short GetCurTab(){return CurTab;}
	void ChangeTab(short tab){
		GetPageCursor();
		SelectTab(&CurTab, tab, NumOfTab);
	}
	
	enum {
		EncStep = 5
	};
	
	// Cursor
	virtual void OnEventResetPrmCursor() {
		//TODO:HMI reset Cursor
	}
	static CHMIXPrm PrmCursor[NumOfTab];
	void GetPageCursor() {
		int curTab = GetCurTab();
		CHMIXPrm aPrm;
		GetCurPrm(&aPrm);
		PrmCursor[curTab] =  aPrm;
	}
	void SetPageCursor() {
		int curTab = GetCurTab();
		SetCursorByPrm(&PrmCursor[curTab]);
	}
};


//==============================================================
// Variables
//==============================================================

namespace {
	CPageClick* mMyPage = 0;
}

short CPageClick::CurTab = 0;
CHMIXPrm CPageClick::PrmCursor[NumOfTab];

//--------------------------------------------------------------
// OnEventAlarm
//--------------------------------------------------------------
void CPageClick::OnEventAlarm(long theInterval)
{
}

void CPageClick::OnEventUpdate()
{
}

//--------------------------------------------------------------
// ListenToMsg
//--------------------------------------------------------------
void CPageClick::ListenToMsg(CMsg *theMsg)
{
}

//--------------------------------------------------------------
// OnEventActivate
//--------------------------------------------------------------
void CPageClick::OnEventActivate()
{
	Startup();
}

//--------------------------------------------------------------
// OnEventDeactivate
//--------------------------------------------------------------
void CPageClick::OnEventDeactivate()
{
	GetPageCursor();
}

//--------------------------------------------------------------
// OnEventEncoder
//--------------------------------------------------------------
CXPageBaseTD::_EncPrmDefStep CPageClick::EncPrmDef[4][3] = {
	{
		{CHMIXPrm::GrpSetup, "ClickTempo",	INVALID,	INVALID},
		{CHMIXPrm::GrpSetup, "ClickBeat",	INVALID,	INVALID},
		{CHMIXPrm::GrpSetup, "ClickRhythmType",	INVALID,	INVALID},
	},
	{
		{CHMIXPrm::GrpSetup, "ClickSound",	INVALID,	INVALID},
		{CHMIXPrm::GrpSetup, "ClickLevel",	INVALID,	5},
		{CHMIXPrm::GrpSetup, "ClickLEDReference",	INVALID,	INVALID},
	},
	{
		{INVALID, "INVALID",	INVALID	, INVALID},
		{INVALID, "INVALID",	INVALID	, INVALID},
		{INVALID, "INVALID",	INVALID	, INVALID},
	},
};
Boolean CPageClick::OnEventEncoder(int encNum, int encVal)
{
	if(encNum == ENC_VALUE) {
		CHMIXPrm aPrm;
		GetCurPrm(&aPrm);
		int aGrp = aPrm.GetGrp();
		int aID = aPrm.GetPrmID();
		
		if( (CHMIXPrm::GrpSetup == aGrp && CHMIPrmSetup::ClickLevel == aID) ||
		    (CHMIXPrm::GrpSetup == aGrp && CHMIPrmSetup::ClickAmbSend == aID) ) {
			PutValByStep(&aPrm, EncStep, encVal);
			return TRUE;
		} else {
			return FALSE;
		}
	} else {
		short aTab = GetCurTab();
		Boolean ret = FALSE;
		ret = HandleEncoderR123withStep(EncPrmDef[aTab], encNum, encVal);
		return ret;
	}
}


//--------------------------------------------------------------
// OnEventKeyDown
//--------------------------------------------------------------
Boolean CPageClick::OnEventKeyDown(int keyNum)
{
	int aVal = 0;

	switch(keyNum) {
	case KEY_F1:
		ChangeTab(CUR_TAB1);
		return TRUE;
	case KEY_F2:
		ChangeTab(CUR_TAB2);
		return TRUE;
	case KEY_F3:
		ChangeTab(CUR_TAB3);
		return TRUE;
	case KEY_F4:
		HMIRequestNextPage(PAGE_CLICK_TIME_CHECK);
		return TRUE;
	case KEY_F5:
		aVal = gHMIPrmSong.GetVal(CHMIPrmSong::ClickSndSw, INVALID);
		gHMIPrmSong.PutVal(CHMIPrmSong::ClickSndSw, !aVal, INVALID);
		return TRUE;
	case KEY_CLICK:
		if(GetCurTab() == TAB_TEMPO) {
			// Cursor move to Tempo
			CHMIXPrm aPrm;
			aPrm.Init(CHMIXPrm::GrpSetup, CHMIPrmSetup::ClickTempo, INVALID);
			SetCursorByPrm(&aPrm);
		} else {
			// Tab move to TEMPO
			ChangeTab(TAB_TEMPO);
			RecreateObjects();
		}
		break;
	default:
		break;
	}
	return FALSE;
}

//--------------------------------------------------------------
// OnEventKeyUp
//--------------------------------------------------------------
Boolean CPageClick::OnEventKeyUp(int keyNum)
{
	return FALSE;
}
void CPageClick::DrawEncPrmLine2(short EncNum)
{
	int posXStart = 0;
	int posXEnd = 0;
	
	int posY = PrmLineDotKnobY;
	long color = C_DGRAY;
	
	switch(EncNum){
	case 0:
		posXStart = PrmLineDotKnob1XStart;
		break;
	case 1:
		posXStart = PrmLineDotKnob2XStart;
		break;
	case 2:
		posXStart = PrmLineDotKnob3XStart;
		break;
	default:
		break;
	}

	posXStart += 5;	// special
	
	posXEnd = posXStart + PrmLineDotLength;
	
	ParseString("XITEM_LINE(%d, %d, %d, %d, %d, %d, %d)", posXStart, posY, posXEnd, posY, color, 0, 1);
}

//--------------------------------------------------------------
// Startup
//--------------------------------------------------------------//
void CPageClick::Startup()
{
	
	short aTab = GetCurTab();
	
	// Title
	DrawTitleMid("CLICK");
	// Tab
	ParseString("XITEM_TAB_UPPER(1, %d, \"TEMPO\")", (CUR_TAB1==aTab) ? 1:0);
	ParseString("XITEM_TAB_UPPER(2, %d, \"SOUND\")", (CUR_TAB2==aTab) ? 1:0);
	ParseString("XITEM_TAB_UPPER(3, %d, \"OTHER\")", (CUR_TAB3==aTab) ? 1:0);
	DrawFuncButton(4, "QUIET CNT");
	
	switch(aTab) {
	case TAB_TEMPO:
		// Parameter Underline
		DrawEncPrmLine(0);
		DrawEncPrmLine(1);
		DrawEncPrmLine(2);
		// R1,2,3
		ParseString("XITEM_STRINGC(%d,  %d, \"Tempo\",   TD_P6x10)", PrmX1posC, PrmYposC);
		ParseString("XITEM_STRINGC(%d,  %d, \"Beat\",    TD_P6x10)", PrmX2posC, PrmYposC);
		ParseString("XITEM_STRINGC(%d,  %d, \"Rhythm\",  TD_P6x10)", PrmX3posC, PrmYposC);
		ParseString("XITEM_VALUE_BOX_C(%d, %d, Setup, ClickTempo, INVALID, L7, 47)", PrmX1posC, ValYposC);
		ParseString("XITEM_VALUE_BOX_C(%d, %d, Setup, ClickBeat,  INVALID, L7, 47)", PrmX2posC, ValYposC);
		ParseString("XITEM_VALUE_BOX_C(%d, %d, Setup, ClickRhythmType,  INVALID, L7, 47)", PrmX3posC, ValYposC);
		// ICON
		ParseString("XITEM_IMAGE(207, 19, \"ICON/METRONOME\")");
		// SOUND SW
		ParseString("XITEM_VALUE_BOX_C( 220, 55, Song, CLICK_SW, INVALID, TD_M, 50)");
		break;
	case TAB_SOUND:
		// Parameter Underline
#if 1
		DrawEncPrmLine2(0);
		DrawEncPrmLine2(1);
		DrawEncPrmLine2(2);
#else
		DrawEncPrmLine(0);
		DrawEncPrmLine(1);
		DrawEncPrmLine(2);
#endif
		// R1
		ParseString("XITEM_STRINGC(%d,  %d, \"Sound\",  TD_P6x10)", PrmX1posC+5, PrmYposC);
		ParseString("XITEM_VALUE_BOX_C(%d,  %d, Setup, ClickSound,  INVALID, L7, 70)", PrmX1posC+5, ValYposC);

		// R2
		ParseString("XITEM_STRINGC(%d,  %d, \"Level\", TD_P6x10)", PrmX2posC+5, PrmYposC);
		ParseString("XITEM_VALUE_BOX(92, 48, Setup, ClickLevel, INVALID, L7, 33)");
		ParseString("XITEM_RECT(%d, %d, %d, %d, %d)", 72+5+2, 49, 82+5+2, 50, C_DGRAY);
		ParseString("XITEM_VALUE_ICON_FADER_EX(81, 39, Setup, ClickLevel, INVALID, MIXER_VOLUME_SINGLE)");
		ParseString("XITEM_IMAGE(128, 57, \"ICON/iconDBsmall\")");

		// R3
		ParseString("XITEM_STRINGC(%d,  %d, \"LED Ref\",  TD_P6x10)", PrmX3posC+5, PrmYposC);
		ParseString("XITEM_VALUE_BOX_C(%d,  %d, Setup, ClickLEDReference, INVALID, L7, 47)", PrmX3posC+5, ValYposC);
		
		// TEMPO
		ParseString("XITEM_STRINGC( 225,  10, \"\x13=\",  S)");
		ParseString("XITEM_VALUE_BOX_C( 240, 10, Setup, ClickTempo, INVALID, TD_SS, 20)");
		// ICON
		ParseString("XITEM_IMAGE(207, 19, \"ICON/METRONOME\")");
		// SOUND SW
		ParseString("XITEM_VALUE_BOX_C( 220, 55, Song, CLICK_SW, INVALID, TD_M, 47)");
		break;
	case TAB_OTHER:
	{
		// Other
		const int ofst = 9;
		const int startY = 20;
		
		// Click Pan
		ParseString("XITEM_VALUE_BOX_EX(%d, %d, %d, %s, %d, TD_P4x6, %d, %d, STD)", 18, startY, CHMIXPrm::GrpSetup, "ClickPan", INVALID, 160, 0);
		// Ambience Send
		ParseString("XITEM_VALUE_BOX_EX(%d, %d, %d, %s, %d, TD_P4x6, %d, %d, STD)", 18, startY+ofst, CHMIXPrm::GrpSetup, "ClickAmbSend", INVALID, 160, 0);
		// Tap Switch
		ParseString("XITEM_VALUE_BOX_EX(%d, %d, %d, %s, %d, TD_P4x6, %d, %d, STD)", 18, startY+ofst*2, CHMIXPrm::GrpSetup, "TapSwitch", INVALID, 160, 0);
		// Tap Pad
		ParseString("XITEM_VALUE_BOX_EX(%d, %d, %d, %s, %d, TD_P4x6, %d, %d, STD)", 18, startY+ofst*3, CHMIXPrm::GrpSetup, "TapPad", INVALID, 160, 0);
		// TEMPO
		ParseString("XITEM_STRINGC( 225,  10, \"\x13=\",  S)");
		ParseString("XITEM_VALUE_BOX_C( 240, 10, Setup, ClickTempo, 0, TD_SS, 20, 0, 0)");
		// ICON
		ParseString("XITEM_IMAGE(207, 19, \"ICON/METRONOME\")");
		// SOUND SW
		ParseString("XITEM_VALUE_BOX_C( 220, 55, Song, CLICK_SW, 0, TD_M, 47, 0, 0)");
	}
		break;
	default:
		break;
	}
	
	SetPageCursor();
}

//--------------------------------------------------------------
// CPageKitEntry
//--------------------------------------------------------------
Boolean CPageClickEntry(UPDW* theEvent)
{
	Boolean ret = PAGE_EVENT_THRU;

	switch (theEvent->common.type) {
	case ePAGE_EVENT_ACTIVATE:
		mMyPage = new CPageClick();
		if (mMyPage != 0) {
			ret = mMyPage->HandleEvent(theEvent);
		}
		break;
	case ePAGE_EVENT_DEACTIVATE:
		if (mMyPage != 0) {
			ret = mMyPage->HandleEvent(theEvent);
		}
		delete mMyPage;
		mMyPage = 0;
		break;
	default:
		if (mMyPage != 0) {
			ret = mMyPage->HandleEvent(theEvent);
		}
		break;
	}
	return ret;
}

//==============================================================
//	PAGE RESOURCE
//==============================================================
extern const SPageRsc pageRscClick =
{
		CPageClickEntry,                    // Event Handler
};
