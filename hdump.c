/*
	hdump - hexa/ascii file dumper

	Copyright (C) 2010 - 2012 Fernando Mercês

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
puts("hdump 2.1\nwritten by Fernando Mercês <fernando@mentebinaria.com.br>");

#define USAGE \
printf("Usage:\n\thdump [-s skip] [-n length] file\n");

/* define o numero de colunas na exibição */
#define COLS 16

unsigned int parse_num(char *s)
{
	unsigned int val;

	/* interpreta como hexa se for prefixado com 0x */
	if (strstr(s, "0x"))
		sscanf(s+2, "%x", &val);
	else
		val = atoi(s);

	return val;
}

int main(int argc, char *argv[])
{
	FILE *file;
	unsigned char buff[COLS];
	register unsigned int i;
	unsigned int read, skip, length, address;
	unsigned char ascii[COLS+1] = {'\0'};
	int c;

	read = skip = length = address = 0;

	if (argc < 2)
	{
		USAGE
		return 1;
	}

	while ((c = getopt (argc, argv, "s:n:vh")) != -1)
	{
		switch (c)
		{
			case 's':
				skip = parse_num(optarg); break;
			case 'n':
				length = parse_num(optarg); break;
			case 'v':
				BANNER; return 0;
			case 'h':
				USAGE; return 0;
			default:
				USAGE; return 1;
		}
	}

	if ((file = fopen(argv[argc-1], "rb")) == NULL)
	{
		fprintf(stderr, "file not found or without read access.\n");
		return 1;
	}

	/* anda 'skip' posicoes para frente (-s) */
	fseek(file, skip, SEEK_SET);

	do
	{
		read = (int) fread(buff, sizeof(char), COLS, file);
		for (i=0; i<read; i++)
		{
			/* imprime o offset */
			if (!i)
				printf("%08x  ", (unsigned int) i+address+skip);

			/* testa se o byte e imprimível e o coloca no vetor ascii */
			ascii[i] = isprint(buff[i]) ? buff[i] : '.';

			/* imprime os bytes separados por espaço */
			printf("%02x%*c", (unsigned int) buff[i], (i+1 == COLS/2) ? 2 : 1, ' ');

			/* define o fim do array ascii (sera usado como string) */
			if (read < COLS)
				ascii[read] = '\0';

			/* imprime os caracteres ascii */
			if (i == read-1)
				printf(" |%s|\n", ascii);
		}
		/* atualiza o numero de endereços lidos */
		address += read;

		/* se a opcao -n foi utilizada, para quando atingir */
		if (length && address >= length)
			break;
	} while (read);

	fclose(file);
	return 0;
}
