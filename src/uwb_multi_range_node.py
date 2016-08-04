#!/usr/bin/env python

"""uwb_multi_range_node.py: Streams UWB multi-range measurements based on UWB timestamps."""

from __future__ import print_function

__author__      = "Benjamin Hepp"
__email__       = "benjamin.hepp@inf.ethz.ch"
__copyright__   = "Copyright 2016 Benjamin Hepp"

import select
import numpy as np
import roslib
roslib.load_manifest('uwb')
import rospy

import uwb.msg


class UWBMultiRange(object):

    INFO_PRINT_RATE = 2

    US_TO_DW_TIMEUNITS = 128. * 499.2  # conversion between microseconds to the decawave timeunits (ca 15.65ps).
    DW_TIMEUNITS_TO_US = 1 / US_TO_DW_TIMEUNITS  # conversion between the decawave timeunits (~15.65ps) to microseconds.
    CONST_2POWER40 = 1099511627776  # Time register in DW1000 is 40 bit so this is needed to detect overflows.
    SPEED_OF_LIGHT_IN_M_PER_US = 299.792458
    SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT = SPEED_OF_LIGHT_IN_M_PER_US / US_TO_DW_TIMEUNITS

    VISUALIZATION_DATA_LENGTH = 500

    def __init__(self):
        self._read_configuration()

        if self.show_plots:
            self._setup_plots()

        rospy.loginfo("Receiving timestamp messages from {}".format(self.uwb_timestamps_topic))
        rospy.loginfo("Publishing multi-range messages to {}".format(self.uwb_multi_range_topic))
        rospy.loginfo("Publishing raw multi-range messages to {}".format(self.uwb_multi_range_raw_topic))
        rospy.loginfo("Publishing multi-range-with-offsets messages to {}".format(
            self.uwb_multi_range_with_offsets_topic))

        # ROS Publishers
        self.uwb_pub = rospy.Publisher(self.uwb_multi_range_topic, uwb.msg.UWBMultiRange, queue_size=1)
        self.uwb_raw_pub = rospy.Publisher(self.uwb_multi_range_raw_topic, uwb.msg.UWBMultiRange, queue_size=1)
        self.uwb_with_offsets_pub = rospy.Publisher(self.uwb_multi_range_with_offsets_topic,
                                                    uwb.msg.UWBMultiRangeWithOffsets, queue_size=1)
        self.uwb_timestamps_sub = rospy.Subscriber(self.uwb_timestamps_topic, uwb.msg.UWBMultiRangeTimestamps,
                                                   self.handle_timestamps_message)

        # Variables for rate display
        self.msg_count = 0
        self.last_now = rospy.get_time()

    def _read_configuration(self):
        self._read_unit_offsets()
        self.show_plots = rospy.get_param('~show_plots', True)
        self.show_slave_clock_offset = rospy.get_param('~show_slave_clock_offset', False)
        self.show_slave_clock_skew = rospy.get_param('~show_slave_clock_skew', False)
        self.uwb_timestamps_topic = rospy.get_param('~timestamps_topic', '/uwb/timestamps')
        self.uwb_multi_range_topic = rospy.get_param('~multi_range_topic', '/uwb/multi_range')
        self.uwb_multi_range_raw_topic = rospy.get_param('~multi_range_raw_topic', '/uwb/multi_range_raw')
        self.uwb_multi_range_with_offsets_topic = rospy.get_param('~multi_range_with_offsets_topic',
                                                                  '/uwb/multi_range_with_offsets')

    def _read_unit_offsets(self):
        if not rospy.has_param('~num_of_units'):
            rospy.logwarn("No unit offset parameters found!")
        num_of_units = rospy.get_param('~num_of_units', 0)
        self._unit_offsets = np.zeros((num_of_units, 3))
        self._unit_coefficients = np.zeros((num_of_units, 2))
        for i in xrange(num_of_units):
            unit_params = rospy.get_param('~unit_{}'.format(i))
            x = unit_params['x']
            y = unit_params['y']
            z = unit_params['z']
            self._unit_offsets[i, :] = [x, y, z]
            p0 = unit_params['p0']
            p1 = unit_params['p1']
            self._unit_coefficients[i, :] = [p0, p1]
        rospy.loginfo("Unit offsets: {}".format(self._unit_offsets))
        rospy.loginfo("Unit coefficients: {}".format(self._unit_coefficients))

    def _setup_plots(self):
        from gui_utils import MainWindow, PlotData
        self.window = MainWindow()
        self.range_plot = PlotData(self.window.addPlot(title="Ranges"), self.VISUALIZATION_DATA_LENGTH)
        self.range_plot.get_plot().addLegend()
        self.window.nextRow()
        self.clock_offset_plot = PlotData(self.window.addPlot(title="Clock offset"), self.VISUALIZATION_DATA_LENGTH)
        self.clock_offset_plot.get_plot().addLegend()
        self.window.nextRow()
        self.clock_skew_plot = PlotData(self.window.addPlot(title="Clock skew"), self.VISUALIZATION_DATA_LENGTH)
        self.clock_skew_plot.get_plot().addLegend()

    def handle_timestamps_message(self, multi_range_raw_msg):
        # Compute time-of-flight and ranges from timestamps measurements
        tofs, ranges, clock_offsets, clock_skews, slave_clock_offset, slave_clock_skew \
            = self.process_timestamps_measurements(multi_range_raw_msg, self._unit_coefficients)

        # Publish multi-range message
        if self.uwb_pub.get_num_connections() > 0:
            ros_msg = uwb.msg.UWBMultiRange()
            ros_msg.header.stamp = rospy.Time.now()
            ros_msg.num_of_units = multi_range_raw_msg.num_of_units
            ros_msg.address = multi_range_raw_msg.address
            ros_msg.remote_address = multi_range_raw_msg.remote_address
            ros_msg.tofs = tofs
            ros_msg.ranges = ranges
            self.uwb_pub.publish(ros_msg)

        if self.uwb_with_offsets_pub.get_num_connections() > 0:
            ros_msg = uwb.msg.UWBMultiRangeWithOffsets()
            ros_msg.header.stamp = rospy.Time.now()
            ros_msg.num_of_units = multi_range_raw_msg.num_of_units
            ros_msg.address = multi_range_raw_msg.address
            ros_msg.remote_address = multi_range_raw_msg.remote_address
            if ros_msg.num_of_units != self._unit_offsets.shape[0]:
                rospy.logwarn("Number of units in timestamp message does not match number of unit offsets in parameters!")
            for i in xrange(ros_msg.num_of_units):
                range_msg = uwb.msg.UWBRangeWithOffset()
                range_msg.tof = tofs[i]
                range_msg.range = ranges[i]
                range_msg.offset.x = self._unit_offsets[i, 0]
                range_msg.offset.y = self._unit_offsets[i, 1]
                range_msg.offset.z = self._unit_offsets[i, 2]
                ros_msg.ranges.append(range_msg)
            self.uwb_with_offsets_pub.publish(ros_msg)

        if self.uwb_raw_pub.get_num_connections() > 0:
            # Compute raw (without rigid configuration model) time-of-flight and ranges from timestamps measurements
            raw_tofs, raw_ranges, _, _, _, _ = self.process_timestamps_measurements(multi_range_raw_msg)

            ros_msg = uwb.msg.UWBMultiRange()
            ros_msg.header.stamp = rospy.Time.now()
            ros_msg.num_of_units = multi_range_raw_msg.num_of_units
            ros_msg.address = multi_range_raw_msg.address
            ros_msg.remote_address = multi_range_raw_msg.remote_address
            ros_msg.tofs = raw_tofs
            ros_msg.ranges = raw_ranges
            self.uwb_raw_pub.publish(ros_msg)

        # Optionally: Update plots
        if self.show_plots:
            self.update_visualization(tofs, ranges, clock_offsets, clock_skews,
                                      slave_clock_offset, slave_clock_skew)

        # Increase rate-counter
        self.msg_count += 1

        # Display rate
        now = rospy.get_time()
        if now - self.last_now >= self.INFO_PRINT_RATE:
            msg_rate = self.msg_count / (now - self.last_now)
            rospy.loginfo("Receiving UWB timestamps with rate {} Hz".format(msg_rate))
            self.last_now = now
            self.msg_count = 0

    def convert_dw_timeunits_to_microseconds(self, dw_timeunits):
        return dw_timeunits * self.DW_TIMEUNITS_TO_US

    def convert_time_of_flight_to_distance(self, tof):
        return self.SPEED_OF_LIGHT_IN_M_PER_US * tof

    def process_timestamps_measurements(self, uwb_multi_range_raw_msg, unit_coefficients=None):
        msg = uwb_multi_range_raw_msg
        num_of_units = msg.num_of_units

        # Set default distances and coefficients if not specified
        if unit_coefficients is None:
            unit_coefficients = np.zeros((num_of_units, 2))
            for i in xrange(num_of_units):
                unit_coefficients[i, :] = [0., 1.]

        tofs = []
        ranges = []
        clock_offsets = []
        clock_skews = []

        timediff_slave = msg.timestamp_master_request_1_recv + msg.timestamp_master_request_2_recv \
                         - 2 * msg.timestamp_slave_reply_send

        timediff_master = 2 * msg.timestamp_slave_reply[0] - msg.timestamp_master_request_1[0] \
                          - msg.timestamp_master_request_2[0]
        tof_master_slave = (timediff_master + timediff_slave) / 4.0
        tof_master_slave_us = self.convert_dw_timeunits_to_microseconds(tof_master_slave)
        range_master_slave = self.convert_time_of_flight_to_distance(tof_master_slave_us)
        range_master_slave = range_master_slave * unit_coefficients[0, 1] + unit_coefficients[0, 0]

        tofs.append(tof_master_slave_us)
        ranges.append(range_master_slave)
        clock_offsets.append(0.0)
        clock_skews.append(0.0)

        adjusted_processing_time_slave = (msg.timestamp_slave_reply[0] - msg.timestamp_master_request_1[0]) \
                                         - 2 * tof_master_slave

        for i in xrange(1, num_of_units):
            # Compute clock offset and skew of listener
            clock_offset, clock_skew = self.process_listener_measurement(i, msg)

            # Compute timediff from master -> slave -> listener
            rtt_master_slave_listener = \
                (msg.timestamp_slave_reply[i] - msg.timestamp_master_request_1[i]) \
                * (1 + clock_skew)

            # Compute TOF from slave -> listener
            tof_slave_listener = rtt_master_slave_listener - tof_master_slave - adjusted_processing_time_slave
            # Convert to microseconds and compute corresponding range
            tof_slave_listener_us = self.convert_dw_timeunits_to_microseconds(tof_slave_listener)
            range_slave_listener = self.convert_time_of_flight_to_distance(tof_slave_listener_us)
            range_slave_listener = range_slave_listener * unit_coefficients[i, 1] + unit_coefficients[i, 0]

            tofs.append(tof_slave_listener_us)
            ranges.append(range_slave_listener)
            clock_offsets.append(clock_offset)
            clock_skews.append(clock_skew)

        # Compute slave clock offset and skew for plotting
        slave_clock_offset, slave_clock_skew = self.process_slave_measurement(tof_master_slave, msg)

        return tofs, ranges, clock_offsets, clock_skews, slave_clock_offset, slave_clock_skew

    def process_slave_measurement(self, tof_master_slave, uwb_multi_range_raw_msg):
        msg = uwb_multi_range_raw_msg

        # clock offset
        timestamp_master = msg.timestamp_master_request_1[0]
        timestamp_slave = msg.timestamp_master_request_1_recv
        clock_offset = timestamp_slave - timestamp_master \
            - tof_master_slave

        # clock skew
        clock_diff_1 = float(msg.timestamp_master_request_2[0] - msg.timestamp_master_request_1[0])
        clock_diff_2 = float(msg.timestamp_master_request_2_recv - msg.timestamp_master_request_1_recv)
        clock_skew = (1 / clock_diff_2 - 1 / clock_diff_1) / (1 / clock_diff_1)

        return clock_offset, clock_skew

    def process_listener_measurement(self, index, uwb_multi_range_raw_msg):
        msg = uwb_multi_range_raw_msg

        # clock offset
        timestamp_master = msg.timestamp_master_request_1[0]
        timestamp_listener = msg.timestamp_master_request_1[index]
        clock_offset = timestamp_listener - timestamp_master

        # clock skew
        clock_diff_1 = float(msg.timestamp_master_request_2[0] - msg.timestamp_master_request_1[0])
        clock_diff_2 = float(msg.timestamp_master_request_2[index] - msg.timestamp_master_request_1[index])
        clock_skew = (1 / clock_diff_2 - 1 / clock_diff_1) / (1 / clock_diff_1)

        return clock_offset, clock_skew

    def update_visualization(self, tofs, ranges, clock_offsets, clock_skews, slave_clock_offset, slave_clock_skew):
        if not self.show_plots:
            return

        import pyqtgraph
        num_of_units = len(tofs)

        # ranges
        while len(self.range_plot) < num_of_units:
            curve_index = len(self.range_plot)
            pen = pyqtgraph.mkPen(curve_index)
            self.range_plot.add_curve(pen=pen, name="{}".format(curve_index))
        for i in xrange(num_of_units):
            self.range_plot.add_point(i, ranges[i])

        # clock offset
        while len(self.clock_offset_plot) < num_of_units:
            curve_index = len(self.clock_offset_plot)
            pen = pyqtgraph.mkPen(curve_index)
            self.clock_offset_plot.add_curve(pen=pen, name="{}".format(curve_index))
        if self.show_slave_clock_offset:
            if len(self.clock_offset_plot) < num_of_units + 1:
                curve_index = len(self.clock_offset_plot)
                pen = pyqtgraph.mkPen(curve_index)
                self.clock_offset_plot.add_curve(pen=pen, name="slave")
        for i in xrange(num_of_units):
            self.clock_offset_plot.add_point(i, clock_offsets[i])
        if self.show_slave_clock_offset:
            self.clock_offset_plot.add_point(len(self.clock_offset_plot) - 1, slave_clock_offset)

        # clock skew
        while len(self.clock_skew_plot) < num_of_units:
            curve_index = len(self.clock_skew_plot)
            pen = pyqtgraph.mkPen(curve_index)
            self.clock_skew_plot.add_curve(pen=pen, name="{}".format(curve_index))
        if self.show_slave_clock_skew:
            if len(self.clock_skew_plot) < num_of_units + 1:
                curve_index = len(self.clock_skew_plot)
                pen = pyqtgraph.mkPen(curve_index)
                self.clock_skew_plot.add_curve(pen=pen, name="slave")
        for i in xrange(num_of_units):
            self.clock_skew_plot.add_point(i, clock_skews[i])
        if self.show_slave_clock_skew:
            self.clock_skew_plot.add_point(len(self.clock_skew_plot) - 1, slave_clock_skew)

    def exec_(self):
        if self.show_plots:
            import sys
            import pyqtgraph
            if sys.flags.interactive != 1 or not hasattr(QtCore, 'PYQT_VERSION'):
                pyqtgraph.QtGui.QApplication.exec_()
        else:
            rospy.spin()

    def stop(self):
        if self.show_plots:
            import pyqtgraph
            pyqtgraph.QtGui.QApplication.quit()
        else:
            rospy.signal_shutdown('User request')


def main():
    import signal

    rospy.init_node('uwb_multi_range_node')
    u = UWBMultiRange()

    def sigint_handler(sig, _):
        if sig == signal.SIGINT:
            u.stop()
    signal.signal(signal.SIGINT, sigint_handler)

    try:
        u.exec_()
    except (rospy.ROSInterruptException, select.error):
        rospy.logwarn("Interrupted... Stopping.")


if __name__ == '__main__':
    main()
