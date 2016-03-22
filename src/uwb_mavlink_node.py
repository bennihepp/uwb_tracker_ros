#!/usr/bin/env python

"""uwb_mavlink_node.py: Streams UWB multi-range timestamps read from serial port."""

__author__      = "Benjamin Hepp"
__email__ = "benjamin.hepp@inf.ethz.ch"
__copyright__   = "Copyright 2015 Benjamin Hepp"

import select
import sys
import serial
import roslib
roslib.load_manifest('uwb')
import rospy

import uwb.msg
import mavlink_bridge


class UWBMavlink(object):

    INFO_PRINT_RATE = 2

    def __init__(self, serial_port, baud_rate, topic):
        self.ser = serial.Serial(serial_port, baud_rate, timeout=0)
        self.uwb_pub = rospy.Publisher(topic, uwb.msg.UWBMultiRangeRaw, queue_size=1)
        self.mav = mavlink_bridge.MAVLinkBridge(self.ser)
        self.mav_error_handler = None
        self.msg_count = 0
        self.last_now = rospy.get_time()

    def set_mavlink_error_handler(self, error_handler):
        self.mav_error_handler = error_handler

    def run(self):
        while not rospy.is_shutdown():
            self.step(False)

    def step(self, check_rospy=True):
        if check_rospy:
            if rospy.is_shutdown():
                return False
        try:
            msg = self.mav.try_receive_message()
            if msg is not None:
                if msg.get_msgId() == mavlink_bridge.uwb.MAVLINK_MSG_ID_UWB_2WAY_MULTI_RANGE_RAW_4:
                    #print("UWB multi range raw: {}".format(msg))

                    ros_msg = uwb.msg.UWBMultiRangeRaw()
                    ros_msg.header.stamp = rospy.Time.now()
                    ros_msg.num_of_units = msg.num_of_units
                    ros_msg.address = msg.address
                    ros_msg.remote_address = msg.remote_address
                    ros_msg.timestamp_master_request_1_recv = msg.timestamp_master_request_1_recv
                    ros_msg.timestamp_slave_reply_send = msg.timestamp_slave_reply_send
                    ros_msg.timestamp_master_request_2_recv = msg.timestamp_master_request_2_recv
                    ros_msg.timestamp_master_request_1 = msg.timestamp_master_request_1
                    ros_msg.timestamp_slave_reply = msg.timestamp_slave_reply
                    ros_msg.timestamp_master_request_2 = msg.timestamp_master_request_2
                    self.uwb_pub.publish(ros_msg)
                elif msg.get_msgId() == mavlink_bridge.uwb.MAVLINK_MSG_ID_UWB_STATUS:
                    print("STATUS: {}".format(msg.description))

                self.msg_count += 1

            now = rospy.get_time()
            if now - self.last_now >= self.INFO_PRINT_RATE:
                msg_rate = self.msg_count / (now - self.last_now)
                print("Receiving MAVLink messages with rate {} Hz".format(msg_rate))
                self.last_now = now
                self.msg_count = 0
        except mavlink_bridge.uwb.MAVError, e:
            if self.mav_error_handler is not None:
                self.mav_error_handler(e)
            else:
                sys.stderr.write("MAVError: {}\n".format(e))
        return True


def main():
    rospy.init_node('uwb_mavlink_node')

    serial_port = rospy.get_param('~serial_port', '/dev/ttyACM0')
    baud_rate = int(rospy.get_param('~baud_rate', "115200"))
    topic = rospy.get_param('~topic', '/uwb/timestamps')
    print("Reading from serial port {} with baud-rate {}".format(serial_port, baud_rate))
    print("Publishing to {}".format(topic))

    try:
        um = UWBMavlink(serial_port, baud_rate, topic)
        um.run()
    except (rospy.ROSInterruptException, select.error):
        print("Interrupted... Stopping.")
    except serial.serialutil.SerialException, e:
        print("Problem with serial port: {}".format(e))


if __name__ == '__main__':
    main()
