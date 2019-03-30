#pragma once

#include <mbed.h>

#include "ControlLoop.hpp"

// implements timer based open loop control of the linear actuator for the hydraulic motor
class HydraulicBrake : public ControlLoop<float>
{
private:
  enum state {engaging=1, disengaging=-1, idle=0};

  PwmOut m_control;
  float m_elapsed;
  state m_state;
  Timer m_timer;

public:
  HydraulicBrake(PinName control, float dt = 0.01f);

  // reset all the way back
  void disengage() {
    set_desired(0.0f);
  }

  void start() override;

private:
  void update() override;
  void bank();
};
