# exprtree #

Reads a c-like expression from standard input and prints a visual depiction of
the syntax tree.


# How to run #
Bash:

	./exprtree <<<"6+7*8/2+*a"


Posix Shell:

	echo "6+7*8/2+*a" | ./exprtree


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
* terminfo/termcap (ncurses is just fine)


# Limitations #

* Does not semantically check left operand of assignment operator. Multiple assignment operators not bounded by parentheses will cause undefined behavior.
* Function calls are not valid operands. (only valid variable names and literals)
* Array dereferences are not valid operators. (only pointer dereferencing)
