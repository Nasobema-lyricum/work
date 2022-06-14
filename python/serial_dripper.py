#!/usr/bin/env python3
"""モジュールの説明タイトル

    Todo:
"""
import serial
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

    snpFlileName = ttk.Label( root, text = 'file', padding = (5, 10))
    snpFlileName.grid(row = 0, column = 0)

    snpBtn = ttk.Button( root, text = 'snoop', command = lambda: root.quit())
    snpBtn.grid(row = 0, column = 1)

    comName = ttk.Label( root, text = 'com', padding = (5, 10))
    comName.grid(row = 1, column = 0)

    snoBtn = ttk.Button( root, text = 'serialNo', command = lambda: root.quit())
    snoBtn.grid(row = 1, column = 1)

    quitBtn = ttk.Button( root, text = 'quit', command = lambda: root.quit())
    quitBtn.grid(row = 2, column = 0)

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


if __name__ == '__main__':
    main()
