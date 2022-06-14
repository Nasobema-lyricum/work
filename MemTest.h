//  ==============================================================
/**
 *	@file	MemTest.h
 *	@brief	enxxx brief description
 *	
 *			detailed description
 *	
 *			$Author: kurosawa $
 *			$Date: 2006/02/17 00:28:30 $
 *			$Revision: 1.1.1.1 $
 */
//  ==============================================================
#ifndef	_MemTest_h_
#define	_MemTest_h_

// ==============================================================
//  includes
// ==============================================================
// #include	<hogehoge>
// #include	"hogehoge.h"

// ==============================================================
//  Class declaration
// ==============================================================
// class HogeHoge

// ==============================================================
//  Classes
// ==============================================================
/**
 *	メモリチェックをするクラスのテンプレート
 *
 *	@attention	0番地はエラーしても正常終了と区別がつかないのでそのつもりで
 *
 */
template <class Datum>
class MemTest {
public:
	Datum	DataBus( volatile Datum * const adrs                                 );
	Datum *	AdrsBus( volatile Datum * const adrs, unsigned long size             );
	Datum *	Device(  volatile Datum * const adrs, unsigned long size             );
	void	Fill(    volatile Datum * const adrs, unsigned long size, Datum data );
	Datum *	ChkFill( volatile Datum * const adrs, unsigned long size, Datum data );
};


// ------------------------  Public  ----------------------------
// MemTest<Datum>::DataBus
// --------------------------------------------------------------
/**
 *	data busをテスト<br>
 *	walking 1 test でデータバスをテストします。<br>
 *
 *	@author	 	kurosawa
 *	@date   	200X/XX/XX
 *	@param		adrs	テストしたいバス上にあるRAMっぽいもののアドレス
 *	@retval		0		正常終了
 *	@retval		not 0	最初にエラーしたパターン
 *	@pre		SPがセットされていないとまずいかも
 *	@post		RAMの中身はこわれるよ
 *	@note 		バス幅に合わせた型でインスタンス化するであります
 */
// --------------------------------------------------------------
template<class Datum>
Datum	MemTest<Datum>::DataBus(volatile Datum * const adrs)
{
	// 2^n を...
	for (Datum ptn = 1; ptn != 0; ptn <<= 1) {

		// どこかRAMっぽいモノが在ると思われるところに書いて...
		*adrs = ptn;

		// 違うものが読めたらNG
		if (*adrs != ptn) return ptn;

	}
	return static_cast<Datum>(0);
}

// ------------------------  Public  ----------------------------
// MemTest<Datum>::AdrsBus
// --------------------------------------------------------------
/**
 *	Address busをテスト<br>
 *	walking 1 test でアドレスバスをテストします。<br>
 *
 *	@author	 	kurosawa
 *	@date   	200X/XX/XX
 *	@param		adrs	テストしたいバス上にあるRAMっぽいもののアドレス
 *	@param		size	テストしたいバス上にあるRAMっぽいもののサイズ(byte)
 *	@retval		0		正常終了
 *	@retval		not 0	最初にエラーしたアドレス
 *	@pre		SPがセットされていないとまずいかも
 *	@post		RAMの中身はこわれえるよ
 *	@note 		バス幅に合わせた型でインスタンス化するであります
 */
// --------------------------------------------------------------
template<class Datum>
Datum *	MemTest<Datum>::AdrsBus(volatile Datum * const adrs, unsigned long size)
{
	const Datum	ptn	   = 0xAA;		// なんでもいいんです
	const Datum	invptn = 0x55;		// なんでもいいんです

	unsigned long	idx;
	unsigned long	tstidx;

	// あらかじめ 2^n になにか書いておく
	for (idx = 1; idx < size / sizeof(Datum) ; idx <<= 1) {
		adrs[idx] = ptn;
	}

	// 0 に 2^n と別なものを書いて...
	adrs[0] = invptn;

	// 2^n を調べて...
	for (idx = 1; idx < size / sizeof(Datum); idx <<= 1) {

		// 書き換わっていれば、どこかHI/LO固定なビットがある。
		if (adrs[idx] != ptn) {
			return const_cast<Datum *>(&adrs[idx]);
		}
	}

	// 0 に 2^n と同じものを書いて... （いらない？）
	// adrs[0] = ptn;

	// どこかの 2^n に
	for (tstidx = 1; tstidx < size / sizeof(Datum); tstidx <<= 1) {

		// 違うものを書いて...
		adrs[tstidx] = invptn;

		// 他の 2^n を調べて...
		for (idx = tstidx << 1; idx < size / sizeof(Datum); idx <<= 1) {
			// 書き換わっていれば、どこかショートしてHIになっている。
			if (adrs[idx] != ptn) {
				return const_cast<Datum *>(&adrs[tstidx]);
			}
		}
	}

	return static_cast<Datum>(0);
}

// ------------------------  Public  ----------------------------
// MemTest<Datum>::Device
// --------------------------------------------------------------
/**
 *	RAMをテスト<br>
 *	すべてのビットについて 0/1 を保持できるかテストする<br>
 *
 *	@author	 	kurosawa
 *	@date   	200X/XX/XX
 *	@param		adrs	テストしたいRAMっぽいもののアドレス
 *	@param		size	テストしたいRAMっぽいもののサイズ(byte)
 *	@retval		0		正常終了
 *	@retval		not 0	最初にエラーしたアドレス
 *	@pre		SPがセットされていないとまずいかも
 *	@post		RAMの中身は全部消えるよ
 *	@note 		バス幅に合わせた型でインスタンス化するであります
 */
// --------------------------------------------------------------
template<class Datum>
Datum *	MemTest<Datum>::Device(volatile Datum * const adrs, unsigned long size)
{
	unsigned long	idx;
	Datum			ptn;

	// すべてのビットに...
	for (ptn = 0, idx = 0; idx < size / sizeof(Datum); ptn++, idx++) {

		// 何でもいいから書いておく
		adrs[idx] = ptn;
	}

	// どのビットも...
	for (ptn = 0, idx = 0; idx < size / sizeof(Datum); ptn++, idx++) {

		// 書いた値が保持できてなければNG
		if (adrs[idx] != ptn) {
			return const_cast<Datum *>(&adrs[idx]);
		}

		// 反転した値を書いておく
		adrs[idx] = ~ptn;
	}

	// どのビットも...
	for (ptn = 0, idx = 0; idx < size / sizeof(Datum); ptn++, idx++) {

		// 反転した値も保持できてなければNG
		if (adrs[idx] != static_cast<Datum>(~ptn)) {
			return const_cast<Datum *>(&adrs[idx]);
		}

		// 0 にクリア
		adrs[idx] = 0;
	}
	return static_cast<Datum>(0);
}

// ------------------------  Public  ----------------------------
// MemTest<Datum>::Fill
// --------------------------------------------------------------
/**
 *	特定の値で埋める
 *
 *	@author	 	kurosawa
 *	@date   	200X/XX/XX
 *	@param		adrs	埋めたいRAMっぽいもののアドレス
 *	@param		size	埋めたいRAMっぽいもののサイズ(byte)
 *	@param		data	この値で埋める
 *	@pre		SPがセットされていないとまずいかも
 *	@note 		バス幅に合わせた型でインスタンス化するであります
 */
// --------------------------------------------------------------
template<class Datum>
void	MemTest<Datum>::Fill(volatile Datum * const adrs, unsigned long size, Datum data)
{
	for (unsigned long idx = 0; idx < size / sizeof(Datum); idx++) {
		adrs[idx] = data;
	}
}

// ------------------------  Public  ----------------------------
// MemTest<datum>::ChkFill
// --------------------------------------------------------------
/**
 *	特定の値で埋まってるかチェック
 *
 *	@author	 	kurosawa
 *	@date   	200X/XX/XX
 *	@param		adrs	埋めたつもりのRAMっぽいもののアドレス
 *	@param		size	埋めたつもりのRAMっぽいもののサイズ(byte)
 *	@param		data	この値で埋まってる予定
 *	@retval		0		正常終了
 *	@retval		not 0	最初にエラーしたアドレス
 *	@pre		SPがセットされていないとまずいかも
 *	@note 		バス幅に合わせた型でインスタンス化するであります
 */
// --------------------------------------------------------------
template<class Datum>
Datum *	MemTest<Datum>::ChkFill(volatile Datum * const adrs, unsigned long size, Datum data)
{
	for (unsigned long idx = 0; idx < size / sizeof(Datum); idx++) {
		if (adrs[idx] != data) {
			return const_cast<Datum *>(&adrs[idx]);
		}
	}
	return static_cast<Datum>(0);
}

#endif	// _MemTest_h_
