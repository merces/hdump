all:
	gcc -O2 -Wall -Wextra -ansi -pedantic -o hdump hdump.c

clean:
	rm -f hdump
