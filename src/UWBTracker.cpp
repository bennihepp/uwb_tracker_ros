/*
 * UWBTracker.cpp
 *
 * Tracker for multiple UWB ranging units.
 *
 *  Created on: Jan 12, 2016
 *      Author: tobias.naegeli@inf.ethz.ch
 */

#include "UWBTracker.h"

// matlab codegen includes
#include "TargetEstimationNominalBatch/TargetEstimationNominalBatch.h"


UWBTracker::UWBTracker()
: nh_("~")
{
  cnt=0;

  target_estimation_params_.tracker_mean[0]=1;
  target_estimation_params_.tracker_mean[1]=1;
  target_estimation_params_.tracker_mean[2]=1;
  target_estimation_params_.m_uwb1[0]=1;
  target_estimation_params_.m_uwb1[0]=1;
  target_estimation_params_.m_uwb1[0]=1;

  target_estimation_params_.m_uwb2[0]=1;
  target_estimation_params_.m_uwb2[0]=1;
  target_estimation_params_.m_uwb2[0]=1;

  target_estimation_params_.m_uwb3[0]=1;
  target_estimation_params_.m_uwb3[0]=1;
  target_estimation_params_.m_uwb3[0]=1;

  target_estimation_params_.m_uwb4[0]=1;
  target_estimation_params_.m_uwb4[0]=1;
  target_estimation_params_.m_uwb4[0]=1;

  target_estimation_params_.UWB1p0=0;
  target_estimation_params_.UWB1p1=1;

  target_estimation_params_.UWB2p0=0;
  target_estimation_params_.UWB2p1=1;

  target_estimation_params_.UWB3p0=0;
  target_estimation_params_.UWB3p1=1;

  target_estimation_params_.UWB4p0=0;
  target_estimation_params_.UWB4p1=1;

  state_vector_pub_ = nh_.advertise<std_msgs::Float32MultiArray>("uwb_state", 10);

  uwb_multi_range_pub_ = nh_.advertise<uwb::UWBMultiRange>("/uwb/multi_range", 10);
  uwb_multi_range_sub_ = nh_.subscribe("/uwb/multi_range", 1, &UWBTracker::handle_multi_range, this);
  uwb_multi_range_raw_sub_ = nh_.subscribe("/uwb/multi_range_raw", 1, &UWBTracker::handle_multi_range_raw, this);

  // call initializer for codegened function
  TargetEstimationNominalBatch_initialize();
}

UWBTracker::~UWBTracker() {
  // call destructor for codegened function
  TargetEstimationNominalBatch_terminate();
}

// TODO
//void UWBTracker::estimate(const uwb::UWBRange& msg)
//{
//  this->cnt++;
//  printf("estimated  %d \n",cnt);
//  float rangeArray[4];
//  rangeArray[0]=1;
//  rangeArray[1]=1;
//  rangeArray[2]=1;
//  rangeArray[3]=1;
//
//  float pos_init[3];
//
//  pos_init[0]=1;
//  pos_init[1]=1;
//  pos_init[2]=1;
//
//  //float x_out[6];
//  std::vector<float> x_out;
//  x_out.resize(6);
//  TargetEstimationNominalBatch(rangeArray, &params, pos_init,  &x_out[0]);
//
//  uwb_state_msg_.data=x_out;
//  state_vector_pub_.publish(uwb_state_msg_);
//}

void UWBTracker::handle_multi_range(const uwb::UWBMultiRange& msg)
{
  ROS_DEBUG_STREAM("Received UWBMultiRange");

  float pos_init[3] = {1, 1, 1};
  const int range_array_size[2] = {1, msg.num_of_units};

  std::vector<float> x_out(6);
  TargetEstimationNominalBatch(&msg.ranges[0], range_array_size, &target_estimation_params_, pos_init,  &x_out[0]);

  uwb_state_msg_.data=x_out;
  state_vector_pub_.publish(uwb_state_msg_);
}

void UWBTracker::handle_multi_range_raw(const uwb::UWBMultiRangeRaw& msg)
{
  ROS_DEBUG_STREAM("Received UWBMultiRangeRaw");

  std::vector<float>& tofs = uwb_multi_range_msg_.tofs;
  tofs.resize(msg.num_of_units);
  std::vector<float>& ranges = uwb_multi_range_msg_.ranges;
  ranges.resize(msg.num_of_units);

  const int timestamp_array_size[2] = {1, msg.num_of_units};
  int tofs_size[2] = {1, msg.num_of_units};
  int ranges_size[2] = {1, msg.num_of_units};

  UWBMultiRangeEstimation(
      msg.num_of_units,
      msg.timestamp_master_request_1_recv,
      msg.timestamp_slave_reply_send,
      msg.timestamp_master_request_2_recv,
      &msg.timestamp_master_request_1[0],
      timestamp_array_size,
      &msg.timestamp_slave_reply[0],
      timestamp_array_size,
      &msg.timestamp_master_request_2[0],
      timestamp_array_size,
      &tofs[0],
      tofs_size,
      &ranges[0],
      ranges_size
  );

  uwb_multi_range_msg_.header.stamp = ros::Time::now();
  uwb_multi_range_msg_.num_of_units = msg.num_of_units;
  uwb_multi_range_msg_.address = msg.address;
  uwb_multi_range_msg_.remote_address = msg.remote_address;

  uwb_multi_range_pub_.publish(uwb_multi_range_msg_);
}
