//==============================================================
//
// chmi_favorite_mgr.cpp
//
//==============================================================
#ifndef	__chmi_favorite_mgr_h__
#define	__chmi_favorite_mgr_h__

#include "pjdef.h"
#include "pjcode.h"
#include "hmi_type.h"
#include "hmi_prm_kit.h"
#include "hmi_prm_setup.h"

class CHMIFavoriteMgr {
private:
	struct SFavoriteKit {
		int kitNum;
		char kitName[NUMDEFTD_SizeofKitName + 1];
		bool userInstIsIncluded;
	};
	
	SFavoriteKit mFavoriteKitTbl[NUMDEFTD_NumofKit];
	int mNumOfFavoriteKit;
public:
	CHMIFavoriteMgr(void) {
		Init();
	}
	void	Init(void);
	void	CreateFavoriteKitTbl(void);
	int 	GetNumOfFavoriteKit(void);
	int	GetFavoriteKitNum(int theIndex);
	void	GetFavoriteKitNumStr(char* theStr, int theIndex);
	void	GetFavoriteKitNameStr(char* theStr, int theIndex);
	bool	GetFavoriteKitUserInstInfo(int theIndex);
	int	SerchFavoriteTblIndex(int theKitNum);
};

extern CHMIFavoriteMgr gHMIFavoriteMgr;

#endif	/* !(__chmi_favorite_mgr_h__) */
