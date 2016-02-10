/*
 * UWBTracker.h
 *
 * Simulates the functionality of the PxHawk FMU
 *
 *  Created on: Jan 12, 2016
 *      Author: nicolas
 */




#ifndef UWB_SRC_UWBTRACKER_H_
#define UWB_SRC_UWBTRACKER_H_

#include <ros/ros.h>



#include <vector>
#include <string>
#include <stdio.h>






// matlab codegen includes

#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "rtGetInf.h"
#include "rtGetNaN.h"
#include "TargetEstimationNominalBatch.h"
#include "TargetEstimationNominalBatch_types.h"
#include "std_msgs/Float32MultiArray.h"

#include <uwb/UWBRange.h>
class UWBTracker {
	ros::NodeHandle nh_;
	int cnt;
	 Params params;
	 ros::Subscriber uwb_range_sub_;
	//ros::Subscriber pose_sub_;
	//ros::Subscriber force_sub_;
	ros::Publisher StateVector_pub_;

	std_msgs::Float32MultiArray uwb_state_msg_;
	//bool odometry_available_;

	//bool kumar_variant_;
	//std::vector<double> K_rate_;
	//std::vector<double> K_attitude_;

	//std::vector<double> u_mot_;

	//ros::Time last_force_msg_;

	//dynamic_reconfigure::Server<mav_control::px_simConfig> dynamic_reconfigure_server_;

public:
	UWBTracker();
	virtual ~UWBTracker();
	void hello();
	void estimate(const uwb::UWBRange& msg);
	//void poseCb(const nav_msgs::Odometry& msg);
//#ifdef MATLAB_COMPATIBLE
//	void forceCb(const std_msgs::Float64MultiArray& msg);
//#else
//	void forceCb(const mavros::ControlSetpoint& msg);
//#endif
//	void dynamicReconfigureCb(mav_control::px_simConfig &config, uint32_t level);
};

#endif /* UWB_SRC_UWBTRACKER_H_ */
