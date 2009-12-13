#define F_CPU 7372800UL

#include <avr/io.h>

void enviar_data(unsigned char dato)
{
	/* Espero que se vacie el registro de datos */
	while(!(UCSRA & _BV(UDRE)));
	/* Envio el dato */
	UDR = dato;
}

unsigned char recibir_data(void)
{
	/* Espero que se vacie el registro de datos */
	while(!(UCSRA & _BV(RXC)));
	/* Envio el dato */
	return UDR;
}

void main (void)
{
	UBRRL = 47; /* 9600 a 7.3728 MHz */
	UCSRB = (1 << RXEN) | (1 << TXEN);
	while(1) {
		enviar_data(recibir_data());
	}
}
