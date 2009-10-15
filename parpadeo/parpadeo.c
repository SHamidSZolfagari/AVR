/*
 * Parpadea un "led" en el PIN0 del PORTC
 */

#define F_CPU 1000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRC = 0x01;
	while (1) {
		_delay_ms(500);
		PORTC = 0x01;
		_delay_ms(500);
		PORTC = 0x00;
	}
	return 0;
}
