#include <avr/io.h>

int     main(void)
{
    DDRB = (1 << PB3);
    for (;;) {
        for (long i=0; i < 60000; i++) {};
        PORTB ^= (1 << PB3);
    }
    return 0;
}