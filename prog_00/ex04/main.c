#include <avr/io.h>
typedef unsigned char uint8_t;

int     main(void)
{
    uint8_t pin_status;
    DDRB = (1 << PB3);

    for (;;) {
        pin_status = PIND & (1 << PD3);	/* Read status of pin PD3 */
        if (pin_status) 	            /* Transmit status of pin PD3 on to pin PB3 to drive LED. */       
           PORTB = (0 << PB3);          /* Switch is closed, pin_status = 1, LED is OFF */
        else		
           PORTB = (1 << PB3); 	        /* Switch is open, pin_status = 0, LED is ON */
    };
    return 0;
}