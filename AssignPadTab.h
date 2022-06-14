//  ==============================================================
/**
    @file   AssignPadTab.h
    @brief  PC132 AssignページのPadタブ

    @author kurosawa
    @date   2017/07/12

    Copyright (C) 2017 Roland Corporation.
 */
//  ==============================================================
#if !defined(__AssignPadTab_h__)
#define __AssignPadTab_h__

// ==============================================================
//  includes
// ==============================================================
#include <cstddef>
#include "AssignTab.h"
#include "xpage_base_td.h"

// ==============================================================
//  Class declaration
// ==============================================================
// class HogeHoge;

// ==============================================================
//  Classes
// ==============================================================
// --------------------------------------------------------------
/**
    @brief      PC132 AssignページのPadタブ

    @author     kurosawa
    @date       2017/07/12
 */
// --------------------------------------------------------------
class AssignPadTab : public AssignTab, CXPageBaseTD {
public:
    AssignPadTab(void);
    AssignPadTab(const AssignPadTab &);
    virtual ~AssignPadTab();

    void    Enter();
    void    Exit();
    void    Draw();

    void        SetLastPrm(CHMIXPrm * p) { lastPrm = p;    }
    CHMIXPrm *  GetLastPrm()             { return lastPrm; }

private:
    // プロパティはすべてstaticにして単一性を保障する
    static CHMIXPrm *   lastPrm;

    // 不用意に使われたくない演算子
    AssignPadTab &        operator=(const AssignPadTab &);
    /*
    AssignPadTab *        operator&();
    const AssignPadTab *  operator&() const;

    static void *   operator new(std::size_t);
    static void     operator delete(void *);
    static void *   operator new[](std::size_t);
    static void     operator delete[](void *);
    */

};

// データ
// プロパティ
// コンストラクション
// オーバーライド
// 実装
#endif  // __AssignPadTab_h__

/* vim: set ts=8 sw=4 sts=4 expandtab: */
