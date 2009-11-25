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

#define MAX_MEDICIONES 17 /* 17 mediciones sin sobrepasar el timer a pulsos de 1Hz */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

static int uart_getchar(FILE *stream);
static int uart_putchar(char dato, FILE *stream);

static unsigned int valores[MAX_MEDICIONES] = {0};
static int i = 0;
static int m = 6;
static int d = 0;

static FILE rs232 = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

ISR (INT0_vect)
{
	if (d == 1)
		printf("%d\n", i);
	if (i == 0) {
		TCCR1B = _BV(CS12) | _BV(CS10); /* prescaler de 1024 */
	}
	else if (i == m) {
		TCCR1B = 0; /* detiene el timer */
		TCNT1 = 0; /* timer a cero */
		return;
	}
	valores[i++] = TCNT1;
}

static void uart_init(void)
{
	UCSRB = _BV(TXEN) | _BV(RXEN); /* TX y RX habilitados */
	UBRRL = 25; /* 2400 baudios a 1MHz */
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
	for (n = 0; n < m; n++) {
		printf("%u\n", valores[n]);
	}
}

static int menu(void)
{
	printf("\n");
	printf("[r] Reiniciar\n");
	printf("[v] Volcar valores\n");
	printf("[c] Cantidad de mediciones (%d)\n", m);
	printf("[m] {debug} Mostrar timer1\n");
	printf("[d] {debug} Mostrar int0 (%d)\n", d);
	printf("\n? ");
	
	return getchar();
}

static void configuracion(void)
{
	int x;
	char buf[4];
	printf("\n(c) ? ");
	fgets(buf, sizeof(buf), stdin);
	sscanf(buf, "%d", &x);
	if (x > 0 && x < 18) {
		m = x;
		TCNT1 = 0;
		i = 0;
	}
	else
		printf("\nERROR: %d, fuera de rango [1, 17]\n", x);
}

void main(void)
{
	uart_init();
	PORTD = _BV(PD2); /* resistor de pull-up */
	MCUCR = _BV(ISC01); /* int0 por flanco H->L */
	GICR = _BV(INT0); /* int0 habilitada */
	sei();
	while(1) {
		switch(menu()) {
			case 'r':
				TCCR1B = 0; /* detengo el timer */
				TCNT1 = 0; /* timer a cero */
				for (i = 0; i < MAX_MEDICIONES; i++)
					valores[i] = 0;
				i = 0;
				break;
			case 'v':
				volcar();
				break;
			case 'c':
				configuracion();
				break;
			case 'm':
				printf("\n%u\n", TCNT1);
				break;
			case 'd':
				d = !d;
				break;
			default:
				printf("\n ERROR: opcion invalida\n");
		}
	}
}
