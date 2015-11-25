#!/usr/bin/env python

"""uwb_dummy_node.py: Streams dummy UWB range measurements."""

__author__      = "Benjamin Hepp"
__email__ = "benjamin.hepp@inf.ethz.ch"
__copyright__   = "Copyright 2015 Benjamin Hepp"

import roslib
roslib.load_manifest('uwb')
import rospy

from uwb.msg import UWBRange, UWBRangeStats
import select

class UWBDummyPublisher(object):

    def __init__(self, uwb_topic, uwb_stats_topic = None):
        if uwb_stats_topic is None:
            uwb_stats_topic = '{}_stats'.format(uwb_topic)
        self.uwb_pub = rospy.Publisher(uwb_topic, UWBRange, queue_size=1)
        self.uwb_stats_pub = rospy.Publisher(uwb_stats_topic, UWBRangeStats, queue_size=1)

    def run(self):
        while not rospy.is_shutdown():
            address = 1
            remote_address = 2
            range = 0.1
            stamp_us = 1
            round_trip_time = 1
            ros_msg = UWBRange()
            ros_msg.address = address
            ros_msg.remote_address = remote_address
            ros_msg.stamp_us = stamp_us
            ros_msg.round_trip_time = round_trip_time
            ros_msg.range = range
            self.uwb_pub.publish(ros_msg)

            ros_msg = UWBRangeStats()
            ros_msg.address = address
            ros_msg.remote_address = remote_address
            ros_msg.stamp_us = stamp_us
            ros_msg.round_trip_time = round_trip_time
            ros_msg.range = range
            self.uwb_stats_pub.publish(ros_msg)


def main():
    rospy.init_node('uwb_dummy_node')

    uwb_topic = rospy.get_param('~uwb_topic', '/uwb/dummy_range')
    print("Publishing to {}".format(uwb_topic))

    pub = UWBDummyPublisher(uwb_topic)
    try:
        pub.run()
    except (rospy.ROSInterruptException, select.error):
        print("Interrupted... Stopping.")


if __name__ == '__main__':
    main()
