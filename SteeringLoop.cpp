#include "SteeringLoop.hpp"

void SteeringLoop::set_desired(float desired)
{
  // This needs to grab a mutex to be thread safe.
  m_mutex.lock();
  m_desired = desired;
  m_mutex.unlock();
}

void SteeringLoop::start(){
  //init
  m_control.pulsewidth_us(1500);

  //Initialize the PID instance structure
  m_pid.Kp = 4.0;
  m_pid.Ki = 0.0;
  m_pid.Kd = 0.0;
  arm_pid_init_f32(&m_pid, 1);

  //can use the callback() to create a callback as well
  m_thread.start(callback(this, &SteeringLoop::loop));
}

void SteeringLoop::update()
{
  m_mutex.lock();
  const float desired = m_desired;
  m_mutex.unlock();

  const float actual = m_encoder;

  // error is between 0 and 1
  const float error = desired - actual;

  //Process the PID controller
  const float out = arm_pid_f32(&m_pid, error);

  //printf("desired %.2f, actual %.2f, error %.2f\n\r", desired.read(), actual.read(), error);

  // translate (0 is at 1500)
  int pw = -int(500.0f * out) + 1500;

  //clip
  pw = max(pw, 1000);
  pw = min(pw, 2000);

  //printf("pw %.2f\n\r", pw);
  m_control.pulsewidth_us(pw);
}

void SteeringLoop::loop()
{
  while(true){
    update();
    wait(m_dt);
  }
}
