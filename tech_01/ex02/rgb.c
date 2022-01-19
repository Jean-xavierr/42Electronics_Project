#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>
#include <avr/eeprom.h>

#define DELAY 50000
uint8_t RED[3] =    {0xf2, 0x19, 0x4b};
uint8_t GREEN[3] =  {0x18, 0xe0, 0x00};
uint8_t BLUE[3] =   {0x4f, 0xa1, 0x82};

int             main(void)
{
    //RGB_LED  Red         Green       Blue
    DDRD = (1 << PD6) | (1 << PD5) | (1 << PD3);
    PORTD = 0xff;

    // TIMER0
    TCCR0A |= (1 << WGM00) | (1 << WGM01);                                     // Fast PWM mode 3
    TCCR0A |= (1 << COM0A0) | (1 << COM0A1) | (1 << COM0B0) | (1 << COM0B1);   // Set OC0A on Compare Match, clear OC0A at BOTTOM (inverting mode).
    TCCR0B |= (1 << CS02);                                                     // prescaler 256
    OCR0A = 0xff;
    OCR0B = 0xff;

    // TIMER2
    TCCR2A |= (1 << WGM20) | (1 << WGM21);                                      // Fast PWM mode 3
    TCCR2A |= (1 << COM2B0) | (1 << COM2B1);                                    // Set OC2B on Compare Match, clear OC2B at BOTTOM (inverting mode).
    TCCR2B |= (1 << CS22) | (1 << CS21);                                        // prescaler 256
    OCR2B = 0xff;

    uint8_t counter = 0;
    uint8_t new_status;
    uint8_t old_status = PIND & (1 << PD2);                                     // check status of button in PD2

    for (;;)  {
        new_status = PIND & (1 << PD2);
        if (new_status != old_status) {
            for (long i = 0; i < DELAY; i++) {}
            if (new_status == 0) {
                OCR0A = RED[counter];
                OCR0B = GREEN[counter];
                OCR2A = BLUE[counter];
                counter++;
                if (counter == 3)
                    counter = 0;
            }
        }
        old_status = new_status;
    }
    return 0;
}