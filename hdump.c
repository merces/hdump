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
#include <sys/select.h>

/**
 * **@Nota:
 * 	IN é definido aqui dessa forma, 
 * para evitar o uso de #ifdefs pelo codigo
 */
#define IN 		"/dev/stdin"

#else  /* WINDOWS */ 

#define IN 		""

#endif /*__unix__ */



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <ctype.h>
#include <getopt.h>

#define VERSION "2.4"
#define BANNER puts("hdump "VERSION" by Fernando Merces - mentebinaria.com.br")
#define USAGE fatal("Usage:\n\thdump [-c columns] [-s skip] [-n length] file")


void fatal(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

/**
 * **@Nota:
 * 	Testa se há algo no buffer de entrada.
 */
bool
available_input (void)
{
#ifdef __unix__
	FILE * in;
	bool available;
	struct timeval tv;
	fd_set fds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if (!(in = fopen ("/dev/stdin", "rb")))
			fatal ("checking standart input");
	

	FD_ZERO (&fds);
	FD_SET (fileno (in), &fds);
	select ((fileno (in) + 1), &fds, NULL, NULL, &tv);
	available = FD_ISSET (fileno (in), &fds);

	fclose (in);

	return available;
#else 
	return false;
#endif /* __unix__ */
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
	FILE *file = NULL;
	unsigned char *buff, *ascii;
	register unsigned int i;
	size_t cols;
	unsigned long int bread, skip, length, address;
	int c;
	bool in = false;

	bread = skip = length = address = cols = 0;

	/**
	 * **@Nota:
	 * 	Caso seja windows, apenas imprime USAGE
	 * 	caso contrário, testa se tem algo no buffer
	 * 	de entrada, se tiver, assume que deve ler de lah
	 * 	ja que nenhum arquivo ou opção foi provida 
	 * 	pelo usuário.
	 */
	in = available_input ();
	if (argc < 2)
	{
		if (!in)
			USAGE;
	} 
 	if (!in && (strstr (argv [argc-2], "-")))
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



	/**
	 * **@Nota:
	 * 	caso seja windows, apenas tenta abrir o arquivo.
	 * 	Caso contrário, testa se há algo disponivel no buffer de entrada
	 * 	se sim, assume que deve ler de lá, evitando confundir as argumentos
	 * 	passados.
	 */

	if (!(file = fopen ((in ? IN : argv[argc-1]), "rb")))
		fatal("file not found or not readable");
		
	if (!in) {
		fseek (file, 0, SEEK_END);
		if (!ftell (file)) {
			fatal ("Could not read the file: file is empty");
		}
		rewind (file);
	}

	


	/* anda #skip posicoes para frente (-s) */
	for ( ;skip > 0; skip --) {
		if (fgetc (file) == EOF) 
			fatal ("skipping too much");
	}

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
