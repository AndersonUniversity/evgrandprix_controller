#include "HydraulicBrake.hpp"
#include "Log.hpp"

HydraulicBrake::HydraulicBrake(PinName control, float dt) :
ControlLoop(dt),
m_control(control),
m_elapsed(0.0f),
m_state(idle)
{
  // disengage();
}

void HydraulicBrake::start()
{
  set_desired(0.0f);

  ControlLoop::start();
}

void HydraulicBrake::bank()
{
  m_elapsed = m_elapsed + m_state*m_timer;
  m_timer.reset();
}

void HydraulicBrake::update()
{
  const float dead_dt = 0.05f;
  const float actual = m_elapsed + m_state*m_timer;
  const float desired = get_desired();
  const float error = desired - actual;

  // determine new state
  state new_state = idle;
  if (error > dead_dt) new_state = engaging;
  else if (error < dead_dt) new_state = disengaging;

  // always disengage for zero brake (this resets our open loop control)
  // We don't want to be idle in this zone
  if (m_elapsed < 0.01f) new_state = disengaging;

  if (m_state != new_state) {
    // We have a state transition
    bank();

    if (new_state == idle) {
      // going into idle (from engaging or disengaging)
      m_timer.stop();
    }

    if (m_state == idle) {
      // newly engaging or disengaging
      m_timer.start();
    }
  }

  m_state = new_state;

  // convert state to a motor control signal
  int pw = 1500; //idle
  if (m_state == engaging) pw = 2000;
  else if (m_state == disengaging) pw = 1000;

  LOG("HYDRAULIC BRAKE: desired %.2f, actual %.2f, error %.2f, pw %d\r\n", desired, actual, error, pw);

  m_control.pulsewidth_us(pw);

}
