//
// Serial reader for multiple UWB ranging units.
//
// Created by Benjamin Hepp on 02.04.16.
// Copyright (c) 2016 Benjamin Hepp. All rights reserved.
//

#pragma once

#include <ros/ros.h>

#include <uwb/UWBMultiRangeTimestamps.h>

#include <ait_link/uwb_link/uwb_link_impl.h>


namespace ait {

  class UWBSerial {
  public:
    UWBSerial();

    virtual ~UWBSerial();

  private:
    void handleMessage(const UWBMessage& msg);
    static void handleMessageWrapper(void* user_data, const UWBMessage& msg);

    void printInfoCallback(const ros::TimerEvent& ev);

    ros::NodeHandle nh_;
    ros::Publisher timestamps_pub_;
    Serial serial_;
    UWBLinkImpl uwb_link_;
    ros::Timer info_timer_;
    int msg_counter_;
    ros::Time last_info_time_;
    ros::Duration print_info_period_;
  };

}
