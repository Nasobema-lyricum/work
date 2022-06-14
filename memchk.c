#include    "dsp_dev.h"

// ==============================================================
//  Defines
// ==============================================================
#define INT_TOP_ADRS    0x00000000UL
#define INT_LAST_ADRS   0x0001FFFFUL
// ------------------------  Public  ----------------------------
// FillIntDlyMem
// --------------------------------------------------------------
/**
 *  内蔵遅延RAMを埋める<br>
 *
 *  \author     kurosawa
 *  \date       2005/03/08
 *  \param      val 埋める値
 *  \pre        SPがセットされていないとまずいかも
 *  \post       RAMの中身は全部消えるよ
 *  \todo 
 */
// --------------------------------------------------------------
void FillIntDlyMem(unsigned long val)
{
    unsigned long   adrs;

    // すべてのビットに...
    for (adrs = INT_TOP_ADRS; adrs <= INT_LAST_ADRS; adrs += 4) {

        // 何でもいいから書いておく
        gDevESC->reg.write._memAdrs = adrs;
        gDevESC->ram._dlData = val;

    }
}

// ------------------------  Public  ----------------------------
// CheckFillIntDlyMem
// --------------------------------------------------------------
/**
 *  内蔵遅延RAMが埋められてるか<br>
 *
 *  \author     kurosawa
 *  \date       2005/03/08
 *  \param      val 埋められてるはずの値
 *  \pre        SPがセットされていないとまずいかも
 *  \post       RAMの中身は全部消えるよ
 *  \todo 
 */
// --------------------------------------------------------------
unsigned long CheckFillIntDlyMem(unsigned long val)
{
    unsigned long   adrs;

    for (adrs = INT_TOP_ADRS; adrs <= INT_LAST_ADRS; adrs += 4) {

        // 書いた値が保持できてなければNG
        gDevESC->reg.write._memAdrs = adrs;
        if (gDevESC->ram._dlData != val) {
            return adrs;
        }
    }

    return 0xFFFFFFFF;
}
// ------------------------  Public  ----------------------------
// TestIntDlyMem
// --------------------------------------------------------------
/**
 *  内蔵遅延RAMをテスト<br>
 *  すべてのビットについて 0/1 を保持できるかテストする<br>
 *
 *  \author     kurosawa
 *  \date       2005/03/08
 *  \retval     0       正常終了
 *  \retval     not 0   最初にエラーしたアドレス
 *  \pre        SPがセットされていないとまずいかも
 *  \post       RAMの中身は全部消えるよ
 *  \todo 
 */
// --------------------------------------------------------------
unsigned long TestIntDlyMem(void)
{
    unsigned long   adrs;
    unsigned long   ptn;
    unsigned long   antiptn;
    volatile unsigned long  tmp;

    // すべてのビットに...
    for (ptn = 0, adrs = INT_TOP_ADRS; adrs <= INT_LAST_ADRS; ptn += 2, adrs += 4) {

        // 何でもいいから書いておく
        gDevESC->reg.write._memAdrs = adrs;
        gDevESC->ram._dlData = ptn;

    }

    // どのビットも...
    for (ptn = 0, antiptn = 0xfffffffe, adrs = INT_TOP_ADRS; adrs <= INT_LAST_ADRS; ptn += 2, antiptn -= 2, adrs += 4) {

        // 書いた値が保持できてなければNG
        gDevESC->reg.write._memAdrs = adrs;
        if (gDevESC->ram._dlData != ptn) return adrs;

        // 反転した値を書いておく
        gDevESC->reg.write._memAdrs = adrs;
        gDevESC->ram._dlData = antiptn;
    }



    // どのビットも...
    for (antiptn = 0xfffffffe, adrs = INT_TOP_ADRS; adrs <= INT_LAST_ADRS; antiptn -= 2, adrs += 4) {

        // 反転した値も保持できてなければNG
        gDevESC->reg.write._memAdrs = adrs;
        if (gDevESC->ram._dlData != antiptn) return adrs;

        // 0 にクリア
        gDevESC->reg.write._memAdrs = adrs;
        gDevESC->ram._dlData = 0;
    }

    return 0xFFFFFFFF;
}

#define EXT_TOP_ADRS    0x00800000UL
#define EXT_LAST_ADRS   0x00fFFFFcUL
// ------------------------  Public  ----------------------------
// FillExtDlyMem
// --------------------------------------------------------------
/**
 *  外部遅延RAMを埋める<br>
 *
 *  \author     kurosawa
 *  \date       2005/03/08
 *  \param      val 埋める値
 *  \pre        SPがセットされていないとまずいかも
 *  \post       RAMの中身は全部消えるよ
 *  \todo 
 */
// --------------------------------------------------------------
void FillExtDlyMem(unsigned long val)
{
    unsigned long   adrs;

    // すべてのビットに...
    for (adrs = EXT_TOP_ADRS; adrs <= EXT_LAST_ADRS; adrs += 4) {

        // 何でもいいから書いておく
        gDevESC->reg.write._memAdrs = adrs;
        gDevESC->ram._dlData = val;

    }
}

// ------------------------  Public  ----------------------------
// CheckFillExtDlyMem
// --------------------------------------------------------------
/**
 *  外部遅延RAMが埋められてるか<br>
 *
 *  \author     kurosawa
 *  \date       2005/03/08
 *  \param      val 埋められてるはずの値
 *  \pre        SPがセットされていないとまずいかも
 *  \post       RAMの中身は全部消えるよ
 *  \todo 
 */
// --------------------------------------------------------------
unsigned long CheckFillExtDlyMem(unsigned long val)
{
    volatile unsigned long   adrs;
    volatile unsigned long   tmp;

    // for (adrs = EXT_LAST_ADRS; adrs >= EXT_TOP_ADRS; adrs -= 4) {
    for (adrs = EXT_TOP_ADRS; adrs <= EXT_LAST_ADRS; adrs += 4) {

        // 書いた値が保持できてなければNG
        gDevESC->reg.write._memAdrs = adrs;
        tmp = gDevESC->ram._dlData;
        if (tmp != val) {
            return adrs;
        }

    }
    return 0xFFFFFFFF;
}

// ------------------------  Public  ----------------------------
// TestExtDlyMem
// --------------------------------------------------------------
/**
 *  外部遅延RAMをテスト<br>
 *  すべてのビットについて 0/1 を保持できるかテストする<br>
 *
 *  \author     kurosawa
 *  \date       2005/03/08
 *  \retval     0       正常終了
 *  \retval     not 0   最初にエラーしたアドレス
 *  \pre        SPがセットされていないとまずいかも
 *  \post       RAMの中身は全部消えるよ
 *  \todo 
 */
// --------------------------------------------------------------
unsigned long TestExtDlyMem(void)
{
    volatile unsigned long   adrs;
    volatile unsigned long   ptn;
    volatile unsigned long   antiptn;
    volatile unsigned long  tmp;

    // すべてのビットに...
    for (ptn = 0, adrs = EXT_TOP_ADRS; adrs <= EXT_LAST_ADRS; ptn += 1, adrs += 4) {

        // 何でもいいから書いておく
        gDevESC->reg.write._memAdrs = adrs;
        gDevESC->ram._dlData = ptn;
    }

    // どのビットも...
    for (ptn = 0, antiptn = 0xffffffff, adrs = EXT_TOP_ADRS; adrs <= EXT_LAST_ADRS; ptn += 1, antiptn -= 1, adrs += 4) {

        // 書いた値が保持できてなければNG
        gDevESC->reg.write._memAdrs = adrs;
        tmp = gDevESC->ram._dlData;
        if (tmp != ptn) {
            return adrs;
        }

        // 反転した値を書いておく
        gDevESC->reg.write._memAdrs = adrs;
        gDevESC->ram._dlData = antiptn;
    }



    // どのビットも...
    for (antiptn = 0xffffffff, adrs = EXT_TOP_ADRS; adrs <= EXT_LAST_ADRS; antiptn -= 1, adrs += 4) {

        // 反転した値も保持できてなければNG
        gDevESC->reg.write._memAdrs = adrs;
        tmp = gDevESC->ram._dlData;
        if (tmp != antiptn) {
            return adrs;
        }

        // 0 にクリア
        gDevESC->reg.write._memAdrs = adrs;
        gDevESC->ram._dlData = 0;
    }

    return 0xFFFFFFFF;
}


// ------------------------  Public  ----------------------------
// TestDlyMem
// --------------------------------------------------------------
/**
 *  RAMをテスト<br>
 *
 *  \author     kurosawa
 *  \date       2005/03/08
 *  \retval     0       正常終了
 *  \retval     not 0   最初にエラーしたアドレス
 *  \pre        SPがセットされていないとまずいかも
 *  \post       RAMの中身は全部消えるよ
 *  \todo 
 */
// --------------------------------------------------------------
#if 0
#define TOP_ADRS    0x003e0000UL
#define LAST_ADRS   0x003FFFFFUL
#else
#define TOP_ADRS    0x00800000UL
#define LAST_ADRS   0x00fFFFFFUL
#endif
unsigned long TestDlyMem(void)
{
    return 0xFFFFFFFF;
}
