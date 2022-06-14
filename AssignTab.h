//  ==============================================================
/**
    @file   AssignTab.h
    @brief  PC132 assignページの抽象tabクラス

    @author kurosawa
    @date   2017/07/12

    Copyright (C) 2017 Roland Corporation.
 */
//  ==============================================================
#if !defined(__AssignTab_h__)
#define __AssignTab_h__

// ==============================================================
//  includes
// ==============================================================
#include <cstddef>
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
    @brief      各タブの中身のクラスのインターフェイス

    @author     kurosawa
    @date       2017/07/12
 */
// --------------------------------------------------------------
class AssignTab {
public:
    virtual ~AssignTab() {}

    virtual void    Enter() =0;
    virtual void    Exit()  =0;
    virtual void    Draw()  =0;

    virtual void        SetLastPrm(CHMIXPrm * p) {}
    virtual CHMIXPrm *  GetLastPrm()             { return NULL; }

protected:
    // 抽象クラスなのでインスタンス化禁止
    AssignTab() {}
    AssignTab(const AssignTab &) {}

private:
    // 不用意に使われたくない演算子
    AssignTab &        operator=(const AssignTab &);

    /*
    AssignTab *        operator&();
    const AssignTab *  operator&() const;
    static void *   operator new(std::size_t);
    static void     operator delete(void *);
    static void *   operator new[](std::size_t);
    static void     operator delete[](void *);
*/
};
#endif  // __AssignTab_h__
/* vim: set ts=8 sw=4 sts=4 expandtab: */
