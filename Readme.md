# exprtree #

Reads a c-like expression from standard input and prints a visual depiction of
the syntax tree.


# How to Build #

### Build Commands ###

	./autogen.sh
	./configure
	make


### Cleanup ###

	make clean

or

	make distclean


# Dependencies #
* terminfo (ncurses is fine)


# How to run #
### Argument ###

	./exprtree -e '6+7*8/2+*a'


### File ###

	./exprtree -f example.txt


#### Output ####

	                                       (+)
	                                        |
	                    ----------------------------------------
	                    |                                      |
	                   (+)                                    (*)
	                    |                                      |
	          --------------------                            (a)
	          |                  |
	         (6)                (/)
	                             |
	                        ----------
	                        |        |
	                       (*)      (2)
	                        |
	                     ------
	                     |    |
	                    (7)  (8)


# Limitations #

* No checking of left operand of assignment operator.
* No checking of left operand of accessor operator
* No checking of operand of increment/decrement operator
* No checking of operand of reference operator
* No type checking of array subscript
* No function calls
* No typecasting
* No `sizeof` macro

