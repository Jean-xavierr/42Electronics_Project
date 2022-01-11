#include <avr/io.h>
#define DELAY 25000

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;

int     main(void)
{
    DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);

    uint8_t count = 0;
    uint8_t new_status;
    uint8_t old_status = PIND & (1 << PD3);

    for (;;) {
        new_status = PIND & (1 << PD3);
        if (new_status != old_status) {
            for (long i = 0; i < DELAY; i++) {}
            if (new_status == 0){
                if (count < 15)
                    count++;
                else
                    count = 0;
                PORTB = 0;
                PORTB = count & ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3));
            }
            old_status = new_status;
        }
    }
    return 0;
}