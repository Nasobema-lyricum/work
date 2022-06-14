//  ==============================================================
/**
    @file   AssignSetupTab.cpp
    @brief  PC132 AssignページのSetupタブ

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

#include    "AssignSetupTab.h"

// ==============================================================
//  Static Member Variables
// ==============================================================
CHMIXPrm *  AssignSetupTab::lastPrm = NULL;

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
AssignSetupTab::AssignSetupTab()
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
AssignSetupTab::~AssignSetupTab()
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
void    AssignSetupTab::Enter()
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
void    AssignSetupTab::Exit()
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
void    AssignSetupTab::Draw()
{
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
void    AssignSetupTab::hogehoge()
{
}
#endif


/* vim: set ts=8 sw=4 sts=4 expandtab: */
