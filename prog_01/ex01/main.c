#include <avr/io.h>

int     main(void)
{
    DDRB = (1 << PB1);

    TCCR1B |= (1 << WGM12);                 // CTC mode (Clear Timer on Compare - datasheet p.131 & p.141)
    TCCR1A |= (1 << COM1A0);                // Toggles OC1A pin each cycle through
    TCCR1B |= (1 << CS12);                  // prescaler (datasheet p.143)

    OCR1A = F_CPU / 256 / 2;                // Output Compare Registers
    for (;;) {}
    return 0;
}