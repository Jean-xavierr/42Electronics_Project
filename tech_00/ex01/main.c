#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>
#include <avr/eeprom.h>

#define DELAY 25000
uint8_t count = 0;

int             main(void)
{
    uint16_t *eeprom_address_count = 0;
    uint16_t *eeprom_address_start = 1;

    if (!eeprom_read_word((uint16_t *) eeprom_address_start)) {
        eeprom_update_word((uint16_t *) eeprom_address_start, 1);
    } else {
        count = eeprom_read_word(eeprom_address_count);
    }

    DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);

    uint8_t new_status;
    uint8_t old_status = PIND & (1 << PD3);

    PORTB = count;
    for (;;) {
        new_status = PIND & (1 << PD3);
        if (new_status != old_status) {
            for (long i = 0; i < DELAY; i++) {}
            if (new_status == 0){
                if (count < 15)
                    count++;
                else
                    count = 0;
                PORTB = count;
            }
            eeprom_update_word(eeprom_address_count, count);
            old_status = new_status;
        }
    }
    return 0;
}