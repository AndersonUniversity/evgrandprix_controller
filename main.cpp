#include <mbed.h>
#include "stats_report.hpp"
#include "ibus.hpp"

/*
NUCLEO L432KC
Two UARTs available
UART1_RX is D4

UART2 is the USB port (STDOUT/STDIN)
UART2_TX is A7
UART2_RX is A2
*/

DigitalOut led1(LED1);


AnalogOut throttle(A4);
AnalogOut regen_brake(A3);
PwmOut steer(D6);
PwmOut hydro_brake(D3);
DigitalOut forward(D11);
DigitalOut reverse(D12);
DigitalOut ignition(D9);
//Serial pc(USBTX, USBRX, 115200); // tx, rx
Serial receiver(NC, D4, 115200);  //uart 1

/*
#define SLEEP_TIME                  500 // (msec)
#define PRINT_AFTER_N_LOOPS         20


// main() runs in its own thread in the OS
int main()
{
    // Loop delay time in ms
    SystemReport sys_state( SLEEP_TIME * PRINT_AFTER_N_LOOPS);

    int count = 0;
    while (true) {
        // Blink LED and wait 0.5 seconds
        led1 = !led1;
        wait_ms(SLEEP_TIME);

        if ((0 == count) || (PRINT_AFTER_N_LOOPS == count)) {
            // Following the main thread wait, report on the current system status
            sys_state.report_state();
            count = 0;
        }
        ++count;
    }
}
*/


int main() {
  uint16_t data[6];
  iBUS ibus(6);

  printf("Starting loop\n\r");

  while(1) {
    const uint8_t ch = receiver.getc();

    if (ibus.read(data, ch) == 0) {
      // A complete message has been read
      led1 = !led1;

      for (int i = 0; i < 6; i++) printf("%d ", data[i]);
      printf("\n\r");

      //channel 1 is steering
      steer.pulsewidth_us(data[0]);

      /*
      // channel 2 is throttle/brake control
      if(data[1] >= 1500){
        //forward
        throttle = (data[1] - 1500) / 500.0;
        regen_brake = 0;
        //hydro_brake = 0;
      }else{
        throttle = 0;
        regen_brake = -(data[1] - 1500) / 500.0;
        //hydro_brake = 1;
      }
      */
      
      //channel 3 is hydrolic hydrolic brake
      if(data[2] > 1500){
        // engage the brake as fast as possible
        hydro_brake.pulsewidth_us(2000);
      }else{
        // disengage the brage as fast as possible
        hydro_brake.pulsewidth_us(1000);
      }

      // channel 4 is ...

    }
  }
}
