all: sw.c csc.c index.c
	gcc -Wall sw.c -o ./bin/swexec
	gcc -Wall csc.c -o ./bin/cscexec
	gcc -Wall index.c -o ./bin/index