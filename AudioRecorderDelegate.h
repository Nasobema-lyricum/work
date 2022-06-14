//  ==============================================================
/**
    @file   AudioPlayerDelegate.h
    @brief  Test Program for LUFT win library

    @author kurosawa
    @date   2013/10/06

    Copyright (C) 2013 BOSS Corporation.
 */
//  ==============================================================
#if !defined(__AudioPlayerDelegate_h__)
#define __AudioPlayerDelegate_h__

// ==============================================================
//  includes
// ==============================================================
#include "RWCAudioPlayerDelegate.h"

// ==============================================================
//  Class declaration
// ==============================================================
// class HogeHoge;

// ==============================================================
//  Classes
// ==============================================================
// --------------------------------------------------------------
/**
    @brief      

    @author     kurosawa
    @date       2013/10/06
 */
// --------------------------------------------------------------
class AudioPlayerDelegate : public RWCAudioPlayerDelegate {
public:
    static AudioPlayerDelegate * getInstance(void);

    virtual void audioPlayerDidFinishPlaySound();
    virtual void audioPlayerDidEndSong();

private:
    // 単一性を保障するため自身以外でのインスタンス化を禁止
    AudioPlayerDelegate(void);
    AudioPlayerDelegate(const AudioPlayerDelegate &);
    virtual ~AudioPlayerDelegate();
};

// データ
// プロパティ
// コンストラクション
// オーバーライド
// 実装

#endif  // __AudioPlayerDelegate_h__

/* vim: set ts=8 sw=4 sts=4 expandtab: */
