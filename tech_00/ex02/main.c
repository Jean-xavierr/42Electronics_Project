#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>
#include <avr/eeprom.h>
#include <string.h>

#define DELAY 25000

char        command[512];
int         counter = 0;

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

void    uart_printstr(char *str)
{
    int i = 0;
    while (str[i])
        uart_tx(str[i++]);
}

__attribute__((signal))void    USART_RX_vect(void)
{
    char c = uart_rx();
    uart_tx(c);
    if (c == 127 && counter > 0) {
        uart_printstr("\b \b");
        counter--;
    }
    else if (c == 13) {
        if (strncmp(command, "write", 4)) {
            uint16_t address = 0;
            for (int i = 6; i < 1024 && i < counter; i++) {
                eeprom_update_word((uint16_t *)address, command[i]);
                address++;
            }
        }
        counter = 0;
    } else {
        command[counter] = c;
        counter++;
    }
}

int             main(void)
{
    uart_init(8);
    uint8_t new_status;
    uint8_t old_status = PIND & (1 << PD2);

    SREG = (1 << 7);
    for (;;)  {
        new_status = PIND & (1 << PD2);
        if (new_status != old_status) {
            for (long i = 0; i < DELAY; i++) {}
            if (new_status == 0) {
                for (int i = 0; i < 1024; i++) {
                    char c = eeprom_read_word((uint16_t *) i);
                    if (c >= 32 && c <= 126)
                        uart_tx(c);
                }
            }
        }

        old_status = new_status;
    }
    return 0;
}