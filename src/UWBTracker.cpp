/*
 * UWBTracker.cpp
 *
 * Tracker for multiple UWB ranging units.
 *
 *  Created on: Jan 12, 2016
 *      Author: tobias.naegeli@inf.ethz.ch
 */

#include <stdexcept>
#include <vector>
#include <sstream>
#include <cmath>

// matlab codegen includes
#include "MultiRangeEstimation/MultiRangeEstimation.h"

#include "UWBTracker.h"


UWBTracker::UWBTracker()
: nh_("~")
{
  loadTargetEstimationParameters();
  loadMultiRangeEstimationParameters();

  state_vector_pub_ = nh_.advertise<std_msgs::Float32MultiArray>("/uwb/tracker_state", 10);
  uwb_multi_range_pub_ = nh_.advertise<uwb::UWBMultiRange>("/uwb/multi_range", 10);

  uwb_multi_range_sub_ = nh_.subscribe("/uwb/multi_range", 1, &UWBTracker::handleMultiRange, this);
  uwb_multi_range_raw_sub_ = nh_.subscribe("/uwb/multi_range_raw", 1, &UWBTracker::handleMultiRangeRaw, this);

  // Call initializer for Matlab codegened function
  MultiRangeEstimation_initialize();
}

UWBTracker::~UWBTracker() {
  // Call destructor for Matlab codegened function
  MultiRangeEstimation_terminate();
}

std::vector<UWBTracker::UnitParameter> UWBTracker::getUnitParameters() {
  int num_of_units;
  if (!nh_.getParam("num_of_units", num_of_units)) {
    throw std::runtime_error("Parameter 'num_of_units' is required but was not set");
  }

  // Read unit model
  std::vector<UnitParameter> unit_parameters;
  for (int i = 0; i < num_of_units; ++i) {
    std::ostringstream unit_name;
    unit_name << "unit_" << i;
    std::map<std::string, scalar_type> param_map;
    if (!nh_.getParam(unit_name.str(), param_map)) {
      std::ostringstream error_msg;
      error_msg << "Parameter '" << unit_name.str() << "' is required but was not set";
      throw std::runtime_error(error_msg.str());
    }
    assert(param_map.find("x") != param_map.end());
    assert(param_map.find("y") != param_map.end());
    assert(param_map.find("z") != param_map.end());
    assert(param_map.find("p0") != param_map.end());
    assert(param_map.find("p1") != param_map.end());
    UnitParameter unit_parameter;
    unit_parameter.x = param_map["x"];
    unit_parameter.y = param_map["y"];
    unit_parameter.z = param_map["z"];
    unit_parameter.p0 = param_map["p0"];
    unit_parameter.p1 = param_map["p1"];
    unit_parameters.push_back(unit_parameter);
  }
  return unit_parameters;
}

void UWBTracker::loadTargetEstimationParameters()
{
  std::vector<UnitParameter> unit_parameters = UWBTracker::getUnitParameters();

  target_estimation_params_.m_uwb1[0] = unit_parameters[0].x;
  target_estimation_params_.m_uwb1[1] = unit_parameters[0].y;
  target_estimation_params_.m_uwb1[2] = unit_parameters[0].z;

  target_estimation_params_.m_uwb2[0] = unit_parameters[1].x;
  target_estimation_params_.m_uwb2[1] = unit_parameters[1].y;
  target_estimation_params_.m_uwb2[2] = unit_parameters[1].z;

  target_estimation_params_.m_uwb3[0] = unit_parameters[2].x;
  target_estimation_params_.m_uwb3[1] = unit_parameters[2].y;
  target_estimation_params_.m_uwb3[2] = unit_parameters[2].z;

  target_estimation_params_.m_uwb4[0] = unit_parameters[3].x;
  target_estimation_params_.m_uwb4[0] = unit_parameters[3].y;
  target_estimation_params_.m_uwb4[0] = unit_parameters[3].z;

  target_estimation_params_.UWB1p0 = unit_parameters[0].p0;
  target_estimation_params_.UWB1p1 = unit_parameters[0].p1;

  target_estimation_params_.UWB2p0 = unit_parameters[1].p0;
  target_estimation_params_.UWB2p1 = unit_parameters[1].p1;

  target_estimation_params_.UWB3p0 = unit_parameters[2].p0;
  target_estimation_params_.UWB3p1 = unit_parameters[2].p1;

  target_estimation_params_.UWB4p0 = unit_parameters[3].p0;
  target_estimation_params_.UWB4p1 = unit_parameters[3].p1;

  // TODO: Remove
//  target_estimation_params_.tracker_mean[0]=1;
//  target_estimation_params_.tracker_mean[1]=1;
//  target_estimation_params_.tracker_mean[2]=1;
}

void UWBTracker::loadMultiRangeEstimationParameters()
{
  // Set constants
  scalar_type US_TO_DW_TIMEUNITS = 128. * 499.2;
  scalar_type DW_TIMEUNITS_TO_US = 1 / US_TO_DW_TIMEUNITS;
  scalar_type SPEED_OF_LIGHT_IN_M_PER_US = 299.792458;
  scalar_type SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT = SPEED_OF_LIGHT_IN_M_PER_US / US_TO_DW_TIMEUNITS;

  multi_range_estimation_params_.US_TO_DW_TIMEUNITS = US_TO_DW_TIMEUNITS;
  multi_range_estimation_params_.DW_TIMEUNITS_TO_US = DW_TIMEUNITS_TO_US;
  multi_range_estimation_params_.SPEED_OF_LIGHT_IN_M_PER_US = SPEED_OF_LIGHT_IN_M_PER_US;
  multi_range_estimation_params_.SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT = SPEED_OF_LIGHT_IN_M_PER_DW_TIMEUNIT;

  std::vector<UnitParameter> unit_parameters = UWBTracker::getUnitParameters();
  // Compute distances of secondary units (> 0) to master unit (0)
  multi_range_estimation_params_.unit_distances.size[0] = 1;
  multi_range_estimation_params_.unit_distances.size[1] = unit_parameters.size();
  multi_range_estimation_params_.unit_distances.data[0] = 0;
  for (int i = 0; i < unit_parameters.size(); ++i) {
    scalar_type dx = unit_parameters[i].x - unit_parameters[0].x;
    scalar_type dy = unit_parameters[i].y - unit_parameters[0].y;
    scalar_type dz = unit_parameters[i].z - unit_parameters[0].z;
    scalar_type distance = std::sqrt(dx*dx + dy*dy + dz*dz);
    multi_range_estimation_params_.unit_distances.data[i] = std::sqrt(distance);
  }
}

void UWBTracker::handleMultiRange(const uwb::UWBMultiRange& msg)
{
  ROS_DEBUG_STREAM("Received UWBMultiRange");
  assert(msg.num_of_units == multi_range_estimation_params_.unit_distances.size[1]);

  // TODO: Initial position for Matlab code
  scalar_type pos_init[3] = {1, 1, 1};

  // Convert UWBMultiRange message arrays to double types
  std::vector<scalar_type> ranges(msg.ranges.begin(), msg.ranges.end());
  const int ranges_array_size[2] = {1, msg.num_of_units};

  // Output from Matlab code
  std::vector<scalar_type> x_out(6);

  // Compute target estimate with Matlab code
  TargetEstimationNominalBatch(&ranges[0], ranges_array_size, &target_estimation_params_, pos_init,  &x_out[0]);

  // Initialize UWB state message from output and publish
  uwb_state_msg_.data.clear();
  uwb_state_msg_.data.insert(uwb_state_msg_.data.end(), x_out.begin(), x_out.end());
  state_vector_pub_.publish(uwb_state_msg_);
}

void UWBTracker::handleMultiRangeRaw(const uwb::UWBMultiRangeRaw& msg)
{
  ROS_DEBUG_STREAM("Received UWBMultiRangeRaw");
  assert(msg.num_of_units == multi_range_estimation_params_.unit_distances.size[1]);

  // Convert UWBMultiRangeRaw message arrays to double types
  const int timestamp_array_size[2] = {1, msg.num_of_units};
  std::vector<scalar_type> timestamp_master_request_1(msg.timestamp_master_request_1.begin(), msg.timestamp_master_request_1.end());
  std::vector<scalar_type> timestamp_slave_reply(msg.timestamp_slave_reply.begin(), msg.timestamp_slave_reply.end());
  std::vector<scalar_type> timestamp_master_request_2(msg.timestamp_master_request_2.begin(), msg.timestamp_master_request_2.end());

  // Output arrays from Matlab code
  std::vector<scalar_type> tofs(msg.num_of_units);
  tofs.resize(msg.num_of_units);
  std::vector<scalar_type> ranges(msg.num_of_units);
  ranges.resize(msg.num_of_units);
  std::vector<scalar_type> clock_offsets(msg.num_of_units);
  clock_offsets.resize(msg.num_of_units + 1);
  std::vector<scalar_type> clock_skews(msg.num_of_units);
  clock_skews.resize(msg.num_of_units + 1);
  // Output sizes from Matlab code
  int tofs_size[2] = {1, msg.num_of_units};
  int ranges_size[2] = {1, msg.num_of_units};
  int clock_offsets_size[2] = {1, msg.num_of_units};
  int clock_skews_size[2] = {1, msg.num_of_units};

  // Compute multi range estimate with Matlab code
  MultiRangeEstimation(
          msg.timestamp_master_request_1_recv,
          msg.timestamp_slave_reply_send,
          msg.timestamp_master_request_2_recv,
          &timestamp_master_request_1[0], timestamp_array_size,
          &timestamp_slave_reply[0], timestamp_array_size,
          &timestamp_master_request_2[0], timestamp_array_size,
          &multi_range_estimation_params_,
          &tofs[0], tofs_size,
          &ranges[0], ranges_size,
          &clock_offsets[0], clock_offsets_size,
          &clock_skews[0], clock_skews_size
  );
  ROS_DEBUG_STREAM("  output size: " << tofs_size[0] << ", " << tofs_size[1]);
  for (int i = 0; i < tofs.size(); ++i) {
    ROS_DEBUG_STREAM("  tof[" << i << "]: " << tofs[i]);
    ROS_DEBUG_STREAM("  range[" << i << "]: " << ranges[i]);
  }

  // TODO: Forward clock offsets and skew???

  // Initialize UWBMultiRange message from output and publish
  uwb_multi_range_msg_.header.stamp = ros::Time::now();
  uwb_multi_range_msg_.num_of_units = msg.num_of_units;
  uwb_multi_range_msg_.address = msg.address;
  uwb_multi_range_msg_.remote_address = msg.remote_address;

  uwb_multi_range_msg_.tofs.clear();
  uwb_multi_range_msg_.tofs.insert(uwb_multi_range_msg_.tofs.end(), tofs.begin(), tofs.end());
  uwb_multi_range_msg_.ranges.clear();
  uwb_multi_range_msg_.ranges.insert(uwb_multi_range_msg_.ranges.end(), ranges.begin(), ranges.end());

  uwb_multi_range_pub_.publish(uwb_multi_range_msg_);
}
