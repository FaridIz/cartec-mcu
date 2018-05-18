#include "system.h" /* include peripheral declarations S32K148 */
#include "std_msgs/String.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int8.h"
#include "ros.h"

// Include C headers (ie, non C++ headers) in this block
extern "C" {
#include "utilities.h"
#include "clocks_and_modes.h"
#include "Scheduler.h"
#include "Steering.h"
#include <Brake.h>
#include "CruiseControl.h"
}

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

// Function prototypes
void ros_callback_ctrl(const std_msgs::Float32MultiArray &msg);

ros::NodeHandle* point_to_node;
ros::Publisher pub("", 0);


int32_t pos = 0;
float32_t control_reference = 0;


void rojo(void){
	GPIO_togglePin(LED_RED);
}

void azul(void){
	GPIO_togglePin(LED_BLUE);
}

void verde(void){
	GPIO_togglePin(LED_GREEN);
	pos = steering_encoder_read_deg();
}

void steering(void){
	steering_set_position(control_reference);
}

void noderos(void){
//	point_to_node->spinOnce();
}

#define NUMBER_OF_TASKS 3

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
				.task_callback = verde,
				.period_ticks  = 250,
				.start_tick	   = 503
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


//	scheduler_init(&tasks[0], NUMBER_OF_TASKS, 140); //140 * 25ns = 3.5us
//	stopwatch();

	float dummy_TPS = 0;
	uint32_t count = 0;
	uint32_t t0 = 0;
	uint32_t t = 0;
	uint8_t set_point = 40;

	tps_temp = 0;

	for(;;){
//		cruisecontrol_dummy();
//		steering_manual_ctrl();
//		stopwatch();
//		t0 = LPIT0->TMR[1].CVAL;
//		obd2_readPID(PID_TPS, &dummy_TPS);
//		t = (t0 - LPIT0->TMR[1].CVAL)*1000/40000000;
//		count++;

		cruisecontrol_set_position(set_point);
//		cruisecontrol_dummy();
	}

	return 0;
}

void ros_callback_ctrl(const std_msgs::Float32MultiArray &msg) {
	control_reference = msg.data[0];

	std_msgs::Int8 to_send;
	to_send.data = 1;
	pub.publish(&to_send);
}

