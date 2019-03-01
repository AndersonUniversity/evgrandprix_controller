#include <mbed.h>
#include "ibus.hpp"

/*
NUCLEO L432KC
Two UARTs available
UART1_RX is D4

UART2 is the USB port (STDOUT/STDIN)
UART2_TX is A7
UART2_RX is A2
*/

DigitalOut led1(LED1);

PwmOut throttle_pwm(A6);
PwmOut regen_pwm(A5);

PwmOut steer(D6);
PwmOut hydro_brake(D3);
DigitalOut forward(D11);
DigitalOut reverse__(D12);
DigitalOut ignition(D9);
//Serial pc(USBTX, USBRX, 115200); // tx, rx
Serial receiver(NC, D4, 115200);  //uart 1

int main()
{
    uint16_t data[6];
    iBUS ibus(6);

    throttle_pwm.period(1e-3);
    steer.period(20e-3);

    printf("Starting loop\n\r");

    while(1) {
        const uint8_t ch = receiver.getc();

        if (ibus.read(data, ch) == 0) {
            // A complete message has been read
            led1 = !led1;

            for (int i = 0; i < 6; i++) printf("%d ", data[i]);
            printf("\n\r");

            // channel 1 is steering
            // left is from 1000 to 1500
            // right is from 1500 to 2000
            steer.pulsewidth_us(data[0]);

            // channel 4 is hydro brake
            const int brakeMode = data[3] > 1750;

            if (data[1] >= 1500) {
                // throttle on. Scaling with pull
                regen_pwm = 0;
                hydro_brake.pulsewidth_us(1000);
                throttle_pwm.write(float(data[1] - 1500) / 500);
                printf("throttle %f", float(data[1] - 1500) / 500);
            } else {
                throttle_pwm = 0;
                // brake according to brakeMode
                if(brakeMode == 1) {
                    // using hydro
                    hydro_brake.pulsewidth_us(2000);
                } else {
                    //brake using regen
                    regen_pwm = 1 - (float(data[1] - 1000) / 500);
                }
            }

            // channel 3 is gear. Forward or Reverse
            if(data[2] == 1000) {
                // In foward gear.
                // hydro_brake.pulsewidth_us(2000);
                reverse__ = 0;
                forward = 1;
            } else {
                // In reverse gear.
                forward = 0;
                reverse__ = 1;
            }
        }
    }
}

