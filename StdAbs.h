//  ==============================================================
/**
    @file   StdAbs.h
    @brief  bgxxx description

    @author kurosawa
    @date   2016/XX/XX

    Copyright (C) 2016 Roland Corporation.
 */
//  ==============================================================
#if !defined(__StdAbs_h__)
#define __StdAbs_h__

// ==============================================================
//  includes
// ==============================================================
#include <cstddef>
// #include "hogehoge.h"

namespace Foobar {
// ==============================================================
//  Class declaration
// ==============================================================
// class HogeHoge;

// ==============================================================
//  Classes
// ==============================================================
// --------------------------------------------------------------
/**
    @brief      comment for class StdAbs

    @author     kurosawa
    @date       200X/XX/XX
 */
// --------------------------------------------------------------
class StdAbs {
public:
    virtual ~StdAbs() {}

    virtual void    foo() = 0;

protected:
    // 抽象クラスなのでインスタンス化禁止
    StdAbs() {}
    StdAbs(const StdAbs &) {}

private:
    // 不用意に使われたくない演算子
    StdAbs &        operator=(const StdAbs &);

    StdAbs *        operator&();
    const StdAbs *  operator&() const;

    static void *   operator new(std::size_t);
    static void     operator delete(void *);
    static void *   operator new[](std::size_t);
    static void     operator delete[](void *);

};

//  にほんごだぜ

}   // namespace Foobar

#endif  // __StdAbs_h__
/* vim: set ts=8 sw=4 sts=4 expandtab: */
