/*
 * Parpadea un "led RGB" en el PIN[0-2]del PORTC
 */

#define F_CPU 1000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRC = 0x07;
	while (1) {
		_delay_ms(500);
		PORTC = 0x01;
		_delay_ms(500);
		PORTC = 0x00;

		_delay_ms(500);
		PORTC = 0x02;
		_delay_ms(500);
		PORTC = 0x00;

		_delay_ms(500);
		PORTC = 0x04;
		_delay_ms(500);
		PORTC = 0x00;
	}
	return 0;
}
