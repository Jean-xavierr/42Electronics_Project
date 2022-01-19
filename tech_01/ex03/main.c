#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdlib.h>

#define DELAY 50000
uint8_t read_hex = 0;
uint8_t counter = 0;
char    input_hex[10];

void    uart_init(uint8_t USART_BAUDRATE)
{
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);	    // Turn on transmission and reception
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);                    // Use 8-bit char size
	UBRR0L = USART_BAUDRATE;	                                // Set Baudrate                   
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

char    uart_rx(void)
{
   	while ((UCSR0A & (1 << RXC0)) == 0);       // Wait till data is received
	return(UDR0);		                       // Return the byte
}

void                           set_input(void)
{
    char red[3] = {input_hex[0], input_hex[1] + '\0'};
    char green[3] = {input_hex[2], input_hex[3] + '\0'};
    char blue[3] = {input_hex[4], input_hex[5] + '\0'};
    OCR0A = (uint8_t)strtol(red, NULL, 16);;
    OCR0B = (uint8_t)strtol(green, NULL, 16);
    OCR2B = (uint8_t)strtol(blue, NULL, 16);
}

__attribute__((signal))void    USART_RX_vect(void)
{
    PORTB ^= (1 << PB3);
    char c = uart_rx();
    if (c == '#') {
        read_hex = 1;
    } else if (read_hex > 0) {
        input_hex[counter++] = c;
        if (counter == 6) {
            counter = 0;
            read_hex = 0;
            uart_printstr(input_hex);
            set_input();
        }
    }
    uart_tx(c);
}

int             main(void)
{
    uart_init(8);

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

    SREG |= (1 << 7);
    for (;;) {}
    return 0;
}