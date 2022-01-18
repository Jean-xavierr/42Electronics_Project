#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>
#include <avr/eeprom.h>

#define DELAY 50000

#define RED     0b1000000
#define GREEN   0b0100000
#define BLUE    0b0001000
#define YELLOW  0b1100000
#define CYAN    0b0101000
#define PURPLE  0b1001000
#define WHITE   0b1101000

const int   colors[] = {RED, GREEN, BLUE, YELLOW, CYAN, PURPLE, WHITE};
uint8_t     counter = 0;

int             main(void)
{
    //        Red          Green       Blue
    DDRD = (1 << PD6) | (1 << PD5) | (1 << PD3);

    uint8_t new_status;
    uint8_t old_status = PIND & (1 << PD2);
    PORTD = 0xff;

    for (;;)  {
        new_status = PIND & (1 << PD2);
        if (new_status != old_status) {
            for (long i = 0; i < DELAY; i++) {}
            if (new_status == 0) {
                PORTD = ~colors[counter];
                if (counter == 6)
                    counter = 0;
                else
                    counter++;
            }
        }
        old_status = new_status;
    }
    return 0;
}