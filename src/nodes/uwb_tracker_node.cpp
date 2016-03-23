/*
 * Node for UWB tracker.
 *
 *  Created on: Jan 13, 2016
 *      Author: tobias.naegli@inf.ethz.ch
 */


#include <ros/ros.h>

#include "UWBTracker.h"

#include <stdio.h>


int main(int argc, char **argv)
{
  ros::init(argc, argv, "uwb_tracker");

  UWBTracker uwb;
  ros::spin();

return 0;
}
