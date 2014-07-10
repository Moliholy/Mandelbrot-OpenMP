AUTHOR: José Molina Colmenero
Parallel Programming for Multicore-based Systems
Assignment 2 - OpenMP





INSTRUCTIONS TO COMPILE AND EXECUTE
===================================

This assignment has been written in C++ using the GNU C++ compiler. In order to compile it just open a system terminal and type the following:

1) Navigate into the Debug folder of the exercise:
2) Type:
	$ make clean
	$ make
	$./mandel-xxxxx <imageRows> <imageColumns> <pixelSize> <centerPointReal> <centerPointImaginary> <maximumIterations> [<minimumBoxSize>]

where xxxxx is "basic" or "magic", depending on the program.

Parameters must of course follow the rules of this assignment.

In each program's main folder I have also included a PPT file as an example of the output obtained when you execute the following command:

./mandel-xxxxx 1000 1000 0.001 -1.0 0.0 600 4


