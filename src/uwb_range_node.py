#!/usr/bin/env python

"""uwb_range.py: Streams UWB range measurements from serial port."""

__author__      = "Benjamin Hepp"
__email__ = "benjamin.hepp@inf.ethz.ch"
__copyright__   = "Copyright 2015 Benjamin Hepp"

import roslib
roslib.load_manifest('uwb')
import rospy

import select
import serial
from uwb.src.mavlink_bridge.uwb import UWBRange
from uwb_protocol import UWBTextStruct

class UWBReader(object):

    def __init__(self, serial_port, baud_rate, uwb_topic):
        self.ser = serial.Serial(serial_port, baud_rate)
        self.uwb_pub = rospy.Publisher(uwb_topic, UWBRange, queue_size=1)
        self.uwb_struct = UWBTextStruct()

    def run(self):
        while not rospy.is_shutdown():
            message = self.ser.readline()
            msg = self.uwb_struct.decode(message)
            if msg is not None:
                print("UWB range: {}".format(msg))
                ros_msg = UWBRange()
                ros_msg.device_id = msg.device_id
                ros_msg.stamp_us = msg.stamp_us
                ros_msg.range = msg.range
                ros_msg.target_id = msg.target_id
                self.uwb_pub.publish(ros_msg)


def main():
    rospy.init_node('uwb_range_node')

    serial_port = rospy.get_param('~serial_port')
    baud_rate = int(rospy.get_param('~baud_rate', "115200"))
    uwb_topic = rospy.get_param('~uwb_topic', '/uwb/range')
    print("Reading from serial port {} with baud-rate {}".format(serial_port, baud_rate))
    print("Publishing to {}".format(uwb_topic))

    ur = UWBReader(serial_port, baud_rate, uwb_topic)
    try:
        ur.run()
    except (rospy.ROSInterruptException, select.error):
        print("Interrupted... Stopping.")


if __name__ == '__main__':
    main()
