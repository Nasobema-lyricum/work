//  ==============================================================
/**
    @file   StdHdr.h
    @brief  brief description

    @author kurosawa
    @date   2021/XX/XX

    Copyright (C) 2021 Roland Corporation.
 */
//  ==============================================================
#if !defined(__StdHdr_h__)
#define __StdHdr_h__

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
    @brief      comment for class StdHdr

    @author     kurosawa
    @date       202X/XX/XX
 */
// --------------------------------------------------------------
class StdHdr {
public:
    static StdHdr & GetInstance(void);
    static StdHdr * GetInstance(void);
    static void     Destroy(void);         // インスタンスを動的に確保する場合

private:
    // プロパティはすべてstaticにして単一性を保障する
    static StdHdr * _instance;             // インスタンスを動的に確保する場合


    // 単一性を保障するため自身以外でのインスタンス化を禁止
    StdHdr(void);
    StdHdr(const StdHdr &);
    virtual ~StdHdr();

    // 不用意に使われたくない演算子
    StdHdr &        operator=(const StdHdr &);
    /*
    StdHdr *        operator&();
    const StdHdr *  operator&() const;

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

}   // namespace Foobar

#endif  // __StdHdr_h__

/* vim: set ts=4 sw=4 sts=4 expandtab: */
