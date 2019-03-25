#pragma once

#include <mbed.h>

class HydroBrake {
private:
  DigitalOut m_A;
  DigitalOut m_B;
  Timer m_timer;

public:
  HydroBrake(PinName a, PinName b);

  void engage(float setpoint);

  void disengage();
};
