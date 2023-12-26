PROJECT FILES AND DESCRIPTIONS
Makefile: Used to compile all programs and create executable files.

findtopk.c: Program that finds the k largest numbers using Fork.

findtopk_mqueue.c: Program that finds the k largest numbers using POSIX message queues.

findtopk_thread.c: Program that finds the k largest numbers using POSIX threads.

report.pdf: Report file containing experimental results.

=============================================================
COMPILING AND RUNNING THE PROJECT
To compile and run the project, use the following commands in the terminal:

make: Compiles all programs and creates executable files.

./findtopk <k> <N> <infile1> ...<infileN> <outfile>: Runs the program using Fork.

./findtopk_mqueue <k> <N> <infile1> ...<infileN> <outfile>: Runs the program using POSIX message queues.

./findtopk_thread <k> <N> <infile1> ...<infileN> <outfile>: Runs the program using POSIX threads.

=============================================================
NOTES
You must have a compiler that supports the C language for compilation and execution processes.

For detailed information and results, you can refer to the report.pdf file.
