# -*- coding: utf-8 -*-
"""
Created on Fri Nov 13 14:58:47 2015

@author: Benjamin Hepp
"""

import serial
import argparse
import sys

from uwb_protocol import UWBTextStruct

parser = argparse.ArgumentParser(description='Read UWB range measurements.')
parser.add_argument('serial_port', type=str,
                   help='which serial port to use')
parser.add_argument('--baud_rate', metavar='b', type=int,
                    default=115200, nargs=1,
                   help='baud-rate')
args = parser.parse_args()
serial_port = args.serial_port
baud_rate = args.baud_rate

print("Serial port: {}, Baud-rate: {}".format(serial_port, baud_rate))

ser = serial.Serial(serial_port, baud_rate, timeout=None)
uwb_struct = UWBTextStruct()
try:
    while True:
        #message = ser.read(uwb_struct.size())
        message = ser.readline()
        msg = uwb_struct.decode(message)
        if msg is not None:
            print("UWB range: {}".format(msg))
        else:
            print("Received invalid message: {}".format(msg))
            sys.stdout.flush()
except KeyboardInterrupt:
    print("Received CTRL+C. Stopping...")
    sys.stdout.flush()

ser.close()
