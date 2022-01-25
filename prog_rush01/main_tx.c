#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#define DELAY 150000
uint16_t i = 500;
uint16_t adc_val = 0;

void    uart_init(uint8_t USART_BAUDRATE)
{
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);	    // Turn on transmission and reception
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);                    // Use 8-bit char size
	UBRR0L = USART_BAUDRATE;	                                // Set Baudrate                   
	UBRR0H = (UBRR0L >> 8);	
}

char    uart_rx(void)
{
   	while ((UCSR0A & (1 << RXC0)) == 0);       // Wait till data is received
	return(UDR0);		                       // Return the byte
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
    ADMUX =  (1 << REFS0) | (1 << ADLAR) | (1 << MUX0);
}

uint16_t        adc_read(uint8_t ch)
{
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)); 
    return ADCH;
}

int         main(void)
{
    DDRC = (1 << PC1);
    
    for (;;) {
        PORTC ^= (1 << PC1);
        _delay_ms(i);
        i += 10;
    }
    return 0;
}