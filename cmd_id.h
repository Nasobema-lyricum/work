/*==============================================================
 *
 *	cmd_id.h
 *
 *		Command ID of Global message
 *
 *		$Author: fuke $
 *		$Revision: 236 $
 *		$Date: 2011-01-27 22:54:43 +0900 #$
 *
 *==============================================================
 */
#ifndef	_cmd_id_h_
#define	_cmd_id_h_

enum {
	CMD_PLAY_GUIDE,			
	CMD_STOP_GUIDE,			
	CMD_MUTE_GUIDE,

        CMD_CHANGE_PATTERN,
        CMD_CHANGE_BEAT,

// dummy	
	CMD_CHANGE_DISPLAY,
	
	CMD_SET_TICK,

	NUMOF_CMDID,
};

/* for CMD_CHANGE_DISPLAY */
enum {					/* "disp data"	"comment" */
	CMD_DISPLAY_PLAY = 0,		/* none		PANEL --> AUDIO, SEQ */
	CMD_DISPLAY_PATCH_SELECT,	/* none		PANEL --> AUDIO, SEQ */
	CMD_DISPLAY_PATCH_RESERVED,	/* none		PANEL --> AUDIO, SEQ */
};

enum {
	CMD_DISPLAY_OFF = 0,
	CMD_DISPLAY_ON
};

#endif	/* _cmd_id_h_ */
