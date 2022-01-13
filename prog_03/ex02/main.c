#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>

void    uart_init(uint8_t USART_BAUDRATE)
{
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);	    // Turn on transmission and reception
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);    // Use 8-bit char size
	UBRR0L = USART_BAUDRATE;	                // Set Baudrate                   
	UBRR0H = (UBRR0L >> 8);		
}

void    uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));	       // Wait for empty transmit buffer
	UDR0 = c;
}

char    uart_rx(void)
{
   	while ((UCSR0A & (1 << RXC0)) == 0);       // Wait till data is received
	return(UDR0);		                       // Return the byte
}

int     main(void)
{
    uart_init(8);
    for (;;) {
        char c = uart_rx();
        uart_tx(c);
    }
    return 0;
}