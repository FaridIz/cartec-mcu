#ifndef _S32K148_HARDWARE_H
#define _S32K148_HARDWARE_H


extern "C"
{
  #include "s32k148_time.h"
  #include "s32k148_uart.h"
}


class s32K148Hardware {

  public:
	s32K148Hardware() {}

    void init() {
      s32k148_time_init();
      s32k148_uart_init();
    }

    int16_t read() {
      return s32k148_uart_receive_byte();;
    }

    void write(uint8_t* data, int length) {
      for(int i = 0; i < length; i++)
        s32k148_uart_send_byte(data[i]);

    }

    unsigned long time() {
      return s32k148_time_now();  // milliseconds
    }

};

#endif
