#define F_CPU 7372800UL

#include <avr/io.h>
#include <avr/interrupt.h>

ISR(USART_RXC_vect)
{
	char c = UDR;
	UDR = c;
}

void main (void)
{
	UBRRL = 47; /* 9600 a 7.3728MHz */
	UCSRB = _BV(RXEN) | _BV(TXEN) | _BV(RXCIE);
	sei();
	while(1);
}
