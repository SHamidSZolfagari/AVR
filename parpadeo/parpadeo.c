/*
 * Parpadea un "led" en el PIN1 del PORTB
 */

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 0x02;
	while (1) {
		_delay_ms(500);
		PORTB = 0x02;
		_delay_ms(500);
		PORTB = 0x00;
	}
	return 0;
}
