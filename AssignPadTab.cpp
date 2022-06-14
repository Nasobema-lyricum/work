//  ==============================================================
/**
    @file   AssignPadTab.cpp
    @brief  PC132 AssignページのPadタブ

    @author kurosawa
    @date   2017/07/12

    Copyright (C) 2017 Roland Corporation.
 */
//  ==============================================================
#if defined(__DBG_KRSW)
#pragma O0
#define DBG_PRINTF
#else
#undef  DBG_PRINTF
#endif

#if defined(DBG_PRINTF)
#include <stdio.h>
#endif

#include    "AssignPadTab.h"

// ==============================================================
//  Static Member Variables
// ==============================================================
CHMIXPrm *  AssignPadTab::lastPrm = NULL;

// ==============================================================
//  Public Functions
// ==============================================================
// ------------------------  Public  ----------------------------
/**
  @brief      コンストラクタ。

  @author     kurosawa
  @date       2017/07/12
  @return     void
  */
// --------------------------------------------------------------
AssignPadTab::AssignPadTab()
{
}

// ------------------------  Public  ----------------------------
/**
  @brief      デストラクタ

  @author     kurosawa
  @date       2017/07/12
  @return     void
  */
// --------------------------------------------------------------
AssignPadTab::~AssignPadTab()
{
}

// ------------------------  Public  ----------------------------
/**
    @brief      このタブに移動してきたときにする処理

    @author     kurosawa
    @date       2017/07/12
    @return     void
 */
// --------------------------------------------------------------
void    AssignPadTab::Enter()
{
}

// ------------------------  Public  ----------------------------
/**
  @brief      他のタブに移動する時にする処理

  @author     kurosawa
  @date       2017/07/12
  @return     void
  */
// --------------------------------------------------------------
void    AssignPadTab::Exit()
{
}

// ------------------------  Public  ----------------------------
/**
  @brief      描画する

  @author     kurosawa
  @date       2017/07/12
  @return     void
  */
// --------------------------------------------------------------
void    AssignPadTab::Draw()
{
        DrawLine( 26, 12,  67, 12, C_WHITE, 0, 0 );
        DrawLine( 71, 12, 123, 12, C_WHITE, 0, 0 );
        DrawLine( 69, 14,  69, 54, C_WHITE, 0, 1 );

        ParseString("XITEM_STRING( 31, 7, \"MASTER\", TD_SSP5 )");
        ParseString("XITEM_STRING( 85, 7, \"DIRECT\", TD_SSP5 )");


        ParseString("XITEM_STRING(  2, 14, \"PAD\", TD_SSP5 )");

        ParseString("XITEM_IMAGE(  19, 14, \"ICON/ICON_1_SMALL\" )");
        ParseString("XITEM_IMAGE(  19, 21, \"ICON/ICON_2_SMALL\" )");
        ParseString("XITEM_IMAGE(  19, 28, \"ICON/ICON_3_SMALL\" )");
        ParseString("XITEM_IMAGE(  19, 35, \"ICON/ICON_4_SMALL\" )");
        ParseString("XITEM_IMAGE(  19, 42, \"ICON/ICON_5_SMALL\" )");
        ParseString("XITEM_IMAGE(  19, 49, \"ICON/ICON_6_SMALL\" )");

        if (gHMIPrmSetup.GetVal(CHMIPrmSetup::MasterDirectSw) == 0) {
            ParseString("XITEM_VALUE_BOX_C( 44, 14, Setup, PadMasterAssign, 0, TD_SSP5, 29 )");
            ParseString("XITEM_VALUE_BOX_C( 44, 21, Setup, PadMasterAssign, 1, TD_SSP5, 29 )");
            ParseString("XITEM_VALUE_BOX_C( 44, 28, Setup, PadMasterAssign, 2, TD_SSP5, 29 )");
            ParseString("XITEM_VALUE_BOX_C( 44, 35, Setup, PadMasterAssign, 3, TD_SSP5, 29 )");
            ParseString("XITEM_VALUE_BOX_C( 44, 42, Setup, PadMasterAssign, 4, TD_SSP5, 29 )");
            ParseString("XITEM_VALUE_BOX_C( 44, 49, Setup, PadMasterAssign, 5, TD_SSP5, 29 )");
        }
        else {
            ParseString("XITEM_IMAGE(  29, 26, \"ICON/frameMasterDirect.bmp\" )");
            ParseString("XITEM_STRING( 31, 28, \"MASTER\",   TD_SSP5 )");
            ParseString("XITEM_STRING( 33, 35, \"DIRECT\",   TD_SSP5 )");
        }

        ParseString("XITEM_VALUE_BOX_C( 95, 14, Setup, PadDirectAssign, 0, TD_SSP5, 51 )");
        ParseString("XITEM_VALUE_BOX_C( 95, 21, Setup, PadDirectAssign, 1, TD_SSP5, 51 )");
        ParseString("XITEM_VALUE_BOX_C( 95, 28, Setup, PadDirectAssign, 2, TD_SSP5, 51 )");
        ParseString("XITEM_VALUE_BOX_C( 95, 35, Setup, PadDirectAssign, 3, TD_SSP5, 51 )");
        ParseString("XITEM_VALUE_BOX_C( 95, 42, Setup, PadDirectAssign, 4, TD_SSP5, 51 )");
        ParseString("XITEM_VALUE_BOX_C( 95, 49, Setup, PadDirectAssign, 5, TD_SSP5, 51 )");


}

// ==============================================================
//  Private Functions
// ==============================================================
#if 0
// -----------------------  Private  ----------------------------
/**
    @brief      ほげほげ

    @author     kurosawa
    @date       201X/XX/XX
    @return     void
 */
// --------------------------------------------------------------
void    AssignPadTab::hogehoge()
{
}
#endif


/* vim: set ts=8 sw=4 sts=4 expandtab: */
