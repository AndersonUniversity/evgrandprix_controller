#include "HydraulicBrake.hpp"

HydraulicBrake::HydraulicBrake(PinName control, float dt) :
ControlLoop(dt),
m_control(control),
m_current(0.0f),
m_status(idle)
{
  // disengage();
}

void HydraulicBrake::start()
{
  set_desired(0.0f);

  ControlLoop::start();
}

void HydraulicBrake::update()
{
  const float dead_dt = 0.05f;
  const float current = m_current + m_timer;
  const float error = get_desired() - current;

  if (error > dead_dt) {
    // we need to be engaging (at full speed)
    m_control.pulsewidth_us(2000);

    if (m_status != engaging) {
      // newly engaged
      m_timer.reset();
      m_timer.start();
      m_status = engaging;
    }
  } if(error < dead_dt){
    // we need to be disengaging (at full speed)
    m_control.pulsewidth_us(1000);

    if (m_status != disengaging) {
      // newly engaged
      m_timer.reset();
      m_timer.start();
      m_status = disengaging;
    }
  } else {
    // deadzone => hold
    m_control.pulsewidth_us(1500);
    m_timer.stop();
    m_status = idle;
  }
}
