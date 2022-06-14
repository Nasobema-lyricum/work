//  ==============================================================
/**
    @file   AssignOtherTab.cpp
    @brief  PC132 AssignページのOtherタブ

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

#include    "AssignOtherTab.h"

// ==============================================================
//  Static Member Variables
// ==============================================================
CHMIXPrm *  AssignOtherTab::lastPrm = NULL;

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
AssignOtherTab::AssignOtherTab()
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
AssignOtherTab::~AssignOtherTab()
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
void    AssignOtherTab::Enter()
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
void    AssignOtherTab::Exit()
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
void    AssignOtherTab::Draw()
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
void    AssignOtherTab::hogehoge()
{
}
#endif


/* vim: set ts=8 sw=4 sts=4 expandtab: */
