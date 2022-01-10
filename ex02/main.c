#include <avr/io.h>

int     main(void)
{
    DDRB = (1 << PB3);           // set PORTB PB3 pins for output
    for (;;) {
        PORTB = (1 << PB3);      // for PB3 in binary 0000100 -> hex=8
    }
    return 0;
}