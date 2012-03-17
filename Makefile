all:
	gcc -O2 -W -Wall -Wextra -ansi -pedantic -o hdump hdump.c

clean:
	rm -f hdump
