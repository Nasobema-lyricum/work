//  ==============================================================
/**
    @file   StdTmplt.h
    @brief  bgxxx brief description

    @author kurosawa
    @date   2016/XX/XX

    Copyright (C) 2016 Roland Corporation.
 */
//  ==============================================================
#if !defined(__Template_StdTmplt_h__)
#define __Template_StdTmplt_h__

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
    @brief      comment for class StdTmplt

    @author     kurosawa
    @date       201X/XX/XX
 */
// --------------------------------------------------------------
template <class Type> class StdTmplt {
public:
 StdTmplt();
 virtual ~StdTmplt();

 void hogehoge();

private:
 // 不用意に使われたくないコピーコンストラクタ
 StdTmplt(const StdTmplt &);

 // 不用意に使われたくない演算子
 StdTmplt &        operator=(const StdTmplt &);
 StdTmplt *        operator&();
 const StdTmplt *  operator&() const;

 static void *   operator new(std::size_t);
 // static void     operator delete(void *);
 static void *   operator new[](std::size_t);
 // static void     operator delete[](void *);

};

// データ
// プロパティ
// コンストラクション
// オーバーライド
// 実装

}   // namespace Template

// #include "stdTmplt.cc"      // uVisionはいらない

#endif  // __Template_StdTmplt_h__

/* vim: set ts=8 sw=4 sts=4 expandtab: */
