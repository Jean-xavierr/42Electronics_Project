#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>
#include <stdlib.h>

#define DELAY 150000
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

    char str_val[6] = {0};
    for (;;) {
        adc_val = adc_read(0);
        itoa(adc_val, str_val, 10);
        uart_printstr(str_val);
        uart_printstr("\n\r");
        for (long i = 0; i < DELAY; i++) {}
    }
    return 0;
}