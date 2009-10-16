/*
 * Parpadea un "led RGB" en el PIN[0-2]del PORTC
 */

#define F_CPU 1000000L
#include <avr/io.h>
#include <util/delay.h>

#define RETARDO 500

#define R 0x01
#define G 0x02
#define B 0x04

int main(void)
{
    DDRC = 0x07;
    
    while (1) {
	PORTC = 0x00;
	_delay_ms(1000);

	PORTC = R;
	_delay_ms(500);
	PORTC = G;
	_delay_ms(500);
	PORTC = B;
	_delay_ms(1000);

	PORTC = R|G;
	_delay_ms(500);
	PORTC = R|B;
	_delay_ms(500);
	PORTC = G|B;
	_delay_ms(1000);

	PORTC = R|G|B;
	_delay_ms(1000);
    }

    return 0;
}
