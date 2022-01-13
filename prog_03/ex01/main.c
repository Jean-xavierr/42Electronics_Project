#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>

void    uart_init(uint8_t USART_BAUDRATE)
{
    UCSR0B |= (1 << TXEN0);	                    // Turn on transmission
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);    // Use 8-bit char size
	UBRR0L = USART_BAUDRATE;	                // Set Baudrate                   
	UBRR0H = (UBRR0L >> 8);		
}

void    uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));	       // Wait for empty transmit buffer
	UDR0 = c;
}

void    uart_printstr(char *str)
{
    int i = 0;
    while (str[i])
        uart_tx(str[i++]);
}

void    TIMER1_COMPA_vect(void)
{
    uart_printstr("Hello World!\n\r");
    DDRB = (1 << PB3);
    PORTB ^= ( 1 << PB3);
    SREG = (1 << 7);
}

int     main(void)
{
    uart_init(8);

    TCCR1B |= (1 << WGM12);                 // CTC mode
    TCCR1A |= (1 << COM1A1);                
    TCCR1B |= (1 << CS10) | (1 << CS12);    // prescaler   

    OCR1A = 31250; 
    TIMSK1 = (1 << OCIE1A);
    SREG = (1 << 7);
    for (;;) {}
    return 0;
}