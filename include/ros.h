#ifndef _ROS_H_
#define _ROS_H_


#include "../ros_serial/ros_lib/ros/node_handle.h"
#include "../ros_serial/s32k148Hardware.h"


namespace ros {
  typedef ros::NodeHandle_<s32K148Hardware> NodeHandle;
}

#endif
