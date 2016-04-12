# UWB multi-range tracking

A ROS package for tracking using _ultra-wideband_ (UWB) radios. The target needs one UWB tag and is localized by a tracker with multiple UWB radios (i.e. a robot).

The package is made up of three nodes:
- _uwb_serial_: Reads binary messages from a serial port. The corresponding UWB driver for an embedded board are provided at [https://developer.mbed.org/users/bhepp/code/AIT_UWB_Tracker/](https://developer.mbed.org/users/bhepp/code/AIT_UWB_Tracker/).
- _uwb_multi_range_: Processes the multi-range timestamps from _uwb_serial_ and publishes calibrated and uncalibrated ranges.
- _uwb_tracker_: Processes the calibrated ranges from _uwb_multi_range_ and tracks the target position using an Extended Kalman Filter. It publishes the state and covariance of the filter but also a corresponding transform.

The uncalibrated range measurements from the _uwb_multi_range_ node are used for calibration.

## Installation

Checkout the repository in your ROS _catkin_ workspace and build the workspace as usual.

## Usage

You can start all three nodes using
```
roslaunch uwb uwb.launch
```

Many parameters like the serial port, baudrate, transform frames and parameter file can be specified as arguments. See the launch file for details.
More low-level parameters can be defined via _rosparam_ (the default parameters should be fine in most cases). Check out the code of the nodes for details.

## Open issues

### Calibration

The calibration script is still missing. I will provide it as soon as possible. The calibration script will generate a _YAML_ file that contains offsets for each UWB unit
in the tracker coordinate frame as well as linear coefficients for the calibrated range-measurements. A motion-capture system is useful for generating this calibration but
it could also be measured by hand.
