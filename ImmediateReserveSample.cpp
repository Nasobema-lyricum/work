//  ==============================================================
/**
    @file   ImmediateReserveSample.cpp
    @brief  bg661  即値で長い時間の予約を入れたいときに使います

    @author kurosawa
    @date   2016/05/11

    Copyright (C) 2016 Roland Corporation.
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

#include    "ImmediateReserveSample.h"
#include    "looper_config.h"

namespace LOOPER {
// ==============================================================
//  Static Member Variables
// ==============================================================

// ==============================================================
//  Public Functions
// ==============================================================
// ------------------------  Public  ----------------------------
/**
  @brief      コンストラクタ。

  @author     kurosawa
  @date       2016/05/11
  @return     void
  */
// --------------------------------------------------------------
ImmediateReserveSample::ImmediateReserveSample()
{
    this->Initialize();
}

// ------------------------  Public  ----------------------------
/**
  @brief      デストラクタ

  @author     kurosawa
  @date       2016/05/11
  @return     void
  */
// --------------------------------------------------------------
ImmediateReserveSample::~ImmediateReserveSample()
{
}

// ------------------------  Public  ----------------------------
/**
    @brief      初期化

    @author     kurosawa
    @date       2016/05/11
    @return     void
 */
// --------------------------------------------------------------
void    ImmediateReserveSample::Initialize()
{
}

// ------------------------  Public  ----------------------------
/**
  @brief      

  @author     kurosawa
  @date       2016/05/11
  @return     void
  */
// --------------------------------------------------------------
uint32_t    ImmediateReserveSample::GetRemainSample()
{
    return CFG::SAMPLE_FREE_RUN_EXE_REC_SKIP_TO_DUB;
}

// ==============================================================
//  Private Functions
// ==============================================================
#if 0
// -----------------------  Private  ----------------------------
/**
    @brief      ほげほげ

    @author     kurosawa
    @date       2016/05/11
    @return     void
 */
// --------------------------------------------------------------
void    ImmediateReserveSample::hogehoge()
{
}
#endif

}   // namespace LOOPER

/* vim: set ts=8 sw=4 sts=4 expandtab: */
