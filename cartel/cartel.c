#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "font5x7.h"

#define DS	_BV(PINC0)
#define SH_CP	_BV(PINC1)
#define ST_CP	_BV(PINC2)

void main(void)
{
	DDRC = 0x07;
	DDRB = 0x7F;
	
	int x,y,d;
	
	char msg[] = "Linux :)";

	while(1) {
		for (x = 0; x < 40; x++) {
			if (x == 0) {
				y = -1;
				PORTC ^= DS;
				PORTC ^= SH_CP;
				PORTC ^= ST_CP;
				PORTC ^= DS;
			} else {
				PORTC ^= SH_CP;
				PORTC ^= ST_CP;
			}
			PORTC ^= SH_CP;
			PORTC ^= ST_CP;
			if (x % 5 == 0) {
				y++;
			}
			PORTB = ~(pgm_read_byte(&(Font5x7[(msg[y] - 0x20)*5 + x % 5])));
			_delay_ms(1);
			PORTB = 0x7F;
		}
	}
}
