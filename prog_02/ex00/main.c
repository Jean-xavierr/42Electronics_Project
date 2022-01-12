#include <avr/io.h>

void        INT1_vect(void)
{
    PORTB ^= (1 << PB3);
    for (long i = 0; i < 50000; i++) {} // effects rebond
    SREG = (1 << 7);
}

int         main(void)
{
    DDRB = (1 << PB3);

    EICRA = (1 << ISC11);   // External Interrupt Control Register
    EIMSK = (1 << INT1);    // External Interrupt Mask Register
    SREG = (1 << 7);        // Global interrupt
    for (;;) {}
    return 0;
}