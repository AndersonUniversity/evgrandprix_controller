#pragma once

#include <mbed.h>

class HydraulicBrake {
private:
  PwmOut m_control;
  Timer m_timer;

public:
  HydraulicBrake(PinName control);

  void engage(float setpoint);

  void disengage();
};
