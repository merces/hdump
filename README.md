hdump 2.3
=========

Fast, simple and portable hexadecimal/ASCII
dumper for binary files, written in ANSI C.
Visit http://hdump.sf.net for 

###1. Build
    $ make

###3. Install
    # make install

###3. Usage
    $ hdump [-c columns] [-s skip] [-n length] file

    OPTIONS
     -c   : number of columns to print dump (default: 16)
     -s   : number of bytes to be skipped / start byte
     -n   : length of bytes to read / minimum number of bytes to print
     -v   : show version and author banner
     -h   : show usage

Prefixing numbers with '0x' make hdump use it as hexadecimal.

###4. Examples

 Print the first 16 bytes of /bin/ls file:
    $ hdump -n 16 /bin/ls
    $ hdump -n 0x10 /bin/ls

 Start at byte 0xff and print a maximum of 32 bytes after:
    $ hdump -s 0xff -n 32 /bin/ls

###5. Uninstall
    # make uninstall

###6. Get involved
SF project page & downloads: https://sourceforge.net/projects/hdump/
Source code: https://github.com/merces/hdump
