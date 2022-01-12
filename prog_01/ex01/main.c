#include <avr/io.h>

int     main(void)
{
    DDRB = (1 << PB1);

    TCCR1B |= (1 << WGM12) | (1 << WGM13);  // Fast PWM mode (fast Pulse Width Modulation - mode 14 p.142)
    TCCR1A |= (1 << WGM11);
    TCCR1A |= (1 << COM1A1);                // Clear OC1A on Compare Match, set OC1A at BOTTOM (ch16-2 p.140)
    TCCR1B |= (1 << CS12);                  // prescaler

    ICR1 = F_CPU / 256;                     // set pwm frequency (62500)
    OCR1A = ICR1 / 10;                      // duty cycle 10 percent
                                            // sawtooth diagram (https://www.mathworks.com/help/sltest/ref/operator_sawtooth.png)

    for (;;) {}
    return 0;
}