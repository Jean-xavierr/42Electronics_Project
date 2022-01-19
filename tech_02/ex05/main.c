#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>
#include <stdlib.h>

#define DELAY 50000
uint16_t adc_val = 0;

void            uart_init(uint8_t USART_BAUDRATE)
{
    UCSR0B |= (1 << TXEN0);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
	UBRR0L = USART_BAUDRATE;                
	UBRR0H = (UBRR0L >> 8);		
}

void            uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void            uart_printstr(char *str)
{
    int i = 0;
    while (str[i])
        uart_tx(str[i++]);
}

void            adc_init()
{
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) |(1 << ADPS0);
    ADMUX =  (1 << REFS0) | (1 << ADLAR) | (1 << MUX2);
}

uint16_t        adc_read(uint8_t ch)
{
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)); 
    return ADCH;
}

int             main(void)
{
    uart_init(8);
    adc_init();

    //RGB_LED  Red         Green       Blue
    DDRD = (1 << PD6) | (1 << PD5) | (1 << PD3);
    PORTD = 0xff;

    DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);

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
    uint8_t old_status = PIND & (1 << PD2);

    uint8_t led_val = 0;
    char str_val[6] = {0};
    for (;;) {
        adc_val = adc_read(0);
        itoa(adc_val, str_val, 10);
        uart_printstr(str_val);
        uart_printstr("\n\r");
        new_status = PIND & (1 << PD2);
        if (adc_val < 25){
            PORTB = (0 << PB0) | (0 << PB1) | (0 << PB2) | (0 << PB3);
        } else if (adc_val > 63 && adc_val < 85) {
            PORTB = (1 << PB0) | (0 << PB1) | (0 << PB2) | (0 << PB3);
        } else if (adc_val > 85 && adc_val < 127) {
            PORTB = (1 << PB0) | (1 << PB1) | (0 << PB2) | (0 << PB3);
        } else if (adc_val > 127 && adc_val < 250) {
            PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (0 << PB3);
        } else if (adc_val > 250) {
            PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);
        }
        if (counter == 0) {
            OCR0A = adc_val;
        } else if (counter == 1) {
            OCR0B = adc_val;
        } else if (counter == 2) {
            OCR2B = adc_val;
        }
        if (new_status != old_status) {
            for (long i = 0; i < DELAY; i++) {}
            if (new_status == 0) {
                counter++;
                if (counter == 3)
                    counter = 0;
            }
        }
        old_status = new_status;
    }
    return 0;
}