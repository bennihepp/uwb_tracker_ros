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
import scipy.stats
roslib.load_manifest('uwb')
import rospy
import tf

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

    class StateEstimate(object):
        def __init__(self, state, covariance):
            self.state = state
            self.covariance = covariance

    def __init__(self, uwb_multi_range_topic, uwb_tracker_topic, tracker_frame, target_frame, show_plots):
        # Get parameters for covariance matrices
        self.initial_position_covariance = rospy.get_param('~initial_position_covariance', 10)
        self.process_covariance_position = rospy.get_param('~process_covariance_position', 0)
        self.process_covariance_velocity = rospy.get_param('~process_covariance_velocity', 1)
        self.measurement_covariance = rospy.get_param('~measurement_covariance', 0.1 ** 2)

        # Get parameters for filter update and initial gauss-newton estimation
        self.ignore_z_position = rospy.get_param('~ignore_z_position', True)
        # The default value of 7.779 represents the 0.9 quantile of a Chi-Square distribution
        # with 4 degrees of freedom (4 UWB measurements).
        self.outlier_threshold_quantile = rospy.get_param('~outlier_threshold_quantile', 0.1)
        self.outlier_thresholds = {}
        self.ikf_iterations = rospy.get_param('~ikf_iterations', 4)
        self.initial_guess_iterations = rospy.get_param('~initial_guess_iterations', 200)
        self.initial_guess_tolerance = rospy.get_param('~initial_guess_tolerance', 1e-5)
        self.initial_guess_residuals_threshold = rospy.get_param('~initial_guess_residuals_threshold', 0.1)
        self.ikf_max_outlier_count = rospy.get_param('~ikf_max_outlier_count', 200)
        self.ikf_prev_outlier_flag = False
        self.ikf_outlier_count = 0

        self.estimates = {}
        self.estimate_times = {}

        self.show_plots = show_plots
        if show_plots:
            self._setup_plots()

        # ROS publishers and subscribers
        self.tracker_frame = tracker_frame
        self.target_frame = target_frame
        self.uwb_pub = rospy.Publisher(uwb_tracker_topic, uwb.msg.UWBTracker, queue_size=1)
        self.tf_broadcaster = tf.TransformBroadcaster()
        self.uwb_multi_range_sub = rospy.Subscriber(uwb_multi_range_topic, uwb.msg.UWBMultiRangeWithOffsets,
                                            self.handle_multi_range_message)

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
        new_estimate = self.update_estimate(multi_range_msg)
        if new_estimate is None:
            rospy.logwarn('Could not compute initial estimate: address={}, remote_address={}'.format(
                multi_range_msg.address, multi_range_msg.remote_address))
        else:
            # Publish tracker message
            ros_msg = uwb.msg.UWBTracker()
            ros_msg.header.stamp = rospy.get_rostime()
            ros_msg.address = multi_range_msg.address
            ros_msg.remote_address = multi_range_msg.remote_address
            ros_msg.state = new_estimate.state
            ros_msg.covariance = np.ravel(new_estimate.covariance)
            self.uwb_pub.publish(ros_msg)

            # Publish target transform
            self.tf_broadcaster.sendTransform(
                (new_estimate.state[0], new_estimate.state[1], new_estimate.state[2]),
                tf.transformations.quaternion_from_euler(0, 0, 0),
                rospy.get_rostime(),
                self.target_frame,
                self.tracker_frame
            )

    def initialize_estimate(self, estimate_id, initial_state):
        x = initial_state
        P = self.initial_position_covariance * np.eye(6)
        P[3:6, 3:6] = 0
        estimate = UWBTracker.StateEstimate(x, P)
        self.estimates[estimate_id] = estimate
        self.estimate_times[estimate_id] = rospy.get_time()

    def update_estimate(self, multi_range_msg):
        estimate_id = (multi_range_msg.address, multi_range_msg.remote_address)
        if estimate_id not in self.estimates:
            initial_state = self.initial_guess(multi_range_msg.ranges)
            if initial_state is None:
                return None
            self.initialize_estimate(estimate_id, initial_state)

        current_time = rospy.get_time()
        timestep = current_time - self.estimate_times[estimate_id]
        estimate = self.estimates[estimate_id]
        new_estimate, outlier_flag = self.update_filter(timestep, estimate, multi_range_msg.ranges)
        if not outlier_flag:
            self.estimates[estimate_id] = new_estimate
            self.estimate_times[estimate_id] = current_time
            if self.ikf_prev_outlier_flag:
                self.ikf_prev_outlier_flag = False
        # If too many outliers are encountered in a row the estimate is deleted.
        # This will lead to a new initial guess for the next multi-range message.
        if outlier_flag:
            if not self.ikf_prev_outlier_flag:
                self.ikf_prev_outlier_flag = True
                self.ikf_outlier_count = 0
            self.ikf_outlier_count += 1
            if self.ikf_outlier_count >= self.ikf_max_outlier_count:
                del self.estimates[estimate_id]
                rospy.loginfo('Too many outliers in a row. Resetting estimate for address={}, remote_address={}'.format(
                    multi_range_msg.address, multi_range_msg.remote_address
                ))

        # Optionally: Update plots
        if self.show_plots:
            self.update_visualization(estimate_id, new_estimate, outlier_flag)

        return new_estimate

    def _compute_measurements_and_jacobians(self, ranges, position, h, H, z):
        for j in xrange(len(ranges)):
            offset = ranges[j].offset
            offset = np.array([[offset.x], [offset.y], [offset.z]])
            # Observation
            if self.ignore_z_position:
                h[j] = np.linalg.norm(position[0:2] - offset[0:2])
            else:
                h[j] = np.linalg.norm(position - offset)

            # Jacobians
            # squared distance by position
            hs_to_x = 2 * position - 2 * offset
            # distance by squared distance
            h_to_hs = 1 / (2 * np.sqrt(h[j]))
            # distance by position
            h_to_x = h_to_hs[0] * hs_to_x
            H[j, 0:3] = h_to_x[:, 0]
            if self.ignore_z_position:
                H[j, 2] = 0
            z[j] = ranges[j].range

    def initial_guess(self, ranges):
        num_of_units = len(ranges)
        position = np.zeros((3, 1))
        H = np.zeros((num_of_units, position.size))
        z = np.zeros((num_of_units, 1))
        h = np.zeros((num_of_units, 1))
        residuals = np.zeros((num_of_units, 1))
        for i in xrange(self.initial_guess_iterations):
            self._compute_measurements_and_jacobians(ranges, position, h, H, z)
            new_residuals = z - h
            position = position + np.dot(np.linalg.lstsq(np.dot(H.T, H), H.T)[0], new_residuals)
            if np.sum((new_residuals - residuals) ** 2) < self.initial_guess_tolerance:
                break
            residuals = new_residuals
        rospy.loginfo('initial guess residuals: {}'.format(residuals))
        if np.any(np.abs(residuals) > self.initial_guess_residuals_threshold):
            # This initial guess is not good enough
            return None
        initial_state = np.zeros((6, 1))
        initial_state[0:3] = position
        return initial_state

    def _compute_process_and_covariance_matrices(self, dt):
        F = np.array(np.bmat([[np.eye(3), dt * np.eye(3)], [np.zeros((3, 3)), np.eye(3)]]))
        self.process_matrix = F
        q_p = self.process_covariance_position
        q_v = self.process_covariance_velocity
        Q = np.diag([q_p, q_p, q_p, q_v, q_v, q_v]) ** 2 * dt
        r = self.measurement_covariance
        R = r * np.eye(4)
        self.process_covariance = Q
        self.measurement_covariance = R
        return F, R, Q

    def update_filter(self, timestep, estimate, ranges):
        num_of_units = len(ranges)
        x = estimate.state
        P = estimate.covariance
        # Compute process matrix and covariance matrices
        F, R, Q = self._compute_process_and_covariance_matrices(timestep)
        rospy.logdebug('F: {}'.format(F))
        rospy.logdebug('Q: {}'.format(Q))
        rospy.logdebug('R: {}'.format(R))
        # Prediction
        x = np.dot(F, x)
        P = np.dot(F, np.dot(P, F.T)) + Q
        # Update
        n = np.copy(x)
        H = np.zeros((num_of_units, x.size))
        z = np.zeros((num_of_units, 1))
        h = np.zeros((num_of_units, 1))
        for i in xrange(self.ikf_iterations):
            new_position = n[0:3]
            self._compute_measurements_and_jacobians(ranges, new_position, h, H, z)
            res = z - h
            S = np.dot(np.dot(H, estimate.covariance), H.T) + R
            K = np.dot(estimate.covariance, np.linalg.lstsq(S.T, H)[0].T)
            mahalanobis = np.sqrt(np.dot(np.linalg.lstsq(S.T, res)[0].T, res))
            if res.size not in self.outlier_thresholds:
                self.outlier_thresholds[res.size] = scipy.stats.chi2.isf(self.outlier_threshold_quantile, res.size)
            outlier_threshold = self.outlier_thresholds[res.size]
            if mahalanobis < outlier_threshold:
                n = x + np.dot(K, (res - np.dot(H, x - n)))
                outlier_flag = False
            else:
                outlier_flag = True
        if outlier_flag:
            new_estimate = estimate
        else:
            new_state = n
            new_covariance = np.dot((np.eye(6) - np.dot(K, H)), P)
            new_estimate = UWBTracker.StateEstimate(new_state, new_covariance)
        return new_estimate, outlier_flag

    def update_visualization(self, estimate_id, estimate, outlier_flag):
        if not self.show_plots:
            return

        import pyqtgraph
        # TODO: Show any visualization or rely on RViz?
        pass

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

    rospy.init_node('uwb_tracker_node')

    show_plots = rospy.get_param('~show_plots', False)
    uwb_multi_range_topic = rospy.get_param('~multi_range_raw_topic', '/uwb/multi_range_with_offsets')
    uwb_tracker_topic = rospy.get_param('~tracker_topic', '/uwb/tracker')
    tracker_frame = rospy.get_param('~tracker_frame', 'uwb')
    target_frame = rospy.get_param('~target_frame', 'target')
    print("Receiving raw multi-range messages from: {}".format(uwb_multi_range_topic))
    print("Publishing tracker messages to {}".format(uwb_tracker_topic))
    print("Publishing tracker transform as {} -> {}".format(tracker_frame, target_frame))

    u = UWBTracker(uwb_multi_range_topic, uwb_tracker_topic, tracker_frame, target_frame, show_plots)
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
