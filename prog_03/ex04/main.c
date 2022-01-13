#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>
#include <string.h>

char        input_username[512];
char        input_passorwd[512];
uint8_t     status = 0;
uint8_t     counter = 0;

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

void    login(void)
{
    uart_printstr("Enter your login:\n\r");
    uart_printstr("\t\033[0;34musername:\033[0;37m ");
}

void    passorwd(void) 
{
    uart_printstr("\n\r");
    uart_printstr("\t\033[0;34mpassword:\033[0;37m ");
}

int    check_valid_login(void)
{
    char        *username = "CyberRoot";
    char        *psswd = "toor1880";
    
    if (!strcmp(username, input_username) && !strcmp(psswd, input_passorwd)) {
        uart_printstr("\n\n\r\e[0;32mHello CyberRoot !\e[0;37m\n\r");
        DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);
        PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);
        return 1;
    } else
        uart_printstr("\n\r\e[0;31mWrong username / password combination\e[0;37m\n\n\r");
    return 0;
}

void    storage_data(char c)
{
    if (status == 0)
        input_username[counter] = c;
    else
        input_passorwd[counter] = c;
    counter++;
}

void    USART_RX_vect(void)
{
    char c = uart_rx();
    if (c == 127 && counter > 0) {
        uart_printstr("\b \b");
        counter--;
    }
    else if (c == 13) {
        if (status == 0) {
            input_username[counter] = '\0';
            status++;
            counter = 0;
            passorwd();
        } else {
            input_passorwd[counter] = '\0';
            if (check_valid_login() == 0) {
                login();
                status = 0;
                counter = 0;
            } else {
                SREG = 0;
                return ;
            }
        }
    } else if (c != 127) {
        status == 0 ? uart_tx(c) : uart_tx('*');
        storage_data(c);
    }
    SREG |= (1 << 7);
}

int     main(void)
{
    uart_init(8);
    login();

    SREG = (1 << 7);
    for (;;) {}
    return 0;
}