#include "TractionMotor.hpp"

bool sw(bool a){
  return not a;
}

TractionMotor::TractionMotor(PinName fwd, PinName rev, PinName foot,
                             PinName throttle, PinName regen):
    m_forward(fwd), m_reverse(rev), m_foot(foot),
    m_throttle(throttle),
    m_regen(regen)
{
  // setup();
  // idle();
}

void TractionMotor::setup()
{
  // 10 kHz
  m_throttle.period_us(100);
  m_regen.period_us(100);
}

void TractionMotor::gear(Gear gear) {
  m_forward = sw(gear == Gear::forward);
  m_reverse = sw(gear == Gear::reverse);
}

void TractionMotor::throttle(float accel) {

  if (accel > 0.0f) {
    m_foot = sw(1);
    m_throttle = accel;
    m_regen = 0.0f;
  } else {
    m_foot = sw(0);
    m_throttle = 0.0f;
    m_regen = -accel;
  }
}

void TractionMotor::idle()
{
  m_foot = sw(0);
  m_throttle = 0.0f;
  m_regen = 0.0f;
}
