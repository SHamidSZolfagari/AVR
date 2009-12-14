/*
 *  Copyright (c) 2009 Gerardo Exequiel Pozzi <vmlinuz386@yahoo.com.ar>
 *
 *  Permission to use, copy, modify, and distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define F_CPU 1000000UL

#define MAX_MEDICIONES 60 /* 60 mediciones */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

static int uart_getchar(FILE *stream);
static int uart_putchar(char dato, FILE *stream);

static unsigned int valores[MAX_MEDICIONES] = {0};
static int i = 0;
static int m = 10;
static unsigned int x = 0;

static FILE rs232 = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

ISR (INT0_vect)
{
	if (i == 0) {
		TCCR1B |= _BV(CS11); /* prescaler de 8 */
		valores[i++] = 0;
	}
	else if (i < m) {
		valores[i++] = x + 1;
	}
	else {
		TCCR1B &= ~(_BV(CS11)); /* detiene el timer */
		TCNT1 = 0; /* timer a cero */
	}
}

ISR (TIMER1_COMPA_vect)
{
	x++; /* incremento cada un ms */
}

static void uart_init(void)
{
	UCSRB = _BV(TXEN) | _BV(RXEN); /* TX y RX habilitados */
	UBRRL = 12; /* 12 = 4800 bps a 1MHz */
	stdin = stdout = stderr = &rs232;
}

static int uart_getchar(FILE *stream)
{
	char valor;
	loop_until_bit_is_set(UCSRA, RXC); /* espero datos */
	if (UCSRA & (_BV(FE) | _BV(DOR))) /* datos OK ? */
		return _FDEV_ERR;
	else
		if ((valor = UDR) == '\r')
			return '\n';
		else
			return valor;
}

static int uart_putchar(char dato, FILE *stream)
{
	if (dato == '\n')
		uart_putchar('\r', stdout);
	loop_until_bit_is_set(UCSRA, UDRE); /* espero que este libre el TX */
	UDR = dato;
	return 0;
}

static void volcar(void)
{
	int n;
	printf("\n");
	for (n = 0; n < m; n++)
		printf("%u\n", valores[n]);
}

static int menu(void)
{
	printf("\n");
	printf("[r] Reiniciar\n");
	printf("[c] Cantidad de mediciones (%d)\n", m);
	printf("[v] Volcar valores\n");
	printf("\n? ");
	
	return getchar();
}

static void configuracion(void)
{
	int mm;
	char buf[4];
	printf("\n(c) [1, %d] ? ", MAX_MEDICIONES);
	fgets(buf, sizeof(buf), stdin);
	sscanf(buf, "%d", &mm);
	if (mm > 0 && mm <= MAX_MEDICIONES)
		m = mm;
	else
		printf("\nERROR: %d, fuera de rango [1, %d]\n", x, MAX_MEDICIONES);
}

void main(void)
{
	uart_init();
	PORTD = _BV(PD2); /* resistor de pull-up */
	MCUCR = _BV(ISC01); /* int0 por flanco H->L */
	GICR = _BV(INT0); /* int0 habilitada */
	TIMSK = _BV(OCIE1A); /* interrupion por comparacion */
	TCCR1B = _BV(WGM12); /* modo de borrado del timer cuando cuando la comparacion coincide */
	OCR1A = 124; /* cuento 125 ints de timer 1MHz / 8 = 125000 da un margen de 1000 overflows */
	sei();
	while(1) {
		switch(menu()) {
			case 'c':
				configuracion();
			case 'r':
				TCCR1B &= ~(_BV(CS11)); /* detiene el timer */
				TCNT1 = 0; /* timer a cero */
				for (i = 0; i < MAX_MEDICIONES; i++)
					valores[i] = 0;
				i = 0;
				x = 0;
				break;
			case 'v':
				volcar();
				break;
			default:
				printf("\n ERROR: opcion invalida\n");
		}
	}
}
