#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>

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

void    uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));	       // Wait for empty transmit buffer
	UDR0 = c;
}

__attribute__((signal))void    USART_RX_vect(void)
{
    PORTB ^= (1 << PB3);
    char c = uart_rx();
    if (c >= 'a' && c <= 'z')
        c -= 32;
    else if (c >= 'A' && c <= 'Z')
        c += 32;
    uart_tx(c);
}

int     main(void)
{
    uart_init(8);

    SREG = (1 << 7);                        // Global interrupt
    for (;;) {}
    return 0;
}