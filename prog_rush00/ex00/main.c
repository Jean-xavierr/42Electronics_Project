#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>

#define TRUE 1
#define FALSE 0

uint8_t start = FALSE;
uint8_t counter = 0;
uint8_t end = FALSE;
uint8_t slave = FALSE;
uint8_t terminated = FALSE;
uint8_t pushed = FALSE;

void    uart_init(uint16_t USART_BAUDRATE)
{
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);	    // Turn on transmission, reception and  
	// UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);                 // Use 7-bit char size (p.203)
	UBRR0L = USART_BAUDRATE & 0xFF;	                            // Set Baudrate                   
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

__attribute__((signal))void INT1_vect(void)
{
    if (!start) {
        TIMSK1 = 1 << OCIE1A;
        uart_tx('S');
        start = TRUE;
    } else if (!terminated) {
        if (slave == TRUE) {
            uart_tx('P');
        } else if (slave == FALSE) {
            TIMSK1 = 0;
            counter = 5;
            if (!end) {
                uart_tx('W');
                PORTB = 1 << PB0;
            } else if (pushed == TRUE) {
                uart_tx('W');
                PORTB = 1 << PB0;
            } else {
                uart_tx('L');
                PORTB = 1 << PB3;
            }
            terminated = TRUE;
        }
    }
    for (long i = 0; i < 200000; i++) {} // effects rebond
}

__attribute__((signal))void    USART_RX_vect(void)
{
    char c = uart_rx();
    if (!start && c == 'S') {
        TIMSK1 = 1 << OCIE1A;
        start = TRUE;
        slave = TRUE;
    } else if (!terminated) {
        if (slave == FALSE) {
            pushed == TRUE;
        } else if (slave == TRUE) {
            TIMSK1 = 0;
            counter = 5;
            if (c == 'W') {
                PORTB = 1 << PB3;
            } else if (c == 'L') {
                PORTB = 1 << PB0;
            }
            terminated = TRUE;
        }
        
    }
}

__attribute__((signal))void     TIMER1_COMPA_vect(void)
{
    uint8_t port;
    if (counter == 0) {
        port = 1 << PB3;
    } else if (counter == 1) {
        port = 1 << PB2;
    } else if (counter == 2) {
        port = 1 << PB1;
    } else if (counter == 3) {
        port = 1 << PB0;
    } else if (counter == 4) {
        port = 1 << PB3 | 1 << PB2 | 1 << PB1 | 1 << PB0;
        end = 1;
    } else {
        port = 0;
        TIMSK1 = 0;
    }
    PORTB = port;
    ++counter;
}

int         main(void)
{
    uart_init(8);
    DDRB = 1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB3;

    EICRA = (1 << ISC11);   // External Interrupt Control Register
    EIMSK = (1 << INT1);    // External Interrupt Mask Register

    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    OCR1A = F_CPU / 1024;

    SREG = (1 << 7);        // Global interrupt

    for (;;) {
        // if (start) {
        //     TIMSK1 = 1 << OCIE1A;
        // } 
    }
    return 0;
}