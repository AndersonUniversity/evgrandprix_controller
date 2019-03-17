#pragma once

#include <mbed.h>

class TractionMotor {
private:
  DigitalOut m_forward;
  DigitalOut m_reverse;
  DigitalOut m_foot;
  PwmOut m_throttle;
  PwmOut m_regen;

public:
  TractionMotor(PinName fwd, PinName rev, PinName foot, PinName throttle,
                PinName regen);

  void forward(float accel);

  void reverse(float accell);

  void idle();
};
