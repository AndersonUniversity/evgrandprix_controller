#include "HydraulicBrake.hpp"

HydraulicBrake::HydraulicBrake(PinName a, PinName b) : m_A(a), m_B(b), m_timer() {
  // disengage();
}

// this methods must be called often in order to trigger the power off for the
// motor
void HydraulicBrake::engage(float setpoint) {
  if (not m_A) {
    // newly engaged
    m_timer.reset();
    m_timer.start();
  }

  if (setpoint > m_timer) {
    // keep engaging
    m_A = 1;
    m_B = 0;
  } else {
    // hold
    m_A = 0;
    m_B = 0;
    m_timer.stop();
  }
}

void HydraulicBrake::disengage() {
  m_A = 0;
  m_B = 1;
  m_timer.stop();
  // forever
}
