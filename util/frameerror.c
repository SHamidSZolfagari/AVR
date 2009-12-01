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
 *  gcc -Wall -ansi -pedantic frameerror.c -o frameerror -lm
 */

#include <stdio.h>
#include <math.h>

#define ERROR_MAX 1.0 /* Máximo error admitido */

#define N_BAUDIOS 9
int baudio[] = {300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};

#define N_CLOCKS 8
int clock[] = {1000000, 2000000, 4000000, 8000000, 1843200, 2457600, 3686400, 7372800};

#define UBRR_BITS 16.0F /* Tamaño del registro URBB */

int main(void)
{
	int b, c;
	float r;
	float err;
	
	for (b = 0; b < N_BAUDIOS; b++) {
		if (b == 0) {
			printf("bps\\hz");
			for (c = 0; c < N_CLOCKS; c++) {
				printf("\t%15d", clock[c]);
			}
			printf("\n");
		}
		printf("%d\t", baudio[b]);
		for (c = 0; c < N_CLOCKS; c++) {
			r = clock[c] / (UBRR_BITS * baudio[b]) - 1;
			if (r - floor(r) < ceil(r) - r) {
				err = (1.0 - baudio[b] / (clock[c] / (UBRR_BITS * (floor(r) + 1.0)))) * 100.0;
				if (err < ERROR_MAX && floor(r) < 256) {
					printf("%8d (%.2f)\t", (int) floor(r), err);
				}
				else {
					printf("%8c (-.--)\t", '-');
				}
			} else {
				err = (baudio[b] / (clock[c] / (UBRR_BITS * (ceil(r) + 1.0))) - 1.0) * 100.0;
				if (err < ERROR_MAX && ceil(r) < 256) {
					printf("%8d (%.2f)\t", (int) ceil(r), err);
				}
				else {
					printf("%8c (-.--)\t", '-');
				}
			}
		}
		printf("\n");
	}
	return(0);
}
