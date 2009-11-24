#define F_CPU 1000000UL

#define MEDICIONES 6

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

static void uart_init(void);
static int uart_getchar(FILE *stream);
static int uart_putchar(char dato, FILE *stream);

static unsigned int valores[MEDICIONES];
static int i = 0;


static FILE rs232 = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

static void uart_init(void)
{
	UCSRB = _BV(TXEN) | _BV(RXEN);
	UBRRL = 25;
	stdin = stdout = stderr = &rs232;
}

static int uart_getchar(FILE *stream)
{
	loop_until_bit_is_set(UCSRA, TXC);
	return UDR;
}

static int uart_putchar(char dato, FILE *stream)
{
	loop_until_bit_is_set(UCSRA, UDRE);
	UDR = dato;
	return 0;
}

ISR (INT0_vect)
{
	if (i == 0)
		TCCR1B = _BV(CS12) | _BV(CS10); /* prescaler de 64 */
	else if (i == MEDICIONES) {
		int n;
		TCCR1B = 0;
		TCNT1 = 0;
		i = 0;
		for (n = 0; n < MEDICIONES; n++) {
			printf("%u\r", valores[n]);
		}
		return;
	}
	valores[i++] = TCNT1;
}


void main(void)
{
	uart_init();
	GICR = _BV(INT0);
	sei();
	while(1);
}
