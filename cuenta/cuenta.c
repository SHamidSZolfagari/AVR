#define F_CPU 1000000UL

#define MAX_MEDICIONES 17

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

static int uart_getchar(FILE *stream);
static int uart_putchar(char dato, FILE *stream);

static unsigned int valores[MAX_MEDICIONES];
static int i = 0;
static int m = 6;

static FILE rs232 = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

ISR (INT0_vect)
{
	if (i == 0)
		TCCR1B = _BV(CS12) | _BV(CS10); /* prescaler de 1024 */
	else if (i == m) {
		TCCR1B = 0;
		TCNT1 = 0;
		i = 0;
		return;
	}
	valores[i++] = TCNT1;
}

static void uart_init(void)
{
	UCSRB = _BV(TXEN) | _BV(RXEN);
	UBRRL = 25;
	stdin = stdout = stderr = &rs232;
}

static int uart_getchar(FILE *stream)
{
	char valor;
	loop_until_bit_is_set(UCSRA, RXC);
	if (UCSRA & (_BV(FE) | _BV(DOR)))
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
	loop_until_bit_is_set(UCSRA, UDRE);
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
	printf("[r] Resetear\n");
	printf("[v] Ver valores\n");
	printf("[c] Cantidad de mediciones (%d)\n", m);
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
	GICR = _BV(INT0);
	sei();
	while(1) {
		switch(menu()) {
			case 'r':
				TCCR1B = 0;
				TCNT1 = 0;
				i = 0;
				break;
			case 'v':
				volcar();
				break;
			case 'c':
				configuracion();
				break;
			default:
				printf("ERROR: opcion invalida\n");
		}
	}
}
