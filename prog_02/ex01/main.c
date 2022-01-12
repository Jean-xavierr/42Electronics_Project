#include <avr/io.h>
#include <avr/interrupt.h>
typedef unsigned char uint8_t;

uint8_t ratio = 0;
uint8_t status = 1;

void    change_status(void)
{
    if (ratio == 100 || ratio == 0)
        status *= -1;
    ratio += status;
}

int     main(void)
{
    DDRB = (1 << PB1);

    // TIMER1

    TCCR1B |= (1 << WGM12) | (1 << WGM13);  // Fast PWM mode (fast Pulse Width Modulation - mode 14 p.142)
    TCCR1A |= (1 << WGM11);
    TCCR1A |= (1 << COM1A1);                // Clear OC1A on Compare Match, set OC1A at BOTTOM (ch16-2 p.140)
    TCCR1B |= (1 << CS12);                  // prescaler

    ICR1 = F_CPU / 256;                     // set pwm frequency (62500)
    OCR1A = ICR1 / ratio;                      // duty cycle 10 percent
                                            // sawtooth diagram (https://www.mathworks.com/help/sltest/ref/operator_sawtooth.png)

    // TIMER0

    TCCR0A |= (1 << WGM01);                 // CTC mode
    TCCR0A |= (1 << COM0A1);                
    TCCR0B |= (1 << CS02);                  // prescaler   

    OCR0A = 255; 
    // TIMSK0 = (1 << TOIE0);
    for (;;) {}
    return 0;
}