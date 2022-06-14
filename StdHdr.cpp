//  ==============================================================
/**
    @file   StdHdr.cpp
    @brief  brief description

    @author kurosawa
    @date   2021/XX/XX

    Copyright (C) 2021 Roland Corporation.
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

#include    "StdHdr.h"

namespace Foobar {
// ==============================================================
//  Static Member Variables
// ==============================================================
StdHdr *    StdHdr::_instance = 0;      // インスタンスを動的に確保する場合

// ==============================================================
//  Public Functions
// ==============================================================
// ------------------------  Public  ----------------------------
/**
    @brief      インスタンスを重複しないように静的に確保

    @author     kurosawa
    @date       202X/XX/XX
    @return     インスタンス(の参照）
 */
// --------------------------------------------------------------
StdHdr &    StdHdr::GetInstance()
{
    static StdHdr   _instance;
    return _instance;
}

// ------------------------  Public  ----------------------------
/**
    @brief      インスタンスを重複しないように静的に確保

    @author     kurosawa
    @date       202X/XX/XX
    @return     インスタンスを指すポインタ
 */
// --------------------------------------------------------------
StdHdr *    StdHdr::GetInstance()
{
    static StdHdr   _instance;
    return &_instance;
}

// ------------------------  Public  ----------------------------
/**
    @brief      インスタンスを重複しないように動的に確保

    @author     kurosawa
    @date       202X/XX/XX
    @return     インスタンス(の参照）
 */
// --------------------------------------------------------------
StdHdr &    StdHdr::GetInstance()
{
    if (_instance == 0) {
        _instance = new StdHdr;
    }
    return *_instance;
}

// ------------------------  Public  ----------------------------
/**
    @brief      インスタンスを重複しないように動的に確保

    @author     kurosawa
    @date       202X/XX/XX
    @return     インスタンスを指すポインタ
 */
// --------------------------------------------------------------
StdHdr *    StdHdr::GetInstance()
{
    if (_instance == 0) {
        _instance = new StdHdr;
    }
    return _instance;
}

// ------------------------  Public  ----------------------------
/**
    @brief      動的に確保したインスタンスを破棄

    @author     kurosawa
    @date       202X/XX/XX
    @return     void
 */
// --------------------------------------------------------------
void    StdHdr::Destroy()
{
    if (_instance != 0) {
        delete _instance;
        _instance = 0;
    }
}

#if 0
// ------------------------  Public  ----------------------------
/**
    @brief      ほげほげ

    @author     kurosawa
    @date       202X/XX/XX
    @return     void
 */
// --------------------------------------------------------------
void    StdHdr::hogehoge()
{
}
#endif

// ==============================================================
//  Private Functions
// ==============================================================
// -----------------------  Private  ----------------------------
/**
    @brief      コンストラクタ。

    @author     kurosawa
    @date       202X/XX/XX
    @return     void
 */
// --------------------------------------------------------------
StdHdr::StdHdr()
{
}

// -----------------------  Private  ----------------------------
/**
    @brief      デストラクタ

    @author     kurosawa
    @date       202X/XX/XX
    @return     void
 */
// --------------------------------------------------------------
StdHdr::~StdHdr()
{
}

#if 0
// -----------------------  Private  ----------------------------
/**
    @brief      ほげほげ

    @author     kurosawa
    @date       202X/XX/XX
    @return     void
 */
// --------------------------------------------------------------
void    StdHdr::hogehoge()
{
}
#endif

}   // namespace Foobar

/* vim: set ts=4 sw=4 sts=4 expandtab: */
