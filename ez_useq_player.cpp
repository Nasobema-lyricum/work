//  ==============================================================
/**
    @file   ez_uver.cpp
    @brief  pc159 EZ HMI version display

    @author kurosawa
    @date   2018/11/15

    Copyright (C) 2018 Roland Corporation.
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

#include "ez_uver.h"

// ==============================================================
//  Static Member Variables
// ==============================================================
int CEZUtilVer::mUtil = 0;

// ==============================================================
//  Public Functions
// ==============================================================
// ------------------------  Public  ----------------------------
/**
  @brief      コンストラクタ。

  @author     kurosawa
  @date       2018/11/15
  @return     void
  */
// --------------------------------------------------------------
CEZUtilVer::CEZUtilVer()
{
}

// ------------------------  Public  ----------------------------
/**
  @brief      デストラクタ

  @author     kurosawa
  @date       2018/11/15
  @return     void
  */
// --------------------------------------------------------------
CEZUtilVer::~CEZUtilVer()
{
}

// ------------------------  Public  ----------------------------
/**
    @brief      

    @author     kurosawa
    @date       2018/11/15
    @return     void
 */
// --------------------------------------------------------------
void    CEZUtilVer::VUtilEnter(int theUtil)
{
        mUtil = theUtil;
}

// ------------------------  Public  ----------------------------
/**
  @brief      

  @author     kurosawa
  @date       2018/11/15
  @return     void
  */
// --------------------------------------------------------------
void    CEZUtilVer::VUtilExit(void) { }

// ------------------------  Public  ----------------------------
/**
  @brief      

  @author     kurosawa
  @date       2018/11/15
  @return     void
  */
// --------------------------------------------------------------
void    CEZUtilVer::VUtilPrint(void)
{
    ClearAll();
    LCDPrintf("%d.VERSION", mUtil + 1);
}

// ------------------------  Public  ----------------------------
/**
  @brief      

  @author     kurosawa
  @date       2018/11/15
  @return     void
  */
// --------------------------------------------------------------
void    CEZUtilVer::VUtilSwitch(const CPDW& thePakt)
{
}

// ==============================================================
//  Private Functions
// ==============================================================
// -----------------------  Private  ----------------------------
/**
  @brief      

  @author     kurosawa
  @date       2018/11/15
  @return     void
  */
// --------------------------------------------------------------
void    CEZUtilVer::ChangeShowType(int delta)
{
}

/* vim: set ts=8 sw=4 sts=4 expandtab: */
