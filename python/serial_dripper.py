#!/usr/bin/env python3
"""モジュールの説明タイトル

    Todo:
"""
from enum import IntEnum
import os
import serial
from serial.tools import list_ports
import tkinter
import tkinter.filedialog
from tkinter import ttk
import threading


class thread_state(IntEnum):
    QUIT  = -1
    PAUSE = 0
    RUN   = 1


snp     = None
sno     = None
period  = None

snpfile_text = None

snpport = None
snoport = None

sno_entry = None

run = thread_state.PAUSE

def main():
    global snp
    global sno
    global period

    global snpfile_text

    global snpport
    global snoport

    global sno_entry

    global run

    root = tkinter.Tk()
    root.title(u'serial dripper')
    root.geometry("400x300")

    ports = list_ports.comports()

    snpfile_text = tkinter.StringVar()

    snpport = tkinter.StringVar()
    snoport = tkinter.StringVar()

    snpfile_label    = ttk.Label( root, textvariable = snpfile_text )
    snp_button       = ttk.Button( root, text = 'start', command = start_sendingsnoop )
    snpport_combobox = ttk.Combobox( root, state = 'readonly', textvariable = snpport, values = ports )

    sno_entry        = ttk.Entry( root )
    sno_button       = ttk.Button( root, text = 'serialNo', command = send_serialno )
    snoport_combobox = ttk.Combobox( root, state = 'readonly', textvariable = snoport, values = ports )

    quit_btn = ttk.Button(root, text = 'quit', command = lambda: root.quit())

    snpport_combobox.bind("<<ComboboxSelected>> ", setup_snpport)
    snoport_combobox.bind("<<ComboboxSelected>> ", setup_snoport)

    sno_entry.insert(tkinter.END, 'SN00001')

    snpfile_label.grid(    row = 0, column = 0 )
    snpport_combobox.grid( row = 0, column = 1 )
    snp_button.grid(       row = 0, column = 2 )

    sno_entry.grid(        row = 1, column = 0 )
    snoport_combobox.grid( row = 1, column = 1 )
    sno_button.grid(       row = 1, column = 2 )

    quit_btn.grid( row = 2, column = 0 )

    run = thread_state.PAUSE
    period = threading.Thread(target = periodic)
    period.start()

    root.mainloop()

    run = thread_state.QUIT
    if snp:
        snp.close()
    if sno:
        sno.close()


def periodic():
    global period
    global idx

    if run != thread_state.QUIT:
        period = threading.Timer(1, periodic)
        period.start()

    if run == thread_state.RUN:
        snp.write( snooplines[idx].replace('\n', '\r\n').encode('cp932') )

        idx += 1
        if idx >= len(snooplines):
            idx = 0
    


def setup_snpport(event):
    global snp

    p = snpport.get().split()[0]
    snp = serial.Serial(port = p, baudrate = 115200, timeout = 0)


def setup_snoport(event):
    global sno

    p = snoport.get().split()[0]
    sno = serial.Serial(port = p, baudrate = 9600, timeout = 0)


def send_serialno():
    if sno:
        sno.write((sno_entry.get() + '\r\n').encode('cp932'))


def start_sendingsnoop():
    global snpfile_text

    global run
    global snooplines
    global idx

    snp_file = tkinter.filedialog.askopenfilename(initialdir='~/')
    if snp_file:
        try:
            snpfile_text.set(os.path.basename(snp_file))
            with open(snp_file, 'r', encoding='cp932') as f:
                snooplines = f.readlines()

            idx = 0
            run = thread_state.RUN

        except OSError:
            run = thread_state.PAUSE

    else:
        run = thread_state.PAUSE


if __name__ == '__main__':
    main()
