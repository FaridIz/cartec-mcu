#ifndef _ROS_H_
#define _ROS_H_


#include "node_handle.h"
#include "s32k148Hardware.h"


namespace ros {
  typedef ros::NodeHandle_<s32K148Hardware> NodeHandle;
}

#endif
