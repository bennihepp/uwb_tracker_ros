//
// Node for reading UWB multi-ranges from serial port.
//
// Created by Benjamin Hepp on 02.04.16.
// Copyright (c) 2016 Benjamin Hepp. All rights reserved.
//


#include <stdio.h>
#include <ros/ros.h>

#include "UWBSerial.h"

using ait::UWBSerial;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "uwb_serial");

  try {
    UWBSerial uwb_serial;
    ros::spin();
  } catch (const std::exception& exc) {
    ROS_ERROR_STREAM("Error occured: " << exc.what());
  }

  return 0;
}
