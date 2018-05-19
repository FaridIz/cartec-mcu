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
void ros_callback_ctrl(const std_msgs::Float32MultiArray &msg);
void cruise(void);
void brake (void);
void steering(void);
void noderos(void);

ros::NodeHandle* point_to_node;
ros::Publisher pub("", 0);

int32_t pos = 0;
float32_t control_reference = 0;
uint8_t obd_flag = 0;
float tps_value = 0;


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

/* ROS ==================================================================================================== */
	ros::NodeHandle nh;
	ros::Subscriber<std_msgs::Float32MultiArray> sub("/board_connection/control_array", &ros_callback_ctrl);

	std_msgs::Int8 ros_speaker;
	pub.topic_ = "/mcu/active";
	pub.msg_ = &ros_speaker;

	nh.initNode();
	nh.advertise(pub);
	nh.subscribe(sub);

	point_to_node = &nh;
/* End ROS ================================================================================================ */

	utilities_init();
	obd2_init();
	steering_init();
	cruisecontrol_init();
	brake_init();

	GPIO_clearPin(LED_RED);
	GPIO_clearPin(LED_BLUE);

	scheduler_init(&tasks[0], NUMBER_OF_TASKS, 140); //140 * 25ns = 3.5us

//	float dummy_tps = 0;
//	float de = 2;
//	obd2_request_PID(PID_TPS);
	for(;;){
//		if(obd2_readable() == 1){
//			GPIO_clearPin(LED_RED);
//			GPIO_setPin(LED_BLUE);
//			obd2_read_PID(PID_TPS, &dummy_tps);
//			obd2_request_PID(PID_TPS);
//		}
//		else{
//			GPIO_clearPin(LED_BLUE);
//			GPIO_setPin(LED_RED);
//		}
//		delay(de);
	}

	return 0;
}



void ros_callback_ctrl(const std_msgs::Float32MultiArray &msg) {
	control_reference = msg.data[0];

	std_msgs::Int8 to_send;
	to_send.data = 1;
	pub.publish(&to_send);
}


void cruise (void){
	if(obd2_readable() == 1){
//		float tps_value = 0;
		obd2_read_PID(PID_TPS, &tps_value);
		cruisecontrol_set_position(tps_value, 20);
		obd2_request_PID(PID_TPS);
	}
	else if(obd_flag == 0){
		obd_flag = 1;
		obd2_request_PID(PID_TPS);
	}
}

void brake (void){
	dummy_brake();
}

void steering(void){
	steering_set_position(700);
}

void noderos(void){
//	point_to_node->spinOnce();
}
