#!/usr/bin/env python

"""uwb_range.py: Streams UWB range measurements from serial port."""

__author__      = "Benjamin Hepp"
__email__ = "benjamin.hepp@inf.ethz.ch"
__copyright__   = "Copyright 2015 Benjamin Hepp"

import roslib
roslib.load_manifest('uwb')
import rospy

from uwb.msg import UWBRange, UWBRangeStats
import select
import sys
import serial
import mavlink_bridge

class UWBReader(object):

    def __init__(self, serial_port, baud_rate, uwb_topic, uwb_stats_topic = None):
        if uwb_stats_topic is None:
            uwb_stats_topic = '{}_stats'.format(uwb_topic)
        self.ser = serial.Serial(serial_port, baud_rate, timeout=0)
        self.uwb_pub = rospy.Publisher(uwb_topic, UWBRange, queue_size=1)
        self.uwb_stats_pub = rospy.Publisher(uwb_stats_topic, UWBRangeStats, queue_size=1)
        self.mav = mavlink_bridge.MAVLinkBridge(self.ser)
        self.mav_error_handler = None

    def set_mavlink_error_handler(self, error_handler):
        self.mav_error_handler = error_handler

    def run(self):
        while not rospy.is_shutdown():
            try:
                msg = self.mav.try_receive_message()
                if msg is not None:
                    if msg.get_msgId() == mavlink_bridge.uwb.MAVLINK_MSG_ID_UWB_RANGE:
                        #print("UWB range: {}".format(msg))
                        ros_msg = UWBRange()
                        ros_msg.address = msg.address
                        ros_msg.remote_address = msg.remote_address
                        ros_msg.stamp_us = msg.stamp_us
                        ros_msg.round_trip_time = msg.round_trip_time
                        ros_msg.range = msg.range
                        self.uwb_pub.publish(ros_msg)
                    elif msg.get_msgId() == mavlink_bridge.uwb.MAVLINK_MSG_ID_UWB_RANGE_STATS:
                        #print("UWB range stat: {}".format(msg))
                        ros_msg = UWBRangeStats()
                        ros_msg.address = msg.address
                        ros_msg.remote_address = msg.remote_address
                        ros_msg.stamp_us = msg.stamp_us
                        ros_msg.round_trip_time = msg.round_trip_time
                        ros_msg.range = msg.range
                        ros_msg.std_noise[0] = msg.std_noise_1
                        ros_msg.std_noise[1] = msg.std_noise_2
                        ros_msg.std_noise[2] = msg.std_noise_3
                        ros_msg.preamble_acc_count[0] = msg.preamble_acc_count_1
                        ros_msg.preamble_acc_count[1] = msg.preamble_acc_count_2
                        ros_msg.preamble_acc_count[2] = msg.preamble_acc_count_3
                        ros_msg.first_path_index[0] = msg.first_path_index_1
                        ros_msg.first_path_index[1] = msg.first_path_index_2
                        ros_msg.first_path_index[2] = msg.first_path_index_3
                        ros_msg.first_path_amp_1[0] = msg.first_path_amp_1_1
                        ros_msg.first_path_amp_1[1] = msg.first_path_amp_1_2
                        ros_msg.first_path_amp_1[2] = msg.first_path_amp_1_3
                        ros_msg.first_path_amp_2[0] = msg.first_path_amp_2_1
                        ros_msg.first_path_amp_2[1] = msg.first_path_amp_2_2
                        ros_msg.first_path_amp_2[2] = msg.first_path_amp_2_3
                        ros_msg.first_path_amp_3[0] = msg.first_path_amp_3_1
                        ros_msg.first_path_amp_3[1] = msg.first_path_amp_3_2
                        ros_msg.first_path_amp_3[2] = msg.first_path_amp_3_3
                        ros_msg.channel_impulse_response_power[0] = msg.channel_impulse_response_power_1
                        ros_msg.channel_impulse_response_power[1] = msg.channel_impulse_response_power_2
                        ros_msg.channel_impulse_response_power[2] = msg.channel_impulse_response_power_3
                        ros_msg.prf[0] = msg.prf_1
                        ros_msg.prf[1] = msg.prf_2
                        ros_msg.prf[2] = msg.prf_3
                        self.uwb_stats_pub.publish(ros_msg)
            except mavlink_bridge.uwb.MAVError, e:
                if self.mav_error_handler is not None:
                    self.mav_error_handler(e)
                else:
                    sys.stderr.write("MAVError: {}\n".format(e))


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
