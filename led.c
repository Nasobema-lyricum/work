//  ==============================================================
/**
 *	@file	led.c
 *	@brief	ESC ES check program
 *	
 *			ＥＳＣ(ES)のテストプログラム
 *	
 *			$Author: $
 *			$Date: $
 *			$Revision: $
*/
//  ==============================================================
#include <limits.h>
#include "types.h"
#include "esc.h"
#include "led.h"

// ==============================================================
//  Enumerations
// ==============================================================
enum	{NUMOF_LED = 8};
typedef enum {LED_OFF, LED_ON, LED_BLINK, ONE_SHOT} LEDState;

// ==============================================================
//  Local Variables
// ==============================================================
static unsigned short	ledDataBuf = 0;
static unsigned char	blinkPeriod[NUMOF_LED] = {0, 0, 0, 0, 0, 0, 0, 0};
static unsigned char	blinkCount[NUMOF_LED] = {0, 0, 0, 0, 0, 0, 0, 0};
static LEDState			ledState[NUMOF_LED] = {LED_OFF, LED_OFF, LED_OFF, LED_OFF,
											   LED_OFF, LED_OFF, LED_OFF, LED_OFF};

// ==============================================================
//  Private Function Prototypes
// ==============================================================

// ==============================================================
//  Private Functions
// ==============================================================
// ==============================================================
//  Public Functions
// ==============================================================
// ------------------------  Public  ----------------------------
// SetOff
// --------------------------------------------------------------
/*!
	消灯します

	@author	 	kurosawa
	@date   	2005/03/01
	@param		data	消灯するLEDのdata lineの番号
	@return		void
	@attention	0 <= scan < NUMOF_SCAN, 0 <= data < 16 であること
*/
// --------------------------------------------------------------
void SetOff(const int data)
{
	ledState[data] = LED_OFF;
	blinkPeriod[data] = 0;
	blinkCount[data] = 0;
	ledDataBuf = ledDataBuf & ~(0x0001 << data);	/// clr
}

// ------------------------  Public  ----------------------------
// SetOn
// --------------------------------------------------------------
/*!
	点灯します

	@author	 	kurosawa
	@date   	2005/03/01
	@param		data	点灯するLEDのdata lineの番号
	@return		void
	@attention	0 <= scan < NUMOF_SCAN, 0 <= data < 16 であること
*/
// --------------------------------------------------------------
void SetOn(const int data)
{
	ledState[data] = LED_ON;
	blinkPeriod[data] = 0;
	blinkCount[data] = 0;
	ledDataBuf = ledDataBuf | (0x0001 << data);		/// set
}

// ------------------------  Public  ----------------------------
// SetBlink
// --------------------------------------------------------------
/*!
	点滅します

	@author	 	kurosawa
	@date   	2005/03/01
	@param		data	点滅するLEDのdata lineの番号
	@param		period	点滅の周期/10msec
	@return		void
	@attention	0 <= scan < NUMOF_SCAN, 0 <= data < 16 <br>
				0 < period < 256 であること　
*/
// --------------------------------------------------------------
void SetBlink(const int data, const int period)
{
	ledState[data] = LED_BLINK;
	blinkPeriod[data] = period;
	blinkCount[data] = period / 2;

	ledDataBuf = ledDataBuf & ~(0x0001 << data);	/// clr
}

// ------------------------  Public  ----------------------------
// SetOneShot
// --------------------------------------------------------------
/*!

	@author	 	kurosawa
	@date   	2005/03/01
	@param		data	点灯するLEDのdata lineの番号
	@param		time	点灯時間/10msec
	@return		void
	@attention	0 <= scan < NUMOF_SCAN, 0 <= data < 16 <br>
				0 < period < 256 であること　
*/
// --------------------------------------------------------------
void SetOneShot(const int data, const int time)
{
	ledState[data] = ONE_SHOT;
	blinkPeriod[data] = 0;
	blinkCount[data] = time ;

	ledDataBuf = ledDataBuf | (0x0001 << data);		/// set
}

// ------------------------  Public  ----------------------------
// ProcLED
// --------------------------------------------------------------
/*!

	@author	 	kurosawa
	@date   	2005/02/23
	@return		void
*/
// --------------------------------------------------------------
void ProcLED(void)
{
	int i;

	for (i = 0; i < NUMOF_LED ; i++) {
		if (ledState[i] == LED_BLINK) {
			blinkCount[i]--;
			if (blinkCount[i] == 0) {
				blinkCount[i] = blinkPeriod[i] / 2;
				ledDataBuf = ( ledDataBuf & ~(0x0001 << i))
						   | (~ledDataBuf &  (0x0001 << i));	/// bit inv.
			}
		}
		if (ledState[i] == ONE_SHOT) {
			blinkCount[i]--;
			if (blinkCount[i] == 0) {
				ledDataBuf = ledDataBuf & ~(0x0001 << i);	/// clr
				ledState[i] = LED_OFF;
			}
		}
	}

#if LED_PORT == PORT3
	P3 = ledDataBuf;
#endif

#if LED_PORT == PORT5
	P5 = ledDataBuf;
#endif
}

void ProcLED(void)
{
    for (i = 0; i < NUMOF_LED ; i++) {
        if ( oneshotCount > 0 ) {
            if (oneshotState[i] == LED_BLINK) {
                blinkCount[i]--;
                if (blinkCount[i] == 0) {
                    if ( (ledData & 0x0001<<i) != 0 )
                }
            }
            oneshotCount--;
        }
        else {
        }
    }
}
