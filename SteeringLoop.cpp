#include "SteeringLoop.hpp"
#include "Log.hpp"

SteeringLoop::SteeringLoop(PinName encoder, PinName control, float dt):
ControlLoop(dt),
m_encoder(encoder),
m_control(control),
m_pid()
{}

void SteeringLoop::start(){
  //init
  m_control.period_ms(20);
  m_control.pulsewidth_us(1500);
  set_desired(0.0f);

  //Initialize the PID instance structure
  m_pid.Kp = 4.0;
  m_pid.Ki = 0.0;
  m_pid.Kd = 0.0;
  arm_pid_init_f32(&m_pid, 1);

  ControlLoop::start();
}

void SteeringLoop::update()
{
  const float desired = get_desired();

  // FIXME The encoder only returns numbers in [0.5, 1] but it should be [0,1] so we transform it to the correct value.
  //const float actual = 2.0f*(m_encoder - 0.5f);
  const float actual = m_encoder;

  // error is between 0 and 1
  const float error = desired - actual;

  //Process the PID controller
  const float out = arm_pid_f32(&m_pid, error);
  // translate (0 is at 1500)

  int pw = -int(500.0f * out) + 1500;




  //clip
  pw = max(pw, 1000);
  pw = min(pw, 2000);

  LOG("STEERING: desired %.2f, actual %.2f, error %.2f, pw %d\r\n",
    desired, actual, error, pw);

  m_control.pulsewidth_us(pw);
}
