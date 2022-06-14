//  ==============================================================
/**
    @file   AudioPlayerDelegate.cpp
    @brief  Test Program for LUFT win library

    @author kurosawa
    @date   2013/10/06

    Copyright (C) 2013 BOSS Corporation.
 */
//  ==============================================================
#include    <windows.h>
#include    "AudioPlayerDelegate.h"

// ==============================================================
//  Static Member Variables
// ==============================================================

// ==============================================================
//  Public Functions
// ==============================================================
// ------------------------  Public  ----------------------------
/**
    @brief      インスタンスを重複しないように静的に確保

    @author     kurosawa
    @date       2013/10/06
    @return     インスタンス(の参照）
 */
// --------------------------------------------------------------
AudioPlayerDelegate *    AudioPlayerDelegate::getInstance()
{
    static AudioPlayerDelegate   _instance;
    return &_instance;
}

// ------------------------  Public  ----------------------------
/**
    @brief

    @author     kurosawa
    @date       2013/10/06
    @return     void
 */
// --------------------------------------------------------------
void    AudioPlayerDelegate::audioPlayerDidFinishPlaySound()
{
    OutputDebugString("delegate audioPlayerDidFinishPlaySound()");
}

// ------------------------  Public  ----------------------------
/**
  @brief

  @author     kurosawa
  @date       2013/10/06
  @return     void
  */
// --------------------------------------------------------------
void    AudioPlayerDelegate::audioPlayerDidEndSong()
{
    OutputDebugString("delegate audioPlayerDidEndSong()");
}

// ==============================================================
//  Private Functions
// ==============================================================
// -----------------------  Private  ----------------------------
/**
    @brief      コンストラクタ。

    @author     kurosawa
    @date       201X/XX/XX
    @return     void
 */
// --------------------------------------------------------------
AudioPlayerDelegate::AudioPlayerDelegate()
{
}

// -----------------------  Private  ----------------------------
/**
    @brief      デストラクタ

    @author     kurosawa
    @date       201X/XX/XX
    @return     void
 */
// --------------------------------------------------------------
AudioPlayerDelegate::~AudioPlayerDelegate()
{
}

#if 0
// -----------------------  Private  ----------------------------
/**
    @brief      ほげほげ

    @author     kurosawa
    @date       201X/XX/XX
    @return     void
 */
// --------------------------------------------------------------
void    AudioPlayerDelegate::hogehoge()
{
}
#endif

/* vim: set ts=8 sw=4 sts=4 expandtab: */
