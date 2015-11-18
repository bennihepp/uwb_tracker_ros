# -*- coding: utf-8 -*-
"""
Created on Fri Nov 18 2015

@author: Benjamin Hepp
"""

import serial
import argparse
import sys
import mavlink_bridge

parser = argparse.ArgumentParser(description='Read UWB range measurements.')
parser.add_argument('--serial_port', type=str,
                    default='/dev/ttyACM0',
                    help='which serial port to use')
parser.add_argument('--baud_rate', metavar='b', type=int,
                    default=115200,
                   help='baud-rate')
args = parser.parse_args()
serial_port = args.serial_port
baud_rate = args.baud_rate

print("Serial port: {}, Baud-rate: {}".format(serial_port, baud_rate))

ser = serial.Serial(serial_port, baud_rate, timeout=0)
mav = mavlink_bridge.MAVLinkBridge(ser)
try:
    while True:

        try:
            msg = mav.try_receive_message()
            if msg is not None:
                if msg.get_msgId() == mavlink_bridge.uwb.MAVLINK_MSG_ID_UWB_RANGE:
                    print("UWB range: {}".format(msg))
                    sys.stdout.flush()
                elif msg.get_msgId() == mavlink_bridge.uwb.MAVLINK_MSG_ID_NAMED_VALUE_INT:
                    print("UWB range stat: {}".format(msg))
                    sys.stdout.flush()
        except mavlink_bridge.uwb.MAVError, e:
            print(e)
except KeyboardInterrupt:
    print("Received CTRL+C. Stopping...")
    sys.stdout.flush()

ser.close()
