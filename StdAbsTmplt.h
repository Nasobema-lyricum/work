//  ==============================================================
/**
    @file   StdAbsTmplt.h
    @brief  bgxxx brief description

    @author kurosawa
    @date   2016/XX/XX

    Copyright (C) 2016 Roland Corporation.
 */
//  ==============================================================
#if !defined(__Template_StdAbsTmplt_h__)
#define __Template_StdAbsTmplt_h__

// ==============================================================
//  includes
// ==============================================================
#include <cstddef>
// #include "hogehoge.h"

namespace Template {
// ==============================================================
//  Class declaration
// ==============================================================
// class HogeHoge;

// ==============================================================
//  Classes
// ==============================================================
// --------------------------------------------------------------
/**
    @brief      comment for class StdAbsTmplt

    @author     kurosawa
    @date       201X/XX/XX
 */
// --------------------------------------------------------------
template <class Data> class StdAbsTmplt {
public:
    virtual ~StdAbsTmplt() {}

    virtual void    Foo() =0;

protected:
    // 抽象クラスなのでインスタンス化禁止
    StdAbsTmplt() {}
    StdAbsTmplt(const StdAbsTmplt &) {}

private:
    // 不用意に使われたくない演算子
    StdAbsTmplt &         operator=(const StdAbsTmplt &);
    StdAbsTmplt *         operator&();
    const StdAbsTmplt *   operator&() const;

    static void *   operator new(std::size_t);
    static void     operator delete(void *);
    static void *   operator new[](std::size_t);
    static void     operator delete[](void *);
};

// データ
// プロパティ
// コンストラクション
// オーバーライド
// 実装

}   // namespace Template

#endif  // __Template_StdAbsTmplt_h__

/* vim: set ts=8 sw=4 sts=4 expandtab: */
