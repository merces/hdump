PREFIX=/usr/local

all:
	gcc -Wall -Wextra -pedantic -o hdump hdump.c

debug:
	gcc -Wall -Wextra -pedantic -DDEBUG -o hdump hdump.c

install:
	install -m 0755 hdump $(PREFIX)/bin/hdump

uninstall:
	rm -f $(PREFIX)/bin/hdump

clean:
	rm -f hdump
