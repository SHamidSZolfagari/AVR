/*
 * Jugando con la matrix de leds de 7x5
 * PC[0-4] x nivel alto
 * PD[0-6] y nivel bajo
 */

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "dado5x7.h"

#define MAX_X 5
#define MAX_Y 7

int main(void)
{
	DDRC = 0x1f; /* PC[0-4] salidas x nivel alto */
	DDRD = 0x7f; /* PD[0-6] salidas y nivel bajo */

	int x, y;
	int i, t;
	
	while (1) {
		for (i = 0; i < 6; i++) {
			for (t = 0; t < 60; t++) {
				for (x = 0; x < 5; x++) {
					PORTC = 0x00;
					PORTC = (1 << x);
					PORTD = ~(pgm_read_byte(&(Font5x7[i*5 + x])));
					_delay_ms(1);
				}
			}
		}
	}

	return 0;
}
