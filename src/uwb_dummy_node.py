#!/usr/bin/env python

"""uwb_dummy_node.py: Streams dummy UWB range measurements."""

__author__      = "Benjamin Hepp"
__email__ = "benjamin.hepp@inf.ethz.ch"
__copyright__   = "Copyright 2015 Benjamin Hepp"

import roslib
roslib.load_manifest('uwb')
import rospy

from uwb.msg import UWBRange, UWBRangeStats, UWBMultiRange, UWBMultiRangeRaw
import select

class UWBDummyPublisher(object):

    def __init__(self, uwb_stats_topic = None):
        uwb_range_topic = '/uwb/range'
        uwb_stats_topic = '/uwb/range_stats'
        uwb_multi_range_topic = '/uwb/multi_range'
        uwb_multi_range_raw_topic = '/uwb/multi_range_raw'
        self.uwb_range_pub = rospy.Publisher(uwb_range_topic, UWBRange, queue_size=1)
        self.uwb_stats_pub = rospy.Publisher(uwb_stats_topic, UWBRangeStats, queue_size=1)
        self.uwb_multi_range_pub = rospy.Publisher(uwb_multi_range_topic, UWBMultiRange, queue_size=1)
        self.uwb_multi_range_raw_pub = rospy.Publisher(uwb_multi_range_raw_topic, UWBMultiRangeRaw, queue_size=1)

    def run(self):
        while not rospy.is_shutdown():
            address = 1
            remote_address = 10
            range = 0.1
            stamp_us = 1
            round_trip_time = 1

            ros_msg = UWBRange()
            ros_msg.header.stamp = rospy.Time.now()
            ros_msg.address = address
            ros_msg.remote_address = remote_address
            ros_msg.stamp_us = stamp_us
            ros_msg.round_trip_time = round_trip_time
            ros_msg.range = range
            self.uwb_range_pub.publish(ros_msg)

            ros_msg = UWBRangeStats()
            ros_msg.header.stamp = rospy.Time.now()
            ros_msg.address = address
            ros_msg.remote_address = remote_address
            ros_msg.stamp_us = stamp_us
            ros_msg.round_trip_time = round_trip_time
            ros_msg.range = range
            self.uwb_stats_pub.publish(ros_msg)

            # ros_msg = UWBMultiRange()
            # ros_msg.header.stamp = rospy.Time.now()
            # ros_msg.num_of_units = 4
            # ros_msg.address = address
            # ros_msg.remote_address = remote_address
            # ros_msg.tofs = [0, 1, 2, 3]
            # ros_msg.ranges = [0, 1, 2, 3]
            # self.uwb_multi_range_pub.publish(ros_msg)

            ros_msg = UWBMultiRangeRaw()
            ros_msg.header.stamp = rospy.Time.now()
            ros_msg.num_of_units = 4
            ros_msg.address = address
            ros_msg.remote_address = remote_address
            ros_msg.timestamp_master_request_1 = [1, 1, 1, 1]
            ros_msg.timestamp_slave_reply = [1, 1, 1, 1]
            ros_msg.timestamp_master_request_2 = [1, 1, 1, 1]
            self.uwb_multi_range_raw_pub.publish(ros_msg)


def main():
    rospy.init_node('uwb_dummy_node')

    pub = UWBDummyPublisher()
    try:
        pub.run()
    except (rospy.ROSInterruptException, select.error):
        print("Interrupted... Stopping.")


if __name__ == '__main__':
    main()
