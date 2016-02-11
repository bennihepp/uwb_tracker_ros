#!/usr/bin/env python

"""uwb_range.py: Streams UWB range measurements from serial port."""

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
import mavlink_bridge

class DataPlot(object):

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


class MainWindow(pyqtgraph.GraphicsWindow):

    def __init__(self, timer, *args, **kwargs):
        self.timer = timer
        super(MainWindow, self).__init__(*args, **kwargs)

    def showEvent(self, event):
        super(MainWindow, self).showEvent(event)
        self.timer.start()

    def closeEvent(self, event):
        self.timer.stop()
        super(MainWindow, self).closeEvent(event)


class UWBMultiRange(object):

    INFO_PRINT_RATE = 5

    US_TO_DW_TIMEUNITS = 128. * 499.2  # conversion between microseconds to the decawave timeunits (ca 15.65ps).
    DW_TIMEUNITS_TO_US = 1 / US_TO_DW_TIMEUNITS  # conversion between the decawave timeunits (~15.65ps) to microseconds.
    CONST_2POWER40 = 1099511627776  # Time register in DW1000 is 40 bit so this is needed to detect overflows.
    SPEED_OF_LIGHT_IN_M_PER_US = 299.792458
    SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT = SPEED_OF_LIGHT_IN_M_PER_US / US_TO_DW_TIMEUNITS

    VISUALIZATION_DATA_LENGTH = 500
    unit_distance = [0, 0.37, 0.37, 0.37]

    def __init__(self, serial_port, baud_rate, uwb_topic, uwb_raw_topic = None):
        if uwb_raw_topic is None:
            uwb_raw_topic = '{}_raw'.format(uwb_topic)
        self.ser = serial.Serial(serial_port, baud_rate, timeout=0)
        self.uwb_pub = rospy.Publisher(uwb_topic, uwb.msg.UWBMultiRange, queue_size=1)
        self.uwb_raw_pub = rospy.Publisher(uwb_raw_topic, uwb.msg.UWBMultiRangeRaw, queue_size=1)
        self.mav = mavlink_bridge.MAVLinkBridge(self.ser)
        self.mav_error_handler = None
        self.msg_count = 0
        self.last_now = rospy.get_time()
        timer = QtCore.QTimer()
        timer.timeout.connect(self.step)
        timer.setInterval(1)
        self.window = MainWindow(timer)
        self.range_plot = DataPlot(self.window.addPlot(title="Ranges"), self.VISUALIZATION_DATA_LENGTH)
        self.range_plot.get_plot().addLegend()
        self.window.nextRow()
        self.clock_offset_plot = DataPlot(self.window.addPlot(title="Clock offset"), self.VISUALIZATION_DATA_LENGTH)
        self.clock_offset_plot.get_plot().addLegend()
        self.window.nextRow()
        self.clock_skew_plot = DataPlot(self.window.addPlot(title="Clock skew"), self.VISUALIZATION_DATA_LENGTH)
        self.clock_skew_plot.get_plot().addLegend()

    def set_mavlink_error_handler(self, error_handler):
        self.mav_error_handler = error_handler

    def convert_dw_timeunits_to_microseconds(self, dw_timeunits):
        return dw_timeunits * self.DW_TIMEUNITS_TO_US

    def convert_time_of_flight_to_distance(self, tof):
        return self.SPEED_OF_LIGHT_IN_M_PER_US * tof

    def process_raw_measurements(self, uwb_multi_range_raw_msg):
        msg = uwb_multi_range_raw_msg
        num_of_units = msg.num_of_units

        tofs = []
        ranges = []
        clock_offsets = []
        clock_skews = []

        timediff_slave = msg.timestamp_master_request_1_recv + msg.timestamp_master_request_2_recv \
                         - 2 * msg.timestamp_slave_reply_send

        timediff_master = 2 * msg.timestamp_slave_reply[0] - msg.timestamp_master_request_1[0] \
                          - msg.timestamp_master_request_2[0]
        timediff_one_way = (timediff_master + timediff_slave) / 4.0
        tof = self.convert_dw_timeunits_to_microseconds(timediff_one_way)
        range = self.convert_time_of_flight_to_distance(tof)

        tofs.append(tof)
        ranges.append(range)
        clock_offsets.append(0.0)
        clock_skews.append(0.0)

        slave_clock_offset, slave_clock_skew = self.process_slave_measurement(range, msg)
        adjusted_processing_time_slave = float(msg.timestamp_slave_reply_send - msg.timestamp_master_request_1_recv) \
                                         / (1 + slave_clock_skew)
        adjusted_processing_time_slave_us = self.convert_dw_timeunits_to_microseconds(adjusted_processing_time_slave)
        adjusted_processing_time_slave_2 = (msg.timestamp_slave_reply[0] - msg.timestamp_master_request_1[0]) \
                                           - 2 * timediff_one_way

        for i in xrange(1, num_of_units):
            clock_offset, clock_skew = self.process_secondary_measurement(i, msg)

            # timediff_primary_slave_primary = (msg.timestamp_slave_reply[0] - msg.timestamp_master_request_1[0]) \
            #                     / (1)
            # timediff_slave_primary_tmp = timediff_primary_slave_primary - adjusted_processing_time_slave_2
            # timediff_slave_primary = timediff_slave_primary_tmp - timediff_one_way
            # #timediff_slave_primary = timediff_primary_slave_primary - timediff_one_way - adjusted_processing_time_slave
            # tof_primary = self.convert_dw_timeunits_to_microseconds(timediff_slave_primary)
            # range_primary = self.convert_time_of_flight_to_distance(tof_primary)

            timediff_primary_slave_secondary = (msg.timestamp_slave_reply[i] - clock_offset - msg.timestamp_master_request_1[0]) \
                                * (1 + clock_skew)
            #
            timediff_slave_secondary = timediff_primary_slave_secondary - timediff_one_way - adjusted_processing_time_slave_2
            tof_secondary = self.convert_dw_timeunits_to_microseconds(timediff_slave_secondary)
            range_secondary = self.convert_time_of_flight_to_distance(tof_secondary)

            tofs.append(tof_secondary)
            ranges.append(range_secondary)
            clock_offsets.append(clock_offset)
            clock_skews.append(clock_skew)

        return tofs, ranges, clock_offsets, clock_skews, slave_clock_offset, slave_clock_skew

    def process_slave_measurement(self, range, uwb_multi_range_raw_msg):
        msg = uwb_multi_range_raw_msg

        # clock offset
        timestamp_master = msg.timestamp_master_request_1[0]
        timestamp_slave = msg.timestamp_master_request_1_recv
        clock_offset = timestamp_slave - timestamp_master \
            - range / self.SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT

        # clock skew
        clock_diff_1 = float(msg.timestamp_master_request_2[0] - msg.timestamp_master_request_1[0])
        clock_diff_2 = float(msg.timestamp_master_request_2_recv - msg.timestamp_master_request_1_recv)
        clock_skew = (1 / clock_diff_2 - 1 / clock_diff_1) / (1 / clock_diff_1)

        return clock_offset, clock_skew

    def process_secondary_measurement(self, index, uwb_multi_range_raw_msg):
        msg = uwb_multi_range_raw_msg

        # clock offset
        timestamp_primary = msg.timestamp_master_request_1[0]
        timestamp_secondary = msg.timestamp_master_request_1[index]
        clock_offset = timestamp_secondary - timestamp_primary \
            - self.unit_distance[index] / self.SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT

        # clock skew
        clock_diff_1 = float(msg.timestamp_master_request_2[0] - msg.timestamp_master_request_1[0])
        clock_diff_2 = float(msg.timestamp_master_request_2[index] - msg.timestamp_master_request_1[index])
        clock_skew = (1 / clock_diff_2 - 1 / clock_diff_1) / (1 / clock_diff_1)

        return clock_offset, clock_skew

    def update_visualization(self, tofs, ranges, clock_offsets, clock_skews, slave_clock_offset, slave_clock_skew):
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
        if len(self.clock_offset_plot) < num_of_units + 1:
            curve_index = len(self.clock_offset_plot)
            pen = pyqtgraph.mkPen(curve_index)
            self.clock_offset_plot.add_curve(pen=pen, name="slave")
        for i in xrange(num_of_units):
            self.clock_offset_plot.add_point(i, clock_offsets[i])
        self.clock_offset_plot.add_point(len(self.clock_offset_plot) - 1, slave_clock_offset)

        # clock skew
        while len(self.clock_skew_plot) < num_of_units:
            curve_index = len(self.clock_skew_plot)
            pen = pyqtgraph.mkPen(curve_index)
            self.clock_skew_plot.add_curve(pen=pen, name="{}".format(curve_index))
        if len(self.clock_skew_plot) < num_of_units + 1:
            curve_index = len(self.clock_skew_plot)
            pen = pyqtgraph.mkPen(curve_index)
            self.clock_skew_plot.add_curve(pen=pen, name="slave")
        for i in xrange(num_of_units):
            self.clock_skew_plot.add_point(i, clock_skews[i])
        self.clock_skew_plot.add_point(len(self.clock_skew_plot) - 1, slave_clock_skew)

    def exec_(self):
        import sys
        if sys.flags.interactive != 1 or not hasattr(QtCore, 'PYQT_VERSION'):
            pyqtgraph.QtGui.QApplication.exec_()

    def run(self):
        while not rospy.is_shutdown():
            self.step(False)

    def step(self, check_rospy = True):
        if check_rospy:
            if rospy.is_shutdown():
                return False
        try:
            msg = self.mav.try_receive_message()
            if msg is not None:
                if msg.get_msgId() == mavlink_bridge.uwb.MAVLINK_MSG_ID_UWB_TRACKER_RAW_4:
                    #print("UWB multi range: {}".format(msg))

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
                    self.uwb_raw_pub.publish(ros_msg)

                    tofs, ranges, clock_offsets, clock_skews, slave_clock_offset, slave_clock_skew \
                        = self.process_raw_measurements(msg)

                    ros_msg = uwb.msg.UWBMultiRange()
                    ros_msg.header.stamp = rospy.Time.now()
                    ros_msg.num_of_units = msg.num_of_units
                    ros_msg.address = msg.address
                    ros_msg.remote_address = msg.remote_address
                    ros_msg.tofs = tofs
                    ros_msg.ranges = ranges
                    self.uwb_pub.publish(ros_msg)

                    self.update_visualization(tofs, ranges, clock_offsets, clock_skews,
                                              slave_clock_offset, slave_clock_skew)

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
    rospy.init_node('uwb_multi_range_node')

    serial_port = rospy.get_param('~serial_port', '/dev/ttyACM0')
    baud_rate = int(rospy.get_param('~baud_rate', "115200"))
    uwb_topic = rospy.get_param('~uwb_topic', '/uwb/multi_range')
    print("Reading from serial port {} with baud-rate {}".format(serial_port, baud_rate))
    print("Publishing to {}".format(uwb_topic))

    ur = UWBMultiRange(serial_port, baud_rate, uwb_topic)
    try:
        # ur.run()
        ur.exec_()
    except (rospy.ROSInterruptException, select.error):
        print("Interrupted... Stopping.")


if __name__ == '__main__':
    main()

