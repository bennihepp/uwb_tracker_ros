# UWB multi-range tracking

A ROS package for tracking using _ultra-wideband_ (UWB) radios. The target needs one UWB tag and is localized by a tracker with multiple UWB radios (i.e. a robot).

The package is made up of three nodes:
- _uwb_serial_: Reads binary messages from a serial port. The corresponding UWB driver for an embedded board are provided at [https://developer.mbed.org/users/bhepp/code/AIT_UWB_Tracker/](https://developer.mbed.org/users/bhepp/code/AIT_UWB_Tracker/).
- _uwb_multi_range_: Processes the multi-range timestamps from _uwb_serial_ and publishes calibrated and uncalibrated ranges.
- _uwb_tracker_: Processes the calibrated ranges from _uwb_multi_range_ and tracks the target position using an Extended Kalman Filter. It publishes the state and covariance of the filter but also a corresponding transform.

## Dependencies

- Boost (system and thread module)
- numpy
- scipy
- rospy

## Installation

Checkout the repository in your ROS _catkin_ workspace and build the workspace as usual.

## Usage

You can start all three nodes using
```
roslaunch uwb uwb.launch
```

Many parameters like the serial port, baudrate, transform frames and parameter file can be specified as arguments. See the launch file for details.
More low-level parameters can be defined via _rosparam_ (the default parameters should be fine in most cases). Check out the code of the nodes for details.

## Calibration

The folder _utils_ contains the MATLAB script _calibrate.m_. This generates a _YAML_ file containing offsets for each UWB unit in the tracker coordinate frame as well as linear coefficients for the calibrated range-measurements. The _uwb_multi_range_ node uses the _YAML_ file to generate calibrated range measurements.

A motion-capture system is useful for generating the calibration data but it could also be
measured by hand. The file _calibrate_example.m_ in the _utils_ folder shows how the data for _calibrate.m_ can be generated from motion-capture data.

__Note__: When recording data for calibration make sure to use the uncalibrated measurements from the _uwb_multi_range_ node (otherwise you might use already calibrated values and get a wrong calibration).
