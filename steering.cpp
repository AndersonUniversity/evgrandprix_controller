#include <mbed.h>
#include <arm_math.h>

arm_pid_instance_f32 pid;

// Sabortooth config
// 1-OFF 2-ON 3-ON 4-OFF 5-ON 6-OFF

AnalogIn steering_angle_desired(A3); // from a pot
AnalogIn steering_angle_actual(A5); // from the shaft encoder (filtered)

// Logic levels as low as 2.7V are considered high by the Sabortooth 2x32
PwmOut steering_motor_control(A6);

const static float update_period = 0.01f;

//When this is a ISR you need to remove the printf calls
void update()
{
  // error is between 0 and 1
  const float error = steering_angle_desired - steering_angle_actual;

  //Process the PID controller
  float pw = arm_pid_f32(&pid, error);

  printf("desired %.2f, actual %.2f, error %.2f\n\r", steering_angle_desired.read(), steering_angle_actual.read(), error);

  // translate (0 is at 1500)
  pw = -pw + 1500.0f;

  //clip
  pw = max(pw, 1000.0f);
  pw = min(pw, 2000.0f);

  printf("pw %.2f\n\r", pw);
  steering_motor_control.pulsewidth_us(int(pw));
}

void steering()
{
  while(true){
    update();
    wait(update_period);
  }
}



Thread steering_thread;

int main()
{
  //init
  steering_motor_control.pulsewidth_us(1500);

  //Initialize the PID instance structure
  pid.Kp = 2000.0;
  pid.Ki = 0.0;
  pid.Kd = 0.0;
  arm_pid_init_f32(&pid, 1);

  //can use the callback() to create a callback as well
  steering_thread.start(callback(steering));

  while(true){
    printf("still here\n\r");
    wait(1);
  }

}
