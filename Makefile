#===============================================================
#
# Makefile	For AP006 PANEL/
#
#
#		$Author: ochi $
#		$Revision: 1.4 $
#		$Date: 2008/04/22 04:54:14 $
#
#===============================================================
.SILENT:

.SUFFIXES:	.o .s .src .c .inc .h

SUFFIX		= .inc:h

SRC_DIR		= PANEL

OBJS		= mode_boot.o \
		mode_manual.o \
		mode_test.o \
		mode_tuner.o \
		panel.o \
		param.o \
		param_mgr.o \
		test_mode.o \
		tuner.o

SRCS		= mode_boot.c \
		mode_manual.c \
		mode_test.c \
		mode_tuner.c \
		panel.c \
		param.c \
		param_mgr.c \
		test_mode.c \
		tuner.c

EXTHDRS		= 

#OPTIMIZE	= -Ot
OPTIMIZE	= 

include		../Makefile_com

###
mode_boot.o: mode_boot.c mode_boot.h ../DEV/sw_drv.h ../INC/cpu.h \
  ../INC/io_port.h ../DEV/ad_drv.h ../INC/types.h panel.h ../INC/types.h \
  param_mgr.h ../EFFECT/effect.h ../INC/types.h ../EFFECT/ctrl_amp.h \
  ../DEV/led_drv.h ../SYS/timer.h
mode_manual.o: mode_manual.c mode_manual.h ../DEV/ad_drv.h ../INC/types.h \
  ../DEV/sw_drv.h ../INC/cpu.h ../INC/io_port.h panel.h ../INC/types.h \
  mode_boot.h param_mgr.h ../DEV/led_drv.h tuner.h ../INC/pitch.h \
  ../INC/types.h ../DEV/dsp_drv.h ../DEV/dsp_dev.h ../SYS/alarm.h \
  ../INC/types.h ../SYS/timer.h ../EFFECT/effect.h ../INC/types.h \
  ../EFFECT/ctrl_amp.h ../DEV/port_drv.h ../DSP/ap006.vdt
mode_test.o: mode_test.c ../INC/types.h mode_test.h ../DEV/led_drv.h \
  ../INC/cpu.h ../SYS/timer.h ../DEV/ad_drv.h ../DEV/sw_drv.h \
  ../INC/io_port.h panel.h test_mode.h ../INC/version.h ../DEV/port_drv.h \
  param_mgr.h mode_boot.h ../INC/update.h
mode_tuner.o: mode_tuner.c ../SYS/init.h ../SYS/int.h mode_tuner.h \
  ../DEV/ad_drv.h ../INC/types.h ../DEV/sw_drv.h ../INC/cpu.h \
  ../INC/io_port.h panel.h ../INC/types.h ../SYS/alarm.h ../INC/types.h \
  param_mgr.h ../DEV/led_drv.h tuner.h ../INC/pitch.h ../INC/types.h \
  mode_manual.h ../SYS/timer.h mode_boot.h
panel.o: panel.c panel.h ../INC/types.h ../SYS/timer.h ../DEV/sw_drv.h \
  ../INC/cpu.h ../INC/io_port.h ../DEV/ad_drv.h ../INC/types.h \
  ../DEV/led_drv.h ../INC/pitch.h ../INC/types.h mode_boot.h \
  mode_manual.h mode_tuner.h mode_test.h param_mgr.h
param_mgr.o: param_mgr.c param_mgr.h ../INC/types.h ../INC/pitch.h \
  ../INC/types.h ../DEV/ad_drv.h ../INC/types.h ../DEV/sw_drv.h \
  ../INC/cpu.h ../INC/io_port.h ../SYS/timer.h ../DEV/led_drv.h \
  ../DEV/port_drv.h tuner.h ../INC/eeprom.h ../INC/memory_adrs.h \
  ../INC/eeprom.h ../TABLE/table.h ../SYS/alarm.h ../INC/types.h \
  ../EFFECT/effect.h ../INC/types.h ../EFFECT/ctrl_amp.h
test_mode.o: test_mode.c ../SYS/init.h test_mode.h ../INC/types.h \
  ../DEV/ad_drv.h ../DEV/led_drv.h ../INC/cpu.h ../DEV/sw_drv.h \
  ../INC/io_port.h mode_test.h ../SYS/timer.h panel.h ../EFFECT/effect.h \
  ../EFFECT/ctrl_amp.h param_mgr.h ../TABLE/table.h ../INC/update.h \
  ../DEV/port_drv.h
tuner.o: tuner.c ../DEV/led_drv.h ../INC/cpu.h tuner.h ../INC/pitch.h \
  ../INC/types.h
