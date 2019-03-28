#pragma once

#include <mbed.h>

class HydraulicBrake {
private:
  DigitalOut m_A;
  DigitalOut m_B;
  Timer m_timer;

public:
  HydraulicBrake(PinName a, PinName b);

  void engage(float setpoint);

  void disengage();
};
