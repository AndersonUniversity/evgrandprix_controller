#include <mbed.h>
#include <arm_math.h>


// Sabortooth config
// 1-OFF 2-ON 3-ON 4-OFF 5-ON 6-OFF
// Logic levels as low as 2.7V are considered high by the Sabortooth 2x32

class SteeringLoop{
private:
  AnalogIn m_encoder;
  PwmOut m_control;
  float m_dt;
  float m_desired;
  arm_pid_instance_f32 m_pid;
  Thread m_thread;
  Mutex m_mutex;

public:
  SteeringLoop(PinName encoder, PinName control, float dt = 0.01f):
  m_encoder(encoder),
  m_control(control),
  m_dt(dt),
  m_desired(0.5),
  m_pid(),
  m_thread(),
  m_mutex()
  {}

  // start the thread
  void start();

  // Thread-safe way to set the desired value
  // desired -- is on the interval [0,1]
  void set_desired(float desired);

private:
  void update();
  void loop();

};
