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

/*
 *  gcc -Wall -ansi -pedantic font2hexa.c -o font2hexa
 */

#include <stdio.h>

#define BUFLEN		10
#define MAX_LETRAS	256
#define TAM_LETRA_X	5
#define TAM_LETRA_Y	7

int main()
{
	char linea[BUFLEN];
	unsigned int n, x, y;
	unsigned char letra[TAM_LETRA_X];

	printf("static unsigned char __attribute__ ((progmem)) Font[] = {");
	for (n = 0; n < MAX_LETRAS && !feof(stdin); n++) {
		printf("\n\t");
		for (y = 0; y < TAM_LETRA_Y; y++) {
			if (y == 0)
				for (x = 0; x < TAM_LETRA_X; x++)
					letra[x] = 0;
			fgets(linea, BUFLEN, stdin);
			for (x = 0; x < TAM_LETRA_X && linea[x] != '\0'; x++)
				if (linea[x] == '*')
					letra[x] |= 1 << y;
		}
		fgets(linea, BUFLEN, stdin);
		for (x = 0; x < TAM_LETRA_X; x++)
			printf("0x%02X,", letra[x]);
	}
	printf("\n};\n");

	return 0;
}
