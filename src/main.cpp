
#include "system.h" /* include peripheral declarations S32K148 */
#include "std_msgs/String.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int8.h"
#include "ros.h"

// Include C headers (ie, non C++ headers) in this block
extern "C" {
#include "clocks_and_modes.h"
#include "Steering.h"
}

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

// Function prototypes
void ros_callback_ctrl(const std_msgs::Float32MultiArray &msg);

ros::Publisher pub("", 0);
// Subscribers are declared in the main scope due to a compiler bug

/* ================================================================================================= */

void LPIT0_init (void) {
  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs */
  LPIT0->MCR = 0x00000001;	/* M_CEN=1: enable module clk (allows writing other LPIT0 regs)*/
}

void LPIT0_chn0_init(uint32_t scheduler_ticks){
	LPIT0->MIER=LPIT_MIER_TIE0_MASK;  		/* Enable timer interrupt channel 0*/
	LPIT0->TMR[0].TVAL = scheduler_ticks;	/* Chan 0 Timeout period: 40M clocks */
	LPIT0->TMR[0].TCTRL = 0x00000001; 		/* T_EN=1: Timer channel is enabled */
}

/* Polling delay function */
void delay(double ms){
	  /*Channel 1*/
	  ms /=1000;
	  ms *= 40000000;
	  LPIT0->TMR[1].TVAL = (uint32_t) ms;
	  LPIT0->TMR[1].TCTRL = 0x00000001; //Enable
	  while (0 == (LPIT0->MSR & LPIT_MSR_TIF1_MASK)) {}
	  LPIT0->MSR |= LPIT_MSR_TIF1_MASK;
}

void cronometer(void){
	/*Channel 1*/
	LPIT0->TMR[1].TVAL  = (uint32_t) 40000000;
	LPIT0->TMR[1].TCTRL = 0x00000001; //Enable
}

ros::NodeHandle nh;

uint32_t steurung;
float32_t control_reference = 0;


int main() {
	/* Clocks configuration and initialization */
	SOSC_init_8MHz();       /* Initialize system oscilator for 8 MHz xtal */
	SPLL_init_160MHz();     /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
	NormalRUNmode_80MHz();  /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */


	/* ================================================================================================ */

	ros::Subscriber<std_msgs::Float32MultiArray> sub("/board_connection/control_array", &ros_callback_ctrl);

	std_msgs::Int8 ros_speaker;
	pub.topic_ = "/mcu/active";
	pub.msg_ = &ros_speaker;

	nh.initNode();
	nh.advertise(pub);
	nh.subscribe(sub);

	/* ================================================================================================ */

	LPIT0_init();

	/* Scheduler initialization */
	steurung = 0;
	LPIT0_chn0_init(40000); // 40000 ticks = 1ms


//	Steering_init();
//	uint32_t start = 0;
//	uint32_t end   = 0;
//	int32_t time  = 0;
//
//	cronometer();
//	for(;;){
//		start = LPIT0->TMR[1].CVAL;
//		steering_set_position(90);
//		end   = LPIT0->TMR[1].CVAL;
////		time = start-end;
//	}
//
//



	while(1) {
//	  nh.spinOnce();
	}
	return 0;
}

void scheduler_task_1(void);
uint32_t task_1_flag = 0x01;
void scheduler_task_2(void);
uint32_t task_2_flag = 0x02;


void scheduler(void){
	steurung++;
	if(steurung == task_1_flag){
		scheduler_task_1();
		task_1_flag += 0x02;
	}
	else if(steurung == task_2_flag){
		scheduler_task_2();
		task_2_flag += 0x0A;
	}
}

void LPIT0_Ch0_IRQHandler (void){
	LPIT0->MSR |= LPIT_MSR_TIF0_MASK;	/* Clear LPIT0 timer flag 0 */
	scheduler();
}

void scheduler_task_1(void){
	nh.spinOnce();
}

void scheduler_task_2(void){
	steering_manual_ctrl();
}


void ros_callback_ctrl(const std_msgs::Float32MultiArray &msg) {
	control_reference = msg.data[0];

	std_msgs::Int8 to_send;
	to_send.data = 1;
	pub.publish(&to_send);
}

