# exprtree #

Reads a c-like expression from standard input and prints a visual depiction of
the syntax tree.


# How to run #
Bash:

	./exprtree <<<"(7+8)*9"


Posix Shell:

	echo "(7+8)*9)" | ./exprtree


File:

	./exprtree < example.txt


# How to Build #

build commands:

	./autogen.sh
	./configure
	make


cleanup:

	make distclean


# Dependencies #
* gmp
* mpfr
* terminfo/termcap (ncurses is just fine)
