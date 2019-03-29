#include <mbed.h>

#include "Watchdog.hpp"
#include "HydraulicBrake.hpp"
#include "TractionMotor.hpp"
#include "ibus.hpp"
#include "SteeringLoop.hpp"

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
SteeringLoop steer(A4, D6);

// interface for linear motor for hydrolic brake actuator (uses the S2 on the Sabortooth)
HydraulicBrake hydraulic_brake(D5);


// Serial pc(USBTX, USBRX, 115200); // tx, rx
Serial ibus_receiver(NC, D4, 115200); // uart 1

Watchdog dog;
iBUS ibus;


void setup() {
  traction_motor.idle();
  hydraulic_brake.start();
  steer.start();
  dog.configure(0.5);
}

void remote_control(uint16_t *data) {
  led1 = !led1;

  // channel 1 is steering
  // left is from 1000 to 1500
  // right is from 1500 to 2000
  steer.set_desired(float(data[0] - 1000) / 1000);

  // channel 4 is gear select (center position is neutral)
  const bool forward = data[3] > 1750;
  const bool reverse = data[3] < 1250;

  // channel 6 is hydraulic brake position
  if(data[5] > 1300){
    // engage the brake
    hydraulic_brake.set_desired(float(data[5] - 1000) / 1000);
  }
  else{
    // deadzone (retract completely)
    hydraulic_brake.disengage();
  }

  // channel 2 is throttle and regen brake
  if (data[1] >= 1450 and data[1] <= 1550) {
    // Dead zone
    traction_motor.idle();
  } else if (data[1] >= 1550) {
    // throttle on. Scaling with pull
    hydraulic_brake.disengage(); //for safety
    if(forward){
      traction_motor.forward(float(data[1] - 1500) / 500);
    }
    else if(reverse){
      traction_motor.reverse(float(data[1] - 1500) / 500);
    }
    //else we are in neutral
  } else {
    // brake using regen
    traction_motor.forward(float(data[1] - 1500) / 500);
  }
}

int main() {
  setup();

  printf("Starting main loop\n\r");

  while (true) {
    const uint8_t ch = ibus_receiver.getc();
    if (ibus.read(ch) == 0) {
      // A complete message has been read
      dog.service();

      for (int i = 0; i < 6; i++) printf("%d ", ibus.data[i]);
      printf("\n\r");

      remote_control(ibus.data);
    }
  }
}
