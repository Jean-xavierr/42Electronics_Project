#include <avr/io.h>

int     main(void)
{
    DDRB = (1 << PB3);
    DDRD = (0 << PD3);
    PORTD = PORTD | (1 << PD3);
    int pin_status;
    for (;;) {
        pin_status = PIND & (1 << PD3);	/*Read status of pin PD3 */
        if(pin_status)			/* Transmit status of pin PD3 on to pin PD3 to drive LED. */
        {
           PORTD = PORTD | (1 << PD3);	/* Switch is open, pin_status = 1, LED is ON */
           PORTB = (0 << PB3);
        }
        else
        {		
           PORTD = PORTD & (~(1<<3));;	/* Switch is closed, pin_status = 0, LED is OFF */
           PORTB = (1 << PB3);
        }
    };
    return 0;
}