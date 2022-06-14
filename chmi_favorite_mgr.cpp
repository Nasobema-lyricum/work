//==============================================================
//
// chmi_favorite_mgr.cpp
//
//==============================================================
#if defined(TARGET_SIMULATION)
#include "kernel.h"
#else
#include <tk/tkernel.h>
#endif // TARGET_SIMULATION
#include "chmi_favorite_mgr.h"
#include "chmi_disp.h"
#include <string.h>

void	CHMIFavoriteMgr::Init(void)
{
	
}

void	CHMIFavoriteMgr::CreateFavoriteKitTbl(void)
{
	int aFavoriteTblIndex(0);
	char tmpStr[NUMDEFTD_SizeofKitName + 1];
	bool aUserInstIsIncluded(0);
	
	for (int aKitNum = 0; aKitNum < NUMDEFTD_NumofKit; aKitNum++) {
		if (gHMIPrmKit.GetVal(CHMIPrmKit::FavoriteSw, aKitNum)) {
			// KitNum
			mFavoriteKitTbl[aFavoriteTblIndex].kitNum = aKitNum;
			// KitName
			gHMIPrmSetup.GetValName(tmpStr, CHMIPrmSetup::KitNum, aKitNum, CHMIPrmSetup::PrmKitName);
			strcpy(mFavoriteKitTbl[aFavoriteTblIndex].kitName, tmpStr);
			// UserInst
			aUserInstIsIncluded = gHMIPrmKit.GetVal(CHMIPrmKit::UserInstIncluded, aKitNum);
			mFavoriteKitTbl[aFavoriteTblIndex].userInstIsIncluded = aUserInstIsIncluded;
			// IndexCount
			aFavoriteTblIndex++;
		}
	}
	mNumOfFavoriteKit = aFavoriteTblIndex;
}

int	CHMIFavoriteMgr::SerchFavoriteTblIndex(int theKitNum)
{
	for (int i = 0; i < mNumOfFavoriteKit; i++) {
		if (mFavoriteKitTbl[i].kitNum == theKitNum) {
			return i;
		}
	}
	return -1;
}

int	CHMIFavoriteMgr::GetNumOfFavoriteKit(void)
{
	return mNumOfFavoriteKit;
}

int	CHMIFavoriteMgr::GetFavoriteKitNum(int theIndex)
{
	if (mNumOfFavoriteKit != 0) {
		return mFavoriteKitTbl[theIndex].kitNum;
	} else {
		return INVALID;
	}
}

void	CHMIFavoriteMgr::GetFavoriteKitNumStr(char* theStr, int theIndex)
{
	sprintf(theStr, "%d", mFavoriteKitTbl[theIndex].kitNum + 1);
}

void	CHMIFavoriteMgr::GetFavoriteKitNameStr(char* theStr, int theIndex)
{
	strcpy(theStr, mFavoriteKitTbl[theIndex].kitName);
}

bool	CHMIFavoriteMgr::GetFavoriteKitUserInstInfo(int theIndex)
{
	return mFavoriteKitTbl[theIndex].userInstIsIncluded;
}

CHMIFavoriteMgr gHMIFavoriteMgr;
