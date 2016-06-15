#!/usr/bin/env python

"""uwb_tracker_node.py: Streams tracked positions based on UWB multi-range messages."""

from __future__ import print_function

__author__      = "Benjamin Hepp"
__email__ = "benjamin.hepp@inf.ethz.ch"
__copyright__   = "Copyright 2015 Benjamin Hepp"

import select
import numpy as np
import roslib
import scipy.stats
roslib.load_manifest('uwb')
import rospy
import tf

import uwb.msg


class UWBTracker(object):
    """Position tracker for ultra-wideband range measurements.

    By default the z-coordinate of the state is ignored.
    This can be modified with the ROS parameter `ignore_z_position`.
    Topics and other options can also be modified with ROS parameters (see the `_read_configuration` method).
    """

    class StateEstimate(object):
        """State estimate consisting of a state vector and a covariance matrix.
        """
        def __init__(self, state, covariance):
            """Initialize a new state estimate.

            Args:
                state (numpy.ndarray): State vector.
                covariance (numpy.ndarray): Covariance matrix.
            """
            self.state = state
            self.covariance = covariance

    def __init__(self):
        """Initialize tracker.
        """
        self._read_configuration()

        self.estimates = {}
        self.estimate_times = {}
        self.ikf_prev_outlier_flags = {}
        self.ikf_outlier_counts = {}
        self.outlier_thresholds = {}

        rospy.loginfo("Receiving raw multi-range messages from: {}".format(self.uwb_multi_range_topic))
        rospy.loginfo("Publishing tracker messages to {}".format(self.uwb_tracker_topic))
        rospy.loginfo("Publishing tracker transform as {} -> {}".format(self.tracker_frame, self.target_frame))

        # ROS publishers and subscribers
        self.tracker_frame = self.tracker_frame
        self.target_frame = self.target_frame
        self.uwb_pub = rospy.Publisher(self.uwb_tracker_topic, uwb.msg.UWBTracker, queue_size=1)
        self.tf_broadcaster = tf.TransformBroadcaster()
        self.uwb_multi_range_sub = rospy.Subscriber(self.uwb_multi_range_topic, uwb.msg.UWBMultiRangeWithOffsets,
                                                    self.handle_multi_range_message)

    def _read_configuration(self):
        """Initialize configuration from ROS parameters.
        """
        self.uwb_multi_range_topic = rospy.get_param('~multi_range_raw_topic', '/uwb/multi_range_with_offsets')
        self.uwb_tracker_topic = rospy.get_param('~tracker_topic', '/uwb/tracker')
        self.tracker_frame = rospy.get_param('~tracker_frame', 'uwb')
        self.target_frame = rospy.get_param('~target_frame', 'target')

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
        self.ikf_iterations = rospy.get_param('~ikf_iterations', 2)
        self.initial_guess_position = np.empty((3, 1), dtype=np.float)
        self.initial_guess_position[0] = rospy.get_param('~initial_guess_position_x', 0)
        self.initial_guess_position[1] = rospy.get_param('~initial_guess_position_y', 0)
        self.initial_guess_position[2] = rospy.get_param('~initial_guess_position_z', 0)
        self.initial_guess_iterations = rospy.get_param('~initial_guess_iterations', 200)
        self.initial_guess_tolerance = rospy.get_param('~initial_guess_tolerance', 1e-5)
        self.initial_guess_residuals_threshold = rospy.get_param('~initial_guess_residuals_threshold', 0.1)
        self.ikf_max_outlier_count = rospy.get_param('~ikf_max_outlier_count', 200)

    def handle_multi_range_message(self, multi_range_msg):
        """Handle a ROS multi-range message by updating and publishing the state.

        Args:
             multi_range_msg (uwb.msg.UWBMultiRangeWithOffsets): ROS multi-range message.
        """
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

            # Publish target transform (rotation is identity)
            self.tf_broadcaster.sendTransform(
                (new_estimate.state[0], new_estimate.state[1], new_estimate.state[2]),
                tf.transformations.quaternion_from_euler(0, 0, 0),
                rospy.get_rostime(),
                self.target_frame,
                self.tracker_frame
            )

    def initialize_estimate(self, estimate_id, initial_state):
        """Initialize a state estimate with identity covariance.

        The initial estimate is saved in the `self.estimates` dictionary.
        The timestamp in the `self.estimate_times` is updated.

        Args:
             estimate_id (int): ID of the tracked target.
             initial_state (int): Initial state of the estimate.

        Returns:
             X (numpy.ndarray): Solution of equation.
        """
        x = initial_state
        P = self.initial_position_covariance * np.eye(6)
        P[3:6, 3:6] = 0
        estimate = UWBTracker.StateEstimate(x, P)
        self.estimates[estimate_id] = estimate
        self.estimate_times[estimate_id] = rospy.get_time()
        self.ikf_prev_outlier_flags[estimate_id] = False
        self.ikf_outlier_counts[estimate_id] = 0

    def _solve_equation_least_squares(self, A, B):
        """Solve system of linear equations A X = B.
        Currently using Pseudo-inverse because it also allows for singular matrices.

        Args:
             A (numpy.ndarray): Left-hand side of equation.
             B (numpy.ndarray): Right-hand side of equation.

        Returns:
             X (numpy.ndarray): Solution of equation.
        """
        # Pseudo-inverse
        X = np.dot(np.linalg.pinv(A), B)
        # LU decomposition
        # lu, piv = scipy.linalg.lu_factor(A)
        # X = scipy.linalg.lu_solve((lu, piv), B)
        # Vanilla least-squares from numpy
        # X, _, _, _ = np.linalg.lstsq(A, B)
        # QR decomposition
        # Q, R, P = scipy.linalg.qr(A, mode='economic', pivoting=True)
        # # Find first zero element in R
        # out = np.where(np.diag(R) == 0)[0]
        # if out.size == 0:
        #     i = R.shape[0]
        # else:
        #     i = out[0]
        # B_prime = np.dot(Q.T, B)
        # X = np.zeros((A.shape[1], B.shape[1]), dtype=A.dtype)
        # X[P[:i], :] = scipy.linalg.solve_triangular(R[:i, :i], B_prime[:i, :])
        return X

    def _compute_measurements_and_jacobians(self, ranges, position, h, H, z):
        """Computes the predicted measurements and the jacobian of the measurement model based on the current state.

        Args:
             ranges (list of uwb.msg.UWBMultiRange): Range measurement message.
             position (numpy.ndarray): Current position state.
             h (``Output``) (numpy.ndarray): Vector for the predicted measurements.
             H (``Output``) (numpy.ndarray): Vector for the computed jacobian of the measurement model.
             z (``Output``) (numpy.ndarray): Vector for the range measurements.

        TODO:
            Could be sped up a bit using Cython
        """
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
            h_to_hs = 1 / (2 * h[j])
            # distance by position
            h_to_x = h_to_hs[0] * hs_to_x
            H[j, 0:3] = h_to_x[:, 0]
            if self.ignore_z_position:
                H[j, 2] = 0
            z[j] = ranges[j].range

    def initial_guess(self, ranges):
        """Computes an initial position guess based on range measurements.

        The initial position is computed using Gauss-Newton method.
        The behavior can be modified with some parameters: `self.initial_guess_...`.

        Args:
             ranges (list of floats): Range measurements.

        Returns:
            initial_state (numpy.ndarray): Initial state vector (velocity components are zero).
        """
        num_of_units = len(ranges)
        position = self.initial_guess_position
        H = np.zeros((num_of_units, position.size))
        z = np.zeros((num_of_units, 1))
        h = np.zeros((num_of_units, 1))
        residuals = np.zeros((num_of_units, 1))
        for i in xrange(self.initial_guess_iterations):
            self._compute_measurements_and_jacobians(ranges, position, h, H, z)
            new_residuals = z - h
            position = position + np.dot(self._solve_equation_least_squares(np.dot(H.T, H), H.T), new_residuals)
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

    def update_estimate(self, multi_range_msg):
        """Update tracker based on a multi-range message.

        Updates estimate and timestamp in the `self.estimate` and `self.estimate_times` dictionaries.

        Args:
             multi_range_msg (uwb.msg.UWBMultiRangeWithOffsets): ROS multi-range message.

        Returns:
            new_estimate (StateEstimate): Updated position estimate.
        """
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
            if self.ikf_prev_outlier_flags[estimate_id]:
                self.ikf_prev_outlier_flags[estimate_id] = False
        # If too many outliers are encountered in a row the estimate is deleted.
        # This will lead to a new initial guess for the next multi-range message.
        if outlier_flag:
            if not self.ikf_prev_outlier_flags[estimate_id]:
                self.ikf_prev_outlier_flags[estimate_id] = True
                self.ikf_outlier_counts[estimate_id] = 0
            self.ikf_outlier_counts[estimate_id] += 1
            if self.ikf_outlier_counts[estimate_id] >= self.ikf_max_outlier_count:
                del self.estimates[estimate_id]
                rospy.loginfo('Too many outliers in a row. Resetting estimate for address={}, remote_address={}'.format(
                    multi_range_msg.address, multi_range_msg.remote_address
                ))

        return new_estimate

    def _ikf_iteration(self, x, n, ranges, h, H, z, estimate, R):
        """Update tracker based on a multi-range message.

        Args:
             multi_range_msg (uwb.msg.UWBMultiRangeWithOffsets): ROS multi-range message.

        Returns:
            new_estimate (StateEstimate): Updated position estimate.
        """
        new_position = n[0:3]
        self._compute_measurements_and_jacobians(ranges, new_position, h, H, z)
        res = z - h
        S = np.dot(np.dot(H, estimate.covariance), H.T) + R
        K = np.dot(estimate.covariance, self._solve_equation_least_squares(S.T, H).T)
        mahalanobis = np.sqrt(np.dot(self._solve_equation_least_squares(S.T, res).T, res))
        if res.size not in self.outlier_thresholds:
            self.outlier_thresholds[res.size] = scipy.stats.chi2.isf(self.outlier_threshold_quantile, res.size)
        outlier_threshold = self.outlier_thresholds[res.size]
        if mahalanobis < outlier_threshold:
            n = x + np.dot(K, (res - np.dot(H, x - n)))
            outlier_flag = False
        else:
            outlier_flag = True
        return n, K, outlier_flag

    def _compute_process_and_covariance_matrices(self, dt):
        """Computes the transition and covariance matrix of the process model and measurement model.

        Args:
             dt (float): Timestep of the discrete transition.

        Returns:
            F (numpy.ndarray): Transition matrix.
            Q (numpy.ndarray): Process covariance matrix.
            R (numpy.ndarray): Measurement covariance matrix.
        """
        F = np.array(np.bmat([[np.eye(3), dt * np.eye(3)], [np.zeros((3, 3)), np.eye(3)]]))
        self.process_matrix = F
        q_p = self.process_covariance_position
        q_v = self.process_covariance_velocity
        Q = np.diag([q_p, q_p, q_p, q_v, q_v, q_v]) ** 2 * dt
        r = self.measurement_covariance
        R = r * np.eye(4)
        self.process_covariance = Q
        self.measurement_covariance = R
        return F, Q, R

    def update_filter(self, timestep, estimate, ranges):
        """Update position filter.

        Args:
             timestep (float): Time elapsed since last update.
             estimate (StateEstimate): Position estimate to update.
             ranges (list of floats): Range measurements.

        Returns:
            new_estimate (StateEstimate): Updated position estimate.
            outlier_flag (bool): Flag indicating whether the measurement was rejected as an outlier.
        """
        num_of_units = len(ranges)
        x = estimate.state
        P = estimate.covariance
        # Compute process matrix and covariance matrices
        F, Q, R = self._compute_process_and_covariance_matrices(timestep)
        # rospy.logdebug('F: {}'.format(F))
        # rospy.logdebug('Q: {}'.format(Q))
        # rospy.logdebug('R: {}'.format(R))
        # Prediction
        x = np.dot(F, x)
        P = np.dot(F, np.dot(P, F.T)) + Q
        # Update
        n = np.copy(x)
        H = np.zeros((num_of_units, x.size))
        z = np.zeros((num_of_units, 1))
        h = np.zeros((num_of_units, 1))
        for i in xrange(self.ikf_iterations):
            n, K, outlier_flag = self._ikf_iteration(x, n, ranges, h, H, z, estimate, R)
        if outlier_flag:
            new_estimate = estimate
        else:
            new_state = n
            new_covariance = np.dot((np.eye(6) - np.dot(K, H)), P)
            new_estimate = UWBTracker.StateEstimate(new_state, new_covariance)
        return new_estimate, outlier_flag

    def exec_(self):
        rospy.spin()

    def stop(self):
        rospy.signal_shutdown('User request')


def main():
    import signal

    rospy.init_node('uwb_tracker_node')
    u = UWBTracker()

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
