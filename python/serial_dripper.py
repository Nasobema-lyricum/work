#!/usr/bin/env python3
"""モジュールの説明タイトル

    Todo:
"""
import serial
import tkinter
import tkinter.filedialog
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

    snp = serial.Serial(port = 'COM6',  baudrate = 115200, timeout = 0)
    sno = serial.Serial(port = 'COM8',  baudrate =   9600, timeout = 0)

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
