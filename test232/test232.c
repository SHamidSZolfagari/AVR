#define F_CPU 7372800UL

#include <avr/io.h>
#include <util/delay.h>

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
	unsigned char data=0;
	UBRRL = 47; /* 9600 a 7.3728 MHz */
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
	while(1) {
		data = recibir_data();
		enviar_data(data);
	}
}
