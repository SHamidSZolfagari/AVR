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

	printf("static unsigned char __attribute__ ((progmem)) Font5x7[] = {");
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
