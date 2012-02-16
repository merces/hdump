/*
	hdump - hexa/ascii file dumper

	Copyright (C) 2010 - 2011 Fernando Mercês

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#define BANNER \
puts("hdump 2.0\nwritten by Fernando Mercês <fernando@mentebinaria.com.br>");

#define USAGE \
printf("Usage:\n\thdump [-b START_BYTE] [-n NUMBER_OF_BYTES] <file>\n");

/* Define o numero de colunas na exibicao */
#define COLS 16

unsigned int parse_num(char *s)
{
	unsigned int val;

	if (strstr(s, "0x") != NULL)
		sscanf(s+2, "%x", &val);
	else
		val = atoi(s);

	return val;
}

int main(int argc, char * argv[])
{
	FILE *arq;
	unsigned char v[COLS];
	register unsigned int i;
	unsigned int lidos, end=0;
	char ascii[COLS+1] = {'\0'};
	unsigned int byte=0;
	unsigned int nbytes=0;
	int c;

	if (argc < 2)
	{
		USAGE
		return 1;
	}

	while ((c = getopt (argc, argv, "b:n:vh")) != -1)
	{
		switch (c)
		{
			case 'b':
				byte = parse_num(optarg); break;
			case 'n':
				nbytes = atoi(optarg); break;
			case 'v':
				BANNER return 0;
			case 'h':
				USAGE	return 0;
			default:
				USAGE	return 1;
		}
	}

	if ( (arq = fopen(argv[argc-1], "rb")) == NULL )
	{
		fprintf(stderr, "file not found or without read access.\n");
		return 1;
	}

	/* anda 'byte' posicoes para frente (-b) */
	fseek(arq, byte, SEEK_SET);

	do
	{
		lidos = (int) fread(v, sizeof(char), COLS, arq);
		for (i=0; i<lidos; i++)
		{
			/* imprime o offset */
			if (!i)
				printf("%08x  ", (unsigned int) i+end+byte);

			/* testa se o byte e imprimivel e o coloca no vetor ascii. Depois
			exibe o byte hexa seguido de um ou dois espacos (se estiver na metade de COLS) */
			ascii[i] = isprint(v[i]) ? v[i] : '.';
			printf("%02x%*c", (unsigned int)v[i], (i+1 == COLS/2) ? 2 : 1, ' ');

			/* define o fim do array ascii */
			if (lidos < COLS)
				ascii[lidos] = '\0';

			/* necessario para imprimir os ASCII na ultima iteracao */
			if (i == lidos - 1)
				printf(" |%s|\n", ascii);
		}
		/* atualiza o numero de endereços lidos */
		/*printf("\n");*/
		end += lidos;

		/* se nbytes esta setado e end ja ultrapassou, para */
		if (nbytes && end >= nbytes) break;
	} while (lidos);

	(void) fclose(arq);
	return 0;
}
