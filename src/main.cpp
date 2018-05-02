#include "system.h" /* include peripheral declarations S32K148 */
#include "std_msgs/String.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int8.h"
#include "ros.h"

// Include C headers (ie, non C++ headers) in this block
extern "C" {
#include "clocks_and_modes.h"
#include "Steering.h"
#include "Scheduler.h"
}

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

// Function prototypes
void ros_callback_ctrl(const std_msgs::Float32MultiArray &msg);


#define PTE21 21
#define PTE22 22
#define PTE23 23


void Port_init(void){
	PCC->PCCn[PCC_PORTE_INDEX] = PCC_PCCn_CGC(1);
	PORTE->PCR[PTE21] = PORT_PCR_MUX(0b001);	//Port E21: MUX = GPIO
	PORTE->PCR[PTE22] = PORT_PCR_MUX(0b001);	//Port E22: MUX = GPIO
	PORTE->PCR[PTE23] = PORT_PCR_MUX(0b001);	//Port E23: MUX = GPIO
	PTE->PDDR |= 0b111<<PTE21;					//PortE 21-23: Data direction = output
}

ros::NodeHandle nh;
ros::Publisher pub("", 0);


uint8_t rojo_flag = 0;
uint8_t azul_flag = 0;

uint16_t pos = 0;
float32_t control_reference = 0;

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

void cronometro(void){
	  /*Channel 1*/
	  LPIT0->TMR[1].TVAL = (uint32_t) 40000000;
	  LPIT0->TMR[1].TCTRL = 0x00000001; //Enable
}

void rojo(void){
	PTE->PTOR |= 1<<PTE21;
}

void azul(void){
	PTE->PTOR |= 1<<PTE23;
}

void verde(void){
	PTE->PTOR |= 1<<PTE22;
}

void steering(void){
	steering_manual_ctrl();
}

void noderos(void){
	nh.spinOnce();
}


scheduler_task_config_t tasks[3] = {
		{
				.task_callback = noderos,
				.period_ticks  = 0x02,
				.start_tick	   = 0x01
		},
		{
				.task_callback = azul,
				.period_ticks  = 2860,
				.start_tick	   = 0x02
		},
//		{
//				.task_callback = verde,
//				.period_ticks  = 250,
//				.start_tick	   = 503
//		}
};


int main(void)
{
	SOSC_init_8MHz();		/* And SOSCDIV1 & SOSCDIV2 =1: divide by 1 */
	SPLL_init_160MHz();		/* And SPLLDIV1 divide by 2; SPLLDIV2 divide by 4 */
	NormalRUNmode_80MHz();

	ros::Subscriber<std_msgs::Float32MultiArray> sub("/board_connection/control_array", &ros_callback_ctrl);

	std_msgs::Int8 ros_speaker;
	pub.topic_ = "/mcu/active";
	pub.msg_ = &ros_speaker;

	nh.initNode();
	nh.advertise(pub);
	nh.subscribe(sub);

	Port_init();
	Steering_init();

	PTE->PCOR |= 1<<PTE21;	//Turn off RED led
	PTE->PCOR |= 1<<PTE22;	//Turn off GREEN led
	PTE->PCOR |= 1<<PTE23;	//Turn off BLUE led


	scheduler_init(&tasks[0], 2, 140); //40000 ticks = 1ms
	cronometro();

	for(;;){

	}

	return 0;
}

void ros_callback_ctrl(const std_msgs::Float32MultiArray &msg) {
	control_reference = msg.data[0];

	std_msgs::Int8 to_send;
	to_send.data = 1;
	pub.publish(&to_send);
}

