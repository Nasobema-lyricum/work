#!/usr/bin/env python3
"""モジュールの説明タイトル

    Todo:
"""
import serial
from serial.tools import list_ports
import tkinter
from tkinter import filedialog
from tkinter import ttk
import threading


snp     = None
sno     = None
period  = None

run = False


def main():
    global snp
    global sno
    global period
    global run

    root = tkinter.Tk()
    root.title(u'serial dripper')
    root.geometry("400x300")


    ports = list_ports.comports()

    snp_filename_label = ttk.Label(    root, text = 'file',  padding = (5, 10)            )
    snp_port_combobox  = ttk.Combobox( root, state = 'readonly', textvariable = snp_port, values = ports )
    snp_button         = ttk.Button(   root, text = 'start', command = snp_btn_click      )

    com_name = ttk.Label(root, text = 'com', padding = (5, 10))
    sno_btn = ttk.Button(root, text = 'serialNo', command = sno_btn_click)

    quit_btn = ttk.Button(root, text = 'quit', command = lambda: root.quit())

    snp_filename_label.grid( row = 0, column = 0 )
    snp_port_combobox.grid(  row = 0, column = 1 )
    snp_button.grid(         row = 0, column = 2 )

    com_name.grid( row = 1, column = 0 )
    sno_btn.grid(  row = 1, column = 1 )

    quit_btn.grid(row = 2, column = 0)

    snp = serial.Serial(port = 'COM10',  baudrate = 115200, timeout = 0)
    sno = serial.Serial(port = 'COM12',  baudrate =   9600, timeout = 0)

    run = False
    period = threading.Thread(target = periodic)
    period.start()

    root.mainloop()

    run = False
    period.join()
    snp.close()


def periodic():
    global snp
    global period

    if run:
        period = threading.Timer(1, periodic)
        period.start()

        snp.write(b'test\r\n')


def snp_btn_click():
    global run
    snp_file = filedialog.askopenfile(initialdir='~/')
    if snp_file:
        #  ファイルを開いてスレッドで周期的に投げる
        run = True
    else:
        run = False


def sno_btn_click():
    # シリアル番号を投げる
    pass


if __name__ == '__main__':
    main()
