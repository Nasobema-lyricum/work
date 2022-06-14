// ==============================================================
/**
 *  @file   MidiParserState.h
 *  @brief  MIDIパーサのアブストラクトなstateクラス(もどき）
 *
 *          $Author: kurosawa $
 *          $Date: 2009/07/17 10:23:04 $
 *          $Revision: 1.2 $
 */
// ==============================================================
#ifndef _MidiParserState
#define _MidiParserState

// ==============================================================
//  includes
// ==============================================================
// #include <hogehoge>
#include "MidiParser.h"

// ==============================================================
//  Typedefs
// ==============================================================
typedef void (*RxMidiParserConc)(Stream * const stream, unsigned char);
typedef void (*RxMidiParserExitState)(Stream * const stream);
typedef void (*RxMidiParserEnterState)(Stream * const stream);

typedef struct {
    RxMidiParserConc        parser;
    RxMidiParserExitState   exitState;
    RxMidiParserEnterState  enterState;
} MidiParserState;

#endif  // _MidiParserState
