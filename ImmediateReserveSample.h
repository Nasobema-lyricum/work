//  ==============================================================
/**
    @file   ImmediateReserveSample.h
    @brief  bg661  即値で長い時間の予約を入れたいときに使います

    @author kurosawa
    @date   2016/05/11

    Copyright (C) 2016 Roland Corporation.
 */
//  ==============================================================
#if !defined(__ImmediateReserveSample_h__)
#define __ImmediateReserveSample_h__

// ==============================================================
//  includes
// ==============================================================
#include <cstddef>
#include "looper_type.h"

namespace LOOPER {
// ==============================================================
//  Class declaration
// ==============================================================
// class HogeHoge;

// ==============================================================
//  Classes
// ==============================================================
// --------------------------------------------------------------
/**
    @brief      comment for class ImmediateReserveSample

    @author     kurosawa
    @date       2016/05/11
 */
// --------------------------------------------------------------
class ImmediateReserveSample {
public:
    ImmediateReserveSample(void);
    ImmediateReserveSample(const ImmediateReserveSample &);
    virtual ~ImmediateReserveSample();

    void        Initialize();
    uint32_t    GetRemainSample();

private:
    // 不用意に使われたくない演算子
    ImmediateReserveSample &        operator=(const ImmediateReserveSample &);
    /*
    ImmediateReserveSample *        operator&();
    const ImmediateReserveSample *  operator&() const;

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

}   // namespace LOOPER

#endif  // __StdHdr_h__

/* vim: set ts=8 sw=4 sts=4 expandtab: */
