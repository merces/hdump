all:
	gcc -Wall -Wextra -pedantic -ansi -o hdump hdump.c 

install:
	install -m 0755 hdump /usr/local/bin/hdump

uninstall:
	rm -f /usr/local/bin/hdump

clean:
	rm -f hdump
