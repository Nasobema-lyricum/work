//  ==============================================================
/**
    @file   AssignOtherTab.h
    @brief  PC132 AssignページのOtherタブ

    @author kurosawa
    @date   2017/07/12

    Copyright (C) 2017 Roland Corporation.
 */
//  ==============================================================
#if !defined(__AssignOtherTab_h__)
#define __AssignOtherTab_h__

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
    @brief      PC132 AssignページのOtherタブ

    @author     kurosawa
    @date       2017/07/12
 */
// --------------------------------------------------------------
class AssignOtherTab : public AssignTab, CXPageBaseTD {
public:
    AssignOtherTab(void);
    AssignOtherTab(const AssignOtherTab &);
    virtual ~AssignOtherTab();

    void    Enter();
    void    Exit();
    void    Draw();

    void        SetLastPrm(CHMIXPrm * p) { lastPrm = p;    }
    CHMIXPrm *  GetLastPrm()             { return lastPrm; }

private:
    // プロパティはすべてstaticにして単一性を保障する
    static CHMIXPrm *   lastPrm;


    // 不用意に使われたくない演算子
    AssignOtherTab &        operator=(const AssignOtherTab &);
    /*
    AssignOtherTab *        operator&();
    const AssignOtherTab *  operator&() const;

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
#endif  // __AssignOtherTab_h__

/* vim: set ts=8 sw=4 sts=4 expandtab: */
