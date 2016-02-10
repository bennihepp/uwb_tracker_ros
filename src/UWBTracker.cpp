/*
 * PxSim.cpp
 *
 *  Created on: Jan 12, 2016
 *      Author: nicolas
 */

#include "UWBTracker.h"


UWBTracker::UWBTracker()
: nh_("~")
//  odometry_available_(false),
//  kumar_variant_(true),
//  last_force_msg_(ros::Time::now())
{
	cnt=0;

	params.tracker_mean[0]=1;
	params.tracker_mean[1]=1;
	params.tracker_mean[2]=1;
	params.m_uwb1[0]=1;
	params.m_uwb1[0]=1;
	params.m_uwb1[0]=1;

	params.m_uwb2[0]=1;
	params.m_uwb2[0]=1;
	params.m_uwb2[0]=1;

	params.m_uwb3[0]=1;
	params.m_uwb3[0]=1;
	params.m_uwb3[0]=1;

	params.m_uwb4[0]=1;
	params.m_uwb4[0]=1;
	params.m_uwb4[0]=1;

	params.UWB1p0=0;
	params.UWB1p1=1;

	params.UWB2p0=0;
	params.UWB2p1=1;

	params.UWB3p0=0;
	params.UWB3p1=1;

	params.UWB4p0=0;
	params.UWB4p1=1;


//	u_mot_.resize(4);
//
//	std::string drone_name;
//	nh_.param<std::string>("drone_name", drone_name, "default_drone_name");
//
//#ifdef ROTORS_SIMULATOR_INSTALLED // if its not installed any instance of PxSim will error
//	act_pub_ = nh_.advertise<mav_msgs::Actuators>("/" + drone_name + "/command/motor_speed", 1);
//#endif
//
	uwb_range_sub_ = nh_.subscribe("/uwb/range", 10, &UWBTracker::estimate, this);
//
	StateVector_pub_ = nh_.advertise<std_msgs::Float32MultiArray>("uwb_state", 10);
//
//	// dynamic reconfigure server for attitude controller gains. This also reads the gains from the launch file
//	dynamic_reconfigure::Server<mav_control::px_simConfig>::CallbackType f = boost::bind(&PxSim::dynamicReconfigureCb, this, _1, _2);
//	dynamic_reconfigure_server_.setCallback(f);
//
//	// call initializer for codegened function
	TargetEstimationNominalBatch_initialize();
printf("constructed");
}

UWBTracker::~UWBTracker() {
//	// call destructor for codegened function
	TargetEstimationNominalBatch_terminate();
}
void UWBTracker::hello(){
	this->cnt++;
	printf("hallo  %d \n",cnt);
}
void UWBTracker::estimate(const uwb::UWBRange& msg)
{
	this->cnt++;
	printf("estimated  %d \n",cnt);
	 float rangeArray[4];
	rangeArray[0]=1;
	rangeArray[1]=1;
	rangeArray[2]=1;
	rangeArray[3]=1;

	float pos_init[3];

	pos_init[0]=1;
	pos_init[1]=1;
	pos_init[2]=1;

	//float x_out[6];
	std::vector<float> x_out;
	x_out.resize(6);
	TargetEstimationNominalBatch(rangeArray,
	  &params, pos_init,  &x_out[0]);


	uwb_state_msg_.data=x_out;
	StateVector_pub_.publish(uwb_state_msg_);
}

//void PxSim::poseCb(const nav_msgs::Odometry& msg)
//{
//	last_odometry_msg_ = msg;
//	odometry_available_ = true;
//}
//#ifdef MATLAB_COMPATIBLE
//void PxSim::forceCb(const std_msgs::Float64MultiArray& msg)
//#else
//void PxSim::forceCb(const mavros::ControlSetpoint& msg)
//#endif
//{
//	if (!odometry_available_)
//		return;
//
//	std::vector<double> q_BW;
//	q_BW.push_back(-last_odometry_msg_.pose.pose.orientation.x);
//	q_BW.push_back(-last_odometry_msg_.pose.pose.orientation.y);
//	q_BW.push_back(-last_odometry_msg_.pose.pose.orientation.z);
//	q_BW.push_back( last_odometry_msg_.pose.pose.orientation.w);
//
//	std::vector<double> omega;
//	omega.push_back( last_odometry_msg_.twist.twist.angular.x);
//	omega.push_back( last_odometry_msg_.twist.twist.angular.y);
//	omega.push_back( last_odometry_msg_.twist.twist.angular.z);
//
//	std::vector<double> F_des;
//
//#ifdef MATLAB_COMPATIBLE
//	F_des.push_back(-msg.data[0]);
//	F_des.push_back(+msg.data[1]);
//	F_des.push_back(-msg.data[2]);
//	attitudeControl(&K_rate_[0], &K_attitude_[0], &q_BW[0], msg.data[3], &F_des[0], &omega[0], kumar_variant_, &u_mot_[0]);
//#else
//	F_des.push_back(-msg.x);
//	F_des.push_back(+msg.y);
//	F_des.push_back(-msg.z);
//	attitudeControl(&K_rate_[0], &K_attitude_[0], &q_BW[0], msg.yaw, &F_des[0], &omega[0], kumar_variant_, &u_mot_[0]);
//#endif
//
//#ifdef ROTORS_SIMULATOR_INSTALLED
//	mav_msgs::Actuators act_msg;
//	act_msg.angular_velocities = u_mot_;
//	act_pub_.publish(act_msg);
//#endif
//}
//
//void PxSim::dynamicReconfigureCb(mav_control::px_simConfig &config, uint32_t level)
//{
//	//	ROS_INFO("Reconfigure Request: %f %f %f %f",
//	//			config.K_attitude_x_y,
//	//			config.K_attitude_z,
//	//			config.K_rate_x_y,
//	//			config.K_rate_z);
//
//	K_rate_.resize(3);
//	K_rate_[0] = config.K_rate_x_y;
//	K_rate_[1] = config.K_rate_x_y;
//	K_rate_[2] = config.K_rate_z;
//
//	K_attitude_.resize(3);
//	K_attitude_[0] = config.K_attitude_x_y;
//	K_attitude_[1] = config.K_attitude_x_y;
//	K_attitude_[2] = config.K_attitude_z;
//}
