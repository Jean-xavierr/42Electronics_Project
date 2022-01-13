#include <avr/io.h>
#include <avr/interrupt.h>
typedef unsigned char uint8_t;

uint8_t ratio = 1;
uint8_t status = 1;

void    TIMER0_COMPA_vect(void)
{
    if (ratio == 100 || ratio == 0)
        status *= -1;
    ratio += status;
    OCR1A = ratio * (ICR1 / 100); 
    SREG |= (1 << 7);
}

int     main(void)
{
    DDRB = (1 << PB1);

    // TIMER1

    TCCR1B |= (1 << WGM12) | (1 << WGM13);  // Fast PWM mode (fast Pulse Width Modulation - mode 14 p.142)
    TCCR1A |= (1 << WGM11);
    TCCR1A |= (1 << COM1A1);                // Clear OC1A on Compare Match, set OC1A at BOTTOM (ch16-2 p.140)
    TCCR1B |= (1 << CS10) | (1 << CS11);                  // prescaler

    ICR1 = F_CPU / 64 / 200;                // set pwm frequency
    OCR1A = ICR1 / ratio;                   // duty cycle percent
                                            // sawtooth diagram (https://www.mathworks.com/help/sltest/ref/operator_sawtooth.png)

    // TIMER0

    TCCR0A |= (1 << WGM01);                 // CTC mode
    TCCR0A |= (1 << COM0A1);                
    TCCR0B |= (1 << CS00) | (1 << CS02);    // prescaler   

    OCR0A = 70; 
    TIMSK0 = (1 << OCIE0A);
    SREG = (1 << 7);
    for (;;) {}
    return 0;
}