/*
	hdump - simple hexa/ascii file dumper

	Copyright (C) 2010 - 2015 Fernando Mercês

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

#ifdef __unix__
#define _FILE_OFFSET_BITS 64

#include <unistd.h>

#endif /*__unix__ */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#define VERSION "2.4"
#define BANNER puts("hdump "VERSION" by Fernando Merces - mentebinaria.com.br")
#define USAGE fatal("Usage:\n\thdump [-c columns] [-s skip] [-n length] file")

void fatal(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}


/* This function handles all the output space characters number calculation */
int get_spaces(int bread, int cols)
{
	int spaces = 0;

	if (bread < cols)
	{
		spaces = cols*3 - bread*3 + 1;

		if (bread < (cols/2))
			spaces += 1;
	}

	return spaces;
}

int main(int argc, char *argv[])
{
	FILE *file;
	unsigned char *buff, *ascii;
	register unsigned int i;
	size_t cols;
	unsigned long int bread, skip, length, address;
	int c;

	bread = skip = length = address = cols = 0;

	if (argc < 2)
		USAGE;

	while ((c = getopt (argc, argv, "c:s:n:vh")) != -1)
	{
		switch (c)
		{
			case 'c':
				cols = (size_t) strtoul(optarg, NULL, 0); break;
			case 's':
				skip = (long) strtoul(optarg, NULL, 0); break;
			case 'n':
				length = strtoul(optarg, NULL, 0); break;
			case 'v':
				BANNER; exit(0);
			case 'h':
				USAGE;
			default:
				USAGE;
		}
	}

	if (!cols)
		cols = 16;

	if (!(file = fopen(argv[argc-1], "rb")))
		fatal("file not found or not readable");

#ifdef __unix__

 	/*
	 * Caso o arquivo de entrada seja na verdade uma
	 * pseudo-tty como /dev/stdin.
	 */
	if (isatty (fileno (file))) {
		for ( ;skip > 0; skip --) {
			if (fgetc (file) == EOF) 
				fatal ("skipping too much");
		}
	}

#else 

	/* anda #skip posicoes para frente (-s) */
	if (fseek(file, skip, SEEK_SET))
		fatal("unable to seek through file");

#endif /* __unix__ */

	buff = (unsigned char *) calloc (1, sizeof(unsigned char) * cols);
	ascii = (unsigned char *) calloc (1, (sizeof(unsigned char) * cols) + 1);

	if (!buff || !ascii)
		fatal("not enough memory");

	do
	{
		bread = (int) fread(buff, sizeof(char), cols, file);
		for (i=0; i<bread; i++)
		{
			/* imprime o offset */
			if (!i)
				printf("%08lx  ", i+address+skip);

			/* testa se o byte e imprimível e o coloca no vetor ascii */
			*(ascii+i) = isprint(*(buff+i)) ? *(buff+i) : '.';

			/* imprime os bytes separados por espaço */
			printf("%02x%*c", (unsigned int) *(buff+i), (i+1 == cols/2) ? 2 : 1, ' ');

			/* define o fim do array ascii (sera usado como string) */
			*(ascii+bread) = '\0';

			/* imprime os caracteres ascii */
			if (i == bread-1)
				printf("%*c|%s|\n", get_spaces(bread, cols), ' ', ascii);
		}
		/* atualiza o numero de endereços lidos */
		address += bread;

		/* se a opcao -n foi utilizada, para quando atingir #length */
		if (length && (address >= length))
			break;
	} while (bread);

	free(buff);
	free(ascii);
	fclose(file);
	return 0;
}
