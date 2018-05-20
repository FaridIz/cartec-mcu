#include "system.h" /* include peripheral declarations S32K148 */
#include "std_msgs/String.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int8.h"
#include "ros.h"

// Include C headers (ie, non C++ headers) in this block
extern "C" {
#include "clocks_and_modes.h"
#include "utilities.h"
#include "Scheduler.h"
#include "Steering.h"
#include "Brake.h"
#include "CruiseControl.h"
}

/* Needed for AVR to use virtual functions */
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

/* Function prototypes */
void ros_callback_ctrl_pos(const std_msgs::Float32MultiArray &msg);
void ros_callback_ctrl_vel(const std_msgs::Float32MultiArray &msg);
void cruise(void);
void brake (void);
void steering(void);
void noderos(void);

ros::NodeHandle* point_to_node;
ros::Publisher pub("", 0);

/* HIGH LEVEL CONTROL SIGNALS */
typedef enum {
	position,
	velocity
}control_mode_t;

struct u_signals_t {
	control_mode_t control_mode;
	float steering;
	float braking;
	float throttle;
	float vel_steering;
	float vel_braking;
	float vel_throttle;
};

struct u_signals_t u_signals;


#define NUMBER_OF_TASKS 4

scheduler_task_config_t tasks[NUMBER_OF_TASKS] = {
		{
				.task_callback = noderos,
				.period_ticks  = 0x02,
				.start_tick	   = 0x01
		},
		{
				.task_callback = steering,
				.period_ticks  = 2858,		// 2858*3.5us = 10.003ms
				.start_tick	   = 0x02
		},
		{
				.task_callback = brake,
				.period_ticks  = 2858,		// 2858*3.5us = 10.003ms
				.start_tick	   = 0x04
		},
		{
				.task_callback = cruise,
				.period_ticks  = 10,		// 28571*3.5us = 99.9985ms ~100ms
				.start_tick	   = 0x06
		}
};


int main(void)
{
	SOSC_init_8MHz();		/* And SOSCDIV1 & SOSCDIV2 =1: divide by 1 */
	SPLL_init_160MHz();		/* And SPLLDIV1 divide by 2; SPLLDIV2 divide by 4 */
	NormalRUNmode_80MHz();

/* ROS ========================== */
	ros::NodeHandle nh;
	ros::Subscriber<std_msgs::Float32MultiArray> sub_pos("/board_connection/control_pos", &ros_callback_ctrl_pos);
	ros::Subscriber<std_msgs::Float32MultiArray> sub_vel("/board_connection/control_vel", &ros_callback_ctrl_vel);

	// Publisher to check if the MCU is listening to pc/ros
	std_msgs::Int8 ros_speaker;
	pub.topic_ = "/mcu/active";
	pub.msg_ = &ros_speaker;

	nh.initNode();
	nh.advertise(pub);
	nh.subscribe(sub_pos);
	nh.subscribe(sub_vel);

	point_to_node = &nh;
/* End ROS ====================== */

	utilities_init();
	obd2_init();
	steering_init();
	cruisecontrol_init();
	brake_init();

	u_signals.control_mode = position;
	u_signals.steering = 0;
	u_signals.braking = 0;
	u_signals.throttle = 0;
	u_signals.vel_braking = 0;
	u_signals.vel_steering = 0;
	u_signals.vel_throttle = 0;

	scheduler_init(&tasks[0], NUMBER_OF_TASKS, 140); //140 * 25ns = 3.5us

	for(;;){

	}

	return 0;
}



void ros_callback_ctrl_pos(const std_msgs::Float32MultiArray &msg) {
	u_signals.control_mode = position;
	u_signals.steering = msg.data[0];
	u_signals.braking = msg.data[1];
	u_signals.throttle = msg.data[2];

	std_msgs::Int8 to_send;
	to_send.data = 0;
	pub.publish(&to_send);

}

void ros_callback_ctrl_vel(const std_msgs::Float32MultiArray &msg) {
	u_signals.control_mode = velocity;
	u_signals.vel_steering = msg.data[0];
	u_signals.vel_braking = msg.data[1];
	u_signals.vel_throttle = msg.data[2];

	std_msgs::Int8 to_send;
	to_send.data = 1;
	pub.publish(&to_send);
}

void cruise (void){
cruisecontrol_handler(u_signals.throttle);
}

void brake (void){
	dummy_brake();
}

void steering(void){
	steering_set_position(u_signals.steering);
}

void noderos(void){
//	point_to_node->spinOnce();
}
