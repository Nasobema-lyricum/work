//==============================================================
//
// song folder select
//
//==============================================================
#include "xpage_base_td.h"
#include "xitem_list.h"
#include "file_list.h"
#include "file_path_def.h"
#include "hmi_prm_song.h"
#include "hmi_prm_smpl.h"
#include "chmi_song_mgr.h"
#include "_prfile2.h"

#include "audio_play_if.h"	//BTS #4308

//==============================================================
// Definitions
//==============================================================
#define DEFAULT_PATH	CARD_DRV_PATH"EXPORT"

class CWAVFile_Filter : public CExtFilter {
	virtual bool Filter(bool file, const char* filename) {
		if (!file) {	// dir
			if (filename[0] == '.')  {
				return false;
			}
			return true;
		}
		if (filename[0] == '.') return false;
		for (int i = 0; i < LIST_NAME_LEN; i++) {
			if (filename[i] == '\0') break;
			if (filename[i] == '.' &&
			     CmpExt(&filename[i+1], "WAV")) return true;
		}
		return false;
	}
};

static char mSongFolderPath[256];
static CWAVFile_Filter wavFileFilter;

static bool	GetParentFolderPath(char *theName)
{
	char strTmp[FILE_PATH_LEN];
	strcpy(strTmp, mSongFolderPath);
	int len = strlen(strTmp);
	int i = len - 1;
	// 最後が '/' の場合はスキップ
	if (strTmp[i] == '/') {
		i--;
	}
	while (strTmp[i] != '/') {
		i--;
		if (i <= 0) {
			// 親ディレクトリが無い
			return false;
		}
	}
	// ひとつ前の '/' で切る
	strTmp[i] = '\0';

	// ルートでない場合
	sprintf(theName, "%s", strTmp);
	return true;
}

static char* TrimFolderName(char *thePath) {
	char *pFolderName;
	int len = strlen(thePath);
	pFolderName = &thePath[len - 1];
	// 最後が '/' の場合は '/' を削除しつつ、スキップ
	if (*pFolderName == '/') {
		*pFolderName = 0;
		pFolderName--;
	}
	while (*pFolderName != '/') {
		pFolderName--;
		if (pFolderName == &thePath[0]) {
			// "/" が無い
			return thePath;
		}
	}
	// "/" の次の文字列以降を返す
	return (pFolderName + 1);
}

class CXPageItemSongWavList : public CXPageItemList {
	char	mTmpStr[64];
	CFileList* mSongWavFileList;
public:
	enum {
		CellIcon,
		CellName,
		CellEnd
	};
	CXPageItemSongWavList(CXItemMgr *theHdl) : CXPageItemList(theHdl)
	{
		static bool isPathInit = false;
		if (!isPathInit) {
			PF_STAT aStat;
			if(pf_fstat(DEFAULT_PATH, &aStat) < 0) {
				// folder が存在しない
				strcpy(mSongFolderPath, CARD_DRV_PATH);
				// 最後の "/" はトル
				mSongFolderPath[strlen(mSongFolderPath)-1] = 0;
			} else {
				strcpy(mSongFolderPath, DEFAULT_PATH);
			}
			isPathInit = true;
		}

		mSongWavFileList = new CFileList;
		mSongWavFileList->SetFilter(&wavFileFilter);
		mSongWavFileList->Open(mSongFolderPath);

		mCell[CellIcon].Init(CXListCell2::TypeImage,  15, 3, 1, CXListCell2::AlignLeft,	 eFONT_ID_S, OFF);
		mCell[CellName].Init(CXListCell2::TypeString,  104, 2, 1, CXListCell2::AlignLeft,	 eFONT_ID_TD_S_ASCHII, ON);
	}
	~CXPageItemSongWavList() {
		if(mSongWavFileList) {
			delete mSongWavFileList;
		}
	}
	int OnEventGetNumOfItems() {
		// リスト上の数を返す
		return mSongWavFileList->NumOfItem();
	}
	virtual const char *OnEventGetIndex(int theCell)
	{
		switch (theCell) {
		case CellIcon:
			return "Icon";
		case CellName:
			return "Name";
		}
		return NULL;
	}
	const SBitMap* OnEventGetItemImage(int theIdx, int theCell)
	{
		switch (theCell) {
		case CellIcon:
		{
			const SBitMap *aBm;
			if (mSongWavFileList->IsDir(theIdx)) {
				aBm = GetImage("ICON/iconFolderSelect");
			} else {
				aBm = GetImage("ICON/iconFileSelect");
			}
			return aBm;
		}
			// break;
		}
		return NULL;
	}
	virtual const char *OnEventGetItemString(int theIdx, int theCell)
	{
		switch (theCell) {
		case CellIcon:
			{
				sprintf(mTmpStr, "%s", "Icon");
				return mTmpStr;
			}
		case CellName:
			{
				return mSongWavFileList->GetName(theIdx);
			}
		default:
			break;
		}
		return NULL;
	}

	virtual void OnEventSetPrm(CXPageToken *theToken)
	{
		int	x, y;

		CXPageItemList::OnEventSetPrm(theToken);
		GetFramePos(x, y);
	}

	virtual void OnEventAlarm()
	{
		CXPageItemList::OnEventAlarm();
	}
/*
	void SetFolder(const char *thePath) {
		mSongWavFileList->Open(thePath);
		Refresh();
	}
*/
	void GetFilePath(char *theFilePath) {
		sprintf(theFilePath, "%s%s", mSongWavFileList->GetPath(), mSongWavFileList->GetName(GetCursor()));
	}
	void GetDirPath(char *theFilePath) {
		char aCurFolderName[256];
		sprintf(aCurFolderName, "%s", mSongWavFileList->GetPath());
		sprintf(theFilePath, "%s", TrimFolderName(aCurFolderName));
	}

	const char *GetFolderPath(void) {
		return mSongWavFileList->GetPath();
	}
	virtual void OnEventSetCursor(int theCursor) {
		CXPageItemList::OnEventSetCursor(theCursor);
		SelectCurSmpl();
	}
	void SelectCurSmpl(void) {
		char strPath[256];
		// song manager for preview
		if(OnEventGetNumOfItems() > 0) {
			GetFilePath(strPath);
			gHMISongMgr.SelectPreview(strPath);
		}
	}
	void UpFolder(void) {
		char aNewPath[256];
		char aCurFolderName[256];
		strcpy(aCurFolderName, mSongWavFileList->GetPath());
		char *pName = TrimFolderName(aCurFolderName);

		strcpy(aNewPath, mSongFolderPath);
		if (GetParentFolderPath(aNewPath)) {
			// get folder name
			mSongWavFileList->Open(aNewPath);
			SetCursor(mSongWavFileList->FindIndex(pName));	// cursor keep
			strcpy(mSongFolderPath, aNewPath);
		} else {
			// 最上位フォルダ
			// nop
			return;
		}
		SelectCurSmpl();
		Refresh();
	}
	void SelectFolder(void) {
		if (GetNumOfItems() <= 0) {
			return;
		}
		int aCurIdx = GetCursor();
		if (!mSongWavFileList->IsDir(aCurIdx)) {
			return;
		}
		char aFolderPath[256];
		// make path
		sprintf(aFolderPath, "%s/%s", mSongFolderPath, mSongWavFileList->GetName(aCurIdx));
		// get folder name
		mSongWavFileList->Open(aFolderPath);
		strcpy(mSongFolderPath, aFolderPath);
		SetCursor(0);	// cursor reset
		SelectCurSmpl();
		Refresh();
	}
	bool IsFolder(void) {
		if (GetNumOfItems() <= 0) {
			return false;
		}
		return mSongWavFileList->IsDir(GetCursor());
	}
	void UpdateList(void) {
		mSongWavFileList->Update();
		SetCursor(0);	// cursor reset
		Refresh();
	}
};

CXPageItem *CallXPageItemSongWavList(CXPageItem *thePtr, CXItemMgr *theHdl)
{
	if (thePtr == NULL) {

		return (CXPageItem *)(new CXPageItemSongWavList(theHdl));
	}
	delete (CXPageItemSongWavList *)thePtr;
	return NULL;
}

class CPageSongFolder : public CXPageBaseTD , CMsgListener {
public:
	virtual void OnEventActivate();
	virtual void OnEventDeactivate();
	virtual Boolean OnEventKeyDown(int keyNum);
	virtual Boolean OnEventKeyUp(int keyNum);
	virtual void OnEventUpdate(void);
	virtual Boolean OnEventEncoder(int encNum, int encVal);
	virtual Boolean	HandleMedia(int theState);

private:
	enum {
		ListXPos = 0,
		ListYPos = 17,
		ListXSize = 126,
		ListItemYSize = 9,
		ListItems = 4,
	};
	CXPageItemSongWavList	*pList;

	void Startup();
	virtual void RestartObjects() { Startup(); }
	virtual void ListenToMsg(CMsg *theMsg);
	virtual void OnEventAlarm(long theInterval);
	bool DoSelect(void);
	void HandlePreview(void);
};

//==============================================================
// Variables
//==============================================================

namespace {
	CPageSongFolder* mMyPage = 0;
}

//--------------------------------------------------------------
// OnEventAlarm
//--------------------------------------------------------------
void CPageSongFolder::OnEventAlarm(long theInterval)
{
}

void CPageSongFolder::OnEventUpdate()
{
	// draw folder path
	cLCDP->SetFont(eFONT_ID_TD_S);
	cLCDP->SetFgColor(eLCD_COLOR_WHITE);
	cLCDP->SetBkColor(eLCD_COLOR_BLACK);

	SRect aRect;
	SetSRect(&aRect, ListXPos + 4, ListYPos - ListItemYSize, 127, ListYPos -2);
	cLCDP->EraseRect(&aRect);

	if(gHMISongMgr.IsMediaAttached()) {
		cLCDP->DrawString(aRect.left + 1, aRect.top + 1, gHMIPrmSmpl.ConvPathForDisp(mSongFolderPath));	// "A:" をトル
	}
}

//--------------------------------------------------------------
// ListenToMsg
//--------------------------------------------------------------
void CPageSongFolder::ListenToMsg(CMsg *theMsg)
{
}

//--------------------------------------------------------------
// OnEventActivate
//--------------------------------------------------------------
void CPageSongFolder::OnEventActivate()
{
	gHMISongMgr.EnterSamplePreview();
	Startup();
}

//--------------------------------------------------------------
// OnEventDeactivate
//--------------------------------------------------------------
void CPageSongFolder::OnEventDeactivate()
{
	gHMISongMgr.ExitSamplePreview();
}

//--------------------------------------------------------------
// OnEventEncoder
//--------------------------------------------------------------
Boolean CPageSongFolder::OnEventEncoder(int encNum, int encVal)
{
	return FALSE;
}

//--------------------------------------------------------------
// DoSelect
//--------------------------------------------------------------
bool CPageSongFolder::DoSelect(void) {
	char strTmpName[256];
	char strTmpDir[256];
	int aRet;

	// set name
	if (pList) {
		if (pList->GetNumOfItems() <= 0) {
			return false;
		}
		pList->GetFilePath(strTmpName);
		pList->GetDirPath(strTmpDir);
	}

	// go to next page
	HMIRequestNextPage(PAGE_SONG);
	return true;
}

//--------------------------------------------------------------
// HandlePreview
//--------------------------------------------------------------
void CPageSongFolder::HandlePreview(void) {
	if (gHMISongMgr.IsPreviewOn()) {
		gHMISongMgr.StopPreview();
	} else {
		if (CAudioPlayer::GetStat() == CAudioPlayer::STAT_CLOSE) {	//BTS #4308
			CHMIMsgBox::DispMessage(MSG_BOX_ID_UNSUPPORTED_FORMAT);
		} else {
			gHMISongMgr.PlayPreview();
		}
	}
}

//--------------------------------------------------------------
// HandleMedia
//--------------------------------------------------------------
Boolean	CPageSongFolder::HandleMedia(int theState)
{
	if(pList) {
		pList->UpdateList();
	}
	HMIRequestPageUpdate();
	return TRUE;
}

//--------------------------------------------------------------
// OnEventKeyDown
//--------------------------------------------------------------
Boolean CPageSongFolder::OnEventKeyDown(int keyNum)
{
	switch(keyNum) {
	case KEY_UP:
	case KEY_DEC:
		if (pList) {
			pList->HandleKeyUp();
		}
		break;
	case KEY_DOWN:
	case KEY_INC:
		if (pList) {
			pList->HandleKeyDown();
		}
		break;
	case KEY_RIGHT:
		if (pList) {
			pList->SelectFolder();
			HMIRequestPageUpdate();
		}
		break;
	case KEY_LEFT:
	case KEY_F2:
		if (pList) {
			pList->UpFolder();
			HMIRequestPageUpdate();
		}
		break;
	case KEY_F1:
		HandlePreview();
		break;
	case KEY_F3:
	case KEY_ENTER:
		if (pList) {
			if (pList->IsFolder()) {
				pList->SelectFolder();
				HMIRequestPageUpdate();
				return TRUE;
			}
		}
		DoSelect();
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

//--------------------------------------------------------------
// OnEventKeyUp
//--------------------------------------------------------------
Boolean CPageSongFolder::OnEventKeyUp(int keyNum)
{
	return FALSE;
}

//--------------------------------------------------------------
// Startup
//--------------------------------------------------------------//
void CPageSongFolder::Startup()
{
	DrawTitleTop("SONG FOLDER");

	// List
	pList = (CXPageItemSongWavList *)CreateObject(CallXPageItemSongWavList, "%d, %d, %d, %d, %d", ListXPos, ListYPos, ListXSize, ListItemYSize, ListItems);
	if (pList) {
		pList->HideIndex();
		pList->SetXCursor(1);	// name select
		pList->EnableSystemCursor();
		pList->SetCursor(0);
		pList->SelectCurSmpl();
	}

	ParseString("XITEM_IMAGE(188, 35, \"ICON/iconFolderCursor\")");

	// Function
	ParseString("XITEM_VALUE_ICON(0, 57, Song, SMPL_PREVIEW_ON, INVALID, \"ANIME/anime_Tab_Preview\", 2)");
	ParseString("XITEM_FUNC_BUTTON(2, \"FOLDER UP\", 0, 0, TD_SSP5)");
	DrawFuncButton(3, "SELECT");
}

//--------------------------------------------------------------
// CPageKitEntry
//--------------------------------------------------------------
Boolean CPageSongFolderEntry(UPDW* theEvent)
{
	Boolean ret = PAGE_EVENT_THRU;

	switch (theEvent->common.type) {
	case ePAGE_EVENT_ACTIVATE:
		mMyPage = new CPageSongFolder();
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
extern const SPageRsc pageRscSongFolder =
{
		CPageSongFolderEntry,                    // Event Handler
};
