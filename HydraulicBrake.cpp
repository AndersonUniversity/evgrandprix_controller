#include "HydraulicBrake.hpp"

HydraulicBrake::HydraulicBrake(PinName control) :
m_control(control),
m_timer()
{
  // disengage();
}

// this methods must be called often in order to trigger the power off for the
// motor
void HydraulicBrake::engage(float setpoint) {
  if (true) { // FIXME
    // newly engaged
    m_timer.reset();
    m_timer.start();
  }

  if (setpoint > m_timer) {
    // keep engaging
    m_control.pulsewidth_us(2000);
  } else {
    // hold
    m_control.pulsewidth_us(1500);
    m_timer.stop();
  }
}

void HydraulicBrake::disengage() {
  m_control.pulsewidth_us(1000);
  m_timer.stop();
  // forever
}
