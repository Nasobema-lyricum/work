//  ==============================================================
/**
    @file   ez_uver.h
    @brief  EZ HMI version display

    @author kurosawa
    @date   2018/11/15

    Copyright (C) 2018 Roland Corporation.
 */
//  ==============================================================
#if !defined(__ez_uver_h__)
#define __ez_uver_h__

// ==============================================================
//  includes
// ==============================================================
#include <cstddef>
#include "ez_hmi.h"

// ==============================================================
//  Class declaration
// ==============================================================
// class HogeHoge;

// ==============================================================
//  Classes
// ==============================================================
// --------------------------------------------------------------
/**
    @brief      EZ HMI バージョン表示

    @author     kurosawa
    @date       2018/11/15
 */
// --------------------------------------------------------------
class CEZUtilVer : public CEZUtil {
public:
    // コンストラクション
    CEZUtilVer(void);
    virtual ~CEZUtilVer();

    // オーバーライド
    virtual void VUtilEnter(int theUtil);
    virtual void VUtilExit(void);
    virtual void VUtilPrint(void);
    virtual void VUtilSwitch(const CPDW& thePakt);

    // 実装

private:
    // プロパティはすべてstaticにして単一性を保障する
    static int  mUtil;

    // 不用意に使われたくないコンストラクタ
    CEZUtilVer(const CEZUtilVer &);

    // 不用意に使われたくない演算子
    CEZUtilVer &        operator=(const CEZUtilVer &);
    /*
    CEZUtilVer *        operator&();
    const CEZUtilVer *  operator&() const;

    static void *   operator new(std::size_t);
    static void     operator delete(void *);
    static void *   operator new[](std::size_t);
    static void     operator delete[](void *);
    */

    // 実装
    void ChangeShowType(int delta);

};

// データ
// プロパティ

#endif  // __ez_uver_h__

/* vim: set ts=8 sw=4 sts=4 expandtab: */
