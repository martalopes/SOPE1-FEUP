all: sw.c csc.c index.c
	gcc -Wall sw.c -o swexec
	gcc -Wall csc.c -o cscexec
	gcc -Wall index.c -o index_exec