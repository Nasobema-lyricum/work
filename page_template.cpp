//==============================================================
// 
// Page Kit Tools
// 
//==============================================================
#include "xpage_base_td.h"

//==============================================================
// Definitions
//==============================================================
class CPageTemplate : public CXPageBaseTD , CMsgListener {
public:
	virtual void OnEventActivate();
	virtual void OnEventDeactivate();
	virtual Boolean OnEventKeyDown(int keyNum);
	virtual Boolean OnEventKeyUp(int keyNum);
	virtual void OnEventUpdate(void);
	virtual Boolean OnEventEncoder(int encNum, int encVal);

	static short CurTab;
	static const short NumOfTab = 1;
	short GetCurTab(){return CurTab;}
	void ChangeTab(short tab){SelectTab(&CurTab, tab, NumOfTab);}
	
private:
	void Startup();
	virtual void RestartObjects() { Startup(); }
	virtual void ListenToMsg(CMsg *theMsg);
	virtual void OnEventAlarm(long theInterval);
};


//==============================================================
// Variables
//==============================================================

namespace {
	CPageTemplate* mMyPage = 0;
}

short CPageTemplate::CurTab = 0;

//--------------------------------------------------------------
// OnEventAlarm
//--------------------------------------------------------------
void CPageTemplate::OnEventAlarm(long theInterval)
{
}

void CPageTemplate::OnEventUpdate()
{
}

//--------------------------------------------------------------
// ListenToMsg
//--------------------------------------------------------------
void CPageTemplate::ListenToMsg(CMsg *theMsg)
{
}

//--------------------------------------------------------------
// OnEventActivate
//--------------------------------------------------------------
void CPageTemplate::OnEventActivate()
{
	Startup();
}

//--------------------------------------------------------------
// OnEventDeactivate
//--------------------------------------------------------------
void CPageTemplate::OnEventDeactivate()
{
}

//--------------------------------------------------------------
// OnEventEncoder
//--------------------------------------------------------------
Boolean CPageTemplate::OnEventEncoder(int encNum, int encVal)
{
	return FALSE;
}


//--------------------------------------------------------------
// OnEventKeyDown
//--------------------------------------------------------------
Boolean CPageTemplate::OnEventKeyDown(int keyNum)
{
	switch(keyNum) {
	case KEY_CLICK:
		if (IsEncSlctOn()) HMIRequestNextPage(PAGE_CLICK);
		break;
	default:
		break;
	}
	return FALSE;
}

//--------------------------------------------------------------
// OnEventKeyUp
//--------------------------------------------------------------
Boolean CPageTemplate::OnEventKeyUp(int keyNum)
{
	return FALSE;
}

//--------------------------------------------------------------
// Startup
//--------------------------------------------------------------//
void CPageTemplate::Startup()
{
	// Title
	ParseString("XITEM_STRING(0, 0, \"template\", TD_P5x5)");

	ParseString("XITEM_STRING(12, 10, \"This is TEMPLATE PAGE!!\", TD_P5x5)");
	ParseString("XITEM_IMAGE(45, 20, \"ICON/ALERT\", 0)");
}

//--------------------------------------------------------------
// CPageKitEntry
//--------------------------------------------------------------
Boolean CPageTemplateEntry(UPDW* theEvent)
{
	Boolean ret = PAGE_EVENT_THRU;

	switch (theEvent->common.type) {
	case ePAGE_EVENT_ACTIVATE:
		mMyPage = new CPageTemplate();
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
extern const SPageRsc pageRscTemplate =
{
		CPageTemplateEntry,                    // Event Handler
};
