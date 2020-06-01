# exprtree #

Reads a c-like expression from standard input and prints a visual depiction of
the syntax tree.


# How to run #
Bash:

	./exprtree <<<"(7+8)*9/2"


Posix Shell:

	echo "(7+8)*9/2" | ./exprtree


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


# Limitations #

* Does not semantically check left operand of assignment operator. Multiple assignment operators not bounded by parentheses will cause undefined behavior.
* function calls are not valid operands (only valid variable names and literals)
