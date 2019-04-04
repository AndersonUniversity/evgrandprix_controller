#include <mbed.h>

#include "Watchdog.hpp"
#include "HydraulicBrake.hpp"
#include "TractionMotor.hpp"
#include "ibus.hpp"
#include "SteeringLoop.hpp"
#include "SystemReport.hpp"
#include "Log.hpp"

/*
NUCLEO L432KC
Two UARTs available
UART1_RX is D4 (for the iBUS)

UART2 is the USB port (STDOUT/STDIN)
UART2_TX is A7
UART2_RX is A2
*/

DigitalOut led1(LED1);

// Traction motor interface
TractionMotor traction_motor(D11, D12, D9, A1, A6);

// steering control connected to the sabortooth H-bridge (S1)
SteeringLoop steer(A0, D6);

// interface for linear motor for hydrolic brake actuator (uses the S2 on the Sabortooth)
HydraulicBrake ebrake(D5);

// Serial pc(USBTX, USBRX, 115200); // tx, rx
Serial ibus_receiver(NC, D0, 115200); // uart 1

Watchdog dog;
iBUS ibus;

// Tuning parameters
const float ebrake_full = 3.0f;

const float steering_full = 0.15f;
const float steering_center = 0.53f;


void setup() {
  traction_motor.setup();
  traction_motor.gear(Gear::neutral);
  traction_motor.idle();

  ebrake.start();
  steer.start();
  dog.configure(0.5f);
}

CommandMsg parse_RC(uint16_t data[])
{
  // PARSE RC PULSE WIDTH DATA (this part is replaced when we go with autonomous control)

  // logging
  {
    mbed::ScopedLock<Mutex> lock(s_stdio_mutex);
    printf("RC: ");
    for (int i = 0; i < 6; i++) printf("%d ", ibus.data[i]);
    printf("\r\n");
  }

  CommandMsg cmd;

  // channel 1 is steering
  // left is from 1000 to 1500
  // right is from 1500 to 2000
  cmd.steering = float(data[0] - 1500) / 500.0f;

  // channel 2 is throttle and regen brake (trigger control)
  cmd.throttle_regen = float(data[1] - 1500) / 500.0f;

  // channel 4 is gear select (center position is neutral)
  cmd.gear = neutral;
  if (data[3] > 1750) cmd.gear = Gear::forward;
  else if (data[3] < 1250) cmd.gear = Gear::reverse;

  // channel 6 is hydraulic brake position
  cmd.ebrake = float(data[5] - 1000) / 1000.0f;

  return cmd;
}

void apply_command(const CommandMsg& cmd)
{
  // APPLY PARSED DATA TO ACTUATORS AND CONTROL LOOPS

  steer.set_desired(-steering_full*cmd.steering + steering_center);

  if(cmd.ebrake > 0.1f){
    // engage the brake
    ebrake.set_desired(ebrake_full * cmd.ebrake);
  }
  else{
    // deadzone (retract completely)
    ebrake.disengage();
  }

  traction_motor.gear(cmd.gear);

  if (cmd.throttle_regen > 0.1f) {
    // throttle on

    // TODO we should probably check the brake to make sure it is not on instead of simply disengaging
    ebrake.disengage(); //for safety

    traction_motor.throttle(cmd.throttle_regen);
    //else we are in neutral
  } else if(cmd.throttle_regen < -0.1f) {
    // brake using regen
    traction_motor.throttle(cmd.throttle_regen);
  } else {
    // Dead zone
    traction_motor.idle();
  }
}

void main_control_loop()
{
  LOG("Starting main control loop\r\n");
  while (true) {

    const uint8_t ch = ibus_receiver.getc();
    if (ibus.read(ch) == 0) {
      // A complete message has been read
      dog.service();

      // status update
      led1 = !led1;

      const CommandMsg cmd = parse_RC(ibus.data);
      // TODO this is where we add an IF statement for autonomous or RC.
      // in either case we call the apply_command()

      cmd.dump();
      apply_command(cmd);
    }
  }
}

void dummy_control_loop()
{
  LOG("Starting dummy control loop\r\n");
  while (true) {

    wait(0.01f);
    dog.service();

    // status update
    led1 = !led1;

    CommandMsg cmd;

    cmd.steering = 0.0f;
    cmd.throttle_regen = 0.5f;
    cmd.gear = Gear::forward;
    cmd.ebrake = 0.0f;

    cmd.dump();
    apply_command(cmd);
  }
}

// Loop delay time in ms
SystemReport stats(10000);

int main() {

  if (dog.watchdogCausedReset())
    LOG("WATCHDOG: The watchdog caused a reset *************\r\n");

  setup();

  LOG("Starting main status loop\r\n");

  Thread thread;
  thread.start(main_control_loop);
  //thread.start(dummy_control_loop);

  while(true){
    wait_ms(stats.sample_time());
    {
      mbed::ScopedLock<Mutex> lock(s_stdio_mutex);
      stats.report_state();
    }
  }
}
