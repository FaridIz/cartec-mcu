
#include "system.h" /* include peripheral declarations S32K148 */
#include "std_msgs/String.h"
#include "std_msgs/Float32MultiArray.h"
#include "S32K148.h"
#include "ros.h"

// Include C headers (ie, non C++ headers) in this block
extern "C" {
#include "clocks_and_modes.h"
#include "ADC.h"
#include "Steering.h"
}

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

// Function prototypes
void callback(const std_msgs::Float32MultiArray &msg);

// Provisional test function prototypes
void init_led(void);
void led_on(void);
void led_toggle(void);

ros::Publisher pub("", 0);
// Subscribers are declared in the main scope due to a compiler bug


int main() {
	/* Clocks configuration and initialization */
	SOSC_init_8MHz();       /* Initialize system oscilator for 8 MHz xtal */
	SPLL_init_160MHz();     /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
	NormalRUNmode_80MHz();  /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */

	Steering_init();
	PWM_set_duty(M1_PWM, 0);
	GPIO_setPin(M1_INA);
	GPIO_setPin(M1_EN);

	init_led();

	ros::NodeHandle nh;
	ros::Subscriber<std_msgs::Float32MultiArray> sub("/board_connection/control_array", &callback);

	std_msgs::String str_msg;
	pub.topic_ = "/mcu/position";
	pub.msg_ = &str_msg;

	nh.initNode();
	nh.advertise(pub);
	nh.subscribe(sub);

	uint32_t lasttime = 0UL;

	while(1) {
	  if(s32k148_time_now() - lasttime > 10) {
	    // For testing purposes
	    lasttime = s32k148_time_now();
	  }
	  nh.spinOnce();
	}


	return 0;
}



void callback(const std_msgs::Float32MultiArray &msg) {
  std_msgs::String str_msg;

  int control_signal_phi = msg.data[1];

  if(control_signal_phi > 30.0) {
    str_msg.data = "h";
	led_toggle();
	pub.publish(&str_msg);
  }

}

void init_led(void) {
  PCC->PCCn[PCC_PORTE_INDEX] = PCC_PCCn_CGC(1);
  PORTE->PCR[22] = PORT_PCR_MUX(0b001);	//Port E23: MUX = GPIO
  PTE->PDDR |= 0b1<<22;					//PortE 21-23: Data direction = output
}

void led_on(void) {
  PTE->PSOR |= 1<<22;
}

void led_toggle(void) {
  PTE->PTOR |= 1<<22;
}
