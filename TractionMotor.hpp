#pragma once

#include <mbed.h>

#include "CommandMsg.hpp"


class TractionMotor {
private:
  DigitalOut m_forward;
  DigitalOut m_reverse;
  DigitalOut m_foot;
  PwmOut m_throttle;
  //PwmOut m_regen;

public:
  // throttle and regen must be PWM pins
  TractionMotor(PinName fwd, PinName rev, PinName foot,
    PinName throttle,
    PinName regen);

  void setup();

  void gear(Gear gear);
  void throttle(float accel);
  void idle();
};
