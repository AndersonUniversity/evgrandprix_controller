#include <mbed.h>
#include <arm_math.h>

#include "ControlLoop.hpp"

// Sabortooth config
// 1-OFF 2-ON 3-ON 4-OFF 5-ON 6-OFF
// Logic levels as low as 2.7V are considered high by the Sabortooth 2x32

class SteeringLoop : public ControlLoop<float> {
private:
  AnalogIn m_encoder;
  PwmOut m_control;
  arm_pid_instance_f32 m_pid;

public:
  SteeringLoop(PinName encoder, PinName control, float dt = 0.01f);

  void start() override;

private:
  void update() override;
};
