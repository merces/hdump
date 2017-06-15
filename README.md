# hdump
____________________________________________________

#### Fast, simple and portable hexadecimal/ASCII dumper written in ANSI C.

### 1. Build
    $ make

### 3. Install
    # make install

### 3. Usage
    $ hdump [-c columns] [-s skip] [-n length] file

    OPTIONS
     -c   : number of columns to use before breaking the line (default: 16)
     -s   : number of bytes to skip / starting byte
     -n   : number of bytes to read / maximum number of bytes to print
     -v   : show version and author banner
     -h   : show usage

Prefixing numbers with '0x' makes hdump use them as hexadecimal.

### 4. Examples

Prints the first 16 bytes of /bin/ls file:

    $ hdump -n 16 /bin/ls
    $ hdump -n 0x10 /bin/ls

Start at byte 0xff and prints a maximum of 32 bytes:

    $ hdump -s 0xff -n 32 /bin/ls

### 5. Uninstall
    # make uninstall
