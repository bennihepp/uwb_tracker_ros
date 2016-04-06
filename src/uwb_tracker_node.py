#!/usr/bin/env python

"""uwb_tracker_node.py: Streams tracked positions based on UWB multi-range messages."""

__author__      = "Benjamin Hepp"
__email__ = "benjamin.hepp@inf.ethz.ch"
__copyright__   = "Copyright 2015 Benjamin Hepp"

import select
import sys
import numpy as np
import serial
import roslib
roslib.load_manifest('uwb')
import rospy
import pyqtgraph
from pyqtgraph.Qt import QtGui, QtCore

import uwb.msg


class PlotData(object):

    def __init__(self, plot, max_data_length=None):
        self.plot = plot
        self.curves = []
        self.data = []
        self.max_data_length = max_data_length

    def add_curve(self, pen, initial_data=None, **kwargs):
        self.curves.append(self.plot.plot(pen=pen, **kwargs))
        if initial_data is None:
            if self.max_data_length is None:
                initial_data = []
            else:
                initial_data = np.zeros((self.max_data_length,))
        self.data.append(initial_data)

    def add_point(self, index, value):
        assert(index < len(self.curves))
        if self.max_data_length is None:
            self.data[index].append(value)
        else:
            self.data[index][:-1] = self.data[index][1:]
            self.data[index][-1] = value
            if len(self.data[index]) > self.max_data_length:
                self.data[index] = self.data[index][-self.max_data_length:len(self.data[index])]
        self.curves[index].setData(self.data[index])

    def get_plot(self):
        return self.plot

    def __len__(self):
        return len(self.curves)


class UWBTracker(object):

    VISUALIZATION_DATA_LENGTH = 500

    def __init__(self, uwb_tracker_topic, uwb_multi_range_raw_topic, show_plots):
        # ROS Publishers
        self.uwb_pub = rospy.Publisher(uwb_tracker_topic, uwb.msg.UWBTracker, queue_size=1)
        self.uwb_raw_sub = rospy.Subscriber(uwb_multi_range_raw_topic, uwb.msg.UWBMultiRange,
                                            self.handle_multi_range_message)

        # TODO: Read parameters and initial state
        self.states = {}

        self.show_plots = show_plots
        if show_plots:
            self._setup_plots()

    def _setup_plots(self):
        from gui_utils import MainWindow
        self.window = MainWindow()
        self.range_plot = PlotData(self.window.addPlot(title="Ranges"), self.VISUALIZATION_DATA_LENGTH)
        self.range_plot.get_plot().addLegend()
        self.window.nextRow()
        self.clock_offset_plot = PlotData(self.window.addPlot(title="Clock offset"), self.VISUALIZATION_DATA_LENGTH)
        self.clock_offset_plot.get_plot().addLegend()
        self.window.nextRow()
        self.clock_skew_plot = PlotData(self.window.addPlot(title="Clock skew"), self.VISUALIZATION_DATA_LENGTH)
        self.clock_skew_plot.get_plot().addLegend()

    def handle_multi_range_message(self, multi_range_msg):
        # Update tracker position based on time-of-flight measurements
        new_state = self.update_state(multi_range_msg)

        # Publish tracker message
        ros_msg = uwb.msg.UWBTracker()
        ros_msg.header.stamp = rospy.Time.now()
        ros_msg.address = multi_range_msg.address
        ros_msg.remote_address = multi_range_msg.remote_address
        ros_msg.state = new_state
        self.uwb_pub.publish(ros_msg)

        # Optionally: Update plots
        if self.show_plots:
            self.update_visualization()

    def initialize_state(self, state_id):
        # TODO: Set initial state estimate
        self.states[state_id] = 0

    def update_state(self, multi_range_msg):
        state_id = (multi_range_msg.address, multi_range_msg.remote_address)
        if state_id not in self.states:
            self.initialize_state(state_id)

        # TODO: Update state
        state = self.states[state_id]
        new_state = state
        self.states[state_id] = new_state
        return new_state

    def update_visualization(self, tofs, ranges, clock_offsets, clock_skews, slave_clock_offset, slave_clock_skew):
        if not self.show_plots:
            return

        import pyqtgraph
        pass

    def exec_(self):
        if self.show_plots:
            import sys
            import pyqtgraph
            if sys.flags.interactive != 1 or not hasattr(QtCore, 'PYQT_VERSION'):
                pyqtgraph.QtGui.QApplication.exec_()
        else:
            rospy.spin()


def main():
    rospy.init_node('uwb_tracker_node')

    show_plots = rospy.get_param('~show_plots', True)
    uwb_multi_range_raw_topic = rospy.get_param('~multi_range_raw_topic', '/uwb/multi_range_raw')
    uwb_tracker_topic = rospy.get_param('~tracker_topic', '/uwb/tracker')
    print("Receiving raw multi-range messages from: {}".format(uwb_multi_range_raw_topic))
    print("Publishing tracker messages to {}".format(uwb_tracker_topic))

    u = UWBTracker(uwb_tracker_topic, uwb_multi_range_raw_topic, show_plots)
    try:
        u.exec_()
    except (rospy.ROSInterruptException, select.error):
        print("Interrupted... Stopping.")


if __name__ == '__main__':
    main()
