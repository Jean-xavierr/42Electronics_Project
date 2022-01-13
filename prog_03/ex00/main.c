#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>

// Using screen to Connect to the Serial Console
// screen /dev/<console_port> 115200

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

int     main(void)
{
	uart_init(8);                               // 115.2k
    uart_tx('Z');
	for (;;) {
    }
    return 0;
}