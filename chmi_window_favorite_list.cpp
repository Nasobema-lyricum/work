//==============================================================
//
// chmi_window_favorite_list.cpp: Favorite List Select
//
//==============================================================
#include "pjdef.h"
#include "pjcode.h"
#include "chmi_window.h"
#include "hmi_mgr.h"
#include "hmi_mgr_def.h"
#include "xitem_list.h"
#include "chmi_song_mgr.h"
#include "hmi_prm_setup.h"
#include "chmi_favorite_mgr.h"
#include "hmi_prm_tmp.h"

#include "router_midi.h"
#include "chmi_msg_box.h"

class CXPageItemFavoriteList : public CXPageItemList {
	char	mTmpStr[64];
	char	mTmpIndex[3 + 1];
	char	mTmpKitNum[3 + 1];
	char	mTmpKitName[NUMDEFTD_SizeofKitName + 1];
	bool	mVirtualTopFolder;
	int	mNumOfFavoriteKit;
public:
	enum {
		CellIndex,
		CellSpace,
		CellKitNum,
		CellSpace2,
		CellKitName,
		CellUinstIcon,
		CellEnd
	};
	enum {
		NumOfMaxList = NUMDEFTD_NumofKit
	};
	CXPageItemFavoriteList(CXItemMgr *theHdl) : CXPageItemList(theHdl)
	{
		mNumOfFavoriteKit = 0;
		mVirtualTopFolder = false;
		mCell[CellIndex].Init(CXListCell2::TypeString, 23, 2, 0, CXListCell2::AlignRight, eFONT_ID_TD_S, OFF);
		mCell[CellSpace].Init(CXListCell2::TypeString, 5, 1, 0, CXListCell2::AlignRight, INVALID, OFF);
		mCell[CellKitNum].Init(CXListCell2::TypeString, 25, 2, 0, CXListCell2::AlignRight, eFONT_ID_TD_S, OFF);
		mCell[CellSpace2].Init(CXListCell2::TypeString, 5, 1, 0, CXListCell2::AlignRight,  INVALID, OFF);
		mCell[CellKitName].Init(CXListCell2::TypeString, 80, 2, 0, CXListCell2::AlignLeft,  eFONT_ID_TD_S, ON);
		mCell[CellUinstIcon].Init(CXListCell2::TypeImage, 15, 3, -1, CXListCell2::AlignLeft, eFONT_ID_TD_S, OFF);
	}
	int OnEventGetNumOfItems() {
		mNumOfFavoriteKit = gHMIFavoriteMgr.GetNumOfFavoriteKit(); // favoriteに登録されているKIT数を取得
		if (mNumOfFavoriteKit > NumOfMaxList) {
			return NumOfMaxList;
		} else {
			return mNumOfFavoriteKit;
		}
	}
	virtual const char *OnEventGetIndex(int theCell)
	{
		switch (theCell) {
		case CellKitName:
			return "Favorite List";
		}
		return NULL;
	}
	const SBitMap* OnEventGetItemImage(int theIdx, int theCell)
	{
		switch (theCell) {
		case CellUinstIcon:
			if (gHMIFavoriteMgr.GetFavoriteKitUserInstInfo(theIdx)) {
				return GetImage("ICON/iconUInstMiniHarf");
			} else {
				return GetImage("ICON/iconUInstMiniHarfEmpty");
			}
		}
		return NULL;
	}
	virtual const char *OnEventGetItemString(int theIdx, int theCell)
	{	
		switch (theCell) {
		case CellIndex:
			{
				sprintf(mTmpIndex, "%d", theIdx + 1);
				return mTmpIndex;
			}
		case CellKitNum:
			{
				gHMIFavoriteMgr.GetFavoriteKitNumStr(mTmpKitNum, theIdx);
				return mTmpKitNum;
			}
		case CellKitName:
			{
				gHMIFavoriteMgr.GetFavoriteKitNameStr(mTmpKitName, theIdx);
				return mTmpKitName;
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

	virtual void OnEventDrawItem(const SRect *theRect, int theY, int theIdx, Boolean isOn)
	{
		SRect	aRect = *theRect;
		int	i, x, y, x0, y0, aWidth, isCursor;
		long	fgColor, bkColor;
		const char *pString;
		const SBitMap *aBm;
		CXListCell2	*theCell;
		SRect	bRect, cRect;

		SRectZoom(&aRect, -1, -1);

		x0 = aRect.left;
		y0 = aRect.top;

		cRect = aRect;

		theCell = mCell;
		for (i = 0; i < MaxCell; i++) {
			if (theCell->GetType() == CXListCell2::TypeEnd) {
				break;
			}
			aWidth = theCell->GetWidth();
			if (aWidth < 0) {
				aWidth = aRect.right - x0;
			} else if (aWidth == 0) {
				theCell++;
				continue;
			}

			if (isOn) {
				if (GetXCursor() >= 0) {
					isCursor = (GetXCursor() == i) ? ON : OFF;
				} else {
					isCursor = ON;
				}
			} else {
				isCursor = OFF;
			}
#if 1
			fgColor = (( isCursor && ((i == CellKitName))) || (i == CellIndex)) ? DEFAULT_BK_COLOR : DEFAULT_FG_COLOR;
			bkColor = (( isCursor && ((i == CellKitName))) || (i == CellIndex)) ? DEFAULT_FG_COLOR : DEFAULT_BK_COLOR;
#else
			fgColor = (( isCursor && ((i == CellKitNum) || (i == CellSpace2) || (i == CellKitName))) || (i == CellIndex)) ? DEFAULT_BK_COLOR : DEFAULT_FG_COLOR;
			bkColor = (( isCursor && ((i == CellKitNum) || (i == CellSpace2) || (i == CellKitName))) || (i == CellIndex)) ? DEFAULT_FG_COLOR : DEFAULT_BK_COLOR;
#endif
			cRect.left   = x0;
			cRect.top    = aRect.top - 1;
			cRect.right  = x0 + aWidth - 1;
			cRect.bottom = aRect.bottom + 1;
			mLCDP->SetPenColor(fgColor, bkColor);
			mLCDP->EraseRect(&cRect);

			switch (theCell->GetType()) {
			case CXListCell2::TypeImage:
				// Image
				aBm = OnEventGetItemImage(theIdx, i);
				if (aBm) {
					//
					switch (theCell->GetAlign()) {
					case CXListCell2::AlignLeft:
						// Left
						SRectSetPos(&bRect, 0, 0);
						SRectSetSize(&bRect, aWidth, SRectGetHeight(&aRect));
						x = x0 + theCell->GetXOfst() + 1;
						y = y0 + theCell->GetYOfst();
						CBitMapXfer::XferRegion(mLCDP->GetBitMapPtr(), aBm, x, y, &bRect, FALSE);	// Not Raw Mode
						break;
					case CXListCell2::AlignMid:
						// Mid
						SRectSetPos(&bRect, 0, 0);
						SRectSetSize(&bRect, aWidth, SRectGetHeight(&aRect));
						x = x0 + ((aWidth + 1) / 2) - ((aBm->xSize + 1) / 2);
						y = y0 + theCell->GetYOfst();
	//					mLCDP->DrawSBitMap(x, y, aBm);
						CBitMapXfer::XferRegion(mLCDP->GetBitMapPtr(), aBm, x, y, &bRect, FALSE);	// Not Raw Mode
						break;
					case CXListCell2::AlignRight:
						// Left
						SRectSetPos(&bRect, 0, 0);
						SRectSetSize(&bRect, aWidth, SRectGetHeight(&aRect));
						x = x0 + aWidth - (aBm->xSize) - theCell->GetXOfst();
						y = y0 + theCell->GetYOfst();
	//					mLCDP->DrawSBitMap(x, y, aBm);
						CBitMapXfer::XferRegion(mLCDP->GetBitMapPtr(), aBm, x, y, &bRect, FALSE);	// Not Raw Mode
						break;
					}
				}
				break;
			case CXListCell2::TypeString:
				// String
				pString = OnEventGetItemString(theIdx, i);
				if (pString) {
					mLCDP->SetFont(theCell->GetFont());
//			 		if (!isCursor) mLCDP->SetFgColor(OnEventGetTextColor(theIdx, i));	// INDEX値を白反転したいのでコメントアウト

					y = y0 + theCell->GetYOfst();

					switch (theCell->GetAlign()) {
					case CXListCell2::AlignLeft:
						// Left
						x = x0 + theCell->GetXOfst() + 2;
						mLCDP->DrawString(x, y, pString);
						break;
					case CXListCell2::AlignMid:
						// Mid
						x = x0 + ((aWidth + 1) / 2);
						mLCDP->DrawStringC(x, y, pString);
						break;
					case CXListCell2::AlignRight:
						// Left
						x = x0 + aWidth - 2 - theCell->GetXOfst();
						mLCDP->DrawStringR(x, y, pString);
						break;
					}
				}
				break;
			case CXListCell2::TypeCustom:
				// Custom
	//			SRectSetPos(&bRect, x0, y0);
	//			SRectSetSize(&bRect, aWidth, SRectGetHeight(&aRect));
	//			OnEventDrawCustomCell( mLCDP, &bRect, theIdx, i );
				OnEventDrawCustomCell( mLCDP, &cRect, theIdx, i , isOn);
				break;
			default:
				i = MaxCell;	// break loop
				break;
			}
			x0 += aWidth;
			if (x0 >= aRect.right) {
				break;
			}
			if (theCell->IsSeparator()) {
				mLCDP->SetFgColor(eLCD_COLOR_BLACK);
				mLCDP->DrawVLine(x0, aRect.top - 1, aRect.bottom + 1);
			}

			theCell++;
		}
	}
};

CXPageItem *CallXPageItemFavoriteList(CXPageItem *thePtr, CXItemMgr *theHdl)
{
	if (thePtr == NULL) {
		
		return (CXPageItem *)(new CXPageItemFavoriteList(theHdl));
	}
	delete (CXPageItemFavoriteList *)thePtr;
	return NULL;
}

//-------------------------------------------------------------
//	CLASS
//-------------------------------------------------------------
class CHMIWinFavoriteList : public CHMIWindow {
	int	mX;
	int	mY;
public:
	enum {
		WindowXSize	= 200,
		WindowYSize	= 74,
		WindowXPos	= (LCD_PIXEL_XSIZE - WindowXSize),
		WindowYPos	= (LCD_PIXEL_YSIZE - WindowYSize),
	};
	enum {
		ListXPos = 20,
		ListYPos = 15,
		ListXSize = 165,
		ListItemYSize = 11,	// UinstIconが表示できる一行の高さを設定
		ListItems = 4,
	};
	CHMIWinFavoriteList() : CHMIWindow() {}
	CXPageItemFavoriteList	*pList;
	
	virtual ~CHMIWinFavoriteList() {
		mX = 0;
	}
	
	virtual void OnEventActivate()
	{
		SetWindowPos(WindowXPos, WindowYPos);
		SetWindowSize(WindowXSize, WindowYSize);

		gHMIFavoriteMgr.CreateFavoriteKitTbl();

		CreateTitle("FAVORITE LIST");
		CreateWindowFrame();
		CreatePrm();

		// Favorite Sw ON の Kit から遷移してきたら、リストの中から現在の KIT を選択した状態にする
		if (gHMIPrmKit.GetVal(CHMIPrmKit::FavoriteSw)) {	// カレントキットのFavoriteSw
			int aCurKit = gHMIPrmSetup.GetVal(CHMIPrmSetup::KitNum, INVALID);
			int aTblIndex = gHMIFavoriteMgr.SerchFavoriteTblIndex(aCurKit);
			if (aTblIndex != -1) {
				pList->SetCursor(aTblIndex);
			} else {
				pList->SetCursor(0);	// リストの先頭にカーソルを合わせる
			}
		} else {
			pList->SetCursor(0);	// リストの先頭にカーソルを合わせる
		}
	}
	void CreatePrm()
	{
		//BOTTOM
		ParseString("XITEM_STRINGC(%d, %d, \"%s\", %s)", 7 + 27, WindowYSize - 9, "SELECT", "TD_P5x5");
		ParseString("XITEM_IMAGE(%d, %d, \"ICON/KNOB_SMALL\")", 7, WindowYSize - 9);
		ParseString("XITEM_FUNC_BUTTON(%d, \"%s\", %d, %d)", 5, "OK", WindowXPos, WindowYPos);

		// List
		pList = (CXPageItemFavoriteList *)CreateObject(CallXPageItemFavoriteList, "%d, %d, %d, %d, %d", ListXPos, ListYPos, ListXSize, ListItemYSize, ListItems);
		if( pList ) {
			pList->HideIndex();
			pList->SetXCursor(1);	// name select
			pList->EnableSystemCursor();
		}
	}
	
	virtual Boolean OnEventKeyDown(int theKey)
	{
		switch (theKey) {
		case KEY_DEC:
		case KEY_UP:
			if (pList) {
				pList->HandleKeyUp();
			}
			break;
		case KEY_INC:
		case KEY_DOWN:
			if (pList) {
				pList->HandleKeyDown();
			}
			break;
		case KEY_KIT:
			Quit();
			break;
		case KEY_EXIT:
			if (HMICheckKeyPress(KEY_SHIFT)) {
				RouterAllSoundOff();
				CHMIMsgBox::DispMessage(MSG_BOX_ID_ALL_SOUND_OFF, TRUE);
				return TRUE;
			}
			Quit();
			break;
		case KEY_ENTER:
		case KEY_F5:
			// Select Kit
			DoSelect();
			break;
		default:
			return FALSE;
		}
		return TRUE;
	}

	virtual Boolean HandleEncoder(int theNum, int theVal) {
		switch (theNum) {
		case ENC_VALUE:
		case ENC_R2:
			if (pList) {
				pList->ChangeCursor(theVal);
			}
			break;
		case ENC_R1:
		default:
			break;
		}
		return TRUE;
	}
	virtual void OnEventDeactivate() {}
	virtual void OnEventRedraw() {}
	virtual void OnEventUpdate() {}
	virtual void OnEventAlarm(long theInterval) {}

	void DoSelect(void) {
		if (pList) {
			int aSelectKitNum = gHMIFavoriteMgr.GetFavoriteKitNum(pList->GetCursor());
			if (aSelectKitNum != INVALID) {
				gHMIPrmSetup.PutVal(CHMIPrmSetup::KitNum, aSelectKitNum, INVALID);
			}
			Quit(); // window を閉じる
		}
	}
};

CHMIWindow *CallHMIFavoriteList(CHMIWindow *thePtr)
{
	if (thePtr == NULL) {
		return (CHMIWindow *)(new CHMIWinFavoriteList());
	}
	delete (CHMIWinFavoriteList *)thePtr;
	return NULL;
}
