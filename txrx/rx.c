#define F_CPU 1000000
#define BAUD 2400
#define UBRR F_CPU/16/BAUD-1

#include <avr/io.h> 

void config_usart(void)
{
	/* Configuro la velocidad en baudios */
	UBRRL = (UBRR);
	UBRRH = ((UBRR) >> 8);
	/* Habilito el TX */
	UCSRB = _BV(RXEN);
	/* frame de 8 bits */
	UCSRC = _BV(URSEL) | _BV(UCSZ0) | _BV(UCSZ1);
}

unsigned char recibir_data(void)
{
	/* Espero que se vacie el registro de datos */
	while(!(UCSRA & _BV(RXC)));
	/* Envio el dato */
	return UDR;
}

void main(void)
{
	DDRB = 0x07;

	config_usart();
	while(1) {
		PORTB = recibir_data() & 0x07;
	}
}
