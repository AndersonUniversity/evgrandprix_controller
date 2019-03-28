#include <mbed.h>

#include "Watchdog.hpp"
#include "HydraulicBrake.hpp"
#include "TractionMotor.hpp"
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

// Traction motor interface
TractionMotor traction_motor(D11, D12, D9, A6, A5);

// Kangaroo interface for the steering control
// TODO wrap this up in a nice interface class
PwmOut steer(D6);

// H-Bridge interface for linear motor for hydrolic brake actuator
HydraulicBrake hydraulic_brake(D3, D4);

// Serial pc(USBTX, USBRX, 115200); // tx, rx
Serial ibus_receiver(NC, D4, 115200); // uart 1

Watchdog dog;

void setup() {
  traction_motor.idle();
  hydraulic_brake.disengage();
  steer.period(20e-3);
  dog.Configure(0.1);
}

void remote_control(uint16_t *data) {
  led1 = !led1;

  // channel 1 is steering
  // left is from 1000 to 1500
  // right is from 1500 to 2000
  steer.pulsewidth_us(data[0]);

  // channel 4 is brake mode select
  const int brakeMode = data[3] > 1750;

  // channel 3 is gear. Forward or Reverse
  const bool forward = data[2] == 1000;

  // channel 2 is throttle and regen brake
  if (data[1] >= 1450 and data[1] <= 1550) {
    // Dead zone
    traction_motor.idle();
    hydraulic_brake.disengage();
  } else if (data[1] >= 1550) {
    // throttle on. Scaling with pull
    if (forward){
      traction_motor.forward(float(data[1] - 1500) / 500);
    }else{
      traction_motor.reverse(float(data[1] - 1500) / 500);
    }
    hydraulic_brake.disengage();
  } else {
    // brake according to brakeMode
    if (brakeMode == 1) {
      // using hydro
      hydraulic_brake.engage(0.5); // number of seconds to full engagement
    } else {
      // brake using regen
      hydraulic_brake.disengage();
      traction_motor.forward(float(data[1] - 1500) / 500);
    }
  }
}

int main() {
  // Setup
  uint16_t data[6];
  iBUS ibus(6);

  setup();

  printf("Starting loop\n\r");

  while (1) {
    const uint8_t ch = ibus_receiver.getc();

    if (ibus.read(data, ch) == 0) {
      // A complete message has been read
      dog.Service();

      for (int i = 0; i < 6; i++)
        printf("%d ", data[i]);
      printf("\n\r");

      remote_control(data);
    }
  }
}
