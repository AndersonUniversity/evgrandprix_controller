#include "TractionMotor.hpp"

TractionMotor::TractionMotor(PinName fwd, PinName rev, PinName foot,
                             PinName throttle, PinName regen)
    : m_forward(fwd), m_reverse(rev), m_foot(foot), m_throttle(throttle),
      m_regen(regen) {
  // idle();
}

void TractionMotor::forward(float accel) {
  m_forward = 1;
  m_reverse = 0;
  m_foot = 1;

  if (accel > 0) {
    m_throttle = accel;
    m_regen = 0;
  } else {
    m_throttle = 0;
    m_regen = -accel;
  }
}

void TractionMotor::reverse(float accel) {
  m_forward = 0;
  m_reverse = 1;
  m_foot = 1;

  if (accel > 0) {
    m_throttle = accel;
    m_regen = 0;
  } else {
    m_throttle = 0;
    m_regen = -accel;
  }
}

void TractionMotor::idle() {
  // neutral gear
  m_forward = 0;
  m_reverse = 0;
  m_foot = 0;

  m_throttle = 0;
  m_regen = 0;
}
