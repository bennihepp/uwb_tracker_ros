/*
 * UWBTracker.h
 *
 * Tracker for multiple UWB ranging units.
 *
 *  Created on: Jan 12, 2016
 *      Author: tobias.naegeli@inf.ethz.ch
 */

#pragma once

#include <ros/ros.h>

#include "std_msgs/Float32MultiArray.h"
#include <uwb/UWBRange.h>
#include <uwb/UWBMultiRange.h>
#include <uwb/UWBMultiRangeRaw.h>

// Matlab codegen includes
#include "MultiRangeEstimation/MultiRangeEstimation_types.h"

class UWBTracker
{
public:
  UWBTracker();
  virtual ~UWBTracker();

  void handleMultiRange(const uwb::UWBMultiRange& msg);
  void handleMultiRangeRaw(const uwb::UWBMultiRangeRaw& msg);

private:
  typedef double scalar_type;

  void initializeMultiRangeEstimationParameters();

  ros::NodeHandle nh_;

  TargetEstimationParams target_estimation_params_;
  MultiRangeEstimationParams multi_range_estimation_params_;

  ros::Subscriber uwb_range_sub_;
  ros::Subscriber uwb_multi_range_sub_;
  ros::Subscriber uwb_multi_range_raw_sub_;

  ros::Publisher state_vector_pub_;
  ros::Publisher uwb_multi_range_pub_;

  uwb::UWBMultiRange uwb_multi_range_msg_;
  std_msgs::Float32MultiArray uwb_state_msg_;
};
