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
* terminfo (ncurses is fine)


# Limitations #

* Does not semantically check left operand of assignment operator.
* Function calls are not valid operands. (only valid variable names and literals)
* Array dereferences are not valid operators. (only pointer dereferencing)
* No union or struct accessing
* No typecasting
* No comma operator
* No sizeof macro

