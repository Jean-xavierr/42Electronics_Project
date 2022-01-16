#ifndef __AVR_ATmega328P
#define __AVR_ATmega328P
#endif

#include <avr/io.h>

#define REBOUNCE_DELAY 100000
#define PIN_STATE(REG, PIN) ((REG & (1 << PIN)) >> PIN)
#define ON 0
#define OFF 1
#define TRUE 1
#define FALSE 0

uint8_t start = FALSE;
uint8_t counter = 0;
uint8_t end = FALSE;
uint8_t slave = TRUE;
uint8_t terminated = FALSE;
uint8_t pushed = FALSE;
uint8_t player_is_ready = FALSE;

void    uart_init(uint16_t USART_BAUDRATE)
{
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);	    // Turn on transmission, reception 
	UBRR0L = USART_BAUDRATE & 0xFF;	                            // Set Baudrate                   
	UBRR0H = (USART_BAUDRATE >> 8);		
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

void                       variable_reset(void)
{
    start = FALSE;
    counter = 0;
    end = FALSE;
    slave = TRUE;
    terminated = FALSE;
    pushed = FALSE;
    player_is_ready = FALSE;
    PORTB = 0;
}

void                       button(void)
{
    if (!start) {
        uart_tx('S');
        if (!player_is_ready) {
            slave = FALSE;
            player_is_ready = TRUE;
        } else {
            start = TRUE;
            TIMSK1 = 1 << OCIE1A;
        }
    } else if (!terminated) {
        if (slave == TRUE) {
            uart_tx('P');
        } else if (slave == FALSE) {
            TIMSK1 = 0;
            counter = 5;
            if (!end) {
                uart_tx('W');
                PORTB = 1 << PB0;
            } else {
                uart_tx('L');
                PORTB = 1 << PB3;
            }
            terminated = TRUE;
            for (uint32_t timer = REBOUNCE_DELAY; timer > 0; --timer) {}
        }
    } else {
        uart_tx('R');
        variable_reset();
    }
}

__attribute__((signal))void    USART_RX_vect(void)
{
    char c = uart_rx();
    if (c == 'S') {
        if (player_is_ready == TRUE) {
            start = TRUE;
            TIMSK1 = 1 << OCIE1A;
        } else {
            player_is_ready = TRUE;
        }
    } else if (!terminated) {
        if (slave == FALSE) {
            TIMSK1 = 0;
            counter = 5;
            if (!end) {
                uart_tx('L');
                PORTB = 1 << PB3;
            } else {
                uart_tx('W');
                PORTB = 1 << PB0;
            }
            terminated = TRUE;
            for (uint32_t timer = REBOUNCE_DELAY; timer > 0; --timer) {}
        } else if (slave == TRUE) {
            TIMSK1 = 0;
            counter = 5;
            if (c == 'W') {
                PORTB = 1 << PB3;
            } else if (c == 'L') {
                PORTB = 1 << PB0;
            }
            terminated = TRUE;
            for (uint32_t timer = REBOUNCE_DELAY; timer > 0; --timer) {}
        }  
    } else if (c == 'R') {
        variable_reset();
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
    uart_init(103);
    DDRB = 1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB3;

    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    OCR1A = F_CPU / 1024;

    SREG = (1 << 7);        // Global interrupt

    uint32_t timer = 0;
    uint8_t current_button_state = OFF;
    uint8_t previous_button_state = OFF;
    for (;;) {
        current_button_state = PIN_STATE(PIND, PD3);
        if (current_button_state != previous_button_state) {
            if (current_button_state == ON) {
                button();
            }
            previous_button_state = current_button_state;
            for (timer = REBOUNCE_DELAY; timer > 0; --timer) {}
        }
    }
    return 0;
}