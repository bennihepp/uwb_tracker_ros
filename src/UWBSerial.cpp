//
// Serial reader for multiple UWB ranging units.
//
// Created by Benjamin Hepp on 02.04.16.
// Copyright (c) 2016 Benjamin Hepp. All rights reserved.
//

#include "UWBSerial.h"

using ait::UWBSerial;

UWBSerial::UWBSerial()
        : nh_("~"), msg_counter_(0), last_info_time_(ros::Time::now())
{
  uwb_link_.registerMessageHandler(UWBSerial::handleMessageWrapper, this);

  double info_print_rate = 2.0;
  nh_.getParam("info_print_rate", info_print_rate);
  print_info_period_ = ros::Duration(info_print_rate);
  info_timer_ = nh_.createTimer(print_info_period_, &UWBSerial::printInfoCallback, this);
  info_timer_.start();

  std::string topic("/uwb/timestamps");
  nh_.getParam("topic", topic);
  ROS_INFO_STREAM("Publishing to " << topic);
  timestamps_pub_ = nh_.advertise<uwb::UWBMultiRangeTimestamps>("/uwb/timestamps", 1);

  std::string device("/dev/ttyACM0");
  int baud_rate = 115200;
  nh_.getParam("serial_port", device);
  nh_.getParam("baud_rate", baud_rate);
  ROS_INFO_STREAM("Reading from serial port " << device << " with baud-rate " << baud_rate);
  uwb_link_.start(device, baud_rate);
}

UWBSerial::~UWBSerial() {
}

void UWBSerial::handleMessage(const UWBMessage& msg) {
  const UWBMessageHeader* msg_header = dynamic_cast<const UWBMessageHeader*>(msg.getParts().front());
  ROS_DEBUG("Received UWB message type: 0x%x\n", msg_header->type);
  switch (msg_header->type) {
    case UWBMessage::UWB_MESSAGE_TYPE_NOP:
      break;
    case UWBMessage::UWB_MESSAGE_TYPE_STATUS: {
      const UWBMessageString *msg_str = dynamic_cast<const UWBMessageString *>(msg.getParts()[1]);
      ROS_WARN_STREAM("STATUS: " << msg_str->getString());
      break;
    }
    case UWBMessage::UWB_MESSAGE_TYPE_MULTI_RANGE: {
      const UWBMessageMultiRange* msg_multi_range = dynamic_cast<const UWBMessageMultiRange*>(msg.getParts()[1]);
      uwb::UWBMultiRangeTimestamps timestamps_msg;
      timestamps_msg.header.stamp = ros::Time::now();
      timestamps_msg.num_of_units = msg_multi_range->getNumOfModules();
      timestamps_msg.address = msg_multi_range->address;
      timestamps_msg.remote_address = msg_multi_range->remote_address;
      timestamps_msg.timestamp_master_request_1_recv = msg_multi_range->timestamp_master_request_1_recv;
      timestamps_msg.timestamp_slave_reply_send = msg_multi_range->timestamp_slave_reply_send;
      timestamps_msg.timestamp_master_request_2_recv = msg_multi_range->timestamp_master_request_2_recv;
      for (int i = 0; i < msg_multi_range->getNumOfModules(); ++i) {
        timestamps_msg.timestamp_master_request_1.push_back(msg_multi_range->timestamp_master_request_1[i]);
        timestamps_msg.timestamp_slave_reply.push_back(msg_multi_range->timestamp_slave_reply[i]);
        timestamps_msg.timestamp_master_request_2.push_back(msg_multi_range->timestamp_master_request_2[i]);
      }
      timestamps_pub_.publish(timestamps_msg);
      ++msg_counter_;
      break;
    }
  }
}

void UWBSerial::printInfoCallback(const ros::TimerEvent& ev) {
  ros::Time now_time = ros::Time::now();
  ros::Duration elapsed_duration = now_time - last_info_time_;
  if (elapsed_duration >= print_info_period_) {
    double msg_rate = msg_counter_ / elapsed_duration.toSec();
    ROS_INFO_STREAM("Receiving MAVLink messages with rate " << msg_rate << " Hz");
    msg_counter_ = 0;
    last_info_time_ = now_time;
  }
}

void UWBSerial::handleMessageWrapper(void* user_data, const UWBMessage& msg) {
  UWBSerial* uwb_serial = reinterpret_cast<UWBSerial*>(user_data);
  uwb_serial->handleMessage(msg);
}
